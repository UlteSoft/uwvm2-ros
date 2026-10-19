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
/** Complete ROS1 geometry_msgs/Transform codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsTransformCodec=Object.freeze({codecIdentity:"UWVMROS1_61aabab03e8f766ea05949c9e5f4989186d72a26f793b8d11752e53f1f300085",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"translation":Ros1Geometry_msgsVector3Codec.defaultValue(),"rotation":Ros1Geometry_msgsQuaternionCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsTransformSize(Ros1Geometry_msgsTransformCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsTransformCapture(value),n=Ros1Geometry_msgsTransformSize(owned),w=new Writer(n);Ros1Geometry_msgsTransformWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Geometry_msgsTransformRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsTransformSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsVector3Size(value["translation"]));
n=addSize(n,Ros1Geometry_msgsQuaternionSize(value["rotation"]));
if(n>56)bad(90);return n;}
function Ros1Geometry_msgsTransformWrite(value,w){
Ros1Geometry_msgsVector3Write(value["translation"],w);
Ros1Geometry_msgsQuaternionWrite(value["rotation"],w);
}
function Ros1Geometry_msgsTransformRead(r){const value={};
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["translation"]=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternionRead(r);
value["rotation"]=decoded;
}
return value;}
function Ros1Geometry_msgsTransformCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["translation"];
out["translation"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["rotation"];
out["rotation"]=Ros1Geometry_msgsQuaternionCapture(field);
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
/** Complete ROS1 geometry_msgs/TransformStamped codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsTransformStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_9237b5dc5d3beacf4310244ce746a4210b9864855b611d1bf54cf23c28d7b79a",maxWireBytes:460,minWireBytes:76,cartesianMaxWireBytes:460n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"child_frame_id":"","transform":Ros1Geometry_msgsTransformCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsTransformStampedSize(Ros1Geometry_msgsTransformStampedCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsTransformStampedCapture(value),n=Ros1Geometry_msgsTransformStampedSize(owned),w=new Writer(n);Ros1Geometry_msgsTransformStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,460,90);if(owned.length<76)bad();const r=new Reader(owned),value=Ros1Geometry_msgsTransformStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsTransformStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],256).length);
n=addSize(n,Ros1Geometry_msgsTransformSize(value["transform"]));
if(n>460)bad(90);return n;}
function Ros1Geometry_msgsTransformStampedWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],256);
Ros1Geometry_msgsTransformWrite(value["transform"],w);
}
function Ros1Geometry_msgsTransformStampedRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["child_frame_id"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTransformRead(r);
value["transform"]=decoded;
}
return value;}
function Ros1Geometry_msgsTransformStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["child_frame_id"];
out["child_frame_id"]=field;
}
{const field=value["transform"];
out["transform"]=Ros1Geometry_msgsTransformCapture(field);
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
/** Complete ROS1 geometry_msgs/TwistStamped codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsTwistStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_27a31bcca6a50fbc6c6259b53d229872336a75628d6afd9de174eee2481946c7",maxWireBytes:192,minWireBytes:64,cartesianMaxWireBytes:192n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"twist":Ros1Geometry_msgsTwistCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsTwistStampedSize(Ros1Geometry_msgsTwistStampedCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsTwistStampedCapture(value),n=Ros1Geometry_msgsTwistStampedSize(owned),w=new Writer(n);Ros1Geometry_msgsTwistStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,192,90);if(owned.length<64)bad();const r=new Reader(owned),value=Ros1Geometry_msgsTwistStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsTwistStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Geometry_msgsTwistSize(value["twist"]));
if(n>192)bad(90);return n;}
function Ros1Geometry_msgsTwistStampedWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Geometry_msgsTwistWrite(value["twist"],w);
}
function Ros1Geometry_msgsTwistStampedRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistRead(r);
value["twist"]=decoded;
}
return value;}
function Ros1Geometry_msgsTwistStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["twist"];
out["twist"]=Ros1Geometry_msgsTwistCapture(field);
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
/** Complete ROS1 nav_msgs/Odometry codec; maxima are not actual lengths. */
export const Ros1Nav_msgsOdometryCodec=Object.freeze({codecIdentity:"UWVMROS1_e0ff5eb7aef868891846ac7618249b12c18fcf0969653f24fdeaa7644e636ca0",maxWireBytes:1084,minWireBytes:700,cartesianMaxWireBytes:1084n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"child_frame_id":"","pose":Ros1Geometry_msgsPoseWithCovarianceCodec.defaultValue(),"twist":Ros1Geometry_msgsTwistWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros1Nav_msgsOdometrySize(Ros1Nav_msgsOdometryCapture(value));return true;},
encode(value){const owned=Ros1Nav_msgsOdometryCapture(value),n=Ros1Nav_msgsOdometrySize(owned),w=new Writer(n);Ros1Nav_msgsOdometryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1084,90);if(owned.length<700)bad();const r=new Reader(owned),value=Ros1Nav_msgsOdometryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Nav_msgsOdometrySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],256).length);
n=addSize(n,Ros1Geometry_msgsPoseWithCovarianceSize(value["pose"]));
n=addSize(n,Ros1Geometry_msgsTwistWithCovarianceSize(value["twist"]));
if(n>1084)bad(90);return n;}
function Ros1Nav_msgsOdometryWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],256);
Ros1Geometry_msgsPoseWithCovarianceWrite(value["pose"],w);
Ros1Geometry_msgsTwistWithCovarianceWrite(value["twist"],w);
}
function Ros1Nav_msgsOdometryRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
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
/** Complete ROS1 sensor_msgs/BatteryState codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsBatteryStateCodec=Object.freeze({codecIdentity:"UWVMROS1_4b176902362348cd3b7eec6ae21e2035d33df6990b4682ddf5123ef6e0339441",maxWireBytes:832,minWireBytes:64,cartesianMaxWireBytes:832n,aggregateWireBytes:0,constants:Object.freeze({"POWER_SUPPLY_STATUS_UNKNOWN":0,"POWER_SUPPLY_STATUS_CHARGING":1,"POWER_SUPPLY_STATUS_DISCHARGING":2,"POWER_SUPPLY_STATUS_NOT_CHARGING":3,"POWER_SUPPLY_STATUS_FULL":4,"POWER_SUPPLY_HEALTH_UNKNOWN":0,"POWER_SUPPLY_HEALTH_GOOD":1,"POWER_SUPPLY_HEALTH_OVERHEAT":2,"POWER_SUPPLY_HEALTH_DEAD":3,"POWER_SUPPLY_HEALTH_OVERVOLTAGE":4,"POWER_SUPPLY_HEALTH_UNSPEC_FAILURE":5,"POWER_SUPPLY_HEALTH_COLD":6,"POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE":7,"POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE":8,"POWER_SUPPLY_TECHNOLOGY_UNKNOWN":0,"POWER_SUPPLY_TECHNOLOGY_NIMH":1,"POWER_SUPPLY_TECHNOLOGY_LION":2,"POWER_SUPPLY_TECHNOLOGY_LIPO":3,"POWER_SUPPLY_TECHNOLOGY_LIFE":4,"POWER_SUPPLY_TECHNOLOGY_NICD":5,"POWER_SUPPLY_TECHNOLOGY_LIMN":6}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"voltage":0,"temperature":0,"current":0,"charge":0,"capacity":0,"design_capacity":0,"percentage":0,"power_supply_status":0,"power_supply_health":0,"power_supply_technology":0,"present":false,"cell_voltage":[],"cell_temperature":[],"location":"","serial_number":""};},
validateValue(value){Ros1Sensor_msgsBatteryStateSize(Ros1Sensor_msgsBatteryStateCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsBatteryStateCapture(value),n=Ros1Sensor_msgsBatteryStateSize(owned),w=new Writer(n);Ros1Sensor_msgsBatteryStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,832,90);if(owned.length<64)bad();const r=new Reader(owned),value=Ros1Sensor_msgsBatteryStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsBatteryStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
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
if(n>832)bad(90);return n;}
function Ros1Sensor_msgsBatteryStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
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
function Ros1Sensor_msgsBatteryStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
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
function Ros1Sensor_msgsBatteryStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
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
/** Complete ROS1 sensor_msgs/Imu codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsImuCodec=Object.freeze({codecIdentity:"UWVMROS1_de9c3f5bb37e1dc20be73cb44f614a9ff8645c4cfbc305dc18425d8d9f130bc3",maxWireBytes:440,minWireBytes:312,cartesianMaxWireBytes:440n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"orientation":Ros1Geometry_msgsQuaternionCodec.defaultValue(),"orientation_covariance":Array.from({length:9},()=>(0)),"angular_velocity":Ros1Geometry_msgsVector3Codec.defaultValue(),"angular_velocity_covariance":Array.from({length:9},()=>(0)),"linear_acceleration":Ros1Geometry_msgsVector3Codec.defaultValue(),"linear_acceleration_covariance":Array.from({length:9},()=>(0))};},
validateValue(value){Ros1Sensor_msgsImuSize(Ros1Sensor_msgsImuCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsImuCapture(value),n=Ros1Sensor_msgsImuSize(owned),w=new Writer(n);Ros1Sensor_msgsImuWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,440,90);if(owned.length<312)bad();const r=new Reader(owned),value=Ros1Sensor_msgsImuRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsImuSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Geometry_msgsQuaternionSize(value["orientation"]));
if(!Array.isArray(value["orientation_covariance"])||value["orientation_covariance"].length!==9)bad();
for(const element of value["orientation_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros1Geometry_msgsVector3Size(value["angular_velocity"]));
if(!Array.isArray(value["angular_velocity_covariance"])||value["angular_velocity_covariance"].length!==9)bad();
for(const element of value["angular_velocity_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros1Geometry_msgsVector3Size(value["linear_acceleration"]));
if(!Array.isArray(value["linear_acceleration_covariance"])||value["linear_acceleration_covariance"].length!==9)bad();
for(const element of value["linear_acceleration_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>440)bad(90);return n;}
function Ros1Sensor_msgsImuWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Geometry_msgsQuaternionWrite(value["orientation"],w);
for(const element of value["orientation_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros1Geometry_msgsVector3Write(value["angular_velocity"],w);
for(const element of value["angular_velocity_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros1Geometry_msgsVector3Write(value["linear_acceleration"],w);
for(const element of value["linear_acceleration_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Sensor_msgsImuRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternionRead(r);
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
const decoded=Ros1Geometry_msgsVector3Read(r);
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
const decoded=Ros1Geometry_msgsVector3Read(r);
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
function Ros1Sensor_msgsImuCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["orientation"];
out["orientation"]=Ros1Geometry_msgsQuaternionCapture(field);
}
{const field=value["orientation_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["orientation_covariance"]=values;
}
{const field=value["angular_velocity"];
out["angular_velocity"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["angular_velocity_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["angular_velocity_covariance"]=values;
}
{const field=value["linear_acceleration"];
out["linear_acceleration"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["linear_acceleration_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["linear_acceleration_covariance"]=values;
}
return out;}
/** Complete ROS1 sensor_msgs/JointState codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_f27093d33cca1405ec35a9e525a95aa4002421a579cb78c6da3be3719f3cb66e",maxWireBytes:472,minWireBytes:32,cartesianMaxWireBytes:472n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"name":[],"position":[],"velocity":[],"effort":[]};},
validateValue(value){Ros1Sensor_msgsJointStateSize(Ros1Sensor_msgsJointStateCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsJointStateCapture(value),n=Ros1Sensor_msgsJointStateSize(owned),w=new Writer(n);Ros1Sensor_msgsJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,472,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros1Sensor_msgsJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
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
if(n>472)bad(90);return n;}
function Ros1Sensor_msgsJointStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
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
function Ros1Sensor_msgsJointStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
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
function Ros1Sensor_msgsJointStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
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
/** Complete ROS1 sensor_msgs/LaserScan codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsLaserScanCodec=Object.freeze({codecIdentity:"UWVMROS1_f7aee00335bf7d165004957149192f58abe5a27b5c4884bdf8d1ae9be06b366b",maxWireBytes:8372,minWireBytes:52,cartesianMaxWireBytes:8372n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"angle_min":0,"angle_max":0,"angle_increment":0,"time_increment":0,"scan_time":0,"range_min":0,"range_max":0,"ranges":[],"intensities":[]};},
validateValue(value){Ros1Sensor_msgsLaserScanSize(Ros1Sensor_msgsLaserScanCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsLaserScanCapture(value),n=Ros1Sensor_msgsLaserScanSize(owned),w=new Writer(n);Ros1Sensor_msgsLaserScanWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8372,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Sensor_msgsLaserScanRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsLaserScanSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
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
if(n>8372)bad(90);return n;}
function Ros1Sensor_msgsLaserScanWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
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
function Ros1Sensor_msgsLaserScanRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
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
function Ros1Sensor_msgsLaserScanCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
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
/** Complete ROS1 sensor_msgs/MagneticField codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsMagneticFieldCodec=Object.freeze({codecIdentity:"UWVMROS1_1817b7ce8903797288ccf293373f81890c75af59d03d948339943a531ab754fe",maxWireBytes:240,minWireBytes:112,cartesianMaxWireBytes:240n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"magnetic_field":Ros1Geometry_msgsVector3Codec.defaultValue(),"magnetic_field_covariance":Array.from({length:9},()=>(0))};},
validateValue(value){Ros1Sensor_msgsMagneticFieldSize(Ros1Sensor_msgsMagneticFieldCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsMagneticFieldCapture(value),n=Ros1Sensor_msgsMagneticFieldSize(owned),w=new Writer(n);Ros1Sensor_msgsMagneticFieldWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,240,90);if(owned.length<112)bad();const r=new Reader(owned),value=Ros1Sensor_msgsMagneticFieldRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsMagneticFieldSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Geometry_msgsVector3Size(value["magnetic_field"]));
if(!Array.isArray(value["magnetic_field_covariance"])||value["magnetic_field_covariance"].length!==9)bad();
for(const element of value["magnetic_field_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>240)bad(90);return n;}
function Ros1Sensor_msgsMagneticFieldWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Geometry_msgsVector3Write(value["magnetic_field"],w);
for(const element of value["magnetic_field_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Sensor_msgsMagneticFieldRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3Read(r);
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
function Ros1Sensor_msgsMagneticFieldCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["magnetic_field"];
out["magnetic_field"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["magnetic_field_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["magnetic_field_covariance"]=values;
}
return out;}
/** Complete ROS1 std_srvs/SetBoolRequest codec; maxima are not actual lengths. */
export const Ros1Std_srvsSetBoolRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1621dbb16e2455a199cc871a0e784642a73bc57e76a4d524778163fab97f6aac",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":false};},
validateValue(value){Ros1Std_srvsSetBoolRequestSize(Ros1Std_srvsSetBoolRequestCapture(value));return true;},
encode(value){const owned=Ros1Std_srvsSetBoolRequestCapture(value),n=Ros1Std_srvsSetBoolRequestSize(owned),w=new Writer(n);Ros1Std_srvsSetBoolRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Std_srvsSetBoolRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_srvsSetBoolRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["data"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Std_srvsSetBoolRequestWrite(value,w){
w.v.setUint8(w.at,value["data"]?1:0,true);w.at+=1;
}
function Ros1Std_srvsSetBoolRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["data"]=decoded!==0;
}
return value;}
function Ros1Std_srvsSetBoolRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 std_srvs/SetBoolResponse codec; maxima are not actual lengths. */
export const Ros1Std_srvsSetBoolResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1e12b91618f540070e34e170e12db7659f8686cdf5fb8e1bf098b8d483522cd6",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Std_srvsSetBoolResponseSize(Ros1Std_srvsSetBoolResponseCapture(value));return true;},
encode(value){const owned=Ros1Std_srvsSetBoolResponseCapture(value),n=Ros1Std_srvsSetBoolResponseSize(owned),w=new Writer(n);Ros1Std_srvsSetBoolResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Std_srvsSetBoolResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_srvsSetBoolResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Std_srvsSetBoolResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Std_srvsSetBoolResponseRead(r){const value={};
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
function Ros1Std_srvsSetBoolResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 std_srvs/TriggerRequest codec; maxima are not actual lengths. */
export const Ros1Std_srvsTriggerRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d4e98dca9dc9a031d6b0f51937613f4f29c36156f0f05475339149794d47db1e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Std_srvsTriggerRequestSize(Ros1Std_srvsTriggerRequestCapture(value));return true;},
encode(value){const owned=Ros1Std_srvsTriggerRequestCapture(value),n=Ros1Std_srvsTriggerRequestSize(owned),w=new Writer(n);Ros1Std_srvsTriggerRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Std_srvsTriggerRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_srvsTriggerRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Std_srvsTriggerRequestWrite(value,w){
}
function Ros1Std_srvsTriggerRequestRead(r){const value={};
return value;}
function Ros1Std_srvsTriggerRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 std_srvs/TriggerResponse codec; maxima are not actual lengths. */
export const Ros1Std_srvsTriggerResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e625281ddea64aff38d8647e2673b418b77474cae0501500b280cabb3d247d1d",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Std_srvsTriggerResponseSize(Ros1Std_srvsTriggerResponseCapture(value));return true;},
encode(value){const owned=Ros1Std_srvsTriggerResponseCapture(value),n=Ros1Std_srvsTriggerResponseSize(owned),w=new Writer(n);Ros1Std_srvsTriggerResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Std_srvsTriggerResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_srvsTriggerResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Std_srvsTriggerResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Std_srvsTriggerResponseRead(r){const value={};
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
function Ros1Std_srvsTriggerResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 turtlebot3_example/Turtlebot3Goal codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_exampleTurtlebot3GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_f6eb642301e239005c1790cfd5b9ff00388d98d27a17cbb0edb103b6ac7bfad5",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"goal":Ros1Geometry_msgsVector3Codec.defaultValue()};},
validateValue(value){Ros1Turtlebot3_exampleTurtlebot3GoalSize(Ros1Turtlebot3_exampleTurtlebot3GoalCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_exampleTurtlebot3GoalCapture(value),n=Ros1Turtlebot3_exampleTurtlebot3GoalSize(owned),w=new Writer(n);Ros1Turtlebot3_exampleTurtlebot3GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Turtlebot3_exampleTurtlebot3GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_exampleTurtlebot3GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsVector3Size(value["goal"]));
if(n>24)bad(90);return n;}
function Ros1Turtlebot3_exampleTurtlebot3GoalWrite(value,w){
Ros1Geometry_msgsVector3Write(value["goal"],w);
}
function Ros1Turtlebot3_exampleTurtlebot3GoalRead(r){const value={};
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["goal"]=decoded;
}
return value;}
function Ros1Turtlebot3_exampleTurtlebot3GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["goal"];
out["goal"]=Ros1Geometry_msgsVector3Capture(field);
}
return out;}
/** Complete ROS1 turtlebot3_example/Turtlebot3Result codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_exampleTurtlebot3ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_c749a56ed9a0c74d1b167f69cc9b4435f9549d49144ef2951a4c3db501bad528",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":""};},
validateValue(value){Ros1Turtlebot3_exampleTurtlebot3ResultSize(Ros1Turtlebot3_exampleTurtlebot3ResultCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_exampleTurtlebot3ResultCapture(value),n=Ros1Turtlebot3_exampleTurtlebot3ResultSize(owned),w=new Writer(n);Ros1Turtlebot3_exampleTurtlebot3ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Turtlebot3_exampleTurtlebot3ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_exampleTurtlebot3ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["result"],256).length);
if(n>260)bad(90);return n;}
function Ros1Turtlebot3_exampleTurtlebot3ResultWrite(value,w){
w.text(value["result"],256);
}
function Ros1Turtlebot3_exampleTurtlebot3ResultRead(r){const value={};
{
const decoded=r.text(256);
value["result"]=decoded;
}
return value;}
function Ros1Turtlebot3_exampleTurtlebot3ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS1 turtlebot3_example/Turtlebot3Feedback codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_exampleTurtlebot3FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_077b5593a1e4c23c89c4d6a87dc1d67d8fa1bf04fa3aa440f54c8eda60b688f8",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":""};},
validateValue(value){Ros1Turtlebot3_exampleTurtlebot3FeedbackSize(Ros1Turtlebot3_exampleTurtlebot3FeedbackCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_exampleTurtlebot3FeedbackCapture(value),n=Ros1Turtlebot3_exampleTurtlebot3FeedbackSize(owned),w=new Writer(n);Ros1Turtlebot3_exampleTurtlebot3FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Turtlebot3_exampleTurtlebot3FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_exampleTurtlebot3FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>260)bad(90);return n;}
function Ros1Turtlebot3_exampleTurtlebot3FeedbackWrite(value,w){
w.text(value["state"],256);
}
function Ros1Turtlebot3_exampleTurtlebot3FeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros1Turtlebot3_exampleTurtlebot3FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 turtlebot3_msgs/SensorState codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_msgsSensorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_0ef7dd1a4b965ae027b081a6b34f111731a3adfbabcc6fd547d4da7378fcb81f",maxWireBytes:172,minWireBytes:44,cartesianMaxWireBytes:172n,aggregateWireBytes:0,constants:Object.freeze({"BUMPER_FORWARD":1,"BUMPER_BACKWARD":2,"CLIFF":1,"SONAR":1,"ILLUMINATION":1,"BUTTON0":1,"BUTTON1":2,"ERROR_LEFT_MOTOR":1,"ERROR_RIGHT_MOTOR":2,"TORQUE_ON":1,"TORQUE_OFF":2}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"bumper":0,"cliff":0,"sonar":0,"illumination":0,"led":0,"button":0,"torque":false,"left_encoder":0,"right_encoder":0,"battery":0};},
validateValue(value){Ros1Turtlebot3_msgsSensorStateSize(Ros1Turtlebot3_msgsSensorStateCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_msgsSensorStateCapture(value),n=Ros1Turtlebot3_msgsSensorStateSize(owned),w=new Writer(n);Ros1Turtlebot3_msgsSensorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,172,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Turtlebot3_msgsSensorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_msgsSensorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
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
if(n>172)bad(90);return n;}
function Ros1Turtlebot3_msgsSensorStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
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
function Ros1Turtlebot3_msgsSensorStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
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
function Ros1Turtlebot3_msgsSensorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
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
/** Complete ROS1 turtlebot3_msgs/Sound codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_msgsSoundCodec=Object.freeze({codecIdentity:"UWVMROS1_0e5af3d5a2acbfff5bf5ba94c1520490fe281e70aff02cfb6ad31acd89e7536a",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"OFF":0,"ON":1,"LOW_BATTERY":2,"ERROR":3,"BUTTON1":4,"BUTTON2":5}),
defaultValue(){return {"value":0};},
validateValue(value){Ros1Turtlebot3_msgsSoundSize(Ros1Turtlebot3_msgsSoundCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_msgsSoundCapture(value),n=Ros1Turtlebot3_msgsSoundSize(owned),w=new Writer(n);Ros1Turtlebot3_msgsSoundWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Turtlebot3_msgsSoundRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_msgsSoundSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Turtlebot3_msgsSoundWrite(value,w){
w.v.setUint8(w.at,value["value"],true);w.at+=1;
}
function Ros1Turtlebot3_msgsSoundRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros1Turtlebot3_msgsSoundCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 turtlebot3_msgs/VersionInfo codec; maxima are not actual lengths. */
export const Ros1Turtlebot3_msgsVersionInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_8f41d37cf43fda2e3649d85fcca83ad735dc5a8122f7829a98b18659bcad8b3a",maxWireBytes:780,minWireBytes:12,cartesianMaxWireBytes:780n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"hardware":"","firmware":"","software":""};},
validateValue(value){Ros1Turtlebot3_msgsVersionInfoSize(Ros1Turtlebot3_msgsVersionInfoCapture(value));return true;},
encode(value){const owned=Ros1Turtlebot3_msgsVersionInfoCapture(value),n=Ros1Turtlebot3_msgsVersionInfoSize(owned),w=new Writer(n);Ros1Turtlebot3_msgsVersionInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,780,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Turtlebot3_msgsVersionInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Turtlebot3_msgsVersionInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["hardware"],256).length);
n=addSize(n,4+textBytes(value["firmware"],256).length);
n=addSize(n,4+textBytes(value["software"],256).length);
if(n>780)bad(90);return n;}
function Ros1Turtlebot3_msgsVersionInfoWrite(value,w){
w.text(value["hardware"],256);
w.text(value["firmware"],256);
w.text(value["software"],256);
}
function Ros1Turtlebot3_msgsVersionInfoRead(r){const value={};
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
function Ros1Turtlebot3_msgsVersionInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
