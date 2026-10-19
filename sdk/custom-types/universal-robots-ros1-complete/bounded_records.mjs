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
/** Complete ROS1 geometry_msgs/PoseStamped codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsPoseStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_bb59c4e82f561266c0f10500bfbbd1a809e9c1122ecbf559a812326af3eb2e95",maxWireBytes:200,minWireBytes:72,cartesianMaxWireBytes:200n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"pose":Ros1Geometry_msgsPoseCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsPoseStampedSize(Ros1Geometry_msgsPoseStampedCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsPoseStampedCapture(value),n=Ros1Geometry_msgsPoseStampedSize(owned),w=new Writer(n);Ros1Geometry_msgsPoseStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,200,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros1Geometry_msgsPoseStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsPoseStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Geometry_msgsPoseSize(value["pose"]));
if(n>200)bad(90);return n;}
function Ros1Geometry_msgsPoseStampedWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Geometry_msgsPoseWrite(value["pose"],w);
}
function Ros1Geometry_msgsPoseStampedRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseRead(r);
value["pose"]=decoded;
}
return value;}
function Ros1Geometry_msgsPoseStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseCapture(field);
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
/** Complete ROS1 geometry_msgs/Wrench codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsWrenchCodec=Object.freeze({codecIdentity:"UWVMROS1_b0938c4e084df68cf7c46445070ac3e9305d06f678275845aec6055db77617c3",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"force":Ros1Geometry_msgsVector3Codec.defaultValue(),"torque":Ros1Geometry_msgsVector3Codec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsWrenchSize(Ros1Geometry_msgsWrenchCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsWrenchCapture(value),n=Ros1Geometry_msgsWrenchSize(owned),w=new Writer(n);Ros1Geometry_msgsWrenchWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros1Geometry_msgsWrenchRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsWrenchSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsVector3Size(value["force"]));
n=addSize(n,Ros1Geometry_msgsVector3Size(value["torque"]));
if(n>48)bad(90);return n;}
function Ros1Geometry_msgsWrenchWrite(value,w){
Ros1Geometry_msgsVector3Write(value["force"],w);
Ros1Geometry_msgsVector3Write(value["torque"],w);
}
function Ros1Geometry_msgsWrenchRead(r){const value={};
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["force"]=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["torque"]=decoded;
}
return value;}
function Ros1Geometry_msgsWrenchCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["force"];
out["force"]=Ros1Geometry_msgsVector3Capture(field);
}
{const field=value["torque"];
out["torque"]=Ros1Geometry_msgsVector3Capture(field);
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/AddToLogRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsAddToLogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_faf6c1ab9c749a05ae3f4e6196cc1c3574560daae1463ebe8684bf13ea938ac4",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Ur_dashboard_msgsAddToLogRequestSize(Ros1Ur_dashboard_msgsAddToLogRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsAddToLogRequestCapture(value),n=Ros1Ur_dashboard_msgsAddToLogRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsAddToLogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsAddToLogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsAddToLogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsAddToLogRequestWrite(value,w){
w.text(value["message"],256);
}
function Ros1Ur_dashboard_msgsAddToLogRequestRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsAddToLogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/AddToLogResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsAddToLogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d495f7609d04abde44cb35ec5114618a2875e0a30986d2f7cc62c8ce99354e7c",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsAddToLogResponseSize(Ros1Ur_dashboard_msgsAddToLogResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsAddToLogResponseCapture(value),n=Ros1Ur_dashboard_msgsAddToLogResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsAddToLogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsAddToLogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsAddToLogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros1Ur_dashboard_msgsAddToLogResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsAddToLogResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsAddToLogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetLoadedProgramRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetLoadedProgramRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_54eecd0b652707f9c16dcbacbef05902112db993500404e1adaf26618e1d3a1a",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsGetLoadedProgramRequestSize(Ros1Ur_dashboard_msgsGetLoadedProgramRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetLoadedProgramRequestCapture(value),n=Ros1Ur_dashboard_msgsGetLoadedProgramRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetLoadedProgramRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetLoadedProgramRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetLoadedProgramRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetLoadedProgramRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsGetLoadedProgramRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsGetLoadedProgramRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetLoadedProgramResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetLoadedProgramResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1680747345d40412a4731c48f08c402ac97fb12dd871fb320c2a9d002453f87a",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_name":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsGetLoadedProgramResponseSize(Ros1Ur_dashboard_msgsGetLoadedProgramResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetLoadedProgramResponseCapture(value),n=Ros1Ur_dashboard_msgsGetLoadedProgramResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetLoadedProgramResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetLoadedProgramResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetLoadedProgramResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["program_name"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetLoadedProgramResponseWrite(value,w){
w.text(value["answer"],256);
w.text(value["program_name"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsGetLoadedProgramResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsGetLoadedProgramResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_dashboard_msgs/GetProgramStateRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetProgramStateRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cf14c6f86ef1904f80e88117dc2f2d29a003b52e21bc6569795771f04cf072a2",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsGetProgramStateRequestSize(Ros1Ur_dashboard_msgsGetProgramStateRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetProgramStateRequestCapture(value),n=Ros1Ur_dashboard_msgsGetProgramStateRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetProgramStateRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetProgramStateRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetProgramStateRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetProgramStateRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsGetProgramStateRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsGetProgramStateRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/ProgramState codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsProgramStateCodec=Object.freeze({codecIdentity:"UWVMROS1_97a57db64f20abcb9929b5296c75d66d994f2e9e26449810197c657694d3850e",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({"STOPPED":"STOPPED","PLAYING":"PLAYING","PAUSED":"PAUSED"}),
defaultValue(){return {"state":""};},
validateValue(value){Ros1Ur_dashboard_msgsProgramStateSize(Ros1Ur_dashboard_msgsProgramStateCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsProgramStateCapture(value),n=Ros1Ur_dashboard_msgsProgramStateSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsProgramStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsProgramStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsProgramStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsProgramStateWrite(value,w){
w.text(value["state"],256);
}
function Ros1Ur_dashboard_msgsProgramStateRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsProgramStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetProgramStateResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetProgramStateResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_366caf89303bfff48d3c0f89aa2a640c3d29af5a0c19d76ceccf26d50c3ab2ed",maxWireBytes:781,minWireBytes:13,cartesianMaxWireBytes:781n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros1Ur_dashboard_msgsProgramStateCodec.defaultValue(),"program_name":"","answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsGetProgramStateResponseSize(Ros1Ur_dashboard_msgsGetProgramStateResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetProgramStateResponseCapture(value),n=Ros1Ur_dashboard_msgsGetProgramStateResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetProgramStateResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,781,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetProgramStateResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetProgramStateResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Ur_dashboard_msgsProgramStateSize(value["state"]));
n=addSize(n,4+textBytes(value["program_name"],256).length);
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>781)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetProgramStateResponseWrite(value,w){
Ros1Ur_dashboard_msgsProgramStateWrite(value["state"],w);
w.text(value["program_name"],256);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsGetProgramStateResponseRead(r){const value={};
{
const decoded=Ros1Ur_dashboard_msgsProgramStateRead(r);
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
function Ros1Ur_dashboard_msgsGetProgramStateResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros1Ur_dashboard_msgsProgramStateCapture(field);
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
/** Complete ROS1 ur_dashboard_msgs/GetRobotModeRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetRobotModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b9ffe283d7dbcb85879dbf6964d393f5d44661227bb26e1acc34308edb288609",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsGetRobotModeRequestSize(Ros1Ur_dashboard_msgsGetRobotModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetRobotModeRequestCapture(value),n=Ros1Ur_dashboard_msgsGetRobotModeRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetRobotModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetRobotModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetRobotModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetRobotModeRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsGetRobotModeRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsGetRobotModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/RobotMode codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsRobotModeCodec=Object.freeze({codecIdentity:"UWVMROS1_b2772853eff2ec9b849b6de4a90ce98487a8eff49d1371283dce503908702066",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NO_CONTROLLER":-1,"DISCONNECTED":0,"CONFIRM_SAFETY":1,"BOOTING":2,"POWER_OFF":3,"POWER_ON":4,"IDLE":5,"BACKDRIVE":6,"RUNNING":7,"UPDATING_FIRMWARE":8}),
defaultValue(){return {"mode":0};},
validateValue(value){Ros1Ur_dashboard_msgsRobotModeSize(Ros1Ur_dashboard_msgsRobotModeCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsRobotModeCapture(value),n=Ros1Ur_dashboard_msgsRobotModeSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsRobotModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsRobotModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsRobotModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_dashboard_msgsRobotModeWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
}
function Ros1Ur_dashboard_msgsRobotModeRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["mode"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsRobotModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetRobotModeResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetRobotModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f6c0d0d03a956a34b00fd63540226b6abb8b106e2a35cb912b4886a5c4cecc3f",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robot_mode":Ros1Ur_dashboard_msgsRobotModeCodec.defaultValue(),"answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsGetRobotModeResponseSize(Ros1Ur_dashboard_msgsGetRobotModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetRobotModeResponseCapture(value),n=Ros1Ur_dashboard_msgsGetRobotModeResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetRobotModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetRobotModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetRobotModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Ur_dashboard_msgsRobotModeSize(value["robot_mode"]));
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetRobotModeResponseWrite(value,w){
Ros1Ur_dashboard_msgsRobotModeWrite(value["robot_mode"],w);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsGetRobotModeResponseRead(r){const value={};
{
const decoded=Ros1Ur_dashboard_msgsRobotModeRead(r);
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
function Ros1Ur_dashboard_msgsGetRobotModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robot_mode"];
out["robot_mode"]=Ros1Ur_dashboard_msgsRobotModeCapture(field);
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetSafetyModeRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetSafetyModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f697e2ff2bcd56523563060a46bca2e1b7af487128f05562fcb18bfa73cbca8b",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsGetSafetyModeRequestSize(Ros1Ur_dashboard_msgsGetSafetyModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetSafetyModeRequestCapture(value),n=Ros1Ur_dashboard_msgsGetSafetyModeRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetSafetyModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetSafetyModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetSafetyModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetSafetyModeRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsGetSafetyModeRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsGetSafetyModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/SafetyMode codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsSafetyModeCodec=Object.freeze({codecIdentity:"UWVMROS1_5a0c2737166cf2e2bd4c17b46c597f674b8d806b802d4e86ee0d76fc55f29cf7",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NORMAL":1,"REDUCED":2,"PROTECTIVE_STOP":3,"RECOVERY":4,"SAFEGUARD_STOP":5,"SYSTEM_EMERGENCY_STOP":6,"ROBOT_EMERGENCY_STOP":7,"VIOLATION":8,"FAULT":9,"VALIDATE_JOINT_ID":10,"UNDEFINED_SAFETY_MODE":11,"AUTOMATIC_MODE_SAFEGUARD_STOP":12,"SYSTEM_THREE_POSITION_ENABLING_STOP":13}),
defaultValue(){return {"mode":0};},
validateValue(value){Ros1Ur_dashboard_msgsSafetyModeSize(Ros1Ur_dashboard_msgsSafetyModeCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsSafetyModeCapture(value),n=Ros1Ur_dashboard_msgsSafetyModeSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsSafetyModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsSafetyModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsSafetyModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_dashboard_msgsSafetyModeWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
}
function Ros1Ur_dashboard_msgsSafetyModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsSafetyModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/GetSafetyModeResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsGetSafetyModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_70121af922588eaab9a34855e3da0cde837b1449d2a2d6c28addc70916df1a15",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"safety_mode":Ros1Ur_dashboard_msgsSafetyModeCodec.defaultValue(),"answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsGetSafetyModeResponseSize(Ros1Ur_dashboard_msgsGetSafetyModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsGetSafetyModeResponseCapture(value),n=Ros1Ur_dashboard_msgsGetSafetyModeResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsGetSafetyModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsGetSafetyModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsGetSafetyModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Ur_dashboard_msgsSafetyModeSize(value["safety_mode"]));
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros1Ur_dashboard_msgsGetSafetyModeResponseWrite(value,w){
Ros1Ur_dashboard_msgsSafetyModeWrite(value["safety_mode"],w);
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsGetSafetyModeResponseRead(r){const value={};
{
const decoded=Ros1Ur_dashboard_msgsSafetyModeRead(r);
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
function Ros1Ur_dashboard_msgsGetSafetyModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["safety_mode"];
out["safety_mode"]=Ros1Ur_dashboard_msgsSafetyModeCapture(field);
}
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/IsInRemoteControlRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsInRemoteControlRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4b3c39f36f953904eb582d8c5f91161f2864782d4a4ab209a7dab32a117175da",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsIsInRemoteControlRequestSize(Ros1Ur_dashboard_msgsIsInRemoteControlRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsInRemoteControlRequestCapture(value),n=Ros1Ur_dashboard_msgsIsInRemoteControlRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsInRemoteControlRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsInRemoteControlRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsInRemoteControlRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsInRemoteControlRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsIsInRemoteControlRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsIsInRemoteControlRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/IsInRemoteControlResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsInRemoteControlResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6c4af27571b48a5b8e8f1f857b2c02a333e7b4b360f8f3645a39630594c1663a",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","in_remote_control":false,"success":false};},
validateValue(value){Ros1Ur_dashboard_msgsIsInRemoteControlResponseSize(Ros1Ur_dashboard_msgsIsInRemoteControlResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsInRemoteControlResponseCapture(value),n=Ros1Ur_dashboard_msgsIsInRemoteControlResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsInRemoteControlResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsInRemoteControlResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsInRemoteControlResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["in_remote_control"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsInRemoteControlResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["in_remote_control"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsIsInRemoteControlResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["in_remote_control"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_dashboard_msgsIsInRemoteControlResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["in_remote_control"];
out["in_remote_control"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/IsProgramRunningRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsProgramRunningRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0be481297028886e15f07ace4178ca0aec595724cd83ab01d0155e7c6aa6666f",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsIsProgramRunningRequestSize(Ros1Ur_dashboard_msgsIsProgramRunningRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsProgramRunningRequestCapture(value),n=Ros1Ur_dashboard_msgsIsProgramRunningRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsProgramRunningRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsProgramRunningRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsProgramRunningRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsProgramRunningRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsIsProgramRunningRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsIsProgramRunningRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/IsProgramRunningResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsProgramRunningResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4d9de4f7e7071d142c09e3184d4a6f29d4044d003ad11ba3b72e1129ecf144ac",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_running":false,"success":false};},
validateValue(value){Ros1Ur_dashboard_msgsIsProgramRunningResponseSize(Ros1Ur_dashboard_msgsIsProgramRunningResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsProgramRunningResponseCapture(value),n=Ros1Ur_dashboard_msgsIsProgramRunningResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsProgramRunningResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsProgramRunningResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsProgramRunningResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["program_running"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsProgramRunningResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["program_running"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsIsProgramRunningResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsIsProgramRunningResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_dashboard_msgs/IsProgramSavedRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsProgramSavedRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_45ce1466c35adc812096bfb8b2cd84dfa87940109831c9f797b5786afa14f5a8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_dashboard_msgsIsProgramSavedRequestSize(Ros1Ur_dashboard_msgsIsProgramSavedRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsProgramSavedRequestCapture(value),n=Ros1Ur_dashboard_msgsIsProgramSavedRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsProgramSavedRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsProgramSavedRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsProgramSavedRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsProgramSavedRequestWrite(value,w){
}
function Ros1Ur_dashboard_msgsIsProgramSavedRequestRead(r){const value={};
return value;}
function Ros1Ur_dashboard_msgsIsProgramSavedRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_dashboard_msgs/IsProgramSavedResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsIsProgramSavedResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_eb53e077fe7b216e27fc57f838666a54a84e479e02e508d41df5cff9f0637a52",maxWireBytes:522,minWireBytes:10,cartesianMaxWireBytes:522n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","program_name":"","program_saved":false,"success":false};},
validateValue(value){Ros1Ur_dashboard_msgsIsProgramSavedResponseSize(Ros1Ur_dashboard_msgsIsProgramSavedResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsIsProgramSavedResponseCapture(value),n=Ros1Ur_dashboard_msgsIsProgramSavedResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsIsProgramSavedResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,522,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsIsProgramSavedResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsIsProgramSavedResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
n=addSize(n,4+textBytes(value["program_name"],256).length);
if(typeof value["program_saved"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>522)bad(90);return n;}
function Ros1Ur_dashboard_msgsIsProgramSavedResponseWrite(value,w){
w.text(value["answer"],256);
w.text(value["program_name"],256);
w.v.setUint8(w.at,value["program_saved"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsIsProgramSavedResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsIsProgramSavedResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_dashboard_msgs/LoadRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsLoadRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_dfbde7b2376c7c9ba3926e4803f16b22f7ade3b2f768a25886a1490656edd2bd",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":""};},
validateValue(value){Ros1Ur_dashboard_msgsLoadRequestSize(Ros1Ur_dashboard_msgsLoadRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsLoadRequestCapture(value),n=Ros1Ur_dashboard_msgsLoadRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsLoadRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsLoadRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsLoadRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsLoadRequestWrite(value,w){
w.text(value["filename"],256);
}
function Ros1Ur_dashboard_msgsLoadRequestRead(r){const value={};
{
const decoded=r.text(256);
value["filename"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsLoadRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/LoadResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsLoadResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0ed3b3f2a0066248734e64c0706e52ebf07113569a98baf249a99adfff59be02",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsLoadResponseSize(Ros1Ur_dashboard_msgsLoadResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsLoadResponseCapture(value),n=Ros1Ur_dashboard_msgsLoadResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsLoadResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsLoadResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsLoadResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros1Ur_dashboard_msgsLoadResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsLoadResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsLoadResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/PopupRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsPopupRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_11556bfb811a28141b6c5e9c11720920a389d735b8c09ed1a8f47d05f293e90d",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Ur_dashboard_msgsPopupRequestSize(Ros1Ur_dashboard_msgsPopupRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsPopupRequestCapture(value),n=Ros1Ur_dashboard_msgsPopupRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsPopupRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsPopupRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsPopupRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsPopupRequestWrite(value,w){
w.text(value["message"],256);
}
function Ros1Ur_dashboard_msgsPopupRequestRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsPopupRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/PopupResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsPopupResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_33ac5b2128d6e7c2e562af7e2b8a8e907c16c07034b377920cba4fbd04ca9e14",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":"","success":false};},
validateValue(value){Ros1Ur_dashboard_msgsPopupResponseSize(Ros1Ur_dashboard_msgsPopupResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsPopupResponseCapture(value),n=Ros1Ur_dashboard_msgsPopupResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsPopupResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsPopupResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsPopupResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros1Ur_dashboard_msgsPopupResponseWrite(value,w){
w.text(value["answer"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsPopupResponseRead(r){const value={};
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
function Ros1Ur_dashboard_msgsPopupResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/RawRequestRequest codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsRawRequestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ca8e3fdb45888bf4feb3c0f4ed05b7b9499a74493aaa0a8727f9f513922adfb4",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"query":""};},
validateValue(value){Ros1Ur_dashboard_msgsRawRequestRequestSize(Ros1Ur_dashboard_msgsRawRequestRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsRawRequestRequestCapture(value),n=Ros1Ur_dashboard_msgsRawRequestRequestSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsRawRequestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsRawRequestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsRawRequestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["query"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsRawRequestRequestWrite(value,w){
w.text(value["query"],256);
}
function Ros1Ur_dashboard_msgsRawRequestRequestRead(r){const value={};
{
const decoded=r.text(256);
value["query"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsRawRequestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["query"];
out["query"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/RawRequestResponse codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsRawRequestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_76dbb0c8d0143bd4d39bbf66c6866b0771013fe67fbad76887c6490c91f39e80",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"answer":""};},
validateValue(value){Ros1Ur_dashboard_msgsRawRequestResponseSize(Ros1Ur_dashboard_msgsRawRequestResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsRawRequestResponseCapture(value),n=Ros1Ur_dashboard_msgsRawRequestResponseSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsRawRequestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsRawRequestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsRawRequestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["answer"],256).length);
if(n>260)bad(90);return n;}
function Ros1Ur_dashboard_msgsRawRequestResponseWrite(value,w){
w.text(value["answer"],256);
}
function Ros1Ur_dashboard_msgsRawRequestResponseRead(r){const value={};
{
const decoded=r.text(256);
value["answer"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsRawRequestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["answer"];
out["answer"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/SetModeGoal codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsSetModeGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_27ba58b57af47e8efff15a70fb0562d5904d019c17e39a499bf003730434a528",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_robot_mode":Ros1Ur_dashboard_msgsRobotModeCodec.defaultValue(),"stop_program":false,"play_program":false};},
validateValue(value){Ros1Ur_dashboard_msgsSetModeGoalSize(Ros1Ur_dashboard_msgsSetModeGoalCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsSetModeGoalCapture(value),n=Ros1Ur_dashboard_msgsSetModeGoalSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsSetModeGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsSetModeGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsSetModeGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Ur_dashboard_msgsRobotModeSize(value["target_robot_mode"]));
if(typeof value["stop_program"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["play_program"]!=="boolean")bad();n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Ur_dashboard_msgsSetModeGoalWrite(value,w){
Ros1Ur_dashboard_msgsRobotModeWrite(value["target_robot_mode"],w);
w.v.setUint8(w.at,value["stop_program"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["play_program"]?1:0,true);w.at+=1;
}
function Ros1Ur_dashboard_msgsSetModeGoalRead(r){const value={};
{
const decoded=Ros1Ur_dashboard_msgsRobotModeRead(r);
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
function Ros1Ur_dashboard_msgsSetModeGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_robot_mode"];
out["target_robot_mode"]=Ros1Ur_dashboard_msgsRobotModeCapture(field);
}
{const field=value["stop_program"];
out["stop_program"]=field;
}
{const field=value["play_program"];
out["play_program"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/SetModeResult codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsSetModeResultCodec=Object.freeze({codecIdentity:"UWVMROS1_e558d01061183cabca7022bd66aeb1958e14dfc62226a701c86020e73ffa1114",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Ur_dashboard_msgsSetModeResultSize(Ros1Ur_dashboard_msgsSetModeResultCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsSetModeResultCapture(value),n=Ros1Ur_dashboard_msgsSetModeResultSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsSetModeResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsSetModeResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsSetModeResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Ur_dashboard_msgsSetModeResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Ur_dashboard_msgsSetModeResultRead(r){const value={};
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
function Ros1Ur_dashboard_msgsSetModeResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 ur_dashboard_msgs/SetModeFeedback codec; maxima are not actual lengths. */
export const Ros1Ur_dashboard_msgsSetModeFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_d29b507dcc5d72ee086c8a5a9a7a762d677ed408ec02dbe661abc17e2de5c7e2",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_robot_mode":0,"current_safety_mode":0};},
validateValue(value){Ros1Ur_dashboard_msgsSetModeFeedbackSize(Ros1Ur_dashboard_msgsSetModeFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Ur_dashboard_msgsSetModeFeedbackCapture(value),n=Ros1Ur_dashboard_msgsSetModeFeedbackSize(owned),w=new Writer(n);Ros1Ur_dashboard_msgsSetModeFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros1Ur_dashboard_msgsSetModeFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_dashboard_msgsSetModeFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["current_robot_mode"],1,true);n=addSize(n,1);
integer(value["current_safety_mode"],1,true);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros1Ur_dashboard_msgsSetModeFeedbackWrite(value,w){
w.v.setInt8(w.at,value["current_robot_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["current_safety_mode"],true);w.at+=1;
}
function Ros1Ur_dashboard_msgsSetModeFeedbackRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_robot_mode"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["current_safety_mode"]=decoded;
}
return value;}
function Ros1Ur_dashboard_msgsSetModeFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_robot_mode"];
out["current_robot_mode"]=field;
}
{const field=value["current_safety_mode"];
out["current_safety_mode"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/Analog codec; maxima are not actual lengths. */
export const Ros1Ur_msgsAnalogCodec=Object.freeze({codecIdentity:"UWVMROS1_638dc3b30def8193a501b4c1c778792a60c8c683a96880e9fa6db6c01774f6a7",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"CURRENT":0,"VOLTAGE":1}),
defaultValue(){return {"pin":0,"domain":0,"state":0};},
validateValue(value){Ros1Ur_msgsAnalogSize(Ros1Ur_msgsAnalogCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsAnalogCapture(value),n=Ros1Ur_msgsAnalogSize(owned),w=new Writer(n);Ros1Ur_msgsAnalogWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_msgsAnalogRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsAnalogSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
integer(value["domain"],1,false);n=addSize(n,1);
if(typeof value["state"]!=="number"||!Number.isFinite(value["state"])||!Number.isFinite(Math.fround(value["state"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros1Ur_msgsAnalogWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["domain"],true);w.at+=1;
w.v.setFloat32(w.at,value["state"],true);w.at+=4;
}
function Ros1Ur_msgsAnalogRead(r){const value={};
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
function Ros1Ur_msgsAnalogCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_msgs/Digital codec; maxima are not actual lengths. */
export const Ros1Ur_msgsDigitalCodec=Object.freeze({codecIdentity:"UWVMROS1_c5840cfc5e26fe2f18817e46a0322a92e02ca21983920c71ebeb585769f5622c",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pin":0,"state":false};},
validateValue(value){Ros1Ur_msgsDigitalSize(Ros1Ur_msgsDigitalCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsDigitalCapture(value),n=Ros1Ur_msgsDigitalSize(owned),w=new Writer(n);Ros1Ur_msgsDigitalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros1Ur_msgsDigitalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsDigitalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["pin"],1,false);n=addSize(n,1);
if(typeof value["state"]!=="boolean")bad();n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros1Ur_msgsDigitalWrite(value,w){
w.v.setUint8(w.at,value["pin"],true);w.at+=1;
w.v.setUint8(w.at,value["state"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsDigitalRead(r){const value={};
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
function Ros1Ur_msgsDigitalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pin"];
out["pin"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/GetRobotSoftwareVersionRequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsGetRobotSoftwareVersionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bbcdd98ee8d951a8a4fa873ad1c755f16c5cce14ceb7a614343dd44ccf84b9c8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ur_msgsGetRobotSoftwareVersionRequestSize(Ros1Ur_msgsGetRobotSoftwareVersionRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsGetRobotSoftwareVersionRequestCapture(value),n=Ros1Ur_msgsGetRobotSoftwareVersionRequestSize(owned),w=new Writer(n);Ros1Ur_msgsGetRobotSoftwareVersionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ur_msgsGetRobotSoftwareVersionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsGetRobotSoftwareVersionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ur_msgsGetRobotSoftwareVersionRequestWrite(value,w){
}
function Ros1Ur_msgsGetRobotSoftwareVersionRequestRead(r){const value={};
return value;}
function Ros1Ur_msgsGetRobotSoftwareVersionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ur_msgs/GetRobotSoftwareVersionResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsGetRobotSoftwareVersionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_36ab51ce55e3630f8ca71f364f4c5cc1f4b010f678777e4fd7b03dd480f92e64",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"major":0,"minor":0,"bugfix":0,"build":0};},
validateValue(value){Ros1Ur_msgsGetRobotSoftwareVersionResponseSize(Ros1Ur_msgsGetRobotSoftwareVersionResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsGetRobotSoftwareVersionResponseCapture(value),n=Ros1Ur_msgsGetRobotSoftwareVersionResponseSize(owned),w=new Writer(n);Ros1Ur_msgsGetRobotSoftwareVersionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Ur_msgsGetRobotSoftwareVersionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsGetRobotSoftwareVersionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["major"],4,false);n=addSize(n,4);
integer(value["minor"],4,false);n=addSize(n,4);
integer(value["bugfix"],4,false);n=addSize(n,4);
integer(value["build"],4,false);n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Ur_msgsGetRobotSoftwareVersionResponseWrite(value,w){
w.v.setUint32(w.at,value["major"],true);w.at+=4;
w.v.setUint32(w.at,value["minor"],true);w.at+=4;
w.v.setUint32(w.at,value["bugfix"],true);w.at+=4;
w.v.setUint32(w.at,value["build"],true);w.at+=4;
}
function Ros1Ur_msgsGetRobotSoftwareVersionResponseRead(r){const value={};
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
function Ros1Ur_msgsGetRobotSoftwareVersionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_msgs/IOStates codec; maxima are not actual lengths. */
export const Ros1Ur_msgsIOStatesCodec=Object.freeze({codecIdentity:"UWVMROS1_4b10f6efdd2f129267c407bc1f8caed03776fef6d49f0a1b9dfbf2451ad8f033",maxWireBytes:404,minWireBytes:20,cartesianMaxWireBytes:404n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"digital_in_states":[],"digital_out_states":[],"flag_states":[],"analog_in_states":[],"analog_out_states":[]};},
validateValue(value){Ros1Ur_msgsIOStatesSize(Ros1Ur_msgsIOStatesCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsIOStatesCapture(value),n=Ros1Ur_msgsIOStatesSize(owned),w=new Writer(n);Ros1Ur_msgsIOStatesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,404,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Ur_msgsIOStatesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsIOStatesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["digital_in_states"])||value["digital_in_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_in_states"]){
n=addSize(n,Ros1Ur_msgsDigitalSize(element));
}
if(!Array.isArray(value["digital_out_states"])||value["digital_out_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["digital_out_states"]){
n=addSize(n,Ros1Ur_msgsDigitalSize(element));
}
if(!Array.isArray(value["flag_states"])||value["flag_states"].length>32)bad();
n=addSize(n,4);
for(const element of value["flag_states"]){
n=addSize(n,Ros1Ur_msgsDigitalSize(element));
}
if(!Array.isArray(value["analog_in_states"])||value["analog_in_states"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_in_states"]){
n=addSize(n,Ros1Ur_msgsAnalogSize(element));
}
if(!Array.isArray(value["analog_out_states"])||value["analog_out_states"].length>16)bad();
n=addSize(n,4);
for(const element of value["analog_out_states"]){
n=addSize(n,Ros1Ur_msgsAnalogSize(element));
}
if(n>404)bad(90);return n;}
function Ros1Ur_msgsIOStatesWrite(value,w){
w.v.setUint32(w.at,value["digital_in_states"].length,true);w.at+=4;
for(const element of value["digital_in_states"]){
Ros1Ur_msgsDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["digital_out_states"].length,true);w.at+=4;
for(const element of value["digital_out_states"]){
Ros1Ur_msgsDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["flag_states"].length,true);w.at+=4;
for(const element of value["flag_states"]){
Ros1Ur_msgsDigitalWrite(element,w);
}
w.v.setUint32(w.at,value["analog_in_states"].length,true);w.at+=4;
for(const element of value["analog_in_states"]){
Ros1Ur_msgsAnalogWrite(element,w);
}
w.v.setUint32(w.at,value["analog_out_states"].length,true);w.at+=4;
for(const element of value["analog_out_states"]){
Ros1Ur_msgsAnalogWrite(element,w);
}
}
function Ros1Ur_msgsIOStatesRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["digital_in_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Ur_msgsDigitalRead(r);
value["digital_in_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["digital_out_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Ur_msgsDigitalRead(r);
value["digital_out_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["flag_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Ur_msgsDigitalRead(r);
value["flag_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["analog_in_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Ur_msgsAnalogRead(r);
value["analog_in_states"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["analog_out_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Ur_msgsAnalogRead(r);
value["analog_out_states"].push(decoded);
}}
}
return value;}
function Ros1Ur_msgsIOStatesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["digital_in_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Ur_msgsDigitalCapture(element);
}out["digital_in_states"]=values;
}
{const field=value["digital_out_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Ur_msgsDigitalCapture(element);
}out["digital_out_states"]=values;
}
{const field=value["flag_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Ur_msgsDigitalCapture(element);
}out["flag_states"]=values;
}
{const field=value["analog_in_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Ur_msgsAnalogCapture(element);
}out["analog_in_states"]=values;
}
{const field=value["analog_out_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Ur_msgsAnalogCapture(element);
}out["analog_out_states"]=values;
}
return out;}
/** Complete ROS1 ur_msgs/MasterboardDataMsg codec; maxima are not actual lengths. */
export const Ros1Ur_msgsMasterboardDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_f80f92b90cb33dd5b118df81447dfcc33c1ab230a91bb915a1be684401da265f",maxWireBytes:62,minWireBytes:62,cartesianMaxWireBytes:62n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"digital_input_bits":0,"digital_output_bits":0,"analog_input_range0":0,"analog_input_range1":0,"analog_input0":0,"analog_input1":0,"analog_output_domain0":0,"analog_output_domain1":0,"analog_output0":0,"analog_output1":0,"masterboard_temperature":0,"robot_voltage_48V":0,"robot_current":0,"master_io_current":0,"master_safety_state":0,"master_onoff_state":0};},
validateValue(value){Ros1Ur_msgsMasterboardDataMsgSize(Ros1Ur_msgsMasterboardDataMsgCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsMasterboardDataMsgCapture(value),n=Ros1Ur_msgsMasterboardDataMsgSize(owned),w=new Writer(n);Ros1Ur_msgsMasterboardDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,62,90);if(owned.length<62)bad();const r=new Reader(owned),value=Ros1Ur_msgsMasterboardDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsMasterboardDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
if(typeof value["robot_voltage_48V"]!=="number"||!Number.isFinite(value["robot_voltage_48V"])||!Number.isFinite(Math.fround(value["robot_voltage_48V"])))bad();n=addSize(n,4);
if(typeof value["robot_current"]!=="number"||!Number.isFinite(value["robot_current"])||!Number.isFinite(Math.fround(value["robot_current"])))bad();n=addSize(n,4);
if(typeof value["master_io_current"]!=="number"||!Number.isFinite(value["master_io_current"])||!Number.isFinite(Math.fround(value["master_io_current"])))bad();n=addSize(n,4);
integer(value["master_safety_state"],1,false);n=addSize(n,1);
integer(value["master_onoff_state"],1,false);n=addSize(n,1);
if(n>62)bad(90);return n;}
function Ros1Ur_msgsMasterboardDataMsgWrite(value,w){
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
w.v.setFloat32(w.at,value["robot_voltage_48V"],true);w.at+=4;
w.v.setFloat32(w.at,value["robot_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["master_io_current"],true);w.at+=4;
w.v.setUint8(w.at,value["master_safety_state"],true);w.at+=1;
w.v.setUint8(w.at,value["master_onoff_state"],true);w.at+=1;
}
function Ros1Ur_msgsMasterboardDataMsgRead(r){const value={};
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
value["robot_voltage_48V"]=decoded;
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
function Ros1Ur_msgsMasterboardDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["robot_voltage_48V"];
out["robot_voltage_48V"]=field;
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
/** Complete ROS1 ur_msgs/RobotModeDataMsg codec; maxima are not actual lengths. */
export const Ros1Ur_msgsRobotModeDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_64c4247d865e0731b13ca13ce2e355df664cabf7f3d646051d9d3ef48b05a231",maxWireBytes:15,minWireBytes:15,cartesianMaxWireBytes:15n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"timestamp":0n,"is_robot_connected":false,"is_real_robot_enabled":false,"is_power_on_robot":false,"is_emergency_stopped":false,"is_protective_stopped":false,"is_program_running":false,"is_program_paused":false};},
validateValue(value){Ros1Ur_msgsRobotModeDataMsgSize(Ros1Ur_msgsRobotModeDataMsgCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsRobotModeDataMsgCapture(value),n=Ros1Ur_msgsRobotModeDataMsgSize(owned),w=new Writer(n);Ros1Ur_msgsRobotModeDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,15,90);if(owned.length<15)bad();const r=new Reader(owned),value=Ros1Ur_msgsRobotModeDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsRobotModeDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["timestamp"],8,false);n=addSize(n,8);
if(typeof value["is_robot_connected"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_real_robot_enabled"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_power_on_robot"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_emergency_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_protective_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_program_running"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["is_program_paused"]!=="boolean")bad();n=addSize(n,1);
if(n>15)bad(90);return n;}
function Ros1Ur_msgsRobotModeDataMsgWrite(value,w){
w.v.setBigUint64(w.at,value["timestamp"],true);w.at+=8;
w.v.setUint8(w.at,value["is_robot_connected"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_real_robot_enabled"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_power_on_robot"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_emergency_stopped"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_protective_stopped"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_program_running"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["is_program_paused"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsRobotModeDataMsgRead(r){const value={};
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
function Ros1Ur_msgsRobotModeDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_msgs/RobotStateRTMsg codec; maxima are not actual lengths. */
export const Ros1Ur_msgsRobotStateRTMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_e3d93b6141f749b93c229d8eabebbeb2782a16621fea4d7d39096794e7a67ce7",maxWireBytes:768,minWireBytes:96,cartesianMaxWireBytes:768n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"time":0,"q_target":[],"qd_target":[],"qdd_target":[],"i_target":[],"m_target":[],"q_actual":[],"qd_actual":[],"i_actual":[],"tool_acc_values":[],"tcp_force":[],"tool_vector":[],"tcp_speed":[],"digital_input_bits":0,"motor_temperatures":[],"controller_timer":0,"test_value":0,"robot_mode":0,"joint_modes":[]};},
validateValue(value){Ros1Ur_msgsRobotStateRTMsgSize(Ros1Ur_msgsRobotStateRTMsgCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsRobotStateRTMsgCapture(value),n=Ros1Ur_msgsRobotStateRTMsgSize(owned),w=new Writer(n);Ros1Ur_msgsRobotStateRTMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,768,90);if(owned.length<96)bad();const r=new Reader(owned),value=Ros1Ur_msgsRobotStateRTMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsRobotStateRTMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros1Ur_msgsRobotStateRTMsgWrite(value,w){
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
function Ros1Ur_msgsRobotStateRTMsgRead(r){const value={};
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
function Ros1Ur_msgsRobotStateRTMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_msgs/SetAnalogOutputRequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetAnalogOutputRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1830a63c8853ebe8842f0e8e6870fbdee136dfe78726ba581fd1c54f910c4789",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":Ros1Ur_msgsAnalogCodec.defaultValue()};},
validateValue(value){Ros1Ur_msgsSetAnalogOutputRequestSize(Ros1Ur_msgsSetAnalogOutputRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetAnalogOutputRequestCapture(value),n=Ros1Ur_msgsSetAnalogOutputRequestSize(owned),w=new Writer(n);Ros1Ur_msgsSetAnalogOutputRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetAnalogOutputRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetAnalogOutputRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Ur_msgsAnalogSize(value["data"]));
if(n>6)bad(90);return n;}
function Ros1Ur_msgsSetAnalogOutputRequestWrite(value,w){
Ros1Ur_msgsAnalogWrite(value["data"],w);
}
function Ros1Ur_msgsSetAnalogOutputRequestRead(r){const value={};
{
const decoded=Ros1Ur_msgsAnalogRead(r);
value["data"]=decoded;
}
return value;}
function Ros1Ur_msgsSetAnalogOutputRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=Ros1Ur_msgsAnalogCapture(field);
}
return out;}
/** Complete ROS1 ur_msgs/SetAnalogOutputResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetAnalogOutputResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bb84c969f904082699f67d385b093b0c6de97bfacc33eab86932f4564a823580",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Ur_msgsSetAnalogOutputResponseSize(Ros1Ur_msgsSetAnalogOutputResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetAnalogOutputResponseCapture(value),n=Ros1Ur_msgsSetAnalogOutputResponseSize(owned),w=new Writer(n);Ros1Ur_msgsSetAnalogOutputResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetAnalogOutputResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetAnalogOutputResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_msgsSetAnalogOutputResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsSetAnalogOutputResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_msgsSetAnalogOutputResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/SetForceModeRequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetForceModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c148d6eb37258e10f5d16e76e41c334fc57a3d751c0c35c47be4cabf4c015a6b",maxWireBytes:335,minWireBytes:207,cartesianMaxWireBytes:335n,aggregateWireBytes:0,constants:Object.freeze({"TCP_TO_ORIGIN":1,"NO_TRANSFORM":2,"TCP_VELOCITY_TO_X_Y":3}),
defaultValue(){return {"task_frame":Ros1Geometry_msgsPoseStampedCodec.defaultValue(),"selection_vector_x":false,"selection_vector_y":false,"selection_vector_z":false,"selection_vector_rx":false,"selection_vector_ry":false,"selection_vector_rz":false,"wrench":Ros1Geometry_msgsWrenchCodec.defaultValue(),"type":0,"speed_limits":Ros1Geometry_msgsTwistCodec.defaultValue(),"deviation_limits":Array.from({length:6},()=>(0)),"damping_factor":0,"gain_scaling":0};},
validateValue(value){Ros1Ur_msgsSetForceModeRequestSize(Ros1Ur_msgsSetForceModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetForceModeRequestCapture(value),n=Ros1Ur_msgsSetForceModeRequestSize(owned),w=new Writer(n);Ros1Ur_msgsSetForceModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,335,90);if(owned.length<207)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetForceModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetForceModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(value["task_frame"]));
if(typeof value["selection_vector_x"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_y"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_z"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_rx"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_ry"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["selection_vector_rz"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Geometry_msgsWrenchSize(value["wrench"]));
integer(value["type"],1,false);n=addSize(n,1);
n=addSize(n,Ros1Geometry_msgsTwistSize(value["speed_limits"]));
if(!Array.isArray(value["deviation_limits"])||value["deviation_limits"].length!==6)bad();
for(const element of value["deviation_limits"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["damping_factor"]!=="number"||!Number.isFinite(value["damping_factor"])||!Number.isFinite(Math.fround(value["damping_factor"])))bad();n=addSize(n,4);
if(typeof value["gain_scaling"]!=="number"||!Number.isFinite(value["gain_scaling"])||!Number.isFinite(Math.fround(value["gain_scaling"])))bad();n=addSize(n,4);
if(n>335)bad(90);return n;}
function Ros1Ur_msgsSetForceModeRequestWrite(value,w){
Ros1Geometry_msgsPoseStampedWrite(value["task_frame"],w);
w.v.setUint8(w.at,value["selection_vector_x"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_y"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_z"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_rx"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_ry"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["selection_vector_rz"]?1:0,true);w.at+=1;
Ros1Geometry_msgsWrenchWrite(value["wrench"],w);
w.v.setUint8(w.at,value["type"],true);w.at+=1;
Ros1Geometry_msgsTwistWrite(value["speed_limits"],w);
for(const element of value["deviation_limits"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["damping_factor"],true);w.at+=4;
w.v.setFloat32(w.at,value["gain_scaling"],true);w.at+=4;
}
function Ros1Ur_msgsSetForceModeRequestRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
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
const decoded=Ros1Geometry_msgsWrenchRead(r);
value["wrench"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["type"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistRead(r);
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
function Ros1Ur_msgsSetForceModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task_frame"];
out["task_frame"]=Ros1Geometry_msgsPoseStampedCapture(field);
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
out["wrench"]=Ros1Geometry_msgsWrenchCapture(field);
}
{const field=value["type"];
out["type"]=field;
}
{const field=value["speed_limits"];
out["speed_limits"]=Ros1Geometry_msgsTwistCapture(field);
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
/** Complete ROS1 ur_msgs/SetForceModeResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetForceModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bb1eeb0b38d81d642066bed1510245423f62d25983a2de89e985d1fd1932308c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Ur_msgsSetForceModeResponseSize(Ros1Ur_msgsSetForceModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetForceModeResponseCapture(value),n=Ros1Ur_msgsSetForceModeResponseSize(owned),w=new Writer(n);Ros1Ur_msgsSetForceModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetForceModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetForceModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_msgsSetForceModeResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsSetForceModeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_msgsSetForceModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/SetIORequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetIORequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3dbd2f05686ddaac16b37dc8ec98303acb672d176be34fc906cf32f2d4cd1872",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"PIN_AOUT0":0,"PIN_AOUT1":1,"PIN_DOUT0":0,"PIN_DOUT1":1,"PIN_DOUT2":2,"PIN_DOUT3":3,"PIN_DOUT4":4,"PIN_DOUT5":5,"PIN_DOUT6":6,"PIN_DOUT7":7,"PIN_CONF_OUT0":8,"PIN_CONF_OUT1":9,"PIN_CONF_OUT2":10,"PIN_CONF_OUT3":11,"PIN_CONF_OUT4":12,"PIN_CONF_OUT5":13,"PIN_CONF_OUT6":14,"PIN_CONF_OUT7":15,"PIN_TOOL_DOUT0":16,"PIN_TOOL_DOUT1":17,"FUN_SET_DIGITAL_OUT":1,"FUN_SET_FLAG":2,"FUN_SET_ANALOG_OUT":3,"FUN_SET_TOOL_VOLTAGE":4,"STATE_OFF":0,"STATE_ON":1,"STATE_TOOL_VOLTAGE_0V":0,"STATE_TOOL_VOLTAGE_12V":12,"STATE_TOOL_VOLTAGE_24V":24}),
defaultValue(){return {"fun":0,"pin":0,"state":0};},
validateValue(value){Ros1Ur_msgsSetIORequestSize(Ros1Ur_msgsSetIORequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetIORequestCapture(value),n=Ros1Ur_msgsSetIORequestSize(owned),w=new Writer(n);Ros1Ur_msgsSetIORequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetIORequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetIORequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["fun"],1,true);n=addSize(n,1);
integer(value["pin"],1,true);n=addSize(n,1);
if(typeof value["state"]!=="number"||!Number.isFinite(value["state"])||!Number.isFinite(Math.fround(value["state"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros1Ur_msgsSetIORequestWrite(value,w){
w.v.setInt8(w.at,value["fun"],true);w.at+=1;
w.v.setInt8(w.at,value["pin"],true);w.at+=1;
w.v.setFloat32(w.at,value["state"],true);w.at+=4;
}
function Ros1Ur_msgsSetIORequestRead(r){const value={};
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
function Ros1Ur_msgsSetIORequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ur_msgs/SetIOResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetIOResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7fc137aa3db3178a450b8b2396b7821c911c79656e52bf48814745c261aecf61",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Ur_msgsSetIOResponseSize(Ros1Ur_msgsSetIOResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetIOResponseCapture(value),n=Ros1Ur_msgsSetIOResponseSize(owned),w=new Writer(n);Ros1Ur_msgsSetIOResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetIOResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetIOResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_msgsSetIOResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsSetIOResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_msgsSetIOResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/SetPayloadRequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetPayloadRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_63ae68b44ce6aaad37cbda779fb51dee78923df27c582b4aab4104b16a307ee3",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"center_of_gravity":Ros1Geometry_msgsVector3Codec.defaultValue()};},
validateValue(value){Ros1Ur_msgsSetPayloadRequestSize(Ros1Ur_msgsSetPayloadRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetPayloadRequestCapture(value),n=Ros1Ur_msgsSetPayloadRequestSize(owned),w=new Writer(n);Ros1Ur_msgsSetPayloadRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetPayloadRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetPayloadRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
n=addSize(n,Ros1Geometry_msgsVector3Size(value["center_of_gravity"]));
if(n>28)bad(90);return n;}
function Ros1Ur_msgsSetPayloadRequestWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
Ros1Geometry_msgsVector3Write(value["center_of_gravity"],w);
}
function Ros1Ur_msgsSetPayloadRequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3Read(r);
value["center_of_gravity"]=decoded;
}
return value;}
function Ros1Ur_msgsSetPayloadRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["center_of_gravity"];
out["center_of_gravity"]=Ros1Geometry_msgsVector3Capture(field);
}
return out;}
/** Complete ROS1 ur_msgs/SetPayloadResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetPayloadResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a3e991f1357f2d3017ccbb7636a6248e8774434d723f5f6c77ff251dd61fce73",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Ur_msgsSetPayloadResponseSize(Ros1Ur_msgsSetPayloadResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetPayloadResponseCapture(value),n=Ros1Ur_msgsSetPayloadResponseSize(owned),w=new Writer(n);Ros1Ur_msgsSetPayloadResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetPayloadResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetPayloadResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_msgsSetPayloadResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsSetPayloadResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_msgsSetPayloadResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/SetSpeedSliderFractionRequest codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetSpeedSliderFractionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f192d6d655b2ed7725bf5afe6977a352870abfd4d07e0c3380a09d280a0a0a32",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_slider_fraction":0};},
validateValue(value){Ros1Ur_msgsSetSpeedSliderFractionRequestSize(Ros1Ur_msgsSetSpeedSliderFractionRequestCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetSpeedSliderFractionRequestCapture(value),n=Ros1Ur_msgsSetSpeedSliderFractionRequestSize(owned),w=new Writer(n);Ros1Ur_msgsSetSpeedSliderFractionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetSpeedSliderFractionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetSpeedSliderFractionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["speed_slider_fraction"]!=="number"||!Number.isFinite(value["speed_slider_fraction"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros1Ur_msgsSetSpeedSliderFractionRequestWrite(value,w){
w.v.setFloat64(w.at,value["speed_slider_fraction"],true);w.at+=8;
}
function Ros1Ur_msgsSetSpeedSliderFractionRequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["speed_slider_fraction"]=decoded;
}
return value;}
function Ros1Ur_msgsSetSpeedSliderFractionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_slider_fraction"];
out["speed_slider_fraction"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/SetSpeedSliderFractionResponse codec; maxima are not actual lengths. */
export const Ros1Ur_msgsSetSpeedSliderFractionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cb08c6758ce2eeeac93bc579d7895b19b84e74b7e2d19d87831023df579288b0",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Ur_msgsSetSpeedSliderFractionResponseSize(Ros1Ur_msgsSetSpeedSliderFractionResponseCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsSetSpeedSliderFractionResponseCapture(value),n=Ros1Ur_msgsSetSpeedSliderFractionResponseSize(owned),w=new Writer(n);Ros1Ur_msgsSetSpeedSliderFractionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ur_msgsSetSpeedSliderFractionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsSetSpeedSliderFractionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ur_msgsSetSpeedSliderFractionResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Ur_msgsSetSpeedSliderFractionResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Ur_msgsSetSpeedSliderFractionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 ur_msgs/ToolDataMsg codec; maxima are not actual lengths. */
export const Ros1Ur_msgsToolDataMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_1fad1aba069bc474306dc421a62950ffeb3a0837668f08a9adf41ac1bc2bb390",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({"ANALOG_INPUT_RANGE_CURRENT":0,"ANALOG_INPUT_RANGE_VOLTAGE":1,"TOOL_BOOTLOADER_MODE":249,"TOOL_RUNNING_MODE":253,"TOOL_IDLE_MODE":255}),
defaultValue(){return {"analog_input_range2":0,"analog_input_range3":0,"analog_input2":0,"analog_input3":0,"tool_voltage_48v":0,"tool_output_voltage":0,"tool_current":0,"tool_temperature":0,"tool_mode":0};},
validateValue(value){Ros1Ur_msgsToolDataMsgSize(Ros1Ur_msgsToolDataMsgCapture(value));return true;},
encode(value){const owned=Ros1Ur_msgsToolDataMsgCapture(value),n=Ros1Ur_msgsToolDataMsgSize(owned),w=new Writer(n);Ros1Ur_msgsToolDataMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros1Ur_msgsToolDataMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ur_msgsToolDataMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros1Ur_msgsToolDataMsgWrite(value,w){
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
function Ros1Ur_msgsToolDataMsgRead(r){const value={};
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
function Ros1Ur_msgsToolDataMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
