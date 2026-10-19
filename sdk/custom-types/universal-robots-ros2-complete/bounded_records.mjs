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
/** Complete ROS2 builtin_interfaces/msg/Duration codec; maxima are not actual lengths. */
export const Ros2Builtin_interfacesMsgDurationCodec=Object.freeze({codecIdentity:"UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nanosec":0};},
validateValue(value){Ros2Builtin_interfacesMsgDurationSize(Ros2Builtin_interfacesMsgDurationCapture(value));return true;},
encode(value){const owned=Ros2Builtin_interfacesMsgDurationCapture(value),n=Ros2Builtin_interfacesMsgDurationSize(owned),w=new Writer(n);Ros2Builtin_interfacesMsgDurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Builtin_interfacesMsgDurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Builtin_interfacesMsgDurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nanosec"],4,false);n=addSize(n,4);
if(value["nanosec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros2Builtin_interfacesMsgDurationWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nanosec"],true);w.at+=4;
}
function Ros2Builtin_interfacesMsgDurationRead(r){const value={};
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
function Ros2Builtin_interfacesMsgDurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nanosec"];
out["nanosec"]=field;
}
return out;}
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
/** Complete ROS2 control_msgs/msg/JointTolerance codec; maxima are not actual lengths. */
export const Ros2Control_msgsMsgJointToleranceCodec=Object.freeze({codecIdentity:"UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a",maxWireBytes:156,minWireBytes:28,cartesianMaxWireBytes:156n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","position":0,"velocity":0,"acceleration":0};},
validateValue(value){Ros2Control_msgsMsgJointToleranceSize(Ros2Control_msgsMsgJointToleranceCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsMsgJointToleranceCapture(value),n=Ros2Control_msgsMsgJointToleranceSize(owned),w=new Writer(n);Ros2Control_msgsMsgJointToleranceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,156,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Control_msgsMsgJointToleranceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsMsgJointToleranceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"]))bad();n=addSize(n,8);
if(typeof value["acceleration"]!=="number"||!Number.isFinite(value["acceleration"]))bad();n=addSize(n,8);
if(n>156)bad(90);return n;}
function Ros2Control_msgsMsgJointToleranceWrite(value,w){
w.text(value["name"],128);
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setFloat64(w.at,value["velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["acceleration"],true);w.at+=8;
}
function Ros2Control_msgsMsgJointToleranceRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["acceleration"]=decoded;
}
return value;}
function Ros2Control_msgsMsgJointToleranceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["position"];
out["position"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["acceleration"];
out["acceleration"]=field;
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
/** Complete ROS2 geometry_msgs/msg/PoseStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgPoseStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_71abdc844f18f02f45a354967a5f1c644fb20dc14cbc61c64aa41b927e0712a2",maxWireBytes:196,minWireBytes:68,cartesianMaxWireBytes:196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"pose":Ros2Geometry_msgsMsgPoseCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgPoseStampedSize(Ros2Geometry_msgsMsgPoseStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgPoseStampedCapture(value),n=Ros2Geometry_msgsMsgPoseStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgPoseStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,196,90);if(owned.length<68)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgPoseStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgPoseStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseSize(value["pose"]));
if(n>196)bad(90);return n;}
function Ros2Geometry_msgsMsgPoseStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgPoseWrite(value["pose"],w);
}
function Ros2Geometry_msgsMsgPoseStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseRead(r);
value["pose"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgPoseStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseCapture(field);
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
/** Complete ROS2 geometry_msgs/msg/Vector3Stamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgVector3StampedCodec=Object.freeze({codecIdentity:"UWVMROS1_c6b67646d3fc6d29c195637ea353dafa7085b2bdb55a15a8b65009200b5ed31b",maxWireBytes:164,minWireBytes:36,cartesianMaxWireBytes:164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"vector":Ros2Geometry_msgsMsgVector3Codec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgVector3StampedSize(Ros2Geometry_msgsMsgVector3StampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgVector3StampedCapture(value),n=Ros2Geometry_msgsMsgVector3StampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgVector3StampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,164,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgVector3StampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgVector3StampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["vector"]));
if(n>164)bad(90);return n;}
function Ros2Geometry_msgsMsgVector3StampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgVector3Write(value["vector"],w);
}
function Ros2Geometry_msgsMsgVector3StampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["vector"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgVector3StampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["vector"];
out["vector"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
return out;}
/** Complete ROS2 geometry_msgs/msg/Wrench codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgWrenchCodec=Object.freeze({codecIdentity:"UWVMROS1_fdfdf2474c0129b0ce1f85972cc87fd80478ac7d2429d9de1ec58951a82a11b6",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"force":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"torque":Ros2Geometry_msgsMsgVector3Codec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgWrenchSize(Ros2Geometry_msgsMsgWrenchCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgWrenchCapture(value),n=Ros2Geometry_msgsMsgWrenchSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgWrenchWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgWrenchRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgWrenchSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["force"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["torque"]));
if(n>48)bad(90);return n;}
function Ros2Geometry_msgsMsgWrenchWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["force"],w);
Ros2Geometry_msgsMsgVector3Write(value["torque"],w);
}
function Ros2Geometry_msgsMsgWrenchRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["force"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["torque"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgWrenchCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["force"];
out["force"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["torque"];
out["torque"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
return out;}
/** Complete ROS2 trajectory_msgs/msg/JointTrajectoryPoint codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgJointTrajectoryPointCodec=Object.freeze({codecIdentity:"UWVMROS1_c4eafb6efd22d299b5c6fc21844d8bf69789cf27ee5414a1d7d2371f0b1acbbf",maxWireBytes:216,minWireBytes:24,cartesianMaxWireBytes:216n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"positions":[],"velocities":[],"accelerations":[],"effort":[],"time_from_start":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Trajectory_msgsMsgJointTrajectoryPointSize(Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value),n=Ros2Trajectory_msgsMsgJointTrajectoryPointSize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,216,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["positions"])||value["positions"].length>6)bad();
n=addSize(n,4);
for(const element of value["positions"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocities"])||value["velocities"].length>6)bad();
n=addSize(n,4);
for(const element of value["velocities"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["accelerations"])||value["accelerations"].length>6)bad();
n=addSize(n,4);
for(const element of value["accelerations"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>6)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["time_from_start"]));
if(n>216)bad(90);return n;}
function Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value,w){
w.v.setUint32(w.at,value["positions"].length,true);w.at+=4;
for(const element of value["positions"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["velocities"].length,true);w.at+=4;
for(const element of value["velocities"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["accelerations"].length,true);w.at+=4;
for(const element of value["accelerations"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["effort"].length,true);w.at+=4;
for(const element of value["effort"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros2Builtin_interfacesMsgDurationWrite(value["time_from_start"],w);
}
function Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r){const value={};
{
{const count=r.u32();if(count>6)bad();value["positions"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["positions"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["velocities"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocities"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["accelerations"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["accelerations"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["effort"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["effort"].push(decoded);
}}
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["time_from_start"]=decoded;
}
return value;}
function Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["positions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["positions"]=values;
}
{const field=value["velocities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocities"]=values;
}
{const field=value["accelerations"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["accelerations"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
{const field=value["time_from_start"];
out["time_from_start"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 trajectory_msgs/msg/JointTrajectory codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgJointTrajectoryCodec=Object.freeze({codecIdentity:"UWVMROS1_e77c37c75123e442edb6a7f6f2a21dcbe33d8bb8fd2577f9906653a6ad5f8225",maxWireBytes:5164,minWireBytes:20,cartesianMaxWireBytes:5164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_names":[],"points":[]};},
validateValue(value){Ros2Trajectory_msgsMsgJointTrajectorySize(Ros2Trajectory_msgsMsgJointTrajectoryCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgJointTrajectoryCapture(value),n=Ros2Trajectory_msgsMsgJointTrajectorySize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgJointTrajectoryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5164,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgJointTrajectoryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgJointTrajectorySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>6)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["points"])||value["points"].length>16)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(element));
}
if(n>5164)bad(90);return n;}
function Ros2Trajectory_msgsMsgJointTrajectoryWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(element,w);
}
}
function Ros2Trajectory_msgsMsgJointTrajectoryRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["joint_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros2Trajectory_msgsMsgJointTrajectoryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Goal codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsActionSetMode_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_8a7484a60bad21933df3f847d3fd554e41098ed596dee270335ce9f092dc313d",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_robot_mode":0,"stop_program":false,"play_program":false};},
validateValue(value){Ros2Ur_dashboard_msgsActionSetMode_GoalSize(Ros2Ur_dashboard_msgsActionSetMode_GoalCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsActionSetMode_GoalCapture(value),n=Ros2Ur_dashboard_msgsActionSetMode_GoalSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsActionSetMode_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsActionSetMode_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsActionSetMode_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["target_robot_mode"],1,true);n=addSize(n,1);
if(typeof value["stop_program"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["play_program"]!=="boolean")bad();n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Ur_dashboard_msgsActionSetMode_GoalWrite(value,w){
w.v.setInt8(w.at,value["target_robot_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["stop_program"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["play_program"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsActionSetMode_GoalRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
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
function Ros2Ur_dashboard_msgsActionSetMode_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_robot_mode"];
out["target_robot_mode"]=field;
}
{const field=value["stop_program"];
out["stop_program"]=field;
}
{const field=value["play_program"];
out["play_program"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Result codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsActionSetMode_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_ef2a557ff27171319d46170ab16a80f5cba5fbd59e44c21cc1b704d01764b913",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Ur_dashboard_msgsActionSetMode_ResultSize(Ros2Ur_dashboard_msgsActionSetMode_ResultCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsActionSetMode_ResultCapture(value),n=Ros2Ur_dashboard_msgsActionSetMode_ResultSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsActionSetMode_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsActionSetMode_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsActionSetMode_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsActionSetMode_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Ur_dashboard_msgsActionSetMode_ResultRead(r){const value={};
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
function Ros2Ur_dashboard_msgsActionSetMode_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Feedback codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_afd74cd0a79174b53499a40b23b040ba96ec3d3e332129999525dcf6ca287ac6",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_robot_mode":0,"current_safety_mode":0};},
validateValue(value){Ros2Ur_dashboard_msgsActionSetMode_FeedbackSize(Ros2Ur_dashboard_msgsActionSetMode_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsActionSetMode_FeedbackCapture(value),n=Ros2Ur_dashboard_msgsActionSetMode_FeedbackSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsActionSetMode_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsActionSetMode_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsActionSetMode_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["current_robot_mode"],1,true);n=addSize(n,1);
integer(value["current_safety_mode"],1,true);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Ur_dashboard_msgsActionSetMode_FeedbackWrite(value,w){
w.v.setInt8(w.at,value["current_robot_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["current_safety_mode"],true);w.at+=1;
}
function Ros2Ur_dashboard_msgsActionSetMode_FeedbackRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_robot_mode"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_safety_mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsActionSetMode_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_robot_mode"];
out["current_robot_mode"]=field;
}
{const field=value["current_safety_mode"];
out["current_safety_mode"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/OperationalMode codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgOperationalModeCodec=Object.freeze({codecIdentity:"UWVMROS1_7a0be8522cdc20e01d382218499030176a0f6565376fadc5bbdd871a01b9316e",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({"MANUAL":"MANUAL","AUTOMATIC":"AUTOMATIC"}),
defaultValue(){return {"mode":""};},
validateValue(value){Ros2Ur_dashboard_msgsMsgOperationalModeSize(Ros2Ur_dashboard_msgsMsgOperationalModeCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgOperationalModeCapture(value),n=Ros2Ur_dashboard_msgsMsgOperationalModeSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgOperationalModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgOperationalModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgOperationalModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["mode"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgOperationalModeWrite(value,w){
w.text(value["mode"],256);
}
function Ros2Ur_dashboard_msgsMsgOperationalModeRead(r){const value={};
{
const decoded=r.text(256);
value["mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgOperationalModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/ProgramInformation codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgProgramInformationCodec=Object.freeze({codecIdentity:"UWVMROS1_9299ce545acd0b6ff064998334820fbe503b71dc47a475ef93545d2cdb41f965",maxWireBytes:676,minWireBytes:36,cartesianMaxWireBytes:676n,aggregateWireBytes:0,constants:Object.freeze({"PROGRAM_STATE_DRAFT":"DRAFT","PROGRAM_STATE_FINAL":"FINAL"}),
defaultValue(){return {"name":"","description":"","created_date":0n,"last_saved_date":0n,"last_modified_date":0n,"program_state":""};},
validateValue(value){Ros2Ur_dashboard_msgsMsgProgramInformationSize(Ros2Ur_dashboard_msgsMsgProgramInformationCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgProgramInformationCapture(value),n=Ros2Ur_dashboard_msgsMsgProgramInformationSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgProgramInformationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,676,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgProgramInformationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgProgramInformationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["description"],256).length);
integer(value["created_date"],8,false);n=addSize(n,8);
integer(value["last_saved_date"],8,false);n=addSize(n,8);
integer(value["last_modified_date"],8,false);n=addSize(n,8);
n=addSize(n,4+textBytes(value["program_state"],256).length);
if(n>676)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgProgramInformationWrite(value,w){
w.text(value["name"],128);
w.text(value["description"],256);
w.v.setBigUint64(w.at,value["created_date"],true);w.at+=8;
w.v.setBigUint64(w.at,value["last_saved_date"],true);w.at+=8;
w.v.setBigUint64(w.at,value["last_modified_date"],true);w.at+=8;
w.text(value["program_state"],256);
}
function Ros2Ur_dashboard_msgsMsgProgramInformationRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["description"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["created_date"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["last_saved_date"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["last_modified_date"]=decoded;
}
{
const decoded=r.text(256);
value["program_state"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgProgramInformationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["description"];
out["description"]=field;
}
{const field=value["created_date"];
out["created_date"]=field;
}
{const field=value["last_saved_date"];
out["last_saved_date"]=field;
}
{const field=value["last_modified_date"];
out["last_modified_date"]=field;
}
{const field=value["program_state"];
out["program_state"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/ProgramState codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgProgramStateCodec=Object.freeze({codecIdentity:"UWVMROS1_1462a3c2323792afbbfc6d48a600a6ab66a50a6f6f583e57a62da2458ea81f26",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({"STOPPED":"STOPPED","PLAYING":"PLAYING","PAUSED":"PAUSED"}),
defaultValue(){return {"state":""};},
validateValue(value){Ros2Ur_dashboard_msgsMsgProgramStateSize(Ros2Ur_dashboard_msgsMsgProgramStateCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgProgramStateCapture(value),n=Ros2Ur_dashboard_msgsMsgProgramStateSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgProgramStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgProgramStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgProgramStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgProgramStateWrite(value,w){
w.text(value["state"],256);
}
function Ros2Ur_dashboard_msgsMsgProgramStateRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgProgramStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/RobotMode codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgRobotModeCodec=Object.freeze({codecIdentity:"UWVMROS1_81a79c84083520d911de108a728ad1877de78a57ec9e27b5cc432590be4616bb",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NO_CONTROLLER":-1,"DISCONNECTED":0,"CONFIRM_SAFETY":1,"BOOTING":2,"POWER_OFF":3,"POWER_ON":4,"IDLE":5,"BACKDRIVE":6,"RUNNING":7,"UPDATING_FIRMWARE":8}),
defaultValue(){return {"mode":0};},
validateValue(value){Ros2Ur_dashboard_msgsMsgRobotModeSize(Ros2Ur_dashboard_msgsMsgRobotModeCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgRobotModeCapture(value),n=Ros2Ur_dashboard_msgsMsgRobotModeSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgRobotModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgRobotModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgRobotModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgRobotModeWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
}
function Ros2Ur_dashboard_msgsMsgRobotModeRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgRobotModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/SafetyMode codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgSafetyModeCodec=Object.freeze({codecIdentity:"UWVMROS1_2eb3d5075a2d7d1765a604c1d1d198379010867d83a98d0327488ca51f5e1aac",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NORMAL":1,"REDUCED":2,"PROTECTIVE_STOP":3,"RECOVERY":4,"SAFEGUARD_STOP":5,"SYSTEM_EMERGENCY_STOP":6,"ROBOT_EMERGENCY_STOP":7,"VIOLATION":8,"FAULT":9,"VALIDATE_JOINT_ID":10,"UNDEFINED_SAFETY_MODE":11,"AUTOMATIC_MODE_SAFEGUARD_STOP":12,"SYSTEM_THREE_POSITION_ENABLING_STOP":13}),
defaultValue(){return {"mode":0};},
validateValue(value){Ros2Ur_dashboard_msgsMsgSafetyModeSize(Ros2Ur_dashboard_msgsMsgSafetyModeCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgSafetyModeCapture(value),n=Ros2Ur_dashboard_msgsMsgSafetyModeSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgSafetyModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgSafetyModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgSafetyModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgSafetyModeWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
}
function Ros2Ur_dashboard_msgsMsgSafetyModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgSafetyModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/SafetyStatus codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgSafetyStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_1b1a7257c40d9c62e0b6291f241825d029fe38f3f3695265a182a270899a79e9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NORMAL":1,"REDUCED":2,"PROTECTIVE_STOP":3,"RECOVERY":4,"SAFEGUARD_STOP":5,"SYSTEM_EMERGENCY_STOP":6,"ROBOT_EMERGENCY_STOP":7,"VIOLATION":8,"FAULT":9,"VALIDATE_JOINT_ID":10,"UNDEFINED_SAFETY_MODE":11,"AUTOMATIC_MODE_SAFEGUARD_STOP":12,"SYSTEM_THREE_POSITION_ENABLING_STOP":13,"TP_THREE_POSITION_ENABLING_STOP":14,"IMMI_EMERGENCY_STOP":15,"IMMI_SAFEGUARD_STOP":16,"PROFISAFE_WAITING_FOR_PARAMETERS":17,"PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP":18,"PROFISAFE_SAFEGUARD_STOP":19,"PROFISAFE_EMERGENCY_STOP":20,"SAFETY_API_SAFEGUARD_STOP":22,"IO_PLANE_STOP":23}),
defaultValue(){return {"status":0};},
validateValue(value){Ros2Ur_dashboard_msgsMsgSafetyStatusSize(Ros2Ur_dashboard_msgsMsgSafetyStatusCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgSafetyStatusCapture(value),n=Ros2Ur_dashboard_msgsMsgSafetyStatusSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgSafetyStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgSafetyStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgSafetyStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgSafetyStatusWrite(value,w){
w.v.setInt8(w.at,value["status"],true);w.at+=1;
}
function Ros2Ur_dashboard_msgsMsgSafetyStatusRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["status"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgSafetyStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/UserRole codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgUserRoleCodec=Object.freeze({codecIdentity:"UWVMROS1_7568bc9417685a9bc56cfdd2b41ff81ab99bd8e20fbe5f9e537b5b5d0689440a",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({"PROGRAMMER":"PROGRAMMER","OPERATOR":"OPERATOR","NONE":"NONE","LOCKED":"LOCKED","RESTRICTED":"RESTRICTED"}),
defaultValue(){return {"role":""};},
validateValue(value){Ros2Ur_dashboard_msgsMsgUserRoleSize(Ros2Ur_dashboard_msgsMsgUserRoleCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgUserRoleCapture(value),n=Ros2Ur_dashboard_msgsMsgUserRoleSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgUserRoleWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgUserRoleRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgUserRoleSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["role"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgUserRoleWrite(value,w){
w.text(value["role"],256);
}
function Ros2Ur_dashboard_msgsMsgUserRoleRead(r){const value={};
{
const decoded=r.text(256);
value["role"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgUserRoleCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["role"];
out["role"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/msg/VersionInformation codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsMsgVersionInformationCodec=Object.freeze({codecIdentity:"UWVMROS1_450c41c4361f1bca51f864dd9ef596851b17ebc79d4f784e7abb8b721752443d",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"major":0,"minor":0,"bugfix":0,"build":0};},
validateValue(value){Ros2Ur_dashboard_msgsMsgVersionInformationSize(Ros2Ur_dashboard_msgsMsgVersionInformationCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsMsgVersionInformationCapture(value),n=Ros2Ur_dashboard_msgsMsgVersionInformationSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsMsgVersionInformationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsMsgVersionInformationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsMsgVersionInformationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["major"],4,false);n=addSize(n,4);
integer(value["minor"],4,false);n=addSize(n,4);
integer(value["bugfix"],4,false);n=addSize(n,4);
integer(value["build"],4,false);n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ur_dashboard_msgsMsgVersionInformationWrite(value,w){
w.v.setUint32(w.at,value["major"],true);w.at+=4;
w.v.setUint32(w.at,value["minor"],true);w.at+=4;
w.v.setUint32(w.at,value["bugfix"],true);w.at+=4;
w.v.setUint32(w.at,value["build"],true);w.at+=4;
}
function Ros2Ur_dashboard_msgsMsgVersionInformationRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["major"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["minor"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["bugfix"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["build"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsMsgVersionInformationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["major"];
out["major"]=field;
}
{const field=value["minor"];
out["minor"]=field;
}
{const field=value["bugfix"];
out["bugfix"]=field;
}
{const field=value["build"];
out["build"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/AddToLog_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ecfe5bb0d0db92bc5dfdec9bf82fbe730e3b3b4d65680812d0540b9055590142",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvAddToLog_RequestSize(Ros2Ur_dashboard_msgsSrvAddToLog_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvAddToLog_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvAddToLog_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvAddToLog_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvAddToLog_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvAddToLog_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvAddToLog_RequestWrite(value,w){
w.text(value["message"],256);
}
function Ros2Ur_dashboard_msgsSrvAddToLog_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvAddToLog_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/AddToLog_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_57b0c150a353d3fca6e96c4dc86e72f71b452663ede1eeb69e51069992d07e4c",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvAddToLog_ResponseSize(Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvAddToLog_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvAddToLog_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvAddToLog_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvAddToLog_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvAddToLog_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvAddToLog_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadProgram_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5c84420393aae13ea448d171a7b2956a55a39f3321f504b9f3bd5b4a38960c2f",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"program_name":"","target_path":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestSize(Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["program_name"],256).length);
n=addSize(n,4+textBytes(value["target_path"],256).length);
if(n>520)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestWrite(value,w){
w.text(value["program_name"],256);
w.text(value["target_path"],256);
}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["program_name"]=decoded;
}
{
const decoded=r.text(256);
value["target_path"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["target_path"];
out["target_path"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadProgram_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_46b0a73805282bad5f819f712275b7dd2ca4a6bb8c73c445ea8ae0a3d0f9e8e0",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseSize(Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d919563629f81d5a8c10f1d2921ea16f3c912a49b8d3bcf65d17b8daa0a38140",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_path":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestSize(Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["target_path"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestWrite(value,w){
w.text(value["target_path"],256);
}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["target_path"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_path"];
out["target_path"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_76e190e6e9f1ddfff3082ea2adb0463d21ba9fb6795153fe6bbacee8a3334667",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false,"support_files_present":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseSize(Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["support_files_present"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["support_files_present"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["support_files_present"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["support_files_present"];
out["support_files_present"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7246d97690c0c827e9cfab2f8297cf27a10e60e41ebb3ba345f361a109057099",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({"CONTROLLER":"controller","SOFTWARE":"software","SYSTEM":"system"}),
defaultValue(){return {"report_type":"SYSTEM"};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestSize(Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["report_type"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestWrite(value,w){
w.text(value["report_type"],256);
}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["report_type"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["report_type"];
out["report_type"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_adebf67876b5ae3377cec0aa04138c2ab73ef7c0d7942c77c6c06b248a836445",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"answer":"","report_id":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseSize(Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["report_id"],256).length);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["answer"],256);
w.text(value["report_id"],256);
}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["report_id"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["report_id"];
out["report_id"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_10b0371b77db32d75a77dcb0605b325caa1915ccfb7d0917896fd75234e6ada7",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dir_path":"."};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestSize(Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["dir_path"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestWrite(value,w){
w.text(value["dir_path"],256);
}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["dir_path"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dir_path"];
out["dir_path"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f370623fe690f218073f8538aa9fcf41c94d4abd3e0507e09607ef11e10edf38",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"answer":"","generated_file_name":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseSize(Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["generated_file_name"],256).length);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["answer"],256);
w.text(value["generated_file_name"],256);
}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["generated_file_name"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["generated_file_name"];
out["generated_file_name"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_030e5746e6a4da8f266e37d11cc3826715099ff1aa1c845a97042aec987707e3",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestSize(Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3e8c7f3c8711faea01de37b9a011f34a9c6dd660cc222e46177b2c1898ec2f7e",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_name":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseSize(Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["program_name"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseWrite(value,w){
w.text(value["answer"],256);
w.text(value["program_name"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["program_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3f348b5d9e04a8ba01f5277ad259c34a6816ae0693f47c49e348a5497a90a325",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestSize(Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6c2da335fd551162020eb394b91e99aaeca17e34e7c2e98de935c81912efb5f6",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"answer":"","operational_mode":Ros2Ur_dashboard_msgsMsgOperationalModeCodec.defaultValue()};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseSize(Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,Ros2Ur_dashboard_msgsMsgOperationalModeSize(value["operational_mode"]));
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["answer"],256);
Ros2Ur_dashboard_msgsMsgOperationalModeWrite(value["operational_mode"],w);
}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgOperationalModeRead(r);
value["operational_mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["operational_mode"];
out["operational_mode"]=Ros2Ur_dashboard_msgsMsgOperationalModeCapture(field);
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_88193ae202f0cb02746bc5556e8e8ee277fb4fae72ee4b96a6539c980dc86f13",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestSize(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c8d53d0a920806e86ceb6d2ac5a692a92e62b057432554bed9dcf57bed0ff9bd",maxWireBytes:277,minWireBytes:21,cartesianMaxWireBytes:277n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false,"version":Ros2Ur_dashboard_msgsMsgVersionInformationCodec.defaultValue()};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseSize(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,277,90);if(owned.length<21)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Ur_dashboard_msgsMsgVersionInformationSize(value["version"]));
if(n>277)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
Ros2Ur_dashboard_msgsMsgVersionInformationWrite(value["version"],w);
}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgVersionInformationRead(r);
value["version"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["version"];
out["version"]=Ros2Ur_dashboard_msgsMsgVersionInformationCapture(field);
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetProgramState_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fe42d1d1aa3d7bdbaa410bce045120d6dd841213ef10c5dd158c2b76fa157f65",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetProgramState_RequestSize(Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetProgramState_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetProgramState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetProgramState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetProgramState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetProgramState_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetProgramState_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetProgramState_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b20731b8552f323837c5939f390060a6c7c92e9447df5e620891708cfce51ff4",maxWireBytes:781,minWireBytes:13,cartesianMaxWireBytes:781n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros2Ur_dashboard_msgsMsgProgramStateCodec.defaultValue(),"program_name":"","answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseSize(Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,781,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgProgramStateSize(value["state"]));
n=addSize(n,4+textBytes(value["program_name"],256).length);
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>781)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseWrite(value,w){
Ros2Ur_dashboard_msgsMsgProgramStateWrite(value["state"],w);
w.text(value["program_name"],256);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgProgramStateRead(r);
value["state"]=decoded;
}
{
const decoded=r.text(256);
value["program_name"]=decoded;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros2Ur_dashboard_msgsMsgProgramStateCapture(field);
}
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPrograms_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8d7b4f3fb9a15854b59c85c5205bf20d3e0f6a1a972ccbdf77ee8377c9e40d6d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetPrograms_RequestSize(Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetPrograms_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetPrograms_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetPrograms_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetPrograms_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetPrograms_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetPrograms_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPrograms_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_111b67c562ba0c77c2924b97058e4126055ad6deb7f99049b8caf8990b3f979c",maxWireBytes:11081,minWireBytes:9,cartesianMaxWireBytes:11081n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"programs":[],"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseSize(Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,11081,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["programs"])||value["programs"].length>16)bad();
n=addSize(n,4);
for(const element of value["programs"]){
n=addSize(n,Ros2Ur_dashboard_msgsMsgProgramInformationSize(element));
}
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>11081)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseWrite(value,w){
w.v.setUint32(w.at,value["programs"].length,true);w.at+=4;
for(const element of value["programs"]){
Ros2Ur_dashboard_msgsMsgProgramInformationWrite(element,w);
}
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["programs"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_dashboard_msgsMsgProgramInformationRead(r);
value["programs"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["programs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_dashboard_msgsMsgProgramInformationCapture(element);
}out["programs"]=values;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotMode_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d1d9cd94a443b20ec0d41a8db1a6d87291657597abc014e7da0f46a58c6c5614",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestSize(Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotMode_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_67db750ad95b3b207d61b18513a117c868040c22c93638f07fa74645d87bcfba",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robot_mode":Ros2Ur_dashboard_msgsMsgRobotModeCodec.defaultValue(),"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseSize(Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgRobotModeSize(value["robot_mode"]));
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseWrite(value,w){
Ros2Ur_dashboard_msgsMsgRobotModeWrite(value["robot_mode"],w);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgRobotModeRead(r);
value["robot_mode"]=decoded;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robot_mode"];
out["robot_mode"]=Ros2Ur_dashboard_msgsMsgRobotModeCapture(field);
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotModel_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9db08f07993bd5786aea5b6bd567f9e9f49329883326ea16e6c49eef1586dc43",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestSize(Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotModel_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_faf0124c542ef7420915527c27e99a7d26c5d6274f55d869bc9416e6b7d9b3c9",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"answer":"","robot_model":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseSize(Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["robot_model"],256).length);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["answer"],256);
w.text(value["robot_model"],256);
}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["robot_model"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["robot_model"];
out["robot_model"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5866cc4b0e10508eeb45ceecf9f9b6036ba55d04c9613622e189ccde2781dcb0",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestSize(Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f65d7ff83dc97f891032d8ab16bf71a9c2326c6ae6a033fe1d6e06f011c5a40b",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"safety_mode":Ros2Ur_dashboard_msgsMsgSafetyModeCodec.defaultValue(),"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseSize(Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgSafetyModeSize(value["safety_mode"]));
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseWrite(value,w){
Ros2Ur_dashboard_msgsMsgSafetyModeWrite(value["safety_mode"],w);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgSafetyModeRead(r);
value["safety_mode"]=decoded;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["safety_mode"];
out["safety_mode"]=Ros2Ur_dashboard_msgsMsgSafetyModeCapture(field);
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0e2958bcd7b121682361c94b2926bcc4552d7ef07dc2af4609d04b1bb7054602",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestSize(Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bb96aaa96f5c78b97ad80a14eb96865dc10b5a1d12524fcbcc75fc6d481c53e4",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false,"safety_status":Ros2Ur_dashboard_msgsMsgSafetyStatusCodec.defaultValue()};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseSize(Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Ur_dashboard_msgsMsgSafetyStatusSize(value["safety_status"]));
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
Ros2Ur_dashboard_msgsMsgSafetyStatusWrite(value["safety_status"],w);
}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgSafetyStatusRead(r);
value["safety_status"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["safety_status"];
out["safety_status"]=Ros2Ur_dashboard_msgsMsgSafetyStatusCapture(field);
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bbb1ca066c77ed459352de69d566f3ea87b94fe593c31c911193416f92a73c9b",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestSize(Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f389641d089fa6fd421348f047732de2122cd714bdb2bd4d1e611592aca349e7",maxWireBytes:269,minWireBytes:13,cartesianMaxWireBytes:269n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"serial_number":0n,"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseSize(Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,269,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["serial_number"],8,false);n=addSize(n,8);
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>269)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["serial_number"],true);w.at+=8;
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["serial_number"]=decoded;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["serial_number"];
out["serial_number"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetUserRole_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a331be09dd2093e4584e6dc80f83caaadd3a8e43cca6a1c8fcfa1e2ea632c9f0",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetUserRole_RequestSize(Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvGetUserRole_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetUserRole_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetUserRole_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetUserRole_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetUserRole_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvGetUserRole_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/GetUserRole_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ed4f53e59fbe4cd33cfbf5d190b60fbe8371dda7da9d7af6e270a882eb142df8",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"user_role":Ros2Ur_dashboard_msgsMsgUserRoleCodec.defaultValue(),"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseSize(Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgUserRoleSize(value["user_role"]));
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseWrite(value,w){
Ros2Ur_dashboard_msgsMsgUserRoleWrite(value["user_role"],w);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgUserRoleRead(r);
value["user_role"]=decoded;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["user_role"];
out["user_role"]=Ros2Ur_dashboard_msgsMsgUserRoleCapture(field);
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4b6b86fc4337c36589e8781b46ddbe6992f750b565ff413ea0b069982d87a463",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestSize(Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f2705c1be319c7c667f4f2224851b06c24b15b4211c2828fab89f09455e4c816",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","remote_control":false,"success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseSize(Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["remote_control"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["remote_control"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["remote_control"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["remote_control"];
out["remote_control"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f28274c0dc01d8aa2f981f969c05a614e6b3922601d578f99c6321a4b20fc80f",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestSize(Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c57cc7ecd7e81f7359824b43884a58f65748a4c77cacbcc45be17aa9147cf499",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_running":false,"success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseSize(Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["program_running"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["program_running"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["program_running"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["program_running"];
out["program_running"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c53d57cffc0480d66257150a8450ba90563fda5153742784831d38abcbc0b099",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestSize(Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestWrite(value,w){
}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestRead(r){const value={};
return value;}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_aa171247f31fd00a1b8820ee21cf20d159bd10d0d2887dc96b3a4dd4d372d115",maxWireBytes:522,minWireBytes:10,cartesianMaxWireBytes:522n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_name":"","program_saved":false,"success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseSize(Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,522,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["program_name"],256).length);
if(typeof value["program_saved"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>522)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseWrite(value,w){
w.text(value["answer"],256);
w.text(value["program_name"],256);
w.v.setUint8(w.at,value["program_saved"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["program_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["program_saved"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["program_saved"];
out["program_saved"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/Load_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvLoad_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1d0cd6cdeaaafce999a7e9924721246b9f4954f22da40fd0a27fb7b657324876",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvLoad_RequestSize(Ros2Ur_dashboard_msgsSrvLoad_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvLoad_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvLoad_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvLoad_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvLoad_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvLoad_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvLoad_RequestWrite(value,w){
w.text(value["filename"],256);
}
function Ros2Ur_dashboard_msgsSrvLoad_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["filename"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvLoad_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/Load_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4e694de8079133a8e789add3640dfe19805cad92991a650167c2aae7067da415",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvLoad_ResponseSize(Ros2Ur_dashboard_msgsSrvLoad_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvLoad_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvLoad_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvLoad_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvLoad_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvLoad_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvLoad_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvLoad_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvLoad_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/Popup_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvPopup_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_369e6eab4f689a846c52fcc5cb539da63fab3939fab254dd1e04c7b274d5f770",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":"","title":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvPopup_RequestSize(Ros2Ur_dashboard_msgsSrvPopup_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvPopup_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvPopup_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvPopup_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvPopup_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvPopup_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
n=addSize(n,4+textBytes(value["title"],256).length);
if(n>520)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvPopup_RequestWrite(value,w){
w.text(value["message"],256);
w.text(value["title"],256);
}
function Ros2Ur_dashboard_msgsSrvPopup_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
const decoded=r.text(256);
value["title"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvPopup_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
{const field=value["title"];
out["title"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/Popup_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0b54f7c477c1ddf8ccb18d12d50eaf308225d4927ee377377ed354426e58f984",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvPopup_ResponseSize(Ros2Ur_dashboard_msgsSrvPopup_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvPopup_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvPopup_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvPopup_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvPopup_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvPopup_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvPopup_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvPopup_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvPopup_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/RawRequest_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2bf66d87b20ca2de9ed94c04c79042a50239728320b43b27c9942405795b4555",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"query":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvRawRequest_RequestSize(Ros2Ur_dashboard_msgsSrvRawRequest_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvRawRequest_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvRawRequest_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvRawRequest_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvRawRequest_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvRawRequest_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["query"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvRawRequest_RequestWrite(value,w){
w.text(value["query"],256);
}
function Ros2Ur_dashboard_msgsSrvRawRequest_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["query"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvRawRequest_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["query"];
out["query"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/RawRequest_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7c37f54149bab54f6e7bbfed41992eb54416b31346bc889a9632d5497111267f",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvRawRequest_ResponseSize(Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvRawRequest_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvRawRequest_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvRawRequest_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvRawRequest_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvRawRequest_ResponseWrite(value,w){
w.text(value["answer"],256);
}
function Ros2Ur_dashboard_msgsSrvRawRequest_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8cc61f7c64211c46e01e52eee2c05f6e709f4f0900cf24fdfd4079b0f7581a00",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"operational_mode":Ros2Ur_dashboard_msgsMsgOperationalModeCodec.defaultValue()};},
validateValue(value){Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestSize(Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgOperationalModeSize(value["operational_mode"]));
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestWrite(value,w){
Ros2Ur_dashboard_msgsMsgOperationalModeWrite(value["operational_mode"],w);
}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgOperationalModeRead(r);
value["operational_mode"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["operational_mode"];
out["operational_mode"]=Ros2Ur_dashboard_msgsMsgOperationalModeCapture(field);
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f5009de077224371e9d2f6c0aecbabd24c80148f2c7e1b1383a64f6f214b202a",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseSize(Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/SetUserRole_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7bd499947803efca81401a2de496d24e7e54ba97f4f86ec2b3089146a0655f5e",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"user_role":Ros2Ur_dashboard_msgsMsgUserRoleCodec.defaultValue()};},
validateValue(value){Ros2Ur_dashboard_msgsSrvSetUserRole_RequestSize(Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvSetUserRole_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvSetUserRole_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvSetUserRole_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvSetUserRole_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_dashboard_msgsMsgUserRoleSize(value["user_role"]));
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvSetUserRole_RequestWrite(value,w){
Ros2Ur_dashboard_msgsMsgUserRoleWrite(value["user_role"],w);
}
function Ros2Ur_dashboard_msgsSrvSetUserRole_RequestRead(r){const value={};
{
const decoded=Ros2Ur_dashboard_msgsMsgUserRoleRead(r);
value["user_role"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["user_role"];
out["user_role"]=Ros2Ur_dashboard_msgsMsgUserRoleCapture(field);
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/SetUserRole_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4cab2bba5030f4e4b9e90252d64ec2e02d6eb87070b75cf43190fecd21da59fd",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"answer":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseSize(Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["answer"],256).length);
if(n>261)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["answer"],256);
}
function Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["answer"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["answer"];
out["answer"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/UploadProgram_Request codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f7501d0115f8d36b1942ce610b1bbc197d0276de508ac8175d539f40494f437b",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"file_path":""};},
validateValue(value){Ros2Ur_dashboard_msgsSrvUploadProgram_RequestSize(Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCapture(value),n=Ros2Ur_dashboard_msgsSrvUploadProgram_RequestSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvUploadProgram_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvUploadProgram_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvUploadProgram_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["file_path"],256).length);
if(n>260)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvUploadProgram_RequestWrite(value,w){
w.text(value["file_path"],256);
}
function Ros2Ur_dashboard_msgsSrvUploadProgram_RequestRead(r){const value={};
{
const decoded=r.text(256);
value["file_path"]=decoded;
}
return value;}
function Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["file_path"];
out["file_path"]=field;
}
return out;}
/** Complete ROS2 ur_dashboard_msgs/srv/UploadProgram_Response codec; maxima are not actual lengths. */
export const Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ae9a28c1cd4729f54968d18a16e2f8f417bdc5e9bc15809de7481f1f20e6b398",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_name":"","success":false};},
validateValue(value){Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseSize(Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCapture(value),n=Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseSize(owned),w=new Writer(n);Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["program_name"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseWrite(value,w){
w.text(value["answer"],256);
w.text(value["program_name"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.text(256);
value["program_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Goal codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_78fda8f2d20c502e24b1ad4fe55f34bba41de20ff88e71b56d7a0631d7a3905a",maxWireBytes:7053,minWireBytes:37,cartesianMaxWireBytes:7053n,aggregateWireBytes:0,constants:Object.freeze({"TOOL_CONTACT":0}),
defaultValue(){return {"trajectory":Ros2Trajectory_msgsMsgJointTrajectoryCodec.defaultValue(),"path_tolerance":[],"goal_tolerance":[],"goal_time_tolerance":Ros2Builtin_interfacesMsgDurationCodec.defaultValue(),"until_type":0};},
validateValue(value){Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalSize(Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCapture(value),n=Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalSize(owned),w=new Writer(n);Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7053,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectorySize(value["trajectory"]));
if(!Array.isArray(value["path_tolerance"])||value["path_tolerance"].length>6)bad();
n=addSize(n,4);
for(const element of value["path_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointToleranceSize(element));
}
if(!Array.isArray(value["goal_tolerance"])||value["goal_tolerance"].length>6)bad();
n=addSize(n,4);
for(const element of value["goal_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointToleranceSize(element));
}
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["goal_time_tolerance"]));
integer(value["until_type"],1,false);n=addSize(n,1);
if(n>7053)bad(90);return n;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalWrite(value,w){
Ros2Trajectory_msgsMsgJointTrajectoryWrite(value["trajectory"],w);
w.v.setUint32(w.at,value["path_tolerance"].length,true);w.at+=4;
for(const element of value["path_tolerance"]){
Ros2Control_msgsMsgJointToleranceWrite(element,w);
}
w.v.setUint32(w.at,value["goal_tolerance"].length,true);w.at+=4;
for(const element of value["goal_tolerance"]){
Ros2Control_msgsMsgJointToleranceWrite(element,w);
}
Ros2Builtin_interfacesMsgDurationWrite(value["goal_time_tolerance"],w);
w.v.setUint8(w.at,value["until_type"],true);w.at+=1;
}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalRead(r){const value={};
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryRead(r);
value["trajectory"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["path_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointToleranceRead(r);
value["path_tolerance"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["goal_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointToleranceRead(r);
value["goal_tolerance"].push(decoded);
}}
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["goal_time_tolerance"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["until_type"]=decoded;
}
return value;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trajectory"];
out["trajectory"]=Ros2Trajectory_msgsMsgJointTrajectoryCapture(field);
}
{const field=value["path_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointToleranceCapture(element);
}out["path_tolerance"]=values;
}
{const field=value["goal_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointToleranceCapture(element);
}out["goal_tolerance"]=values;
}
{const field=value["goal_time_tolerance"];
out["goal_time_tolerance"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
{const field=value["until_type"];
out["until_type"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Result codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_de92b9ccfb9c93f3afae9da5e8e31df9a6d6a606479fe27b9bcf14ff329bc869",maxWireBytes:268,minWireBytes:12,cartesianMaxWireBytes:268n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESSFUL":0,"INVALID_GOAL":-1,"INVALID_JOINTS":-2,"OLD_HEADER_TIMESTAMP":-3,"PATH_TOLERANCE_VIOLATED":-4,"GOAL_TOLERANCE_VIOLATED":-5,"TRIGGERED":0,"NOT_TRIGGERED":1}),
defaultValue(){return {"error_code":0,"until_condition_result":0,"error_string":""};},
validateValue(value){Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultSize(Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCapture(value),n=Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultSize(owned),w=new Writer(n);Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,268,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["error_code"],4,true);n=addSize(n,4);
integer(value["until_condition_result"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["error_string"],256).length);
if(n>268)bad(90);return n;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultWrite(value,w){
w.v.setInt32(w.at,value["error_code"],true);w.at+=4;
w.v.setInt32(w.at,value["until_condition_result"],true);w.at+=4;
w.text(value["error_string"],256);
}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["until_condition_result"]=decoded;
}
{
const decoded=r.text(256);
value["error_string"]=decoded;
}
return value;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["until_condition_result"];
out["until_condition_result"]=field;
}
{const field=value["error_string"];
out["error_string"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Feedback codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_5a719f991f5c73784f9dc9e962ecfa0541e00daef7f747c312d4b633d63040ac",maxWireBytes:2352,minWireBytes:88,cartesianMaxWireBytes:2352n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_names":[],"desired":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue(),"actual":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue(),"error":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue()};},
validateValue(value){Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackSize(Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCapture(value),n=Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackSize(owned),w=new Writer(n);Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2352,90);if(owned.length<88)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>6)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["desired"]));
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["actual"]));
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["error"]));
if(n>2352)bad(90);return n;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,256);
}
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["desired"],w);
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["actual"],w);
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["error"],w);
}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["joint_names"].push(decoded);
}}
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["desired"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["actual"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["error"]=decoded;
}
return value;}
function Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["desired"];
out["desired"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
{const field=value["actual"];
out["actual"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
{const field=value["error"];
out["error"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
return out;}
/** Complete ROS2 ur_msgs/action/SendScript_Goal codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionSendScript_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_c277cd00663786f95201e61baa658b92def3becc1f37e54aa6093bb11135819c",maxWireBytes:8465,minWireBytes:17,cartesianMaxWireBytes:8465n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"program":"","script_name":"","start_timeout":Ros2Builtin_interfacesMsgDurationCodec.defaultValue(),"fail_on_warnings":true};},
validateValue(value){Ros2Ur_msgsActionSendScript_GoalSize(Ros2Ur_msgsActionSendScript_GoalCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionSendScript_GoalCapture(value),n=Ros2Ur_msgsActionSendScript_GoalSize(owned),w=new Writer(n);Ros2Ur_msgsActionSendScript_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8465,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionSendScript_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionSendScript_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["program"],8192).length);
n=addSize(n,4+textBytes(value["script_name"],256).length);
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["start_timeout"]));
if(typeof value["fail_on_warnings"]!=="boolean")bad();n=addSize(n,1);
if(n>8465)bad(90);return n;}
function Ros2Ur_msgsActionSendScript_GoalWrite(value,w){
w.text(value["program"],8192);
w.text(value["script_name"],256);
Ros2Builtin_interfacesMsgDurationWrite(value["start_timeout"],w);
w.v.setUint8(w.at,value["fail_on_warnings"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsActionSendScript_GoalRead(r){const value={};
{
const decoded=r.text(8192);
value["program"]=decoded;
}
{
const decoded=r.text(256);
value["script_name"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["start_timeout"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["fail_on_warnings"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsActionSendScript_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["program"];
out["program"]=field;
}
{const field=value["script_name"];
out["script_name"]=field;
}
{const field=value["start_timeout"];
out["start_timeout"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
{const field=value["fail_on_warnings"];
out["fail_on_warnings"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/action/SendScript_Result codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionSendScript_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_d4c4d9721c3a576f590ba4967b2444a82969d6ccba94ddbf8c4a974535ee17d2",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Ur_msgsActionSendScript_ResultSize(Ros2Ur_msgsActionSendScript_ResultCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionSendScript_ResultCapture(value),n=Ros2Ur_msgsActionSendScript_ResultSize(owned),w=new Writer(n);Ros2Ur_msgsActionSendScript_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionSendScript_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionSendScript_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Ur_msgsActionSendScript_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Ur_msgsActionSendScript_ResultRead(r){const value={};
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
function Ros2Ur_msgsActionSendScript_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/action/SendScript_Feedback codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionSendScript_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_094876d6467bd2b34f74cc6b574393117753add5254285e4e95d85b07328ef2c",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_msgsActionSendScript_FeedbackSize(Ros2Ur_msgsActionSendScript_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionSendScript_FeedbackCapture(value),n=Ros2Ur_msgsActionSendScript_FeedbackSize(owned),w=new Writer(n);Ros2Ur_msgsActionSendScript_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionSendScript_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionSendScript_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_msgsActionSendScript_FeedbackWrite(value,w){
}
function Ros2Ur_msgsActionSendScript_FeedbackRead(r){const value={};
return value;}
function Ros2Ur_msgsActionSendScript_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_msgs/action/ToolContact_Goal codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionToolContact_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_150298ad16b1bc37d47bd3324cd4ec301ad8402c07779e8a435ce61daa7578dd",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_msgsActionToolContact_GoalSize(Ros2Ur_msgsActionToolContact_GoalCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionToolContact_GoalCapture(value),n=Ros2Ur_msgsActionToolContact_GoalSize(owned),w=new Writer(n);Ros2Ur_msgsActionToolContact_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionToolContact_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionToolContact_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_msgsActionToolContact_GoalWrite(value,w){
}
function Ros2Ur_msgsActionToolContact_GoalRead(r){const value={};
return value;}
function Ros2Ur_msgsActionToolContact_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_msgs/action/ToolContact_Result codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionToolContact_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_b79f6080ac18b67c25131458a2af0a3c235d624c00fc5c13da534cc546c94578",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_msgsActionToolContact_ResultSize(Ros2Ur_msgsActionToolContact_ResultCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionToolContact_ResultCapture(value),n=Ros2Ur_msgsActionToolContact_ResultSize(owned),w=new Writer(n);Ros2Ur_msgsActionToolContact_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionToolContact_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionToolContact_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_msgsActionToolContact_ResultWrite(value,w){
}
function Ros2Ur_msgsActionToolContact_ResultRead(r){const value={};
return value;}
function Ros2Ur_msgsActionToolContact_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_msgs/action/ToolContact_Feedback codec; maxima are not actual lengths. */
export const Ros2Ur_msgsActionToolContact_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_c6a11fd615f72a70a98514703de4c007a2bb161fbd8a953c5e8b899f471f4c5b",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_msgsActionToolContact_FeedbackSize(Ros2Ur_msgsActionToolContact_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsActionToolContact_FeedbackCapture(value),n=Ros2Ur_msgsActionToolContact_FeedbackSize(owned),w=new Writer(n);Ros2Ur_msgsActionToolContact_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_msgsActionToolContact_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsActionToolContact_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_msgsActionToolContact_FeedbackWrite(value,w){
}
function Ros2Ur_msgsActionToolContact_FeedbackRead(r){const value={};
return value;}
function Ros2Ur_msgsActionToolContact_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_msgs/msg/Analog codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgAnalogCodec=Object.freeze({codecIdentity:"UWVMROS1_68873e85836e1a65d7b67a52f5db8b3c3be082f889d2df4342386b282988f2a4",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"CURRENT":0,"VOLTAGE":1}),
defaultValue(){return {"pin":0,"domain":0,"state":0};},
validateValue(value){Ros2Ur_msgsMsgAnalogSize(Ros2Ur_msgsMsgAnalogCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgAnalogCapture(value),n=Ros2Ur_msgsMsgAnalogSize(owned),w=new Writer(n);Ros2Ur_msgsMsgAnalogWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgAnalogRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgAnalogSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
integer(value["domain"],1,false);n=addSize(n,1);
if(typeof value["state"]!=="number"||!Number.isFinite(value["state"])||!Number.isFinite(Math.fround(value["state"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2Ur_msgsMsgAnalogWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["domain"],true);w.at+=1;
w.v.setFloat32(w.at,value["state"],true);w.at+=4;
}
function Ros2Ur_msgsMsgAnalogRead(r){const value={};
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
value["state"]=decoded;
}
return value;}
function Ros2Ur_msgsMsgAnalogCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["domain"];
out["domain"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/msg/Digital codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgDigitalCodec=Object.freeze({codecIdentity:"UWVMROS1_e5ebd0a17ee528cff1e0a636eb0be9c7e4128d3e77be722c5f1be73df4f4c11e",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pin":0,"state":false};},
validateValue(value){Ros2Ur_msgsMsgDigitalSize(Ros2Ur_msgsMsgDigitalCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgDigitalCapture(value),n=Ros2Ur_msgsMsgDigitalSize(owned),w=new Writer(n);Ros2Ur_msgsMsgDigitalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgDigitalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgDigitalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
if(typeof value["state"]!=="boolean")bad();n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Ur_msgsMsgDigitalWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["state"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsMsgDigitalRead(r){const value={};
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
function Ros2Ur_msgsMsgDigitalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/msg/FrictionModelParameters codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgFrictionModelParametersCodec=Object.freeze({codecIdentity:"UWVMROS1_7cc9c7af77340f56b0559b4e2ad32972ff65f2a6cf89ebfbe2c48c645f8e9c89",maxWireBytes:104,minWireBytes:8,cartesianMaxWireBytes:104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"viscous_scale":[],"coulomb_scale":[]};},
validateValue(value){Ros2Ur_msgsMsgFrictionModelParametersSize(Ros2Ur_msgsMsgFrictionModelParametersCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgFrictionModelParametersCapture(value),n=Ros2Ur_msgsMsgFrictionModelParametersSize(owned),w=new Writer(n);Ros2Ur_msgsMsgFrictionModelParametersWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,104,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgFrictionModelParametersRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgFrictionModelParametersSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["viscous_scale"])||value["viscous_scale"].length>6)bad();
n=addSize(n,4);
for(const element of value["viscous_scale"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["coulomb_scale"])||value["coulomb_scale"].length>6)bad();
n=addSize(n,4);
for(const element of value["coulomb_scale"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>104)bad(90);return n;}
function Ros2Ur_msgsMsgFrictionModelParametersWrite(value,w){
w.v.setUint32(w.at,value["viscous_scale"].length,true);w.at+=4;
for(const element of value["viscous_scale"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["coulomb_scale"].length,true);w.at+=4;
for(const element of value["coulomb_scale"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Ur_msgsMsgFrictionModelParametersRead(r){const value={};
{
{const count=r.u32();if(count>6)bad();value["viscous_scale"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["viscous_scale"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["coulomb_scale"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["coulomb_scale"].push(decoded);
}}
}
return value;}
function Ros2Ur_msgsMsgFrictionModelParametersCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["viscous_scale"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["viscous_scale"]=values;
}
{const field=value["coulomb_scale"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["coulomb_scale"]=values;
}
return out;}
/** Complete ROS2 ur_msgs/msg/IOStates codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgIOStatesCodec=Object.freeze({codecIdentity:"UWVMROS1_c22f9f784c89b6854fa84df416ed3704e3a312b15c8fc41162e7f5f53223fef5",maxWireBytes:404,minWireBytes:20,cartesianMaxWireBytes:404n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"digital_in_states":[],"digital_out_states":[],"flag_states":[],"analog_in_states":[],"analog_out_states":[]};},
validateValue(value){Ros2Ur_msgsMsgIOStatesSize(Ros2Ur_msgsMsgIOStatesCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgIOStatesCapture(value),n=Ros2Ur_msgsMsgIOStatesSize(owned),w=new Writer(n);Ros2Ur_msgsMsgIOStatesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,404,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgIOStatesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgIOStatesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["digital_in_states"])||value["digital_in_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_in_states"]){
n=addSize(n,Ros2Ur_msgsMsgDigitalSize(element));
}
if(!Array.isArray(value["digital_out_states"])||value["digital_out_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_out_states"]){
n=addSize(n,Ros2Ur_msgsMsgDigitalSize(element));
}
if(!Array.isArray(value["flag_states"])||value["flag_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["flag_states"]){
n=addSize(n,Ros2Ur_msgsMsgDigitalSize(element));
}
if(!Array.isArray(value["analog_in_states"])||value["analog_in_states"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_in_states"]){
n=addSize(n,Ros2Ur_msgsMsgAnalogSize(element));
}
if(!Array.isArray(value["analog_out_states"])||value["analog_out_states"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_out_states"]){
n=addSize(n,Ros2Ur_msgsMsgAnalogSize(element));
}
if(n>404)bad(90);return n;}
function Ros2Ur_msgsMsgIOStatesWrite(value,w){
w.v.setUint32(w.at,value["digital_in_states"].length,true);w.at+=4;
for(const element of value["digital_in_states"]){
Ros2Ur_msgsMsgDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["digital_out_states"].length,true);w.at+=4;
for(const element of value["digital_out_states"]){
Ros2Ur_msgsMsgDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["flag_states"].length,true);w.at+=4;
for(const element of value["flag_states"]){
Ros2Ur_msgsMsgDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["analog_in_states"].length,true);w.at+=4;
for(const element of value["analog_in_states"]){
Ros2Ur_msgsMsgAnalogWrite(element,w);
}
w.v.setUint32(w.at,value["analog_out_states"].length,true);w.at+=4;
for(const element of value["analog_out_states"]){
Ros2Ur_msgsMsgAnalogWrite(element,w);
}
}
function Ros2Ur_msgsMsgIOStatesRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["digital_in_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigitalRead(r);
value["digital_in_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["digital_out_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigitalRead(r);
value["digital_out_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["flag_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigitalRead(r);
value["flag_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["analog_in_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgAnalogRead(r);
value["analog_in_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["analog_out_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgAnalogRead(r);
value["analog_out_states"].push(decoded);
}}
}
return value;}
function Ros2Ur_msgsMsgIOStatesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["digital_in_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_msgsMsgDigitalCapture(element);
}out["digital_in_states"]=values;
}
{const field=value["digital_out_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_msgsMsgDigitalCapture(element);
}out["digital_out_states"]=values;
}
{const field=value["flag_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_msgsMsgDigitalCapture(element);
}out["flag_states"]=values;
}
{const field=value["analog_in_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_msgsMsgAnalogCapture(element);
}out["analog_in_states"]=values;
}
{const field=value["analog_out_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Ur_msgsMsgAnalogCapture(element);
}out["analog_out_states"]=values;
}
return out;}
/** Complete ROS2 ur_msgs/msg/MasterboardDataMsg codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgMasterboardDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_eb3b0aab27bb5683859356ca7cf9648e53d311c1fd32cac97f68ce1d4622a9e5",maxWireBytes:62,minWireBytes:62,cartesianMaxWireBytes:62n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"digital_input_bits":0,"digital_output_bits":0,"analog_input_range0":0,"analog_input_range1":0,"analog_input0":0,"analog_input1":0,"analog_output_domain0":0,"analog_output_domain1":0,"analog_output0":0,"analog_output1":0,"masterboard_temperature":0,"robot_voltage_48v":0,"robot_current":0,"master_io_current":0,"master_safety_state":0,"master_onoff_state":0};},
validateValue(value){Ros2Ur_msgsMsgMasterboardDataMsgSize(Ros2Ur_msgsMsgMasterboardDataMsgCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgMasterboardDataMsgCapture(value),n=Ros2Ur_msgsMsgMasterboardDataMsgSize(owned),w=new Writer(n);Ros2Ur_msgsMsgMasterboardDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,62,90);if(owned.length<62)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgMasterboardDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgMasterboardDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["digital_input_bits"],4,false);n=addSize(n,4);
integer(value["digital_output_bits"],4,false);n=addSize(n,4);
integer(value["analog_input_range0"],1,true);n=addSize(n,1);
integer(value["analog_input_range1"],1,true);n=addSize(n,1);
if(typeof value["analog_input0"]!=="number"||!Number.isFinite(value["analog_input0"]))bad();n=addSize(n,8);
if(typeof value["analog_input1"]!=="number"||!Number.isFinite(value["analog_input1"]))bad();n=addSize(n,8);
integer(value["analog_output_domain0"],1,true);n=addSize(n,1);
integer(value["analog_output_domain1"],1,true);n=addSize(n,1);
if(typeof value["analog_output0"]!=="number"||!Number.isFinite(value["analog_output0"]))bad();n=addSize(n,8);
if(typeof value["analog_output1"]!=="number"||!Number.isFinite(value["analog_output1"]))bad();n=addSize(n,8);
if(typeof value["masterboard_temperature"]!=="number"||!Number.isFinite(value["masterboard_temperature"])||!Number.isFinite(Math.fround(value["masterboard_temperature"])))bad();n=addSize(n,4);
if(typeof value["robot_voltage_48v"]!=="number"||!Number.isFinite(value["robot_voltage_48v"])||!Number.isFinite(Math.fround(value["robot_voltage_48v"])))bad();n=addSize(n,4);
if(typeof value["robot_current"]!=="number"||!Number.isFinite(value["robot_current"])||!Number.isFinite(Math.fround(value["robot_current"])))bad();n=addSize(n,4);
if(typeof value["master_io_current"]!=="number"||!Number.isFinite(value["master_io_current"])||!Number.isFinite(Math.fround(value["master_io_current"])))bad();n=addSize(n,4);
integer(value["master_safety_state"],1,false);n=addSize(n,1);
integer(value["master_onoff_state"],1,false);n=addSize(n,1);
if(n>62)bad(90);return n;}
function Ros2Ur_msgsMsgMasterboardDataMsgWrite(value,w){
w.v.setUint32(w.at,value["digital_input_bits"],true);w.at+=4;
w.v.setUint32(w.at,value["digital_output_bits"],true);w.at+=4;
w.v.setInt8(w.at,value["analog_input_range0"],true);w.at+=1;
w.v.setInt8(w.at,value["analog_input_range1"],true);w.at+=1;
w.v.setFloat64(w.at,value["analog_input0"],true);w.at+=8;
w.v.setFloat64(w.at,value["analog_input1"],true);w.at+=8;
w.v.setInt8(w.at,value["analog_output_domain0"],true);w.at+=1;
w.v.setInt8(w.at,value["analog_output_domain1"],true);w.at+=1;
w.v.setFloat64(w.at,value["analog_output0"],true);w.at+=8;
w.v.setFloat64(w.at,value["analog_output1"],true);w.at+=8;
w.v.setFloat32(w.at,value["masterboard_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["robot_voltage_48v"],true);w.at+=4;
w.v.setFloat32(w.at,value["robot_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["master_io_current"],true);w.at+=4;
w.v.setUint8(w.at,value["master_safety_state"],true);w.at+=1;
w.v.setUint8(w.at,value["master_onoff_state"],true);w.at+=1;
}
function Ros2Ur_msgsMsgMasterboardDataMsgRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["digital_input_bits"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["digital_output_bits"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_input_range0"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_input_range1"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_input0"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_input1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_output_domain0"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_output_domain1"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_output0"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_output1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["masterboard_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["robot_voltage_48v"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["robot_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["master_io_current"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["master_safety_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["master_onoff_state"]=decoded;
}
return value;}
function Ros2Ur_msgsMsgMasterboardDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["digital_input_bits"];
out["digital_input_bits"]=field;
}
{const field=value["digital_output_bits"];
out["digital_output_bits"]=field;
}
{const field=value["analog_input_range0"];
out["analog_input_range0"]=field;
}
{const field=value["analog_input_range1"];
out["analog_input_range1"]=field;
}
{const field=value["analog_input0"];
out["analog_input0"]=field;
}
{const field=value["analog_input1"];
out["analog_input1"]=field;
}
{const field=value["analog_output_domain0"];
out["analog_output_domain0"]=field;
}
{const field=value["analog_output_domain1"];
out["analog_output_domain1"]=field;
}
{const field=value["analog_output0"];
out["analog_output0"]=field;
}
{const field=value["analog_output1"];
out["analog_output1"]=field;
}
{const field=value["masterboard_temperature"];
out["masterboard_temperature"]=field;
}
{const field=value["robot_voltage_48v"];
out["robot_voltage_48v"]=field;
}
{const field=value["robot_current"];
out["robot_current"]=field;
}
{const field=value["master_io_current"];
out["master_io_current"]=field;
}
{const field=value["master_safety_state"];
out["master_safety_state"]=field;
}
{const field=value["master_onoff_state"];
out["master_onoff_state"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/msg/RobotModeDataMsg codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgRobotModeDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_0181d85868dc6251289c6f7bc723757fa573c1345b6502497e335655d328bb31",maxWireBytes:15,minWireBytes:15,cartesianMaxWireBytes:15n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"timestamp":0n,"is_robot_connected":false,"is_real_robot_enabled":false,"is_power_on_robot":false,"is_emergency_stopped":false,"is_protective_stopped":false,"is_program_running":false,"is_program_paused":false};},
validateValue(value){Ros2Ur_msgsMsgRobotModeDataMsgSize(Ros2Ur_msgsMsgRobotModeDataMsgCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgRobotModeDataMsgCapture(value),n=Ros2Ur_msgsMsgRobotModeDataMsgSize(owned),w=new Writer(n);Ros2Ur_msgsMsgRobotModeDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,15,90);if(owned.length<15)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgRobotModeDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgRobotModeDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["timestamp"],8,false);n=addSize(n,8);
if(typeof value["is_robot_connected"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_real_robot_enabled"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_power_on_robot"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_emergency_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_protective_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_program_running"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_program_paused"]!=="boolean")bad();n=addSize(n,1);
if(n>15)bad(90);return n;}
function Ros2Ur_msgsMsgRobotModeDataMsgWrite(value,w){
w.v.setBigUint64(w.at,value["timestamp"],true);w.at+=8;
w.v.setUint8(w.at,value["is_robot_connected"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_real_robot_enabled"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_power_on_robot"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_emergency_stopped"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_protective_stopped"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_program_running"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_program_paused"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsMsgRobotModeDataMsgRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["timestamp"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_robot_connected"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_real_robot_enabled"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_power_on_robot"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_emergency_stopped"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_protective_stopped"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_program_running"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_program_paused"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsMsgRobotModeDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["timestamp"];
out["timestamp"]=field;
}
{const field=value["is_robot_connected"];
out["is_robot_connected"]=field;
}
{const field=value["is_real_robot_enabled"];
out["is_real_robot_enabled"]=field;
}
{const field=value["is_power_on_robot"];
out["is_power_on_robot"]=field;
}
{const field=value["is_emergency_stopped"];
out["is_emergency_stopped"]=field;
}
{const field=value["is_protective_stopped"];
out["is_protective_stopped"]=field;
}
{const field=value["is_program_running"];
out["is_program_running"]=field;
}
{const field=value["is_program_paused"];
out["is_program_paused"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/msg/RobotStateRTMsg codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgRobotStateRTMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_a30600c42e98b89da49d63073a5d30b0989aa0cb21f2bafec229dee10591d9cd",maxWireBytes:768,minWireBytes:96,cartesianMaxWireBytes:768n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"time":0,"q_target":[],"qd_target":[],"qdd_target":[],"i_target":[],"m_target":[],"q_actual":[],"qd_actual":[],"i_actual":[],"tool_acc_values":[],"tcp_force":[],"tool_vector":[],"tcp_speed":[],"digital_input_bits":0,"motor_temperatures":[],"controller_timer":0,"test_value":0,"robot_mode":0,"joint_modes":[]};},
validateValue(value){Ros2Ur_msgsMsgRobotStateRTMsgSize(Ros2Ur_msgsMsgRobotStateRTMsgCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgRobotStateRTMsgCapture(value),n=Ros2Ur_msgsMsgRobotStateRTMsgSize(owned),w=new Writer(n);Ros2Ur_msgsMsgRobotStateRTMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,768,90);if(owned.length<96)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgRobotStateRTMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgRobotStateRTMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["time"]!=="number"||!Number.isFinite(value["time"]))bad();n=addSize(n,8);
if(!Array.isArray(value["q_target"])||value["q_target"].length>6)bad();
n=addSize(n,4);
for(const element of value["q_target"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["qd_target"])||value["qd_target"].length>6)bad();
n=addSize(n,4);
for(const element of value["qd_target"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["qdd_target"])||value["qdd_target"].length>6)bad();
n=addSize(n,4);
for(const element of value["qdd_target"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["i_target"])||value["i_target"].length>6)bad();
n=addSize(n,4);
for(const element of value["i_target"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["m_target"])||value["m_target"].length>6)bad();
n=addSize(n,4);
for(const element of value["m_target"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["q_actual"])||value["q_actual"].length>6)bad();
n=addSize(n,4);
for(const element of value["q_actual"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["qd_actual"])||value["qd_actual"].length>6)bad();
n=addSize(n,4);
for(const element of value["qd_actual"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["i_actual"])||value["i_actual"].length>6)bad();
n=addSize(n,4);
for(const element of value["i_actual"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tool_acc_values"])||value["tool_acc_values"].length>6)bad();
n=addSize(n,4);
for(const element of value["tool_acc_values"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tcp_force"])||value["tcp_force"].length>6)bad();
n=addSize(n,4);
for(const element of value["tcp_force"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tool_vector"])||value["tool_vector"].length>6)bad();
n=addSize(n,4);
for(const element of value["tool_vector"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tcp_speed"])||value["tcp_speed"].length>6)bad();
n=addSize(n,4);
for(const element of value["tcp_speed"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["digital_input_bits"]!=="number"||!Number.isFinite(value["digital_input_bits"]))bad();n=addSize(n,8);
if(!Array.isArray(value["motor_temperatures"])||value["motor_temperatures"].length>6)bad();
n=addSize(n,4);
for(const element of value["motor_temperatures"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["controller_timer"]!=="number"||!Number.isFinite(value["controller_timer"]))bad();n=addSize(n,8);
if(typeof value["test_value"]!=="number"||!Number.isFinite(value["test_value"]))bad();n=addSize(n,8);
if(typeof value["robot_mode"]!=="number"||!Number.isFinite(value["robot_mode"]))bad();n=addSize(n,8);
if(!Array.isArray(value["joint_modes"])||value["joint_modes"].length>6)bad();
n=addSize(n,4);
for(const element of value["joint_modes"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>768)bad(90);return n;}
function Ros2Ur_msgsMsgRobotStateRTMsgWrite(value,w){
w.v.setFloat64(w.at,value["time"],true);w.at+=8;
w.v.setUint32(w.at,value["q_target"].length,true);w.at+=4;
for(const element of value["q_target"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["qd_target"].length,true);w.at+=4;
for(const element of value["qd_target"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["qdd_target"].length,true);w.at+=4;
for(const element of value["qdd_target"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["i_target"].length,true);w.at+=4;
for(const element of value["i_target"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["m_target"].length,true);w.at+=4;
for(const element of value["m_target"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["q_actual"].length,true);w.at+=4;
for(const element of value["q_actual"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["qd_actual"].length,true);w.at+=4;
for(const element of value["qd_actual"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["i_actual"].length,true);w.at+=4;
for(const element of value["i_actual"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["tool_acc_values"].length,true);w.at+=4;
for(const element of value["tool_acc_values"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["tcp_force"].length,true);w.at+=4;
for(const element of value["tcp_force"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["tool_vector"].length,true);w.at+=4;
for(const element of value["tool_vector"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["tcp_speed"].length,true);w.at+=4;
for(const element of value["tcp_speed"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["digital_input_bits"],true);w.at+=8;
w.v.setUint32(w.at,value["motor_temperatures"].length,true);w.at+=4;
for(const element of value["motor_temperatures"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["controller_timer"],true);w.at+=8;
w.v.setFloat64(w.at,value["test_value"],true);w.at+=8;
w.v.setFloat64(w.at,value["robot_mode"],true);w.at+=8;
w.v.setUint32(w.at,value["joint_modes"].length,true);w.at+=4;
for(const element of value["joint_modes"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Ur_msgsMsgRobotStateRTMsgRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["time"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["q_target"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["q_target"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["qd_target"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["qd_target"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["qdd_target"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["qdd_target"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["i_target"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["i_target"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["m_target"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["m_target"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["q_actual"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["q_actual"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["qd_actual"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["qd_actual"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["i_actual"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["i_actual"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["tool_acc_values"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tool_acc_values"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["tcp_force"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tcp_force"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["tool_vector"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tool_vector"].push(decoded);
}}
}
{
{const count=r.u32();if(count>6)bad();value["tcp_speed"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tcp_speed"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["digital_input_bits"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["motor_temperatures"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["motor_temperatures"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["controller_timer"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["test_value"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["robot_mode"]=decoded;
}
{
{const count=r.u32();if(count>6)bad();value["joint_modes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_modes"].push(decoded);
}}
}
return value;}
function Ros2Ur_msgsMsgRobotStateRTMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["time"];
out["time"]=field;
}
{const field=value["q_target"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q_target"]=values;
}
{const field=value["qd_target"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["qd_target"]=values;
}
{const field=value["qdd_target"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["qdd_target"]=values;
}
{const field=value["i_target"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["i_target"]=values;
}
{const field=value["m_target"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["m_target"]=values;
}
{const field=value["q_actual"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q_actual"]=values;
}
{const field=value["qd_actual"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["qd_actual"]=values;
}
{const field=value["i_actual"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["i_actual"]=values;
}
{const field=value["tool_acc_values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tool_acc_values"]=values;
}
{const field=value["tcp_force"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tcp_force"]=values;
}
{const field=value["tool_vector"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tool_vector"]=values;
}
{const field=value["tcp_speed"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tcp_speed"]=values;
}
{const field=value["digital_input_bits"];
out["digital_input_bits"]=field;
}
{const field=value["motor_temperatures"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["motor_temperatures"]=values;
}
{const field=value["controller_timer"];
out["controller_timer"]=field;
}
{const field=value["test_value"];
out["test_value"]=field;
}
{const field=value["robot_mode"];
out["robot_mode"]=field;
}
{const field=value["joint_modes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_modes"]=values;
}
return out;}
/** Complete ROS2 ur_msgs/msg/ToolDataMsg codec; maxima are not actual lengths. */
export const Ros2Ur_msgsMsgToolDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_1eec69666515ffbdca162a10967fce03ee4c25f0c27d56c5a812b87c9a229aa0",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({"ANALOG_INPUT_RANGE_CURRENT":0,"ANALOG_INPUT_RANGE_VOLTAGE":1,"TOOL_BOOTLOADER_MODE":249,"TOOL_RUNNING_MODE":253,"TOOL_IDLE_MODE":255}),
defaultValue(){return {"analog_input_range2":0,"analog_input_range3":0,"analog_input2":0,"analog_input3":0,"tool_voltage_48v":0,"tool_output_voltage":0,"tool_current":0,"tool_temperature":0,"tool_mode":0};},
validateValue(value){Ros2Ur_msgsMsgToolDataMsgSize(Ros2Ur_msgsMsgToolDataMsgCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsMsgToolDataMsgCapture(value),n=Ros2Ur_msgsMsgToolDataMsgSize(owned),w=new Writer(n);Ros2Ur_msgsMsgToolDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Ur_msgsMsgToolDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsMsgToolDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["analog_input_range2"],1,true);n=addSize(n,1);
integer(value["analog_input_range3"],1,true);n=addSize(n,1);
if(typeof value["analog_input2"]!=="number"||!Number.isFinite(value["analog_input2"]))bad();n=addSize(n,8);
if(typeof value["analog_input3"]!=="number"||!Number.isFinite(value["analog_input3"]))bad();n=addSize(n,8);
if(typeof value["tool_voltage_48v"]!=="number"||!Number.isFinite(value["tool_voltage_48v"])||!Number.isFinite(Math.fround(value["tool_voltage_48v"])))bad();n=addSize(n,4);
integer(value["tool_output_voltage"],1,false);n=addSize(n,1);
if(typeof value["tool_current"]!=="number"||!Number.isFinite(value["tool_current"])||!Number.isFinite(Math.fround(value["tool_current"])))bad();n=addSize(n,4);
if(typeof value["tool_temperature"]!=="number"||!Number.isFinite(value["tool_temperature"])||!Number.isFinite(Math.fround(value["tool_temperature"])))bad();n=addSize(n,4);
integer(value["tool_mode"],1,false);n=addSize(n,1);
if(n>32)bad(90);return n;}
function Ros2Ur_msgsMsgToolDataMsgWrite(value,w){
w.v.setInt8(w.at,value["analog_input_range2"],true);w.at+=1;
w.v.setInt8(w.at,value["analog_input_range3"],true);w.at+=1;
w.v.setFloat64(w.at,value["analog_input2"],true);w.at+=8;
w.v.setFloat64(w.at,value["analog_input3"],true);w.at+=8;
w.v.setFloat32(w.at,value["tool_voltage_48v"],true);w.at+=4;
w.v.setUint8(w.at,value["tool_output_voltage"],true);w.at+=1;
w.v.setFloat32(w.at,value["tool_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["tool_temperature"],true);w.at+=4;
w.v.setUint8(w.at,value["tool_mode"],true);w.at+=1;
}
function Ros2Ur_msgsMsgToolDataMsgRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_input_range2"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["analog_input_range3"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_input2"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["analog_input3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tool_voltage_48v"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["tool_output_voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tool_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tool_temperature"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["tool_mode"]=decoded;
}
return value;}
function Ros2Ur_msgsMsgToolDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["analog_input_range2"];
out["analog_input_range2"]=field;
}
{const field=value["analog_input_range3"];
out["analog_input_range3"]=field;
}
{const field=value["analog_input2"];
out["analog_input2"]=field;
}
{const field=value["analog_input3"];
out["analog_input3"]=field;
}
{const field=value["tool_voltage_48v"];
out["tool_voltage_48v"]=field;
}
{const field=value["tool_output_voltage"];
out["tool_output_voltage"]=field;
}
{const field=value["tool_current"];
out["tool_current"]=field;
}
{const field=value["tool_temperature"];
out["tool_temperature"]=field;
}
{const field=value["tool_mode"];
out["tool_mode"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c2d78972cb5b63138d1e780e8ff0c082dd77d5b76b09642d2c655e9f16780101",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestSize(Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCapture(value),n=Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestWrite(value,w){
}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestRead(r){const value={};
return value;}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bbe9984c34d69f93ad8dbfe6059c6a2e1bf43dbd27eabd97cf0a9507308d814a",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"major":0,"minor":0,"bugfix":0,"build":0};},
validateValue(value){Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseSize(Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCapture(value),n=Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["major"],4,false);n=addSize(n,4);
integer(value["minor"],4,false);n=addSize(n,4);
integer(value["bugfix"],4,false);n=addSize(n,4);
integer(value["build"],4,false);n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseWrite(value,w){
w.v.setUint32(w.at,value["major"],true);w.at+=4;
w.v.setUint32(w.at,value["minor"],true);w.at+=4;
w.v.setUint32(w.at,value["bugfix"],true);w.at+=4;
w.v.setUint32(w.at,value["build"],true);w.at+=4;
}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["major"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["minor"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["bugfix"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["build"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["major"];
out["major"]=field;
}
{const field=value["minor"];
out["minor"]=field;
}
{const field=value["bugfix"];
out["bugfix"]=field;
}
{const field=value["build"];
out["build"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetAnalogOutput_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_71c3eca5c6c90758f084e275fc495feb5d1c08a467c97d941fb5864c54309bea",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":Ros2Ur_msgsMsgAnalogCodec.defaultValue()};},
validateValue(value){Ros2Ur_msgsSrvSetAnalogOutput_RequestSize(Ros2Ur_msgsSrvSetAnalogOutput_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetAnalogOutput_RequestCapture(value),n=Ros2Ur_msgsSrvSetAnalogOutput_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetAnalogOutput_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetAnalogOutput_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetAnalogOutput_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_msgsMsgAnalogSize(value["data"]));
if(n>6)bad(90);return n;}
function Ros2Ur_msgsSrvSetAnalogOutput_RequestWrite(value,w){
Ros2Ur_msgsMsgAnalogWrite(value["data"],w);
}
function Ros2Ur_msgsSrvSetAnalogOutput_RequestRead(r){const value={};
{
const decoded=Ros2Ur_msgsMsgAnalogRead(r);
value["data"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetAnalogOutput_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=Ros2Ur_msgsMsgAnalogCapture(field);
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetAnalogOutput_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_71a5f3580657cb3ca702dbb63b7af3330c922624c9e9774e47e6533e2fc0a05a",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetAnalogOutput_ResponseSize(Ros2Ur_msgsSrvSetAnalogOutput_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetAnalogOutput_ResponseCapture(value),n=Ros2Ur_msgsSrvSetAnalogOutput_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetAnalogOutput_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetAnalogOutput_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetAnalogOutput_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetAnalogOutput_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetAnalogOutput_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetAnalogOutput_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetForceMode_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetForceMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e43a6c22510a2078de3cb2dc17ba1b7184080a6aa29a1653b7986723c0cfef6e",maxWireBytes:331,minWireBytes:203,cartesianMaxWireBytes:331n,aggregateWireBytes:0,constants:Object.freeze({"TCP_TO_ORIGIN":1,"NO_TRANSFORM":2,"TCP_VELOCITY_TO_X_Y":3}),
defaultValue(){return {"task_frame":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"selection_vector_x":false,"selection_vector_y":false,"selection_vector_z":false,"selection_vector_rx":false,"selection_vector_ry":false,"selection_vector_rz":false,"wrench":Ros2Geometry_msgsMsgWrenchCodec.defaultValue(),"type":2,"speed_limits":Ros2Geometry_msgsMsgTwistCodec.defaultValue(),"deviation_limits":[0.009999999776482582,0.009999999776482582,0.009999999776482582,0.009999999776482582,0.009999999776482582,0.009999999776482582],"damping_factor":0.02500000037252903,"gain_scaling":0.5};},
validateValue(value){Ros2Ur_msgsSrvSetForceMode_RequestSize(Ros2Ur_msgsSrvSetForceMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetForceMode_RequestCapture(value),n=Ros2Ur_msgsSrvSetForceMode_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetForceMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,331,90);if(owned.length<203)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetForceMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetForceMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["task_frame"]));
if(typeof value["selection_vector_x"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_y"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_z"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_rx"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_ry"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_rz"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Geometry_msgsMsgWrenchSize(value["wrench"]));
integer(value["type"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(value["speed_limits"]));
if(!Array.isArray(value["deviation_limits"])||value["deviation_limits"].length!==6)bad();
for(const element of value["deviation_limits"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["damping_factor"]!=="number"||!Number.isFinite(value["damping_factor"])||!Number.isFinite(Math.fround(value["damping_factor"])))bad();n=addSize(n,4);
if(typeof value["gain_scaling"]!=="number"||!Number.isFinite(value["gain_scaling"])||!Number.isFinite(Math.fround(value["gain_scaling"])))bad();n=addSize(n,4);
if(n>331)bad(90);return n;}
function Ros2Ur_msgsSrvSetForceMode_RequestWrite(value,w){
Ros2Geometry_msgsMsgPoseStampedWrite(value["task_frame"],w);
w.v.setUint8(w.at,value["selection_vector_x"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_y"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_z"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_rx"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_ry"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_rz"]?1:0,true);w.at+=1;
Ros2Geometry_msgsMsgWrenchWrite(value["wrench"],w);
w.v.setUint8(w.at,value["type"],true);w.at+=1;
Ros2Geometry_msgsMsgTwistWrite(value["speed_limits"],w);
for(const element of value["deviation_limits"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["damping_factor"],true);w.at+=4;
w.v.setFloat32(w.at,value["gain_scaling"],true);w.at+=4;
}
function Ros2Ur_msgsSrvSetForceMode_RequestRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["task_frame"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_x"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_y"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_z"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_rx"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_ry"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["selection_vector_rz"]=decoded!==0;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchRead(r);
value["wrench"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["type"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["speed_limits"]=decoded;
}
{
{const count=6;if(count>6)bad();value["deviation_limits"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["deviation_limits"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["damping_factor"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["gain_scaling"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetForceMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task_frame"];
out["task_frame"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["selection_vector_x"];
out["selection_vector_x"]=field;
}
{const field=value["selection_vector_y"];
out["selection_vector_y"]=field;
}
{const field=value["selection_vector_z"];
out["selection_vector_z"]=field;
}
{const field=value["selection_vector_rx"];
out["selection_vector_rx"]=field;
}
{const field=value["selection_vector_ry"];
out["selection_vector_ry"]=field;
}
{const field=value["selection_vector_rz"];
out["selection_vector_rz"]=field;
}
{const field=value["wrench"];
out["wrench"]=Ros2Geometry_msgsMsgWrenchCapture(field);
}
{const field=value["type"];
out["type"]=field;
}
{const field=value["speed_limits"];
out["speed_limits"]=Ros2Geometry_msgsMsgTwistCapture(field);
}
{const field=value["deviation_limits"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["deviation_limits"]=values;
}
{const field=value["damping_factor"];
out["damping_factor"]=field;
}
{const field=value["gain_scaling"];
out["gain_scaling"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetForceMode_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetForceMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3f5438d426b7b68557ac8f43dc8efc2e2106782ec0cd4fd0dee7cdd69ddc7de8",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetForceMode_ResponseSize(Ros2Ur_msgsSrvSetForceMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetForceMode_ResponseCapture(value),n=Ros2Ur_msgsSrvSetForceMode_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetForceMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetForceMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetForceMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetForceMode_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetForceMode_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetForceMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetFrictionModelParameters_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e44ad15f9c45d5a65eb13e6bc6e973bb20eebdafa04a5f2e8bcf4b0b54ce91c2",maxWireBytes:104,minWireBytes:8,cartesianMaxWireBytes:104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"parameters":Ros2Ur_msgsMsgFrictionModelParametersCodec.defaultValue()};},
validateValue(value){Ros2Ur_msgsSrvSetFrictionModelParameters_RequestSize(Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCapture(value),n=Ros2Ur_msgsSrvSetFrictionModelParameters_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetFrictionModelParameters_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,104,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetFrictionModelParameters_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetFrictionModelParameters_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Ur_msgsMsgFrictionModelParametersSize(value["parameters"]));
if(n>104)bad(90);return n;}
function Ros2Ur_msgsSrvSetFrictionModelParameters_RequestWrite(value,w){
Ros2Ur_msgsMsgFrictionModelParametersWrite(value["parameters"],w);
}
function Ros2Ur_msgsSrvSetFrictionModelParameters_RequestRead(r){const value={};
{
const decoded=Ros2Ur_msgsMsgFrictionModelParametersRead(r);
value["parameters"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["parameters"];
out["parameters"]=Ros2Ur_msgsMsgFrictionModelParametersCapture(field);
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetFrictionModelParameters_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1caca29dda3dc32360fd010dfb352b6feb54bba66ee16edd2f4353e3382207b3",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseSize(Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCapture(value),n=Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetGravity_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetGravity_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3d7fed6ae8accbddd5b09f93c3e49f2b9f99cae4cf80a951eb31ace30ec5a834",maxWireBytes:164,minWireBytes:36,cartesianMaxWireBytes:164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gravity":Ros2Geometry_msgsMsgVector3StampedCodec.defaultValue()};},
validateValue(value){Ros2Ur_msgsSrvSetGravity_RequestSize(Ros2Ur_msgsSrvSetGravity_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetGravity_RequestCapture(value),n=Ros2Ur_msgsSrvSetGravity_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetGravity_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,164,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetGravity_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetGravity_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3StampedSize(value["gravity"]));
if(n>164)bad(90);return n;}
function Ros2Ur_msgsSrvSetGravity_RequestWrite(value,w){
Ros2Geometry_msgsMsgVector3StampedWrite(value["gravity"],w);
}
function Ros2Ur_msgsSrvSetGravity_RequestRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3StampedRead(r);
value["gravity"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetGravity_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gravity"];
out["gravity"]=Ros2Geometry_msgsMsgVector3StampedCapture(field);
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetGravity_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetGravity_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_84694df7fbb4fd15bfecaf1aae683ef1795b0c4941d1987d951467448da5f793",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"status":""};},
validateValue(value){Ros2Ur_msgsSrvSetGravity_ResponseSize(Ros2Ur_msgsSrvSetGravity_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetGravity_ResponseCapture(value),n=Ros2Ur_msgsSrvSetGravity_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetGravity_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetGravity_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetGravity_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["status"],256).length);
if(n>261)bad(90);return n;}
function Ros2Ur_msgsSrvSetGravity_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["status"],256);
}
function Ros2Ur_msgsSrvSetGravity_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["status"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetGravity_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetIO_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c8ac4dddeb474521b4c701a0229e99af954416b7b8d7f32d2a19629d38b378c7",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"PIN_AOUT0":0,"PIN_AOUT1":1,"PIN_DOUT0":0,"PIN_DOUT1":1,"PIN_DOUT2":2,"PIN_DOUT3":3,"PIN_DOUT4":4,"PIN_DOUT5":5,"PIN_DOUT6":6,"PIN_DOUT7":7,"PIN_CONF_OUT0":8,"PIN_CONF_OUT1":9,"PIN_CONF_OUT2":10,"PIN_CONF_OUT3":11,"PIN_CONF_OUT4":12,"PIN_CONF_OUT5":13,"PIN_CONF_OUT6":14,"PIN_CONF_OUT7":15,"PIN_TOOL_DOUT0":16,"PIN_TOOL_DOUT1":17,"FUN_SET_DIGITAL_OUT":1,"FUN_SET_FLAG":2,"FUN_SET_ANALOG_OUT":3,"FUN_SET_TOOL_VOLTAGE":4,"STATE_OFF":0,"STATE_ON":1,"STATE_TOOL_VOLTAGE_0V":0,"STATE_TOOL_VOLTAGE_12V":12,"STATE_TOOL_VOLTAGE_24V":24}),
defaultValue(){return {"fun":0,"pin":0,"state":0};},
validateValue(value){Ros2Ur_msgsSrvSetIO_RequestSize(Ros2Ur_msgsSrvSetIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetIO_RequestCapture(value),n=Ros2Ur_msgsSrvSetIO_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["fun"],1,true);n=addSize(n,1);
integer(value["pin"],1,true);n=addSize(n,1);
if(typeof value["state"]!=="number"||!Number.isFinite(value["state"])||!Number.isFinite(Math.fround(value["state"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2Ur_msgsSrvSetIO_RequestWrite(value,w){
w.v.setInt8(w.at,value["fun"],true);w.at+=1;
w.v.setInt8(w.at,value["pin"],true);w.at+=1;
w.v.setFloat32(w.at,value["state"],true);w.at+=4;
}
function Ros2Ur_msgsSrvSetIO_RequestRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["fun"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["state"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["fun"];
out["fun"]=field;
}
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetIO_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bd32373034093ccdab140a5ba8ce8a420ef2ce441430e8306b2484f0ca535090",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetIO_ResponseSize(Ros2Ur_msgsSrvSetIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetIO_ResponseCapture(value),n=Ros2Ur_msgsSrvSetIO_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetIO_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetIO_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetPayload_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetPayload_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e76e256e1accf2d742c4cf529e5af1e830db75f3db703a1f368d28077a87e735",maxWireBytes:84,minWireBytes:84,cartesianMaxWireBytes:84n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"center_of_gravity":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"ixx":0,"iyy":0,"izz":0,"ixy":0,"ixz":0,"iyz":0,"transition_time":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Ur_msgsSrvSetPayload_RequestSize(Ros2Ur_msgsSrvSetPayload_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetPayload_RequestCapture(value),n=Ros2Ur_msgsSrvSetPayload_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetPayload_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,84,90);if(owned.length<84)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetPayload_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetPayload_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["center_of_gravity"]));
if(typeof value["ixx"]!=="number"||!Number.isFinite(value["ixx"]))bad();n=addSize(n,8);
if(typeof value["iyy"]!=="number"||!Number.isFinite(value["iyy"]))bad();n=addSize(n,8);
if(typeof value["izz"]!=="number"||!Number.isFinite(value["izz"]))bad();n=addSize(n,8);
if(typeof value["ixy"]!=="number"||!Number.isFinite(value["ixy"]))bad();n=addSize(n,8);
if(typeof value["ixz"]!=="number"||!Number.isFinite(value["ixz"]))bad();n=addSize(n,8);
if(typeof value["iyz"]!=="number"||!Number.isFinite(value["iyz"]))bad();n=addSize(n,8);
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["transition_time"]));
if(n>84)bad(90);return n;}
function Ros2Ur_msgsSrvSetPayload_RequestWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
Ros2Geometry_msgsMsgVector3Write(value["center_of_gravity"],w);
w.v.setFloat64(w.at,value["ixx"],true);w.at+=8;
w.v.setFloat64(w.at,value["iyy"],true);w.at+=8;
w.v.setFloat64(w.at,value["izz"],true);w.at+=8;
w.v.setFloat64(w.at,value["ixy"],true);w.at+=8;
w.v.setFloat64(w.at,value["ixz"],true);w.at+=8;
w.v.setFloat64(w.at,value["iyz"],true);w.at+=8;
Ros2Builtin_interfacesMsgDurationWrite(value["transition_time"],w);
}
function Ros2Ur_msgsSrvSetPayload_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["center_of_gravity"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ixx"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["iyy"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["izz"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ixy"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ixz"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["iyz"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["transition_time"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetPayload_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["center_of_gravity"];
out["center_of_gravity"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["ixx"];
out["ixx"]=field;
}
{const field=value["iyy"];
out["iyy"]=field;
}
{const field=value["izz"];
out["izz"]=field;
}
{const field=value["ixy"];
out["ixy"]=field;
}
{const field=value["ixz"];
out["ixz"]=field;
}
{const field=value["iyz"];
out["iyz"]=field;
}
{const field=value["transition_time"];
out["transition_time"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetPayload_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetPayload_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_036dbaa7d42b6cba6ad29a082e078401d507292483d260ba532f7d6947c1624c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetPayload_ResponseSize(Ros2Ur_msgsSrvSetPayload_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetPayload_ResponseCapture(value),n=Ros2Ur_msgsSrvSetPayload_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetPayload_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetPayload_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetPayload_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetPayload_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetPayload_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetPayload_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetSpeedSliderFraction_Request codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d8f19fb240e4b70cad74c5111cdaebad14b480f21da953c95d474064b091abd8",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_slider_fraction":0};},
validateValue(value){Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestSize(Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCapture(value),n=Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["speed_slider_fraction"]!=="number"||!Number.isFinite(value["speed_slider_fraction"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestWrite(value,w){
w.v.setFloat64(w.at,value["speed_slider_fraction"],true);w.at+=8;
}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["speed_slider_fraction"]=decoded;
}
return value;}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_slider_fraction"];
out["speed_slider_fraction"]=field;
}
return out;}
/** Complete ROS2 ur_msgs/srv/SetSpeedSliderFraction_Response codec; maxima are not actual lengths. */
export const Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1ab1ad5ab4b1ed302710ec055dff7644cc6e966af919ff311eaea6cddcf7dbb9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseSize(Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCapture(value),n=Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseSize(owned),w=new Writer(n);Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
