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
/** Complete original kuka_driver_interfaces/msg/FRIState; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgFRIState {
static readonly maxWireBytes:i32=40;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=40;
/** Exact native field session_state, native scalar int32. */
session_state:i32=0;
/** Exact native field connection_quality, native scalar int32. */
connection_quality:i32=0;
/** Exact native field safety_state, native scalar int32. */
safety_state:i32=0;
/** Exact native field command_mode, native scalar int32. */
command_mode:i32=0;
/** Exact native field control_mode, native scalar int32. */
control_mode:i32=0;
/** Exact native field operation_mode, native scalar int32. */
operation_mode:i32=0;
/** Exact native field drive_state, native scalar int32. */
drive_state:i32=0;
/** Exact native field overlay_type, native scalar int32. */
overlay_type:i32=0;
/** Exact native field tracking_performance, native scalar float64. */
tracking_performance:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(!isFinite(this.tracking_performance))return -22;
if(8>40-n)return -90;n+=8;
if(n>40)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.session_state,4);
w.integer(<u64>this.connection_quality,4);
w.integer(<u64>this.safety_state,4);
w.integer(<u64>this.command_mode,4);
w.integer(<u64>this.control_mode,4);
w.integer(<u64>this.operation_mode,4);
w.integer(<u64>this.drive_state,4);
w.integer(<u64>this.overlay_type,4);
w.integer(<u64>reinterpret<u64>(this.tracking_performance),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgFRIState{const value=new Ros2Kuka_driver_interfacesMsgFRIState();
{
const decoded=<i32>r.integer(4);
value.session_state=decoded;
}
{
const decoded=<i32>r.integer(4);
value.connection_quality=decoded;
}
{
const decoded=<i32>r.integer(4);
value.safety_state=decoded;
}
{
const decoded=<i32>r.integer(4);
value.command_mode=decoded;
}
{
const decoded=<i32>r.integer(4);
value.control_mode=decoded;
}
{
const decoded=<i32>r.integer(4);
value.operation_mode=decoded;
}
{
const decoded=<i32>r.integer(4);
value.drive_state=decoded;
}
{
const decoded=<i32>r.integer(4);
value.overlay_type=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tracking_performance=decoded;
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgFRIStateCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgFRIState>{
static readonly codecIdentity:string="UWVMROS1_27be8431e4a8cda3bb4d6d5a8a69fd86a0d07a1d920799c232d63a4941ee6716";
static readonly maxWireBytes:i32=40;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgFRIState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgFRIState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgFRIState>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgFRIState>();if(bytes.length>40){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgFRIState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgFRIState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgFRIState():changetype<Ros2Kuka_driver_interfacesMsgFRIState>(out.value);}
}
/** Complete original kuka_driver_interfaces/msg/FRIStateArray; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgFRIStateArray {
static readonly maxWireBytes:i32=1736;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1736;
/** Exact native field robot_names, native scalar string. */
robot_names:Array<string>=new Array<string>();
/** Exact native field states, native scalar kuka_driver_interfaces/msg/FRIState. */
states:Array<Ros2Kuka_driver_interfacesMsgFRIState>=new Array<Ros2Kuka_driver_interfacesMsgFRIState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.robot_names.length>16)return -22;
if(4>1736-n)return -90;n+=4;
for(let i=0;i<this.robot_names.length;++i){
if(!validUnicode(this.robot_names[i])||this.robot_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.robot_names[i]);if(size>64)return -22;if(4+size>1736-n)return -90;n+=4+size;
if(n>1736)return -90;
}
}
{
if(this.states.length>16)return -22;
if(4>1736-n)return -90;n+=4;
for(let i=0;i<this.states.length;++i){
const child=this.states[i].wireSize();if(child<0)return child;if(child>1736-n)return -90;n+=child;
if(n>1736)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.robot_names.length);
for(let i=0;i<this.robot_names.length;++i){
w.text(this.robot_names[i]);
}
w.u32(<u32>this.states.length);
for(let i=0;i<this.states.length;++i){
this.states[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgFRIStateArray{const value=new Ros2Kuka_driver_interfacesMsgFRIStateArray();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.robot_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.robot_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.states=new Array<Ros2Kuka_driver_interfacesMsgFRIState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Kuka_driver_interfacesMsgFRIState.readControl(r);
value.states.push(decoded);
}
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgFRIStateArrayCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgFRIStateArray>{
static readonly codecIdentity:string="UWVMROS1_4312aa4103fd0c92fe7cd9d1201c40e327b05812cdc60130d11b8c2e0eb47449";
static readonly maxWireBytes:i32=1736;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgFRIStateArray):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgFRIStateArray):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgFRIStateArray>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgFRIStateArray>();if(bytes.length>1736){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgFRIStateArray.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgFRIStateArray{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgFRIStateArray():changetype<Ros2Kuka_driver_interfacesMsgFRIStateArray>(out.value);}
}
/** Complete original kuka_driver_interfaces/msg/FriConfiguration; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgFriConfiguration {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field receive_multiplier, native scalar int32. */
receive_multiplier:i32=0;
/** Exact native field send_period_ms, native scalar int32. */
send_period_ms:i32=0;
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
w.integer(<u64>this.receive_multiplier,4);
w.integer(<u64>this.send_period_ms,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgFriConfiguration{const value=new Ros2Kuka_driver_interfacesMsgFriConfiguration();
{
const decoded=<i32>r.integer(4);
value.receive_multiplier=decoded;
}
{
const decoded=<i32>r.integer(4);
value.send_period_ms=decoded;
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgFriConfigurationCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgFriConfiguration>{
static readonly codecIdentity:string="UWVMROS1_a60335766ad606adc5699c833cc6bc43adc3067691bcdbf587bbfb56e8a7127e";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgFriConfiguration):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgFriConfiguration):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgFriConfiguration>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgFriConfiguration>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgFriConfiguration.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgFriConfiguration{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgFriConfiguration():changetype<Ros2Kuka_driver_interfacesMsgFriConfiguration>(out.value);}
}
/** Complete original kuka_driver_interfaces/msg/HardwareEvent; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgHardwareEvent {
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field robot_name, native scalar string. */
robot_name:string="";
/** Exact native field event, native scalar uint8. */
event:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.robot_name)||this.robot_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.robot_name);if(size>512)return -22;if(4+size>517-n)return -90;n+=4+size;
if(n>517)return -90;
}
{
if(1>517-n)return -90;n+=1;
if(n>517)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.robot_name);
w.integer(<u64>this.event,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgHardwareEvent{const value=new Ros2Kuka_driver_interfacesMsgHardwareEvent();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.robot_name=decoded;
}
{
const decoded=<u8>r.integer(1);
value.event=decoded;
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgHardwareEventCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgHardwareEvent>{
static readonly codecIdentity:string="UWVMROS1_cb542e9311957712257c6d8ae273b3d2d2339760978e11e3798982c84fbc5e68";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgHardwareEvent):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgHardwareEvent):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgHardwareEvent>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgHardwareEvent>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgHardwareEvent.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgHardwareEvent{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgHardwareEvent():changetype<Ros2Kuka_driver_interfacesMsgHardwareEvent>(out.value);}
}
/** Complete original kuka_driver_interfaces/msg/KssStatus; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgKssStatus {
static readonly UNSPECIFIED:u8=0;
static readonly JOINT_POSITION_CONTROL:u8=1;
static readonly CARTESIAN_POSITION_CONTROL:u8=5;
static readonly RSI_4MS:u8=1;
static readonly RSI_12MS:u8=2;
static readonly T1:u8=1;
static readonly T2:u8=2;
static readonly AUT:u8=3;
static readonly EXT:u8=4;
static readonly maxWireBytes:i32=9;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9;
/** Exact native field control_mode, native scalar uint8. */
control_mode:u8=0;
/** Exact native field cycle_time, native scalar uint8. */
cycle_time:u8=0;
/** Exact native field drives_powered, native scalar bool. */
drives_powered:bool=false;
/** Exact native field emergency_stop, native scalar bool. */
emergency_stop:bool=false;
/** Exact native field guard_stop, native scalar bool. */
guard_stop:bool=false;
/** Exact native field in_motion, native scalar bool. */
in_motion:bool=false;
/** Exact native field motion_possible, native scalar bool. */
motion_possible:bool=false;
/** Exact native field operation_mode, native scalar uint8. */
operation_mode:u8=0;
/** Exact native field robot_stopped, native scalar bool. */
robot_stopped:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.control_mode,1);
w.integer(<u64>this.cycle_time,1);
w.u8(this.drives_powered?1:0);
w.u8(this.emergency_stop?1:0);
w.u8(this.guard_stop?1:0);
w.u8(this.in_motion?1:0);
w.u8(this.motion_possible?1:0);
w.integer(<u64>this.operation_mode,1);
w.u8(this.robot_stopped?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgKssStatus{const value=new Ros2Kuka_driver_interfacesMsgKssStatus();
{
const decoded=<u8>r.integer(1);
value.control_mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.cycle_time=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.drives_powered=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.emergency_stop=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.guard_stop=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.in_motion=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.motion_possible=decoded;
}
{
const decoded=<u8>r.integer(1);
value.operation_mode=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.robot_stopped=decoded;
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgKssStatusCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgKssStatus>{
static readonly codecIdentity:string="UWVMROS1_b28bc2f19a0f410f0cd1d5b1d59daf1d09acb3e8312b4267f622a4d119f16b82";
static readonly maxWireBytes:i32=9;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgKssStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgKssStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgKssStatus>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgKssStatus>();if(bytes.length>9){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgKssStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgKssStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgKssStatus():changetype<Ros2Kuka_driver_interfacesMsgKssStatus>(out.value);}
}
/** Complete original kuka_driver_interfaces/msg/KssStatusArray; spellings are published in generation proof. */
export class Ros2Kuka_driver_interfacesMsgKssStatusArray {
static readonly maxWireBytes:i32=1240;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1240;
/** Exact native field robot_names, native scalar string. */
robot_names:Array<string>=new Array<string>();
/** Exact native field statuses, native scalar kuka_driver_interfaces/msg/KssStatus. */
statuses:Array<Ros2Kuka_driver_interfacesMsgKssStatus>=new Array<Ros2Kuka_driver_interfacesMsgKssStatus>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.robot_names.length>16)return -22;
if(4>1240-n)return -90;n+=4;
for(let i=0;i<this.robot_names.length;++i){
if(!validUnicode(this.robot_names[i])||this.robot_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.robot_names[i]);if(size>64)return -22;if(4+size>1240-n)return -90;n+=4+size;
if(n>1240)return -90;
}
}
{
if(this.statuses.length>16)return -22;
if(4>1240-n)return -90;n+=4;
for(let i=0;i<this.statuses.length;++i){
const child=this.statuses[i].wireSize();if(child<0)return child;if(child>1240-n)return -90;n+=child;
if(n>1240)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.robot_names.length);
for(let i=0;i<this.robot_names.length;++i){
w.text(this.robot_names[i]);
}
w.u32(<u32>this.statuses.length);
for(let i=0;i<this.statuses.length;++i){
this.statuses[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Kuka_driver_interfacesMsgKssStatusArray{const value=new Ros2Kuka_driver_interfacesMsgKssStatusArray();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.robot_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.robot_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.statuses=new Array<Ros2Kuka_driver_interfacesMsgKssStatus>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Kuka_driver_interfacesMsgKssStatus.readControl(r);
value.statuses.push(decoded);
}
}
return value;}
}
export class Ros2Kuka_driver_interfacesMsgKssStatusArrayCodec implements VendorCodec<Ros2Kuka_driver_interfacesMsgKssStatusArray>{
static readonly codecIdentity:string="UWVMROS1_3c8e93b369eefea89c0aa56ab657721edd041ac66e18cfbf4fc4201df60dbebd";
static readonly maxWireBytes:i32=1240;
encodeChecked(value:Ros2Kuka_driver_interfacesMsgKssStatusArray):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Kuka_driver_interfacesMsgKssStatusArray):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Kuka_driver_interfacesMsgKssStatusArray>{const out=new VendorDecoded<Ros2Kuka_driver_interfacesMsgKssStatusArray>();if(bytes.length>1240){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Kuka_driver_interfacesMsgKssStatusArray.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Kuka_driver_interfacesMsgKssStatusArray{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Kuka_driver_interfacesMsgKssStatusArray():changetype<Ros2Kuka_driver_interfacesMsgKssStatusArray>(out.value);}
}
/** Complete original lifecycle_msgs/msg/State; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsMsgState {
static readonly PRIMARY_STATE_UNKNOWN:u8=0;
static readonly PRIMARY_STATE_UNCONFIGURED:u8=1;
static readonly PRIMARY_STATE_INACTIVE:u8=2;
static readonly PRIMARY_STATE_ACTIVE:u8=3;
static readonly PRIMARY_STATE_FINALIZED:u8=4;
static readonly TRANSITION_STATE_CONFIGURING:u8=10;
static readonly TRANSITION_STATE_CLEANINGUP:u8=11;
static readonly TRANSITION_STATE_SHUTTINGDOWN:u8=12;
static readonly TRANSITION_STATE_ACTIVATING:u8=13;
static readonly TRANSITION_STATE_DEACTIVATING:u8=14;
static readonly TRANSITION_STATE_ERRORPROCESSING:u8=15;
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field id, native scalar uint8. */
id:u8=0;
/** Exact native field label, native scalar string. */
label:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>517-n)return -90;n+=1;
if(n>517)return -90;
}
{
if(!validUnicode(this.label)||this.label.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.label);if(size>512)return -22;if(4+size>517-n)return -90;n+=4+size;
if(n>517)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.id,1);
w.text(this.label);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Lifecycle_msgsMsgState{const value=new Ros2Lifecycle_msgsMsgState();
{
const decoded=<u8>r.integer(1);
value.id=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.label=decoded;
}
return value;}
}
export class Ros2Lifecycle_msgsMsgStateCodec implements VendorCodec<Ros2Lifecycle_msgsMsgState>{
static readonly codecIdentity:string="UWVMROS1_ebd49e5f87ddaa204cc4072ceef9840676c6f65f67448a027e26570dae0ded94";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Lifecycle_msgsMsgState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsMsgState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsMsgState>{const out=new VendorDecoded<Ros2Lifecycle_msgsMsgState>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsMsgState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsMsgState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsMsgState():changetype<Ros2Lifecycle_msgsMsgState>(out.value);}
}
/** Complete original lifecycle_msgs/msg/Transition; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsMsgTransition {
static readonly TRANSITION_CREATE:u8=0;
static readonly TRANSITION_CONFIGURE:u8=1;
static readonly TRANSITION_CLEANUP:u8=2;
static readonly TRANSITION_ACTIVATE:u8=3;
static readonly TRANSITION_DEACTIVATE:u8=4;
static readonly TRANSITION_UNCONFIGURED_SHUTDOWN:u8=5;
static readonly TRANSITION_INACTIVE_SHUTDOWN:u8=6;
static readonly TRANSITION_ACTIVE_SHUTDOWN:u8=7;
static readonly TRANSITION_DESTROY:u8=8;
static readonly TRANSITION_ON_CONFIGURE_SUCCESS:u8=10;
static readonly TRANSITION_ON_CONFIGURE_FAILURE:u8=11;
static readonly TRANSITION_ON_CONFIGURE_ERROR:u8=12;
static readonly TRANSITION_ON_CLEANUP_SUCCESS:u8=20;
static readonly TRANSITION_ON_CLEANUP_FAILURE:u8=21;
static readonly TRANSITION_ON_CLEANUP_ERROR:u8=22;
static readonly TRANSITION_ON_ACTIVATE_SUCCESS:u8=30;
static readonly TRANSITION_ON_ACTIVATE_FAILURE:u8=31;
static readonly TRANSITION_ON_ACTIVATE_ERROR:u8=32;
static readonly TRANSITION_ON_DEACTIVATE_SUCCESS:u8=40;
static readonly TRANSITION_ON_DEACTIVATE_FAILURE:u8=41;
static readonly TRANSITION_ON_DEACTIVATE_ERROR:u8=42;
static readonly TRANSITION_ON_SHUTDOWN_SUCCESS:u8=50;
static readonly TRANSITION_ON_SHUTDOWN_FAILURE:u8=51;
static readonly TRANSITION_ON_SHUTDOWN_ERROR:u8=52;
static readonly TRANSITION_ON_ERROR_SUCCESS:u8=60;
static readonly TRANSITION_ON_ERROR_FAILURE:u8=61;
static readonly TRANSITION_ON_ERROR_ERROR:u8=62;
static readonly TRANSITION_CALLBACK_SUCCESS:u8=97;
static readonly TRANSITION_CALLBACK_FAILURE:u8=98;
static readonly TRANSITION_CALLBACK_ERROR:u8=99;
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field id, native scalar uint8. */
id:u8=0;
/** Exact native field label, native scalar string. */
label:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>517-n)return -90;n+=1;
if(n>517)return -90;
}
{
if(!validUnicode(this.label)||this.label.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.label);if(size>512)return -22;if(4+size>517-n)return -90;n+=4+size;
if(n>517)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.id,1);
w.text(this.label);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Lifecycle_msgsMsgTransition{const value=new Ros2Lifecycle_msgsMsgTransition();
{
const decoded=<u8>r.integer(1);
value.id=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.label=decoded;
}
return value;}
}
export class Ros2Lifecycle_msgsMsgTransitionCodec implements VendorCodec<Ros2Lifecycle_msgsMsgTransition>{
static readonly codecIdentity:string="UWVMROS1_1f9bb864622854b446a37cf3c6e29e9cf41f8efa37375c5a56205b4c02ed10cb";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Lifecycle_msgsMsgTransition):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsMsgTransition):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsMsgTransition>{const out=new VendorDecoded<Ros2Lifecycle_msgsMsgTransition>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsMsgTransition.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsMsgTransition{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsMsgTransition():changetype<Ros2Lifecycle_msgsMsgTransition>(out.value);}
}
/** Complete original lifecycle_msgs/srv/ChangeState_Request; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsSrvChangeState_Request {
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field transition, native scalar lifecycle_msgs/msg/Transition. */
transition:Ros2Lifecycle_msgsMsgTransition=new Ros2Lifecycle_msgsMsgTransition();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.transition.wireSize();if(child<0)return child;if(child>517-n)return -90;n+=child;
if(n>517)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.transition.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Lifecycle_msgsSrvChangeState_Request{const value=new Ros2Lifecycle_msgsSrvChangeState_Request();
{
const decoded=Ros2Lifecycle_msgsMsgTransition.readControl(r);
value.transition=decoded;
}
return value;}
}
export class Ros2Lifecycle_msgsSrvChangeState_RequestCodec implements VendorCodec<Ros2Lifecycle_msgsSrvChangeState_Request>{
static readonly codecIdentity:string="UWVMROS1_b04270f03616b8e581036d327d18bde62d38627ae8acf88fb77d0ed7b3b3d3d1";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Lifecycle_msgsSrvChangeState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsSrvChangeState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsSrvChangeState_Request>{const out=new VendorDecoded<Ros2Lifecycle_msgsSrvChangeState_Request>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsSrvChangeState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsSrvChangeState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsSrvChangeState_Request():changetype<Ros2Lifecycle_msgsSrvChangeState_Request>(out.value);}
}
/** Complete original lifecycle_msgs/srv/ChangeState_Response; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsSrvChangeState_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
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
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Lifecycle_msgsSrvChangeState_Response{const value=new Ros2Lifecycle_msgsSrvChangeState_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Lifecycle_msgsSrvChangeState_ResponseCodec implements VendorCodec<Ros2Lifecycle_msgsSrvChangeState_Response>{
static readonly codecIdentity:string="UWVMROS1_d2dc541d6995976a2a1a2c03d499aa9dccaf487736b11704d778c86fddaf5032";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Lifecycle_msgsSrvChangeState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsSrvChangeState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsSrvChangeState_Response>{const out=new VendorDecoded<Ros2Lifecycle_msgsSrvChangeState_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsSrvChangeState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsSrvChangeState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsSrvChangeState_Response():changetype<Ros2Lifecycle_msgsSrvChangeState_Response>(out.value);}
}
/** Complete original lifecycle_msgs/srv/GetState_Request; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsSrvGetState_Request {
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
static readControl(r:Reader):Ros2Lifecycle_msgsSrvGetState_Request{const value=new Ros2Lifecycle_msgsSrvGetState_Request();
return value;}
}
export class Ros2Lifecycle_msgsSrvGetState_RequestCodec implements VendorCodec<Ros2Lifecycle_msgsSrvGetState_Request>{
static readonly codecIdentity:string="UWVMROS1_86a3d813de32971a271e062acfaf201eb718aff8d68a4e343d6fceaae2b5098d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Lifecycle_msgsSrvGetState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsSrvGetState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsSrvGetState_Request>{const out=new VendorDecoded<Ros2Lifecycle_msgsSrvGetState_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsSrvGetState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsSrvGetState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsSrvGetState_Request():changetype<Ros2Lifecycle_msgsSrvGetState_Request>(out.value);}
}
/** Complete original lifecycle_msgs/srv/GetState_Response; spellings are published in generation proof. */
export class Ros2Lifecycle_msgsSrvGetState_Response {
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field current_state, native scalar lifecycle_msgs/msg/State. */
current_state:Ros2Lifecycle_msgsMsgState=new Ros2Lifecycle_msgsMsgState();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.current_state.wireSize();if(child<0)return child;if(child>517-n)return -90;n+=child;
if(n>517)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.current_state.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Lifecycle_msgsSrvGetState_Response{const value=new Ros2Lifecycle_msgsSrvGetState_Response();
{
const decoded=Ros2Lifecycle_msgsMsgState.readControl(r);
value.current_state=decoded;
}
return value;}
}
export class Ros2Lifecycle_msgsSrvGetState_ResponseCodec implements VendorCodec<Ros2Lifecycle_msgsSrvGetState_Response>{
static readonly codecIdentity:string="UWVMROS1_697418ec4095e98c855b1f9d238a3bd74a59ccbbc3eb31981c4df3c3b829ea39";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Lifecycle_msgsSrvGetState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Lifecycle_msgsSrvGetState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Lifecycle_msgsSrvGetState_Response>{const out=new VendorDecoded<Ros2Lifecycle_msgsSrvGetState_Response>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Lifecycle_msgsSrvGetState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Lifecycle_msgsSrvGetState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Lifecycle_msgsSrvGetState_Response():changetype<Ros2Lifecycle_msgsSrvGetState_Response>(out.value);}
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
