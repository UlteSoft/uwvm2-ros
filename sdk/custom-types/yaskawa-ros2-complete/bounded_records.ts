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
/** Complete original geometry_msgs/msg/TransformStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTransformStamped {
static readonly maxWireBytes:i32=648;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=648;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field child_frame_id, native scalar string. */
child_frame_id:string="";
/** Exact native field transform, native scalar geometry_msgs/msg/Transform. */
transform:Ros2Geometry_msgsMsgTransform=new Ros2Geometry_msgsMsgTransform();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>648-n)return -90;n+=child;
if(n>648)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>512)return -22;if(4+size>648-n)return -90;n+=4+size;
if(n>648)return -90;
}
{
const child=this.transform.wireSize();if(child<0)return child;if(child>648-n)return -90;n+=child;
if(n>648)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.child_frame_id);
this.transform.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTransformStamped{const value=new Ros2Geometry_msgsMsgTransformStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.child_frame_id=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTransform.readControl(r);
value.transform=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTransformStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgTransformStamped>{
static readonly codecIdentity:string="UWVMROS1_be553b5a9c73b687cde56ff1c381e3c67168fd2c0366830be3c65382c5bbd792";
static readonly maxWireBytes:i32=648;
encodeChecked(value:Ros2Geometry_msgsMsgTransformStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTransformStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTransformStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTransformStamped>();if(bytes.length>648){out.status=90;return out;}if(bytes.length<72){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTransformStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTransformStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTransformStamped():changetype<Ros2Geometry_msgsMsgTransformStamped>(out.value);}
}
/** Complete original industrial_msgs/msg/DebugLevel; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgDebugLevel {
static readonly DEBUG:u8=5;
static readonly INFO:u8=4;
static readonly WARN:u8=3;
static readonly ERROR:u8=2;
static readonly FATAL:u8=1;
static readonly NONE:u8=0;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field val, native scalar uint8. */
val:u8=0;
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
w.integer(<u64>this.val,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgDebugLevel{const value=new Ros2Industrial_msgsMsgDebugLevel();
{
const decoded=<u8>r.integer(1);
value.val=decoded;
}
return value;}
}
export class Ros2Industrial_msgsMsgDebugLevelCodec implements VendorCodec<Ros2Industrial_msgsMsgDebugLevel>{
static readonly codecIdentity:string="UWVMROS1_989475cc5d981d8c5ab91fb091235871f7a1c7d87d246eb2250086e6c3c37c79";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsMsgDebugLevel):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgDebugLevel):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgDebugLevel>{const out=new VendorDecoded<Ros2Industrial_msgsMsgDebugLevel>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgDebugLevel.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgDebugLevel{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgDebugLevel():changetype<Ros2Industrial_msgsMsgDebugLevel>(out.value);}
}
/** Complete original industrial_msgs/msg/DeviceInfo; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgDeviceInfo {
static readonly maxWireBytes:i32=2580;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2580;
/** Exact native field model, native scalar string. */
model:string="";
/** Exact native field serial_number, native scalar string. */
serial_number:string="";
/** Exact native field hw_version, native scalar string. */
hw_version:string="";
/** Exact native field sw_version, native scalar string. */
sw_version:string="";
/** Exact native field address, native scalar string. */
address:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.model)||this.model.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.model);if(size>512)return -22;if(4+size>2580-n)return -90;n+=4+size;
if(n>2580)return -90;
}
{
if(!validUnicode(this.serial_number)||this.serial_number.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.serial_number);if(size>512)return -22;if(4+size>2580-n)return -90;n+=4+size;
if(n>2580)return -90;
}
{
if(!validUnicode(this.hw_version)||this.hw_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.hw_version);if(size>512)return -22;if(4+size>2580-n)return -90;n+=4+size;
if(n>2580)return -90;
}
{
if(!validUnicode(this.sw_version)||this.sw_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.sw_version);if(size>512)return -22;if(4+size>2580-n)return -90;n+=4+size;
if(n>2580)return -90;
}
{
if(!validUnicode(this.address)||this.address.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.address);if(size>512)return -22;if(4+size>2580-n)return -90;n+=4+size;
if(n>2580)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.model);
w.text(this.serial_number);
w.text(this.hw_version);
w.text(this.sw_version);
w.text(this.address);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgDeviceInfo{const value=new Ros2Industrial_msgsMsgDeviceInfo();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.model=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.serial_number=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.hw_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.sw_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.address=decoded;
}
return value;}
}
export class Ros2Industrial_msgsMsgDeviceInfoCodec implements VendorCodec<Ros2Industrial_msgsMsgDeviceInfo>{
static readonly codecIdentity:string="UWVMROS1_f9a5962800e17495f34dbb73f37373df22f900e528c630497417f78cdecb283b";
static readonly maxWireBytes:i32=2580;
encodeChecked(value:Ros2Industrial_msgsMsgDeviceInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgDeviceInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgDeviceInfo>{const out=new VendorDecoded<Ros2Industrial_msgsMsgDeviceInfo>();if(bytes.length>2580){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgDeviceInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgDeviceInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgDeviceInfo():changetype<Ros2Industrial_msgsMsgDeviceInfo>(out.value);}
}
/** Complete original industrial_msgs/msg/RobotMode; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgRobotMode {
static readonly UNKNOWN:i8=-1;
static readonly MANUAL:i8=1;
static readonly AUTO:i8=2;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field val, native scalar int8. */
val:i8=0;
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
w.integer(<u64>this.val,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgRobotMode{const value=new Ros2Industrial_msgsMsgRobotMode();
{
const decoded=<i8>r.integer(1);
value.val=decoded;
}
return value;}
}
export class Ros2Industrial_msgsMsgRobotModeCodec implements VendorCodec<Ros2Industrial_msgsMsgRobotMode>{
static readonly codecIdentity:string="UWVMROS1_4f9173c5e27815ca33be603b8645a22420bda3ab308962127060d2413d092c4c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsMsgRobotMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgRobotMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgRobotMode>{const out=new VendorDecoded<Ros2Industrial_msgsMsgRobotMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgRobotMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgRobotMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgRobotMode():changetype<Ros2Industrial_msgsMsgRobotMode>(out.value);}
}
/** Complete original industrial_msgs/msg/TriState; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgTriState {
static readonly UNKNOWN:i8=-1;
static readonly TRUE:i8=1;
static readonly ON:i8=1;
static readonly ENABLED:i8=1;
static readonly HIGH:i8=1;
static readonly CLOSED:i8=1;
static readonly FALSE:i8=0;
static readonly OFF:i8=0;
static readonly DISABLED:i8=0;
static readonly LOW:i8=0;
static readonly OPEN:i8=0;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field val, native scalar int8. */
val:i8=0;
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
w.integer(<u64>this.val,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgTriState{const value=new Ros2Industrial_msgsMsgTriState();
{
const decoded=<i8>r.integer(1);
value.val=decoded;
}
return value;}
}
export class Ros2Industrial_msgsMsgTriStateCodec implements VendorCodec<Ros2Industrial_msgsMsgTriState>{
static readonly codecIdentity:string="UWVMROS1_b7ae62ce19d642ad46e962ffb50cd76c43c96ac673ac36d690f99c1dd8d2a841";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsMsgTriState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgTriState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgTriState>{const out=new VendorDecoded<Ros2Industrial_msgsMsgTriState>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgTriState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgTriState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgTriState():changetype<Ros2Industrial_msgsMsgTriState>(out.value);}
}
/** Complete original industrial_msgs/msg/RobotStatus; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgRobotStatus {
static readonly maxWireBytes:i32=150;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=150;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field mode, native scalar industrial_msgs/msg/RobotMode. */
mode:Ros2Industrial_msgsMsgRobotMode=new Ros2Industrial_msgsMsgRobotMode();
/** Exact native field e_stopped, native scalar industrial_msgs/msg/TriState. */
e_stopped:Ros2Industrial_msgsMsgTriState=new Ros2Industrial_msgsMsgTriState();
/** Exact native field drives_powered, native scalar industrial_msgs/msg/TriState. */
drives_powered:Ros2Industrial_msgsMsgTriState=new Ros2Industrial_msgsMsgTriState();
/** Exact native field motion_possible, native scalar industrial_msgs/msg/TriState. */
motion_possible:Ros2Industrial_msgsMsgTriState=new Ros2Industrial_msgsMsgTriState();
/** Exact native field in_motion, native scalar industrial_msgs/msg/TriState. */
in_motion:Ros2Industrial_msgsMsgTriState=new Ros2Industrial_msgsMsgTriState();
/** Exact native field in_error, native scalar industrial_msgs/msg/TriState. */
in_error:Ros2Industrial_msgsMsgTriState=new Ros2Industrial_msgsMsgTriState();
/** Exact native field error_codes, native scalar int32. */
error_codes:Array<i32>=new Array<i32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.mode.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.e_stopped.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.drives_powered.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.motion_possible.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.in_motion.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
const child=this.in_error.wireSize();if(child<0)return child;if(child>150-n)return -90;n+=child;
if(n>150)return -90;
}
{
if(this.error_codes.length>16)return -22;
if(4>150-n)return -90;n+=4;
for(let i=0;i<this.error_codes.length;++i){
if(4>150-n)return -90;n+=4;
if(n>150)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.mode.writeControl(w);
this.e_stopped.writeControl(w);
this.drives_powered.writeControl(w);
this.motion_possible.writeControl(w);
this.in_motion.writeControl(w);
this.in_error.writeControl(w);
w.u32(<u32>this.error_codes.length);
for(let i=0;i<this.error_codes.length;++i){
w.integer(<u64>this.error_codes[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgRobotStatus{const value=new Ros2Industrial_msgsMsgRobotStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgRobotMode.readControl(r);
value.mode=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriState.readControl(r);
value.e_stopped=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriState.readControl(r);
value.drives_powered=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriState.readControl(r);
value.motion_possible=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriState.readControl(r);
value.in_motion=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriState.readControl(r);
value.in_error=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.error_codes=new Array<i32>();for(let i:u32=0;i<count;++i){
const decoded=<i32>r.integer(4);
value.error_codes.push(decoded);
}
}
return value;}
}
export class Ros2Industrial_msgsMsgRobotStatusCodec implements VendorCodec<Ros2Industrial_msgsMsgRobotStatus>{
static readonly codecIdentity:string="UWVMROS1_1086f16ba5d614f3d87c3ab195240b5a0683ef490f2e92434e4ea3bdbe7554ba";
static readonly maxWireBytes:i32=150;
encodeChecked(value:Ros2Industrial_msgsMsgRobotStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgRobotStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgRobotStatus>{const out=new VendorDecoded<Ros2Industrial_msgsMsgRobotStatus>();if(bytes.length>150){out.status=90;return out;}if(bytes.length<22){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgRobotStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgRobotStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgRobotStatus():changetype<Ros2Industrial_msgsMsgRobotStatus>(out.value);}
}
/** Complete original industrial_msgs/msg/ServiceReturnCode; spellings are published in generation proof. */
export class Ros2Industrial_msgsMsgServiceReturnCode {
static readonly SUCCESS:i8=1;
static readonly FAILURE:i8=-1;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field val, native scalar int8. */
val:i8=0;
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
w.integer(<u64>this.val,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsMsgServiceReturnCode{const value=new Ros2Industrial_msgsMsgServiceReturnCode();
{
const decoded=<i8>r.integer(1);
value.val=decoded;
}
return value;}
}
export class Ros2Industrial_msgsMsgServiceReturnCodeCodec implements VendorCodec<Ros2Industrial_msgsMsgServiceReturnCode>{
static readonly codecIdentity:string="UWVMROS1_62eb4415a63d7686f16ea343193238838c78aa8e9b9786a65c92fb227e03d3e5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsMsgServiceReturnCode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsMsgServiceReturnCode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsMsgServiceReturnCode>{const out=new VendorDecoded<Ros2Industrial_msgsMsgServiceReturnCode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsMsgServiceReturnCode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsMsgServiceReturnCode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsMsgServiceReturnCode():changetype<Ros2Industrial_msgsMsgServiceReturnCode>(out.value);}
}
/** Complete original industrial_msgs/srv/CmdJointTrajectory_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvCmdJointTrajectory_Request {
static readonly maxWireBytes:i32=4164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4164;
/** Exact native field trajectory, native scalar trajectory_msgs/msg/JointTrajectory. */
trajectory:Ros2Trajectory_msgsMsgJointTrajectory=new Ros2Trajectory_msgsMsgJointTrajectory();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trajectory.wireSize();if(child<0)return child;if(child>4164-n)return -90;n+=child;
if(n>4164)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trajectory.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvCmdJointTrajectory_Request{const value=new Ros2Industrial_msgsSrvCmdJointTrajectory_Request();
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectory.readControl(r);
value.trajectory=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvCmdJointTrajectory_Request>{
static readonly codecIdentity:string="UWVMROS1_1def3934b243546a4591b7be1ef2eb27002fbd9d3725302016e3b1846805149d";
static readonly maxWireBytes:i32=4164;
encodeChecked(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvCmdJointTrajectory_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvCmdJointTrajectory_Request>();if(bytes.length>4164){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvCmdJointTrajectory_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvCmdJointTrajectory_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvCmdJointTrajectory_Request():changetype<Ros2Industrial_msgsSrvCmdJointTrajectory_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/CmdJointTrajectory_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvCmdJointTrajectory_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.code.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvCmdJointTrajectory_Response{const value=new Ros2Industrial_msgsSrvCmdJointTrajectory_Response();
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvCmdJointTrajectory_Response>{
static readonly codecIdentity:string="UWVMROS1_1e4d064ef659e5d1cc73bdd90f82657a16087c96e0f75b632e2be2be884bca8b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvCmdJointTrajectory_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvCmdJointTrajectory_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvCmdJointTrajectory_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvCmdJointTrajectory_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvCmdJointTrajectory_Response():changetype<Ros2Industrial_msgsSrvCmdJointTrajectory_Response>(out.value);}
}
/** Complete original industrial_msgs/srv/GetRobotInfo_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvGetRobotInfo_Request {
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
static readControl(r:Reader):Ros2Industrial_msgsSrvGetRobotInfo_Request{const value=new Ros2Industrial_msgsSrvGetRobotInfo_Request();
return value;}
}
export class Ros2Industrial_msgsSrvGetRobotInfo_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvGetRobotInfo_Request>{
static readonly codecIdentity:string="UWVMROS1_688a272f6ee39921013c6c39a7b0b0c4e2cda5119787088234103518ba25c1d2";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Industrial_msgsSrvGetRobotInfo_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvGetRobotInfo_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvGetRobotInfo_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvGetRobotInfo_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvGetRobotInfo_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvGetRobotInfo_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvGetRobotInfo_Request():changetype<Ros2Industrial_msgsSrvGetRobotInfo_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/GetRobotInfo_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvGetRobotInfo_Response {
static readonly maxWireBytes:i32=32768;
static readonly aggregateWireBytes:i32=32768;
static readonly cartesianWireBytes:u64=43865;
/** Exact native field controller, native scalar industrial_msgs/msg/DeviceInfo. */
controller:Ros2Industrial_msgsMsgDeviceInfo=new Ros2Industrial_msgsMsgDeviceInfo();
/** Exact native field robots, native scalar industrial_msgs/msg/DeviceInfo. */
robots:Array<Ros2Industrial_msgsMsgDeviceInfo>=new Array<Ros2Industrial_msgsMsgDeviceInfo>();
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.controller.wireSize();if(child<0)return child;if(child>32768-n)return -90;n+=child;
if(n>32768)return -90;
}
{
if(this.robots.length>16)return -22;
if(4>32768-n)return -90;n+=4;
for(let i=0;i<this.robots.length;++i){
const child=this.robots[i].wireSize();if(child<0)return child;if(child>32768-n)return -90;n+=child;
if(n>32768)return -90;
}
}
{
const child=this.code.wireSize();if(child<0)return child;if(child>32768-n)return -90;n+=child;
if(n>32768)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.controller.writeControl(w);
w.u32(<u32>this.robots.length);
for(let i=0;i<this.robots.length;++i){
this.robots[i].writeControl(w);
}
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvGetRobotInfo_Response{const value=new Ros2Industrial_msgsSrvGetRobotInfo_Response();
{
const decoded=Ros2Industrial_msgsMsgDeviceInfo.readControl(r);
value.controller=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.robots=new Array<Ros2Industrial_msgsMsgDeviceInfo>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Industrial_msgsMsgDeviceInfo.readControl(r);
value.robots.push(decoded);
}
}
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvGetRobotInfo_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvGetRobotInfo_Response>{
static readonly codecIdentity:string="UWVMROS1_6d37cdcd8afc918329d7856e682d7fcdae2931832474a9194143c21079ed4152";
static readonly maxWireBytes:i32=32768;
encodeChecked(value:Ros2Industrial_msgsSrvGetRobotInfo_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvGetRobotInfo_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvGetRobotInfo_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvGetRobotInfo_Response>();if(bytes.length>32768){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvGetRobotInfo_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvGetRobotInfo_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvGetRobotInfo_Response():changetype<Ros2Industrial_msgsSrvGetRobotInfo_Response>(out.value);}
}
/** Complete original industrial_msgs/srv/SetDrivePower_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvSetDrivePower_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field drive_power, native scalar bool. */
drive_power:bool=false;
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
w.u8(this.drive_power?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvSetDrivePower_Request{const value=new Ros2Industrial_msgsSrvSetDrivePower_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.drive_power=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvSetDrivePower_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvSetDrivePower_Request>{
static readonly codecIdentity:string="UWVMROS1_b4fc5e109e131a97820ab5856b34839c572ac0b0c256d70457c176e401844f96";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvSetDrivePower_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvSetDrivePower_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvSetDrivePower_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvSetDrivePower_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvSetDrivePower_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvSetDrivePower_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvSetDrivePower_Request():changetype<Ros2Industrial_msgsSrvSetDrivePower_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/SetDrivePower_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvSetDrivePower_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.code.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvSetDrivePower_Response{const value=new Ros2Industrial_msgsSrvSetDrivePower_Response();
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvSetDrivePower_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvSetDrivePower_Response>{
static readonly codecIdentity:string="UWVMROS1_050fb9a727ebedd240e9023b9a2379a5d72e0d9fa335bead0c40d783abb22cff";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvSetDrivePower_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvSetDrivePower_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvSetDrivePower_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvSetDrivePower_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvSetDrivePower_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvSetDrivePower_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvSetDrivePower_Response():changetype<Ros2Industrial_msgsSrvSetDrivePower_Response>(out.value);}
}
/** Complete original industrial_msgs/srv/SetRemoteLoggerLevel_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field level, native scalar industrial_msgs/msg/DebugLevel. */
level:Ros2Industrial_msgsMsgDebugLevel=new Ros2Industrial_msgsMsgDebugLevel();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.level.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.level.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request{const value=new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request();
{
const decoded=Ros2Industrial_msgsMsgDebugLevel.readControl(r);
value.level=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request>{
static readonly codecIdentity:string="UWVMROS1_bbadd39c05cab90b14f4b1fad23ff05ac83325f6cc57810cd649e04fa29fe014";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request():changetype<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/SetRemoteLoggerLevel_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.code.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response{const value=new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response();
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response>{
static readonly codecIdentity:string="UWVMROS1_b8681d031a4bb5a67b842277af2e2156e8339bdef23be88d2b64e296a7e3d63b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response():changetype<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response>(out.value);}
}
/** Complete original industrial_msgs/srv/StartMotion_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvStartMotion_Request {
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
static readControl(r:Reader):Ros2Industrial_msgsSrvStartMotion_Request{const value=new Ros2Industrial_msgsSrvStartMotion_Request();
return value;}
}
export class Ros2Industrial_msgsSrvStartMotion_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvStartMotion_Request>{
static readonly codecIdentity:string="UWVMROS1_0a96fb9489f863612a91bf82f995f2f28bcb7fb403bde74c617925c789e4dcd8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Industrial_msgsSrvStartMotion_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvStartMotion_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvStartMotion_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvStartMotion_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvStartMotion_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvStartMotion_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvStartMotion_Request():changetype<Ros2Industrial_msgsSrvStartMotion_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/StartMotion_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvStartMotion_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.code.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvStartMotion_Response{const value=new Ros2Industrial_msgsSrvStartMotion_Response();
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvStartMotion_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvStartMotion_Response>{
static readonly codecIdentity:string="UWVMROS1_3bb3650fbe638a6d300bcd9fcd5acf574bd54d00a7368edd3014bf44dcf66c16";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvStartMotion_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvStartMotion_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvStartMotion_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvStartMotion_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvStartMotion_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvStartMotion_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvStartMotion_Response():changetype<Ros2Industrial_msgsSrvStartMotion_Response>(out.value);}
}
/** Complete original industrial_msgs/srv/StopMotion_Request; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvStopMotion_Request {
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
static readControl(r:Reader):Ros2Industrial_msgsSrvStopMotion_Request{const value=new Ros2Industrial_msgsSrvStopMotion_Request();
return value;}
}
export class Ros2Industrial_msgsSrvStopMotion_RequestCodec implements VendorCodec<Ros2Industrial_msgsSrvStopMotion_Request>{
static readonly codecIdentity:string="UWVMROS1_fe5a7cf0da30270eab5cfb1ab8c1fbd37d2b4c4f91ef29662a0ddcac39e6ef49";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Industrial_msgsSrvStopMotion_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvStopMotion_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvStopMotion_Request>{const out=new VendorDecoded<Ros2Industrial_msgsSrvStopMotion_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvStopMotion_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvStopMotion_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvStopMotion_Request():changetype<Ros2Industrial_msgsSrvStopMotion_Request>(out.value);}
}
/** Complete original industrial_msgs/srv/StopMotion_Response; spellings are published in generation proof. */
export class Ros2Industrial_msgsSrvStopMotion_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field code, native scalar industrial_msgs/msg/ServiceReturnCode. */
code:Ros2Industrial_msgsMsgServiceReturnCode=new Ros2Industrial_msgsMsgServiceReturnCode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.code.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.code.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Industrial_msgsSrvStopMotion_Response{const value=new Ros2Industrial_msgsSrvStopMotion_Response();
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCode.readControl(r);
value.code=decoded;
}
return value;}
}
export class Ros2Industrial_msgsSrvStopMotion_ResponseCodec implements VendorCodec<Ros2Industrial_msgsSrvStopMotion_Response>{
static readonly codecIdentity:string="UWVMROS1_c26bda240d86becc5b8b4ea80380eba3602092a5ec4a06818dbfbd38c24015b5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Industrial_msgsSrvStopMotion_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Industrial_msgsSrvStopMotion_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Industrial_msgsSrvStopMotion_Response>{const out=new VendorDecoded<Ros2Industrial_msgsSrvStopMotion_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Industrial_msgsSrvStopMotion_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Industrial_msgsSrvStopMotion_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Industrial_msgsSrvStopMotion_Response():changetype<Ros2Industrial_msgsSrvStopMotion_Response>(out.value);}
}
/** Complete original motoros2_interfaces/msg/AlarmCauseRemedy; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgAlarmCauseRemedy {
static readonly maxWireBytes:i32=1032;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1032;
/** Exact native field cause, native scalar string. */
cause:string="";
/** Exact native field remedy, native scalar string. */
remedy:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.cause)||this.cause.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.cause);if(size>512)return -22;if(4+size>1032-n)return -90;n+=4+size;
if(n>1032)return -90;
}
{
if(!validUnicode(this.remedy)||this.remedy.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.remedy);if(size>512)return -22;if(4+size>1032-n)return -90;n+=4+size;
if(n>1032)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.cause);
w.text(this.remedy);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgAlarmCauseRemedy{const value=new Ros2Motoros2_interfacesMsgAlarmCauseRemedy();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.cause=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.remedy=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgAlarmCauseRemedyCodec implements VendorCodec<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>{
static readonly codecIdentity:string="UWVMROS1_f977609022f4ca593b9089d0038d1c28d1bc4b533e272f38c3379495f326ddf5";
static readonly maxWireBytes:i32=1032;
encodeChecked(value:Ros2Motoros2_interfacesMsgAlarmCauseRemedy):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgAlarmCauseRemedy):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>();if(bytes.length>1032){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgAlarmCauseRemedy.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgAlarmCauseRemedy{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgAlarmCauseRemedy():changetype<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>(out.value);}
}
/** Complete original motoros2_interfaces/msg/AlarmInfo; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgAlarmInfo {
static readonly maxWireBytes:i32=25248;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25248;
/** Exact native field number, native scalar uint32. */
number:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field sub_code, native scalar int32. */
sub_code:i32=0;
/** Exact native field datetime, native scalar builtin_interfaces/msg/Time. */
datetime:Ros2Builtin_interfacesMsgTime=new Ros2Builtin_interfacesMsgTime();
/** Exact native field contents, native scalar string. */
contents:string="";
/** Exact native field description, native scalar string. */
description:string="";
/** Exact native field help, native scalar motoros2_interfaces/msg/AlarmCauseRemedy. */
help:Array<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>=new Array<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>25248-n)return -90;n+=4;
if(n>25248)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>25248-n)return -90;n+=4+size;
if(n>25248)return -90;
}
{
if(4>25248-n)return -90;n+=4;
if(n>25248)return -90;
}
{
const child=this.datetime.wireSize();if(child<0)return child;if(child>25248-n)return -90;n+=child;
if(n>25248)return -90;
}
{
if(!validUnicode(this.contents)||this.contents.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.contents);if(size>4096)return -22;if(4+size>25248-n)return -90;n+=4+size;
if(n>25248)return -90;
}
{
if(!validUnicode(this.description)||this.description.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.description);if(size>4096)return -22;if(4+size>25248-n)return -90;n+=4+size;
if(n>25248)return -90;
}
{
if(this.help.length>16)return -22;
if(4>25248-n)return -90;n+=4;
for(let i=0;i<this.help.length;++i){
const child=this.help[i].wireSize();if(child<0)return child;if(child>25248-n)return -90;n+=child;
if(n>25248)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.number,4);
w.text(this.message);
w.integer(<u64>this.sub_code,4);
this.datetime.writeControl(w);
w.text(this.contents);
w.text(this.description);
w.u32(<u32>this.help.length);
for(let i=0;i<this.help.length;++i){
this.help[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgAlarmInfo{const value=new Ros2Motoros2_interfacesMsgAlarmInfo();
{
const decoded=<u32>r.integer(4);
value.number=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sub_code=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgTime.readControl(r);
value.datetime=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.contents=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.description=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.help=new Array<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgAlarmCauseRemedy.readControl(r);
value.help.push(decoded);
}
}
return value;}
}
export class Ros2Motoros2_interfacesMsgAlarmInfoCodec implements VendorCodec<Ros2Motoros2_interfacesMsgAlarmInfo>{
static readonly codecIdentity:string="UWVMROS1_7cf3fa3037f28f295dd62009e708ea3e46a80f83471d0a0f9fb55af125c0c27f";
static readonly maxWireBytes:i32=25248;
encodeChecked(value:Ros2Motoros2_interfacesMsgAlarmInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgAlarmInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgAlarmInfo>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgAlarmInfo>();if(bytes.length>25248){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgAlarmInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgAlarmInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgAlarmInfo():changetype<Ros2Motoros2_interfacesMsgAlarmInfo>(out.value);}
}
/** Complete original motoros2_interfaces/msg/ErrorInfo; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgErrorInfo {
static readonly maxWireBytes:i32=532;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=532;
/** Exact native field number, native scalar uint32. */
number:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field sub_code, native scalar int32. */
sub_code:i32=0;
/** Exact native field datetime, native scalar builtin_interfaces/msg/Time. */
datetime:Ros2Builtin_interfacesMsgTime=new Ros2Builtin_interfacesMsgTime();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>532-n)return -90;n+=4;
if(n>532)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>532-n)return -90;n+=4+size;
if(n>532)return -90;
}
{
if(4>532-n)return -90;n+=4;
if(n>532)return -90;
}
{
const child=this.datetime.wireSize();if(child<0)return child;if(child>532-n)return -90;n+=child;
if(n>532)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.number,4);
w.text(this.message);
w.integer(<u64>this.sub_code,4);
this.datetime.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgErrorInfo{const value=new Ros2Motoros2_interfacesMsgErrorInfo();
{
const decoded=<u32>r.integer(4);
value.number=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const decoded=<i32>r.integer(4);
value.sub_code=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgTime.readControl(r);
value.datetime=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgErrorInfoCodec implements VendorCodec<Ros2Motoros2_interfacesMsgErrorInfo>{
static readonly codecIdentity:string="UWVMROS1_1cfe60c0af6bc157db84c4d9e0b20a8f2e5c78fd4afd6908d32b3217f3734b67";
static readonly maxWireBytes:i32=532;
encodeChecked(value:Ros2Motoros2_interfacesMsgErrorInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgErrorInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgErrorInfo>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgErrorInfo>();if(bytes.length>532){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgErrorInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgErrorInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgErrorInfo():changetype<Ros2Motoros2_interfacesMsgErrorInfo>(out.value);}
}
/** Complete original motoros2_interfaces/msg/InformJobCrudResultCodes; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {
static readonly RC_OK:u32=1;
static readonly STR_OK:string="Success";
static readonly RC_ERR_REFRESHING_JOB_LIST:u32=10;
static readonly STR_ERR_REFRESHING_JOB_LIST:string="Error refreshing job list";
static readonly RC_ERR_RETRIEVING_FILE_COUNT:u32=11;
static readonly STR_ERR_RETRIEVING_FILE_COUNT:string="Error retrieving file count";
static readonly RC_ERR_TOO_MANY_JOBS:u32=12;
static readonly STR_ERR_TOO_MANY_JOBS:string="Too many jobs";
static readonly RC_ERR_RETRIEVING_JOB_NAME_FROM_LIST:u32=13;
static readonly STR_ERR_RETRIEVING_JOB_NAME_FROM_LIST:string="Failed retrieving job name from list";
static readonly RC_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE:u32=14;
static readonly STR_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE:string="Error adding job name to service response";
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
static readControl(r:Reader):Ros2Motoros2_interfacesMsgInformJobCrudResultCodes{const value=new Ros2Motoros2_interfacesMsgInformJobCrudResultCodes();
return value;}
}
export class Ros2Motoros2_interfacesMsgInformJobCrudResultCodesCodec implements VendorCodec<Ros2Motoros2_interfacesMsgInformJobCrudResultCodes>{
static readonly codecIdentity:string="UWVMROS1_f5ed037b83926b48a88fa486ed8db5f5d3e39e4a486d4a90190ffd6a70452613";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesMsgInformJobCrudResultCodes):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgInformJobCrudResultCodes):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgInformJobCrudResultCodes>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgInformJobCrudResultCodes>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgInformJobCrudResultCodes.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgInformJobCrudResultCodes{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgInformJobCrudResultCodes():changetype<Ros2Motoros2_interfacesMsgInformJobCrudResultCodes>(out.value);}
}
/** Complete original motoros2_interfaces/msg/InitTrajEnum; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgInitTrajEnum {
static readonly INIT_TRAJ_OK:u16=0;
static readonly INIT_TRAJ_OK_STR:string="OK";
static readonly INIT_TRAJ_UNSPECIFIED:u16=200;
static readonly INIT_TRAJ_UNSPECIFIED_STR:string="Unspecified failure";
static readonly INIT_TRAJ_TOO_SMALL:u16=201;
static readonly INIT_TRAJ_TOO_SMALL_STR:string="Trajectory must contain at least two points.";
static readonly INIT_TRAJ_TOO_BIG:u16=202;
static readonly INIT_TRAJ_TOO_BIG_STR:string="Trajectory contains too many points (Not enough memory).";
static readonly INIT_TRAJ_ALREADY_IN_MOTION:u16=203;
static readonly INIT_TRAJ_ALREADY_IN_MOTION_STR:string="Already running a trajectory.";
static readonly INIT_TRAJ_INVALID_STARTING_POS:u16=204;
static readonly INIT_TRAJ_INVALID_STARTING_POS_STR:string="The first point must match the robot's current position.";
static readonly INIT_TRAJ_INVALID_VELOCITY:u16=205;
static readonly INIT_TRAJ_INVALID_VELOCITY_STR:string="The commanded velocity is too high.";
static readonly INIT_TRAJ_INVALID_JOINTNAME:u16=206;
static readonly INIT_TRAJ_INVALID_JOINTNAME_STR:string="Invalid joint name specified. Check motoros2_config.yaml.";
static readonly INIT_TRAJ_INCOMPLETE_JOINTLIST:u16=207;
static readonly INIT_TRAJ_INCOMPLETE_JOINTLIST_STR:string="Trajectory must contain data for all joints.";
static readonly INIT_TRAJ_INVALID_TIME:u16=208;
static readonly INIT_TRAJ_INVALID_TIME_STR:string="Invalid time in trajectory.";
static readonly INIT_TRAJ_WRONG_MODE:u16=209;
static readonly INIT_TRAJ_WRONG_MODE_STR:string="Must call start_traj_mode service";
static readonly INIT_TRAJ_BACKWARD_TIME:u16=210;
static readonly INIT_TRAJ_BACKWARD_TIME_STR:string="Trajectory message contains waypoints that are not strictly increasing in time.";
static readonly INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS:u16=211;
static readonly INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS_STR:string="Trajectory did not contain position data for all axes.";
static readonly INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES:u16=212;
static readonly INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES_STR:string="Trajectory did not contain velocity data for all axes.";
static readonly INIT_TRAJ_INVALID_ENDING_VELOCITY:u16=213;
static readonly INIT_TRAJ_INVALID_ENDING_VELOCITY_STR:string="The final point in the trajectory must have zero velocity.";
static readonly INIT_TRAJ_INVALID_ENDING_ACCELERATION:u16=214;
static readonly INIT_TRAJ_INVALID_ENDING_ACCELERATION_STR:string="The final point in the trajectory must have zero acceleration.";
static readonly INIT_TRAJ_DUPLICATE_JOINT_NAME:u16=215;
static readonly INIT_TRAJ_DUPLICATE_JOINT_NAME_STR:string="The trajectory contains duplicate joint names.";
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field value, native scalar uint16. */
value:u16=0;
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
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgInitTrajEnum{const value=new Ros2Motoros2_interfacesMsgInitTrajEnum();
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgInitTrajEnumCodec implements VendorCodec<Ros2Motoros2_interfacesMsgInitTrajEnum>{
static readonly codecIdentity:string="UWVMROS1_9279979f9e3570f48d6e23bf812a4680e3deed9af3309019efdff5464d7b7763";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Motoros2_interfacesMsgInitTrajEnum):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgInitTrajEnum):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgInitTrajEnum>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgInitTrajEnum>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgInitTrajEnum.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgInitTrajEnum{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgInitTrajEnum():changetype<Ros2Motoros2_interfacesMsgInitTrajEnum>(out.value);}
}
/** Complete original motoros2_interfaces/msg/IoResultCodes; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgIoResultCodes {
static readonly OK:u32=0;
static readonly OK_STR:string="Success";
static readonly READ_ADDRESS_INVALID:u32=1001;
static readonly READ_ADDRESS_INVALID_STR:string="Address cannot be read from (out of readable range)";
static readonly WRITE_ADDRESS_INVALID:u32=1002;
static readonly WRITE_ADDRESS_INVALID_STR:string="Address cannot be written to (out of writable range)";
static readonly WRITE_VALUE_INVALID:u32=1003;
static readonly WRITE_VALUE_INVALID_STR:string="Illegal value for the type of IO element addressed";
static readonly READ_API_ERROR:u32=1004;
static readonly READ_API_ERROR_STR:string="The MotoPlus function mpReadIO returned -1. No further information is available";
static readonly WRITE_API_ERROR:u32=1005;
static readonly WRITE_API_ERROR_STR:string="The MotoPlus function mpWriteIO returned -1. No further information is available";
static readonly UNKNOWN_API_ERROR_STR:string="Unknown error accessing I/O. No further information is available";
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
static readControl(r:Reader):Ros2Motoros2_interfacesMsgIoResultCodes{const value=new Ros2Motoros2_interfacesMsgIoResultCodes();
return value;}
}
export class Ros2Motoros2_interfacesMsgIoResultCodesCodec implements VendorCodec<Ros2Motoros2_interfacesMsgIoResultCodes>{
static readonly codecIdentity:string="UWVMROS1_ef9ab829ae79d4e58336a56259b6beb2ad26a8b1b4070013d9004ac4ab269915";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesMsgIoResultCodes):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgIoResultCodes):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgIoResultCodes>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgIoResultCodes>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgIoResultCodes.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgIoResultCodes{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgIoResultCodes():changetype<Ros2Motoros2_interfacesMsgIoResultCodes>(out.value);}
}
/** Complete original motoros2_interfaces/msg/MotionReadyEnum; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgMotionReadyEnum {
static readonly READY:u16=1;
static readonly READY_STR:string="Ready";
static readonly NOT_READY_UNSPECIFIED:u16=100;
static readonly NOT_READY_UNSPECIFIED_STR:string="Unspecified";
static readonly NOT_READY_ALARM:u16=101;
static readonly NOT_READY_ALARM_STR:string="The controller has an active Alarm";
static readonly NOT_READY_ERROR:u16=102;
static readonly NOT_READY_ERROR_STR:string="The controller has an active Error";
static readonly NOT_READY_ESTOP:u16=103;
static readonly NOT_READY_ESTOP_STR:string="The controller is in E-Stop";
static readonly NOT_READY_NOT_PLAY:u16=104;
static readonly NOT_READY_NOT_PLAY_STR:string="The teach pendant must not be in TEACH mode";
static readonly NOT_READY_NOT_REMOTE:u16=105;
static readonly NOT_READY_NOT_REMOTE_STR:string="The teach pendant must be in REMOTE mode";
static readonly NOT_READY_SERVO_OFF:u16=106;
static readonly NOT_READY_SERVO_OFF_STR:string="Servo power is OFF. Please call start_traj_mode or start_point_queue_mode service";
static readonly NOT_READY_HOLD:u16=107;
static readonly NOT_READY_HOLD_STR:string="The controller is in HOLD";
static readonly NOT_READY_JOB_NOT_STARTED:u16=108;
static readonly NOT_READY_JOB_NOT_STARTED_STR:string="The INIT_ROS job has not started. Please call start_traj_mode or start_point_queue_mode service";
static readonly NOT_READY_NOT_ON_WAIT_CMD:u16=109;
static readonly NOT_READY_NOT_ON_WAIT_CMD_STR:string="INFORM job is not on a WAIT command. Check the format of INIT_ROS";
static readonly NOT_READY_PFL_ACTIVE:u16=111;
static readonly NOT_READY_PFL_ACTIVE_STR:string="A PFL event has occurred. Please return the robot to a safe state";
static readonly NOT_READY_INC_MOVE_ERROR:u16=112;
static readonly NOT_READY_INC_MOVE_ERROR_STR:string="The controller is in an error state. Please call reset_error to attempt to clear the error";
static readonly NOT_READY_OTHER_PROGRAM_RUNNING:u16=113;
static readonly NOT_READY_OTHER_PROGRAM_RUNNING_STR:string="Controller is running another job";
static readonly NOT_READY_OTHER_TRAJ_MODE_ACTIVE:u16=114;
static readonly NOT_READY_OTHER_TRAJ_MODE_ACTIVE_STR:string="Another trajectory mode is already active. Please call stop_traj_mode service";
static readonly NOT_READY_NOT_CONT_CYCLE_MODE:u16=115;
static readonly NOT_READY_NOT_CONT_CYCLE_MODE_STR:string="AUTO cycle mode not set. Please set the cycle mode to AUTO";
static readonly NOT_READY_MAJOR_ALARM:u16=116;
static readonly NOT_READY_MAJOR_ALARM_STR:string="Major Alarms can't be reset. Please check the teach pendant";
static readonly NOT_READY_ECO_MODE:u16=117;
static readonly NOT_READY_ECO_MODE_STR:string="Couldn't disable eco mode";
static readonly NOT_READY_SERVO_ON_TIMEOUT:u16=118;
static readonly NOT_READY_SERVO_ON_TIMEOUT_STR:string="Timed out while waiting for servo on";
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field value, native scalar uint16. */
value:u16=0;
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
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgMotionReadyEnum{const value=new Ros2Motoros2_interfacesMsgMotionReadyEnum();
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgMotionReadyEnumCodec implements VendorCodec<Ros2Motoros2_interfacesMsgMotionReadyEnum>{
static readonly codecIdentity:string="UWVMROS1_c8d4c2b0eba665e01cddc730421a3a42c5b344f4de35ae0a32204891603326d6";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Motoros2_interfacesMsgMotionReadyEnum):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgMotionReadyEnum):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgMotionReadyEnum>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgMotionReadyEnum>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgMotionReadyEnum.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgMotionReadyEnum{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgMotionReadyEnum():changetype<Ros2Motoros2_interfacesMsgMotionReadyEnum>(out.value);}
}
/** Complete original motoros2_interfaces/msg/QueueResultEnum; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgQueueResultEnum {
static readonly SUCCESS:u16=1;
static readonly SUCCESS_STR:string="";
static readonly WRONG_MODE:u16=2;
static readonly WRONG_MODE_STR:string="Must call start_point_queue_mode service";
static readonly INIT_FAILURE:u16=3;
static readonly INIT_FAILURE_STR:string="Failed to initialize the streaming trajectory";
static readonly BUSY:u16=4;
static readonly BUSY_STR:string="Busy processing the previous trajectory point. Please resend the next point shortly";
static readonly INVALID_JOINT_LIST:u16=5;
static readonly INVALID_JOINT_LIST_STR:string="Point must contain data for all joints";
static readonly UNABLE_TO_PROCESS_POINT:u16=6;
static readonly UNABLE_TO_PROCESS_POINT_STR:string="Error while processing the trajectory point. Check debug log for more details";
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field value, native scalar uint16. */
value:u16=0;
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
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgQueueResultEnum{const value=new Ros2Motoros2_interfacesMsgQueueResultEnum();
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgQueueResultEnumCodec implements VendorCodec<Ros2Motoros2_interfacesMsgQueueResultEnum>{
static readonly codecIdentity:string="UWVMROS1_146c6ce3f275b9e4a81622d73cca3d2298cf9f7bb95a783c0dea6279cb5b4f23";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Motoros2_interfacesMsgQueueResultEnum):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgQueueResultEnum):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgQueueResultEnum>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgQueueResultEnum>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgQueueResultEnum.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgQueueResultEnum{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgQueueResultEnum():changetype<Ros2Motoros2_interfacesMsgQueueResultEnum>(out.value);}
}
/** Complete original motoros2_interfaces/msg/SelectionResultCodes; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesMsgSelectionResultCodes {
static readonly OK:u32=0;
static readonly OK_STR:string="";
static readonly INVALID_CONTROLLER_STATE:u32=400;
static readonly INVALID_CONTROLLER_STATE_STR:string="Pendant is not in REMOTE mode";
static readonly INVALID_CONTROL_GROUP:u32=401;
static readonly INVALID_CONTROL_GROUP_STR:string="Invalid control group";
static readonly INVALID_SELECTION_INDEX:u32=402;
static readonly INVALID_SELECTION_INDEX_STR:string="Invalid tool selection";
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field value, native scalar uint32. */
value:u32=0;
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
w.integer(<u64>this.value,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesMsgSelectionResultCodes{const value=new Ros2Motoros2_interfacesMsgSelectionResultCodes();
{
const decoded=<u32>r.integer(4);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesMsgSelectionResultCodesCodec implements VendorCodec<Ros2Motoros2_interfacesMsgSelectionResultCodes>{
static readonly codecIdentity:string="UWVMROS1_46168b1f851163985e75842d9d795c5715160864a7736e7f92f95818c9435602";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Motoros2_interfacesMsgSelectionResultCodes):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesMsgSelectionResultCodes):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesMsgSelectionResultCodes>{const out=new VendorDecoded<Ros2Motoros2_interfacesMsgSelectionResultCodes>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesMsgSelectionResultCodes.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesMsgSelectionResultCodes{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesMsgSelectionResultCodes():changetype<Ros2Motoros2_interfacesMsgSelectionResultCodes>(out.value);}
}
/** Complete original motoros2_interfaces/srv/GetActiveAlarmInfo_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request {
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
static readControl(r:Reader):Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request{const value=new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request();
return value;}
}
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request>{
static readonly codecIdentity:string="UWVMROS1_aadcd63f3bc47424c38767afdc1589561e3c90e8aeae9bdceef88b1033d57a7e";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request():changetype<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/GetActiveAlarmInfo_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response {
static readonly maxWireBytes:i32=32768;
static readonly aggregateWireBytes:i32=32768;
static readonly cartesianWireBytes:u64=413008;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field result_message, native scalar string. */
result_message:string="";
/** Exact native field alarms, native scalar motoros2_interfaces/msg/AlarmInfo. */
alarms:Array<Ros2Motoros2_interfacesMsgAlarmInfo>=new Array<Ros2Motoros2_interfacesMsgAlarmInfo>();
/** Exact native field errors, native scalar motoros2_interfaces/msg/ErrorInfo. */
errors:Array<Ros2Motoros2_interfacesMsgErrorInfo>=new Array<Ros2Motoros2_interfacesMsgErrorInfo>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>32768-n)return -90;n+=4;
if(n>32768)return -90;
}
{
if(!validUnicode(this.result_message)||this.result_message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.result_message);if(size>512)return -22;if(4+size>32768-n)return -90;n+=4+size;
if(n>32768)return -90;
}
{
if(this.alarms.length>16)return -22;
if(4>32768-n)return -90;n+=4;
for(let i=0;i<this.alarms.length;++i){
const child=this.alarms[i].wireSize();if(child<0)return child;if(child>32768-n)return -90;n+=child;
if(n>32768)return -90;
}
}
{
if(this.errors.length>16)return -22;
if(4>32768-n)return -90;n+=4;
for(let i=0;i<this.errors.length;++i){
const child=this.errors[i].wireSize();if(child<0)return child;if(child>32768-n)return -90;n+=child;
if(n>32768)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.result_message);
w.u32(<u32>this.alarms.length);
for(let i=0;i<this.alarms.length;++i){
this.alarms[i].writeControl(w);
}
w.u32(<u32>this.errors.length);
for(let i=0;i<this.errors.length;++i){
this.errors[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response{const value=new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.result_message=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.alarms=new Array<Ros2Motoros2_interfacesMsgAlarmInfo>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgAlarmInfo.readControl(r);
value.alarms.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.errors=new Array<Ros2Motoros2_interfacesMsgErrorInfo>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgErrorInfo.readControl(r);
value.errors.push(decoded);
}
}
return value;}
}
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response>{
static readonly codecIdentity:string="UWVMROS1_e11bdbb95b6ded88d035696260c58dad32bcf3604aa4e89dd41c0dab26541567";
static readonly maxWireBytes:i32=32768;
encodeChecked(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response>();if(bytes.length>32768){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response():changetype<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ListInformJobs_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvListInformJobs_Request {
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
static readControl(r:Reader):Ros2Motoros2_interfacesSrvListInformJobs_Request{const value=new Ros2Motoros2_interfacesSrvListInformJobs_Request();
return value;}
}
export class Ros2Motoros2_interfacesSrvListInformJobs_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvListInformJobs_Request>{
static readonly codecIdentity:string="UWVMROS1_e77eedadef98d98321b1757b88284022afcdb1ffef52b98820b0c4ba596d643c";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesSrvListInformJobs_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvListInformJobs_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvListInformJobs_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvListInformJobs_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvListInformJobs_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvListInformJobs_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvListInformJobs_Request():changetype<Ros2Motoros2_interfacesSrvListInformJobs_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ListInformJobs_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvListInformJobs_Response {
static readonly maxWireBytes:i32=8780;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8780;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field names, native scalar string. */
names:Array<string>=new Array<string>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>8780-n)return -90;n+=4;
if(n>8780)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>8780-n)return -90;n+=4+size;
if(n>8780)return -90;
}
{
if(this.names.length>16)return -22;
if(4>8780-n)return -90;n+=4;
for(let i=0;i<this.names.length;++i){
if(!validUnicode(this.names[i])||this.names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.names[i]);if(size>512)return -22;if(4+size>8780-n)return -90;n+=4+size;
if(n>8780)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u32(<u32>this.names.length);
for(let i=0;i<this.names.length;++i){
w.text(this.names[i]);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvListInformJobs_Response{const value=new Ros2Motoros2_interfacesSrvListInformJobs_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.names.push(decoded);
}
}
return value;}
}
export class Ros2Motoros2_interfacesSrvListInformJobs_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvListInformJobs_Response>{
static readonly codecIdentity:string="UWVMROS1_49021f0abc00f8df1ef49605a244cb6bba8d8b45ece5365ae35d747d2506eadc";
static readonly maxWireBytes:i32=8780;
encodeChecked(value:Ros2Motoros2_interfacesSrvListInformJobs_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvListInformJobs_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvListInformJobs_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvListInformJobs_Response>();if(bytes.length>8780){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvListInformJobs_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvListInformJobs_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvListInformJobs_Response():changetype<Ros2Motoros2_interfacesSrvListInformJobs_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/QueueTrajPoint_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvQueueTrajPoint_Request {
static readonly maxWireBytes:i32=1228;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1228;
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field point, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
point:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.joint_names.length>12)return -22;
if(4>1228-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>1228-n)return -90;n+=4+size;
if(n>1228)return -90;
}
}
{
const child=this.point.wireSize();if(child<0)return child;if(child>1228-n)return -90;n+=child;
if(n>1228)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
this.point.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvQueueTrajPoint_Request{const value=new Ros2Motoros2_interfacesSrvQueueTrajPoint_Request();
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
value.point=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request>{
static readonly codecIdentity:string="UWVMROS1_3d37f9981d6abaca33ec0b5378fa3a1ac301791cc9be94d5bdcf4ea6aa81b8d0";
static readonly maxWireBytes:i32=1228;
encodeChecked(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request>();if(bytes.length>1228){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvQueueTrajPoint_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvQueueTrajPoint_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvQueueTrajPoint_Request():changetype<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/QueueTrajPoint_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvQueueTrajPoint_Response {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar motoros2_interfaces/msg/QueueResultEnum. */
result_code:Ros2Motoros2_interfacesMsgQueueResultEnum=new Ros2Motoros2_interfacesMsgQueueResultEnum();
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.result_code.wireSize();if(child<0)return child;if(child>518-n)return -90;n+=child;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.result_code.writeControl(w);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvQueueTrajPoint_Response{const value=new Ros2Motoros2_interfacesSrvQueueTrajPoint_Response();
{
const decoded=Ros2Motoros2_interfacesMsgQueueResultEnum.readControl(r);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvQueueTrajPoint_Response>{
static readonly codecIdentity:string="UWVMROS1_d4887ed24d39332a5ccfd4a3d072004dd8f314fe5c166bf0bd57520a826cbccd";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvQueueTrajPoint_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvQueueTrajPoint_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvQueueTrajPoint_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvQueueTrajPoint_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvQueueTrajPoint_Response():changetype<Ros2Motoros2_interfacesSrvQueueTrajPoint_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadGroupIO_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadGroupIO_Request {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field address, native scalar uint32. */
address:u32=0;
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
w.integer(<u64>this.address,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadGroupIO_Request{const value=new Ros2Motoros2_interfacesSrvReadGroupIO_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadGroupIO_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadGroupIO_Request>{
static readonly codecIdentity:string="UWVMROS1_6ee09ac379e169fc23b81a551e9a9ed21c3f82b5010859984e911b17152beb7f";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadGroupIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadGroupIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadGroupIO_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadGroupIO_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadGroupIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadGroupIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadGroupIO_Request():changetype<Ros2Motoros2_interfacesSrvReadGroupIO_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadGroupIO_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadGroupIO_Response {
static readonly maxWireBytes:i32=522;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=522;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field value, native scalar uint8. */
value:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>522-n)return -90;n+=4;
if(n>522)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>522-n)return -90;n+=4+size;
if(n>522)return -90;
}
{
if(1>522-n)return -90;n+=1;
if(n>522)return -90;
}
{
if(1>522-n)return -90;n+=1;
if(n>522)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
w.integer(<u64>this.value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadGroupIO_Response{const value=new Ros2Motoros2_interfacesSrvReadGroupIO_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadGroupIO_Response>{
static readonly codecIdentity:string="UWVMROS1_292fcf6e8c46a2509ee4cc5238d443157f37fed0de898367248b967e9001ea0d";
static readonly maxWireBytes:i32=522;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadGroupIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadGroupIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadGroupIO_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadGroupIO_Response>();if(bytes.length>522){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadGroupIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadGroupIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadGroupIO_Response():changetype<Ros2Motoros2_interfacesSrvReadGroupIO_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadMRegister_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadMRegister_Request {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field address, native scalar uint32. */
address:u32=0;
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
w.integer(<u64>this.address,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadMRegister_Request{const value=new Ros2Motoros2_interfacesSrvReadMRegister_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadMRegister_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadMRegister_Request>{
static readonly codecIdentity:string="UWVMROS1_f31f0983466054f5ac5dcb2ab80ab7721d606a681b833ebb376cc87f96054f6f";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadMRegister_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadMRegister_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadMRegister_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadMRegister_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadMRegister_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadMRegister_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadMRegister_Request():changetype<Ros2Motoros2_interfacesSrvReadMRegister_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadMRegister_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadMRegister_Response {
static readonly maxWireBytes:i32=523;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=523;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field value, native scalar uint16. */
value:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>523-n)return -90;n+=4;
if(n>523)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>523-n)return -90;n+=4+size;
if(n>523)return -90;
}
{
if(1>523-n)return -90;n+=1;
if(n>523)return -90;
}
{
if(2>523-n)return -90;n+=2;
if(n>523)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadMRegister_Response{const value=new Ros2Motoros2_interfacesSrvReadMRegister_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadMRegister_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadMRegister_Response>{
static readonly codecIdentity:string="UWVMROS1_8d6ded2876adfa2079711205cb0110a82ead6f51db05f266bf285308cd815a04";
static readonly maxWireBytes:i32=523;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadMRegister_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadMRegister_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadMRegister_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadMRegister_Response>();if(bytes.length>523){out.status=90;return out;}if(bytes.length<11){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadMRegister_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadMRegister_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadMRegister_Response():changetype<Ros2Motoros2_interfacesSrvReadMRegister_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadSingleIO_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadSingleIO_Request {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field address, native scalar uint32. */
address:u32=0;
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
w.integer(<u64>this.address,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadSingleIO_Request{const value=new Ros2Motoros2_interfacesSrvReadSingleIO_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadSingleIO_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadSingleIO_Request>{
static readonly codecIdentity:string="UWVMROS1_30e28db303b772f26b2b3cf6165a0ff79037d037d60e8115b4ee8e177f17e8bb";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadSingleIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadSingleIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadSingleIO_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadSingleIO_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadSingleIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadSingleIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadSingleIO_Request():changetype<Ros2Motoros2_interfacesSrvReadSingleIO_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ReadSingleIO_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvReadSingleIO_Response {
static readonly maxWireBytes:i32=525;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=525;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field value, native scalar int32. */
value:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>525-n)return -90;n+=4;
if(n>525)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>525-n)return -90;n+=4+size;
if(n>525)return -90;
}
{
if(1>525-n)return -90;n+=1;
if(n>525)return -90;
}
{
if(4>525-n)return -90;n+=4;
if(n>525)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
w.integer(<u64>this.value,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvReadSingleIO_Response{const value=new Ros2Motoros2_interfacesSrvReadSingleIO_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=<i32>r.integer(4);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvReadSingleIO_Response>{
static readonly codecIdentity:string="UWVMROS1_b42013571274e85769ffac4d47129de8b76e4a675bdef3efefebc9ee19e92082";
static readonly maxWireBytes:i32=525;
encodeChecked(value:Ros2Motoros2_interfacesSrvReadSingleIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvReadSingleIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvReadSingleIO_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvReadSingleIO_Response>();if(bytes.length>525){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvReadSingleIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvReadSingleIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvReadSingleIO_Response():changetype<Ros2Motoros2_interfacesSrvReadSingleIO_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ResetError_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvResetError_Request {
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
static readControl(r:Reader):Ros2Motoros2_interfacesSrvResetError_Request{const value=new Ros2Motoros2_interfacesSrvResetError_Request();
return value;}
}
export class Ros2Motoros2_interfacesSrvResetError_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvResetError_Request>{
static readonly codecIdentity:string="UWVMROS1_2737855d0608478946e2d9ab7f06c92585199c1082ec8406a012f50ad56beae3";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesSrvResetError_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvResetError_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvResetError_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvResetError_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvResetError_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvResetError_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvResetError_Request():changetype<Ros2Motoros2_interfacesSrvResetError_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/ResetError_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvResetError_Response {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar motoros2_interfaces/msg/MotionReadyEnum. */
result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum=new Ros2Motoros2_interfacesMsgMotionReadyEnum();
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.result_code.wireSize();if(child<0)return child;if(child>518-n)return -90;n+=child;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.result_code.writeControl(w);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvResetError_Response{const value=new Ros2Motoros2_interfacesSrvResetError_Response();
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum.readControl(r);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvResetError_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvResetError_Response>{
static readonly codecIdentity:string="UWVMROS1_0af14ee02c157ecc39b143225a580e475473306389ccc3ba198cf22ca47d82f3";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Motoros2_interfacesSrvResetError_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvResetError_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvResetError_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvResetError_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvResetError_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvResetError_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvResetError_Response():changetype<Ros2Motoros2_interfacesSrvResetError_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/SelectMotionTool_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvSelectMotionTool_Request {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field group_number, native scalar uint32. */
group_number:u32=0;
/** Exact native field tool_number, native scalar uint32. */
tool_number:u32=0;
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
w.integer(<u64>this.group_number,4);
w.integer(<u64>this.tool_number,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvSelectMotionTool_Request{const value=new Ros2Motoros2_interfacesSrvSelectMotionTool_Request();
{
const decoded=<u32>r.integer(4);
value.group_number=decoded;
}
{
const decoded=<u32>r.integer(4);
value.tool_number=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvSelectMotionTool_Request>{
static readonly codecIdentity:string="UWVMROS1_e6c884ae0a97337c1e441efe099b7869dc95205fcbca0a8082d4d9b06f5b438c";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvSelectMotionTool_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvSelectMotionTool_Request>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvSelectMotionTool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvSelectMotionTool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvSelectMotionTool_Request():changetype<Ros2Motoros2_interfacesSrvSelectMotionTool_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/SelectMotionTool_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvSelectMotionTool_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field result_code, native scalar motoros2_interfaces/msg/SelectionResultCodes. */
result_code:Ros2Motoros2_interfacesMsgSelectionResultCodes=new Ros2Motoros2_interfacesMsgSelectionResultCodes();
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.result_code.wireSize();if(child<0)return child;if(child>521-n)return -90;n+=child;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.result_code.writeControl(w);
w.text(this.message);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvSelectMotionTool_Response{const value=new Ros2Motoros2_interfacesSrvSelectMotionTool_Response();
{
const decoded=Ros2Motoros2_interfacesMsgSelectionResultCodes.readControl(r);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvSelectMotionTool_Response>{
static readonly codecIdentity:string="UWVMROS1_088349a49c14c52282c25b428628462c48618c3b5202df560dcc5c9bed742640";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvSelectMotionTool_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvSelectMotionTool_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvSelectMotionTool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvSelectMotionTool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvSelectMotionTool_Response():changetype<Ros2Motoros2_interfacesSrvSelectMotionTool_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/StartPointQueueMode_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvStartPointQueueMode_Request {
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
static readControl(r:Reader):Ros2Motoros2_interfacesSrvStartPointQueueMode_Request{const value=new Ros2Motoros2_interfacesSrvStartPointQueueMode_Request();
return value;}
}
export class Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request>{
static readonly codecIdentity:string="UWVMROS1_58a4827fc6535ef0ffc64ee25c5c4af75aa0fe199fa0350a88a95d2e991164a1";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvStartPointQueueMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvStartPointQueueMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvStartPointQueueMode_Request():changetype<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/StartPointQueueMode_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvStartPointQueueMode_Response {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar motoros2_interfaces/msg/MotionReadyEnum. */
result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum=new Ros2Motoros2_interfacesMsgMotionReadyEnum();
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.result_code.wireSize();if(child<0)return child;if(child>518-n)return -90;n+=child;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.result_code.writeControl(w);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvStartPointQueueMode_Response{const value=new Ros2Motoros2_interfacesSrvStartPointQueueMode_Response();
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum.readControl(r);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvStartPointQueueMode_Response>{
static readonly codecIdentity:string="UWVMROS1_bc553d5640e1c4644a5d5bdc2dfcefc9dbe789ef1e2a23dd620f08963467b6ff";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvStartPointQueueMode_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvStartPointQueueMode_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvStartPointQueueMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvStartPointQueueMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvStartPointQueueMode_Response():changetype<Ros2Motoros2_interfacesSrvStartPointQueueMode_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/StartTrajMode_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvStartTrajMode_Request {
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
static readControl(r:Reader):Ros2Motoros2_interfacesSrvStartTrajMode_Request{const value=new Ros2Motoros2_interfacesSrvStartTrajMode_Request();
return value;}
}
export class Ros2Motoros2_interfacesSrvStartTrajMode_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvStartTrajMode_Request>{
static readonly codecIdentity:string="UWVMROS1_0b8b2ad5db5901047035e1070b675618afc516a1ae8c3308cfdcad1174d34789";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Motoros2_interfacesSrvStartTrajMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvStartTrajMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvStartTrajMode_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvStartTrajMode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvStartTrajMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvStartTrajMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvStartTrajMode_Request():changetype<Ros2Motoros2_interfacesSrvStartTrajMode_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/StartTrajMode_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvStartTrajMode_Response {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar motoros2_interfaces/msg/MotionReadyEnum. */
result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum=new Ros2Motoros2_interfacesMsgMotionReadyEnum();
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.result_code.wireSize();if(child<0)return child;if(child>518-n)return -90;n+=child;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.result_code.writeControl(w);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvStartTrajMode_Response{const value=new Ros2Motoros2_interfacesSrvStartTrajMode_Response();
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum.readControl(r);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvStartTrajMode_Response>{
static readonly codecIdentity:string="UWVMROS1_170e25737e6a2b06f5d5d9f331d748c97cf0306284271757953a4a801c722939";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Motoros2_interfacesSrvStartTrajMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvStartTrajMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvStartTrajMode_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvStartTrajMode_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvStartTrajMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvStartTrajMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvStartTrajMode_Response():changetype<Ros2Motoros2_interfacesSrvStartTrajMode_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteGroupIO_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteGroupIO_Request {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field address, native scalar uint32. */
address:u32=0;
/** Exact native field value, native scalar uint8. */
value:u8=0;
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
w.integer(<u64>this.address,4);
w.integer(<u64>this.value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteGroupIO_Request{const value=new Ros2Motoros2_interfacesSrvWriteGroupIO_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteGroupIO_Request>{
static readonly codecIdentity:string="UWVMROS1_2d348ed71408a0dd1dd28ee2ed2ca1db56bb07a14546d0fe13ccf64e8553d52c";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteGroupIO_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteGroupIO_Request>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteGroupIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteGroupIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteGroupIO_Request():changetype<Ros2Motoros2_interfacesSrvWriteGroupIO_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteGroupIO_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteGroupIO_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>521-n)return -90;n+=4;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteGroupIO_Response{const value=new Ros2Motoros2_interfacesSrvWriteGroupIO_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteGroupIO_Response>{
static readonly codecIdentity:string="UWVMROS1_06769934711b27fd32cdcbf849504e160189ee82ea32e0e71a2903af47c5d766";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteGroupIO_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteGroupIO_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteGroupIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteGroupIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteGroupIO_Response():changetype<Ros2Motoros2_interfacesSrvWriteGroupIO_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteMRegister_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteMRegister_Request {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field address, native scalar uint32. */
address:u32=0;
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
w.integer(<u64>this.address,4);
w.integer(<u64>this.value,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteMRegister_Request{const value=new Ros2Motoros2_interfacesSrvWriteMRegister_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
{
const decoded=<u16>r.integer(2);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteMRegister_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteMRegister_Request>{
static readonly codecIdentity:string="UWVMROS1_8df4536be27f0632052725d5616fd5b047f49fc306fc7735854d9de4fb15d4ef";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteMRegister_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteMRegister_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteMRegister_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteMRegister_Request>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteMRegister_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteMRegister_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteMRegister_Request():changetype<Ros2Motoros2_interfacesSrvWriteMRegister_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteMRegister_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteMRegister_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>521-n)return -90;n+=4;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteMRegister_Response{const value=new Ros2Motoros2_interfacesSrvWriteMRegister_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteMRegister_Response>{
static readonly codecIdentity:string="UWVMROS1_a61cc0596a7545ef4836bdc64dc9d6b4ed99b0abd18a726eb37e6215f51c729a";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteMRegister_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteMRegister_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteMRegister_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteMRegister_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteMRegister_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteMRegister_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteMRegister_Response():changetype<Ros2Motoros2_interfacesSrvWriteMRegister_Response>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteSingleIO_Request; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteSingleIO_Request {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field address, native scalar uint32. */
address:u32=0;
/** Exact native field value, native scalar int32. */
value:i32=0;
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
w.integer(<u64>this.address,4);
w.integer(<u64>this.value,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteSingleIO_Request{const value=new Ros2Motoros2_interfacesSrvWriteSingleIO_Request();
{
const decoded=<u32>r.integer(4);
value.address=decoded;
}
{
const decoded=<i32>r.integer(4);
value.value=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteSingleIO_Request>{
static readonly codecIdentity:string="UWVMROS1_6361affcd8b7ebaaa1fe6b5806e64a7c18f7a96a1178614cfce487de813a6392";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteSingleIO_Request>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteSingleIO_Request>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteSingleIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteSingleIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteSingleIO_Request():changetype<Ros2Motoros2_interfacesSrvWriteSingleIO_Request>(out.value);}
}
/** Complete original motoros2_interfaces/srv/WriteSingleIO_Response; spellings are published in generation proof. */
export class Ros2Motoros2_interfacesSrvWriteSingleIO_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field result_code, native scalar uint32. */
result_code:u32=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>521-n)return -90;n+=4;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,4);
w.text(this.message);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Motoros2_interfacesSrvWriteSingleIO_Response{const value=new Ros2Motoros2_interfacesSrvWriteSingleIO_Response();
{
const decoded=<u32>r.integer(4);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCodec implements VendorCodec<Ros2Motoros2_interfacesSrvWriteSingleIO_Response>{
static readonly codecIdentity:string="UWVMROS1_12328db372a572fe9b0c7207331a7971d5d7c9033376e3be410db579b85fb794";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Motoros2_interfacesSrvWriteSingleIO_Response>{const out=new VendorDecoded<Ros2Motoros2_interfacesSrvWriteSingleIO_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Motoros2_interfacesSrvWriteSingleIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Motoros2_interfacesSrvWriteSingleIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Motoros2_interfacesSrvWriteSingleIO_Response():changetype<Ros2Motoros2_interfacesSrvWriteSingleIO_Response>(out.value);}
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
/** Complete original tf2_msgs/msg/TFMessage; spellings are published in generation proof. */
export class Ros2Tf2_msgsMsgTFMessage {
static readonly maxWireBytes:i32=10372;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=10372;
/** Exact native field transforms, native scalar geometry_msgs/msg/TransformStamped. */
transforms:Array<Ros2Geometry_msgsMsgTransformStamped>=new Array<Ros2Geometry_msgsMsgTransformStamped>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.transforms.length>16)return -22;
if(4>10372-n)return -90;n+=4;
for(let i=0;i<this.transforms.length;++i){
const child=this.transforms[i].wireSize();if(child<0)return child;if(child>10372-n)return -90;n+=child;
if(n>10372)return -90;
}
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
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Tf2_msgsMsgTFMessage{const value=new Ros2Tf2_msgsMsgTFMessage();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.transforms=new Array<Ros2Geometry_msgsMsgTransformStamped>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTransformStamped.readControl(r);
value.transforms.push(decoded);
}
}
return value;}
}
export class Ros2Tf2_msgsMsgTFMessageCodec implements VendorCodec<Ros2Tf2_msgsMsgTFMessage>{
static readonly codecIdentity:string="UWVMROS1_4428597a9c2783fd8f0f083a16e8fd1b3d286487f4800c55fcc5571e7f79e65a";
static readonly maxWireBytes:i32=10372;
encodeChecked(value:Ros2Tf2_msgsMsgTFMessage):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Tf2_msgsMsgTFMessage):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Tf2_msgsMsgTFMessage>{const out=new VendorDecoded<Ros2Tf2_msgsMsgTFMessage>();if(bytes.length>10372){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Tf2_msgsMsgTFMessage.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Tf2_msgsMsgTFMessage{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Tf2_msgsMsgTFMessage():changetype<Ros2Tf2_msgsMsgTFMessage>(out.value);}
}
