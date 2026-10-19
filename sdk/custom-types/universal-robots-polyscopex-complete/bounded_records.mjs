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
/** Complete ROS2 geometry_msgs/msg/Vector3 codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgVector3Codec=Object.freeze({codecIdentity:"UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros2Geometry_msgsMsgVector3Size(Ros2Geometry_msgsMsgVector3Capture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgVector3Capture(value),n=Ros2Geometry_msgsMsgVector3Size(owned),w=new Writer(n);Ros2Geometry_msgsMsgVector3Write(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgVector3Read(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgVector3Size(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros2Geometry_msgsMsgVector3Write(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
}
function Ros2Geometry_msgsMsgVector3Read(r){const value={};
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
function Ros2Geometry_msgsMsgVector3Capture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 geometry_msgs/msg/Twist codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTwistCodec=Object.freeze({codecIdentity:"UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"linear":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"angular":Ros2Geometry_msgsMsgVector3Codec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTwistSize(Ros2Geometry_msgsMsgTwistCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTwistCapture(value),n=Ros2Geometry_msgsMsgTwistSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTwistWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTwistRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTwistSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["linear"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["angular"]));
if(n>48)bad(90);return n;}
function Ros2Geometry_msgsMsgTwistWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["linear"],w);
Ros2Geometry_msgsMsgVector3Write(value["angular"],w);
}
function Ros2Geometry_msgsMsgTwistRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["linear"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["angular"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTwistCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["linear"];
out["linear"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["angular"];
out["angular"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
return out;}
/** Complete ROS2 std_msgs/msg/Header codec; maxima are not actual lengths. */
export const Ros2Std_msgsMsgHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96",maxWireBytes:140,minWireBytes:12,cartesianMaxWireBytes:140n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":Ros2Builtin_interfacesMsgTimeCodec.defaultValue(),"frame_id":""};},
validateValue(value){Ros2Std_msgsMsgHeaderSize(Ros2Std_msgsMsgHeaderCapture(value));return true;},
encode(value){const owned=Ros2Std_msgsMsgHeaderCapture(value),n=Ros2Std_msgsMsgHeaderSize(owned),w=new Writer(n);Ros2Std_msgsMsgHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,140,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Std_msgsMsgHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_msgsMsgHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Builtin_interfacesMsgTimeSize(value["stamp"]));
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(n>140)bad(90);return n;}
function Ros2Std_msgsMsgHeaderWrite(value,w){
Ros2Builtin_interfacesMsgTimeWrite(value["stamp"],w);
w.text(value["frame_id"],128);
}
function Ros2Std_msgsMsgHeaderRead(r){const value={};
{
const decoded=Ros2Builtin_interfacesMsgTimeRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.text(128);
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
/** Complete ROS2 std_msgs/msg/String codec; maxima are not actual lengths. */
export const Ros2Std_msgsMsgStringCodec=Object.freeze({codecIdentity:"UWVMROS1_c48b5497cbf487f56eb073a982b93acd501c6bb72f0e9ef9dce2175ce7ac15c5",maxWireBytes:1028,minWireBytes:4,cartesianMaxWireBytes:1028n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros2Std_msgsMsgStringSize(Ros2Std_msgsMsgStringCapture(value));return true;},
encode(value){const owned=Ros2Std_msgsMsgStringCapture(value),n=Ros2Std_msgsMsgStringSize(owned),w=new Writer(n);Ros2Std_msgsMsgStringWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1028,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Std_msgsMsgStringRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_msgsMsgStringSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],1024).length);
if(n>1028)bad(90);return n;}
function Ros2Std_msgsMsgStringWrite(value,w){
w.text(value["data"],1024);
}
function Ros2Std_msgsMsgStringRead(r){const value={};
{
const decoded=r.text(1024);
value["data"]=decoded;
}
return value;}
function Ros2Std_msgsMsgStringCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/RobotMode codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgRobotModeCodec=Object.freeze({codecIdentity:"UWVMROS1_a16ec0ed8caa1a2b48cd4116d88877911921848469cd139f4783e4f2c549e7ea",maxWireBytes:141,minWireBytes:13,cartesianMaxWireBytes:141n,aggregateWireBytes:0,constants:Object.freeze({"DISCONNECTED":0,"CONFIRM_SAFETY":1,"BOOTING":2,"POWER_OFF":3,"POWER_ON":4,"IDLE":5,"BACKDRIVE":6,"RUNNING":7,"UPDATING_FIRMWARE":8}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgRobotModeSize(Ros2UrinterfacesMsgRobotModeCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgRobotModeCapture(value),n=Ros2UrinterfacesMsgRobotModeSize(owned),w=new Writer(n);Ros2UrinterfacesMsgRobotModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,141,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgRobotModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgRobotModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["data"],1,true);n=addSize(n,1);
if(n>141)bad(90);return n;}
function Ros2UrinterfacesMsgRobotModeWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["data"],true);w.at+=1;
}
function Ros2UrinterfacesMsgRobotModeRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgRobotModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/action/SetMode_Goal codec; maxima are not actual lengths. */
export const Ros2UrinterfacesActionSetMode_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_d1d998e217d6f3d3eefd8ffccecff8463580346150a59a1c7a2dac8d609e0e09",maxWireBytes:143,minWireBytes:15,cartesianMaxWireBytes:143n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_robot_mode":Ros2UrinterfacesMsgRobotModeCodec.defaultValue(),"stop_program":false,"play_program":false};},
validateValue(value){Ros2UrinterfacesActionSetMode_GoalSize(Ros2UrinterfacesActionSetMode_GoalCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesActionSetMode_GoalCapture(value),n=Ros2UrinterfacesActionSetMode_GoalSize(owned),w=new Writer(n);Ros2UrinterfacesActionSetMode_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,143,90);if(owned.length<15)bad();const r=new Reader(owned),value=Ros2UrinterfacesActionSetMode_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesActionSetMode_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2UrinterfacesMsgRobotModeSize(value["target_robot_mode"]));
if(typeof value["stop_program"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["play_program"]!=="boolean")bad();n=addSize(n,1);
if(n>143)bad(90);return n;}
function Ros2UrinterfacesActionSetMode_GoalWrite(value,w){
Ros2UrinterfacesMsgRobotModeWrite(value["target_robot_mode"],w);
w.v.setUint8(w.at,value["stop_program"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["play_program"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesActionSetMode_GoalRead(r){const value={};
{
const decoded=Ros2UrinterfacesMsgRobotModeRead(r);
value["target_robot_mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["stop_program"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["play_program"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesActionSetMode_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_robot_mode"];
out["target_robot_mode"]=Ros2UrinterfacesMsgRobotModeCapture(field);
}
{const field=value["stop_program"];
out["stop_program"]=field;
}
{const field=value["play_program"];
out["play_program"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/action/SetMode_Result codec; maxima are not actual lengths. */
export const Ros2UrinterfacesActionSetMode_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_500243a508824c42fe2da424040aa60bf2426ac607f2f6016fab014f81518af7",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2UrinterfacesActionSetMode_ResultSize(Ros2UrinterfacesActionSetMode_ResultCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesActionSetMode_ResultCapture(value),n=Ros2UrinterfacesActionSetMode_ResultSize(owned),w=new Writer(n);Ros2UrinterfacesActionSetMode_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2UrinterfacesActionSetMode_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesActionSetMode_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2UrinterfacesActionSetMode_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2UrinterfacesActionSetMode_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros2UrinterfacesActionSetMode_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/action/SetMode_Feedback codec; maxima are not actual lengths. */
export const Ros2UrinterfacesActionSetMode_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_22fb71c17066829bd66c98bc631c82b2bfef3c501e08960a6bfad3d902f76537",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_robot_mode":0,"current_safety_mode":0};},
validateValue(value){Ros2UrinterfacesActionSetMode_FeedbackSize(Ros2UrinterfacesActionSetMode_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesActionSetMode_FeedbackCapture(value),n=Ros2UrinterfacesActionSetMode_FeedbackSize(owned),w=new Writer(n);Ros2UrinterfacesActionSetMode_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2UrinterfacesActionSetMode_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesActionSetMode_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["current_robot_mode"],1,true);n=addSize(n,1);
integer(value["current_safety_mode"],1,true);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2UrinterfacesActionSetMode_FeedbackWrite(value,w){
w.v.setInt8(w.at,value["current_robot_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["current_safety_mode"],true);w.at+=1;
}
function Ros2UrinterfacesActionSetMode_FeedbackRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_robot_mode"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_safety_mode"]=decoded;
}
return value;}
function Ros2UrinterfacesActionSetMode_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_robot_mode"];
out["current_robot_mode"]=field;
}
{const field=value["current_safety_mode"];
out["current_safety_mode"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/ActiveTCP codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgActiveTCPCodec=Object.freeze({codecIdentity:"UWVMROS1_e62d31728eca201dee256d88d1d268bf2ecb9607ed5c48783432f966edf459ae",maxWireBytes:400,minWireBytes:16,cartesianMaxWireBytes:400n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":""};},
validateValue(value){Ros2UrinterfacesMsgActiveTCPSize(Ros2UrinterfacesMsgActiveTCPCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgActiveTCPCapture(value),n=Ros2UrinterfacesMsgActiveTCPSize(owned),w=new Writer(n);Ros2UrinterfacesMsgActiveTCPWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,400,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgActiveTCPRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgActiveTCPSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>400)bad(90);return n;}
function Ros2UrinterfacesMsgActiveTCPWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["data"],256);
}
function Ros2UrinterfacesMsgActiveTCPRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgActiveTCPCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/Analog codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgAnalogCodec=Object.freeze({codecIdentity:"UWVMROS1_e81cffd909f3bc387ee00c37537434341ef93fd160e465be033aa9bc412b979b",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"CURRENT":0,"VOLTAGE":1}),
defaultValue(){return {"pin":0,"domain":0,"value":0};},
validateValue(value){Ros2UrinterfacesMsgAnalogSize(Ros2UrinterfacesMsgAnalogCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgAnalogCapture(value),n=Ros2UrinterfacesMsgAnalogSize(owned),w=new Writer(n);Ros2UrinterfacesMsgAnalogWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgAnalogRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgAnalogSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
integer(value["domain"],1,false);n=addSize(n,1);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2UrinterfacesMsgAnalogWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["domain"],true);w.at+=1;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2UrinterfacesMsgAnalogRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["pin"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["domain"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgAnalogCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["domain"];
out["domain"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/AnalogIO codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgAnalogIOCodec=Object.freeze({codecIdentity:"UWVMROS1_3130f3e3857f21bab5092d9c6abea8c45965c07ebb9c1ab14b28fa51b7d71e0c",maxWireBytes:200,minWireBytes:8,cartesianMaxWireBytes:200n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"analog_inputs":[],"analog_outputs":[]};},
validateValue(value){Ros2UrinterfacesMsgAnalogIOSize(Ros2UrinterfacesMsgAnalogIOCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgAnalogIOCapture(value),n=Ros2UrinterfacesMsgAnalogIOSize(owned),w=new Writer(n);Ros2UrinterfacesMsgAnalogIOWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,200,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgAnalogIORead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgAnalogIOSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["analog_inputs"])||value["analog_inputs"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_inputs"]){
n=addSize(n,Ros2UrinterfacesMsgAnalogSize(element));
}
if(!Array.isArray(value["analog_outputs"])||value["analog_outputs"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_outputs"]){
n=addSize(n,Ros2UrinterfacesMsgAnalogSize(element));
}
if(n>200)bad(90);return n;}
function Ros2UrinterfacesMsgAnalogIOWrite(value,w){
w.v.setUint32(w.at,value["analog_inputs"].length,true);w.at+=4;
for(const element of value["analog_inputs"]){
Ros2UrinterfacesMsgAnalogWrite(element,w);
}
w.v.setUint32(w.at,value["analog_outputs"].length,true);w.at+=4;
for(const element of value["analog_outputs"]){
Ros2UrinterfacesMsgAnalogWrite(element,w);
}
}
function Ros2UrinterfacesMsgAnalogIORead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["analog_inputs"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalogRead(r);
value["analog_inputs"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["analog_outputs"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalogRead(r);
value["analog_outputs"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgAnalogIOCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["analog_inputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgAnalogCapture(element);
}out["analog_inputs"]=values;
}
{const field=value["analog_outputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgAnalogCapture(element);
}out["analog_outputs"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/AnalogIOStateStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgAnalogIOStateStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_a40e839bb4acfcb26503c79f01216b7aea9ab8ff25c50dca42578b206ab40a89",maxWireBytes:340,minWireBytes:20,cartesianMaxWireBytes:340n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"io_state":Ros2UrinterfacesMsgAnalogIOCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgAnalogIOStateStampedSize(Ros2UrinterfacesMsgAnalogIOStateStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgAnalogIOStateStampedCapture(value),n=Ros2UrinterfacesMsgAnalogIOStateStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgAnalogIOStateStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,340,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgAnalogIOStateStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgAnalogIOStateStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2UrinterfacesMsgAnalogIOSize(value["io_state"]));
if(n>340)bad(90);return n;}
function Ros2UrinterfacesMsgAnalogIOStateStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2UrinterfacesMsgAnalogIOWrite(value["io_state"],w);
}
function Ros2UrinterfacesMsgAnalogIOStateStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgAnalogIORead(r);
value["io_state"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgAnalogIOStateStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["io_state"];
out["io_state"]=Ros2UrinterfacesMsgAnalogIOCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/AnalogStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgAnalogStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_749557c6e79b882385927c62d3c45963797ce116d7be6235bd9f186a7b4da76c",maxWireBytes:146,minWireBytes:18,cartesianMaxWireBytes:146n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":Ros2UrinterfacesMsgAnalogCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgAnalogStampedSize(Ros2UrinterfacesMsgAnalogStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgAnalogStampedCapture(value),n=Ros2UrinterfacesMsgAnalogStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgAnalogStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,146,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgAnalogStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgAnalogStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2UrinterfacesMsgAnalogSize(value["data"]));
if(n>146)bad(90);return n;}
function Ros2UrinterfacesMsgAnalogStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2UrinterfacesMsgAnalogWrite(value["data"],w);
}
function Ros2UrinterfacesMsgAnalogStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgAnalogRead(r);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgAnalogStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=Ros2UrinterfacesMsgAnalogCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/BoolStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgBoolStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_1a4128638df10bd9a8d33af0ed3c2ee9b4deb771d45dfe83f2a9a75c7f77de66",maxWireBytes:141,minWireBytes:13,cartesianMaxWireBytes:141n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":false};},
validateValue(value){Ros2UrinterfacesMsgBoolStampedSize(Ros2UrinterfacesMsgBoolStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgBoolStampedCapture(value),n=Ros2UrinterfacesMsgBoolStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgBoolStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,141,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgBoolStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgBoolStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["data"]!=="boolean")bad();n=addSize(n,1);
if(n>141)bad(90);return n;}
function Ros2UrinterfacesMsgBoolStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["data"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesMsgBoolStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["data"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesMsgBoolStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/ControlCycle codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgControlCycleCodec=Object.freeze({codecIdentity:"UWVMROS1_99a4d96e5673f5ac49eab13cd8b28af55ac83e1626c39ed20ff46655df4d6278",maxWireBytes:148,minWireBytes:20,cartesianMaxWireBytes:148n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0n};},
validateValue(value){Ros2UrinterfacesMsgControlCycleSize(Ros2UrinterfacesMsgControlCycleCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgControlCycleCapture(value),n=Ros2UrinterfacesMsgControlCycleSize(owned),w=new Writer(n);Ros2UrinterfacesMsgControlCycleWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,148,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgControlCycleRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgControlCycleSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["data"],8,false);n=addSize(n,8);
if(n>148)bad(90);return n;}
function Ros2UrinterfacesMsgControlCycleWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setBigUint64(w.at,value["data"],true);w.at+=8;
}
function Ros2UrinterfacesMsgControlCycleRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgControlCycleCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/Current codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgCurrentCodec=Object.freeze({codecIdentity:"UWVMROS1_759f2139335bcb720f7f4a38cd8b1413f203f2dd2b72149fa0de0fb7a0a778df",maxWireBytes:144,minWireBytes:16,cartesianMaxWireBytes:144n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgCurrentSize(Ros2UrinterfacesMsgCurrentCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgCurrentCapture(value),n=Ros2UrinterfacesMsgCurrentSize(owned),w=new Writer(n);Ros2UrinterfacesMsgCurrentWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,144,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgCurrentRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgCurrentSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["data"]!=="number"||!Number.isFinite(value["data"])||!Number.isFinite(Math.fround(value["data"])))bad();n=addSize(n,4);
if(n>144)bad(90);return n;}
function Ros2UrinterfacesMsgCurrentWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["data"],true);w.at+=4;
}
function Ros2UrinterfacesMsgCurrentRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgCurrentCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/Digital codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgDigitalCodec=Object.freeze({codecIdentity:"UWVMROS1_c60c73df0383bb9a3e23ae3148cff9d6eb49b88c2d10add0af6a86ec9af54512",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pin":0,"state":false};},
validateValue(value){Ros2UrinterfacesMsgDigitalSize(Ros2UrinterfacesMsgDigitalCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgDigitalCapture(value),n=Ros2UrinterfacesMsgDigitalSize(owned),w=new Writer(n);Ros2UrinterfacesMsgDigitalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgDigitalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgDigitalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
if(typeof value["state"]!=="boolean")bad();n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2UrinterfacesMsgDigitalWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["state"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesMsgDigitalRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["pin"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["state"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesMsgDigitalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/DigitalIO codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgDigitalIOCodec=Object.freeze({codecIdentity:"UWVMROS1_11235b6010feb7e3e5e6c2be8acd3fbba1c25ecbf97ae5e0c899a0e3de3c4e4a",maxWireBytes:136,minWireBytes:8,cartesianMaxWireBytes:136n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"digital_inputs":[],"digital_outputs":[]};},
validateValue(value){Ros2UrinterfacesMsgDigitalIOSize(Ros2UrinterfacesMsgDigitalIOCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgDigitalIOCapture(value),n=Ros2UrinterfacesMsgDigitalIOSize(owned),w=new Writer(n);Ros2UrinterfacesMsgDigitalIOWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,136,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgDigitalIORead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgDigitalIOSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["digital_inputs"])||value["digital_inputs"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_inputs"]){
n=addSize(n,Ros2UrinterfacesMsgDigitalSize(element));
}
if(!Array.isArray(value["digital_outputs"])||value["digital_outputs"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_outputs"]){
n=addSize(n,Ros2UrinterfacesMsgDigitalSize(element));
}
if(n>136)bad(90);return n;}
function Ros2UrinterfacesMsgDigitalIOWrite(value,w){
w.v.setUint32(w.at,value["digital_inputs"].length,true);w.at+=4;
for(const element of value["digital_inputs"]){
Ros2UrinterfacesMsgDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["digital_outputs"].length,true);w.at+=4;
for(const element of value["digital_outputs"]){
Ros2UrinterfacesMsgDigitalWrite(element,w);
}
}
function Ros2UrinterfacesMsgDigitalIORead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["digital_inputs"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigitalRead(r);
value["digital_inputs"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["digital_outputs"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigitalRead(r);
value["digital_outputs"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgDigitalIOCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["digital_inputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgDigitalCapture(element);
}out["digital_inputs"]=values;
}
{const field=value["digital_outputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgDigitalCapture(element);
}out["digital_outputs"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/DigitalIOStateStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgDigitalIOStateStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_542db76916f234e79216804426dc4e8d386a0ef547907391c921f51ed45e64c9",maxWireBytes:412,minWireBytes:28,cartesianMaxWireBytes:412n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"configurable_io":Ros2UrinterfacesMsgDigitalIOCodec.defaultValue(),"standard_io":Ros2UrinterfacesMsgDigitalIOCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgDigitalIOStateStampedSize(Ros2UrinterfacesMsgDigitalIOStateStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgDigitalIOStateStampedCapture(value),n=Ros2UrinterfacesMsgDigitalIOStateStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgDigitalIOStateStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,412,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgDigitalIOStateStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgDigitalIOStateStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2UrinterfacesMsgDigitalIOSize(value["configurable_io"]));
n=addSize(n,Ros2UrinterfacesMsgDigitalIOSize(value["standard_io"]));
if(n>412)bad(90);return n;}
function Ros2UrinterfacesMsgDigitalIOStateStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2UrinterfacesMsgDigitalIOWrite(value["configurable_io"],w);
Ros2UrinterfacesMsgDigitalIOWrite(value["standard_io"],w);
}
function Ros2UrinterfacesMsgDigitalIOStateStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigitalIORead(r);
value["configurable_io"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigitalIORead(r);
value["standard_io"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgDigitalIOStateStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["configurable_io"];
out["configurable_io"]=Ros2UrinterfacesMsgDigitalIOCapture(field);
}
{const field=value["standard_io"];
out["standard_io"]=Ros2UrinterfacesMsgDigitalIOCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/DigitalStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgDigitalStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_dc86d5a04c0801089ddf6b61b5fd33d63df8b5599550c6a0e84e19bbff375865",maxWireBytes:142,minWireBytes:14,cartesianMaxWireBytes:142n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":Ros2UrinterfacesMsgDigitalCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgDigitalStampedSize(Ros2UrinterfacesMsgDigitalStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgDigitalStampedCapture(value),n=Ros2UrinterfacesMsgDigitalStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgDigitalStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,142,90);if(owned.length<14)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgDigitalStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgDigitalStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2UrinterfacesMsgDigitalSize(value["data"]));
if(n>142)bad(90);return n;}
function Ros2UrinterfacesMsgDigitalStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2UrinterfacesMsgDigitalWrite(value["data"],w);
}
function Ros2UrinterfacesMsgDigitalStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigitalRead(r);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgDigitalStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=Ros2UrinterfacesMsgDigitalCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/DoubleMultiArrayStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgDoubleMultiArrayStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_82b52f88248ea8d6cd7b809d1fd5aa55b4bbe1618044e739998a7ec74a23a8ae",maxWireBytes:272,minWireBytes:16,cartesianMaxWireBytes:272n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":[]};},
validateValue(value){Ros2UrinterfacesMsgDoubleMultiArrayStampedSize(Ros2UrinterfacesMsgDoubleMultiArrayStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgDoubleMultiArrayStampedCapture(value),n=Ros2UrinterfacesMsgDoubleMultiArrayStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgDoubleMultiArrayStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,272,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgDoubleMultiArrayStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgDoubleMultiArrayStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["data"])||value["data"].length>16)bad();
n=addSize(n,4);
for(const element of value["data"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>272)bad(90);return n;}
function Ros2UrinterfacesMsgDoubleMultiArrayStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2UrinterfacesMsgDoubleMultiArrayStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgDoubleMultiArrayStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/Float32Stamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgFloat32StampedCodec=Object.freeze({codecIdentity:"UWVMROS1_5989940d19890b6d3b78172e9920708745b78e35c1c4fb238412c37a813d30d3",maxWireBytes:144,minWireBytes:16,cartesianMaxWireBytes:144n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgFloat32StampedSize(Ros2UrinterfacesMsgFloat32StampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgFloat32StampedCapture(value),n=Ros2UrinterfacesMsgFloat32StampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgFloat32StampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,144,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgFloat32StampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgFloat32StampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["data"]!=="number"||!Number.isFinite(value["data"])||!Number.isFinite(Math.fround(value["data"])))bad();n=addSize(n,4);
if(n>144)bad(90);return n;}
function Ros2UrinterfacesMsgFloat32StampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["data"],true);w.at+=4;
}
function Ros2UrinterfacesMsgFloat32StampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgFloat32StampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/Float64Stamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgFloat64StampedCodec=Object.freeze({codecIdentity:"UWVMROS1_4f8950feff6f20dbdbf9cac1f01168824be3698a4bd90355a470b580930edb21",maxWireBytes:148,minWireBytes:20,cartesianMaxWireBytes:148n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgFloat64StampedSize(Ros2UrinterfacesMsgFloat64StampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgFloat64StampedCapture(value),n=Ros2UrinterfacesMsgFloat64StampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgFloat64StampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,148,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgFloat64StampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgFloat64StampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["data"]!=="number"||!Number.isFinite(value["data"]))bad();n=addSize(n,8);
if(n>148)bad(90);return n;}
function Ros2UrinterfacesMsgFloat64StampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["data"],true);w.at+=8;
}
function Ros2UrinterfacesMsgFloat64StampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgFloat64StampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/JointMode codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgJointModeCodec=Object.freeze({codecIdentity:"UWVMROS1_6d766192096ced3c8568a98fb286c309e00cfb97e34b2611fff70e5bb1eaa95c",maxWireBytes:147,minWireBytes:19,cartesianMaxWireBytes:147n,aggregateWireBytes:0,constants:Object.freeze({"JOINT_MODE_RESET":235,"JOINT_MODE_SHUTTING_DOWN":236,"JOINT_MODE_PART_D_CALIBRATION":237,"JOINT_MODE_BACKDRIVE":238,"JOINT_MODE_POWER_OFF":239,"JOINT_MODE_READY_FOR_POWER_OFF":240,"JOINT_MODE_NOT_RESPONDING":245,"JOINT_MODE_MOTOR_INITIALISATION":246,"JOINT_MODE_BOOTING":247,"JOINT_MODE_PART_D_CALIBRATION_ERROR":248,"JOINT_MODE_BOOTLOADER":249,"JOINT_MODE_CALIBRATION":250,"JOINT_MODE_VIOLATION":251,"JOINT_MODE_FAULT":252,"JOINT_MODE_RUNNING":253,"JOINT_MODE_IDLE":255}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_modes":Array.from({length:6},()=>(0)),"tool_mode":0};},
validateValue(value){Ros2UrinterfacesMsgJointModeSize(Ros2UrinterfacesMsgJointModeCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgJointModeCapture(value),n=Ros2UrinterfacesMsgJointModeSize(owned),w=new Writer(n);Ros2UrinterfacesMsgJointModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,147,90);if(owned.length<19)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgJointModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgJointModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_modes"])||value["joint_modes"].length!==6)bad();
for(const element of value["joint_modes"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["tool_mode"],1,false);n=addSize(n,1);
if(n>147)bad(90);return n;}
function Ros2UrinterfacesMsgJointModeWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
for(const element of value["joint_modes"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["tool_mode"],true);w.at+=1;
}
function Ros2UrinterfacesMsgJointModeRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=6;if(count>6)bad();value["joint_modes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["joint_modes"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["tool_mode"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgJointModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_modes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_modes"]=values;
}
{const field=value["tool_mode"];
out["tool_mode"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/JointTemperature codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgJointTemperatureCodec=Object.freeze({codecIdentity:"UWVMROS1_af77f1ec4c6b4d71e6387315d2388e7493c229dbf216848e61d5638cb308d017",maxWireBytes:164,minWireBytes:36,cartesianMaxWireBytes:164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2UrinterfacesMsgJointTemperatureSize(Ros2UrinterfacesMsgJointTemperatureCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgJointTemperatureCapture(value),n=Ros2UrinterfacesMsgJointTemperatureSize(owned),w=new Writer(n);Ros2UrinterfacesMsgJointTemperatureWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,164,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgJointTemperatureRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgJointTemperatureSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["data"])||value["data"].length!==6)bad();
for(const element of value["data"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>164)bad(90);return n;}
function Ros2UrinterfacesMsgJointTemperatureWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
for(const element of value["data"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2UrinterfacesMsgJointTemperatureRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=6;if(count>6)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgJointTemperatureCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/JointVoltage codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgJointVoltageCodec=Object.freeze({codecIdentity:"UWVMROS1_9fa2179402b5dc29b898ac1321c1f6f86676300939776a6e32de4529f3776b61",maxWireBytes:188,minWireBytes:60,cartesianMaxWireBytes:188n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2UrinterfacesMsgJointVoltageSize(Ros2UrinterfacesMsgJointVoltageCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgJointVoltageCapture(value),n=Ros2UrinterfacesMsgJointVoltageSize(owned),w=new Writer(n);Ros2UrinterfacesMsgJointVoltageWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,188,90);if(owned.length<60)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgJointVoltageRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgJointVoltageSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["data"])||value["data"].length!==6)bad();
for(const element of value["data"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>188)bad(90);return n;}
function Ros2UrinterfacesMsgJointVoltageWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
for(const element of value["data"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2UrinterfacesMsgJointVoltageRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=6;if(count>6)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgJointVoltageCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/JsonStringType codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgJsonStringTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_05da0dcf2728cd73f875421c457804a146ceffe76e9d4619be71c2c92efdd728",maxWireBytes:1028,minWireBytes:4,cartesianMaxWireBytes:1028n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"json_string":Ros2Std_msgsMsgStringCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgJsonStringTypeSize(Ros2UrinterfacesMsgJsonStringTypeCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgJsonStringTypeCapture(value),n=Ros2UrinterfacesMsgJsonStringTypeSize(owned),w=new Writer(n);Ros2UrinterfacesMsgJsonStringTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1028,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgJsonStringTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgJsonStringTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgStringSize(value["json_string"]));
if(n>1028)bad(90);return n;}
function Ros2UrinterfacesMsgJsonStringTypeWrite(value,w){
Ros2Std_msgsMsgStringWrite(value["json_string"],w);
}
function Ros2UrinterfacesMsgJsonStringTypeRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgStringRead(r);
value["json_string"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgJsonStringTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["json_string"];
out["json_string"]=Ros2Std_msgsMsgStringCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/RuntimeState codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgRuntimeStateCodec=Object.freeze({codecIdentity:"UWVMROS1_711ca0e324cbeb36fcedea4881b8658a4b453402c410c8d3efaa57205aaf74b6",maxWireBytes:141,minWireBytes:13,cartesianMaxWireBytes:141n,aggregateWireBytes:0,constants:Object.freeze({"PROGRAM_STATE_UNDEFINED":-1,"PROGRAM_STATE_STOPPING":0,"PROGRAM_STATE_STOPPED":1,"PROGRAM_STATE_RUNNING":2,"PROGRAM_STATE_PAUSING":3,"PROGRAM_STATE_PAUSED":4,"PROGRAM_STATE_RESUMING":5,"PROGRAM_STATE_RETRACTING":6}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgRuntimeStateSize(Ros2UrinterfacesMsgRuntimeStateCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgRuntimeStateCapture(value),n=Ros2UrinterfacesMsgRuntimeStateSize(owned),w=new Writer(n);Ros2UrinterfacesMsgRuntimeStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,141,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgRuntimeStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgRuntimeStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["data"],1,true);n=addSize(n,1);
if(n>141)bad(90);return n;}
function Ros2UrinterfacesMsgRuntimeStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["data"],true);w.at+=1;
}
function Ros2UrinterfacesMsgRuntimeStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgRuntimeStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/SafetyStatus codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgSafetyStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_8f266e1ee8ab2ee9791ff4b52f9dae8ef982844c8d24a0bdfb4bb93cc55b78d4",maxWireBytes:141,minWireBytes:13,cartesianMaxWireBytes:141n,aggregateWireBytes:0,constants:Object.freeze({"NORMAL":1,"REDUCED":2,"PROTECTIVE_STOP":3,"RECOVERY":4,"SAFEGUARD_STOP":5,"SYSTEM_EMERGENCY_STOP":6,"ROBOT_EMERGENCY_STOP":7,"VIOLATION":8,"FAULT":9,"VALIDATE_JOINT_ID":10,"UNDEFINED_SAFETY_MODE":11,"AUTOMATIC_MODE_SAFEGUARD_STOP":12,"SYSTEM_THREE_POSITION_ENABLING_STOP":13,"TP_THREE_POSITION_ENABLING_STOP":14,"IMMI_EMERGENCY_STOP":15,"IMMI_SAFEGUARD_STOP":16,"PROFISAFE_WAITING_FOR_PARAMETERS":17,"PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP":18,"PROFISAFE_SAFEGUARD_STOP":19,"PROFISAFE_EMERGENCY_STOP":20,"SAFETY_API_SAFEGUARD_STOP":22,"IO_PLANE_STOP":23}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgSafetyStatusSize(Ros2UrinterfacesMsgSafetyStatusCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgSafetyStatusCapture(value),n=Ros2UrinterfacesMsgSafetyStatusSize(owned),w=new Writer(n);Ros2UrinterfacesMsgSafetyStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,141,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgSafetyStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgSafetyStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["data"],1,true);n=addSize(n,1);
if(n>141)bad(90);return n;}
function Ros2UrinterfacesMsgSafetyStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["data"],true);w.at+=1;
}
function Ros2UrinterfacesMsgSafetyStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgSafetyStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/SharedVariablesEditResponse codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgSharedVariablesEditResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_870ce8f649642b6edcce8ba5c7cb27818ec138d7afb2cde926638d21ea748212",maxWireBytes:1029,minWireBytes:5,cartesianMaxWireBytes:1029n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"result_message":Ros2Std_msgsMsgStringCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgSharedVariablesEditResponseSize(Ros2UrinterfacesMsgSharedVariablesEditResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgSharedVariablesEditResponseCapture(value),n=Ros2UrinterfacesMsgSharedVariablesEditResponseSize(owned),w=new Writer(n);Ros2UrinterfacesMsgSharedVariablesEditResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1029,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgSharedVariablesEditResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgSharedVariablesEditResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Std_msgsMsgStringSize(value["result_message"]));
if(n>1029)bad(90);return n;}
function Ros2UrinterfacesMsgSharedVariablesEditResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
Ros2Std_msgsMsgStringWrite(value["result_message"],w);
}
function Ros2UrinterfacesMsgSharedVariablesEditResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=Ros2Std_msgsMsgStringRead(r);
value["result_message"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgSharedVariablesEditResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["result_message"];
out["result_message"]=Ros2Std_msgsMsgStringCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/SpeedSliderFraction codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgSpeedSliderFractionCodec=Object.freeze({codecIdentity:"UWVMROS1_55158782c2bc4d4741e8f2850efddf09d27666f9c3aeeded762d8d8e4198fccd",maxWireBytes:148,minWireBytes:20,cartesianMaxWireBytes:148n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":0};},
validateValue(value){Ros2UrinterfacesMsgSpeedSliderFractionSize(Ros2UrinterfacesMsgSpeedSliderFractionCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgSpeedSliderFractionCapture(value),n=Ros2UrinterfacesMsgSpeedSliderFractionSize(owned),w=new Writer(n);Ros2UrinterfacesMsgSpeedSliderFractionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,148,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgSpeedSliderFractionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgSpeedSliderFractionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["data"]!=="number"||!Number.isFinite(value["data"]))bad();n=addSize(n,8);
if(n>148)bad(90);return n;}
function Ros2UrinterfacesMsgSpeedSliderFractionWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["data"],true);w.at+=8;
}
function Ros2UrinterfacesMsgSpeedSliderFractionRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgSpeedSliderFractionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/TCPState codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgTCPStateCodec=Object.freeze({codecIdentity:"UWVMROS1_49d8f678cc32669dd81528bb4c5d97f28019745159ef5e7267c8a076cd315b92",maxWireBytes:244,minWireBytes:116,cartesianMaxWireBytes:244n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"pose":Ros2Geometry_msgsMsgPoseCodec.defaultValue(),"speed":Ros2Geometry_msgsMsgTwistCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgTCPStateSize(Ros2UrinterfacesMsgTCPStateCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgTCPStateCapture(value),n=Ros2UrinterfacesMsgTCPStateSize(owned),w=new Writer(n);Ros2UrinterfacesMsgTCPStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,244,90);if(owned.length<116)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgTCPStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgTCPStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseSize(value["pose"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(value["speed"]));
if(n>244)bad(90);return n;}
function Ros2UrinterfacesMsgTCPStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgPoseWrite(value["pose"],w);
Ros2Geometry_msgsMsgTwistWrite(value["speed"],w);
}
function Ros2UrinterfacesMsgTCPStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseRead(r);
value["pose"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["speed"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgTCPStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseCapture(field);
}
{const field=value["speed"];
out["speed"]=Ros2Geometry_msgsMsgTwistCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/ToolOutputMode codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgToolOutputModeCodec=Object.freeze({codecIdentity:"UWVMROS1_21df545459050dc86de1fb111c9a4b2c8bb50c3729137b8d5c5134dd33b84660",maxWireBytes:143,minWireBytes:15,cartesianMaxWireBytes:143n,aggregateWireBytes:0,constants:Object.freeze({"TOOL_OUTPUT":0,"TOOL_POWER":1,"TOOL_SAFE":2,"OUTPUT_MODE_DISABLED":0,"OUTPUT_MODE_SINKING":1,"OUTPUT_MODE_SOURCING":2,"OUTPUT_MODE_PUSH_PULL":3}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"output_mode":0,"output_pin_mode":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2UrinterfacesMsgToolOutputModeSize(Ros2UrinterfacesMsgToolOutputModeCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgToolOutputModeCapture(value),n=Ros2UrinterfacesMsgToolOutputModeSize(owned),w=new Writer(n);Ros2UrinterfacesMsgToolOutputModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,143,90);if(owned.length<15)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgToolOutputModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgToolOutputModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["output_mode"],1,false);n=addSize(n,1);
if(!Array.isArray(value["output_pin_mode"])||value["output_pin_mode"].length!==2)bad();
for(const element of value["output_pin_mode"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>143)bad(90);return n;}
function Ros2UrinterfacesMsgToolOutputModeWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["output_mode"],true);w.at+=1;
for(const element of value["output_pin_mode"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2UrinterfacesMsgToolOutputModeRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["output_mode"]=decoded;
}
{
{const count=2;if(count>2)bad();value["output_pin_mode"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["output_pin_mode"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgToolOutputModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["output_mode"];
out["output_mode"]=field;
}
{const field=value["output_pin_mode"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["output_pin_mode"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/msg/VariableUpdate codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgVariableUpdateCodec=Object.freeze({codecIdentity:"UWVMROS1_7028ea5c674be886f4fac1d2ac0452167489f2c23d7ea759cdbec90feccbdb20",maxWireBytes:1168,minWireBytes:16,cartesianMaxWireBytes:1168n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":Ros2UrinterfacesMsgJsonStringTypeCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesMsgVariableUpdateSize(Ros2UrinterfacesMsgVariableUpdateCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgVariableUpdateCapture(value),n=Ros2UrinterfacesMsgVariableUpdateSize(owned),w=new Writer(n);Ros2UrinterfacesMsgVariableUpdateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1168,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgVariableUpdateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgVariableUpdateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2UrinterfacesMsgJsonStringTypeSize(value["data"]));
if(n>1168)bad(90);return n;}
function Ros2UrinterfacesMsgVariableUpdateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2UrinterfacesMsgJsonStringTypeWrite(value["data"],w);
}
function Ros2UrinterfacesMsgVariableUpdateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2UrinterfacesMsgJsonStringTypeRead(r);
value["data"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgVariableUpdateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
out["data"]=Ros2UrinterfacesMsgJsonStringTypeCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/msg/VoltageCurrentStamped codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgVoltageCurrentStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_bcc3f6871a7bf0ff691d0c9c1b6596a633597de795b486c2b82ac922ede76805",maxWireBytes:156,minWireBytes:28,cartesianMaxWireBytes:156n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"voltage":0,"current":0};},
validateValue(value){Ros2UrinterfacesMsgVoltageCurrentStampedSize(Ros2UrinterfacesMsgVoltageCurrentStampedCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgVoltageCurrentStampedCapture(value),n=Ros2UrinterfacesMsgVoltageCurrentStampedSize(owned),w=new Writer(n);Ros2UrinterfacesMsgVoltageCurrentStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,156,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgVoltageCurrentStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgVoltageCurrentStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["voltage"]!=="number"||!Number.isFinite(value["voltage"]))bad();n=addSize(n,8);
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"]))bad();n=addSize(n,8);
if(n>156)bad(90);return n;}
function Ros2UrinterfacesMsgVoltageCurrentStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["current"],true);w.at+=8;
}
function Ros2UrinterfacesMsgVoltageCurrentStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
return value;}
function Ros2UrinterfacesMsgVoltageCurrentStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["voltage"];
out["voltage"]=field;
}
{const field=value["current"];
out["current"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/msg/WorldModelNames codec; maxima are not actual lengths. */
export const Ros2UrinterfacesMsgWorldModelNamesCodec=Object.freeze({codecIdentity:"UWVMROS1_499f722fc97565bd0124888d7c20f4e9f8947893c49b1608dcf201fe3538d7a9",maxWireBytes:16592,minWireBytes:16,cartesianMaxWireBytes:16592n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"data":[]};},
validateValue(value){Ros2UrinterfacesMsgWorldModelNamesSize(Ros2UrinterfacesMsgWorldModelNamesCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesMsgWorldModelNamesCapture(value),n=Ros2UrinterfacesMsgWorldModelNamesSize(owned),w=new Writer(n);Ros2UrinterfacesMsgWorldModelNamesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16592,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesMsgWorldModelNamesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesMsgWorldModelNamesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["data"])||value["data"].length>16)bad();
n=addSize(n,4);
for(const element of value["data"]){
n=addSize(n,Ros2Std_msgsMsgStringSize(element));
}
if(n>16592)bad(90);return n;}
function Ros2UrinterfacesMsgWorldModelNamesWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
Ros2Std_msgsMsgStringWrite(element,w);
}
}
function Ros2UrinterfacesMsgWorldModelNamesRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Std_msgsMsgStringRead(r);
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesMsgWorldModelNamesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Std_msgsMsgStringCapture(element);
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetAnalogOutput_Request codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetAnalogOutput_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8be07a94dd334a03871180a9c73bab11377664e60500ac68d312398d51acd32d",maxWireBytes:100,minWireBytes:4,cartesianMaxWireBytes:100n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":[]};},
validateValue(value){Ros2UrinterfacesSrvSetAnalogOutput_RequestSize(Ros2UrinterfacesSrvSetAnalogOutput_RequestCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetAnalogOutput_RequestCapture(value),n=Ros2UrinterfacesSrvSetAnalogOutput_RequestSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetAnalogOutput_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,100,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetAnalogOutput_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetAnalogOutput_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["data"])||value["data"].length>16)bad();
n=addSize(n,4);
for(const element of value["data"]){
n=addSize(n,Ros2UrinterfacesMsgAnalogSize(element));
}
if(n>100)bad(90);return n;}
function Ros2UrinterfacesSrvSetAnalogOutput_RequestWrite(value,w){
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
Ros2UrinterfacesMsgAnalogWrite(element,w);
}
}
function Ros2UrinterfacesSrvSetAnalogOutput_RequestRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalogRead(r);
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesSrvSetAnalogOutput_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgAnalogCapture(element);
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetAnalogOutput_Response codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetAnalogOutput_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_14016138286129e4f155ac2f82f5573e8a4af7d7c5ede55ce57f3ad76cb82e4e",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2UrinterfacesSrvSetAnalogOutput_ResponseSize(Ros2UrinterfacesSrvSetAnalogOutput_ResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetAnalogOutput_ResponseCapture(value),n=Ros2UrinterfacesSrvSetAnalogOutput_ResponseSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetAnalogOutput_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetAnalogOutput_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetAnalogOutput_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2UrinterfacesSrvSetAnalogOutput_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesSrvSetAnalogOutput_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesSrvSetAnalogOutput_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetDigitalOutput_Request codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetDigitalOutput_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ec6c587810af7f3d04b10058e69c8095775e8b6feede5fc0f60ca1dfda0b5e47",maxWireBytes:36,minWireBytes:4,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":[]};},
validateValue(value){Ros2UrinterfacesSrvSetDigitalOutput_RequestSize(Ros2UrinterfacesSrvSetDigitalOutput_RequestCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetDigitalOutput_RequestCapture(value),n=Ros2UrinterfacesSrvSetDigitalOutput_RequestSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetDigitalOutput_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetDigitalOutput_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetDigitalOutput_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["data"])||value["data"].length>16)bad();
n=addSize(n,4);
for(const element of value["data"]){
n=addSize(n,Ros2UrinterfacesMsgDigitalSize(element));
}
if(n>36)bad(90);return n;}
function Ros2UrinterfacesSrvSetDigitalOutput_RequestWrite(value,w){
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
Ros2UrinterfacesMsgDigitalWrite(element,w);
}
}
function Ros2UrinterfacesSrvSetDigitalOutput_RequestRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigitalRead(r);
value["data"].push(decoded);
}}
}
return value;}
function Ros2UrinterfacesSrvSetDigitalOutput_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2UrinterfacesMsgDigitalCapture(element);
}out["data"]=values;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetDigitalOutput_Response codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetDigitalOutput_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0c5f0b8a7f1d14c6223838c3a28cb2236848b81ecfda29d2b704557579e9162d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2UrinterfacesSrvSetDigitalOutput_ResponseSize(Ros2UrinterfacesSrvSetDigitalOutput_ResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetDigitalOutput_ResponseCapture(value),n=Ros2UrinterfacesSrvSetDigitalOutput_ResponseSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetDigitalOutput_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetDigitalOutput_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetDigitalOutput_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2UrinterfacesSrvSetDigitalOutput_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesSrvSetDigitalOutput_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesSrvSetDigitalOutput_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetSpeedFraction_Request codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetSpeedFraction_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_21fca7e952e32f24c905094c09776968c8cd80b7712a03de7c2a693ea79a7d63",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2UrinterfacesSrvSetSpeedFraction_RequestSize(Ros2UrinterfacesSrvSetSpeedFraction_RequestCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetSpeedFraction_RequestCapture(value),n=Ros2UrinterfacesSrvSetSpeedFraction_RequestSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetSpeedFraction_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetSpeedFraction_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetSpeedFraction_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2UrinterfacesSrvSetSpeedFraction_RequestWrite(value,w){
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2UrinterfacesSrvSetSpeedFraction_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2UrinterfacesSrvSetSpeedFraction_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SetSpeedFraction_Response codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSetSpeedFraction_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_386b067db70c17a88297d005650f6a05c0fb72eb5ee142a25c3e51c70b80c6e6",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2UrinterfacesSrvSetSpeedFraction_ResponseSize(Ros2UrinterfacesSrvSetSpeedFraction_ResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSetSpeedFraction_ResponseCapture(value),n=Ros2UrinterfacesSrvSetSpeedFraction_ResponseSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSetSpeedFraction_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSetSpeedFraction_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSetSpeedFraction_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2UrinterfacesSrvSetSpeedFraction_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2UrinterfacesSrvSetSpeedFraction_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2UrinterfacesSrvSetSpeedFraction_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 urinterfaces/srv/SharedVariablesEditor_Request codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSharedVariablesEditor_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c32d00237370b2eeb596c76bd9cda1ccfcab7618c71dec77d273f712462f5fa3",maxWireBytes:1028,minWireBytes:4,cartesianMaxWireBytes:1028n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"request":Ros2UrinterfacesMsgJsonStringTypeCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesSrvSharedVariablesEditor_RequestSize(Ros2UrinterfacesSrvSharedVariablesEditor_RequestCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSharedVariablesEditor_RequestCapture(value),n=Ros2UrinterfacesSrvSharedVariablesEditor_RequestSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSharedVariablesEditor_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1028,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSharedVariablesEditor_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSharedVariablesEditor_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2UrinterfacesMsgJsonStringTypeSize(value["request"]));
if(n>1028)bad(90);return n;}
function Ros2UrinterfacesSrvSharedVariablesEditor_RequestWrite(value,w){
Ros2UrinterfacesMsgJsonStringTypeWrite(value["request"],w);
}
function Ros2UrinterfacesSrvSharedVariablesEditor_RequestRead(r){const value={};
{
const decoded=Ros2UrinterfacesMsgJsonStringTypeRead(r);
value["request"]=decoded;
}
return value;}
function Ros2UrinterfacesSrvSharedVariablesEditor_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["request"];
out["request"]=Ros2UrinterfacesMsgJsonStringTypeCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/srv/SharedVariablesEditor_Response codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvSharedVariablesEditor_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4134f7ed9715b3f6e3fd7ecca44fcae67cdd5dc2fa432b07ab75485901d1d5ea",maxWireBytes:1029,minWireBytes:5,cartesianMaxWireBytes:1029n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"response":Ros2UrinterfacesMsgSharedVariablesEditResponseCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesSrvSharedVariablesEditor_ResponseSize(Ros2UrinterfacesSrvSharedVariablesEditor_ResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvSharedVariablesEditor_ResponseCapture(value),n=Ros2UrinterfacesSrvSharedVariablesEditor_ResponseSize(owned),w=new Writer(n);Ros2UrinterfacesSrvSharedVariablesEditor_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1029,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvSharedVariablesEditor_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvSharedVariablesEditor_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2UrinterfacesMsgSharedVariablesEditResponseSize(value["response"]));
if(n>1029)bad(90);return n;}
function Ros2UrinterfacesSrvSharedVariablesEditor_ResponseWrite(value,w){
Ros2UrinterfacesMsgSharedVariablesEditResponseWrite(value["response"],w);
}
function Ros2UrinterfacesSrvSharedVariablesEditor_ResponseRead(r){const value={};
{
const decoded=Ros2UrinterfacesMsgSharedVariablesEditResponseRead(r);
value["response"]=decoded;
}
return value;}
function Ros2UrinterfacesSrvSharedVariablesEditor_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["response"];
out["response"]=Ros2UrinterfacesMsgSharedVariablesEditResponseCapture(field);
}
return out;}
/** Complete ROS2 urinterfaces/srv/VariablesViewer_Request codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvVariablesViewer_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6ecb0db6328d813bfb987ae987d3b7f2c6913dba2cf2a62f644a8eb2f760152a",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2UrinterfacesSrvVariablesViewer_RequestSize(Ros2UrinterfacesSrvVariablesViewer_RequestCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvVariablesViewer_RequestCapture(value),n=Ros2UrinterfacesSrvVariablesViewer_RequestSize(owned),w=new Writer(n);Ros2UrinterfacesSrvVariablesViewer_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvVariablesViewer_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvVariablesViewer_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2UrinterfacesSrvVariablesViewer_RequestWrite(value,w){
}
function Ros2UrinterfacesSrvVariablesViewer_RequestRead(r){const value={};
return value;}
function Ros2UrinterfacesSrvVariablesViewer_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 urinterfaces/srv/VariablesViewer_Response codec; maxima are not actual lengths. */
export const Ros2UrinterfacesSrvVariablesViewer_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e8cb16ca847189ac38c0d972afbdd2208d8641c29a0ee0cce8284e9e0dd249a2",maxWireBytes:1168,minWireBytes:16,cartesianMaxWireBytes:1168n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"response":Ros2UrinterfacesMsgVariableUpdateCodec.defaultValue()};},
validateValue(value){Ros2UrinterfacesSrvVariablesViewer_ResponseSize(Ros2UrinterfacesSrvVariablesViewer_ResponseCapture(value));return true;},
encode(value){const owned=Ros2UrinterfacesSrvVariablesViewer_ResponseCapture(value),n=Ros2UrinterfacesSrvVariablesViewer_ResponseSize(owned),w=new Writer(n);Ros2UrinterfacesSrvVariablesViewer_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1168,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2UrinterfacesSrvVariablesViewer_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2UrinterfacesSrvVariablesViewer_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2UrinterfacesMsgVariableUpdateSize(value["response"]));
if(n>1168)bad(90);return n;}
function Ros2UrinterfacesSrvVariablesViewer_ResponseWrite(value,w){
Ros2UrinterfacesMsgVariableUpdateWrite(value["response"],w);
}
function Ros2UrinterfacesSrvVariablesViewer_ResponseRead(r){const value={};
{
const decoded=Ros2UrinterfacesMsgVariableUpdateRead(r);
value["response"]=decoded;
}
return value;}
function Ros2UrinterfacesSrvVariablesViewer_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["response"];
out["response"]=Ros2UrinterfacesMsgVariableUpdateCapture(field);
}
return out;}
