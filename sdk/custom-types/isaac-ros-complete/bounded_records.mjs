/** Generated bounded-v2 canonical codecs. No ROS, filesystem or network imports.
 * Records are ordinary owned values. Decode throws before exposing malformed bytes;
 * encode validates the complete value before allocating its one wire output.
 * 64-bit integers remain BigInt. Strings are strict UTF-8 with no embedded NUL.
 * Owned Service transport is a separate explicit raw12 adapter, not legacy RPC. */
const utf8Encoder=new TextEncoder(),utf8Decoder=new TextDecoder("utf-8",{fatal:true,ignoreBOM:true});
export class ControlCodecError extends Error{constructor(code){super("invalid original-control canonical value");this.code=code;}}
function bad(code=22){throw new ControlCodecError(code);}
const nativeTypedLength=Object.getOwnPropertyDescriptor(Object.getPrototypeOf(Uint8Array.prototype),"length").get,nativeTypedSet=Uint8Array.prototype.set;
function copyRaw(value,bound,code=22){if(!(value instanceof Uint8Array))bad();const n=nativeTypedLength.call(value);if(n>bound)bad(code);const out=new Uint8Array(n);nativeTypedSet.call(out,value);return out;}
function addSize(n,m){if(!Number.isSafeInteger(n)||!Number.isSafeInteger(m)||n<0||m<0||n>32768||m>32768-n)bad(90);return n+m;}
function textBytes(value,bound){if(typeof value!=="string"||value.length>bound||value.includes("\0"))bad();for(let i=0;i<value.length;++i){const c=value.charCodeAt(i);if(c>=0xd800&&c<=0xdbff){if(++i===value.length)bad();const d=value.charCodeAt(i);if(d<0xdc00||d>0xdfff)bad();}else if(c>=0xdc00&&c<=0xdfff)bad();}const b=utf8Encoder.encode(value);if(b.length>bound)bad();return b;}
function integer(value,width,signed){if(width===8){if(typeof value!=="bigint"||value<(signed?-(1n<<63n):0n)||value>(signed?(1n<<63n)-1n:(1n<<64n)-1n))bad();}else if(!Number.isInteger(value)||value<(signed?-(2**(width*8-1)):0)||value>(signed?2**(width*8-1)-1:2**(width*8)-1))bad();}
class Reader{constructor(bytes){if(!(bytes instanceof Uint8Array)||bytes.length>32768)bad();this.b=bytes;this.v=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);this.at=0;}take(n){if(!Number.isInteger(n)||n<0||n>this.b.length-this.at)bad();const p=this.at;this.at+=n;return p;}u32(){return this.v.getUint32(this.take(4),true);}text(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n);let s;try{s=utf8Decoder.decode(this.b.subarray(p,p+n));}catch{bad();}if(s.includes("\0"))bad();return s;}raw(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n);return this.b.slice(p,p+n);}}
class Writer{constructor(size){if(size>32768)bad();this.b=new Uint8Array(size);this.v=new DataView(this.b.buffer);this.at=0;}text(value,bound){const b=textBytes(value,bound);this.v.setUint32(this.at,b.length,true);this.at+=4;this.b.set(b,this.at);this.at+=b.length;}raw(value){this.v.setUint32(this.at,value.length,true);this.at+=4;this.b.set(value,this.at);this.at+=value.length;}}
/** Complete ROS2 builtin_interfaces/msg/Time codec; maxima are not actual lengths. */
export const Ros2Builtin_interfacesMsgTimeCodec=Object.freeze({codecIdentity:"UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nanosec":0};},
validateValue(value){Ros2Builtin_interfacesMsgTimeSize(Ros2Builtin_interfacesMsgTimeCapture(value));return true;},
encode(value){const owned=Ros2Builtin_interfacesMsgTimeCapture(value),n=Ros2Builtin_interfacesMsgTimeSize(owned),w=new Writer(n);Ros2Builtin_interfacesMsgTimeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Builtin_interfacesMsgTimeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Builtin_interfacesMsgTimeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nanosec"],4,false);n=addSize(n,4);
if(value["nanosec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros2Builtin_interfacesMsgTimeWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nanosec"],true);w.at+=4;
}
function Ros2Builtin_interfacesMsgTimeRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["nanosec"]=decoded;
}
if(value["nanosec"]>=1000000000)bad();
return value;}
function Ros2Builtin_interfacesMsgTimeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nanosec"];
out["nanosec"]=field;
}
return out;}
/** Complete ROS2 std_msgs/msg/Header codec; maxima are not actual lengths. */
export const Ros2Std_msgsMsgHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396",maxWireBytes:76,minWireBytes:12,cartesianMaxWireBytes:76n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":Ros2Builtin_interfacesMsgTimeCodec.defaultValue(),"frame_id":""};},
validateValue(value){Ros2Std_msgsMsgHeaderSize(Ros2Std_msgsMsgHeaderCapture(value));return true;},
encode(value){const owned=Ros2Std_msgsMsgHeaderCapture(value),n=Ros2Std_msgsMsgHeaderSize(owned),w=new Writer(n);Ros2Std_msgsMsgHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,76,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Std_msgsMsgHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_msgsMsgHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Builtin_interfacesMsgTimeSize(value["stamp"]));
n=addSize(n,4+textBytes(value["frame_id"],64).length);
if(n>76)bad(90);return n;}
function Ros2Std_msgsMsgHeaderWrite(value,w){
Ros2Builtin_interfacesMsgTimeWrite(value["stamp"],w);
w.text(value["frame_id"],64);
}
function Ros2Std_msgsMsgHeaderRead(r){const value={};
{
const decoded=Ros2Builtin_interfacesMsgTimeRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.text(64);
value["frame_id"]=decoded;
}
return value;}
function Ros2Std_msgsMsgHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["stamp"];
out["stamp"]=Ros2Builtin_interfacesMsgTimeCapture(field);
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS2 geometry_msgs/msg/Point codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgPointCodec=Object.freeze({codecIdentity:"UWVMROS1_80f43d054abe333673c940c6d0fd88e8e26cea4a1cff53ff789cccaa38240616",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros2Geometry_msgsMsgPointSize(Ros2Geometry_msgsMsgPointCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgPointCapture(value),n=Ros2Geometry_msgsMsgPointSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros2Geometry_msgsMsgPointWrite(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
}
function Ros2Geometry_msgsMsgPointRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["x"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["y"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["z"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
return out;}
/** Complete ROS2 geometry_msgs/msg/Quaternion codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgQuaternionCodec=Object.freeze({codecIdentity:"UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0.0,"y":0.0,"z":0.0,"w":1.0};},
validateValue(value){Ros2Geometry_msgsMsgQuaternionSize(Ros2Geometry_msgsMsgQuaternionCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgQuaternionCapture(value),n=Ros2Geometry_msgsMsgQuaternionSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgQuaternionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgQuaternionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgQuaternionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"]))bad();n=addSize(n,8);
if(n>32)bad(90);return n;}
function Ros2Geometry_msgsMsgQuaternionWrite(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
w.v.setFloat64(w.at,value["w"],true);w.at+=8;
}
function Ros2Geometry_msgsMsgQuaternionRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["x"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["y"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["z"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgQuaternionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["w"];
out["w"]=field;
}
return out;}
/** Complete ROS2 geometry_msgs/msg/Pose codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgPoseCodec=Object.freeze({codecIdentity:"UWVMROS1_9468a10b8b2728fe71a370b4348ffe5e81b884ff957533f8700aa16e70b12777",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"position":Ros2Geometry_msgsMsgPointCodec.defaultValue(),"orientation":Ros2Geometry_msgsMsgQuaternionCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgPoseSize(Ros2Geometry_msgsMsgPoseCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgPoseCapture(value),n=Ros2Geometry_msgsMsgPoseSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgPoseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgPoseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgPoseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgPointSize(value["position"]));
n=addSize(n,Ros2Geometry_msgsMsgQuaternionSize(value["orientation"]));
if(n>56)bad(90);return n;}
function Ros2Geometry_msgsMsgPoseWrite(value,w){
Ros2Geometry_msgsMsgPointWrite(value["position"],w);
Ros2Geometry_msgsMsgQuaternionWrite(value["orientation"],w);
}
function Ros2Geometry_msgsMsgPoseRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgPointRead(r);
value["position"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternionRead(r);
value["orientation"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgPoseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["position"];
out["position"]=Ros2Geometry_msgsMsgPointCapture(field);
}
{const field=value["orientation"];
out["orientation"]=Ros2Geometry_msgsMsgQuaternionCapture(field);
}
return out;}
/** Complete ROS2 geometry_msgs/msg/PoseWithCovariance codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgPoseWithCovarianceCodec=Object.freeze({codecIdentity:"UWVMROS1_8eb3e25c45769eb29691011c05315ed50f2b97eff488f9e8ee2dc70bddab4dc2",maxWireBytes:344,minWireBytes:344,cartesianMaxWireBytes:344n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":Ros2Geometry_msgsMsgPoseCodec.defaultValue(),"covariance":Array.from({length:36},()=>(0))};},
validateValue(value){Ros2Geometry_msgsMsgPoseWithCovarianceSize(Ros2Geometry_msgsMsgPoseWithCovarianceCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgPoseWithCovarianceCapture(value),n=Ros2Geometry_msgsMsgPoseWithCovarianceSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgPoseWithCovarianceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,344,90);if(owned.length<344)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgPoseWithCovarianceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgPoseWithCovarianceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgPoseSize(value["pose"]));
if(!Array.isArray(value["covariance"])||value["covariance"].length!==36)bad();
for(const element of value["covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>344)bad(90);return n;}
function Ros2Geometry_msgsMsgPoseWithCovarianceWrite(value,w){
Ros2Geometry_msgsMsgPoseWrite(value["pose"],w);
for(const element of value["covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Geometry_msgsMsgPoseWithCovarianceRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgPoseRead(r);
value["pose"]=decoded;
}
{
{const count=36;if(count>36)bad();value["covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["covariance"].push(decoded);
}}
}
return value;}
function Ros2Geometry_msgsMsgPoseWithCovarianceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseCapture(field);
}
{const field=value["covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==36)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["covariance"]=values;
}
return out;}
/** Complete ROS2 geometry_msgs/msg/PoseWithCovarianceStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgPoseWithCovarianceStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_ea380172cde0ee5fef319aff50d92a80c314510e3d12efec45e9ae1f74974af3",maxWireBytes:420,minWireBytes:356,cartesianMaxWireBytes:420n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"pose":Ros2Geometry_msgsMsgPoseWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgPoseWithCovarianceStampedSize(Ros2Geometry_msgsMsgPoseWithCovarianceStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgPoseWithCovarianceStampedCapture(value),n=Ros2Geometry_msgsMsgPoseWithCovarianceStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgPoseWithCovarianceStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,420,90);if(owned.length<356)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgPoseWithCovarianceStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgPoseWithCovarianceStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseWithCovarianceSize(value["pose"]));
if(n>420)bad(90);return n;}
function Ros2Geometry_msgsMsgPoseWithCovarianceStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgPoseWithCovarianceWrite(value["pose"],w);
}
function Ros2Geometry_msgsMsgPoseWithCovarianceStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovarianceRead(r);
value["pose"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgPoseWithCovarianceStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseWithCovarianceCapture(field);
}
return out;}
/** Complete ROS2 isaac_ros_apriltag_interfaces/msg/AprilTagDetection codec; maxima are not actual lengths. */
export const Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCodec=Object.freeze({codecIdentity:"UWVMROS1_5d840f0d9239a948ce5d17bbee6e248db8151ff31070667e506e9c1647ac6fe7",maxWireBytes:612,minWireBytes:484,cartesianMaxWireBytes:612n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"family":"","id":0,"center":Ros2Geometry_msgsMsgPointCodec.defaultValue(),"corners":Array.from({length:4},()=>(Ros2Geometry_msgsMsgPointCodec.defaultValue())),"pose":Ros2Geometry_msgsMsgPoseWithCovarianceStampedCodec.defaultValue()};},
validateValue(value){Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionSize(Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCapture(value));return true;},
encode(value){const owned=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCapture(value),n=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionSize(owned),w=new Writer(n);Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,612,90);if(owned.length<484)bad();const r=new Reader(owned),value=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["family"],64).length);
integer(value["id"],4,true);n=addSize(n,4);
n=addSize(n,Ros2Geometry_msgsMsgPointSize(value["center"]));
if(!Array.isArray(value["corners"])||value["corners"].length!==4)bad();
for(const element of value["corners"]){
n=addSize(n,Ros2Geometry_msgsMsgPointSize(element));
}
n=addSize(n,Ros2Geometry_msgsMsgPoseWithCovarianceStampedSize(value["pose"]));
if(n>612)bad(90);return n;}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionWrite(value,w){
w.text(value["family"],64);
w.v.setInt32(w.at,value["id"],true);w.at+=4;
Ros2Geometry_msgsMsgPointWrite(value["center"],w);
for(const element of value["corners"]){
Ros2Geometry_msgsMsgPointWrite(element,w);
}
Ros2Geometry_msgsMsgPoseWithCovarianceStampedWrite(value["pose"],w);
}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionRead(r){const value={};
{
const decoded=r.text(64);
value["family"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["id"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPointRead(r);
value["center"]=decoded;
}
{
{const count=4;if(count>4)bad();value["corners"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgPointRead(r);
value["corners"].push(decoded);
}}
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovarianceStampedRead(r);
value["pose"]=decoded;
}
return value;}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["family"];
out["family"]=field;
}
{const field=value["id"];
out["id"]=field;
}
{const field=value["center"];
out["center"]=Ros2Geometry_msgsMsgPointCapture(field);
}
{const field=value["corners"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgPointCapture(element);
}out["corners"]=values;
}
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseWithCovarianceStampedCapture(field);
}
return out;}
/** Complete ROS2 isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray codec; maxima are not actual lengths. */
export const Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayCodec=Object.freeze({codecIdentity:"UWVMROS1_60359e405aedd2a627e60bc35e849989fe5ed1d426afb2a884036aef93bfe0a5",maxWireBytes:9872,minWireBytes:16,cartesianMaxWireBytes:9872n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"detections":[]};},
validateValue(value){Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArraySize(Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayCapture(value));return true;},
encode(value){const owned=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayCapture(value),n=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArraySize(owned),w=new Writer(n);Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9872,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArraySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["detections"])||value["detections"].length>16)bad();
n=addSize(n,4);
for(const element of value["detections"]){
n=addSize(n,Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionSize(element));
}
if(n>9872)bad(90);return n;}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["detections"].length,true);w.at+=4;
for(const element of value["detections"]){
Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionWrite(element,w);
}
}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["detections"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionRead(r);
value["detections"].push(decoded);
}}
}
return value;}
function Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["detections"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCapture(element);
}out["detections"]=values;
}
return out;}
