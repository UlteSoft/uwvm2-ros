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
/** Complete ROS2 nav_msgs/msg/Odometry codec; maxima are not actual lengths. */
export const Ros2Nav_msgsMsgOdometryCodec=Object.freeze({codecIdentity:"UWVMROS1_fdfdd74c211c22bfd167c3e33a098fe21198cb095bbd0313fc765f38b1f334ec",maxWireBytes:952,minWireBytes:696,cartesianMaxWireBytes:952n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"child_frame_id":"","pose":Ros2Geometry_msgsMsgPoseWithCovarianceCodec.defaultValue(),"twist":Ros2Geometry_msgsMsgTwistWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros2Nav_msgsMsgOdometrySize(Ros2Nav_msgsMsgOdometryCapture(value));return true;},
encode(value){const owned=Ros2Nav_msgsMsgOdometryCapture(value),n=Ros2Nav_msgsMsgOdometrySize(owned),w=new Writer(n);Ros2Nav_msgsMsgOdometryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,952,90);if(owned.length<696)bad();const r=new Reader(owned),value=Ros2Nav_msgsMsgOdometryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Nav_msgsMsgOdometrySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],128).length);
n=addSize(n,Ros2Geometry_msgsMsgPoseWithCovarianceSize(value["pose"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistWithCovarianceSize(value["twist"]));
if(n>952)bad(90);return n;}
function Ros2Nav_msgsMsgOdometryWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],128);
Ros2Geometry_msgsMsgPoseWithCovarianceWrite(value["pose"],w);
Ros2Geometry_msgsMsgTwistWithCovarianceWrite(value["twist"],w);
}
function Ros2Nav_msgsMsgOdometryRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(128);
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
/** Complete ROS2 scout_msgs/msg/ScoutActuatorState codec; maxima are not actual lengths. */
export const Ros2Scout_msgsMsgScoutActuatorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_e7955c07aedaf42f8d97f873d60ae5c0dfc562faa1d64848f11b333f797350c6",maxWireBytes:19,minWireBytes:19,cartesianMaxWireBytes:19n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"rpm":0,"current":0,"driver_voltage":0,"driver_temperature":0,"motor_temperature":0};},
validateValue(value){Ros2Scout_msgsMsgScoutActuatorStateSize(Ros2Scout_msgsMsgScoutActuatorStateCapture(value));return true;},
encode(value){const owned=Ros2Scout_msgsMsgScoutActuatorStateCapture(value),n=Ros2Scout_msgsMsgScoutActuatorStateSize(owned),w=new Writer(n);Ros2Scout_msgsMsgScoutActuatorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,19,90);if(owned.length<19)bad();const r=new Reader(owned),value=Ros2Scout_msgsMsgScoutActuatorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Scout_msgsMsgScoutActuatorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["rpm"],2,true);n=addSize(n,2);
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"]))bad();n=addSize(n,8);
if(typeof value["driver_voltage"]!=="number"||!Number.isFinite(value["driver_voltage"])||!Number.isFinite(Math.fround(value["driver_voltage"])))bad();n=addSize(n,4);
if(typeof value["driver_temperature"]!=="number"||!Number.isFinite(value["driver_temperature"])||!Number.isFinite(Math.fround(value["driver_temperature"])))bad();n=addSize(n,4);
integer(value["motor_temperature"],1,true);n=addSize(n,1);
if(n>19)bad(90);return n;}
function Ros2Scout_msgsMsgScoutActuatorStateWrite(value,w){
w.v.setInt16(w.at,value["rpm"],true);w.at+=2;
w.v.setFloat64(w.at,value["current"],true);w.at+=8;
w.v.setFloat32(w.at,value["driver_voltage"],true);w.at+=4;
w.v.setFloat32(w.at,value["driver_temperature"],true);w.at+=4;
w.v.setInt8(w.at,value["motor_temperature"],true);w.at+=1;
}
function Ros2Scout_msgsMsgScoutActuatorStateRead(r){const value={};
{
const decoded=r.v.getInt16(r.take(2),true);
value["rpm"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["driver_voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["driver_temperature"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["motor_temperature"]=decoded;
}
return value;}
function Ros2Scout_msgsMsgScoutActuatorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rpm"];
out["rpm"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["driver_voltage"];
out["driver_voltage"]=field;
}
{const field=value["driver_temperature"];
out["driver_temperature"]=field;
}
{const field=value["motor_temperature"];
out["motor_temperature"]=field;
}
return out;}
/** Complete ROS2 scout_msgs/msg/ScoutLightCmd codec; maxima are not actual lengths. */
export const Ros2Scout_msgsMsgScoutLightCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_a82206813682fbf1a6b41009c12191ad9ce47c98444d8ea9792e7b212ee602d7",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({"LIGHT_CONST_OFF":0,"LIGHT_CONST_ON":1,"LIGHT_BREATH":2,"LIGHT_CUSTOM":3}),
defaultValue(){return {"cmd_ctrl_allowed":false,"front_mode":0,"front_custom_value":0,"rear_mode":0,"rear_custom_value":0};},
validateValue(value){Ros2Scout_msgsMsgScoutLightCmdSize(Ros2Scout_msgsMsgScoutLightCmdCapture(value));return true;},
encode(value){const owned=Ros2Scout_msgsMsgScoutLightCmdCapture(value),n=Ros2Scout_msgsMsgScoutLightCmdSize(owned),w=new Writer(n);Ros2Scout_msgsMsgScoutLightCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Scout_msgsMsgScoutLightCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Scout_msgsMsgScoutLightCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["cmd_ctrl_allowed"]!=="boolean")bad();n=addSize(n,1);
integer(value["front_mode"],1,false);n=addSize(n,1);
integer(value["front_custom_value"],1,false);n=addSize(n,1);
integer(value["rear_mode"],1,false);n=addSize(n,1);
integer(value["rear_custom_value"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Scout_msgsMsgScoutLightCmdWrite(value,w){
w.v.setUint8(w.at,value["cmd_ctrl_allowed"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["front_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["front_custom_value"],true);w.at+=1;
w.v.setUint8(w.at,value["rear_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["rear_custom_value"],true);w.at+=1;
}
function Ros2Scout_msgsMsgScoutLightCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cmd_ctrl_allowed"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front_mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front_custom_value"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["rear_mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["rear_custom_value"]=decoded;
}
return value;}
function Ros2Scout_msgsMsgScoutLightCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cmd_ctrl_allowed"];
out["cmd_ctrl_allowed"]=field;
}
{const field=value["front_mode"];
out["front_mode"]=field;
}
{const field=value["front_custom_value"];
out["front_custom_value"]=field;
}
{const field=value["rear_mode"];
out["rear_mode"]=field;
}
{const field=value["rear_custom_value"];
out["rear_custom_value"]=field;
}
return out;}
/** Complete ROS2 scout_msgs/msg/ScoutLightState codec; maxima are not actual lengths. */
export const Ros2Scout_msgsMsgScoutLightStateCodec=Object.freeze({codecIdentity:"UWVMROS1_4f42b9365cc76673b156e725aac9448e0f0d806105d3717cdb2628c384a6d76a",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"custom_value":0};},
validateValue(value){Ros2Scout_msgsMsgScoutLightStateSize(Ros2Scout_msgsMsgScoutLightStateCapture(value));return true;},
encode(value){const owned=Ros2Scout_msgsMsgScoutLightStateCapture(value),n=Ros2Scout_msgsMsgScoutLightStateSize(owned),w=new Writer(n);Ros2Scout_msgsMsgScoutLightStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Scout_msgsMsgScoutLightStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Scout_msgsMsgScoutLightStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["custom_value"],1,false);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Scout_msgsMsgScoutLightStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["custom_value"],true);w.at+=1;
}
function Ros2Scout_msgsMsgScoutLightStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["custom_value"]=decoded;
}
return value;}
function Ros2Scout_msgsMsgScoutLightStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["custom_value"];
out["custom_value"]=field;
}
return out;}
/** Complete ROS2 scout_msgs/msg/ScoutRCState codec; maxima are not actual lengths. */
export const Ros2Scout_msgsMsgScoutRCStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ba13e4c728041d14352c021fe5c1c5eee5d531c84ac64c61d944d6192dcd4298",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"swa":0,"swb":0,"swc":0,"swd":0,"stick_right_v":0,"stick_right_h":0,"stick_left_v":0,"stick_left_h":0,"var_a":0};},
validateValue(value){Ros2Scout_msgsMsgScoutRCStateSize(Ros2Scout_msgsMsgScoutRCStateCapture(value));return true;},
encode(value){const owned=Ros2Scout_msgsMsgScoutRCStateCapture(value),n=Ros2Scout_msgsMsgScoutRCStateSize(owned),w=new Writer(n);Ros2Scout_msgsMsgScoutRCStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Scout_msgsMsgScoutRCStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Scout_msgsMsgScoutRCStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["swa"],1,false);n=addSize(n,1);
integer(value["swb"],1,false);n=addSize(n,1);
integer(value["swc"],1,false);n=addSize(n,1);
integer(value["swd"],1,false);n=addSize(n,1);
integer(value["stick_right_v"],1,true);n=addSize(n,1);
integer(value["stick_right_h"],1,true);n=addSize(n,1);
integer(value["stick_left_v"],1,true);n=addSize(n,1);
integer(value["stick_left_h"],1,true);n=addSize(n,1);
integer(value["var_a"],1,true);n=addSize(n,1);
if(n>9)bad(90);return n;}
function Ros2Scout_msgsMsgScoutRCStateWrite(value,w){
w.v.setUint8(w.at,value["swa"],true);w.at+=1;
w.v.setUint8(w.at,value["swb"],true);w.at+=1;
w.v.setUint8(w.at,value["swc"],true);w.at+=1;
w.v.setUint8(w.at,value["swd"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_right_v"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_right_h"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_left_v"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_left_h"],true);w.at+=1;
w.v.setInt8(w.at,value["var_a"],true);w.at+=1;
}
function Ros2Scout_msgsMsgScoutRCStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["swa"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["swb"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["swc"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["swd"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_right_v"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_right_h"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_left_v"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_left_h"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["var_a"]=decoded;
}
return value;}
function Ros2Scout_msgsMsgScoutRCStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["swa"];
out["swa"]=field;
}
{const field=value["swb"];
out["swb"]=field;
}
{const field=value["swc"];
out["swc"]=field;
}
{const field=value["swd"];
out["swd"]=field;
}
{const field=value["stick_right_v"];
out["stick_right_v"]=field;
}
{const field=value["stick_right_h"];
out["stick_right_h"]=field;
}
{const field=value["stick_left_v"];
out["stick_left_v"]=field;
}
{const field=value["stick_left_h"];
out["stick_left_h"]=field;
}
{const field=value["var_a"];
out["var_a"]=field;
}
return out;}
/** Complete ROS2 scout_msgs/msg/ScoutStatus codec; maxima are not actual lengths. */
export const Ros2Scout_msgsMsgScoutStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_75db4db1dcc07210d543b8e6bd6b6a7aadbf23db8043ff372a3b224c5c235b86",maxWireBytes:249,minWireBytes:121,cartesianMaxWireBytes:249n,aggregateWireBytes:0,constants:Object.freeze({"MOTOR_ID_FRONT_RIGHT":0,"MOTOR_ID_FRONT_LEFT":1,"MOTOR_ID_REAR_RIGHT":2,"MOTOR_ID_REAR_LEFT":3,"LIGHT_ID_FRONT":0,"LIGHT_ID_REAR":1}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"linear_velocity":0,"angular_velocity":0,"vehicle_state":0,"control_mode":0,"error_code":0,"battery_voltage":0,"actuator_states":Array.from({length:4},()=>(Ros2Scout_msgsMsgScoutActuatorStateCodec.defaultValue())),"light_control_enabled":false,"front_light_state":Ros2Scout_msgsMsgScoutLightStateCodec.defaultValue(),"rear_light_state":Ros2Scout_msgsMsgScoutLightStateCodec.defaultValue()};},
validateValue(value){Ros2Scout_msgsMsgScoutStatusSize(Ros2Scout_msgsMsgScoutStatusCapture(value));return true;},
encode(value){const owned=Ros2Scout_msgsMsgScoutStatusCapture(value),n=Ros2Scout_msgsMsgScoutStatusSize(owned),w=new Writer(n);Ros2Scout_msgsMsgScoutStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,249,90);if(owned.length<121)bad();const r=new Reader(owned),value=Ros2Scout_msgsMsgScoutStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Scout_msgsMsgScoutStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["linear_velocity"]!=="number"||!Number.isFinite(value["linear_velocity"]))bad();n=addSize(n,8);
if(typeof value["angular_velocity"]!=="number"||!Number.isFinite(value["angular_velocity"]))bad();n=addSize(n,8);
integer(value["vehicle_state"],1,false);n=addSize(n,1);
integer(value["control_mode"],1,false);n=addSize(n,1);
integer(value["error_code"],2,false);n=addSize(n,2);
if(typeof value["battery_voltage"]!=="number"||!Number.isFinite(value["battery_voltage"]))bad();n=addSize(n,8);
if(!Array.isArray(value["actuator_states"])||value["actuator_states"].length!==4)bad();
for(const element of value["actuator_states"]){
n=addSize(n,Ros2Scout_msgsMsgScoutActuatorStateSize(element));
}
if(typeof value["light_control_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Scout_msgsMsgScoutLightStateSize(value["front_light_state"]));
n=addSize(n,Ros2Scout_msgsMsgScoutLightStateSize(value["rear_light_state"]));
if(n>249)bad(90);return n;}
function Ros2Scout_msgsMsgScoutStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["linear_velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["angular_velocity"],true);w.at+=8;
w.v.setUint8(w.at,value["vehicle_state"],true);w.at+=1;
w.v.setUint8(w.at,value["control_mode"],true);w.at+=1;
w.v.setUint16(w.at,value["error_code"],true);w.at+=2;
w.v.setFloat64(w.at,value["battery_voltage"],true);w.at+=8;
for(const element of value["actuator_states"]){
Ros2Scout_msgsMsgScoutActuatorStateWrite(element,w);
}
w.v.setUint8(w.at,value["light_control_enabled"]?1:0,true);w.at+=1;
Ros2Scout_msgsMsgScoutLightStateWrite(value["front_light_state"],w);
Ros2Scout_msgsMsgScoutLightStateWrite(value["rear_light_state"],w);
}
function Ros2Scout_msgsMsgScoutStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["linear_velocity"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["angular_velocity"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["vehicle_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["control_mode"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["error_code"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_voltage"]=decoded;
}
{
{const count=4;if(count>4)bad();value["actuator_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Scout_msgsMsgScoutActuatorStateRead(r);
value["actuator_states"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["light_control_enabled"]=decoded!==0;
}
{
const decoded=Ros2Scout_msgsMsgScoutLightStateRead(r);
value["front_light_state"]=decoded;
}
{
const decoded=Ros2Scout_msgsMsgScoutLightStateRead(r);
value["rear_light_state"]=decoded;
}
return value;}
function Ros2Scout_msgsMsgScoutStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["linear_velocity"];
out["linear_velocity"]=field;
}
{const field=value["angular_velocity"];
out["angular_velocity"]=field;
}
{const field=value["vehicle_state"];
out["vehicle_state"]=field;
}
{const field=value["control_mode"];
out["control_mode"]=field;
}
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["battery_voltage"];
out["battery_voltage"]=field;
}
{const field=value["actuator_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Scout_msgsMsgScoutActuatorStateCapture(element);
}out["actuator_states"]=values;
}
{const field=value["light_control_enabled"];
out["light_control_enabled"]=field;
}
{const field=value["front_light_state"];
out["front_light_state"]=Ros2Scout_msgsMsgScoutLightStateCapture(field);
}
{const field=value["rear_light_state"];
out["rear_light_state"]=Ros2Scout_msgsMsgScoutLightStateCapture(field);
}
return out;}
