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
/** Complete ROS2 geometry_msgs/msg/Transform codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTransformCodec=Object.freeze({codecIdentity:"UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"translation":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"rotation":Ros2Geometry_msgsMsgQuaternionCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTransformSize(Ros2Geometry_msgsMsgTransformCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTransformCapture(value),n=Ros2Geometry_msgsMsgTransformSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTransformWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTransformRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTransformSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["translation"]));
n=addSize(n,Ros2Geometry_msgsMsgQuaternionSize(value["rotation"]));
if(n>56)bad(90);return n;}
function Ros2Geometry_msgsMsgTransformWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["translation"],w);
Ros2Geometry_msgsMsgQuaternionWrite(value["rotation"],w);
}
function Ros2Geometry_msgsMsgTransformRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["translation"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternionRead(r);
value["rotation"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTransformCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["translation"];
out["translation"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["rotation"];
out["rotation"]=Ros2Geometry_msgsMsgQuaternionCapture(field);
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
/** Complete ROS2 geometry_msgs/msg/TransformStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTransformStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_1cb6033612e0632fe87e6cf44bf5a899220370c372ba25b989a066171925ee7a",maxWireBytes:456,minWireBytes:72,cartesianMaxWireBytes:456n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"child_frame_id":"","transform":Ros2Geometry_msgsMsgTransformCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTransformStampedSize(Ros2Geometry_msgsMsgTransformStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTransformStampedCapture(value),n=Ros2Geometry_msgsMsgTransformStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTransformStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,456,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTransformStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTransformStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],256).length);
n=addSize(n,Ros2Geometry_msgsMsgTransformSize(value["transform"]));
if(n>456)bad(90);return n;}
function Ros2Geometry_msgsMsgTransformStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],256);
Ros2Geometry_msgsMsgTransformWrite(value["transform"],w);
}
function Ros2Geometry_msgsMsgTransformStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["child_frame_id"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTransformRead(r);
value["transform"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTransformStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["child_frame_id"];
out["child_frame_id"]=field;
}
{const field=value["transform"];
out["transform"]=Ros2Geometry_msgsMsgTransformCapture(field);
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
/** Complete ROS2 geometry_msgs/msg/TwistStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTwistStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_7448b885388ff3454de1a35b8c5c895ee58b791267b435df1e4cf7c1eee0f573",maxWireBytes:188,minWireBytes:60,cartesianMaxWireBytes:188n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"twist":Ros2Geometry_msgsMsgTwistCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTwistStampedSize(Ros2Geometry_msgsMsgTwistStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTwistStampedCapture(value),n=Ros2Geometry_msgsMsgTwistStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTwistStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,188,90);if(owned.length<60)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTwistStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTwistStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(value["twist"]));
if(n>188)bad(90);return n;}
function Ros2Geometry_msgsMsgTwistStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgTwistWrite(value["twist"],w);
}
function Ros2Geometry_msgsMsgTwistStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["twist"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTwistStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["twist"];
out["twist"]=Ros2Geometry_msgsMsgTwistCapture(field);
}
return out;}
/** Complete ROS2 geometry_msgs/msg/TwistWithCovariance codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTwistWithCovarianceCodec=Object.freeze({codecIdentity:"UWVMROS1_96c9fddad7b7c240e850d8c27ddb5f8343764cac791ef9e210196f7941aaf404",maxWireBytes:336,minWireBytes:336,cartesianMaxWireBytes:336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"twist":Ros2Geometry_msgsMsgTwistCodec.defaultValue(),"covariance":Array.from({length:36},()=>(0))};},
validateValue(value){Ros2Geometry_msgsMsgTwistWithCovarianceSize(Ros2Geometry_msgsMsgTwistWithCovarianceCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTwistWithCovarianceCapture(value),n=Ros2Geometry_msgsMsgTwistWithCovarianceSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTwistWithCovarianceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,336,90);if(owned.length<336)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTwistWithCovarianceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTwistWithCovarianceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(value["twist"]));
if(!Array.isArray(value["covariance"])||value["covariance"].length!==36)bad();
for(const element of value["covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>336)bad(90);return n;}
function Ros2Geometry_msgsMsgTwistWithCovarianceWrite(value,w){
Ros2Geometry_msgsMsgTwistWrite(value["twist"],w);
for(const element of value["covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Geometry_msgsMsgTwistWithCovarianceRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["twist"]=decoded;
}
{
{const count=36;if(count>36)bad();value["covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["covariance"].push(decoded);
}}
}
return value;}
function Ros2Geometry_msgsMsgTwistWithCovarianceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["twist"];
out["twist"]=Ros2Geometry_msgsMsgTwistCapture(field);
}
{const field=value["covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==36)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["covariance"]=values;
}
return out;}
/** Complete ROS2 nav_msgs/msg/Odometry codec; maxima are not actual lengths. */
export const Ros2Nav_msgsMsgOdometryCodec=Object.freeze({codecIdentity:"UWVMROS1_4eb9aa4edb3d288080deaae0326c26a75923e69d40c9b10219baf6d441ea8111",maxWireBytes:1080,minWireBytes:696,cartesianMaxWireBytes:1080n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"child_frame_id":"","pose":Ros2Geometry_msgsMsgPoseWithCovarianceCodec.defaultValue(),"twist":Ros2Geometry_msgsMsgTwistWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros2Nav_msgsMsgOdometrySize(Ros2Nav_msgsMsgOdometryCapture(value));return true;},
encode(value){const owned=Ros2Nav_msgsMsgOdometryCapture(value),n=Ros2Nav_msgsMsgOdometrySize(owned),w=new Writer(n);Ros2Nav_msgsMsgOdometryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1080,90);if(owned.length<696)bad();const r=new Reader(owned),value=Ros2Nav_msgsMsgOdometryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Nav_msgsMsgOdometrySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],256).length);
n=addSize(n,Ros2Geometry_msgsMsgPoseWithCovarianceSize(value["pose"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistWithCovarianceSize(value["twist"]));
if(n>1080)bad(90);return n;}
function Ros2Nav_msgsMsgOdometryWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],256);
Ros2Geometry_msgsMsgPoseWithCovarianceWrite(value["pose"],w);
Ros2Geometry_msgsMsgTwistWithCovarianceWrite(value["twist"],w);
}
function Ros2Nav_msgsMsgOdometryRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["child_frame_id"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovarianceRead(r);
value["pose"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistWithCovarianceRead(r);
value["twist"]=decoded;
}
return value;}
function Ros2Nav_msgsMsgOdometryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["child_frame_id"];
out["child_frame_id"]=field;
}
{const field=value["pose"];
out["pose"]=Ros2Geometry_msgsMsgPoseWithCovarianceCapture(field);
}
{const field=value["twist"];
out["twist"]=Ros2Geometry_msgsMsgTwistWithCovarianceCapture(field);
}
return out;}
/** Complete ROS2 sensor_msgs/msg/BatteryState codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgBatteryStateCodec=Object.freeze({codecIdentity:"UWVMROS1_9083c57dde00bba912db4dd736027cb92bbed2b1a79f965570dee86e1ff5a96e",maxWireBytes:828,minWireBytes:60,cartesianMaxWireBytes:828n,aggregateWireBytes:0,constants:Object.freeze({"POWER_SUPPLY_STATUS_UNKNOWN":0,"POWER_SUPPLY_STATUS_CHARGING":1,"POWER_SUPPLY_STATUS_DISCHARGING":2,"POWER_SUPPLY_STATUS_NOT_CHARGING":3,"POWER_SUPPLY_STATUS_FULL":4,"POWER_SUPPLY_HEALTH_UNKNOWN":0,"POWER_SUPPLY_HEALTH_GOOD":1,"POWER_SUPPLY_HEALTH_OVERHEAT":2,"POWER_SUPPLY_HEALTH_DEAD":3,"POWER_SUPPLY_HEALTH_OVERVOLTAGE":4,"POWER_SUPPLY_HEALTH_UNSPEC_FAILURE":5,"POWER_SUPPLY_HEALTH_COLD":6,"POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE":7,"POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE":8,"POWER_SUPPLY_TECHNOLOGY_UNKNOWN":0,"POWER_SUPPLY_TECHNOLOGY_NIMH":1,"POWER_SUPPLY_TECHNOLOGY_LION":2,"POWER_SUPPLY_TECHNOLOGY_LIPO":3,"POWER_SUPPLY_TECHNOLOGY_LIFE":4,"POWER_SUPPLY_TECHNOLOGY_NICD":5,"POWER_SUPPLY_TECHNOLOGY_LIMN":6,"POWER_SUPPLY_TECHNOLOGY_TERNARY":7,"POWER_SUPPLY_TECHNOLOGY_VRLA":8}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"voltage":0,"temperature":0,"current":0,"charge":0,"capacity":0,"design_capacity":0,"percentage":0,"power_supply_status":0,"power_supply_health":0,"power_supply_technology":0,"present":false,"cell_voltage":[],"cell_temperature":[],"location":"","serial_number":""};},
validateValue(value){Ros2Sensor_msgsMsgBatteryStateSize(Ros2Sensor_msgsMsgBatteryStateCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgBatteryStateCapture(value),n=Ros2Sensor_msgsMsgBatteryStateSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgBatteryStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,828,90);if(owned.length<60)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgBatteryStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgBatteryStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["voltage"]!=="number"||!Number.isFinite(value["voltage"])||!Number.isFinite(Math.fround(value["voltage"])))bad();n=addSize(n,4);
if(typeof value["temperature"]!=="number"||!Number.isFinite(value["temperature"])||!Number.isFinite(Math.fround(value["temperature"])))bad();n=addSize(n,4);
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["charge"]!=="number"||!Number.isFinite(value["charge"])||!Number.isFinite(Math.fround(value["charge"])))bad();n=addSize(n,4);
if(typeof value["capacity"]!=="number"||!Number.isFinite(value["capacity"])||!Number.isFinite(Math.fround(value["capacity"])))bad();n=addSize(n,4);
if(typeof value["design_capacity"]!=="number"||!Number.isFinite(value["design_capacity"])||!Number.isFinite(Math.fround(value["design_capacity"])))bad();n=addSize(n,4);
if(typeof value["percentage"]!=="number"||!Number.isFinite(value["percentage"])||!Number.isFinite(Math.fround(value["percentage"])))bad();n=addSize(n,4);
integer(value["power_supply_status"],1,false);n=addSize(n,1);
integer(value["power_supply_health"],1,false);n=addSize(n,1);
integer(value["power_supply_technology"],1,false);n=addSize(n,1);
if(typeof value["present"]!=="boolean")bad();n=addSize(n,1);
if(!Array.isArray(value["cell_voltage"])||value["cell_voltage"].length>16)bad();
n=addSize(n,4);
for(const element of value["cell_voltage"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["cell_temperature"])||value["cell_temperature"].length>16)bad();
n=addSize(n,4);
for(const element of value["cell_temperature"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
n=addSize(n,4+textBytes(value["location"],256).length);
n=addSize(n,4+textBytes(value["serial_number"],256).length);
if(n>828)bad(90);return n;}
function Ros2Sensor_msgsMsgBatteryStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["voltage"],true);w.at+=4;
w.v.setFloat32(w.at,value["temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat32(w.at,value["charge"],true);w.at+=4;
w.v.setFloat32(w.at,value["capacity"],true);w.at+=4;
w.v.setFloat32(w.at,value["design_capacity"],true);w.at+=4;
w.v.setFloat32(w.at,value["percentage"],true);w.at+=4;
w.v.setUint8(w.at,value["power_supply_status"],true);w.at+=1;
w.v.setUint8(w.at,value["power_supply_health"],true);w.at+=1;
w.v.setUint8(w.at,value["power_supply_technology"],true);w.at+=1;
w.v.setUint8(w.at,value["present"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["cell_voltage"].length,true);w.at+=4;
for(const element of value["cell_voltage"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["cell_temperature"].length,true);w.at+=4;
for(const element of value["cell_temperature"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.text(value["location"],256);
w.text(value["serial_number"],256);
}
function Ros2Sensor_msgsMsgBatteryStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["charge"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["capacity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["design_capacity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["percentage"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["power_supply_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["power_supply_health"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["power_supply_technology"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["present"]=decoded!==0;
}
{
{const count=r.u32();if(count>16)bad();value["cell_voltage"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cell_voltage"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["cell_temperature"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cell_temperature"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["location"]=decoded;
}
{
const decoded=r.text(256);
value["serial_number"]=decoded;
}
return value;}
function Ros2Sensor_msgsMsgBatteryStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["voltage"];
out["voltage"]=field;
}
{const field=value["temperature"];
out["temperature"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["charge"];
out["charge"]=field;
}
{const field=value["capacity"];
out["capacity"]=field;
}
{const field=value["design_capacity"];
out["design_capacity"]=field;
}
{const field=value["percentage"];
out["percentage"]=field;
}
{const field=value["power_supply_status"];
out["power_supply_status"]=field;
}
{const field=value["power_supply_health"];
out["power_supply_health"]=field;
}
{const field=value["power_supply_technology"];
out["power_supply_technology"]=field;
}
{const field=value["present"];
out["present"]=field;
}
{const field=value["cell_voltage"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cell_voltage"]=values;
}
{const field=value["cell_temperature"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cell_temperature"]=values;
}
{const field=value["location"];
out["location"]=field;
}
{const field=value["serial_number"];
out["serial_number"]=field;
}
return out;}
/** Complete ROS2 sensor_msgs/msg/Imu codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgImuCodec=Object.freeze({codecIdentity:"UWVMROS1_e38d1c936a2a4a870b2f069af8d1796f69e6ce7b8cae0a792925ed2f6c0cae1b",maxWireBytes:436,minWireBytes:308,cartesianMaxWireBytes:436n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"orientation":Ros2Geometry_msgsMsgQuaternionCodec.defaultValue(),"orientation_covariance":Array.from({length:9},()=>(0)),"angular_velocity":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"angular_velocity_covariance":Array.from({length:9},()=>(0)),"linear_acceleration":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"linear_acceleration_covariance":Array.from({length:9},()=>(0))};},
validateValue(value){Ros2Sensor_msgsMsgImuSize(Ros2Sensor_msgsMsgImuCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgImuCapture(value),n=Ros2Sensor_msgsMsgImuSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgImuWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,436,90);if(owned.length<308)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgImuRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgImuSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgQuaternionSize(value["orientation"]));
if(!Array.isArray(value["orientation_covariance"])||value["orientation_covariance"].length!==9)bad();
for(const element of value["orientation_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["angular_velocity"]));
if(!Array.isArray(value["angular_velocity_covariance"])||value["angular_velocity_covariance"].length!==9)bad();
for(const element of value["angular_velocity_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["linear_acceleration"]));
if(!Array.isArray(value["linear_acceleration_covariance"])||value["linear_acceleration_covariance"].length!==9)bad();
for(const element of value["linear_acceleration_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>436)bad(90);return n;}
function Ros2Sensor_msgsMsgImuWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgQuaternionWrite(value["orientation"],w);
for(const element of value["orientation_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros2Geometry_msgsMsgVector3Write(value["angular_velocity"],w);
for(const element of value["angular_velocity_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros2Geometry_msgsMsgVector3Write(value["linear_acceleration"],w);
for(const element of value["linear_acceleration_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Sensor_msgsMsgImuRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternionRead(r);
value["orientation"]=decoded;
}
{
{const count=9;if(count>9)bad();value["orientation_covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["orientation_covariance"].push(decoded);
}}
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["angular_velocity"]=decoded;
}
{
{const count=9;if(count>9)bad();value["angular_velocity_covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["angular_velocity_covariance"].push(decoded);
}}
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["linear_acceleration"]=decoded;
}
{
{const count=9;if(count>9)bad();value["linear_acceleration_covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["linear_acceleration_covariance"].push(decoded);
}}
}
return value;}
function Ros2Sensor_msgsMsgImuCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["orientation"];
out["orientation"]=Ros2Geometry_msgsMsgQuaternionCapture(field);
}
{const field=value["orientation_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["orientation_covariance"]=values;
}
{const field=value["angular_velocity"];
out["angular_velocity"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["angular_velocity_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["angular_velocity_covariance"]=values;
}
{const field=value["linear_acceleration"];
out["linear_acceleration"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["linear_acceleration_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["linear_acceleration_covariance"]=values;
}
return out;}
/** Complete ROS2 sensor_msgs/msg/JointState codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_5c4d1ef0128c748e20f6b30c9c3bb5ef6e3c529a0b05b81764ac4774ce765bcf",maxWireBytes:468,minWireBytes:28,cartesianMaxWireBytes:468n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"name":[],"position":[],"velocity":[],"effort":[]};},
validateValue(value){Ros2Sensor_msgsMsgJointStateSize(Ros2Sensor_msgsMsgJointStateCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgJointStateCapture(value),n=Ros2Sensor_msgsMsgJointStateSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,468,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["name"])||value["name"].length>2)bad();
n=addSize(n,4);
for(const element of value["name"]){
n=addSize(n,4+textBytes(element,128).length);
}
if(!Array.isArray(value["position"])||value["position"].length>2)bad();
n=addSize(n,4);
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocity"])||value["velocity"].length>2)bad();
n=addSize(n,4);
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>2)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
if(n>468)bad(90);return n;}
function Ros2Sensor_msgsMsgJointStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["name"].length,true);w.at+=4;
for(const element of value["name"]){
w.text(element,128);
}
w.v.setUint32(w.at,value["position"].length,true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["velocity"].length,true);w.at+=4;
for(const element of value["velocity"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["effort"].length,true);w.at+=4;
for(const element of value["effort"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Sensor_msgsMsgJointStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["name"]=[];for(let i=0;i<count;++i){
const decoded=r.text(128);
value["name"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["effort"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["effort"].push(decoded);
}}
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
return value;}
function Ros2Sensor_msgsMsgJointStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["name"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["name"]=values;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
return out;}
/** Complete ROS2 sensor_msgs/msg/LaserScan codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgLaserScanCodec=Object.freeze({codecIdentity:"UWVMROS1_89faa8e14fc7f79a62a576d35c0bb377caf0cbaef6a6f181a5af2da0ead6cce4",maxWireBytes:8368,minWireBytes:48,cartesianMaxWireBytes:8368n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"angle_min":0,"angle_max":0,"angle_increment":0,"time_increment":0,"scan_time":0,"range_min":0,"range_max":0,"ranges":[],"intensities":[]};},
validateValue(value){Ros2Sensor_msgsMsgLaserScanSize(Ros2Sensor_msgsMsgLaserScanCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgLaserScanCapture(value),n=Ros2Sensor_msgsMsgLaserScanSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgLaserScanWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8368,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgLaserScanRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgLaserScanSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["angle_min"]!=="number"||!Number.isFinite(value["angle_min"])||!Number.isFinite(Math.fround(value["angle_min"])))bad();n=addSize(n,4);
if(typeof value["angle_max"]!=="number"||!Number.isFinite(value["angle_max"])||!Number.isFinite(Math.fround(value["angle_max"])))bad();n=addSize(n,4);
if(typeof value["angle_increment"]!=="number"||!Number.isFinite(value["angle_increment"])||!Number.isFinite(Math.fround(value["angle_increment"])))bad();n=addSize(n,4);
if(typeof value["time_increment"]!=="number"||!Number.isFinite(value["time_increment"])||!Number.isFinite(Math.fround(value["time_increment"])))bad();n=addSize(n,4);
if(typeof value["scan_time"]!=="number"||!Number.isFinite(value["scan_time"])||!Number.isFinite(Math.fround(value["scan_time"])))bad();n=addSize(n,4);
if(typeof value["range_min"]!=="number"||!Number.isFinite(value["range_min"])||!Number.isFinite(Math.fround(value["range_min"])))bad();n=addSize(n,4);
if(typeof value["range_max"]!=="number"||!Number.isFinite(value["range_max"])||!Number.isFinite(Math.fround(value["range_max"])))bad();n=addSize(n,4);
if(!Array.isArray(value["ranges"])||value["ranges"].length>1024)bad();
n=addSize(n,4);
for(const element of value["ranges"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["intensities"])||value["intensities"].length>1024)bad();
n=addSize(n,4);
for(const element of value["intensities"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>8368)bad(90);return n;}
function Ros2Sensor_msgsMsgLaserScanWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["angle_min"],true);w.at+=4;
w.v.setFloat32(w.at,value["angle_max"],true);w.at+=4;
w.v.setFloat32(w.at,value["angle_increment"],true);w.at+=4;
w.v.setFloat32(w.at,value["time_increment"],true);w.at+=4;
w.v.setFloat32(w.at,value["scan_time"],true);w.at+=4;
w.v.setFloat32(w.at,value["range_min"],true);w.at+=4;
w.v.setFloat32(w.at,value["range_max"],true);w.at+=4;
w.v.setUint32(w.at,value["ranges"].length,true);w.at+=4;
for(const element of value["ranges"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["intensities"].length,true);w.at+=4;
for(const element of value["intensities"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Sensor_msgsMsgLaserScanRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["angle_min"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["angle_max"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["angle_increment"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["time_increment"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["scan_time"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["range_min"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["range_max"]=decoded;
}
{
{const count=r.u32();if(count>1024)bad();value["ranges"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ranges"].push(decoded);
}}
}
{
{const count=r.u32();if(count>1024)bad();value["intensities"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["intensities"].push(decoded);
}}
}
return value;}
function Ros2Sensor_msgsMsgLaserScanCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["angle_min"];
out["angle_min"]=field;
}
{const field=value["angle_max"];
out["angle_max"]=field;
}
{const field=value["angle_increment"];
out["angle_increment"]=field;
}
{const field=value["time_increment"];
out["time_increment"]=field;
}
{const field=value["scan_time"];
out["scan_time"]=field;
}
{const field=value["range_min"];
out["range_min"]=field;
}
{const field=value["range_max"];
out["range_max"]=field;
}
{const field=value["ranges"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["ranges"]=values;
}
{const field=value["intensities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["intensities"]=values;
}
return out;}
/** Complete ROS2 sensor_msgs/msg/MagneticField codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgMagneticFieldCodec=Object.freeze({codecIdentity:"UWVMROS1_56ae1437e0fbe7a6722ecf4795483d15f4c616e2e891372c30fa78f18be2cf8b",maxWireBytes:236,minWireBytes:108,cartesianMaxWireBytes:236n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"magnetic_field":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"magnetic_field_covariance":Array.from({length:9},()=>(0))};},
validateValue(value){Ros2Sensor_msgsMsgMagneticFieldSize(Ros2Sensor_msgsMsgMagneticFieldCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgMagneticFieldCapture(value),n=Ros2Sensor_msgsMsgMagneticFieldSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgMagneticFieldWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,236,90);if(owned.length<108)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgMagneticFieldRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgMagneticFieldSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["magnetic_field"]));
if(!Array.isArray(value["magnetic_field_covariance"])||value["magnetic_field_covariance"].length!==9)bad();
for(const element of value["magnetic_field_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>236)bad(90);return n;}
function Ros2Sensor_msgsMsgMagneticFieldWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgVector3Write(value["magnetic_field"],w);
for(const element of value["magnetic_field_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Sensor_msgsMsgMagneticFieldRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["magnetic_field"]=decoded;
}
{
{const count=9;if(count>9)bad();value["magnetic_field_covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["magnetic_field_covariance"].push(decoded);
}}
}
return value;}
function Ros2Sensor_msgsMsgMagneticFieldCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["magnetic_field"];
out["magnetic_field"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["magnetic_field_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["magnetic_field_covariance"]=values;
}
return out;}
/** Complete ROS2 std_srvs/srv/SetBool_Request codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvSetBool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2523e7915bb78fe01b68f2a856611b32a88f624a6e197a6ff312b613e8c65910",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":false};},
validateValue(value){Ros2Std_srvsSrvSetBool_RequestSize(Ros2Std_srvsSrvSetBool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvSetBool_RequestCapture(value),n=Ros2Std_srvsSrvSetBool_RequestSize(owned),w=new Writer(n);Ros2Std_srvsSrvSetBool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvSetBool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvSetBool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["data"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Std_srvsSrvSetBool_RequestWrite(value,w){
w.v.setUint8(w.at,value["data"]?1:0,true);w.at+=1;
}
function Ros2Std_srvsSrvSetBool_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["data"]=decoded!==0;
}
return value;}
function Ros2Std_srvsSrvSetBool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS2 std_srvs/srv/SetBool_Response codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvSetBool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f2d46a8440538094221a151716d8ec44db02455f59845c50aaf4e7964c27ebe4",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Std_srvsSrvSetBool_ResponseSize(Ros2Std_srvsSrvSetBool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvSetBool_ResponseCapture(value),n=Ros2Std_srvsSrvSetBool_ResponseSize(owned),w=new Writer(n);Ros2Std_srvsSrvSetBool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvSetBool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvSetBool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Std_srvsSrvSetBool_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Std_srvsSrvSetBool_ResponseRead(r){const value={};
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
function Ros2Std_srvsSrvSetBool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 std_srvs/srv/Trigger_Request codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvTrigger_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Std_srvsSrvTrigger_RequestSize(Ros2Std_srvsSrvTrigger_RequestCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvTrigger_RequestCapture(value),n=Ros2Std_srvsSrvTrigger_RequestSize(owned),w=new Writer(n);Ros2Std_srvsSrvTrigger_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvTrigger_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvTrigger_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Std_srvsSrvTrigger_RequestWrite(value,w){
}
function Ros2Std_srvsSrvTrigger_RequestRead(r){const value={};
return value;}
function Ros2Std_srvsSrvTrigger_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 std_srvs/srv/Trigger_Response codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvTrigger_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_58806c592727ad1011b32edb6bca3ca32d8e5dbe7f2fa20c86220daf64a10392",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Std_srvsSrvTrigger_ResponseSize(Ros2Std_srvsSrvTrigger_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvTrigger_ResponseCapture(value),n=Ros2Std_srvsSrvTrigger_ResponseSize(owned),w=new Writer(n);Ros2Std_srvsSrvTrigger_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvTrigger_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvTrigger_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Std_srvsSrvTrigger_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Std_srvsSrvTrigger_ResponseRead(r){const value={};
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
function Ros2Std_srvsSrvTrigger_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Goal codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsActionPatrol_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_929abdbc422698464043e7e8d12c2a5edd418dcd52bbde4a542b33e6862ba96f",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"goal":Ros2Geometry_msgsMsgVector3Codec.defaultValue()};},
validateValue(value){Ros2Turtlebot3_msgsActionPatrol_GoalSize(Ros2Turtlebot3_msgsActionPatrol_GoalCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsActionPatrol_GoalCapture(value),n=Ros2Turtlebot3_msgsActionPatrol_GoalSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsActionPatrol_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsActionPatrol_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsActionPatrol_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["goal"]));
if(n>24)bad(90);return n;}
function Ros2Turtlebot3_msgsActionPatrol_GoalWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["goal"],w);
}
function Ros2Turtlebot3_msgsActionPatrol_GoalRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["goal"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsActionPatrol_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["goal"];
out["goal"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
return out;}
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Result codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsActionPatrol_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_1d1fa277be54e1df3bd314a632ff80d79f22a306fc232219a7c23cce7a7703a3",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":""};},
validateValue(value){Ros2Turtlebot3_msgsActionPatrol_ResultSize(Ros2Turtlebot3_msgsActionPatrol_ResultCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsActionPatrol_ResultCapture(value),n=Ros2Turtlebot3_msgsActionPatrol_ResultSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsActionPatrol_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsActionPatrol_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsActionPatrol_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["result"],256).length);
if(n>260)bad(90);return n;}
function Ros2Turtlebot3_msgsActionPatrol_ResultWrite(value,w){
w.text(value["result"],256);
}
function Ros2Turtlebot3_msgsActionPatrol_ResultRead(r){const value={};
{
const decoded=r.text(256);
value["result"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsActionPatrol_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Feedback codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsActionPatrol_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_7c2f3824653a3048e1b93fa95584543a01efae8bb67253d3371a2988407a601e",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":""};},
validateValue(value){Ros2Turtlebot3_msgsActionPatrol_FeedbackSize(Ros2Turtlebot3_msgsActionPatrol_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsActionPatrol_FeedbackCapture(value),n=Ros2Turtlebot3_msgsActionPatrol_FeedbackSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsActionPatrol_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsActionPatrol_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsActionPatrol_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>260)bad(90);return n;}
function Ros2Turtlebot3_msgsActionPatrol_FeedbackWrite(value,w){
w.text(value["state"],256);
}
function Ros2Turtlebot3_msgsActionPatrol_FeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsActionPatrol_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/msg/SensorState codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsMsgSensorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_7974ef7ca70ababf0e3f7a5e4099335a1398933833260798482f155291a62358",maxWireBytes:168,minWireBytes:40,cartesianMaxWireBytes:168n,aggregateWireBytes:0,constants:Object.freeze({"BUMPER_FORWARD":1,"BUMPER_BACKWARD":2,"CLIFF":1,"SONAR":1,"ILLUMINATION":1,"BUTTON0":1,"BUTTON1":2,"ERROR_LEFT_MOTOR":1,"ERROR_RIGHT_MOTOR":2,"TORQUE_ON":1,"TORQUE_OFF":2}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"bumper":0,"cliff":0,"sonar":0,"illumination":0,"led":0,"button":0,"torque":false,"left_encoder":0,"right_encoder":0,"battery":0};},
validateValue(value){Ros2Turtlebot3_msgsMsgSensorStateSize(Ros2Turtlebot3_msgsMsgSensorStateCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsMsgSensorStateCapture(value),n=Ros2Turtlebot3_msgsMsgSensorStateSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsMsgSensorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,168,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsMsgSensorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsMsgSensorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["bumper"],1,false);n=addSize(n,1);
if(typeof value["cliff"]!=="number"||!Number.isFinite(value["cliff"])||!Number.isFinite(Math.fround(value["cliff"])))bad();n=addSize(n,4);
if(typeof value["sonar"]!=="number"||!Number.isFinite(value["sonar"])||!Number.isFinite(Math.fround(value["sonar"])))bad();n=addSize(n,4);
if(typeof value["illumination"]!=="number"||!Number.isFinite(value["illumination"])||!Number.isFinite(Math.fround(value["illumination"])))bad();n=addSize(n,4);
integer(value["led"],1,false);n=addSize(n,1);
integer(value["button"],1,false);n=addSize(n,1);
if(typeof value["torque"]!=="boolean")bad();n=addSize(n,1);
integer(value["left_encoder"],4,true);n=addSize(n,4);
integer(value["right_encoder"],4,true);n=addSize(n,4);
if(typeof value["battery"]!=="number"||!Number.isFinite(value["battery"])||!Number.isFinite(Math.fround(value["battery"])))bad();n=addSize(n,4);
if(n>168)bad(90);return n;}
function Ros2Turtlebot3_msgsMsgSensorStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["bumper"],true);w.at+=1;
w.v.setFloat32(w.at,value["cliff"],true);w.at+=4;
w.v.setFloat32(w.at,value["sonar"],true);w.at+=4;
w.v.setFloat32(w.at,value["illumination"],true);w.at+=4;
w.v.setUint8(w.at,value["led"],true);w.at+=1;
w.v.setUint8(w.at,value["button"],true);w.at+=1;
w.v.setUint8(w.at,value["torque"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["left_encoder"],true);w.at+=4;
w.v.setInt32(w.at,value["right_encoder"],true);w.at+=4;
w.v.setFloat32(w.at,value["battery"],true);w.at+=4;
}
function Ros2Turtlebot3_msgsMsgSensorStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["bumper"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cliff"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["sonar"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["illumination"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["led"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["button"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["torque"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["left_encoder"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["right_encoder"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["battery"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsMsgSensorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["bumper"];
out["bumper"]=field;
}
{const field=value["cliff"];
out["cliff"]=field;
}
{const field=value["sonar"];
out["sonar"]=field;
}
{const field=value["illumination"];
out["illumination"]=field;
}
{const field=value["led"];
out["led"]=field;
}
{const field=value["button"];
out["button"]=field;
}
{const field=value["torque"];
out["torque"]=field;
}
{const field=value["left_encoder"];
out["left_encoder"]=field;
}
{const field=value["right_encoder"];
out["right_encoder"]=field;
}
{const field=value["battery"];
out["battery"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/msg/Sound codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsMsgSoundCodec=Object.freeze({codecIdentity:"UWVMROS1_e8bd9a5fdf84ad1df0f264bd13a0e8966918de19ab62e91c7524dd8fc17a4e4b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"OFF":0,"ON":1,"LOW_BATTERY":2,"ERROR":3,"BUTTON1":4,"BUTTON2":5}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Turtlebot3_msgsMsgSoundSize(Ros2Turtlebot3_msgsMsgSoundCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsMsgSoundCapture(value),n=Ros2Turtlebot3_msgsMsgSoundSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsMsgSoundWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsMsgSoundRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsMsgSoundSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Turtlebot3_msgsMsgSoundWrite(value,w){
w.v.setUint8(w.at,value["value"],true);w.at+=1;
}
function Ros2Turtlebot3_msgsMsgSoundRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsMsgSoundCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/msg/VersionInfo codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsMsgVersionInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_7668c77b01a05eeb8ba3c5f259f3f3a9dfad3d4fdf8aa566d65f6e8f1c0f2d6a",maxWireBytes:780,minWireBytes:12,cartesianMaxWireBytes:780n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"hardware":"","firmware":"","software":""};},
validateValue(value){Ros2Turtlebot3_msgsMsgVersionInfoSize(Ros2Turtlebot3_msgsMsgVersionInfoCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsMsgVersionInfoCapture(value),n=Ros2Turtlebot3_msgsMsgVersionInfoSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsMsgVersionInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,780,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsMsgVersionInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsMsgVersionInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["hardware"],256).length);
n=addSize(n,4+textBytes(value["firmware"],256).length);
n=addSize(n,4+textBytes(value["software"],256).length);
if(n>780)bad(90);return n;}
function Ros2Turtlebot3_msgsMsgVersionInfoWrite(value,w){
w.text(value["hardware"],256);
w.text(value["firmware"],256);
w.text(value["software"],256);
}
function Ros2Turtlebot3_msgsMsgVersionInfoRead(r){const value={};
{
const decoded=r.text(256);
value["hardware"]=decoded;
}
{
const decoded=r.text(256);
value["firmware"]=decoded;
}
{
const decoded=r.text(256);
value["software"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsMsgVersionInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["hardware"];
out["hardware"]=field;
}
{const field=value["firmware"];
out["firmware"]=field;
}
{const field=value["software"];
out["software"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Dqn_Request codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvDqn_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_14042e65db9f4feef7a43bfb44255795470bb0ed91a3cf301ebec98c2d4e2d0e",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"action":0,"init":false};},
validateValue(value){Ros2Turtlebot3_msgsSrvDqn_RequestSize(Ros2Turtlebot3_msgsSrvDqn_RequestCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvDqn_RequestCapture(value),n=Ros2Turtlebot3_msgsSrvDqn_RequestSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvDqn_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvDqn_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvDqn_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["action"],1,false);n=addSize(n,1);
if(typeof value["init"]!=="boolean")bad();n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvDqn_RequestWrite(value,w){
w.v.setUint8(w.at,value["action"],true);w.at+=1;
w.v.setUint8(w.at,value["init"]?1:0,true);w.at+=1;
}
function Ros2Turtlebot3_msgsSrvDqn_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["action"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["init"]=decoded!==0;
}
return value;}
function Ros2Turtlebot3_msgsSrvDqn_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["action"];
out["action"]=field;
}
{const field=value["init"];
out["init"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Dqn_Response codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvDqn_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5d256e9c87d7959917f07e7cfcc0049fc18f79f547d83b72bd388a63c1417c6e",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":[],"reward":0,"done":false};},
validateValue(value){Ros2Turtlebot3_msgsSrvDqn_ResponseSize(Ros2Turtlebot3_msgsSrvDqn_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvDqn_ResponseCapture(value),n=Ros2Turtlebot3_msgsSrvDqn_ResponseSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvDqn_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvDqn_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvDqn_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["state"])||value["state"].length>128)bad();
n=addSize(n,4);
for(const element of value["state"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["reward"]!=="number"||!Number.isFinite(value["reward"])||!Number.isFinite(Math.fround(value["reward"])))bad();n=addSize(n,4);
if(typeof value["done"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvDqn_ResponseWrite(value,w){
w.v.setUint32(w.at,value["state"].length,true);w.at+=4;
for(const element of value["state"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["reward"],true);w.at+=4;
w.v.setUint8(w.at,value["done"]?1:0,true);w.at+=1;
}
function Ros2Turtlebot3_msgsSrvDqn_ResponseRead(r){const value={};
{
{const count=r.u32();if(count>128)bad();value["state"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["state"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["reward"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["done"]=decoded!==0;
}
return value;}
function Ros2Turtlebot3_msgsSrvDqn_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>128)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["state"]=values;
}
{const field=value["reward"];
out["reward"]=field;
}
{const field=value["done"];
out["done"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Goal_Request codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvGoal_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f76094f3cf00772bb80b59e846f1160ae6255fc0d931bbcba362104081698ded",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Turtlebot3_msgsSrvGoal_RequestSize(Ros2Turtlebot3_msgsSrvGoal_RequestCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvGoal_RequestCapture(value),n=Ros2Turtlebot3_msgsSrvGoal_RequestSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvGoal_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvGoal_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvGoal_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvGoal_RequestWrite(value,w){
}
function Ros2Turtlebot3_msgsSrvGoal_RequestRead(r){const value={};
return value;}
function Ros2Turtlebot3_msgsSrvGoal_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Goal_Response codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvGoal_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8b9022a7fb8c68a0b063b080cd05c230b653169badbe205bcd8051b8020f7990",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose_x":0,"pose_y":0,"success":false};},
validateValue(value){Ros2Turtlebot3_msgsSrvGoal_ResponseSize(Ros2Turtlebot3_msgsSrvGoal_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvGoal_ResponseCapture(value),n=Ros2Turtlebot3_msgsSrvGoal_ResponseSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvGoal_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvGoal_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvGoal_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["pose_x"]!=="number"||!Number.isFinite(value["pose_x"])||!Number.isFinite(Math.fround(value["pose_x"])))bad();n=addSize(n,4);
if(typeof value["pose_y"]!=="number"||!Number.isFinite(value["pose_y"])||!Number.isFinite(Math.fround(value["pose_y"])))bad();n=addSize(n,4);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>9)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvGoal_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["pose_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["pose_y"],true);w.at+=4;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Turtlebot3_msgsSrvGoal_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pose_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pose_y"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Turtlebot3_msgsSrvGoal_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose_x"];
out["pose_x"]=field;
}
{const field=value["pose_y"];
out["pose_y"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Sound_Request codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvSound_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0dcba95f5ba646d9a24b0054ba603daf1d1e5e4e9a6ffe7387cd61bff2e4151a",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Turtlebot3_msgsSrvSound_RequestSize(Ros2Turtlebot3_msgsSrvSound_RequestCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvSound_RequestCapture(value),n=Ros2Turtlebot3_msgsSrvSound_RequestSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvSound_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvSound_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvSound_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvSound_RequestWrite(value,w){
w.v.setUint8(w.at,value["value"],true);w.at+=1;
}
function Ros2Turtlebot3_msgsSrvSound_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros2Turtlebot3_msgsSrvSound_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 turtlebot3_msgs/srv/Sound_Response codec; maxima are not actual lengths. */
export const Ros2Turtlebot3_msgsSrvSound_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bc5a5fd3e2bf733a278b8a1315c8905b27fff49b4844df238d5574149068998e",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Turtlebot3_msgsSrvSound_ResponseSize(Ros2Turtlebot3_msgsSrvSound_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Turtlebot3_msgsSrvSound_ResponseCapture(value),n=Ros2Turtlebot3_msgsSrvSound_ResponseSize(owned),w=new Writer(n);Ros2Turtlebot3_msgsSrvSound_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Turtlebot3_msgsSrvSound_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Turtlebot3_msgsSrvSound_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Turtlebot3_msgsSrvSound_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Turtlebot3_msgsSrvSound_ResponseRead(r){const value={};
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
function Ros2Turtlebot3_msgsSrvSound_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
