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
/** Complete ROS1 geometry_msgs/Point codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsPointCodec=Object.freeze({codecIdentity:"UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros1Geometry_msgsPointSize(Ros1Geometry_msgsPointCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsPointCapture(value),n=Ros1Geometry_msgsPointSize(owned),w=new Writer(n);Ros1Geometry_msgsPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Geometry_msgsPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros1Geometry_msgsPointWrite(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
}
function Ros1Geometry_msgsPointRead(r){const value={};
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
function Ros1Geometry_msgsPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 geometry_msgs/Quaternion codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsQuaternionCodec=Object.freeze({codecIdentity:"UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"w":0};},
validateValue(value){Ros1Geometry_msgsQuaternionSize(Ros1Geometry_msgsQuaternionCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsQuaternionCapture(value),n=Ros1Geometry_msgsQuaternionSize(owned),w=new Writer(n);Ros1Geometry_msgsQuaternionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros1Geometry_msgsQuaternionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsQuaternionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"]))bad();n=addSize(n,8);
if(n>32)bad(90);return n;}
function Ros1Geometry_msgsQuaternionWrite(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
w.v.setFloat64(w.at,value["w"],true);w.at+=8;
}
function Ros1Geometry_msgsQuaternionRead(r){const value={};
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
function Ros1Geometry_msgsQuaternionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 geometry_msgs/Pose codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsPoseCodec=Object.freeze({codecIdentity:"UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"position":Ros1Geometry_msgsPointCodec.defaultValue(),"orientation":Ros1Geometry_msgsQuaternionCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsPoseSize(Ros1Geometry_msgsPoseCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsPoseCapture(value),n=Ros1Geometry_msgsPoseSize(owned),w=new Writer(n);Ros1Geometry_msgsPoseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Geometry_msgsPoseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsPoseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPointSize(value["position"]));
n=addSize(n,Ros1Geometry_msgsQuaternionSize(value["orientation"]));
if(n>56)bad(90);return n;}
function Ros1Geometry_msgsPoseWrite(value,w){
Ros1Geometry_msgsPointWrite(value["position"],w);
Ros1Geometry_msgsQuaternionWrite(value["orientation"],w);
}
function Ros1Geometry_msgsPoseRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPointRead(r);
value["position"]=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternionRead(r);
value["orientation"]=decoded;
}
return value;}
function Ros1Geometry_msgsPoseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["position"];
out["position"]=Ros1Geometry_msgsPointCapture(field);
}
{const field=value["orientation"];
out["orientation"]=Ros1Geometry_msgsQuaternionCapture(field);
}
return out;}
/** Complete ROS1 geometry_msgs/PoseWithCovariance codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsPoseWithCovarianceCodec=Object.freeze({codecIdentity:"UWVMROS1_17b25963aafb3adfd0432febe040c9b38a531a7a667949b89e0f3937585ef8cf",maxWireBytes:344,minWireBytes:344,cartesianMaxWireBytes:344n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":Ros1Geometry_msgsPoseCodec.defaultValue(),"covariance":Array.from({length:36},()=>(0))};},
validateValue(value){Ros1Geometry_msgsPoseWithCovarianceSize(Ros1Geometry_msgsPoseWithCovarianceCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsPoseWithCovarianceCapture(value),n=Ros1Geometry_msgsPoseWithCovarianceSize(owned),w=new Writer(n);Ros1Geometry_msgsPoseWithCovarianceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,344,90);if(owned.length<344)bad();const r=new Reader(owned),value=Ros1Geometry_msgsPoseWithCovarianceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsPoseWithCovarianceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPoseSize(value["pose"]));
if(!Array.isArray(value["covariance"])||value["covariance"].length!==36)bad();
for(const element of value["covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>344)bad(90);return n;}
function Ros1Geometry_msgsPoseWithCovarianceWrite(value,w){
Ros1Geometry_msgsPoseWrite(value["pose"],w);
for(const element of value["covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Geometry_msgsPoseWithCovarianceRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPoseRead(r);
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
function Ros1Geometry_msgsPoseWithCovarianceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseCapture(field);
}
{const field=value["covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==36)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["covariance"]=values;
}
return out;}
/** Complete ROS1 geometry_msgs/Vector3 codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsVector3Codec=Object.freeze({codecIdentity:"UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros1Geometry_msgsVector3Size(Ros1Geometry_msgsVector3Capture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsVector3Capture(value),n=Ros1Geometry_msgsVector3Size(owned),w=new Writer(n);Ros1Geometry_msgsVector3Write(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Geometry_msgsVector3Read(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsVector3Size(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"]))bad();n=addSize(n,8);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"]))bad();n=addSize(n,8);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"]))bad();n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros1Geometry_msgsVector3Write(value,w){
w.v.setFloat64(w.at,value["x"],true);w.at+=8;
w.v.setFloat64(w.at,value["y"],true);w.at+=8;
w.v.setFloat64(w.at,value["z"],true);w.at+=8;
}
function Ros1Geometry_msgsVector3Read(r){const value={};
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
function Ros1Geometry_msgsVector3Capture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 geometry_msgs/Twist codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsTwistCodec=Object.freeze({codecIdentity:"UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"linear":Ros1Geometry_msgsVector3Codec.defaultValue(),"angular":Ros1Geometry_msgsVector3Codec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsTwistSize(Ros1Geometry_msgsTwistCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsTwistCapture(value),n=Ros1Geometry_msgsTwistSize(owned),w=new Writer(n);Ros1Geometry_msgsTwistWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros1Geometry_msgsTwistRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsTwistSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsVector3Size(value["linear"]));
n=addSize(n,Ros1Geometry_msgsVector3Size(value["angular"]));
if(n>48)bad(90);return n;}
function Ros1Geometry_msgsTwistWrite(value,w){
Ros1Geometry_msgsVector3Write(value["linear"],w);
Ros1Geometry_msgsVector3Write(value["angular"],w);
}
function Ros1Geometry_msgsTwistRead(r){const value={};
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["linear"]=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["angular"]=decoded;
}
return value;}
function Ros1Geometry_msgsTwistCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["linear"];
out["linear"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["angular"];
out["angular"]=Ros1Geometry_msgsVector3Capture(field);
}
return out;}
/** Complete ROS1 geometry_msgs/TwistWithCovariance codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsTwistWithCovarianceCodec=Object.freeze({codecIdentity:"UWVMROS1_ff6790d181503b424d84726c8f4a7852cef14c6dd93e6d32cf3c37023f74bcdf",maxWireBytes:336,minWireBytes:336,cartesianMaxWireBytes:336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"twist":Ros1Geometry_msgsTwistCodec.defaultValue(),"covariance":Array.from({length:36},()=>(0))};},
validateValue(value){Ros1Geometry_msgsTwistWithCovarianceSize(Ros1Geometry_msgsTwistWithCovarianceCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsTwistWithCovarianceCapture(value),n=Ros1Geometry_msgsTwistWithCovarianceSize(owned),w=new Writer(n);Ros1Geometry_msgsTwistWithCovarianceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,336,90);if(owned.length<336)bad();const r=new Reader(owned),value=Ros1Geometry_msgsTwistWithCovarianceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsTwistWithCovarianceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsTwistSize(value["twist"]));
if(!Array.isArray(value["covariance"])||value["covariance"].length!==36)bad();
for(const element of value["covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>336)bad(90);return n;}
function Ros1Geometry_msgsTwistWithCovarianceWrite(value,w){
Ros1Geometry_msgsTwistWrite(value["twist"],w);
for(const element of value["covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Geometry_msgsTwistWithCovarianceRead(r){const value={};
{
const decoded=Ros1Geometry_msgsTwistRead(r);
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
function Ros1Geometry_msgsTwistWithCovarianceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["twist"];
out["twist"]=Ros1Geometry_msgsTwistCapture(field);
}
{const field=value["covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==36)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["covariance"]=values;
}
return out;}
/** Complete ROS1 time codec; maxima are not actual lengths. */
export const Ros1TimeCodec=Object.freeze({codecIdentity:"UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nsec":0};},
validateValue(value){Ros1TimeSize(Ros1TimeCapture(value));return true;},
encode(value){const owned=Ros1TimeCapture(value),n=Ros1TimeSize(owned),w=new Writer(n);Ros1TimeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1TimeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1TimeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,false);n=addSize(n,4);
integer(value["nsec"],4,false);n=addSize(n,4);
if(value["nsec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros1TimeWrite(value,w){
w.v.setUint32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nsec"],true);w.at+=4;
}
function Ros1TimeRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["nsec"]=decoded;
}
if(value["nsec"]>=1000000000)bad();
return value;}
function Ros1TimeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nsec"];
out["nsec"]=field;
}
return out;}
/** Complete ROS1 std_msgs/Header codec; maxima are not actual lengths. */
export const Ros1Std_msgsHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435",maxWireBytes:144,minWireBytes:16,cartesianMaxWireBytes:144n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"seq":0,"stamp":Ros1TimeCodec.defaultValue(),"frame_id":""};},
validateValue(value){Ros1Std_msgsHeaderSize(Ros1Std_msgsHeaderCapture(value));return true;},
encode(value){const owned=Ros1Std_msgsHeaderCapture(value),n=Ros1Std_msgsHeaderSize(owned),w=new Writer(n);Ros1Std_msgsHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,144,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Std_msgsHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_msgsHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["seq"],4,false);n=addSize(n,4);
n=addSize(n,Ros1TimeSize(value["stamp"]));
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(n>144)bad(90);return n;}
function Ros1Std_msgsHeaderWrite(value,w){
w.v.setUint32(w.at,value["seq"],true);w.at+=4;
Ros1TimeWrite(value["stamp"],w);
w.text(value["frame_id"],128);
}
function Ros1Std_msgsHeaderRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["seq"]=decoded;
}
{
const decoded=Ros1TimeRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.text(128);
value["frame_id"]=decoded;
}
return value;}
function Ros1Std_msgsHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["seq"];
out["seq"]=field;
}
{const field=value["stamp"];
out["stamp"]=Ros1TimeCapture(field);
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS1 nav_msgs/Odometry codec; maxima are not actual lengths. */
export const Ros1Nav_msgsOdometryCodec=Object.freeze({codecIdentity:"UWVMROS1_c10d3ce1f2824c33d728d0006fb83134330d0332af122f179cf2dac32181a2ca",maxWireBytes:956,minWireBytes:700,cartesianMaxWireBytes:956n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"child_frame_id":"","pose":Ros1Geometry_msgsPoseWithCovarianceCodec.defaultValue(),"twist":Ros1Geometry_msgsTwistWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros1Nav_msgsOdometrySize(Ros1Nav_msgsOdometryCapture(value));return true;},
encode(value){const owned=Ros1Nav_msgsOdometryCapture(value),n=Ros1Nav_msgsOdometrySize(owned),w=new Writer(n);Ros1Nav_msgsOdometryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,956,90);if(owned.length<700)bad();const r=new Reader(owned),value=Ros1Nav_msgsOdometryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Nav_msgsOdometrySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],128).length);
n=addSize(n,Ros1Geometry_msgsPoseWithCovarianceSize(value["pose"]));
n=addSize(n,Ros1Geometry_msgsTwistWithCovarianceSize(value["twist"]));
if(n>956)bad(90);return n;}
function Ros1Nav_msgsOdometryWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],128);
Ros1Geometry_msgsPoseWithCovarianceWrite(value["pose"],w);
Ros1Geometry_msgsTwistWithCovarianceWrite(value["twist"],w);
}
function Ros1Nav_msgsOdometryRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(128);
value["child_frame_id"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseWithCovarianceRead(r);
value["pose"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistWithCovarianceRead(r);
value["twist"]=decoded;
}
return value;}
function Ros1Nav_msgsOdometryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["child_frame_id"];
out["child_frame_id"]=field;
}
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseWithCovarianceCapture(field);
}
{const field=value["twist"];
out["twist"]=Ros1Geometry_msgsTwistWithCovarianceCapture(field);
}
return out;}
/** Complete ROS1 scout_msgs/ScoutBmsStatus codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutBmsStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_6921538c08e86ab5b9ecdbf974c6cb5b580488f4ed96ccbdb8b7824d51e9cf95",maxWireBytes:44,minWireBytes:44,cartesianMaxWireBytes:44n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"SOC":0,"SOH":0,"battery_voltage":0,"battery_current":0,"battery_temperature":0,"Alarm_Status_1":0,"Alarm_Status_2":0,"Warning_Status_1":0,"Warning_Status_2":0};},
validateValue(value){Ros1Scout_msgsScoutBmsStatusSize(Ros1Scout_msgsScoutBmsStatusCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutBmsStatusCapture(value),n=Ros1Scout_msgsScoutBmsStatusSize(owned),w=new Writer(n);Ros1Scout_msgsScoutBmsStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,44,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutBmsStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutBmsStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["SOC"]!=="number"||!Number.isFinite(value["SOC"]))bad();n=addSize(n,8);
if(typeof value["SOH"]!=="number"||!Number.isFinite(value["SOH"]))bad();n=addSize(n,8);
if(typeof value["battery_voltage"]!=="number"||!Number.isFinite(value["battery_voltage"]))bad();n=addSize(n,8);
if(typeof value["battery_current"]!=="number"||!Number.isFinite(value["battery_current"]))bad();n=addSize(n,8);
if(typeof value["battery_temperature"]!=="number"||!Number.isFinite(value["battery_temperature"]))bad();n=addSize(n,8);
integer(value["Alarm_Status_1"],1,false);n=addSize(n,1);
integer(value["Alarm_Status_2"],1,false);n=addSize(n,1);
integer(value["Warning_Status_1"],1,false);n=addSize(n,1);
integer(value["Warning_Status_2"],1,false);n=addSize(n,1);
if(n>44)bad(90);return n;}
function Ros1Scout_msgsScoutBmsStatusWrite(value,w){
w.v.setFloat64(w.at,value["SOC"],true);w.at+=8;
w.v.setFloat64(w.at,value["SOH"],true);w.at+=8;
w.v.setFloat64(w.at,value["battery_voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["battery_current"],true);w.at+=8;
w.v.setFloat64(w.at,value["battery_temperature"],true);w.at+=8;
w.v.setUint8(w.at,value["Alarm_Status_1"],true);w.at+=1;
w.v.setUint8(w.at,value["Alarm_Status_2"],true);w.at+=1;
w.v.setUint8(w.at,value["Warning_Status_1"],true);w.at+=1;
w.v.setUint8(w.at,value["Warning_Status_2"],true);w.at+=1;
}
function Ros1Scout_msgsScoutBmsStatusRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["SOC"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["SOH"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_temperature"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["Alarm_Status_1"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["Alarm_Status_2"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["Warning_Status_1"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["Warning_Status_2"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutBmsStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["SOC"];
out["SOC"]=field;
}
{const field=value["SOH"];
out["SOH"]=field;
}
{const field=value["battery_voltage"];
out["battery_voltage"]=field;
}
{const field=value["battery_current"];
out["battery_current"]=field;
}
{const field=value["battery_temperature"];
out["battery_temperature"]=field;
}
{const field=value["Alarm_Status_1"];
out["Alarm_Status_1"]=field;
}
{const field=value["Alarm_Status_2"];
out["Alarm_Status_2"]=field;
}
{const field=value["Warning_Status_1"];
out["Warning_Status_1"]=field;
}
{const field=value["Warning_Status_2"];
out["Warning_Status_2"]=field;
}
return out;}
/** Complete ROS1 scout_msgs/ScoutDriverState codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutDriverStateCodec=Object.freeze({codecIdentity:"UWVMROS1_4c56c71ae621b2b8822d742894d19946492002453c93b72faccb4f2abc9d4275",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"driver_voltage":0,"driver_temperature":0,"driver_state":0};},
validateValue(value){Ros1Scout_msgsScoutDriverStateSize(Ros1Scout_msgsScoutDriverStateCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutDriverStateCapture(value),n=Ros1Scout_msgsScoutDriverStateSize(owned),w=new Writer(n);Ros1Scout_msgsScoutDriverStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutDriverStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutDriverStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["driver_voltage"]!=="number"||!Number.isFinite(value["driver_voltage"]))bad();n=addSize(n,8);
if(typeof value["driver_temperature"]!=="number"||!Number.isFinite(value["driver_temperature"]))bad();n=addSize(n,8);
integer(value["driver_state"],1,false);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros1Scout_msgsScoutDriverStateWrite(value,w){
w.v.setFloat64(w.at,value["driver_voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["driver_temperature"],true);w.at+=8;
w.v.setUint8(w.at,value["driver_state"],true);w.at+=1;
}
function Ros1Scout_msgsScoutDriverStateRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["driver_voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["driver_temperature"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["driver_state"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutDriverStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["driver_voltage"];
out["driver_voltage"]=field;
}
{const field=value["driver_temperature"];
out["driver_temperature"]=field;
}
{const field=value["driver_state"];
out["driver_state"]=field;
}
return out;}
/** Complete ROS1 scout_msgs/ScoutLightCmd codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutLightCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_cf08cccf454f88111d5cc4a4b1ace08de72cf1ae9a9400c23a148126da95942a",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({"LIGHT_CONST_OFF":0,"LIGHT_CONST_ON":1,"LIGHT_BREATH":2,"LIGHT_CUSTOM":3}),
defaultValue(){return {"enable_cmd_light_control":false,"front_mode":0,"front_custom_value":0,"rear_mode":0,"rear_custom_value":0};},
validateValue(value){Ros1Scout_msgsScoutLightCmdSize(Ros1Scout_msgsScoutLightCmdCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutLightCmdCapture(value),n=Ros1Scout_msgsScoutLightCmdSize(owned),w=new Writer(n);Ros1Scout_msgsScoutLightCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutLightCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutLightCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enable_cmd_light_control"]!=="boolean")bad();n=addSize(n,1);
integer(value["front_mode"],1,false);n=addSize(n,1);
integer(value["front_custom_value"],1,false);n=addSize(n,1);
integer(value["rear_mode"],1,false);n=addSize(n,1);
integer(value["rear_custom_value"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros1Scout_msgsScoutLightCmdWrite(value,w){
w.v.setUint8(w.at,value["enable_cmd_light_control"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["front_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["front_custom_value"],true);w.at+=1;
w.v.setUint8(w.at,value["rear_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["rear_custom_value"],true);w.at+=1;
}
function Ros1Scout_msgsScoutLightCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_cmd_light_control"]=decoded!==0;
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
function Ros1Scout_msgsScoutLightCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enable_cmd_light_control"];
out["enable_cmd_light_control"]=field;
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
/** Complete ROS1 scout_msgs/ScoutLightState codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutLightStateCodec=Object.freeze({codecIdentity:"UWVMROS1_af55048f9200cfa80fc419664204a515f628cc610a352ab3ed8443dd7ac0ae51",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"custom_value":0};},
validateValue(value){Ros1Scout_msgsScoutLightStateSize(Ros1Scout_msgsScoutLightStateCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutLightStateCapture(value),n=Ros1Scout_msgsScoutLightStateSize(owned),w=new Writer(n);Ros1Scout_msgsScoutLightStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutLightStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutLightStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["custom_value"],1,false);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros1Scout_msgsScoutLightStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["custom_value"],true);w.at+=1;
}
function Ros1Scout_msgsScoutLightStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["custom_value"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutLightStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["custom_value"];
out["custom_value"]=field;
}
return out;}
/** Complete ROS1 scout_msgs/ScoutMotorState codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ca67a704d7bb4ba6b2839db1fdb8a75bc753868ec871d101c24c85d8202a79b7",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current":0,"rpm":0,"temperature":0,"motor_pose":0};},
validateValue(value){Ros1Scout_msgsScoutMotorStateSize(Ros1Scout_msgsScoutMotorStateCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutMotorStateCapture(value),n=Ros1Scout_msgsScoutMotorStateSize(owned),w=new Writer(n);Ros1Scout_msgsScoutMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"]))bad();n=addSize(n,8);
if(typeof value["rpm"]!=="number"||!Number.isFinite(value["rpm"]))bad();n=addSize(n,8);
if(typeof value["temperature"]!=="number"||!Number.isFinite(value["temperature"]))bad();n=addSize(n,8);
if(typeof value["motor_pose"]!=="number"||!Number.isFinite(value["motor_pose"]))bad();n=addSize(n,8);
if(n>32)bad(90);return n;}
function Ros1Scout_msgsScoutMotorStateWrite(value,w){
w.v.setFloat64(w.at,value["current"],true);w.at+=8;
w.v.setFloat64(w.at,value["rpm"],true);w.at+=8;
w.v.setFloat64(w.at,value["temperature"],true);w.at+=8;
w.v.setFloat64(w.at,value["motor_pose"],true);w.at+=8;
}
function Ros1Scout_msgsScoutMotorStateRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["rpm"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["temperature"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["motor_pose"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current"];
out["current"]=field;
}
{const field=value["rpm"];
out["rpm"]=field;
}
{const field=value["temperature"];
out["temperature"]=field;
}
{const field=value["motor_pose"];
out["motor_pose"]=field;
}
return out;}
/** Complete ROS1 scout_msgs/ScoutRsStatus codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutRsStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_deb068b75fb629df3724c141ed3be9f6ee8a4aac7727cdaa28769cb61211f7de",maxWireBytes:153,minWireBytes:25,cartesianMaxWireBytes:153n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"stick_left_h":0,"stick_left_v":0,"stick_right_h":0,"stick_right_v":0,"swa":0,"swb":0,"swc":0,"swd":0,"var_a":0};},
validateValue(value){Ros1Scout_msgsScoutRsStatusSize(Ros1Scout_msgsScoutRsStatusCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutRsStatusCapture(value),n=Ros1Scout_msgsScoutRsStatusSize(owned),w=new Writer(n);Ros1Scout_msgsScoutRsStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,153,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutRsStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutRsStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
integer(value["stick_left_h"],1,true);n=addSize(n,1);
integer(value["stick_left_v"],1,true);n=addSize(n,1);
integer(value["stick_right_h"],1,true);n=addSize(n,1);
integer(value["stick_right_v"],1,true);n=addSize(n,1);
integer(value["swa"],1,false);n=addSize(n,1);
integer(value["swb"],1,false);n=addSize(n,1);
integer(value["swc"],1,false);n=addSize(n,1);
integer(value["swd"],1,false);n=addSize(n,1);
integer(value["var_a"],1,false);n=addSize(n,1);
if(n>153)bad(90);return n;}
function Ros1Scout_msgsScoutRsStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["stick_left_h"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_left_v"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_right_h"],true);w.at+=1;
w.v.setInt8(w.at,value["stick_right_v"],true);w.at+=1;
w.v.setUint8(w.at,value["swa"],true);w.at+=1;
w.v.setUint8(w.at,value["swb"],true);w.at+=1;
w.v.setUint8(w.at,value["swc"],true);w.at+=1;
w.v.setUint8(w.at,value["swd"],true);w.at+=1;
w.v.setUint8(w.at,value["var_a"],true);w.at+=1;
}
function Ros1Scout_msgsScoutRsStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_left_h"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_left_v"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_right_h"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["stick_right_v"]=decoded;
}
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
const decoded=r.v.getUint8(r.take(1),true);
value["var_a"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutRsStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["stick_left_h"];
out["stick_left_h"]=field;
}
{const field=value["stick_left_v"];
out["stick_left_v"]=field;
}
{const field=value["stick_right_h"];
out["stick_right_h"]=field;
}
{const field=value["stick_right_v"];
out["stick_right_v"]=field;
}
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
{const field=value["var_a"];
out["var_a"]=field;
}
return out;}
/** Complete ROS1 scout_msgs/ScoutStatus codec; maxima are not actual lengths. */
export const Ros1Scout_msgsScoutStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_ff62854d4de0aae3dfbbc312822c6f912294f0feb04a7fff943df602d40bcbe2",maxWireBytes:381,minWireBytes:253,cartesianMaxWireBytes:381n,aggregateWireBytes:0,constants:Object.freeze({"MOTOR_ID_FRONT_RIGHT":0,"MOTOR_ID_FRONT_LEFT":1,"MOTOR_ID_REAR_RIGHT":2,"MOTOR_ID_REAR_LEFT":3,"LIGHT_ID_FRONT":0,"LIGHT_ID_REAR":1}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"linear_velocity":0,"angular_velocity":0,"lateral_velocity":0,"base_state":0,"control_mode":0,"fault_code":0,"battery_voltage":0,"motor_states":Array.from({length:4},()=>(Ros1Scout_msgsScoutMotorStateCodec.defaultValue())),"driver_states":Array.from({length:4},()=>(Ros1Scout_msgsScoutDriverStateCodec.defaultValue())),"light_control_enabled":false,"front_light_state":Ros1Scout_msgsScoutLightStateCodec.defaultValue(),"rear_light_state":Ros1Scout_msgsScoutLightStateCodec.defaultValue()};},
validateValue(value){Ros1Scout_msgsScoutStatusSize(Ros1Scout_msgsScoutStatusCapture(value));return true;},
encode(value){const owned=Ros1Scout_msgsScoutStatusCapture(value),n=Ros1Scout_msgsScoutStatusSize(owned),w=new Writer(n);Ros1Scout_msgsScoutStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,381,90);if(owned.length<253)bad();const r=new Reader(owned),value=Ros1Scout_msgsScoutStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Scout_msgsScoutStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["linear_velocity"]!=="number"||!Number.isFinite(value["linear_velocity"]))bad();n=addSize(n,8);
if(typeof value["angular_velocity"]!=="number"||!Number.isFinite(value["angular_velocity"]))bad();n=addSize(n,8);
if(typeof value["lateral_velocity"]!=="number"||!Number.isFinite(value["lateral_velocity"]))bad();n=addSize(n,8);
integer(value["base_state"],1,false);n=addSize(n,1);
integer(value["control_mode"],1,false);n=addSize(n,1);
integer(value["fault_code"],2,false);n=addSize(n,2);
if(typeof value["battery_voltage"]!=="number"||!Number.isFinite(value["battery_voltage"]))bad();n=addSize(n,8);
if(!Array.isArray(value["motor_states"])||value["motor_states"].length!==4)bad();
for(const element of value["motor_states"]){
n=addSize(n,Ros1Scout_msgsScoutMotorStateSize(element));
}
if(!Array.isArray(value["driver_states"])||value["driver_states"].length!==4)bad();
for(const element of value["driver_states"]){
n=addSize(n,Ros1Scout_msgsScoutDriverStateSize(element));
}
if(typeof value["light_control_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Scout_msgsScoutLightStateSize(value["front_light_state"]));
n=addSize(n,Ros1Scout_msgsScoutLightStateSize(value["rear_light_state"]));
if(n>381)bad(90);return n;}
function Ros1Scout_msgsScoutStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["linear_velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["angular_velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["lateral_velocity"],true);w.at+=8;
w.v.setUint8(w.at,value["base_state"],true);w.at+=1;
w.v.setUint8(w.at,value["control_mode"],true);w.at+=1;
w.v.setUint16(w.at,value["fault_code"],true);w.at+=2;
w.v.setFloat64(w.at,value["battery_voltage"],true);w.at+=8;
for(const element of value["motor_states"]){
Ros1Scout_msgsScoutMotorStateWrite(element,w);
}
for(const element of value["driver_states"]){
Ros1Scout_msgsScoutDriverStateWrite(element,w);
}
w.v.setUint8(w.at,value["light_control_enabled"]?1:0,true);w.at+=1;
Ros1Scout_msgsScoutLightStateWrite(value["front_light_state"],w);
Ros1Scout_msgsScoutLightStateWrite(value["rear_light_state"],w);
}
function Ros1Scout_msgsScoutStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
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
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lateral_velocity"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["base_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["control_mode"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["fault_code"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_voltage"]=decoded;
}
{
{const count=4;if(count>4)bad();value["motor_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Scout_msgsScoutMotorStateRead(r);
value["motor_states"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["driver_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Scout_msgsScoutDriverStateRead(r);
value["driver_states"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["light_control_enabled"]=decoded!==0;
}
{
const decoded=Ros1Scout_msgsScoutLightStateRead(r);
value["front_light_state"]=decoded;
}
{
const decoded=Ros1Scout_msgsScoutLightStateRead(r);
value["rear_light_state"]=decoded;
}
return value;}
function Ros1Scout_msgsScoutStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["linear_velocity"];
out["linear_velocity"]=field;
}
{const field=value["angular_velocity"];
out["angular_velocity"]=field;
}
{const field=value["lateral_velocity"];
out["lateral_velocity"]=field;
}
{const field=value["base_state"];
out["base_state"]=field;
}
{const field=value["control_mode"];
out["control_mode"]=field;
}
{const field=value["fault_code"];
out["fault_code"]=field;
}
{const field=value["battery_voltage"];
out["battery_voltage"]=field;
}
{const field=value["motor_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Scout_msgsScoutMotorStateCapture(element);
}out["motor_states"]=values;
}
{const field=value["driver_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Scout_msgsScoutDriverStateCapture(element);
}out["driver_states"]=values;
}
{const field=value["light_control_enabled"];
out["light_control_enabled"]=field;
}
{const field=value["front_light_state"];
out["front_light_state"]=Ros1Scout_msgsScoutLightStateCapture(field);
}
{const field=value["rear_light_state"];
out["rear_light_state"]=Ros1Scout_msgsScoutLightStateCapture(field);
}
return out;}
