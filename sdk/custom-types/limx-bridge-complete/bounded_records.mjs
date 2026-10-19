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
/** Complete ROS2 uwvm_limx_bridge/msg/NativeHeader codec; maxima are not actual lengths. */
export const Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2",maxWireBytes:144,minWireBytes:16,cartesianMaxWireBytes:144n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"seq":0,"stamp_sec":0,"stamp_nsec":0,"frame_id":""};},
validateValue(value){Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(value),n=Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(owned),w=new Writer(n);Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,144,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["seq"],4,false);n=addSize(n,4);
integer(value["stamp_sec"],4,false);n=addSize(n,4);
integer(value["stamp_nsec"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(n>144)bad(90);return n;}
function Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(value,w){
w.v.setUint32(w.at,value["seq"],true);w.at+=4;
w.v.setUint32(w.at,value["stamp_sec"],true);w.at+=4;
w.v.setUint32(w.at,value["stamp_nsec"],true);w.at+=4;
w.text(value["frame_id"],128);
}
function Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["seq"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["stamp_sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["stamp_nsec"]=decoded;
}
{
const decoded=r.text(128);
value["frame_id"]=decoded;
}
return value;}
function Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["seq"];
out["seq"]=field;
}
{const field=value["stamp_sec"];
out["stamp_sec"]=field;
}
{const field=value["stamp_nsec"];
out["stamp_nsec"]=field;
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS2 uwvm_limx_bridge/msg/ImuData codec; maxima are not actual lengths. */
export const Ros2Uwvm_limx_bridgeMsgImuDataCodec=Object.freeze({codecIdentity:"UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b",maxWireBytes:260,minWireBytes:132,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec.defaultValue(),"imustamp":0n,"status":0,"euler":Array.from({length:3},()=>(0)),"quat":Array.from({length:4},()=>(0)),"acc":Array.from({length:3},()=>(0)),"gyro":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Uwvm_limx_bridgeMsgImuDataSize(Ros2Uwvm_limx_bridgeMsgImuDataCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_limx_bridgeMsgImuDataCapture(value),n=Ros2Uwvm_limx_bridgeMsgImuDataSize(owned),w=new Writer(n);Ros2Uwvm_limx_bridgeMsgImuDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<132)bad();const r=new Reader(owned),value=Ros2Uwvm_limx_bridgeMsgImuDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_limx_bridgeMsgImuDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(value["header"]));
integer(value["imustamp"],8,false);n=addSize(n,8);
integer(value["status"],4,false);n=addSize(n,4);
if(!Array.isArray(value["euler"])||value["euler"].length!==3)bad();
for(const element of value["euler"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["quat"])||value["quat"].length!==4)bad();
for(const element of value["quat"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["acc"])||value["acc"].length!==3)bad();
for(const element of value["acc"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["gyro"])||value["gyro"].length!==3)bad();
for(const element of value["gyro"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>260)bad(90);return n;}
function Ros2Uwvm_limx_bridgeMsgImuDataWrite(value,w){
Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(value["header"],w);
w.v.setBigUint64(w.at,value["imustamp"],true);w.at+=8;
w.v.setUint32(w.at,value["status"],true);w.at+=4;
for(const element of value["euler"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["quat"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["acc"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["gyro"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Uwvm_limx_bridgeMsgImuDataRead(r){const value={};
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["imustamp"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["status"]=decoded;
}
{
{const count=3;if(count>3)bad();value["euler"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["euler"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["quat"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["quat"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["acc"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["acc"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["gyro"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["gyro"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_limx_bridgeMsgImuDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(field);
}
{const field=value["imustamp"];
out["imustamp"]=field;
}
{const field=value["status"];
out["status"]=field;
}
{const field=value["euler"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["euler"]=values;
}
{const field=value["quat"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["quat"]=values;
}
{const field=value["acc"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["acc"]=values;
}
{const field=value["gyro"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["gyro"]=values;
}
return out;}
/** Complete ROS2 uwvm_limx_bridge/msg/JointState codec; maxima are not actual lengths. */
export const Ros2Uwvm_limx_bridgeMsgJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4",maxWireBytes:5544,minWireBytes:40,cartesianMaxWireBytes:5544n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec.defaultValue(),"names":[],"q":[],"v":[],"vd":[],"tau":[],"na":0};},
validateValue(value){Ros2Uwvm_limx_bridgeMsgJointStateSize(Ros2Uwvm_limx_bridgeMsgJointStateCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_limx_bridgeMsgJointStateCapture(value),n=Ros2Uwvm_limx_bridgeMsgJointStateSize(owned),w=new Writer(n);Ros2Uwvm_limx_bridgeMsgJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5544,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros2Uwvm_limx_bridgeMsgJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_limx_bridgeMsgJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(value["header"]));
if(!Array.isArray(value["names"])||value["names"].length>64)bad();
n=addSize(n,4);
for(const element of value["names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["q"])||value["q"].length>64)bad();
n=addSize(n,4);
for(const element of value["q"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["v"])||value["v"].length>64)bad();
n=addSize(n,4);
for(const element of value["v"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["vd"])||value["vd"].length>64)bad();
n=addSize(n,4);
for(const element of value["vd"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["tau"])||value["tau"].length>64)bad();
n=addSize(n,4);
for(const element of value["tau"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
integer(value["na"],4,false);n=addSize(n,4);
if(n>5544)bad(90);return n;}
function Ros2Uwvm_limx_bridgeMsgJointStateWrite(value,w){
Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["names"].length,true);w.at+=4;
for(const element of value["names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["q"].length,true);w.at+=4;
for(const element of value["q"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["v"].length,true);w.at+=4;
for(const element of value["v"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["vd"].length,true);w.at+=4;
for(const element of value["vd"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["tau"].length,true);w.at+=4;
for(const element of value["tau"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["na"],true);w.at+=4;
}
function Ros2Uwvm_limx_bridgeMsgJointStateRead(r){const value={};
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>64)bad();value["names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["q"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["v"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["v"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["vd"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["vd"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["tau"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tau"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["na"]=decoded;
}
return value;}
function Ros2Uwvm_limx_bridgeMsgJointStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(field);
}
{const field=value["names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["names"]=values;
}
{const field=value["q"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q"]=values;
}
{const field=value["v"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["v"]=values;
}
{const field=value["vd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["vd"]=values;
}
{const field=value["tau"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau"]=values;
}
{const field=value["na"];
out["na"]=field;
}
return out;}
/** Complete ROS2 uwvm_limx_bridge/msg/JointCmd codec; maxima are not actual lengths. */
export const Ros2Uwvm_limx_bridgeMsgJointCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0",maxWireBytes:5872,minWireBytes:48,cartesianMaxWireBytes:5872n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec.defaultValue(),"names":[],"q":[],"v":[],"tau":[],"kp":[],"kd":[],"mode":[],"na":0};},
validateValue(value){Ros2Uwvm_limx_bridgeMsgJointCmdSize(Ros2Uwvm_limx_bridgeMsgJointCmdCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_limx_bridgeMsgJointCmdCapture(value),n=Ros2Uwvm_limx_bridgeMsgJointCmdSize(owned),w=new Writer(n);Ros2Uwvm_limx_bridgeMsgJointCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5872,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Uwvm_limx_bridgeMsgJointCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_limx_bridgeMsgJointCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(value["header"]));
if(!Array.isArray(value["names"])||value["names"].length>64)bad();
n=addSize(n,4);
for(const element of value["names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["q"])||value["q"].length>64)bad();
n=addSize(n,4);
for(const element of value["q"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["v"])||value["v"].length>64)bad();
n=addSize(n,4);
for(const element of value["v"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["tau"])||value["tau"].length>64)bad();
n=addSize(n,4);
for(const element of value["tau"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["kp"])||value["kp"].length>64)bad();
n=addSize(n,4);
for(const element of value["kp"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["kd"])||value["kd"].length>64)bad();
n=addSize(n,4);
for(const element of value["kd"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["mode"])||value["mode"].length>64)bad();
n=addSize(n,4);
for(const element of value["mode"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["na"],4,false);n=addSize(n,4);
if(n>5872)bad(90);return n;}
function Ros2Uwvm_limx_bridgeMsgJointCmdWrite(value,w){
Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["names"].length,true);w.at+=4;
for(const element of value["names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["q"].length,true);w.at+=4;
for(const element of value["q"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["v"].length,true);w.at+=4;
for(const element of value["v"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["tau"].length,true);w.at+=4;
for(const element of value["tau"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["kp"].length,true);w.at+=4;
for(const element of value["kp"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["kd"].length,true);w.at+=4;
for(const element of value["kd"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["mode"].length,true);w.at+=4;
for(const element of value["mode"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["na"],true);w.at+=4;
}
function Ros2Uwvm_limx_bridgeMsgJointCmdRead(r){const value={};
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>64)bad();value["names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["q"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["v"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["v"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["tau"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tau"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["kp"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kp"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["kd"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kd"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["mode"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["mode"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["na"]=decoded;
}
return value;}
function Ros2Uwvm_limx_bridgeMsgJointCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(field);
}
{const field=value["names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["names"]=values;
}
{const field=value["q"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q"]=values;
}
{const field=value["v"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["v"]=values;
}
{const field=value["tau"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau"]=values;
}
{const field=value["kp"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["kp"]=values;
}
{const field=value["kd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["kd"]=values;
}
{const field=value["mode"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mode"]=values;
}
{const field=value["na"];
out["na"]=field;
}
return out;}
/** Complete ROS2 uwvm_limx_bridge/msg/JointCmdLimx codec; maxima are not actual lengths. */
export const Ros2Uwvm_limx_bridgeMsgJointCmdLimxCodec=Object.freeze({codecIdentity:"UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9",maxWireBytes:5940,minWireBytes:52,cartesianMaxWireBytes:5940n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec.defaultValue(),"names":[],"q":[],"v":[],"tau":[],"kp":[],"kd":[],"mode":[],"na":0,"parallel_solver_mode":[]};},
validateValue(value){Ros2Uwvm_limx_bridgeMsgJointCmdLimxSize(Ros2Uwvm_limx_bridgeMsgJointCmdLimxCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_limx_bridgeMsgJointCmdLimxCapture(value),n=Ros2Uwvm_limx_bridgeMsgJointCmdLimxSize(owned),w=new Writer(n);Ros2Uwvm_limx_bridgeMsgJointCmdLimxWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5940,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros2Uwvm_limx_bridgeMsgJointCmdLimxRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_limx_bridgeMsgJointCmdLimxSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Uwvm_limx_bridgeMsgNativeHeaderSize(value["header"]));
if(!Array.isArray(value["names"])||value["names"].length>64)bad();
n=addSize(n,4);
for(const element of value["names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["q"])||value["q"].length>64)bad();
n=addSize(n,4);
for(const element of value["q"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["v"])||value["v"].length>64)bad();
n=addSize(n,4);
for(const element of value["v"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["tau"])||value["tau"].length>64)bad();
n=addSize(n,4);
for(const element of value["tau"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["kp"])||value["kp"].length>64)bad();
n=addSize(n,4);
for(const element of value["kp"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["kd"])||value["kd"].length>64)bad();
n=addSize(n,4);
for(const element of value["kd"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["mode"])||value["mode"].length>64)bad();
n=addSize(n,4);
for(const element of value["mode"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["na"],4,false);n=addSize(n,4);
if(!Array.isArray(value["parallel_solver_mode"])||value["parallel_solver_mode"].length>64)bad();
n=addSize(n,4);
for(const element of value["parallel_solver_mode"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>5940)bad(90);return n;}
function Ros2Uwvm_limx_bridgeMsgJointCmdLimxWrite(value,w){
Ros2Uwvm_limx_bridgeMsgNativeHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["names"].length,true);w.at+=4;
for(const element of value["names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["q"].length,true);w.at+=4;
for(const element of value["q"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["v"].length,true);w.at+=4;
for(const element of value["v"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["tau"].length,true);w.at+=4;
for(const element of value["tau"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["kp"].length,true);w.at+=4;
for(const element of value["kp"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["kd"].length,true);w.at+=4;
for(const element of value["kd"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["mode"].length,true);w.at+=4;
for(const element of value["mode"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["na"],true);w.at+=4;
w.v.setUint32(w.at,value["parallel_solver_mode"].length,true);w.at+=4;
for(const element of value["parallel_solver_mode"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Uwvm_limx_bridgeMsgJointCmdLimxRead(r){const value={};
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>64)bad();value["names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["q"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["v"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["v"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["tau"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tau"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["kp"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kp"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["kd"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kd"].push(decoded);
}}
}
{
{const count=r.u32();if(count>64)bad();value["mode"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["mode"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["na"]=decoded;
}
{
{const count=r.u32();if(count>64)bad();value["parallel_solver_mode"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["parallel_solver_mode"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_limx_bridgeMsgJointCmdLimxCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Uwvm_limx_bridgeMsgNativeHeaderCapture(field);
}
{const field=value["names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["names"]=values;
}
{const field=value["q"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q"]=values;
}
{const field=value["v"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["v"]=values;
}
{const field=value["tau"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau"]=values;
}
{const field=value["kp"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["kp"]=values;
}
{const field=value["kd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["kd"]=values;
}
{const field=value["mode"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mode"]=values;
}
{const field=value["na"];
out["na"]=field;
}
{const field=value["parallel_solver_mode"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["parallel_solver_mode"]=values;
}
return out;}
