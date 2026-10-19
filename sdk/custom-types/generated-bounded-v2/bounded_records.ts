/** Generated bounded-v2 canonical values; no native I/O or authority.
 * Move only the relative SDK module path if relocating this generated directory.
 * Managed values are encoded into linear-memory byte arrays before an import.
 * valid() must succeed before exposing decode(); malformed decode returns a default. */
import {Codec,Reader,Writer,validUnicode,validUtf8} from "../../assemblyscript/codecs";
/** Complete ROS1 uwvm_ros_bounded_v2/Point managed value. */
export class Ros1Uwvm_ros_bounded_v2Point {
/** Declared value; initialized elements alone are serialized. */
value:i32=0;
/** Declared enabled; initialized elements alone are serialized. */
enabled:bool=false;
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.integer(<u64>this.value,4);
w.u8(this.enabled?1:0);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2Point {const value=new Ros1Uwvm_ros_bounded_v2Point();
{
const decoded=<i32>r.integer(4);
value.value=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.enabled=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/Point; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2PointCodec implements Codec<Ros1Uwvm_ros_bounded_v2Point> {
static readonly codecIdentity:string="UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28";
static readonly maxWireBytes:i32=5;
encode(value:Ros1Uwvm_ros_bounded_v2Point):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(5);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<5||bytes.length>5)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2Point.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2Point {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2Point();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2Point.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/Envelope managed value. */
export class Ros1Uwvm_ros_bounded_v2Envelope {
/** Declared label; initialized elements alone are serialized. */
label:string="";
/** Declared samples; initialized elements alone are serialized. */
samples:Array<u16>=new Array<u16>();
/** Declared points; initialized elements alone are serialized. */
points:Array<Ros1Uwvm_ros_bounded_v2Point>=new Array<Ros1Uwvm_ros_bounded_v2Point>();
/** Declared labels; initialized elements alone are serialized. */
labels:Array<string>=new Array<string>();
constructor(){
for(let i=0;i<2;++i)this.labels.push("");
}
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.label)||this.label.indexOf("\0")>=0||String.UTF8.byteLength(this.label)>32)return false;
if(this.samples.length>8)return false;
for(let i=0;i<this.samples.length;++i){
}
if(this.points.length>4)return false;
for(let i=0;i<this.points.length;++i){
if(!this.points[i].validValue())return false;
}
if(this.labels.length!=2)return false;
for(let i=0;i<this.labels.length;++i){
if(!validUnicode(this.labels[i])||this.labels[i].indexOf("\0")>=0||String.UTF8.byteLength(this.labels[i])>16)return false;
}
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.text(this.label);
w.u32(<u32>this.samples.length);
for(let i=0;i<this.samples.length;++i){
w.integer(<u64>this.samples[i],2);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeBounded(w);
}
for(let i=0;i<this.labels.length;++i){
w.text(this.labels[i]);
}
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2Envelope {const value=new Ros1Uwvm_ros_bounded_v2Envelope();
{
const n=r.u32();if(n>32){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.label=decoded;
}
{
const count=r.u32();if(!r.valid||count>8){r.valid=false;return value;}
value.samples=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.samples.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.points=new Array<Ros1Uwvm_ros_bounded_v2Point>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Uwvm_ros_bounded_v2Point.readBounded(r);
value.points.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.labels=new Array<string>();for(let i:u32=0;i<count;++i){
const n=r.u32();if(n>16){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.labels.push(decoded);
}
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/Envelope; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2EnvelopeCodec implements Codec<Ros1Uwvm_ros_bounded_v2Envelope> {
static readonly codecIdentity:string="UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4";
static readonly maxWireBytes:i32=120;
encode(value:Ros1Uwvm_ros_bounded_v2Envelope):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(120);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<20||bytes.length>120)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2Envelope.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2Envelope {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2Envelope();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2Envelope.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoRequest managed value. */
export class Ros1Uwvm_ros_bounded_v2EchoRequest {
/** Declared sample; initialized elements alone are serialized. */
sample:Ros1Uwvm_ros_bounded_v2Envelope=new Ros1Uwvm_ros_bounded_v2Envelope();
/** Declared limit; initialized elements alone are serialized. */
limit:u32=0;
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.sample.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
this.sample.writeBounded(w);
w.integer(<u64>this.limit,4);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2EchoRequest {const value=new Ros1Uwvm_ros_bounded_v2EchoRequest();
{
const decoded=Ros1Uwvm_ros_bounded_v2Envelope.readBounded(r);
value.sample=decoded;
}
{
const decoded=<u32>r.integer(4);
value.limit=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/EchoRequest; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2EchoRequestCodec implements Codec<Ros1Uwvm_ros_bounded_v2EchoRequest> {
static readonly codecIdentity:string="UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420";
static readonly maxWireBytes:i32=124;
encode(value:Ros1Uwvm_ros_bounded_v2EchoRequest):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(124);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<24||bytes.length>124)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2EchoRequest.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2EchoRequest {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2EchoRequest();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2EchoRequest.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoResponse managed value. */
export class Ros1Uwvm_ros_bounded_v2EchoResponse {
/** Declared accepted; initialized elements alone are serialized. */
accepted:bool=false;
/** Declared echoed; initialized elements alone are serialized. */
echoed:Ros1Uwvm_ros_bounded_v2Envelope=new Ros1Uwvm_ros_bounded_v2Envelope();
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.echoed.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.u8(this.accepted?1:0);
this.echoed.writeBounded(w);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2EchoResponse {const value=new Ros1Uwvm_ros_bounded_v2EchoResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.accepted=decoded;
}
{
const decoded=Ros1Uwvm_ros_bounded_v2Envelope.readBounded(r);
value.echoed=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/EchoResponse; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2EchoResponseCodec implements Codec<Ros1Uwvm_ros_bounded_v2EchoResponse> {
static readonly codecIdentity:string="UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b";
static readonly maxWireBytes:i32=121;
encode(value:Ros1Uwvm_ros_bounded_v2EchoResponse):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(121);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<21||bytes.length>121)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2EchoResponse.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2EchoResponse {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2EchoResponse();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2EchoResponse.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyRequest managed value. */
export class Ros1Uwvm_ros_bounded_v2EmptyRequest {
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2EmptyRequest {const value=new Ros1Uwvm_ros_bounded_v2EmptyRequest();
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/EmptyRequest; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2EmptyRequestCodec implements Codec<Ros1Uwvm_ros_bounded_v2EmptyRequest> {
static readonly codecIdentity:string="UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40";
static readonly maxWireBytes:i32=0;
encode(value:Ros1Uwvm_ros_bounded_v2EmptyRequest):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(0);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<0||bytes.length>0)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2EmptyRequest.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2EmptyRequest {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2EmptyRequest();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2EmptyRequest.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyResponse managed value. */
export class Ros1Uwvm_ros_bounded_v2EmptyResponse {
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2EmptyResponse {const value=new Ros1Uwvm_ros_bounded_v2EmptyResponse();
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/EmptyResponse; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2EmptyResponseCodec implements Codec<Ros1Uwvm_ros_bounded_v2EmptyResponse> {
static readonly codecIdentity:string="UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817";
static readonly maxWireBytes:i32=0;
encode(value:Ros1Uwvm_ros_bounded_v2EmptyResponse):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(0);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<0||bytes.length>0)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2EmptyResponse.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2EmptyResponse {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2EmptyResponse();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2EmptyResponse.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestGoal managed value. */
export class Ros1Uwvm_ros_bounded_v2DigestGoal {
/** Declared sample; initialized elements alone are serialized. */
sample:Ros1Uwvm_ros_bounded_v2Envelope=new Ros1Uwvm_ros_bounded_v2Envelope();
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.sample.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
this.sample.writeBounded(w);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2DigestGoal {const value=new Ros1Uwvm_ros_bounded_v2DigestGoal();
{
const decoded=Ros1Uwvm_ros_bounded_v2Envelope.readBounded(r);
value.sample=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/DigestGoal; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2DigestGoalCodec implements Codec<Ros1Uwvm_ros_bounded_v2DigestGoal> {
static readonly codecIdentity:string="UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc";
static readonly maxWireBytes:i32=120;
encode(value:Ros1Uwvm_ros_bounded_v2DigestGoal):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(120);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<20||bytes.length>120)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2DigestGoal.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2DigestGoal {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2DigestGoal();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2DigestGoal.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestResult managed value. */
export class Ros1Uwvm_ros_bounded_v2DigestResult {
/** Declared accepted; initialized elements alone are serialized. */
accepted:bool=false;
/** Declared sum; initialized elements alone are serialized. */
sum:u32=0;
/** Declared note; initialized elements alone are serialized. */
note:string="";
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.note)||this.note.indexOf("\0")>=0||String.UTF8.byteLength(this.note)>32)return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.u8(this.accepted?1:0);
w.integer(<u64>this.sum,4);
w.text(this.note);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2DigestResult {const value=new Ros1Uwvm_ros_bounded_v2DigestResult();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.accepted=decoded;
}
{
const decoded=<u32>r.integer(4);
value.sum=decoded;
}
{
const n=r.u32();if(n>32){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.note=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/DigestResult; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2DigestResultCodec implements Codec<Ros1Uwvm_ros_bounded_v2DigestResult> {
static readonly codecIdentity:string="UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd";
static readonly maxWireBytes:i32=41;
encode(value:Ros1Uwvm_ros_bounded_v2DigestResult):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(41);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<9||bytes.length>41)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2DigestResult.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2DigestResult {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2DigestResult();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2DigestResult.readBounded(r);}
}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestFeedback managed value. */
export class Ros1Uwvm_ros_bounded_v2DigestFeedback {
/** Declared processed; initialized elements alone are serialized. */
processed:u32=0;
/** Declared phase; initialized elements alone are serialized. */
phase:string="";
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.phase)||this.phase.indexOf("\0")>=0||String.UTF8.byteLength(this.phase)>16)return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.integer(<u64>this.processed,4);
w.text(this.phase);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros1Uwvm_ros_bounded_v2DigestFeedback {const value=new Ros1Uwvm_ros_bounded_v2DigestFeedback();
{
const decoded=<u32>r.integer(4);
value.processed=decoded;
}
{
const n=r.u32();if(n>16){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.phase=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/DigestFeedback; no native registration is implied. */
export class Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec implements Codec<Ros1Uwvm_ros_bounded_v2DigestFeedback> {
static readonly codecIdentity:string="UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619";
static readonly maxWireBytes:i32=24;
encode(value:Ros1Uwvm_ros_bounded_v2DigestFeedback):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(24);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<8||bytes.length>24)return false;const r=new Reader(bytes);Ros1Uwvm_ros_bounded_v2DigestFeedback.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros1Uwvm_ros_bounded_v2DigestFeedback {if(!this.valid(bytes))return new Ros1Uwvm_ros_bounded_v2DigestFeedback();const r=new Reader(bytes);return Ros1Uwvm_ros_bounded_v2DigestFeedback.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Point managed value. */
export class Ros2Uwvm_ros_bounded_v2MsgPoint {
/** Declared value; initialized elements alone are serialized. */
value:i32=0;
/** Declared enabled; initialized elements alone are serialized. */
enabled:bool=false;
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.integer(<u64>this.value,4);
w.u8(this.enabled?1:0);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2MsgPoint {const value=new Ros2Uwvm_ros_bounded_v2MsgPoint();
{
const decoded=<i32>r.integer(4);
value.value=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.enabled=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/msg/Point; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2MsgPointCodec implements Codec<Ros2Uwvm_ros_bounded_v2MsgPoint> {
static readonly codecIdentity:string="UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333";
static readonly maxWireBytes:i32=5;
encode(value:Ros2Uwvm_ros_bounded_v2MsgPoint):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(5);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<5||bytes.length>5)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2MsgPoint.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2MsgPoint {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2MsgPoint();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2MsgPoint.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Envelope managed value. */
export class Ros2Uwvm_ros_bounded_v2MsgEnvelope {
/** Declared label; initialized elements alone are serialized. */
label:string="";
/** Declared samples; initialized elements alone are serialized. */
samples:Array<u16>=new Array<u16>();
/** Declared points; initialized elements alone are serialized. */
points:Array<Ros2Uwvm_ros_bounded_v2MsgPoint>=new Array<Ros2Uwvm_ros_bounded_v2MsgPoint>();
/** Declared labels; initialized elements alone are serialized. */
labels:Array<string>=new Array<string>();
constructor(){
for(let i=0;i<2;++i)this.labels.push("");
}
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.label)||this.label.indexOf("\0")>=0||String.UTF8.byteLength(this.label)>32)return false;
if(this.samples.length>8)return false;
for(let i=0;i<this.samples.length;++i){
}
if(this.points.length>4)return false;
for(let i=0;i<this.points.length;++i){
if(!this.points[i].validValue())return false;
}
if(this.labels.length!=2)return false;
for(let i=0;i<this.labels.length;++i){
if(!validUnicode(this.labels[i])||this.labels[i].indexOf("\0")>=0||String.UTF8.byteLength(this.labels[i])>16)return false;
}
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.text(this.label);
w.u32(<u32>this.samples.length);
for(let i=0;i<this.samples.length;++i){
w.integer(<u64>this.samples[i],2);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeBounded(w);
}
for(let i=0;i<this.labels.length;++i){
w.text(this.labels[i]);
}
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2MsgEnvelope {const value=new Ros2Uwvm_ros_bounded_v2MsgEnvelope();
{
const n=r.u32();if(n>32){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.label=decoded;
}
{
const count=r.u32();if(!r.valid||count>8){r.valid=false;return value;}
value.samples=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.samples.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.points=new Array<Ros2Uwvm_ros_bounded_v2MsgPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_ros_bounded_v2MsgPoint.readBounded(r);
value.points.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.labels=new Array<string>();for(let i:u32=0;i<count;++i){
const n=r.u32();if(n>16){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.labels.push(decoded);
}
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/msg/Envelope; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec implements Codec<Ros2Uwvm_ros_bounded_v2MsgEnvelope> {
static readonly codecIdentity:string="UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36";
static readonly maxWireBytes:i32=120;
encode(value:Ros2Uwvm_ros_bounded_v2MsgEnvelope):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(120);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<20||bytes.length>120)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2MsgEnvelope.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2MsgEnvelope {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2MsgEnvelope();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2MsgEnvelope.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Request managed value. */
export class Ros2Uwvm_ros_bounded_v2SrvEcho_Request {
/** Declared sample; initialized elements alone are serialized. */
sample:Ros2Uwvm_ros_bounded_v2MsgEnvelope=new Ros2Uwvm_ros_bounded_v2MsgEnvelope();
/** Declared limit; initialized elements alone are serialized. */
limit:u32=0;
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.sample.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
this.sample.writeBounded(w);
w.integer(<u64>this.limit,4);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2SrvEcho_Request {const value=new Ros2Uwvm_ros_bounded_v2SrvEcho_Request();
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope.readBounded(r);
value.sample=decoded;
}
{
const decoded=<u32>r.integer(4);
value.limit=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/srv/Echo_Request; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2SrvEcho_RequestCodec implements Codec<Ros2Uwvm_ros_bounded_v2SrvEcho_Request> {
static readonly codecIdentity:string="UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3";
static readonly maxWireBytes:i32=124;
encode(value:Ros2Uwvm_ros_bounded_v2SrvEcho_Request):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(124);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<24||bytes.length>124)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2SrvEcho_Request.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2SrvEcho_Request {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2SrvEcho_Request();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2SrvEcho_Request.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Response managed value. */
export class Ros2Uwvm_ros_bounded_v2SrvEcho_Response {
/** Declared accepted; initialized elements alone are serialized. */
accepted:bool=false;
/** Declared echoed; initialized elements alone are serialized. */
echoed:Ros2Uwvm_ros_bounded_v2MsgEnvelope=new Ros2Uwvm_ros_bounded_v2MsgEnvelope();
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.echoed.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.u8(this.accepted?1:0);
this.echoed.writeBounded(w);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2SrvEcho_Response {const value=new Ros2Uwvm_ros_bounded_v2SrvEcho_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.accepted=decoded;
}
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope.readBounded(r);
value.echoed=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/srv/Echo_Response; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseCodec implements Codec<Ros2Uwvm_ros_bounded_v2SrvEcho_Response> {
static readonly codecIdentity:string="UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5";
static readonly maxWireBytes:i32=121;
encode(value:Ros2Uwvm_ros_bounded_v2SrvEcho_Response):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(121);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<21||bytes.length>121)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2SrvEcho_Response.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2SrvEcho_Response {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2SrvEcho_Response();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2SrvEcho_Response.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Request managed value. */
export class Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {const value=new Ros2Uwvm_ros_bounded_v2SrvEmpty_Request();
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/srv/Empty_Request; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestCodec implements Codec<Ros2Uwvm_ros_bounded_v2SrvEmpty_Request> {
static readonly codecIdentity:string="UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4";
static readonly maxWireBytes:i32=0;
encode(value:Ros2Uwvm_ros_bounded_v2SrvEmpty_Request):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(0);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<0||bytes.length>0)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2SrvEmpty_Request.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2SrvEmpty_Request();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2SrvEmpty_Request.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Response managed value. */
export class Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {const value=new Ros2Uwvm_ros_bounded_v2SrvEmpty_Response();
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/srv/Empty_Response; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseCodec implements Codec<Ros2Uwvm_ros_bounded_v2SrvEmpty_Response> {
static readonly codecIdentity:string="UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b";
static readonly maxWireBytes:i32=0;
encode(value:Ros2Uwvm_ros_bounded_v2SrvEmpty_Response):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(0);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<0||bytes.length>0)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2SrvEmpty_Response.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2SrvEmpty_Response();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2SrvEmpty_Response.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Goal managed value. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {
/** Declared sample; initialized elements alone are serialized. */
sample:Ros2Uwvm_ros_bounded_v2MsgEnvelope=new Ros2Uwvm_ros_bounded_v2MsgEnvelope();
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!this.sample.validValue())return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
this.sample.writeBounded(w);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {const value=new Ros2Uwvm_ros_bounded_v2ActionDigest_Goal();
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope.readBounded(r);
value.sample=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/action/Digest_Goal; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_GoalCodec implements Codec<Ros2Uwvm_ros_bounded_v2ActionDigest_Goal> {
static readonly codecIdentity:string="UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c";
static readonly maxWireBytes:i32=120;
encode(value:Ros2Uwvm_ros_bounded_v2ActionDigest_Goal):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(120);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<20||bytes.length>120)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2ActionDigest_Goal.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2ActionDigest_Goal();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2ActionDigest_Goal.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Result managed value. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_Result {
/** Declared accepted; initialized elements alone are serialized. */
accepted:bool=false;
/** Declared sum; initialized elements alone are serialized. */
sum:u32=0;
/** Declared note; initialized elements alone are serialized. */
note:string="";
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.note)||this.note.indexOf("\0")>=0||String.UTF8.byteLength(this.note)>32)return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.u8(this.accepted?1:0);
w.integer(<u64>this.sum,4);
w.text(this.note);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2ActionDigest_Result {const value=new Ros2Uwvm_ros_bounded_v2ActionDigest_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.accepted=decoded;
}
{
const decoded=<u32>r.integer(4);
value.sum=decoded;
}
{
const n=r.u32();if(n>32){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.note=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/action/Digest_Result; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_ResultCodec implements Codec<Ros2Uwvm_ros_bounded_v2ActionDigest_Result> {
static readonly codecIdentity:string="UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da";
static readonly maxWireBytes:i32=41;
encode(value:Ros2Uwvm_ros_bounded_v2ActionDigest_Result):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(41);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<9||bytes.length>41)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2ActionDigest_Result.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2ActionDigest_Result {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2ActionDigest_Result();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2ActionDigest_Result.readBounded(r);}
}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Feedback managed value. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {
/** Declared processed; initialized elements alone are serialized. */
processed:u32=0;
/** Declared phase; initialized elements alone are serialized. */
phase:string="";
/** Verify UTF-16/scalar/container limits before any wire allocation. */
validValue():bool {
if(!validUnicode(this.phase)||this.phase.indexOf("\0")>=0||String.UTF8.byteLength(this.phase)>16)return false;
return true;}
/** Internal shared writer; the public codec first validates the complete value. */
writeBounded(w:Writer):void {
w.integer(<u64>this.processed,4);
w.text(this.phase);
}
/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */
static readBounded(r:Reader):Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {const value=new Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback();
{
const decoded=<u32>r.integer(4);
value.processed=decoded;
}
{
const n=r.u32();if(n>16){r.valid=false;return value;}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(b.buffer);
value.phase=decoded;
}
return value;}
}
/** SDK typed Codec for uwvm_ros_bounded_v2/action/Digest_Feedback; no native registration is implied. */
export class Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackCodec implements Codec<Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback> {
static readonly codecIdentity:string="UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1";
static readonly maxWireBytes:i32=24;
encode(value:Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback):Uint8Array {if(!value.validValue())return new Uint8Array(0);const w=new Writer(24);value.writeBounded(w);return w.finish();}
valid(bytes:Uint8Array):bool {if(bytes.length<8||bytes.length>24)return false;const r=new Reader(bytes);Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback.readBounded(r);return r.done();}
decode(bytes:Uint8Array):Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {if(!this.valid(bytes))return new Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback();const r=new Reader(bytes);return Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback.readBounded(r);}
}
