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
/** Complete original std_msgs/msg/Header; spellings are published in generation proof. */
export class Ros2Std_msgsMsgHeader {
static readonly maxWireBytes:i32=76;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=76;
/** Exact native field stamp, native scalar builtin_interfaces/msg/Time. */
stamp:Ros2Builtin_interfacesMsgTime=new Ros2Builtin_interfacesMsgTime();
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>76-n)return -90;n+=child;
if(n>76)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>64)return -22;if(4+size>76-n)return -90;n+=4+size;
if(n>76)return -90;
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
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros2Std_msgsMsgHeaderCodec implements VendorCodec<Ros2Std_msgsMsgHeader>{
static readonly codecIdentity:string="UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396";
static readonly maxWireBytes:i32=76;
encodeChecked(value:Ros2Std_msgsMsgHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_msgsMsgHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_msgsMsgHeader>{const out=new VendorDecoded<Ros2Std_msgsMsgHeader>();if(bytes.length>76){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_msgsMsgHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_msgsMsgHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_msgsMsgHeader():changetype<Ros2Std_msgsMsgHeader>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectoryPoint; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectoryPoint {
static readonly maxWireBytes:i32=408;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=408;
/** Exact native field positions, native scalar float64. */
positions:Array<f64>=new Array<f64>();
/** Exact native field velocities, native scalar float64. */
velocities:Array<f64>=new Array<f64>();
/** Exact native field accelerations, native scalar float64. */
accelerations:Array<f64>=new Array<f64>();
/** Exact native field effort, native scalar float64. */
effort:Array<f64>=new Array<f64>();
/** Exact native field time_from_start, native scalar builtin_interfaces/msg/Duration. */
time_from_start:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.positions.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.positions.length;++i){
if(!isFinite(this.positions[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.velocities.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.velocities.length;++i){
if(!isFinite(this.velocities[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.accelerations.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.accelerations.length;++i){
if(!isFinite(this.accelerations[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.effort.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
const child=this.time_from_start.wireSize();if(child<0)return child;if(child>408-n)return -90;n+=child;
if(n>408)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.positions.length);
for(let i=0;i<this.positions.length;++i){
w.integer(<u64>reinterpret<u64>(this.positions[i]),8);
}
w.u32(<u32>this.velocities.length);
for(let i=0;i<this.velocities.length;++i){
w.integer(<u64>reinterpret<u64>(this.velocities[i]),8);
}
w.u32(<u32>this.accelerations.length);
for(let i=0;i<this.accelerations.length;++i){
w.integer(<u64>reinterpret<u64>(this.accelerations[i]),8);
}
w.u32(<u32>this.effort.length);
for(let i=0;i<this.effort.length;++i){
w.integer(<u64>reinterpret<u64>(this.effort[i]),8);
}
this.time_from_start.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const value=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.positions=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.positions.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.velocities=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocities.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.accelerations=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.accelerations.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryPointCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5";
static readonly maxWireBytes:i32=408;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();if(bytes.length>408){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectoryPoint():changetype<Ros2Trajectory_msgsMsgJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectory; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectory {
static readonly maxWireBytes:i32=4164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4164;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
points:Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>4164-n)return -90;n+=child;
if(n>4164)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>4164-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>4164-n)return -90;n+=4+size;
if(n>4164)return -90;
}
}
{
if(this.points.length>8)return -22;
if(4>4164-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>4164-n)return -90;n+=child;
if(n>4164)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectory{const value=new Ros2Trajectory_msgsMsgJointTrajectory();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>8){r.valid=false;return value;}
value.points=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0";
static readonly maxWireBytes:i32=4164;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>();if(bytes.length>4164){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectory():changetype<Ros2Trajectory_msgsMsgJointTrajectory>(out.value);}
}
/** Complete original geometry_msgs/msg/Vector3; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgVector3 {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float64. */
x:f64=0;
/** Exact native field y, native scalar float64. */
y:f64=0;
/** Exact native field z, native scalar float64. */
z:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.x),8);
w.integer(<u64>reinterpret<u64>(this.y),8);
w.integer(<u64>reinterpret<u64>(this.z),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgVector3{const value=new Ros2Geometry_msgsMsgVector3();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgVector3Codec implements VendorCodec<Ros2Geometry_msgsMsgVector3>{
static readonly codecIdentity:string="UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Geometry_msgsMsgVector3):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgVector3):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgVector3>{const out=new VendorDecoded<Ros2Geometry_msgsMsgVector3>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgVector3.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgVector3{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgVector3():changetype<Ros2Geometry_msgsMsgVector3>(out.value);}
}
/** Complete original geometry_msgs/msg/Quaternion; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgQuaternion {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field x, native scalar float64. */
x:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field y, native scalar float64. */
y:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field z, native scalar float64. */
z:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field w, native scalar float64. */
w:f64=reinterpret<f64>(u64(0x3ff0000000000000));
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.y))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.z))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.w))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.x),8);
w.integer(<u64>reinterpret<u64>(this.y),8);
w.integer(<u64>reinterpret<u64>(this.z),8);
w.integer(<u64>reinterpret<u64>(this.w),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgQuaternion{const value=new Ros2Geometry_msgsMsgQuaternion();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgQuaternionCodec implements VendorCodec<Ros2Geometry_msgsMsgQuaternion>{
static readonly codecIdentity:string="UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Geometry_msgsMsgQuaternion):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgQuaternion):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgQuaternion>{const out=new VendorDecoded<Ros2Geometry_msgsMsgQuaternion>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgQuaternion.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgQuaternion{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgQuaternion():changetype<Ros2Geometry_msgsMsgQuaternion>(out.value);}
}
/** Complete original geometry_msgs/msg/Transform; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTransform {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field translation, native scalar geometry_msgs/msg/Vector3. */
translation:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field rotation, native scalar geometry_msgs/msg/Quaternion. */
rotation:Ros2Geometry_msgsMsgQuaternion=new Ros2Geometry_msgsMsgQuaternion();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.translation.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
{
const child=this.rotation.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.translation.writeControl(w);
this.rotation.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTransform{const value=new Ros2Geometry_msgsMsgTransform();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.translation=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternion.readControl(r);
value.rotation=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTransformCodec implements VendorCodec<Ros2Geometry_msgsMsgTransform>{
static readonly codecIdentity:string="UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros2Geometry_msgsMsgTransform):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTransform):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTransform>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTransform>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTransform.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTransform{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTransform():changetype<Ros2Geometry_msgsMsgTransform>(out.value);}
}
/** Complete original geometry_msgs/msg/Twist; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTwist {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field linear, native scalar geometry_msgs/msg/Vector3. */
linear:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field angular, native scalar geometry_msgs/msg/Vector3. */
angular:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.linear.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
{
const child=this.angular.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.linear.writeControl(w);
this.angular.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTwist{const value=new Ros2Geometry_msgsMsgTwist();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.linear=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.angular=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTwistCodec implements VendorCodec<Ros2Geometry_msgsMsgTwist>{
static readonly codecIdentity:string="UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Geometry_msgsMsgTwist):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTwist):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTwist>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTwist>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTwist.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTwist{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTwist():changetype<Ros2Geometry_msgsMsgTwist>(out.value);}
}
/** Complete original trajectory_msgs/msg/MultiDOFJointTrajectoryPoint; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {
static readonly maxWireBytes:i32=324;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=324;
/** Exact native field transforms, native scalar geometry_msgs/msg/Transform. */
transforms:Array<Ros2Geometry_msgsMsgTransform>=new Array<Ros2Geometry_msgsMsgTransform>();
/** Exact native field velocities, native scalar geometry_msgs/msg/Twist. */
velocities:Array<Ros2Geometry_msgsMsgTwist>=new Array<Ros2Geometry_msgsMsgTwist>();
/** Exact native field accelerations, native scalar geometry_msgs/msg/Twist. */
accelerations:Array<Ros2Geometry_msgsMsgTwist>=new Array<Ros2Geometry_msgsMsgTwist>();
/** Exact native field time_from_start, native scalar builtin_interfaces/msg/Duration. */
time_from_start:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.transforms.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.transforms.length;++i){
const child=this.transforms[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
if(this.velocities.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.velocities.length;++i){
const child=this.velocities[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
if(this.accelerations.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.accelerations.length;++i){
const child=this.accelerations[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
const child=this.time_from_start.wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.transforms.length);
for(let i=0;i<this.transforms.length;++i){
this.transforms[i].writeControl(w);
}
w.u32(<u32>this.velocities.length);
for(let i=0;i<this.velocities.length;++i){
this.velocities[i].writeControl(w);
}
w.u32(<u32>this.accelerations.length);
for(let i=0;i<this.accelerations.length;++i){
this.accelerations[i].writeControl(w);
}
this.time_from_start.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint{const value=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.transforms=new Array<Ros2Geometry_msgsMsgTransform>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTransform.readControl(r);
value.transforms.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.velocities=new Array<Ros2Geometry_msgsMsgTwist>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.velocities.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.accelerations=new Array<Ros2Geometry_msgsMsgTwist>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.accelerations.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec implements VendorCodec<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242";
static readonly maxWireBytes:i32=324;
encodeChecked(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();if(bytes.length>324){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint():changetype<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/msg/MultiDOFJointTrajectory; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {
static readonly maxWireBytes:i32=1516;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1516;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
points:Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>=new Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1516-n)return -90;n+=child;
if(n>1516)return -90;
}
{
if(this.joint_names.length>2)return -22;
if(4>1516-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>1516-n)return -90;n+=4+size;
if(n>1516)return -90;
}
}
{
if(this.points.length>4)return -22;
if(4>1516-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>1516-n)return -90;n+=child;
if(n>1516)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgMultiDOFJointTrajectory{const value=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.points=new Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCodec implements VendorCodec<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401";
static readonly maxWireBytes:i32=1516;
encodeChecked(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>();if(bytes.length>1516){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgMultiDOFJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory():changetype<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>(out.value);}
}
/** Complete original control_msgs/msg/JointTolerance; spellings are published in generation proof. */
export class Ros2Control_msgsMsgJointTolerance {
static readonly maxWireBytes:i32=92;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=92;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>64)return -22;if(4+size>92-n)return -90;n+=4+size;
if(n>92)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsMsgJointTolerance{const value=new Ros2Control_msgsMsgJointTolerance();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.acceleration=decoded;
}
return value;}
}
export class Ros2Control_msgsMsgJointToleranceCodec implements VendorCodec<Ros2Control_msgsMsgJointTolerance>{
static readonly codecIdentity:string="UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2";
static readonly maxWireBytes:i32=92;
encodeChecked(value:Ros2Control_msgsMsgJointTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsMsgJointTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsMsgJointTolerance>{const out=new VendorDecoded<Ros2Control_msgsMsgJointTolerance>();if(bytes.length>92){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsMsgJointTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsMsgJointTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsMsgJointTolerance():changetype<Ros2Control_msgsMsgJointTolerance>(out.value);}
}
/** Complete original control_msgs/msg/JointComponentTolerance; spellings are published in generation proof. */
export class Ros2Control_msgsMsgJointComponentTolerance {
static readonly X_AXIS:u16=1;
static readonly Y_AXIS:u16=2;
static readonly Z_AXIS:u16=3;
static readonly TRANSLATION:u16=4;
static readonly ROTATION:u16=5;
static readonly maxWireBytes:i32=542;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=542;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field component, native scalar uint16. */
component:u16=0;
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>512)return -22;if(4+size>542-n)return -90;n+=4+size;
if(n>542)return -90;
}
{
if(2>542-n)return -90;n+=2;
if(n>542)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.joint_name);
w.integer(<u64>this.component,2);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsMsgJointComponentTolerance{const value=new Ros2Control_msgsMsgJointComponentTolerance();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const decoded=<u16>r.integer(2);
value.component=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.acceleration=decoded;
}
return value;}
}
export class Ros2Control_msgsMsgJointComponentToleranceCodec implements VendorCodec<Ros2Control_msgsMsgJointComponentTolerance>{
static readonly codecIdentity:string="UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342";
static readonly maxWireBytes:i32=542;
encodeChecked(value:Ros2Control_msgsMsgJointComponentTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsMsgJointComponentTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsMsgJointComponentTolerance>{const out=new VendorDecoded<Ros2Control_msgsMsgJointComponentTolerance>();if(bytes.length>542){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsMsgJointComponentTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsMsgJointComponentTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsMsgJointComponentTolerance():changetype<Ros2Control_msgsMsgJointComponentTolerance>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Goal; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Goal {
static readonly maxWireBytes:i32=20920;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20920;
/** Exact native field trajectory, native scalar trajectory_msgs/msg/JointTrajectory. */
trajectory:Ros2Trajectory_msgsMsgJointTrajectory=new Ros2Trajectory_msgsMsgJointTrajectory();
/** Exact native field multi_dof_trajectory, native scalar trajectory_msgs/msg/MultiDOFJointTrajectory. */
multi_dof_trajectory:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory();
/** Exact native field path_tolerance, native scalar control_msgs/msg/JointTolerance. */
path_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field component_path_tolerance, native scalar control_msgs/msg/JointComponentTolerance. */
component_path_tolerance:Array<Ros2Control_msgsMsgJointComponentTolerance>=new Array<Ros2Control_msgsMsgJointComponentTolerance>();
/** Exact native field goal_tolerance, native scalar control_msgs/msg/JointTolerance. */
goal_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field component_goal_tolerance, native scalar control_msgs/msg/JointComponentTolerance. */
component_goal_tolerance:Array<Ros2Control_msgsMsgJointComponentTolerance>=new Array<Ros2Control_msgsMsgJointComponentTolerance>();
/** Exact native field goal_time_tolerance, native scalar builtin_interfaces/msg/Duration. */
goal_time_tolerance:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trajectory.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
{
const child=this.multi_dof_trajectory.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
{
if(this.path_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.path_tolerance.length;++i){
const child=this.path_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.component_path_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.component_path_tolerance.length;++i){
const child=this.component_path_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.goal_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.goal_tolerance.length;++i){
const child=this.goal_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.component_goal_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.component_goal_tolerance.length;++i){
const child=this.component_goal_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
const child=this.goal_time_tolerance.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trajectory.writeControl(w);
this.multi_dof_trajectory.writeControl(w);
w.u32(<u32>this.path_tolerance.length);
for(let i=0;i<this.path_tolerance.length;++i){
this.path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.component_path_tolerance.length);
for(let i=0;i<this.component_path_tolerance.length;++i){
this.component_path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.goal_tolerance.length);
for(let i=0;i<this.goal_tolerance.length;++i){
this.goal_tolerance[i].writeControl(w);
}
w.u32(<u32>this.component_goal_tolerance.length);
for(let i=0;i<this.component_goal_tolerance.length;++i){
this.component_goal_tolerance[i].writeControl(w);
}
this.goal_time_tolerance.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Goal{const value=new Ros2Control_msgsActionFollowJointTrajectory_Goal();
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectory.readControl(r);
value.trajectory=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectory.readControl(r);
value.multi_dof_trajectory=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.path_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.component_path_tolerance=new Array<Ros2Control_msgsMsgJointComponentTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentTolerance.readControl(r);
value.component_path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.goal_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.goal_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.component_goal_tolerance=new Array<Ros2Control_msgsMsgJointComponentTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentTolerance.readControl(r);
value.component_goal_tolerance.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.goal_time_tolerance=decoded;
}
return value;}
}
export class Ros2Control_msgsActionFollowJointTrajectory_GoalCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Goal>{
static readonly codecIdentity:string="UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16";
static readonly maxWireBytes:i32=20920;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Goal>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Goal>();if(bytes.length>20920){out.status=90;return out;}if(bytes.length<64){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Goal():changetype<Ros2Control_msgsActionFollowJointTrajectory_Goal>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Result; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Result {
static readonly SUCCESSFUL:i32=0;
static readonly INVALID_GOAL:i32=-1;
static readonly INVALID_JOINTS:i32=-2;
static readonly OLD_HEADER_TIMESTAMP:i32=-3;
static readonly PATH_TOLERANCE_VIOLATED:i32=-4;
static readonly GOAL_TOLERANCE_VIOLATED:i32=-5;
static readonly maxWireBytes:i32=4104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4104;
/** Exact native field error_code, native scalar int32. */
error_code:i32=0;
/** Exact native field error_string, native scalar string. */
error_string:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4104-n)return -90;n+=4;
if(n>4104)return -90;
}
{
if(!validUnicode(this.error_string)||this.error_string.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_string);if(size>4096)return -22;if(4+size>4104-n)return -90;n+=4+size;
if(n>4104)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.error_code,4);
w.text(this.error_string);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Result{const value=new Ros2Control_msgsActionFollowJointTrajectory_Result();
{
const decoded=<i32>r.integer(4);
value.error_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_string=decoded;
}
return value;}
}
export class Ros2Control_msgsActionFollowJointTrajectory_ResultCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Result>{
static readonly codecIdentity:string="UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70";
static readonly maxWireBytes:i32=4104;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Result>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Result>();if(bytes.length>4104){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Result():changetype<Ros2Control_msgsActionFollowJointTrajectory_Result>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Feedback; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Feedback {
static readonly maxWireBytes:i32=4128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4128;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field desired, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
desired:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field actual, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
actual:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field error, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
error:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field multi_dof_joint_names, native scalar string. */
multi_dof_joint_names:Array<string>=new Array<string>();
/** Exact native field multi_dof_desired, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_desired:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact native field multi_dof_actual, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_actual:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact native field multi_dof_error, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_error:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>4128-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>4128-n)return -90;n+=4+size;
if(n>4128)return -90;
}
}
{
const child=this.desired.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.actual.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.error.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
if(this.multi_dof_joint_names.length>2)return -22;
if(4>4128-n)return -90;n+=4;
for(let i=0;i<this.multi_dof_joint_names.length;++i){
if(!validUnicode(this.multi_dof_joint_names[i])||this.multi_dof_joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.multi_dof_joint_names[i]);if(size>512)return -22;if(4+size>4128-n)return -90;n+=4+size;
if(n>4128)return -90;
}
}
{
const child=this.multi_dof_desired.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.multi_dof_actual.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.multi_dof_error.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
this.desired.writeControl(w);
this.actual.writeControl(w);
this.error.writeControl(w);
w.u32(<u32>this.multi_dof_joint_names.length);
for(let i=0;i<this.multi_dof_joint_names.length;++i){
w.text(this.multi_dof_joint_names[i]);
}
this.multi_dof_desired.writeControl(w);
this.multi_dof_actual.writeControl(w);
this.multi_dof_error.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Feedback{const value=new Ros2Control_msgsActionFollowJointTrajectory_Feedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.desired=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.actual=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.error=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.multi_dof_joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.multi_dof_joint_names.push(decoded);
}
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_desired=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_actual=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_error=decoded;
}
return value;}
}
export class Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Feedback>{
static readonly codecIdentity:string="UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb";
static readonly maxWireBytes:i32=4128;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Feedback>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Feedback>();if(bytes.length>4128){out.status=90;return out;}if(bytes.length<152){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Feedback():changetype<Ros2Control_msgsActionFollowJointTrajectory_Feedback>(out.value);}
}
/** Complete original fanuc_msgs/msg/IOType; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgIOType {
static readonly AI:string="AI";
static readonly AO:string="AO";
static readonly DI:string="DI";
static readonly DO:string="DO";
static readonly RI:string="RI";
static readonly RO:string="RO";
static readonly SI:string="SI";
static readonly SO:string="SO";
static readonly WI:string="WI";
static readonly WO:string="WO";
static readonly UI:string="UI";
static readonly UO:string="UO";
static readonly WSI:string="WSI";
static readonly WSO:string="WSO";
static readonly F:string="F";
static readonly M:string="M";
static readonly GI:string="GI";
static readonly GO:string="GO";
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field type, native scalar string. */
ros_type:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgIOType{const value=new Ros2Fanuc_msgsMsgIOType();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgIOTypeCodec implements VendorCodec<Ros2Fanuc_msgsMsgIOType>{
static readonly codecIdentity:string="UWVMROS1_1815aa6b1bcab1efe21de0e31570ca650553b5c70f0793301f82c0ca7934c586";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Fanuc_msgsMsgIOType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgIOType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgIOType>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgIOType>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgIOType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgIOType{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgIOType():changetype<Ros2Fanuc_msgsMsgIOType>(out.value);}
}
/** Complete original fanuc_msgs/msg/AnalogIO; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgAnalogIO {
static readonly maxWireBytes:i32=74;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=74;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>74-n)return -90;n+=child;
if(n>74)return -90;
}
{
if(2>74-n)return -90;n+=2;
if(n>74)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>74-n)return -90;n+=4;
if(n>74)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgAnalogIO{const value=new Ros2Fanuc_msgsMsgAnalogIO();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgAnalogIOCodec implements VendorCodec<Ros2Fanuc_msgsMsgAnalogIO>{
static readonly codecIdentity:string="UWVMROS1_d62097527ba99bf6e803283350161cd1415173a22eb3cb20331a232366076d00";
static readonly maxWireBytes:i32=74;
encodeChecked(value:Ros2Fanuc_msgsMsgAnalogIO):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgAnalogIO):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgAnalogIO>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgAnalogIO>();if(bytes.length>74){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgAnalogIO.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgAnalogIO{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgAnalogIO():changetype<Ros2Fanuc_msgsMsgAnalogIO>(out.value);}
}
/** Complete original fanuc_msgs/msg/AnalogIOCmd; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgAnalogIOCmd {
static readonly maxWireBytes:i32=2372;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2372;
/** Exact native field values, native scalar fanuc_msgs/msg/AnalogIO. */
values:Array<Ros2Fanuc_msgsMsgAnalogIO>=new Array<Ros2Fanuc_msgsMsgAnalogIO>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>2372-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>2372-n)return -90;n+=child;
if(n>2372)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgAnalogIOCmd{const value=new Ros2Fanuc_msgsMsgAnalogIOCmd();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgAnalogIO>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgAnalogIO.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgAnalogIOCmdCodec implements VendorCodec<Ros2Fanuc_msgsMsgAnalogIOCmd>{
static readonly codecIdentity:string="UWVMROS1_62f38a088ed23f8daf7163f26514ae1aabd7b1b64845ccdbc281cb7839d857ba";
static readonly maxWireBytes:i32=2372;
encodeChecked(value:Ros2Fanuc_msgsMsgAnalogIOCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgAnalogIOCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgAnalogIOCmd>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgAnalogIOCmd>();if(bytes.length>2372){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgAnalogIOCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgAnalogIOCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgAnalogIOCmd():changetype<Ros2Fanuc_msgsMsgAnalogIOCmd>(out.value);}
}
/** Complete original fanuc_msgs/msg/AnalogIOState; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgAnalogIOState {
static readonly maxWireBytes:i32=2372;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2372;
/** Exact native field values, native scalar fanuc_msgs/msg/AnalogIO. */
values:Array<Ros2Fanuc_msgsMsgAnalogIO>=new Array<Ros2Fanuc_msgsMsgAnalogIO>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>2372-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>2372-n)return -90;n+=child;
if(n>2372)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgAnalogIOState{const value=new Ros2Fanuc_msgsMsgAnalogIOState();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgAnalogIO>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgAnalogIO.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgAnalogIOStateCodec implements VendorCodec<Ros2Fanuc_msgsMsgAnalogIOState>{
static readonly codecIdentity:string="UWVMROS1_820b41fecbf6db8e2b2e67d60cd1167cf474b22ca6c489f4d2d64dd33eaac9a4";
static readonly maxWireBytes:i32=2372;
encodeChecked(value:Ros2Fanuc_msgsMsgAnalogIOState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgAnalogIOState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgAnalogIOState>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgAnalogIOState>();if(bytes.length>2372){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgAnalogIOState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgAnalogIOState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgAnalogIOState():changetype<Ros2Fanuc_msgsMsgAnalogIOState>(out.value);}
}
/** Complete original fanuc_msgs/msg/BoolIO; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgBoolIO {
static readonly maxWireBytes:i32=71;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=71;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar bool. */
value:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>71-n)return -90;n+=child;
if(n>71)return -90;
}
{
if(2>71-n)return -90;n+=2;
if(n>71)return -90;
}
{
if(1>71-n)return -90;n+=1;
if(n>71)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
w.u8(this.value?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgBoolIO{const value=new Ros2Fanuc_msgsMsgBoolIO();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgBoolIOCodec implements VendorCodec<Ros2Fanuc_msgsMsgBoolIO>{
static readonly codecIdentity:string="UWVMROS1_9209ae1b8f5407a31beface0a3286f58bd823aabdc560c2a4bf4a9b1ca125aa7";
static readonly maxWireBytes:i32=71;
encodeChecked(value:Ros2Fanuc_msgsMsgBoolIO):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgBoolIO):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgBoolIO>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgBoolIO>();if(bytes.length>71){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgBoolIO.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgBoolIO{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgBoolIO():changetype<Ros2Fanuc_msgsMsgBoolIO>(out.value);}
}
/** Complete original fanuc_msgs/msg/CollaborativeSpeedScaling; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgCollaborativeSpeedScaling {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field collaborative_speed_scaling, native scalar float64. */
collaborative_speed_scaling:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.collaborative_speed_scaling))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.collaborative_speed_scaling),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgCollaborativeSpeedScaling{const value=new Ros2Fanuc_msgsMsgCollaborativeSpeedScaling();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.collaborative_speed_scaling=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgCollaborativeSpeedScalingCodec implements VendorCodec<Ros2Fanuc_msgsMsgCollaborativeSpeedScaling>{
static readonly codecIdentity:string="UWVMROS1_693a3b581527d2eaf0f697f31e87ca2da428d1f51b821d42d6b06af387a2fb45";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Fanuc_msgsMsgCollaborativeSpeedScaling):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgCollaborativeSpeedScaling):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgCollaborativeSpeedScaling>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgCollaborativeSpeedScaling>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgCollaborativeSpeedScaling.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgCollaborativeSpeedScaling{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgCollaborativeSpeedScaling():changetype<Ros2Fanuc_msgsMsgCollaborativeSpeedScaling>(out.value);}
}
/** Complete original fanuc_msgs/msg/ConnectionStatus; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgConnectionStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field is_connected, native scalar bool. */
is_connected:bool=false;
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
w.u8(this.is_connected?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgConnectionStatus{const value=new Ros2Fanuc_msgsMsgConnectionStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_connected=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgConnectionStatusCodec implements VendorCodec<Ros2Fanuc_msgsMsgConnectionStatus>{
static readonly codecIdentity:string="UWVMROS1_8ce24fb87538922b1f3c0ae856eb223d716143064eb46a6cfce5fec01b45b078";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsMsgConnectionStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgConnectionStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgConnectionStatus>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgConnectionStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgConnectionStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgConnectionStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgConnectionStatus():changetype<Ros2Fanuc_msgsMsgConnectionStatus>(out.value);}
}
/** Complete original fanuc_msgs/msg/ForceSensor; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgForceSensor {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field force_x, native scalar float32. */
force_x:f32=0;
/** Exact native field force_y, native scalar float32. */
force_y:f32=0;
/** Exact native field force_z, native scalar float32. */
force_z:f32=0;
/** Exact native field moment_x, native scalar float32. */
moment_x:f32=0;
/** Exact native field moment_y, native scalar float32. */
moment_y:f32=0;
/** Exact native field moment_z, native scalar float32. */
moment_z:f32=0;
/** Exact native field fs_type, native scalar uint32. */
fs_type:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.force_x))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.force_y))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.force_z))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.moment_x))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.moment_y))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.moment_z))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.force_x),4);
w.integer(<u64>reinterpret<u32>(this.force_y),4);
w.integer(<u64>reinterpret<u32>(this.force_z),4);
w.integer(<u64>reinterpret<u32>(this.moment_x),4);
w.integer(<u64>reinterpret<u32>(this.moment_y),4);
w.integer(<u64>reinterpret<u32>(this.moment_z),4);
w.integer(<u64>this.fs_type,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgForceSensor{const value=new Ros2Fanuc_msgsMsgForceSensor();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.force_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.force_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.force_z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.moment_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.moment_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.moment_z=decoded;
}
{
const decoded=<u32>r.integer(4);
value.fs_type=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgForceSensorCodec implements VendorCodec<Ros2Fanuc_msgsMsgForceSensor>{
static readonly codecIdentity:string="UWVMROS1_06c99b43d7347646179850f3ba79192da7ce71296667d93d88659efbc1004032";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros2Fanuc_msgsMsgForceSensor):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgForceSensor):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgForceSensor>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgForceSensor>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgForceSensor.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgForceSensor{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgForceSensor():changetype<Ros2Fanuc_msgsMsgForceSensor>(out.value);}
}
/** Complete original fanuc_msgs/msg/Frame; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgFrame {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field w, native scalar float32. */
w:f32=0;
/** Exact native field p, native scalar float32. */
p:f32=0;
/** Exact native field r, native scalar float32. */
r:f32=0;
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
if(!isFinite(this.w))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.p))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.r))return -22;
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
w.integer(<u64>reinterpret<u32>(this.w),4);
w.integer(<u64>reinterpret<u32>(this.p),4);
w.integer(<u64>reinterpret<u32>(this.r),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgFrame{const value=new Ros2Fanuc_msgsMsgFrame();
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
value.w=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.p=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.r=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgFrameCodec implements VendorCodec<Ros2Fanuc_msgsMsgFrame>{
static readonly codecIdentity:string="UWVMROS1_609d1b7e49315f7f0a0fe86fa997673acd0d009e57b563690f36624b59b9effb";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Fanuc_msgsMsgFrame):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgFrame):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgFrame>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgFrame>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgFrame.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgFrame{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgFrame():changetype<Ros2Fanuc_msgsMsgFrame>(out.value);}
}
/** Complete original fanuc_msgs/msg/IOCmd; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgIOCmd {
static readonly maxWireBytes:i32=2276;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2276;
/** Exact native field values, native scalar fanuc_msgs/msg/BoolIO. */
values:Array<Ros2Fanuc_msgsMsgBoolIO>=new Array<Ros2Fanuc_msgsMsgBoolIO>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>2276-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>2276-n)return -90;n+=child;
if(n>2276)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgIOCmd{const value=new Ros2Fanuc_msgsMsgIOCmd();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgBoolIO>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgBoolIO.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgIOCmdCodec implements VendorCodec<Ros2Fanuc_msgsMsgIOCmd>{
static readonly codecIdentity:string="UWVMROS1_462552bbaa3c9ed4caaf3108960d514e4ab61fcf2d6a84f0465b485bec1de45d";
static readonly maxWireBytes:i32=2276;
encodeChecked(value:Ros2Fanuc_msgsMsgIOCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgIOCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgIOCmd>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgIOCmd>();if(bytes.length>2276){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgIOCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgIOCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgIOCmd():changetype<Ros2Fanuc_msgsMsgIOCmd>(out.value);}
}
/** Complete original fanuc_msgs/msg/IOState; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgIOState {
static readonly maxWireBytes:i32=2276;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2276;
/** Exact native field values, native scalar fanuc_msgs/msg/BoolIO. */
values:Array<Ros2Fanuc_msgsMsgBoolIO>=new Array<Ros2Fanuc_msgsMsgBoolIO>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>2276-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>2276-n)return -90;n+=child;
if(n>2276)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgIOState{const value=new Ros2Fanuc_msgsMsgIOState();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgBoolIO>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgBoolIO.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgIOStateCodec implements VendorCodec<Ros2Fanuc_msgsMsgIOState>{
static readonly codecIdentity:string="UWVMROS1_332533445014c03d626a2c4dbdffd9421748aa6b73f49d407441fd6b9f2b24d8";
static readonly maxWireBytes:i32=2276;
encodeChecked(value:Ros2Fanuc_msgsMsgIOState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgIOState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgIOState>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgIOState>();if(bytes.length>2276){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgIOState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgIOState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgIOState():changetype<Ros2Fanuc_msgsMsgIOState>(out.value);}
}
/** Complete original fanuc_msgs/msg/NumReg; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgNumReg {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>6-n)return -90;n+=2;
if(n>6)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.index,2);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgNumReg{const value=new Ros2Fanuc_msgsMsgNumReg();
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgNumRegCodec implements VendorCodec<Ros2Fanuc_msgsMsgNumReg>{
static readonly codecIdentity:string="UWVMROS1_0fb9344c5768ea52ab072939f368f1e73dcc0ff83e74b4cb6d35fca47c1d29cc";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Fanuc_msgsMsgNumReg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgNumReg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgNumReg>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgNumReg>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgNumReg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgNumReg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgNumReg():changetype<Ros2Fanuc_msgsMsgNumReg>(out.value);}
}
/** Complete original fanuc_msgs/msg/NumRegCmd; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgNumRegCmd {
static readonly maxWireBytes:i32=196;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=196;
/** Exact native field values, native scalar fanuc_msgs/msg/NumReg. */
values:Array<Ros2Fanuc_msgsMsgNumReg>=new Array<Ros2Fanuc_msgsMsgNumReg>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>196-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>196-n)return -90;n+=child;
if(n>196)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgNumRegCmd{const value=new Ros2Fanuc_msgsMsgNumRegCmd();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgNumReg>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgNumReg.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgNumRegCmdCodec implements VendorCodec<Ros2Fanuc_msgsMsgNumRegCmd>{
static readonly codecIdentity:string="UWVMROS1_9c76500073889377525a0802c46bd96589a2639dd9534cf7ad2b9956cd95b5b6";
static readonly maxWireBytes:i32=196;
encodeChecked(value:Ros2Fanuc_msgsMsgNumRegCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgNumRegCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgNumRegCmd>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgNumRegCmd>();if(bytes.length>196){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgNumRegCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgNumRegCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgNumRegCmd():changetype<Ros2Fanuc_msgsMsgNumRegCmd>(out.value);}
}
/** Complete original fanuc_msgs/msg/NumRegState; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgNumRegState {
static readonly maxWireBytes:i32=196;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=196;
/** Exact native field values, native scalar fanuc_msgs/msg/NumReg. */
values:Array<Ros2Fanuc_msgsMsgNumReg>=new Array<Ros2Fanuc_msgsMsgNumReg>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.values.length>32)return -22;
if(4>196-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
const child=this.values[i].wireSize();if(child<0)return child;if(child>196-n)return -90;n+=child;
if(n>196)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
this.values[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgNumRegState{const value=new Ros2Fanuc_msgsMsgNumRegState();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.values=new Array<Ros2Fanuc_msgsMsgNumReg>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgNumReg.readControl(r);
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsMsgNumRegStateCodec implements VendorCodec<Ros2Fanuc_msgsMsgNumRegState>{
static readonly codecIdentity:string="UWVMROS1_9c8ff4917a6dd033e6a6776ae8525e93c2924d776f05fd54e7e8d2251fc48b3a";
static readonly maxWireBytes:i32=196;
encodeChecked(value:Ros2Fanuc_msgsMsgNumRegState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgNumRegState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgNumRegState>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgNumRegState>();if(bytes.length>196){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgNumRegState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgNumRegState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgNumRegState():changetype<Ros2Fanuc_msgsMsgNumRegState>(out.value);}
}
/** Complete original fanuc_msgs/msg/RMIStatus; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgRMIStatus {
static readonly maxWireBytes:i32=1048;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1048;
/** Exact native field program_status, native scalar uint32. */
program_status:u32=0;
/** Exact native field next_sequence, native scalar uint32. */
next_sequence:u32=0;
/** Exact native field last_sequence, native scalar uint32. */
last_sequence:u32=0;
/** Exact native field last_sequence_error, native scalar int32. */
last_sequence_error:i32=0;
/** Exact native field last_sequence_instruction, native scalar string. */
last_sequence_instruction:string="";
/** Exact native field last_sequence_error_message, native scalar string. */
last_sequence_error_message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>1048-n)return -90;n+=4;
if(n>1048)return -90;
}
{
if(4>1048-n)return -90;n+=4;
if(n>1048)return -90;
}
{
if(4>1048-n)return -90;n+=4;
if(n>1048)return -90;
}
{
if(4>1048-n)return -90;n+=4;
if(n>1048)return -90;
}
{
if(!validUnicode(this.last_sequence_instruction)||this.last_sequence_instruction.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.last_sequence_instruction);if(size>512)return -22;if(4+size>1048-n)return -90;n+=4+size;
if(n>1048)return -90;
}
{
if(!validUnicode(this.last_sequence_error_message)||this.last_sequence_error_message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.last_sequence_error_message);if(size>512)return -22;if(4+size>1048-n)return -90;n+=4+size;
if(n>1048)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.program_status,4);
w.integer(<u64>this.next_sequence,4);
w.integer(<u64>this.last_sequence,4);
w.integer(<u64>this.last_sequence_error,4);
w.text(this.last_sequence_instruction);
w.text(this.last_sequence_error_message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgRMIStatus{const value=new Ros2Fanuc_msgsMsgRMIStatus();
{
const decoded=<u32>r.integer(4);
value.program_status=decoded;
}
{
const decoded=<u32>r.integer(4);
value.next_sequence=decoded;
}
{
const decoded=<u32>r.integer(4);
value.last_sequence=decoded;
}
{
const decoded=<i32>r.integer(4);
value.last_sequence_error=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.last_sequence_instruction=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.last_sequence_error_message=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgRMIStatusCodec implements VendorCodec<Ros2Fanuc_msgsMsgRMIStatus>{
static readonly codecIdentity:string="UWVMROS1_dc2e7eaabf44807142581905838f8951653682a6c6d61ebcf56494b16a10d5e8";
static readonly maxWireBytes:i32=1048;
encodeChecked(value:Ros2Fanuc_msgsMsgRMIStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgRMIStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgRMIStatus>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgRMIStatus>();if(bytes.length>1048){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgRMIStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgRMIStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgRMIStatus():changetype<Ros2Fanuc_msgsMsgRMIStatus>(out.value);}
}
/** Complete original fanuc_msgs/msg/RobotStatus; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgRobotStatus {
static readonly CONTACT_STOP_MODE_NONE:u8=0;
static readonly CONTACT_STOP_MODE_SAFE:u8=1;
static readonly CONTACT_STOP_MODE_STOP:u8=2;
static readonly CONTACT_STOP_MODE_DSBL:u8=3;
static readonly CONTACT_STOP_MODE_ESCP:u8=4;
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field in_error, native scalar bool. */
in_error:bool=false;
/** Exact native field tp_enabled, native scalar bool. */
tp_enabled:bool=false;
/** Exact native field e_stopped, native scalar bool. */
e_stopped:bool=false;
/** Exact native field motion_possible, native scalar bool. */
motion_possible:bool=false;
/** Exact native field contact_stop_mode, native scalar uint8. */
contact_stop_mode:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
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
w.u8(this.in_error?1:0);
w.u8(this.tp_enabled?1:0);
w.u8(this.e_stopped?1:0);
w.u8(this.motion_possible?1:0);
w.integer(<u64>this.contact_stop_mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgRobotStatus{const value=new Ros2Fanuc_msgsMsgRobotStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.in_error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.tp_enabled=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.e_stopped=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.motion_possible=decoded;
}
{
const decoded=<u8>r.integer(1);
value.contact_stop_mode=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgRobotStatusCodec implements VendorCodec<Ros2Fanuc_msgsMsgRobotStatus>{
static readonly codecIdentity:string="UWVMROS1_3f31ee3631dc6f4c90eb5ded6209ccebf0f4193ddfe5f5032bdd2bcd71f8cc31";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Fanuc_msgsMsgRobotStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgRobotStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgRobotStatus>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgRobotStatus>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgRobotStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgRobotStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgRobotStatus():changetype<Ros2Fanuc_msgsMsgRobotStatus>(out.value);}
}
/** Complete original fanuc_msgs/msg/RobotStatusExt; spellings are published in generation proof. */
export class Ros2Fanuc_msgsMsgRobotStatusExt {
static readonly maxWireBytes:i32=1042;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1042;
/** Exact native field error_code, native scalar string. */
error_code:string="";
/** Exact native field in_motion, native scalar bool. */
in_motion:bool=false;
/** Exact native field drives_powered, native scalar bool. */
drives_powered:bool=false;
/** Exact native field gen_override, native scalar int32. */
gen_override:i32=0;
/** Exact native field speed_clamp_limit, native scalar float32. */
speed_clamp_limit:f32=0;
/** Exact native field control_mode, native scalar string. */
control_mode:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.error_code)||this.error_code.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_code);if(size>512)return -22;if(4+size>1042-n)return -90;n+=4+size;
if(n>1042)return -90;
}
{
if(1>1042-n)return -90;n+=1;
if(n>1042)return -90;
}
{
if(1>1042-n)return -90;n+=1;
if(n>1042)return -90;
}
{
if(4>1042-n)return -90;n+=4;
if(n>1042)return -90;
}
{
if(!isFinite(this.speed_clamp_limit))return -22;
if(4>1042-n)return -90;n+=4;
if(n>1042)return -90;
}
{
if(!validUnicode(this.control_mode)||this.control_mode.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.control_mode);if(size>512)return -22;if(4+size>1042-n)return -90;n+=4+size;
if(n>1042)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.error_code);
w.u8(this.in_motion?1:0);
w.u8(this.drives_powered?1:0);
w.integer(<u64>this.gen_override,4);
w.integer(<u64>reinterpret<u32>(this.speed_clamp_limit),4);
w.text(this.control_mode);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsMsgRobotStatusExt{const value=new Ros2Fanuc_msgsMsgRobotStatusExt();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_code=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.in_motion=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.drives_powered=decoded;
}
{
const decoded=<i32>r.integer(4);
value.gen_override=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.speed_clamp_limit=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.control_mode=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsMsgRobotStatusExtCodec implements VendorCodec<Ros2Fanuc_msgsMsgRobotStatusExt>{
static readonly codecIdentity:string="UWVMROS1_8ba47a11bfbe02026fe79b32e1ed02e5f609499ea55bb57c9f09af9fba389648";
static readonly maxWireBytes:i32=1042;
encodeChecked(value:Ros2Fanuc_msgsMsgRobotStatusExt):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsMsgRobotStatusExt):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsMsgRobotStatusExt>{const out=new VendorDecoded<Ros2Fanuc_msgsMsgRobotStatusExt>();if(bytes.length>1042){out.status=90;return out;}if(bytes.length<18){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsMsgRobotStatusExt.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsMsgRobotStatusExt{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsMsgRobotStatusExt():changetype<Ros2Fanuc_msgsMsgRobotStatusExt>(out.value);}
}
/** Complete original fanuc_msgs/srv/CfgForceSensor_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvCfgForceSensor_Request {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field do_reset, native scalar uint32. */
do_reset:u32=0;
/** Exact native field fs_type, native scalar uint32. */
fs_type:u32=0;
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
w.integer(<u64>this.do_reset,4);
w.integer(<u64>this.fs_type,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvCfgForceSensor_Request{const value=new Ros2Fanuc_msgsSrvCfgForceSensor_Request();
{
const decoded=<u32>r.integer(4);
value.do_reset=decoded;
}
{
const decoded=<u32>r.integer(4);
value.fs_type=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvCfgForceSensor_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvCfgForceSensor_Request>{
static readonly codecIdentity:string="UWVMROS1_01973ed21546cb747ad2ce2a990a8ec4e70efb66e1c8fc8856cf1082870984ff";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Fanuc_msgsSrvCfgForceSensor_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvCfgForceSensor_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvCfgForceSensor_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvCfgForceSensor_Request>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvCfgForceSensor_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvCfgForceSensor_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvCfgForceSensor_Request():changetype<Ros2Fanuc_msgsSrvCfgForceSensor_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/CfgForceSensor_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvCfgForceSensor_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvCfgForceSensor_Response{const value=new Ros2Fanuc_msgsSrvCfgForceSensor_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvCfgForceSensor_Response>{
static readonly codecIdentity:string="UWVMROS1_41799ce44c937b1524889391db61e2b4d35795055cb4bbacfe5cc8bbd5b9a37b";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvCfgForceSensor_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvCfgForceSensor_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvCfgForceSensor_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvCfgForceSensor_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvCfgForceSensor_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvCfgForceSensor_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvCfgForceSensor_Response():changetype<Ros2Fanuc_msgsSrvCfgForceSensor_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetAnalogIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetAnalogIO_Request {
static readonly maxWireBytes:i32=70;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=70;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>70-n)return -90;n+=child;
if(n>70)return -90;
}
{
if(2>70-n)return -90;n+=2;
if(n>70)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetAnalogIO_Request{const value=new Ros2Fanuc_msgsSrvGetAnalogIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetAnalogIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetAnalogIO_Request>{
static readonly codecIdentity:string="UWVMROS1_cd567f97de2de581975c3d4f280eec545dea37c941d316fc3708bbc4d3af94d9";
static readonly maxWireBytes:i32=70;
encodeChecked(value:Ros2Fanuc_msgsSrvGetAnalogIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetAnalogIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetAnalogIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetAnalogIO_Request>();if(bytes.length>70){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetAnalogIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetAnalogIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetAnalogIO_Request():changetype<Ros2Fanuc_msgsSrvGetAnalogIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetAnalogIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetAnalogIO_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetAnalogIO_Response{const value=new Ros2Fanuc_msgsSrvGetAnalogIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetAnalogIO_Response>{
static readonly codecIdentity:string="UWVMROS1_3b53d3ce95d95454a782778c160fcc102ead869c5681c9c67224c373fef1b217";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Fanuc_msgsSrvGetAnalogIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetAnalogIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetAnalogIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetAnalogIO_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetAnalogIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetAnalogIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetAnalogIO_Response():changetype<Ros2Fanuc_msgsSrvGetAnalogIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetBoolIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetBoolIO_Request {
static readonly maxWireBytes:i32=70;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=70;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>70-n)return -90;n+=child;
if(n>70)return -90;
}
{
if(2>70-n)return -90;n+=2;
if(n>70)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetBoolIO_Request{const value=new Ros2Fanuc_msgsSrvGetBoolIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetBoolIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetBoolIO_Request>{
static readonly codecIdentity:string="UWVMROS1_ce8c4b1efd2828e92656e897baf041e8965367a236079231101807c898d4b50c";
static readonly maxWireBytes:i32=70;
encodeChecked(value:Ros2Fanuc_msgsSrvGetBoolIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetBoolIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetBoolIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetBoolIO_Request>();if(bytes.length>70){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetBoolIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetBoolIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetBoolIO_Request():changetype<Ros2Fanuc_msgsSrvGetBoolIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetBoolIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetBoolIO_Response {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field value, native scalar bool. */
value:bool=false;
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
w.integer(<u64>this.result,4);
w.u8(this.value?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetBoolIO_Response{const value=new Ros2Fanuc_msgsSrvGetBoolIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetBoolIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetBoolIO_Response>{
static readonly codecIdentity:string="UWVMROS1_7dbdcdde8d76c4ef422ea158551dd912273220c25a08f28fc8d19fdf8bc8a01c";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Fanuc_msgsSrvGetBoolIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetBoolIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetBoolIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetBoolIO_Response>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetBoolIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetBoolIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetBoolIO_Response():changetype<Ros2Fanuc_msgsSrvGetBoolIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetCartesianPosition_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetCartesianPosition_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetCartesianPosition_Request{const value=new Ros2Fanuc_msgsSrvGetCartesianPosition_Request();
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetCartesianPosition_Request>{
static readonly codecIdentity:string="UWVMROS1_a694a17bde88131efcadac4f134464a7777beab4b09e17624ae7b173ab62c3e4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetCartesianPosition_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetCartesianPosition_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetCartesianPosition_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetCartesianPosition_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetCartesianPosition_Request():changetype<Ros2Fanuc_msgsSrvGetCartesianPosition_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetCartesianPosition_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetCartesianPosition_Response {
static readonly maxWireBytes:i32=54;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=54;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field timetag, native scalar int32. */
timetag:i32=0;
/** Exact native field utool, native scalar uint8. */
utool:u8=0;
/** Exact native field uframe, native scalar uint8. */
uframe:u8=0;
/** Exact native field front, native scalar uint8. */
front:u8=0;
/** Exact native field up, native scalar uint8. */
up:u8=0;
/** Exact native field left, native scalar uint8. */
left:u8=0;
/** Exact native field flip, native scalar uint8. */
flip:u8=0;
/** Exact native field turn4, native scalar int8. */
turn4:i8=0;
/** Exact native field turn5, native scalar int8. */
turn5:i8=0;
/** Exact native field turn6, native scalar int8. */
turn6:i8=0;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field w, native scalar float32. */
w:f32=0;
/** Exact native field p, native scalar float32. */
p:f32=0;
/** Exact native field r, native scalar float32. */
r:f32=0;
/** Exact native field ext1, native scalar float32. */
ext1:f32=0;
/** Exact native field ext2, native scalar float32. */
ext2:f32=0;
/** Exact native field ext3, native scalar float32. */
ext3:f32=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
{
if(!isFinite(this.x))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.w))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.p))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.r))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.ext1))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.ext2))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(!isFinite(this.ext3))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
{
if(1>54-n)return -90;n+=1;
if(n>54)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.timetag,4);
w.integer(<u64>this.utool,1);
w.integer(<u64>this.uframe,1);
w.integer(<u64>this.front,1);
w.integer(<u64>this.up,1);
w.integer(<u64>this.left,1);
w.integer(<u64>this.flip,1);
w.integer(<u64>this.turn4,1);
w.integer(<u64>this.turn5,1);
w.integer(<u64>this.turn6,1);
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.w),4);
w.integer(<u64>reinterpret<u32>(this.p),4);
w.integer(<u64>reinterpret<u32>(this.r),4);
w.integer(<u64>reinterpret<u32>(this.ext1),4);
w.integer(<u64>reinterpret<u32>(this.ext2),4);
w.integer(<u64>reinterpret<u32>(this.ext3),4);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetCartesianPosition_Response{const value=new Ros2Fanuc_msgsSrvGetCartesianPosition_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.timetag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool=decoded;
}
{
const decoded=<u8>r.integer(1);
value.uframe=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front=decoded;
}
{
const decoded=<u8>r.integer(1);
value.up=decoded;
}
{
const decoded=<u8>r.integer(1);
value.left=decoded;
}
{
const decoded=<u8>r.integer(1);
value.flip=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn5=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn6=decoded;
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
value.z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.p=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.r=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext3=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetCartesianPosition_Response>{
static readonly codecIdentity:string="UWVMROS1_0516f0145717b06550674e02f0b9c47f9b312f4f361c791f56257fbb79e67329";
static readonly maxWireBytes:i32=54;
encodeChecked(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetCartesianPosition_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetCartesianPosition_Response>();if(bytes.length>54){out.status=90;return out;}if(bytes.length<54){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetCartesianPosition_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetCartesianPosition_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetCartesianPosition_Response():changetype<Ros2Fanuc_msgsSrvGetCartesianPosition_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetGroupIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetGroupIO_Request {
static readonly maxWireBytes:i32=70;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=70;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>70-n)return -90;n+=child;
if(n>70)return -90;
}
{
if(2>70-n)return -90;n+=2;
if(n>70)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetGroupIO_Request{const value=new Ros2Fanuc_msgsSrvGetGroupIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetGroupIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetGroupIO_Request>{
static readonly codecIdentity:string="UWVMROS1_dbd2e8e791e8debc745843827ed296b51bf84159f4c4fe5d912c231d621de216";
static readonly maxWireBytes:i32=70;
encodeChecked(value:Ros2Fanuc_msgsSrvGetGroupIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetGroupIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetGroupIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetGroupIO_Request>();if(bytes.length>70){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetGroupIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetGroupIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetGroupIO_Request():changetype<Ros2Fanuc_msgsSrvGetGroupIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetGroupIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetGroupIO_Response {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field value, native scalar uint16. */
value:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
{
if(2>6-n)return -90;n+=2;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetGroupIO_Response{const value=new Ros2Fanuc_msgsSrvGetGroupIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetGroupIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetGroupIO_Response>{
static readonly codecIdentity:string="UWVMROS1_9e131b91f36badc90ddae54a9855095241ab86c51b419b684d62879fae0bda7f";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Fanuc_msgsSrvGetGroupIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetGroupIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetGroupIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetGroupIO_Response>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetGroupIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetGroupIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetGroupIO_Response():changetype<Ros2Fanuc_msgsSrvGetGroupIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetNumReg_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetNumReg_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>2-n)return -90;n+=2;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.index,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetNumReg_Request{const value=new Ros2Fanuc_msgsSrvGetNumReg_Request();
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetNumReg_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetNumReg_Request>{
static readonly codecIdentity:string="UWVMROS1_727b5ff4ab9c8976280df12e80d3aee03b31288087adb94ee82161574fc8dcb5";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetNumReg_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetNumReg_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetNumReg_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetNumReg_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetNumReg_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetNumReg_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetNumReg_Request():changetype<Ros2Fanuc_msgsSrvGetNumReg_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetNumReg_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetNumReg_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetNumReg_Response{const value=new Ros2Fanuc_msgsSrvGetNumReg_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetNumReg_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetNumReg_Response>{
static readonly codecIdentity:string="UWVMROS1_78ae7ff82b3b2cd7d512fee8335f14a0759a71425fcf3d1b150dbe5e2a0179b3";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Fanuc_msgsSrvGetNumReg_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetNumReg_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetNumReg_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetNumReg_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetNumReg_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetNumReg_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetNumReg_Response():changetype<Ros2Fanuc_msgsSrvGetNumReg_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadComp_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadComp_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>2-n)return -90;n+=2;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadComp_Request{const value=new Ros2Fanuc_msgsSrvGetPayloadComp_Request();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadComp_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadComp_Request>{
static readonly codecIdentity:string="UWVMROS1_d48b88619f4a68ceeed02336e9ef078c27905707a21d977448fbbfbd560f398d";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadComp_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadComp_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadComp_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadComp_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadComp_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadComp_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadComp_Request():changetype<Ros2Fanuc_msgsSrvGetPayloadComp_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadComp_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadComp_Response {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cg_x, native scalar float32. */
cg_x:f32=0;
/** Exact native field cg_y, native scalar float32. */
cg_y:f32=0;
/** Exact native field cg_z, native scalar float32. */
cg_z:f32=0;
/** Exact native field in_x, native scalar float32. */
in_x:f32=0;
/** Exact native field in_y, native scalar float32. */
in_y:f32=0;
/** Exact native field in_z, native scalar float32. */
in_z:f32=0;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_x))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_y))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_z))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_x))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_y))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_z))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.mass),4);
w.integer(<u64>reinterpret<u32>(this.cg_x),4);
w.integer(<u64>reinterpret<u32>(this.cg_y),4);
w.integer(<u64>reinterpret<u32>(this.cg_z),4);
w.integer(<u64>reinterpret<u32>(this.in_x),4);
w.integer(<u64>reinterpret<u32>(this.in_y),4);
w.integer(<u64>reinterpret<u32>(this.in_z),4);
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadComp_Response{const value=new Ros2Fanuc_msgsSrvGetPayloadComp_Response();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_z=decoded;
}
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadComp_Response>{
static readonly codecIdentity:string="UWVMROS1_012b216c949f99755a36678b6be28d032f698b472ce4fa9ba975bc6d453ae4e4";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadComp_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadComp_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadComp_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadComp_Response>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadComp_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadComp_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadComp_Response():changetype<Ros2Fanuc_msgsSrvGetPayloadComp_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadID_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadID_Request {
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
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadID_Request{const value=new Ros2Fanuc_msgsSrvGetPayloadID_Request();
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadID_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadID_Request>{
static readonly codecIdentity:string="UWVMROS1_6c943659905c8adc7aa4b73832584994c83bfe009d18dc30a11d99b9fc0d6eef";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadID_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadID_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadID_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadID_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadID_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadID_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadID_Request():changetype<Ros2Fanuc_msgsSrvGetPayloadID_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadID_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadID_Response {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>6-n)return -90;n+=2;
if(n>6)return -90;
}
{
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadID_Response{const value=new Ros2Fanuc_msgsSrvGetPayloadID_Response();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadID_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadID_Response>{
static readonly codecIdentity:string="UWVMROS1_5551f4eab590c87fd7679d593c9c446903bdaa02aa1ed2512b146768122a950c";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadID_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadID_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadID_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadID_Response>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadID_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadID_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadID_Response():changetype<Ros2Fanuc_msgsSrvGetPayloadID_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadValue_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadValue_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>2-n)return -90;n+=2;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadValue_Request{const value=new Ros2Fanuc_msgsSrvGetPayloadValue_Request();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadValue_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadValue_Request>{
static readonly codecIdentity:string="UWVMROS1_d82667bd644873ae4fa5de0289ed94791eb2a7af49c1b8f86ed7f91cf875407a";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadValue_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadValue_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadValue_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadValue_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadValue_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadValue_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadValue_Request():changetype<Ros2Fanuc_msgsSrvGetPayloadValue_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPayloadValue_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPayloadValue_Response {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cg_x, native scalar float32. */
cg_x:f32=0;
/** Exact native field cg_y, native scalar float32. */
cg_y:f32=0;
/** Exact native field cg_z, native scalar float32. */
cg_z:f32=0;
/** Exact native field in_x, native scalar float32. */
in_x:f32=0;
/** Exact native field in_y, native scalar float32. */
in_y:f32=0;
/** Exact native field in_z, native scalar float32. */
in_z:f32=0;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_x))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_y))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.cg_z))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_x))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_y))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.in_z))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.mass),4);
w.integer(<u64>reinterpret<u32>(this.cg_x),4);
w.integer(<u64>reinterpret<u32>(this.cg_y),4);
w.integer(<u64>reinterpret<u32>(this.cg_z),4);
w.integer(<u64>reinterpret<u32>(this.in_x),4);
w.integer(<u64>reinterpret<u32>(this.in_y),4);
w.integer(<u64>reinterpret<u32>(this.in_z),4);
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPayloadValue_Response{const value=new Ros2Fanuc_msgsSrvGetPayloadValue_Response();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_z=decoded;
}
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPayloadValue_Response>{
static readonly codecIdentity:string="UWVMROS1_c3361ceec2ccd5b4d3ef566cbd885f8cddba75b4f1dc1712144147827689f06d";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPayloadValue_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPayloadValue_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadValue_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPayloadValue_Response>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPayloadValue_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPayloadValue_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPayloadValue_Response():changetype<Ros2Fanuc_msgsSrvGetPayloadValue_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPosReg_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPosReg_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>2-n)return -90;n+=2;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.index,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPosReg_Request{const value=new Ros2Fanuc_msgsSrvGetPosReg_Request();
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPosReg_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPosReg_Request>{
static readonly codecIdentity:string="UWVMROS1_f7f1391c5ea385bff615d95c5e38f01a809828b142112829dc436916b063505a";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPosReg_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPosReg_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPosReg_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPosReg_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPosReg_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPosReg_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPosReg_Request():changetype<Ros2Fanuc_msgsSrvGetPosReg_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetPosReg_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetPosReg_Response {
static readonly maxWireBytes:i32=601;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=601;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field representation, native scalar string. */
representation:string="";
/** Exact native field utool, native scalar uint8. */
utool:u8=0;
/** Exact native field uframe, native scalar uint8. */
uframe:u8=0;
/** Exact native field front, native scalar uint8. */
front:u8=0;
/** Exact native field up, native scalar uint8. */
up:u8=0;
/** Exact native field left, native scalar uint8. */
left:u8=0;
/** Exact native field flip, native scalar uint8. */
flip:u8=0;
/** Exact native field turn4, native scalar int8. */
turn4:i8=0;
/** Exact native field turn5, native scalar int8. */
turn5:i8=0;
/** Exact native field turn6, native scalar int8. */
turn6:i8=0;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field w, native scalar float32. */
w:f32=0;
/** Exact native field p, native scalar float32. */
p:f32=0;
/** Exact native field r, native scalar float32. */
r:f32=0;
/** Exact native field ext1, native scalar float32. */
ext1:f32=0;
/** Exact native field ext2, native scalar float32. */
ext2:f32=0;
/** Exact native field ext3, native scalar float32. */
ext3:f32=0;
/** Exact native field j1, native scalar float32. */
j1:f32=0;
/** Exact native field j2, native scalar float32. */
j2:f32=0;
/** Exact native field j3, native scalar float32. */
j3:f32=0;
/** Exact native field j4, native scalar float32. */
j4:f32=0;
/** Exact native field j5, native scalar float32. */
j5:f32=0;
/** Exact native field j6, native scalar float32. */
j6:f32=0;
/** Exact native field j7, native scalar float32. */
j7:f32=0;
/** Exact native field j8, native scalar float32. */
j8:f32=0;
/** Exact native field j9, native scalar float32. */
j9:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!validUnicode(this.representation)||this.representation.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.representation);if(size>512)return -22;if(4+size>601-n)return -90;n+=4+size;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(1>601-n)return -90;n+=1;
if(n>601)return -90;
}
{
if(!isFinite(this.x))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.w))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.p))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.r))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.ext1))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.ext2))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.ext3))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j1))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j2))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j3))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j4))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j5))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j6))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j7))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j8))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
{
if(!isFinite(this.j9))return -22;
if(4>601-n)return -90;n+=4;
if(n>601)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.text(this.representation);
w.integer(<u64>this.utool,1);
w.integer(<u64>this.uframe,1);
w.integer(<u64>this.front,1);
w.integer(<u64>this.up,1);
w.integer(<u64>this.left,1);
w.integer(<u64>this.flip,1);
w.integer(<u64>this.turn4,1);
w.integer(<u64>this.turn5,1);
w.integer(<u64>this.turn6,1);
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.w),4);
w.integer(<u64>reinterpret<u32>(this.p),4);
w.integer(<u64>reinterpret<u32>(this.r),4);
w.integer(<u64>reinterpret<u32>(this.ext1),4);
w.integer(<u64>reinterpret<u32>(this.ext2),4);
w.integer(<u64>reinterpret<u32>(this.ext3),4);
w.integer(<u64>reinterpret<u32>(this.j1),4);
w.integer(<u64>reinterpret<u32>(this.j2),4);
w.integer(<u64>reinterpret<u32>(this.j3),4);
w.integer(<u64>reinterpret<u32>(this.j4),4);
w.integer(<u64>reinterpret<u32>(this.j5),4);
w.integer(<u64>reinterpret<u32>(this.j6),4);
w.integer(<u64>reinterpret<u32>(this.j7),4);
w.integer(<u64>reinterpret<u32>(this.j8),4);
w.integer(<u64>reinterpret<u32>(this.j9),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetPosReg_Response{const value=new Ros2Fanuc_msgsSrvGetPosReg_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.representation=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool=decoded;
}
{
const decoded=<u8>r.integer(1);
value.uframe=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front=decoded;
}
{
const decoded=<u8>r.integer(1);
value.up=decoded;
}
{
const decoded=<u8>r.integer(1);
value.left=decoded;
}
{
const decoded=<u8>r.integer(1);
value.flip=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn5=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn6=decoded;
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
value.z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.p=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.r=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j9=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetPosReg_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetPosReg_Response>{
static readonly codecIdentity:string="UWVMROS1_e540225b0d532c460905582ad99ee175dd771e971c7e75f5dfb9325d39ed6b27";
static readonly maxWireBytes:i32=601;
encodeChecked(value:Ros2Fanuc_msgsSrvGetPosReg_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetPosReg_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetPosReg_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetPosReg_Response>();if(bytes.length>601){out.status=90;return out;}if(bytes.length<89){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetPosReg_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetPosReg_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetPosReg_Response():changetype<Ros2Fanuc_msgsSrvGetPosReg_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetTCPSpeed_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetTCPSpeed_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetTCPSpeed_Request{const value=new Ros2Fanuc_msgsSrvGetTCPSpeed_Request();
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetTCPSpeed_Request>{
static readonly codecIdentity:string="UWVMROS1_6bdfd44465b75bbd4e232f802dbd3c0bfa28e90a93f9a28b3c15fb150bce7ac7";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetTCPSpeed_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetTCPSpeed_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetTCPSpeed_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetTCPSpeed_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetTCPSpeed_Request():changetype<Ros2Fanuc_msgsSrvGetTCPSpeed_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetTCPSpeed_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetTCPSpeed_Response {
static readonly maxWireBytes:i32=13;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=13;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field timetag, native scalar int32. */
timetag:i32=0;
/** Exact native field speed, native scalar float32. */
speed:f32=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>13-n)return -90;n+=4;
if(n>13)return -90;
}
{
if(4>13-n)return -90;n+=4;
if(n>13)return -90;
}
{
if(!isFinite(this.speed))return -22;
if(4>13-n)return -90;n+=4;
if(n>13)return -90;
}
{
if(1>13-n)return -90;n+=1;
if(n>13)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.timetag,4);
w.integer(<u64>reinterpret<u32>(this.speed),4);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetTCPSpeed_Response{const value=new Ros2Fanuc_msgsSrvGetTCPSpeed_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.timetag=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.speed=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetTCPSpeed_Response>{
static readonly codecIdentity:string="UWVMROS1_3ec07d99342f275506452f32076f9e1ea01bf32f43662311708a9b44ca951147";
static readonly maxWireBytes:i32=13;
encodeChecked(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetTCPSpeed_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetTCPSpeed_Response>();if(bytes.length>13){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetTCPSpeed_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetTCPSpeed_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetTCPSpeed_Response():changetype<Ros2Fanuc_msgsSrvGetTCPSpeed_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUFrameData_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUFrameData_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field uframe_number, native scalar uint8. */
uframe_number:u8=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.uframe_number,1);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUFrameData_Request{const value=new Ros2Fanuc_msgsSrvGetUFrameData_Request();
{
const decoded=<u8>r.integer(1);
value.uframe_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUFrameData_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUFrameData_Request>{
static readonly codecIdentity:string="UWVMROS1_c1ef7077fd7f61ed8ff46931c8e39c5950658cfe1d36cffac10083ab304307be";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUFrameData_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUFrameData_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameData_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameData_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUFrameData_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUFrameData_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUFrameData_Request():changetype<Ros2Fanuc_msgsSrvGetUFrameData_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUFrameData_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUFrameData_Response {
static readonly maxWireBytes:i32=30;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=30;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field uframe_number, native scalar uint8. */
uframe_number:u8=0;
/** Exact native field frame, native scalar fanuc_msgs/msg/Frame. */
frame:Ros2Fanuc_msgsMsgFrame=new Ros2Fanuc_msgsMsgFrame();
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
{
const child=this.frame.wireSize();if(child<0)return child;if(child>30-n)return -90;n+=child;
if(n>30)return -90;
}
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.uframe_number,1);
this.frame.writeControl(w);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUFrameData_Response{const value=new Ros2Fanuc_msgsSrvGetUFrameData_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.uframe_number=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrame.readControl(r);
value.frame=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUFrameData_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUFrameData_Response>{
static readonly codecIdentity:string="UWVMROS1_f956e1447bd4a15f547fdf5d6775836018d50153270fe8863c4b30d83422fd2e";
static readonly maxWireBytes:i32=30;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUFrameData_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUFrameData_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameData_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameData_Response>();if(bytes.length>30){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUFrameData_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUFrameData_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUFrameData_Response():changetype<Ros2Fanuc_msgsSrvGetUFrameData_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUFrameUTool_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUFrameUTool_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUFrameUTool_Request{const value=new Ros2Fanuc_msgsSrvGetUFrameUTool_Request();
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUFrameUTool_Request>{
static readonly codecIdentity:string="UWVMROS1_3d57da6f8003b33b2a079d4f284eb0f182d7e088074112b7f4ee25d3113bd443";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameUTool_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameUTool_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUFrameUTool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUFrameUTool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUFrameUTool_Request():changetype<Ros2Fanuc_msgsSrvGetUFrameUTool_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUFrameUTool_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUFrameUTool_Response {
static readonly maxWireBytes:i32=7;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=7;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field uframe_number, native scalar uint8. */
uframe_number:u8=0;
/** Exact native field utool_number, native scalar uint8. */
utool_number:u8=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>7-n)return -90;n+=4;
if(n>7)return -90;
}
{
if(1>7-n)return -90;n+=1;
if(n>7)return -90;
}
{
if(1>7-n)return -90;n+=1;
if(n>7)return -90;
}
{
if(1>7-n)return -90;n+=1;
if(n>7)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.uframe_number,1);
w.integer(<u64>this.utool_number,1);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUFrameUTool_Response{const value=new Ros2Fanuc_msgsSrvGetUFrameUTool_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.uframe_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUFrameUTool_Response>{
static readonly codecIdentity:string="UWVMROS1_f9cb64b89f17b6811e4073b9f94d0fd6844d21ec4b846f78bfd13cb14a35a6b3";
static readonly maxWireBytes:i32=7;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameUTool_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUFrameUTool_Response>();if(bytes.length>7){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUFrameUTool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUFrameUTool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUFrameUTool_Response():changetype<Ros2Fanuc_msgsSrvGetUFrameUTool_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUToolData_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUToolData_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field utool_number, native scalar uint8. */
utool_number:u8=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.utool_number,1);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUToolData_Request{const value=new Ros2Fanuc_msgsSrvGetUToolData_Request();
{
const decoded=<u8>r.integer(1);
value.utool_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUToolData_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUToolData_Request>{
static readonly codecIdentity:string="UWVMROS1_10c3799dcb1b09dbcb6d1b794a00ecfaf69a8f457d95eeb92855f79dba7be1bc";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUToolData_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUToolData_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUToolData_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUToolData_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUToolData_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUToolData_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUToolData_Request():changetype<Ros2Fanuc_msgsSrvGetUToolData_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/GetUToolData_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvGetUToolData_Response {
static readonly maxWireBytes:i32=30;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=30;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field utool_number, native scalar uint8. */
utool_number:u8=0;
/** Exact native field frame, native scalar fanuc_msgs/msg/Frame. */
frame:Ros2Fanuc_msgsMsgFrame=new Ros2Fanuc_msgsMsgFrame();
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
{
const child=this.frame.wireSize();if(child<0)return child;if(child>30-n)return -90;n+=child;
if(n>30)return -90;
}
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.utool_number,1);
this.frame.writeControl(w);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvGetUToolData_Response{const value=new Ros2Fanuc_msgsSrvGetUToolData_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool_number=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrame.readControl(r);
value.frame=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvGetUToolData_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvGetUToolData_Response>{
static readonly codecIdentity:string="UWVMROS1_f3a2b94f337cec9c58156b803675f38885350cc3fc3171234c0d361afc800d81";
static readonly maxWireBytes:i32=30;
encodeChecked(value:Ros2Fanuc_msgsSrvGetUToolData_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvGetUToolData_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvGetUToolData_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvGetUToolData_Response>();if(bytes.length>30){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvGetUToolData_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvGetUToolData_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvGetUToolData_Response():changetype<Ros2Fanuc_msgsSrvGetUToolData_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/ReadError_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvReadError_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field count, native scalar uint8. */
count:u8=1;
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
w.integer(<u64>this.count,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvReadError_Request{const value=new Ros2Fanuc_msgsSrvReadError_Request();
{
const decoded=<u8>r.integer(1);
value.count=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvReadError_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvReadError_Request>{
static readonly codecIdentity:string="UWVMROS1_4b67ed8e2ecccefb6bdead80249f5916e6ea192d822183292db43351839fb6f9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsSrvReadError_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvReadError_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvReadError_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvReadError_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvReadError_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvReadError_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvReadError_Request():changetype<Ros2Fanuc_msgsSrvReadError_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/ReadError_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvReadError_Response {
static readonly maxWireBytes:i32=2585;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2585;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field count, native scalar uint8. */
count:u8=0;
/** Exact native field error_data, native scalar string. */
error_data:Array<string>=new Array<string>();
constructor(){
for(let i=0;i<5;++i)this.error_data.push("");
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>2585-n)return -90;n+=4;
if(n>2585)return -90;
}
{
if(1>2585-n)return -90;n+=1;
if(n>2585)return -90;
}
{
if(this.error_data.length!=5)return -22;
for(let i=0;i<this.error_data.length;++i){
if(!validUnicode(this.error_data[i])||this.error_data[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_data[i]);if(size>512)return -22;if(4+size>2585-n)return -90;n+=4+size;
if(n>2585)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result,4);
w.integer(<u64>this.count,1);
for(let i=0;i<this.error_data.length;++i){
w.text(this.error_data[i]);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvReadError_Response{const value=new Ros2Fanuc_msgsSrvReadError_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.count=decoded;
}
{
const count:u32=5;if(!r.valid||count>5){r.valid=false;return value;}
value.error_data=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_data.push(decoded);
}
}
return value;}
}
export class Ros2Fanuc_msgsSrvReadError_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvReadError_Response>{
static readonly codecIdentity:string="UWVMROS1_6e28db10c7afb06cb1256f6d74a43e26c5efa6c13fec8826ae0e623a5eb954b6";
static readonly maxWireBytes:i32=2585;
encodeChecked(value:Ros2Fanuc_msgsSrvReadError_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvReadError_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvReadError_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvReadError_Response>();if(bytes.length>2585){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvReadError_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvReadError_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvReadError_Response():changetype<Ros2Fanuc_msgsSrvReadError_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/Reset_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvReset_Request {
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
static readControl(r:Reader):Ros2Fanuc_msgsSrvReset_Request{const value=new Ros2Fanuc_msgsSrvReset_Request();
return value;}
}
export class Ros2Fanuc_msgsSrvReset_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvReset_Request>{
static readonly codecIdentity:string="UWVMROS1_b73f661d569f6af8dad1cd6625f4c0c55ef2c14c79b3e9af0f17fdc02736abc4";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Fanuc_msgsSrvReset_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvReset_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvReset_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvReset_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvReset_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvReset_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvReset_Request():changetype<Ros2Fanuc_msgsSrvReset_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/Reset_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvReset_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvReset_Response{const value=new Ros2Fanuc_msgsSrvReset_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvReset_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvReset_Response>{
static readonly codecIdentity:string="UWVMROS1_5e9b92e5a112b4934ce0893afaa09156fdb47c26b0aa6ae9a27d61920d308ab8";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvReset_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvReset_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvReset_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvReset_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvReset_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvReset_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvReset_Response():changetype<Ros2Fanuc_msgsSrvReset_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetAnalogIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetAnalogIO_Request {
static readonly maxWireBytes:i32=74;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=74;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>74-n)return -90;n+=child;
if(n>74)return -90;
}
{
if(2>74-n)return -90;n+=2;
if(n>74)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>74-n)return -90;n+=4;
if(n>74)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetAnalogIO_Request{const value=new Ros2Fanuc_msgsSrvSetAnalogIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetAnalogIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetAnalogIO_Request>{
static readonly codecIdentity:string="UWVMROS1_a6bb4acfae23db79464faff849f359657537026d7c79f8a6c60259caec5e87f4";
static readonly maxWireBytes:i32=74;
encodeChecked(value:Ros2Fanuc_msgsSrvSetAnalogIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetAnalogIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetAnalogIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetAnalogIO_Request>();if(bytes.length>74){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetAnalogIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetAnalogIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetAnalogIO_Request():changetype<Ros2Fanuc_msgsSrvSetAnalogIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetAnalogIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetAnalogIO_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetAnalogIO_Response{const value=new Ros2Fanuc_msgsSrvSetAnalogIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetAnalogIO_Response>{
static readonly codecIdentity:string="UWVMROS1_aeac383e2c4a292f1237494bff5b261cd0197542e00456b4ee0edf9b7fe8a39b";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetAnalogIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetAnalogIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetAnalogIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetAnalogIO_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetAnalogIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetAnalogIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetAnalogIO_Response():changetype<Ros2Fanuc_msgsSrvSetAnalogIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetBoolIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetBoolIO_Request {
static readonly maxWireBytes:i32=71;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=71;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar bool. */
value:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>71-n)return -90;n+=child;
if(n>71)return -90;
}
{
if(2>71-n)return -90;n+=2;
if(n>71)return -90;
}
{
if(1>71-n)return -90;n+=1;
if(n>71)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
w.u8(this.value?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetBoolIO_Request{const value=new Ros2Fanuc_msgsSrvSetBoolIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetBoolIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetBoolIO_Request>{
static readonly codecIdentity:string="UWVMROS1_0e8ace58c769670a2519e733d020a3ca902bff99982514d5cd9ecda391c1a4cf";
static readonly maxWireBytes:i32=71;
encodeChecked(value:Ros2Fanuc_msgsSrvSetBoolIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetBoolIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetBoolIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetBoolIO_Request>();if(bytes.length>71){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetBoolIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetBoolIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetBoolIO_Request():changetype<Ros2Fanuc_msgsSrvSetBoolIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetBoolIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetBoolIO_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetBoolIO_Response{const value=new Ros2Fanuc_msgsSrvSetBoolIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetBoolIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetBoolIO_Response>{
static readonly codecIdentity:string="UWVMROS1_c9c68ca577dd6e4d1743385d9c69c8f1c6e0780ae247877d823ab97a68e675a0";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetBoolIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetBoolIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetBoolIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetBoolIO_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetBoolIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetBoolIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetBoolIO_Response():changetype<Ros2Fanuc_msgsSrvSetBoolIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetGenOverride_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetGenOverride_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field value, native scalar int8. */
value:i8=0;
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
w.integer(<u64>this.value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetGenOverride_Request{const value=new Ros2Fanuc_msgsSrvSetGenOverride_Request();
{
const decoded=<i8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetGenOverride_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetGenOverride_Request>{
static readonly codecIdentity:string="UWVMROS1_7d321e33aaf6f58013c33ffc012c1ddbd27aefff3f21d7456030a1d590362039";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Fanuc_msgsSrvSetGenOverride_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetGenOverride_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetGenOverride_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetGenOverride_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetGenOverride_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetGenOverride_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetGenOverride_Request():changetype<Ros2Fanuc_msgsSrvSetGenOverride_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetGenOverride_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetGenOverride_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetGenOverride_Response{const value=new Ros2Fanuc_msgsSrvSetGenOverride_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetGenOverride_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetGenOverride_Response>{
static readonly codecIdentity:string="UWVMROS1_1ffcb25cad7dbf51c216753dac2c117a191fb081a5a562e63d34d447e61f0a59";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetGenOverride_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetGenOverride_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetGenOverride_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetGenOverride_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetGenOverride_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetGenOverride_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetGenOverride_Response():changetype<Ros2Fanuc_msgsSrvSetGenOverride_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetGroupIO_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetGroupIO_Request {
static readonly maxWireBytes:i32=72;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=72;
/** Exact native field io_type, native scalar fanuc_msgs/msg/IOType. */
io_type:Ros2Fanuc_msgsMsgIOType=new Ros2Fanuc_msgsMsgIOType();
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar uint16. */
value:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.io_type.wireSize();if(child<0)return child;if(child>72-n)return -90;n+=child;
if(n>72)return -90;
}
{
if(2>72-n)return -90;n+=2;
if(n>72)return -90;
}
{
if(2>72-n)return -90;n+=2;
if(n>72)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.io_type.writeControl(w);
w.integer(<u64>this.index,2);
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetGroupIO_Request{const value=new Ros2Fanuc_msgsSrvSetGroupIO_Request();
{
const decoded=Ros2Fanuc_msgsMsgIOType.readControl(r);
value.io_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetGroupIO_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetGroupIO_Request>{
static readonly codecIdentity:string="UWVMROS1_42a76aec283e26e3a2c99bb27a23778e34effb6c3328754793f783038a2b84bd";
static readonly maxWireBytes:i32=72;
encodeChecked(value:Ros2Fanuc_msgsSrvSetGroupIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetGroupIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetGroupIO_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetGroupIO_Request>();if(bytes.length>72){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetGroupIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetGroupIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetGroupIO_Request():changetype<Ros2Fanuc_msgsSrvSetGroupIO_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetGroupIO_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetGroupIO_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetGroupIO_Response{const value=new Ros2Fanuc_msgsSrvSetGroupIO_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetGroupIO_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetGroupIO_Response>{
static readonly codecIdentity:string="UWVMROS1_aa6d2c99e8817273f8f2ab179df61d60efd8ee2cee0e1ec76c647956e22bff17";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetGroupIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetGroupIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetGroupIO_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetGroupIO_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetGroupIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetGroupIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetGroupIO_Response():changetype<Ros2Fanuc_msgsSrvSetGroupIO_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetNumReg_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetNumReg_Request {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>6-n)return -90;n+=2;
if(n>6)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.index,2);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetNumReg_Request{const value=new Ros2Fanuc_msgsSrvSetNumReg_Request();
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetNumReg_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetNumReg_Request>{
static readonly codecIdentity:string="UWVMROS1_7e362b671b8770db33f3876308c41a930863a0805771fa05fcbb170782207e36";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Fanuc_msgsSrvSetNumReg_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetNumReg_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetNumReg_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetNumReg_Request>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetNumReg_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetNumReg_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetNumReg_Request():changetype<Ros2Fanuc_msgsSrvSetNumReg_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetNumReg_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetNumReg_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetNumReg_Response{const value=new Ros2Fanuc_msgsSrvSetNumReg_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetNumReg_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetNumReg_Response>{
static readonly codecIdentity:string="UWVMROS1_7c7850ad8bb5c861d9b436311a8cf985e2e029d362877be80c5a6cb19021e482";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetNumReg_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetNumReg_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetNumReg_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetNumReg_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetNumReg_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetNumReg_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetNumReg_Response():changetype<Ros2Fanuc_msgsSrvSetNumReg_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadComp_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadComp_Request {
static readonly maxWireBytes:i32=30;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=30;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cg_x, native scalar float32. */
cg_x:f32=0;
/** Exact native field cg_y, native scalar float32. */
cg_y:f32=0;
/** Exact native field cg_z, native scalar float32. */
cg_z:f32=0;
/** Exact native field in_x, native scalar float32. */
in_x:f32=0;
/** Exact native field in_y, native scalar float32. */
in_y:f32=0;
/** Exact native field in_z, native scalar float32. */
in_z:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>30-n)return -90;n+=2;
if(n>30)return -90;
}
{
if(!isFinite(this.mass))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.cg_x))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.cg_y))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.cg_z))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.in_x))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.in_y))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.in_z))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
w.integer(<u64>reinterpret<u32>(this.mass),4);
w.integer(<u64>reinterpret<u32>(this.cg_x),4);
w.integer(<u64>reinterpret<u32>(this.cg_y),4);
w.integer(<u64>reinterpret<u32>(this.cg_z),4);
w.integer(<u64>reinterpret<u32>(this.in_x),4);
w.integer(<u64>reinterpret<u32>(this.in_y),4);
w.integer(<u64>reinterpret<u32>(this.in_z),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadComp_Request{const value=new Ros2Fanuc_msgsSrvSetPayloadComp_Request();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_z=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadComp_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadComp_Request>{
static readonly codecIdentity:string="UWVMROS1_92c495d71657249e15c172e850aa91869929a25817437567778df1b01d9037a8";
static readonly maxWireBytes:i32=30;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadComp_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadComp_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadComp_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadComp_Request>();if(bytes.length>30){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadComp_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadComp_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadComp_Request():changetype<Ros2Fanuc_msgsSrvSetPayloadComp_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadComp_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadComp_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadComp_Response{const value=new Ros2Fanuc_msgsSrvSetPayloadComp_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadComp_Response>{
static readonly codecIdentity:string="UWVMROS1_37a9876e2a9c514fa8c4337ff1351f778d350318bdc6d8aa5d569b4039a2f76b";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadComp_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadComp_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadComp_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadComp_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadComp_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadComp_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadComp_Response():changetype<Ros2Fanuc_msgsSrvSetPayloadComp_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadID_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadID_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>2-n)return -90;n+=2;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadID_Request{const value=new Ros2Fanuc_msgsSrvSetPayloadID_Request();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadID_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadID_Request>{
static readonly codecIdentity:string="UWVMROS1_aa15b59db17d2afb7ef9e74676a9b744763954ae063efafb91a3ab3c9852a1c0";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadID_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadID_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadID_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadID_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadID_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadID_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadID_Request():changetype<Ros2Fanuc_msgsSrvSetPayloadID_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadID_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadID_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadID_Response{const value=new Ros2Fanuc_msgsSrvSetPayloadID_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadID_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadID_Response>{
static readonly codecIdentity:string="UWVMROS1_7b704b36da10dc71b4cfaad92ea02ed22e4966077dc0940b03b8b8e49c5769c7";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadID_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadID_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadID_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadID_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadID_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadID_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadID_Response():changetype<Ros2Fanuc_msgsSrvSetPayloadID_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadValue_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadValue_Request {
static readonly maxWireBytes:i32=31;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=31;
/** Exact native field payload_schedule_id, native scalar uint16. */
payload_schedule_id:u16=0;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cg_x, native scalar float32. */
cg_x:f32=0;
/** Exact native field cg_y, native scalar float32. */
cg_y:f32=0;
/** Exact native field cg_z, native scalar float32. */
cg_z:f32=0;
/** Exact native field use_in, native scalar bool. */
use_in:bool=false;
/** Exact native field in_x, native scalar float32. */
in_x:f32=0;
/** Exact native field in_y, native scalar float32. */
in_y:f32=0;
/** Exact native field in_z, native scalar float32. */
in_z:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>31-n)return -90;n+=2;
if(n>31)return -90;
}
{
if(!isFinite(this.mass))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(!isFinite(this.cg_x))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(!isFinite(this.cg_y))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(!isFinite(this.cg_z))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(1>31-n)return -90;n+=1;
if(n>31)return -90;
}
{
if(!isFinite(this.in_x))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(!isFinite(this.in_y))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
{
if(!isFinite(this.in_z))return -22;
if(4>31-n)return -90;n+=4;
if(n>31)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.payload_schedule_id,2);
w.integer(<u64>reinterpret<u32>(this.mass),4);
w.integer(<u64>reinterpret<u32>(this.cg_x),4);
w.integer(<u64>reinterpret<u32>(this.cg_y),4);
w.integer(<u64>reinterpret<u32>(this.cg_z),4);
w.u8(this.use_in?1:0);
w.integer(<u64>reinterpret<u32>(this.in_x),4);
w.integer(<u64>reinterpret<u32>(this.in_y),4);
w.integer(<u64>reinterpret<u32>(this.in_z),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadValue_Request{const value=new Ros2Fanuc_msgsSrvSetPayloadValue_Request();
{
const decoded=<u16>r.integer(2);
value.payload_schedule_id=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cg_z=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_in=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.in_z=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadValue_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadValue_Request>{
static readonly codecIdentity:string="UWVMROS1_5d56ce8f2f8df9c3450d6a5dc77e9e77861471805aa6b9c6f8815c2e7c324974";
static readonly maxWireBytes:i32=31;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadValue_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadValue_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadValue_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadValue_Request>();if(bytes.length>31){out.status=90;return out;}if(bytes.length<31){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadValue_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadValue_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadValue_Request():changetype<Ros2Fanuc_msgsSrvSetPayloadValue_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPayloadValue_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPayloadValue_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPayloadValue_Response{const value=new Ros2Fanuc_msgsSrvSetPayloadValue_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPayloadValue_Response>{
static readonly codecIdentity:string="UWVMROS1_09f30071b49ac57353429e8a2862ee80d3dcab9237e8192d008c12a3628cc6ab";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPayloadValue_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPayloadValue_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadValue_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPayloadValue_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPayloadValue_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPayloadValue_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPayloadValue_Response():changetype<Ros2Fanuc_msgsSrvSetPayloadValue_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPosReg_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPosReg_Request {
static readonly maxWireBytes:i32=599;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=599;
/** Exact native field index, native scalar uint16. */
index:u16=0;
/** Exact native field representation, native scalar string. */
representation:string="";
/** Exact native field utool, native scalar uint8. */
utool:u8=0;
/** Exact native field uframe, native scalar uint8. */
uframe:u8=0;
/** Exact native field front, native scalar uint8. */
front:u8=0;
/** Exact native field up, native scalar uint8. */
up:u8=0;
/** Exact native field left, native scalar uint8. */
left:u8=0;
/** Exact native field flip, native scalar uint8. */
flip:u8=0;
/** Exact native field turn4, native scalar int8. */
turn4:i8=0;
/** Exact native field turn5, native scalar int8. */
turn5:i8=0;
/** Exact native field turn6, native scalar int8. */
turn6:i8=0;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field w, native scalar float32. */
w:f32=0;
/** Exact native field p, native scalar float32. */
p:f32=0;
/** Exact native field r, native scalar float32. */
r:f32=0;
/** Exact native field ext1, native scalar float32. */
ext1:f32=0;
/** Exact native field ext2, native scalar float32. */
ext2:f32=0;
/** Exact native field ext3, native scalar float32. */
ext3:f32=0;
/** Exact native field j1, native scalar float32. */
j1:f32=0;
/** Exact native field j2, native scalar float32. */
j2:f32=0;
/** Exact native field j3, native scalar float32. */
j3:f32=0;
/** Exact native field j4, native scalar float32. */
j4:f32=0;
/** Exact native field j5, native scalar float32. */
j5:f32=0;
/** Exact native field j6, native scalar float32. */
j6:f32=0;
/** Exact native field j7, native scalar float32. */
j7:f32=0;
/** Exact native field j8, native scalar float32. */
j8:f32=0;
/** Exact native field j9, native scalar float32. */
j9:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>599-n)return -90;n+=2;
if(n>599)return -90;
}
{
if(!validUnicode(this.representation)||this.representation.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.representation);if(size>512)return -22;if(4+size>599-n)return -90;n+=4+size;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(1>599-n)return -90;n+=1;
if(n>599)return -90;
}
{
if(!isFinite(this.x))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.w))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.p))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.r))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.ext1))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.ext2))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.ext3))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j1))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j2))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j3))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j4))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j5))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j6))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j7))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j8))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
{
if(!isFinite(this.j9))return -22;
if(4>599-n)return -90;n+=4;
if(n>599)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.index,2);
w.text(this.representation);
w.integer(<u64>this.utool,1);
w.integer(<u64>this.uframe,1);
w.integer(<u64>this.front,1);
w.integer(<u64>this.up,1);
w.integer(<u64>this.left,1);
w.integer(<u64>this.flip,1);
w.integer(<u64>this.turn4,1);
w.integer(<u64>this.turn5,1);
w.integer(<u64>this.turn6,1);
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.w),4);
w.integer(<u64>reinterpret<u32>(this.p),4);
w.integer(<u64>reinterpret<u32>(this.r),4);
w.integer(<u64>reinterpret<u32>(this.ext1),4);
w.integer(<u64>reinterpret<u32>(this.ext2),4);
w.integer(<u64>reinterpret<u32>(this.ext3),4);
w.integer(<u64>reinterpret<u32>(this.j1),4);
w.integer(<u64>reinterpret<u32>(this.j2),4);
w.integer(<u64>reinterpret<u32>(this.j3),4);
w.integer(<u64>reinterpret<u32>(this.j4),4);
w.integer(<u64>reinterpret<u32>(this.j5),4);
w.integer(<u64>reinterpret<u32>(this.j6),4);
w.integer(<u64>reinterpret<u32>(this.j7),4);
w.integer(<u64>reinterpret<u32>(this.j8),4);
w.integer(<u64>reinterpret<u32>(this.j9),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPosReg_Request{const value=new Ros2Fanuc_msgsSrvSetPosReg_Request();
{
const decoded=<u16>r.integer(2);
value.index=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.representation=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool=decoded;
}
{
const decoded=<u8>r.integer(1);
value.uframe=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front=decoded;
}
{
const decoded=<u8>r.integer(1);
value.up=decoded;
}
{
const decoded=<u8>r.integer(1);
value.left=decoded;
}
{
const decoded=<u8>r.integer(1);
value.flip=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn5=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn6=decoded;
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
value.z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.p=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.r=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.j9=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPosReg_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPosReg_Request>{
static readonly codecIdentity:string="UWVMROS1_f9af742e55a044a1a363a45cbe4a0a8ce4ca62fcaf43759041cef89149d96d30";
static readonly maxWireBytes:i32=599;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPosReg_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPosReg_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPosReg_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPosReg_Request>();if(bytes.length>599){out.status=90;return out;}if(bytes.length<87){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPosReg_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPosReg_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPosReg_Request():changetype<Ros2Fanuc_msgsSrvSetPosReg_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetPosReg_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetPosReg_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetPosReg_Response{const value=new Ros2Fanuc_msgsSrvSetPosReg_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetPosReg_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetPosReg_Response>{
static readonly codecIdentity:string="UWVMROS1_fb6f889d30c921df31f55cdc760346eb0d655816e71b52d4aac7a1e56d4e9cb2";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSetPosReg_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetPosReg_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetPosReg_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetPosReg_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetPosReg_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetPosReg_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetPosReg_Response():changetype<Ros2Fanuc_msgsSrvSetPosReg_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUFrameData_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUFrameData_Request {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field uframe_number, native scalar uint8. */
uframe_number:u8=0;
/** Exact native field frame, native scalar fanuc_msgs/msg/Frame. */
frame:Ros2Fanuc_msgsMsgFrame=new Ros2Fanuc_msgsMsgFrame();
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
const child=this.frame.wireSize();if(child<0)return child;if(child>26-n)return -90;n+=child;
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
w.integer(<u64>this.uframe_number,1);
this.frame.writeControl(w);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUFrameData_Request{const value=new Ros2Fanuc_msgsSrvSetUFrameData_Request();
{
const decoded=<u8>r.integer(1);
value.uframe_number=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrame.readControl(r);
value.frame=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUFrameData_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUFrameData_Request>{
static readonly codecIdentity:string="UWVMROS1_a067808d8fe5ca1418e18ac492313ad9211cc8f3bf402ad0ac51a2850a2bc169";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUFrameData_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUFrameData_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameData_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameData_Request>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUFrameData_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUFrameData_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUFrameData_Request():changetype<Ros2Fanuc_msgsSrvSetUFrameData_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUFrameData_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUFrameData_Response {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUFrameData_Response{const value=new Ros2Fanuc_msgsSrvSetUFrameData_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUFrameData_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUFrameData_Response>{
static readonly codecIdentity:string="UWVMROS1_f436e4fe5bd06b048f4f53d2df4933ac8eac5a784f31b42ccf13f5eaf1696eee";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUFrameData_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUFrameData_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameData_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameData_Response>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUFrameData_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUFrameData_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUFrameData_Response():changetype<Ros2Fanuc_msgsSrvSetUFrameData_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUFrameUTool_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUFrameUTool_Request {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field uframe_number, native scalar uint8. */
uframe_number:u8=0;
/** Exact native field utool_number, native scalar uint8. */
utool_number:u8=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.uframe_number,1);
w.integer(<u64>this.utool_number,1);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUFrameUTool_Request{const value=new Ros2Fanuc_msgsSrvSetUFrameUTool_Request();
{
const decoded=<u8>r.integer(1);
value.uframe_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.utool_number=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUFrameUTool_Request>{
static readonly codecIdentity:string="UWVMROS1_0b86b0a37e4dc23e0ff651df867f5958e7440bf4712b47779d69ca699af4c09f";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameUTool_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameUTool_Request>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUFrameUTool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUFrameUTool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUFrameUTool_Request():changetype<Ros2Fanuc_msgsSrvSetUFrameUTool_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUFrameUTool_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUFrameUTool_Response {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUFrameUTool_Response{const value=new Ros2Fanuc_msgsSrvSetUFrameUTool_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUFrameUTool_Response>{
static readonly codecIdentity:string="UWVMROS1_b77837445c9663f29f23465cc7178a183d245486f74bcff732fc2f4025b82a67";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameUTool_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUFrameUTool_Response>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUFrameUTool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUFrameUTool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUFrameUTool_Response():changetype<Ros2Fanuc_msgsSrvSetUFrameUTool_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUToolData_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUToolData_Request {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field utool_number, native scalar uint8. */
utool_number:u8=0;
/** Exact native field frame, native scalar fanuc_msgs/msg/Frame. */
frame:Ros2Fanuc_msgsMsgFrame=new Ros2Fanuc_msgsMsgFrame();
/** Exact native field group, native scalar uint8. */
group:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
const child=this.frame.wireSize();if(child<0)return child;if(child>26-n)return -90;n+=child;
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
w.integer(<u64>this.utool_number,1);
this.frame.writeControl(w);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUToolData_Request{const value=new Ros2Fanuc_msgsSrvSetUToolData_Request();
{
const decoded=<u8>r.integer(1);
value.utool_number=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrame.readControl(r);
value.frame=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUToolData_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUToolData_Request>{
static readonly codecIdentity:string="UWVMROS1_7d5b911133645744c8def79bc16c8b7c4ee4e87a057ea33741031d6a94e620ab";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUToolData_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUToolData_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUToolData_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUToolData_Request>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUToolData_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUToolData_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUToolData_Request():changetype<Ros2Fanuc_msgsSrvSetUToolData_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SetUToolData_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSetUToolData_Response {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field group, native scalar uint8. */
group:u8=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.group,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSetUToolData_Response{const value=new Ros2Fanuc_msgsSrvSetUToolData_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSetUToolData_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSetUToolData_Response>{
static readonly codecIdentity:string="UWVMROS1_b33d265f275dfcb5fe20432d5e38b55574c85260135bded477c2d5fa2c85c7cb";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Fanuc_msgsSrvSetUToolData_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSetUToolData_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSetUToolData_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSetUToolData_Response>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSetUToolData_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSetUToolData_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSetUToolData_Response():changetype<Ros2Fanuc_msgsSrvSetUToolData_Response>(out.value);}
}
/** Complete original fanuc_msgs/srv/SwitchControlState_Request; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSwitchControlState_Request {
static readonly STOP:u32=0;
static readonly START:u32=1;
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field status, native scalar uint32. */
status:u32=0;
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
w.integer(<u64>this.status,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSwitchControlState_Request{const value=new Ros2Fanuc_msgsSrvSwitchControlState_Request();
{
const decoded=<u32>r.integer(4);
value.status=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSwitchControlState_RequestCodec implements VendorCodec<Ros2Fanuc_msgsSrvSwitchControlState_Request>{
static readonly codecIdentity:string="UWVMROS1_b485b7071fe2737c7dc8f0360a15b7be30f997430142d8260454fd423172cc21";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSwitchControlState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSwitchControlState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSwitchControlState_Request>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSwitchControlState_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSwitchControlState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSwitchControlState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSwitchControlState_Request():changetype<Ros2Fanuc_msgsSrvSwitchControlState_Request>(out.value);}
}
/** Complete original fanuc_msgs/srv/SwitchControlState_Response; spellings are published in generation proof. */
export class Ros2Fanuc_msgsSrvSwitchControlState_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Fanuc_msgsSrvSwitchControlState_Response{const value=new Ros2Fanuc_msgsSrvSwitchControlState_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Fanuc_msgsSrvSwitchControlState_ResponseCodec implements VendorCodec<Ros2Fanuc_msgsSrvSwitchControlState_Response>{
static readonly codecIdentity:string="UWVMROS1_8a3d7a3c1037174f4118843e34222ef8811eff3cc885a0c688d98071e1a9496c";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Fanuc_msgsSrvSwitchControlState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Fanuc_msgsSrvSwitchControlState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Fanuc_msgsSrvSwitchControlState_Response>{const out=new VendorDecoded<Ros2Fanuc_msgsSrvSwitchControlState_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Fanuc_msgsSrvSwitchControlState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Fanuc_msgsSrvSwitchControlState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Fanuc_msgsSrvSwitchControlState_Response():changetype<Ros2Fanuc_msgsSrvSwitchControlState_Response>(out.value);}
}
/** Complete original rmi_msgs/msg/CallParam; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgCallParam {
static readonly AR:string="AR";
static readonly PR:string="PR";
static readonly SR:string="SR";
static readonly R:string="R";
static readonly P:string="P";
static readonly INT:string="INT";
static readonly INTEGER:string="INT";
static readonly FLOAT:string="FLOAT";
static readonly STRING:string="String";
static readonly STR:string="String";
static readonly maxWireBytes:i32=592;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=592;
/** Exact native field type, native scalar string. */
ros_type:string="";
/** Exact native field int_value, native scalar int32. */
int_value:i32=0;
/** Exact native field float_value, native scalar float32. */
float_value:f32=0;
/** Exact native field string_value, native scalar string. */
string_value:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>592-n)return -90;n+=4+size;
if(n>592)return -90;
}
{
if(4>592-n)return -90;n+=4;
if(n>592)return -90;
}
{
if(!isFinite(this.float_value))return -22;
if(4>592-n)return -90;n+=4;
if(n>592)return -90;
}
{
if(!validUnicode(this.string_value)||this.string_value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.string_value);if(size>512)return -22;if(4+size>592-n)return -90;n+=4+size;
if(n>592)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
w.integer(<u64>this.int_value,4);
w.integer(<u64>reinterpret<u32>(this.float_value),4);
w.text(this.string_value);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgCallParam{const value=new Ros2Rmi_msgsMsgCallParam();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
{
const decoded=<i32>r.integer(4);
value.int_value=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.float_value=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.string_value=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgCallParamCodec implements VendorCodec<Ros2Rmi_msgsMsgCallParam>{
static readonly codecIdentity:string="UWVMROS1_ed625d80db6c4314b8373f2d205344d6ab8d71be46182dab5a497d36564a2255";
static readonly maxWireBytes:i32=592;
encodeChecked(value:Ros2Rmi_msgsMsgCallParam):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgCallParam):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgCallParam>{const out=new VendorDecoded<Ros2Rmi_msgsMsgCallParam>();if(bytes.length>592){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgCallParam.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgCallParam{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgCallParam():changetype<Ros2Rmi_msgsMsgCallParam>(out.value);}
}
/** Complete original rmi_msgs/msg/Cartesian; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgCartesian {
static readonly maxWireBytes:i32=45;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=45;
/** Exact native field utool, native scalar int8. */
utool:i8=1;
/** Exact native field uframe, native scalar int8. */
uframe:i8=0;
/** Exact native field front, native scalar uint8. */
front:u8=1;
/** Exact native field up, native scalar uint8. */
up:u8=1;
/** Exact native field left, native scalar uint8. */
left:u8=0;
/** Exact native field flip, native scalar uint8. */
flip:u8=0;
/** Exact native field turn4, native scalar int8. */
turn4:i8=0;
/** Exact native field turn5, native scalar int8. */
turn5:i8=0;
/** Exact native field turn6, native scalar int8. */
turn6:i8=0;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field w, native scalar float32. */
w:f32=0;
/** Exact native field p, native scalar float32. */
p:f32=0;
/** Exact native field r, native scalar float32. */
r:f32=0;
/** Exact native field ext1, native scalar float32. */
ext1:f32=0;
/** Exact native field ext2, native scalar float32. */
ext2:f32=0;
/** Exact native field ext3, native scalar float32. */
ext3:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(1>45-n)return -90;n+=1;
if(n>45)return -90;
}
{
if(!isFinite(this.x))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.w))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.p))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.r))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.ext1))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.ext2))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
{
if(!isFinite(this.ext3))return -22;
if(4>45-n)return -90;n+=4;
if(n>45)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.utool,1);
w.integer(<u64>this.uframe,1);
w.integer(<u64>this.front,1);
w.integer(<u64>this.up,1);
w.integer(<u64>this.left,1);
w.integer(<u64>this.flip,1);
w.integer(<u64>this.turn4,1);
w.integer(<u64>this.turn5,1);
w.integer(<u64>this.turn6,1);
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.w),4);
w.integer(<u64>reinterpret<u32>(this.p),4);
w.integer(<u64>reinterpret<u32>(this.r),4);
w.integer(<u64>reinterpret<u32>(this.ext1),4);
w.integer(<u64>reinterpret<u32>(this.ext2),4);
w.integer(<u64>reinterpret<u32>(this.ext3),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgCartesian{const value=new Ros2Rmi_msgsMsgCartesian();
{
const decoded=<i8>r.integer(1);
value.utool=decoded;
}
{
const decoded=<i8>r.integer(1);
value.uframe=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front=decoded;
}
{
const decoded=<u8>r.integer(1);
value.up=decoded;
}
{
const decoded=<u8>r.integer(1);
value.left=decoded;
}
{
const decoded=<u8>r.integer(1);
value.flip=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn5=decoded;
}
{
const decoded=<i8>r.integer(1);
value.turn6=decoded;
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
value.z=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.p=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.r=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ext3=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgCartesianCodec implements VendorCodec<Ros2Rmi_msgsMsgCartesian>{
static readonly codecIdentity:string="UWVMROS1_2a0e1fff119007499a63d55c9722f666be1436a9212d9e43b556af347e8e4b40";
static readonly maxWireBytes:i32=45;
encodeChecked(value:Ros2Rmi_msgsMsgCartesian):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgCartesian):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgCartesian>{const out=new VendorDecoded<Ros2Rmi_msgsMsgCartesian>();if(bytes.length>45){out.status=90;return out;}if(bytes.length<45){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgCartesian.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgCartesian{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgCartesian():changetype<Ros2Rmi_msgsMsgCartesian>(out.value);}
}
/** Complete original rmi_msgs/msg/Command; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgCommand {
static readonly INITIAZLIE:string="INITIALIZE";
static readonly INIT:string="INITIALIZE";
static readonly ABORT:string="ABORT";
static readonly PAUSE:string="PAUSE";
static readonly RESUME:string="RESUME";
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field type, native scalar string. */
ros_type:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgCommand{const value=new Ros2Rmi_msgsMsgCommand();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgCommandCodec implements VendorCodec<Ros2Rmi_msgsMsgCommand>{
static readonly codecIdentity:string="UWVMROS1_d2741856b8cdf57aa716566edfc705957611557b7da5b6099b173903bdd9960c";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Rmi_msgsMsgCommand):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgCommand):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgCommand>{const out=new VendorDecoded<Ros2Rmi_msgsMsgCommand>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgCommand.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgCommand{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgCommand():changetype<Ros2Rmi_msgsMsgCommand>(out.value);}
}
/** Complete original rmi_msgs/msg/CommandOptions; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgCommandOptions {
static readonly RTSA_ON:string="ON";
static readonly RTSA_OFF:string="OFF";
static readonly PLTZ_ZERODN:string="ZERODN";
static readonly PLTZ_ZEROUP:string="ZEROUP";
static readonly PLTZ_PSPIDN:string="PSPIDN";
static readonly PLTZ_PSPIUP:string="PSPIUP";
static readonly PLTZ_MSPIDN:string="MSPIDN";
static readonly PLTZ_MSPIUP:string="MSPIUP";
static readonly maxWireBytes:i32=1036;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1036;
/** Exact native field use_group_mask, native scalar bool. */
use_group_mask:bool=false;
/** Exact native field group_mask, native scalar uint8. */
group_mask:u8=1;
/** Exact native field use_rtsa, native scalar bool. */
use_rtsa:bool=false;
/** Exact native field rtsa, native scalar string. */
rtsa:string="ON";
/** Exact native field use_pltzmode, native scalar bool. */
use_pltzmode:bool=false;
/** Exact native field pltzmode, native scalar string. */
pltzmode:string="ZERODN";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1036-n)return -90;n+=1;
if(n>1036)return -90;
}
{
if(1>1036-n)return -90;n+=1;
if(n>1036)return -90;
}
{
if(1>1036-n)return -90;n+=1;
if(n>1036)return -90;
}
{
if(!validUnicode(this.rtsa)||this.rtsa.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.rtsa);if(size>512)return -22;if(4+size>1036-n)return -90;n+=4+size;
if(n>1036)return -90;
}
{
if(1>1036-n)return -90;n+=1;
if(n>1036)return -90;
}
{
if(!validUnicode(this.pltzmode)||this.pltzmode.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.pltzmode);if(size>512)return -22;if(4+size>1036-n)return -90;n+=4+size;
if(n>1036)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.use_group_mask?1:0);
w.integer(<u64>this.group_mask,1);
w.u8(this.use_rtsa?1:0);
w.text(this.rtsa);
w.u8(this.use_pltzmode?1:0);
w.text(this.pltzmode);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgCommandOptions{const value=new Ros2Rmi_msgsMsgCommandOptions();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_group_mask=decoded;
}
{
const decoded=<u8>r.integer(1);
value.group_mask=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_rtsa=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.rtsa=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_pltzmode=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.pltzmode=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgCommandOptionsCodec implements VendorCodec<Ros2Rmi_msgsMsgCommandOptions>{
static readonly codecIdentity:string="UWVMROS1_0be0c508035725bf9686807cb8b5267e23f709b7aa34b0910a44ff3604670147";
static readonly maxWireBytes:i32=1036;
encodeChecked(value:Ros2Rmi_msgsMsgCommandOptions):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgCommandOptions):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgCommandOptions>{const out=new VendorDecoded<Ros2Rmi_msgsMsgCommandOptions>();if(bytes.length>1036){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgCommandOptions.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgCommandOptions{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgCommandOptions():changetype<Ros2Rmi_msgsMsgCommandOptions>(out.value);}
}
/** Complete original rmi_msgs/msg/Joint; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgJoint {
static readonly maxWireBytes:i32=36;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=36;
/** Exact native field positions, native scalar float32. */
positions:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<9;++i)this.positions.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.positions.length!=9)return -22;
for(let i=0;i<this.positions.length;++i){
if(!isFinite(this.positions[i]))return -22;
if(4>36-n)return -90;n+=4;
if(n>36)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.positions.length;++i){
w.integer(<u64>reinterpret<u32>(this.positions[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgJoint{const value=new Ros2Rmi_msgsMsgJoint();
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.positions=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.positions.push(decoded);
}
}
return value;}
}
export class Ros2Rmi_msgsMsgJointCodec implements VendorCodec<Ros2Rmi_msgsMsgJoint>{
static readonly codecIdentity:string="UWVMROS1_0ba90adf6cc287bc306fd2a77c52c2e4d62731cfac37bb2cc116ee846249a42d";
static readonly maxWireBytes:i32=36;
encodeChecked(value:Ros2Rmi_msgsMsgJoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgJoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgJoint>{const out=new VendorDecoded<Ros2Rmi_msgsMsgJoint>();if(bytes.length>36){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgJoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgJoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgJoint():changetype<Ros2Rmi_msgsMsgJoint>(out.value);}
}
/** Complete original rmi_msgs/msg/MotionInstructionOptions; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgMotionInstructionOptions {
static readonly LCB_TYPE_TB:string="TB";
static readonly LCB_TYPE_TA:string="TA";
static readonly LCB_TYPE_DB:string="DB";
static readonly PORT_TYPE_DOUT:u8=1;
static readonly PORT_TYPE_DO:u8=1;
static readonly PORT_TYPE_ROUT:u8=2;
static readonly PORT_TYPE_RO:u8=2;
static readonly maxWireBytes:i32=2612;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2612;
/** Exact native field use_acc, native scalar bool. */
use_acc:bool=false;
/** Exact native field acc, native scalar uint8. */
acc:u8=0;
/** Exact native field use_offset_pr_number, native scalar bool. */
use_offset_pr_number:bool=false;
/** Exact native field offset_pr_number, native scalar uint16. */
offset_pr_number:u16=0;
/** Exact native field use_vision_pr_number, native scalar bool. */
use_vision_pr_number:bool=false;
/** Exact native field vision_pr_number, native scalar uint16. */
vision_pr_number:u16=0;
/** Exact native field use_wrist_joint, native scalar bool. */
use_wrist_joint:bool=false;
/** Exact native field wrist_joint, native scalar string. */
wrist_joint:string="ON";
/** Exact native field use_mrot, native scalar bool. */
use_mrot:bool=false;
/** Exact native field mrot, native scalar string. */
mrot:string="ON";
/** Exact native field use_lcb_type, native scalar bool. */
use_lcb_type:bool=false;
/** Exact native field lcb_type, native scalar string. */
lcb_type:string="";
/** Exact native field use_lcb_value, native scalar bool. */
use_lcb_value:bool=false;
/** Exact native field lcb_value, native scalar uint16. */
lcb_value:u16=0;
/** Exact native field use_port_type, native scalar bool. */
use_port_type:bool=false;
/** Exact native field port_type, native scalar uint8. */
port_type:u8=0;
/** Exact native field use_port_number, native scalar bool. */
use_port_number:bool=false;
/** Exact native field port_number, native scalar uint16. */
port_number:u16=0;
/** Exact native field use_port_value, native scalar bool. */
use_port_value:bool=false;
/** Exact native field port_value, native scalar string. */
port_value:string="";
/** Exact native field use_tool_offset_pr_number, native scalar bool. */
use_tool_offset_pr_number:bool=false;
/** Exact native field tool_offset_pr_number, native scalar uint16. */
tool_offset_pr_number:u16=0;
/** Exact native field use_alim, native scalar bool. */
use_alim:bool=false;
/** Exact native field alim, native scalar int32. */
alim:i32=0;
/** Exact native field use_alim_reg, native scalar bool. */
use_alim_reg:bool=false;
/** Exact native field alim_reg, native scalar uint16. */
alim_reg:u16=0;
/** Exact native field use_no_blend, native scalar bool. */
use_no_blend:bool=false;
/** Exact native field no_blend, native scalar string. */
no_blend:string="ON";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(!validUnicode(this.wrist_joint)||this.wrist_joint.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.wrist_joint);if(size>512)return -22;if(4+size>2612-n)return -90;n+=4+size;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(!validUnicode(this.mrot)||this.mrot.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.mrot);if(size>512)return -22;if(4+size>2612-n)return -90;n+=4+size;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(!validUnicode(this.lcb_type)||this.lcb_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.lcb_type);if(size>512)return -22;if(4+size>2612-n)return -90;n+=4+size;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(!validUnicode(this.port_value)||this.port_value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.port_value);if(size>512)return -22;if(4+size>2612-n)return -90;n+=4+size;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(4>2612-n)return -90;n+=4;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(2>2612-n)return -90;n+=2;
if(n>2612)return -90;
}
{
if(1>2612-n)return -90;n+=1;
if(n>2612)return -90;
}
{
if(!validUnicode(this.no_blend)||this.no_blend.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.no_blend);if(size>512)return -22;if(4+size>2612-n)return -90;n+=4+size;
if(n>2612)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.use_acc?1:0);
w.integer(<u64>this.acc,1);
w.u8(this.use_offset_pr_number?1:0);
w.integer(<u64>this.offset_pr_number,2);
w.u8(this.use_vision_pr_number?1:0);
w.integer(<u64>this.vision_pr_number,2);
w.u8(this.use_wrist_joint?1:0);
w.text(this.wrist_joint);
w.u8(this.use_mrot?1:0);
w.text(this.mrot);
w.u8(this.use_lcb_type?1:0);
w.text(this.lcb_type);
w.u8(this.use_lcb_value?1:0);
w.integer(<u64>this.lcb_value,2);
w.u8(this.use_port_type?1:0);
w.integer(<u64>this.port_type,1);
w.u8(this.use_port_number?1:0);
w.integer(<u64>this.port_number,2);
w.u8(this.use_port_value?1:0);
w.text(this.port_value);
w.u8(this.use_tool_offset_pr_number?1:0);
w.integer(<u64>this.tool_offset_pr_number,2);
w.u8(this.use_alim?1:0);
w.integer(<u64>this.alim,4);
w.u8(this.use_alim_reg?1:0);
w.integer(<u64>this.alim_reg,2);
w.u8(this.use_no_blend?1:0);
w.text(this.no_blend);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgMotionInstructionOptions{const value=new Ros2Rmi_msgsMsgMotionInstructionOptions();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_acc=decoded;
}
{
const decoded=<u8>r.integer(1);
value.acc=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_offset_pr_number=decoded;
}
{
const decoded=<u16>r.integer(2);
value.offset_pr_number=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_vision_pr_number=decoded;
}
{
const decoded=<u16>r.integer(2);
value.vision_pr_number=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_wrist_joint=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.wrist_joint=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_mrot=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.mrot=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_lcb_type=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.lcb_type=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_lcb_value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.lcb_value=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_port_type=decoded;
}
{
const decoded=<u8>r.integer(1);
value.port_type=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_port_number=decoded;
}
{
const decoded=<u16>r.integer(2);
value.port_number=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_port_value=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.port_value=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_tool_offset_pr_number=decoded;
}
{
const decoded=<u16>r.integer(2);
value.tool_offset_pr_number=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_alim=decoded;
}
{
const decoded=<i32>r.integer(4);
value.alim=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_alim_reg=decoded;
}
{
const decoded=<u16>r.integer(2);
value.alim_reg=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_no_blend=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.no_blend=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgMotionInstructionOptionsCodec implements VendorCodec<Ros2Rmi_msgsMsgMotionInstructionOptions>{
static readonly codecIdentity:string="UWVMROS1_f8d9ed2e083c577b0cae8965df71aa99643d12ce5a5a6ce558894edfb756bcc6";
static readonly maxWireBytes:i32=2612;
encodeChecked(value:Ros2Rmi_msgsMsgMotionInstructionOptions):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgMotionInstructionOptions):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgMotionInstructionOptions>{const out=new VendorDecoded<Ros2Rmi_msgsMsgMotionInstructionOptions>();if(bytes.length>2612){out.status=90;return out;}if(bytes.length<52){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgMotionInstructionOptions.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgMotionInstructionOptions{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgMotionInstructionOptions():changetype<Ros2Rmi_msgsMsgMotionInstructionOptions>(out.value);}
}
/** Complete original rmi_msgs/msg/MotionType; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgMotionType {
static readonly J:string="J";
static readonly JOINT:string="J";
static readonly L:string="L";
static readonly LINEAR:string="L";
static readonly C:string="C";
static readonly CIRCULAR:string="C";
static readonly S:string="S";
static readonly SPLINE:string="S";
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field type, native scalar string. */
ros_type:string="J";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgMotionType{const value=new Ros2Rmi_msgsMsgMotionType();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgMotionTypeCodec implements VendorCodec<Ros2Rmi_msgsMsgMotionType>{
static readonly codecIdentity:string="UWVMROS1_c51cc7a32f6853d13bedaf3306dd79e445de6c0d16ae0ec39034ce0ae6d19c8a";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Rmi_msgsMsgMotionType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgMotionType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgMotionType>{const out=new VendorDecoded<Ros2Rmi_msgsMsgMotionType>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgMotionType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgMotionType{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgMotionType():changetype<Ros2Rmi_msgsMsgMotionType>(out.value);}
}
/** Complete original rmi_msgs/msg/Representation; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgRepresentation {
static readonly JOINT:string="JOINT";
static readonly J:string="JOINT";
static readonly CARTESIAN:string="CARTESIAN";
static readonly CART:string="CARTESIAN";
static readonly C:string="CARTESIAN";
static readonly maxWireBytes:i32=516;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=516;
/** Exact native field representation, native scalar string. */
representation:string="JOINT";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.representation)||this.representation.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.representation);if(size>512)return -22;if(4+size>516-n)return -90;n+=4+size;
if(n>516)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.representation);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgRepresentation{const value=new Ros2Rmi_msgsMsgRepresentation();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.representation=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgRepresentationCodec implements VendorCodec<Ros2Rmi_msgsMsgRepresentation>{
static readonly codecIdentity:string="UWVMROS1_b439506f2dc2135bb484b3abb308dfdf5c07367af5a4d6d0f5435ad9bba8ea6f";
static readonly maxWireBytes:i32=516;
encodeChecked(value:Ros2Rmi_msgsMsgRepresentation):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgRepresentation):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgRepresentation>{const out=new VendorDecoded<Ros2Rmi_msgsMsgRepresentation>();if(bytes.length>516){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgRepresentation.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgRepresentation{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgRepresentation():changetype<Ros2Rmi_msgsMsgRepresentation>(out.value);}
}
/** Complete original rmi_msgs/msg/SpeedType; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgSpeedType {
static readonly PERCENT:string="Percent";
static readonly PERCENTAGE:string="Percent";
static readonly MMSEC:string="mmSec";
static readonly INCHMIN:string="InchMin";
static readonly TIME:string="Time";
static readonly SECONDS:string="Time";
static readonly SEC:string="Time";
static readonly MSEC:string="mSec";
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field type, native scalar string. */
ros_type:string="Percent";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgSpeedType{const value=new Ros2Rmi_msgsMsgSpeedType();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgSpeedTypeCodec implements VendorCodec<Ros2Rmi_msgsMsgSpeedType>{
static readonly codecIdentity:string="UWVMROS1_e4f4dbda84d8d8cd4d026b0beb52005663c7c4ee602c8d1e4d5853589777c282";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Rmi_msgsMsgSpeedType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgSpeedType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgSpeedType>{const out=new VendorDecoded<Ros2Rmi_msgsMsgSpeedType>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgSpeedType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgSpeedType{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgSpeedType():changetype<Ros2Rmi_msgsMsgSpeedType>(out.value);}
}
/** Complete original rmi_msgs/msg/TermType; spellings are published in generation proof. */
export class Ros2Rmi_msgsMsgTermType {
static readonly FINE:string="FINE";
static readonly CNT:string="CNT";
static readonly CR:string="CR";
static readonly CD:string="CD";
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field type, native scalar string. */
ros_type:string="FINE";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.ros_type)||this.ros_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ros_type);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.ros_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsMsgTermType{const value=new Ros2Rmi_msgsMsgTermType();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ros_type=decoded;
}
return value;}
}
export class Ros2Rmi_msgsMsgTermTypeCodec implements VendorCodec<Ros2Rmi_msgsMsgTermType>{
static readonly codecIdentity:string="UWVMROS1_5ddbb483126e0ab5799ed865480da457179d83a271581c1471aec940bedc4535";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Rmi_msgsMsgTermType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsMsgTermType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsMsgTermType>{const out=new VendorDecoded<Ros2Rmi_msgsMsgTermType>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsMsgTermType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsMsgTermType{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsMsgTermType():changetype<Ros2Rmi_msgsMsgTermType>(out.value);}
}
/** Complete original rmi_msgs/srv/AddCall_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddCall_Request {
static readonly maxWireBytes:i32=9992;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9992;
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field params, native scalar rmi_msgs/msg/CallParam. */
params:Array<Ros2Rmi_msgsMsgCallParam>=new Array<Ros2Rmi_msgsMsgCallParam>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>512)return -22;if(4+size>9992-n)return -90;n+=4+size;
if(n>9992)return -90;
}
{
if(this.params.length>16)return -22;
if(4>9992-n)return -90;n+=4;
for(let i=0;i<this.params.length;++i){
const child=this.params[i].wireSize();if(child<0)return child;if(child>9992-n)return -90;n+=child;
if(n>9992)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.program_name);
w.u32(<u32>this.params.length);
for(let i=0;i<this.params.length;++i){
this.params[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddCall_Request{const value=new Ros2Rmi_msgsSrvAddCall_Request();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.params=new Array<Ros2Rmi_msgsMsgCallParam>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Rmi_msgsMsgCallParam.readControl(r);
value.params.push(decoded);
}
}
return value;}
}
export class Ros2Rmi_msgsSrvAddCall_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddCall_Request>{
static readonly codecIdentity:string="UWVMROS1_4d652cfbaef29487a0a688932ee926d7a4026c5e7ac8a0eace583b503c37bcd8";
static readonly maxWireBytes:i32=9992;
encodeChecked(value:Ros2Rmi_msgsSrvAddCall_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddCall_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddCall_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddCall_Request>();if(bytes.length>9992){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddCall_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddCall_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddCall_Request():changetype<Ros2Rmi_msgsSrvAddCall_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddCall_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddCall_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddCall_Response{const value=new Ros2Rmi_msgsSrvAddCall_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddCall_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddCall_Response>{
static readonly codecIdentity:string="UWVMROS1_4f8495dfc3c69ec640444fd24843a7bb3e30008d90945bdd64817d8acb7964e1";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddCall_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddCall_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddCall_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddCall_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddCall_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddCall_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddCall_Response():changetype<Ros2Rmi_msgsSrvAddCall_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddMotionInstruction_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddMotionInstruction_Request {
static readonly maxWireBytes:i32=4014;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4014;
/** Exact native field motion_type, native scalar rmi_msgs/msg/MotionType. */
motion_type:Ros2Rmi_msgsMsgMotionType=new Ros2Rmi_msgsMsgMotionType();
/** Exact native field incremental, native scalar bool. */
incremental:bool=false;
/** Exact native field representation, native scalar rmi_msgs/msg/Representation. */
representation:Ros2Rmi_msgsMsgRepresentation=new Ros2Rmi_msgsMsgRepresentation();
/** Exact native field joint, native scalar rmi_msgs/msg/Joint. */
joint:Ros2Rmi_msgsMsgJoint=new Ros2Rmi_msgsMsgJoint();
/** Exact native field cartesian, native scalar rmi_msgs/msg/Cartesian. */
cartesian:Ros2Rmi_msgsMsgCartesian=new Ros2Rmi_msgsMsgCartesian();
/** Exact native field speed_type, native scalar rmi_msgs/msg/SpeedType. */
speed_type:Ros2Rmi_msgsMsgSpeedType=new Ros2Rmi_msgsMsgSpeedType();
/** Exact native field speed_value, native scalar uint16. */
speed_value:u16=0;
/** Exact native field term_type, native scalar rmi_msgs/msg/TermType. */
term_type:Ros2Rmi_msgsMsgTermType=new Ros2Rmi_msgsMsgTermType();
/** Exact native field term_value, native scalar uint8. */
term_value:u8=0;
/** Exact native field via_representation, native scalar rmi_msgs/msg/Representation. */
via_representation:Ros2Rmi_msgsMsgRepresentation=new Ros2Rmi_msgsMsgRepresentation();
/** Exact native field via_joint, native scalar rmi_msgs/msg/Joint. */
via_joint:Ros2Rmi_msgsMsgJoint=new Ros2Rmi_msgsMsgJoint();
/** Exact native field via_cartesian, native scalar rmi_msgs/msg/Cartesian. */
via_cartesian:Ros2Rmi_msgsMsgCartesian=new Ros2Rmi_msgsMsgCartesian();
/** Exact native field options, native scalar rmi_msgs/msg/MotionInstructionOptions. */
options:Ros2Rmi_msgsMsgMotionInstructionOptions=new Ros2Rmi_msgsMsgMotionInstructionOptions();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.motion_type.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
if(1>4014-n)return -90;n+=1;
if(n>4014)return -90;
}
{
const child=this.representation.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.joint.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.cartesian.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.speed_type.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
if(2>4014-n)return -90;n+=2;
if(n>4014)return -90;
}
{
const child=this.term_type.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
if(1>4014-n)return -90;n+=1;
if(n>4014)return -90;
}
{
const child=this.via_representation.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.via_joint.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.via_cartesian.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
{
const child=this.options.wireSize();if(child<0)return child;if(child>4014-n)return -90;n+=child;
if(n>4014)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.motion_type.writeControl(w);
w.u8(this.incremental?1:0);
this.representation.writeControl(w);
this.joint.writeControl(w);
this.cartesian.writeControl(w);
this.speed_type.writeControl(w);
w.integer(<u64>this.speed_value,2);
this.term_type.writeControl(w);
w.integer(<u64>this.term_value,1);
this.via_representation.writeControl(w);
this.via_joint.writeControl(w);
this.via_cartesian.writeControl(w);
this.options.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddMotionInstruction_Request{const value=new Ros2Rmi_msgsSrvAddMotionInstruction_Request();
{
const decoded=Ros2Rmi_msgsMsgMotionType.readControl(r);
value.motion_type=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.incremental=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgRepresentation.readControl(r);
value.representation=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgJoint.readControl(r);
value.joint=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCartesian.readControl(r);
value.cartesian=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgSpeedType.readControl(r);
value.speed_type=decoded;
}
{
const decoded=<u16>r.integer(2);
value.speed_value=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgTermType.readControl(r);
value.term_type=decoded;
}
{
const decoded=<u8>r.integer(1);
value.term_value=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgRepresentation.readControl(r);
value.via_representation=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgJoint.readControl(r);
value.via_joint=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCartesian.readControl(r);
value.via_cartesian=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgMotionInstructionOptions.readControl(r);
value.options=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddMotionInstruction_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddMotionInstruction_Request>{
static readonly codecIdentity:string="UWVMROS1_3f22c49cb2d32e6a8f3954f3d3cbf0021c87e0363b1f2ded4ac20a97a5b59391";
static readonly maxWireBytes:i32=4014;
encodeChecked(value:Ros2Rmi_msgsSrvAddMotionInstruction_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddMotionInstruction_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddMotionInstruction_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddMotionInstruction_Request>();if(bytes.length>4014){out.status=90;return out;}if(bytes.length<238){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddMotionInstruction_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddMotionInstruction_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddMotionInstruction_Request():changetype<Ros2Rmi_msgsSrvAddMotionInstruction_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddMotionInstruction_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddMotionInstruction_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddMotionInstruction_Response{const value=new Ros2Rmi_msgsSrvAddMotionInstruction_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddMotionInstruction_Response>{
static readonly codecIdentity:string="UWVMROS1_79fee3e965fda63c3a53ad2178ff48614878acc9f6af74cc27a8c2812b68ac6b";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddMotionInstruction_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddMotionInstruction_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddMotionInstruction_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddMotionInstruction_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddMotionInstruction_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddMotionInstruction_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddMotionInstruction_Response():changetype<Ros2Rmi_msgsSrvAddMotionInstruction_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetPayload_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetPayload_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field schedule_number, native scalar uint8. */
schedule_number:u8=0;
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
w.integer(<u64>this.schedule_number,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetPayload_Request{const value=new Ros2Rmi_msgsSrvAddSetPayload_Request();
{
const decoded=<u8>r.integer(1);
value.schedule_number=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetPayload_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetPayload_Request>{
static readonly codecIdentity:string="UWVMROS1_45da9f81e8c26f49490a4e6eba5c8be0ee67285124540eb1c3e1f2c3e0fd821b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetPayload_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetPayload_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetPayload_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetPayload_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetPayload_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetPayload_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetPayload_Request():changetype<Ros2Rmi_msgsSrvAddSetPayload_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetPayload_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetPayload_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetPayload_Response{const value=new Ros2Rmi_msgsSrvAddSetPayload_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetPayload_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetPayload_Response>{
static readonly codecIdentity:string="UWVMROS1_bfc5bc7b92f292c2980496cb18fbe953ea1b49d45f7950f0125bbbf880bdf72a";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetPayload_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetPayload_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetPayload_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetPayload_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetPayload_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetPayload_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetPayload_Response():changetype<Ros2Rmi_msgsSrvAddSetPayload_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetUFrame_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetUFrame_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field frame_number, native scalar uint8. */
frame_number:u8=0;
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
w.integer(<u64>this.frame_number,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetUFrame_Request{const value=new Ros2Rmi_msgsSrvAddSetUFrame_Request();
{
const decoded=<u8>r.integer(1);
value.frame_number=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetUFrame_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetUFrame_Request>{
static readonly codecIdentity:string="UWVMROS1_64c83dd2be89b4b6dcf3b3c2ea70184971e6a359914ff1a5cd26d4b114d35475";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetUFrame_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetUFrame_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetUFrame_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetUFrame_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetUFrame_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetUFrame_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetUFrame_Request():changetype<Ros2Rmi_msgsSrvAddSetUFrame_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetUFrame_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetUFrame_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetUFrame_Response{const value=new Ros2Rmi_msgsSrvAddSetUFrame_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetUFrame_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetUFrame_Response>{
static readonly codecIdentity:string="UWVMROS1_e344ef6efacc8d569111d1c6ef4028e1a5bb54af49d3e8f486ae89b83c674647";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetUFrame_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetUFrame_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetUFrame_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetUFrame_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetUFrame_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetUFrame_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetUFrame_Response():changetype<Ros2Rmi_msgsSrvAddSetUFrame_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetUTool_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetUTool_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field tool_number, native scalar uint8. */
tool_number:u8=0;
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
w.integer(<u64>this.tool_number,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetUTool_Request{const value=new Ros2Rmi_msgsSrvAddSetUTool_Request();
{
const decoded=<u8>r.integer(1);
value.tool_number=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetUTool_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetUTool_Request>{
static readonly codecIdentity:string="UWVMROS1_d1327022326cd41caed6001449517a41f589e148c38d6db30923922175f05c46";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetUTool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetUTool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetUTool_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetUTool_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetUTool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetUTool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetUTool_Request():changetype<Ros2Rmi_msgsSrvAddSetUTool_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddSetUTool_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddSetUTool_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddSetUTool_Response{const value=new Ros2Rmi_msgsSrvAddSetUTool_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddSetUTool_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddSetUTool_Response>{
static readonly codecIdentity:string="UWVMROS1_dd4c36458027b87559200cf5f6aa72a60af2d103f663f2fa9daca47ce8020ff6";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddSetUTool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddSetUTool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddSetUTool_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddSetUTool_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddSetUTool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddSetUTool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddSetUTool_Response():changetype<Ros2Rmi_msgsSrvAddSetUTool_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddWaitDIN_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddWaitDIN_Request {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field port_number, native scalar uint16. */
port_number:u16=0;
/** Exact native field port_value, native scalar string. */
port_value:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.port_value)||this.port_value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.port_value);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.port_number,2);
w.text(this.port_value);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddWaitDIN_Request{const value=new Ros2Rmi_msgsSrvAddWaitDIN_Request();
{
const decoded=<u16>r.integer(2);
value.port_number=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.port_value=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddWaitDIN_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddWaitDIN_Request>{
static readonly codecIdentity:string="UWVMROS1_c32214f1ff1b0b6980583fc3088f412e856efb5eaa9c336cf798ef8ad85c61ab";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Rmi_msgsSrvAddWaitDIN_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddWaitDIN_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddWaitDIN_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddWaitDIN_Request>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddWaitDIN_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddWaitDIN_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddWaitDIN_Request():changetype<Ros2Rmi_msgsSrvAddWaitDIN_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddWaitDIN_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddWaitDIN_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddWaitDIN_Response{const value=new Ros2Rmi_msgsSrvAddWaitDIN_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddWaitDIN_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddWaitDIN_Response>{
static readonly codecIdentity:string="UWVMROS1_7d3933f573619ba49f19df7c11f1d73ef211c17581f660f8c0fbcc152cd519d4";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddWaitDIN_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddWaitDIN_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddWaitDIN_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddWaitDIN_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddWaitDIN_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddWaitDIN_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddWaitDIN_Response():changetype<Ros2Rmi_msgsSrvAddWaitDIN_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/AddWaitTime_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddWaitTime_Request {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field time, native scalar float32. */
time:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.time))return -22;
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.time),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddWaitTime_Request{const value=new Ros2Rmi_msgsSrvAddWaitTime_Request();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.time=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddWaitTime_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvAddWaitTime_Request>{
static readonly codecIdentity:string="UWVMROS1_8487f4b7dcfb044c72f7fb62a31ffe9cdc4321a9ebdaa0550a363e32acbadc46";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Rmi_msgsSrvAddWaitTime_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddWaitTime_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddWaitTime_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddWaitTime_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddWaitTime_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddWaitTime_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddWaitTime_Request():changetype<Ros2Rmi_msgsSrvAddWaitTime_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/AddWaitTime_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvAddWaitTime_Response {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field result, native scalar int32. */
result:i32=0;
/** Exact native field sequence_id, native scalar int32. */
sequence_id:i32=0;
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
w.integer(<u64>this.result,4);
w.integer(<u64>this.sequence_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvAddWaitTime_Response{const value=new Ros2Rmi_msgsSrvAddWaitTime_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sequence_id=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvAddWaitTime_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvAddWaitTime_Response>{
static readonly codecIdentity:string="UWVMROS1_f9578c08b971f32f91d1c94b8713cafe648e25147394e20c85bb335254fa20c8";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Rmi_msgsSrvAddWaitTime_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvAddWaitTime_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvAddWaitTime_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvAddWaitTime_Response>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvAddWaitTime_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvAddWaitTime_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvAddWaitTime_Response():changetype<Ros2Rmi_msgsSrvAddWaitTime_Response>(out.value);}
}
/** Complete original rmi_msgs/srv/CallCommand_Request; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvCallCommand_Request {
static readonly maxWireBytes:i32=1104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1104;
/** Exact native field command, native scalar rmi_msgs/msg/Command. */
command:Ros2Rmi_msgsMsgCommand=new Ros2Rmi_msgsMsgCommand();
/** Exact native field options, native scalar rmi_msgs/msg/CommandOptions. */
options:Ros2Rmi_msgsMsgCommandOptions=new Ros2Rmi_msgsMsgCommandOptions();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.command.wireSize();if(child<0)return child;if(child>1104-n)return -90;n+=child;
if(n>1104)return -90;
}
{
const child=this.options.wireSize();if(child<0)return child;if(child>1104-n)return -90;n+=child;
if(n>1104)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.command.writeControl(w);
this.options.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvCallCommand_Request{const value=new Ros2Rmi_msgsSrvCallCommand_Request();
{
const decoded=Ros2Rmi_msgsMsgCommand.readControl(r);
value.command=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCommandOptions.readControl(r);
value.options=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvCallCommand_RequestCodec implements VendorCodec<Ros2Rmi_msgsSrvCallCommand_Request>{
static readonly codecIdentity:string="UWVMROS1_d1f890a2beec0ca9294ff4f641d1738855d576a917ed05eb34517886c73f381c";
static readonly maxWireBytes:i32=1104;
encodeChecked(value:Ros2Rmi_msgsSrvCallCommand_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvCallCommand_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvCallCommand_Request>{const out=new VendorDecoded<Ros2Rmi_msgsSrvCallCommand_Request>();if(bytes.length>1104){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvCallCommand_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvCallCommand_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvCallCommand_Request():changetype<Ros2Rmi_msgsSrvCallCommand_Request>(out.value);}
}
/** Complete original rmi_msgs/srv/CallCommand_Response; spellings are published in generation proof. */
export class Ros2Rmi_msgsSrvCallCommand_Response {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field result, native scalar int32. */
result:i32=0;
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
w.integer(<u64>this.result,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Rmi_msgsSrvCallCommand_Response{const value=new Ros2Rmi_msgsSrvCallCommand_Response();
{
const decoded=<i32>r.integer(4);
value.result=decoded;
}
return value;}
}
export class Ros2Rmi_msgsSrvCallCommand_ResponseCodec implements VendorCodec<Ros2Rmi_msgsSrvCallCommand_Response>{
static readonly codecIdentity:string="UWVMROS1_64d92e6fadc651073469318ad15d2e98ec3d059c629838cefc5820b17ab6f1bc";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Rmi_msgsSrvCallCommand_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Rmi_msgsSrvCallCommand_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Rmi_msgsSrvCallCommand_Response>{const out=new VendorDecoded<Ros2Rmi_msgsSrvCallCommand_Response>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Rmi_msgsSrvCallCommand_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Rmi_msgsSrvCallCommand_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Rmi_msgsSrvCallCommand_Response():changetype<Ros2Rmi_msgsSrvCallCommand_Response>(out.value);}
}
/** Complete original sensor_msgs/msg/JointState; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgJointState {
static readonly maxWireBytes:i32=1196;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1196;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field name, native scalar string. */
name:Array<string>=new Array<string>();
/** Exact native field position, native scalar float64. */
position:Array<f64>=new Array<f64>();
/** Exact native field velocity, native scalar float64. */
velocity:Array<f64>=new Array<f64>();
/** Exact native field effort, native scalar float64. */
effort:Array<f64>=new Array<f64>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1196-n)return -90;n+=child;
if(n>1196)return -90;
}
{
if(this.name.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>64)return -22;if(4+size>1196-n)return -90;n+=4+size;
if(n>1196)return -90;
}
}
{
if(this.position.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
}
}
{
if(this.velocity.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
}
}
{
if(this.effort.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
}
}
if((this.name.length!=0&&this.position.length!=0&&this.name.length!=this.position.length)||(this.name.length!=0&&this.velocity.length!=0&&this.name.length!=this.velocity.length)||(this.name.length!=0&&this.effort.length!=0&&this.name.length!=this.effort.length)||(this.position.length!=0&&this.velocity.length!=0&&this.position.length!=this.velocity.length)||(this.position.length!=0&&this.effort.length!=0&&this.position.length!=this.effort.length)||(this.velocity.length!=0&&this.effort.length!=0&&this.velocity.length!=this.effort.length))return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.name.length);
for(let i=0;i<this.name.length;++i){
w.text(this.name[i]);
}
w.u32(<u32>this.position.length);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u64>(this.position[i]),8);
}
w.u32(<u32>this.velocity.length);
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u64>(this.velocity[i]),8);
}
w.u32(<u32>this.effort.length);
for(let i=0;i<this.effort.length;++i){
w.integer(<u64>reinterpret<u64>(this.effort[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgJointState{const value=new Ros2Sensor_msgsMsgJointState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.name=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
if((value.name.length!=0&&value.position.length!=0&&value.name.length!=value.position.length)||(value.name.length!=0&&value.velocity.length!=0&&value.name.length!=value.velocity.length)||(value.name.length!=0&&value.effort.length!=0&&value.name.length!=value.effort.length)||(value.position.length!=0&&value.velocity.length!=0&&value.position.length!=value.velocity.length)||(value.position.length!=0&&value.effort.length!=0&&value.position.length!=value.effort.length)||(value.velocity.length!=0&&value.effort.length!=0&&value.velocity.length!=value.effort.length))r.valid=false;
return value;}
}
export class Ros2Sensor_msgsMsgJointStateCodec implements VendorCodec<Ros2Sensor_msgsMsgJointState>{
static readonly codecIdentity:string="UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884";
static readonly maxWireBytes:i32=1196;
encodeChecked(value:Ros2Sensor_msgsMsgJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgJointState>{const out=new VendorDecoded<Ros2Sensor_msgsMsgJointState>();if(bytes.length>1196){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgJointState():changetype<Ros2Sensor_msgsMsgJointState>(out.value);}
}
/** Complete original std_srvs/srv/Trigger_Request; spellings are published in generation proof. */
export class Ros2Std_srvsSrvTrigger_Request {
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
static readControl(r:Reader):Ros2Std_srvsSrvTrigger_Request{const value=new Ros2Std_srvsSrvTrigger_Request();
return value;}
}
export class Ros2Std_srvsSrvTrigger_RequestCodec implements VendorCodec<Ros2Std_srvsSrvTrigger_Request>{
static readonly codecIdentity:string="UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Std_srvsSrvTrigger_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvTrigger_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvTrigger_Request>{const out=new VendorDecoded<Ros2Std_srvsSrvTrigger_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvTrigger_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvTrigger_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvTrigger_Request():changetype<Ros2Std_srvsSrvTrigger_Request>(out.value);}
}
/** Complete original std_srvs/srv/Trigger_Response; spellings are published in generation proof. */
export class Ros2Std_srvsSrvTrigger_Response {
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>517-n)return -90;n+=1;
if(n>517)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>517-n)return -90;n+=4+size;
if(n>517)return -90;
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
static readControl(r:Reader):Ros2Std_srvsSrvTrigger_Response{const value=new Ros2Std_srvsSrvTrigger_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Std_srvsSrvTrigger_ResponseCodec implements VendorCodec<Ros2Std_srvsSrvTrigger_Response>{
static readonly codecIdentity:string="UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Std_srvsSrvTrigger_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvTrigger_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvTrigger_Response>{const out=new VendorDecoded<Ros2Std_srvsSrvTrigger_Response>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvTrigger_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvTrigger_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvTrigger_Response():changetype<Ros2Std_srvsSrvTrigger_Response>(out.value);}
}
