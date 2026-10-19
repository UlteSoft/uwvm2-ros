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
/** Complete ROS2 uwvm_groot_bridge/msg/RGBFrame codec; maxima are not actual lengths. */
export const Ros2Uwvm_groot_bridgeMsgRGBFrameCodec=Object.freeze({codecIdentity:"UWVMROS1_db7172fe7a7f0672c907f31ca0b1cb6286b804246d62e0414ac4313ae1096364",maxWireBytes:860,minWireBytes:28,cartesianMaxWireBytes:860n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"key":"","shape":Array.from({length:5},()=>(0)),"pixels":[]};},
validateValue(value){Ros2Uwvm_groot_bridgeMsgRGBFrameSize(Ros2Uwvm_groot_bridgeMsgRGBFrameCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_groot_bridgeMsgRGBFrameCapture(value),n=Ros2Uwvm_groot_bridgeMsgRGBFrameSize(owned),w=new Writer(n);Ros2Uwvm_groot_bridgeMsgRGBFrameWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,860,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Uwvm_groot_bridgeMsgRGBFrameRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_groot_bridgeMsgRGBFrameSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["key"],64).length);
if(!Array.isArray(value["shape"])||value["shape"].length!==5)bad();
for(const element of value["shape"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["pixels"])||value["pixels"].length>768)bad();
n=addSize(n,4);
for(const element of value["pixels"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>860)bad(90);return n;}
function Ros2Uwvm_groot_bridgeMsgRGBFrameWrite(value,w){
w.text(value["key"],64);
for(const element of value["shape"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["pixels"].length,true);w.at+=4;
for(const element of value["pixels"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Uwvm_groot_bridgeMsgRGBFrameRead(r){const value={};
{
const decoded=r.text(64);
value["key"]=decoded;
}
{
{const count=5;if(count>5)bad();value["shape"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["shape"].push(decoded);
}}
}
{
{const count=r.u32();if(count>768)bad();value["pixels"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["pixels"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_groot_bridgeMsgRGBFrameCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["key"];
out["key"]=field;
}
{const field=value["shape"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==5)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["shape"]=values;
}
{const field=value["pixels"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>768)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["pixels"]=values;
}
return out;}
/** Complete ROS2 uwvm_groot_bridge/msg/FloatTensor codec; maxima are not actual lengths. */
export const Ros2Uwvm_groot_bridgeMsgFloatTensorCodec=Object.freeze({codecIdentity:"UWVMROS1_a091d322ab737d2a9415f889fb78a18df7aec6ce74cfab0dcd5bb2a61ae3abae",maxWireBytes:1364,minWireBytes:20,cartesianMaxWireBytes:1364n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"key":"","shape":Array.from({length:3},()=>(0)),"values":[]};},
validateValue(value){Ros2Uwvm_groot_bridgeMsgFloatTensorSize(Ros2Uwvm_groot_bridgeMsgFloatTensorCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_groot_bridgeMsgFloatTensorCapture(value),n=Ros2Uwvm_groot_bridgeMsgFloatTensorSize(owned),w=new Writer(n);Ros2Uwvm_groot_bridgeMsgFloatTensorWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1364,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Uwvm_groot_bridgeMsgFloatTensorRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_groot_bridgeMsgFloatTensorSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["key"],64).length);
if(!Array.isArray(value["shape"])||value["shape"].length!==3)bad();
for(const element of value["shape"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["values"])||value["values"].length>320)bad();
n=addSize(n,4);
for(const element of value["values"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>1364)bad(90);return n;}
function Ros2Uwvm_groot_bridgeMsgFloatTensorWrite(value,w){
w.text(value["key"],64);
for(const element of value["shape"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Uwvm_groot_bridgeMsgFloatTensorRead(r){const value={};
{
const decoded=r.text(64);
value["key"]=decoded;
}
{
{const count=3;if(count>3)bad();value["shape"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["shape"].push(decoded);
}}
}
{
{const count=r.u32();if(count>320)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["values"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_groot_bridgeMsgFloatTensorCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["key"];
out["key"]=field;
}
{const field=value["shape"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["shape"]=values;
}
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>320)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["values"]=values;
}
return out;}
/** Complete ROS2 uwvm_groot_bridge/msg/TextSequence codec; maxima are not actual lengths. */
export const Ros2Uwvm_groot_bridgeMsgTextSequenceCodec=Object.freeze({codecIdentity:"UWVMROS1_895be974f2560f0afff7614f5400ff7205e013cda269825a8443628621c8e80d",maxWireBytes:1120,minWireBytes:16,cartesianMaxWireBytes:1120n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"key":"","shape":Array.from({length:2},()=>(0)),"texts":[]};},
validateValue(value){Ros2Uwvm_groot_bridgeMsgTextSequenceSize(Ros2Uwvm_groot_bridgeMsgTextSequenceCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_groot_bridgeMsgTextSequenceCapture(value),n=Ros2Uwvm_groot_bridgeMsgTextSequenceSize(owned),w=new Writer(n);Ros2Uwvm_groot_bridgeMsgTextSequenceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1120,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Uwvm_groot_bridgeMsgTextSequenceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_groot_bridgeMsgTextSequenceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["key"],64).length);
if(!Array.isArray(value["shape"])||value["shape"].length!==2)bad();
for(const element of value["shape"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["texts"])||value["texts"].length>4)bad();
n=addSize(n,4);
for(const element of value["texts"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>1120)bad(90);return n;}
function Ros2Uwvm_groot_bridgeMsgTextSequenceWrite(value,w){
w.text(value["key"],64);
for(const element of value["shape"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["texts"].length,true);w.at+=4;
for(const element of value["texts"]){
w.text(element,256);
}
}
function Ros2Uwvm_groot_bridgeMsgTextSequenceRead(r){const value={};
{
const decoded=r.text(64);
value["key"]=decoded;
}
{
{const count=2;if(count>2)bad();value["shape"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["shape"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["texts"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["texts"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_groot_bridgeMsgTextSequenceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["key"];
out["key"]=field;
}
{const field=value["shape"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["shape"]=values;
}
{const field=value["texts"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["texts"]=values;
}
return out;}
/** Complete ROS2 uwvm_groot_bridge/srv/InferAction_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_groot_bridgeSrvInferAction_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_78f85929fb8646e9a92f17cafa3e8296fde42aaf49a56b271531ebad869a0fa6",maxWireBytes:10296,minWireBytes:20,cartesianMaxWireBytes:10296n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"application_nonce":0n,"video":[],"state":[],"language":[]};},
validateValue(value){Ros2Uwvm_groot_bridgeSrvInferAction_RequestSize(Ros2Uwvm_groot_bridgeSrvInferAction_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_groot_bridgeSrvInferAction_RequestCapture(value),n=Ros2Uwvm_groot_bridgeSrvInferAction_RequestSize(owned),w=new Writer(n);Ros2Uwvm_groot_bridgeSrvInferAction_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10296,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Uwvm_groot_bridgeSrvInferAction_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_groot_bridgeSrvInferAction_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["application_nonce"],8,false);n=addSize(n,8);
if(!Array.isArray(value["video"])||value["video"].length>3)bad();
n=addSize(n,4);
for(const element of value["video"]){
n=addSize(n,Ros2Uwvm_groot_bridgeMsgRGBFrameSize(element));
}
if(!Array.isArray(value["state"])||value["state"].length>4)bad();
n=addSize(n,4);
for(const element of value["state"]){
n=addSize(n,Ros2Uwvm_groot_bridgeMsgFloatTensorSize(element));
}
if(!Array.isArray(value["language"])||value["language"].length>2)bad();
n=addSize(n,4);
for(const element of value["language"]){
n=addSize(n,Ros2Uwvm_groot_bridgeMsgTextSequenceSize(element));
}
if(n>10296)bad(90);return n;}
function Ros2Uwvm_groot_bridgeSrvInferAction_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["application_nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["video"].length,true);w.at+=4;
for(const element of value["video"]){
Ros2Uwvm_groot_bridgeMsgRGBFrameWrite(element,w);
}
w.v.setUint32(w.at,value["state"].length,true);w.at+=4;
for(const element of value["state"]){
Ros2Uwvm_groot_bridgeMsgFloatTensorWrite(element,w);
}
w.v.setUint32(w.at,value["language"].length,true);w.at+=4;
for(const element of value["language"]){
Ros2Uwvm_groot_bridgeMsgTextSequenceWrite(element,w);
}
}
function Ros2Uwvm_groot_bridgeSrvInferAction_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["application_nonce"]=decoded;
}
{
{const count=r.u32();if(count>3)bad();value["video"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgRGBFrameRead(r);
value["video"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["state"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgFloatTensorRead(r);
value["state"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["language"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgTextSequenceRead(r);
value["language"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_groot_bridgeSrvInferAction_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["application_nonce"];
out["application_nonce"]=field;
}
{const field=value["video"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_groot_bridgeMsgRGBFrameCapture(element);
}out["video"]=values;
}
{const field=value["state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_groot_bridgeMsgFloatTensorCapture(element);
}out["state"]=values;
}
{const field=value["language"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_groot_bridgeMsgTextSequenceCapture(element);
}out["language"]=values;
}
return out;}
/** Complete ROS2 uwvm_groot_bridge/srv/InferAction_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_groot_bridgeSrvInferAction_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_787281a2adea187ae43ff951a414dd7563b3e08318414e5199e1950b8143a3ce",maxWireBytes:10929,minWireBytes:17,cartesianMaxWireBytes:10929n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":0,"application_nonce":0n,"action":[],"info_is_empty":false};},
validateValue(value){Ros2Uwvm_groot_bridgeSrvInferAction_ResponseSize(Ros2Uwvm_groot_bridgeSrvInferAction_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_groot_bridgeSrvInferAction_ResponseCapture(value),n=Ros2Uwvm_groot_bridgeSrvInferAction_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_groot_bridgeSrvInferAction_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10929,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Uwvm_groot_bridgeSrvInferAction_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_groot_bridgeSrvInferAction_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],4,false);n=addSize(n,4);
integer(value["application_nonce"],8,false);n=addSize(n,8);
if(!Array.isArray(value["action"])||value["action"].length>8)bad();
n=addSize(n,4);
for(const element of value["action"]){
n=addSize(n,Ros2Uwvm_groot_bridgeMsgFloatTensorSize(element));
}
if(typeof value["info_is_empty"]!=="boolean")bad();n=addSize(n,1);
if(n>10929)bad(90);return n;}
function Ros2Uwvm_groot_bridgeSrvInferAction_ResponseWrite(value,w){
w.v.setUint32(w.at,value["status"],true);w.at+=4;
w.v.setBigUint64(w.at,value["application_nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["action"].length,true);w.at+=4;
for(const element of value["action"]){
Ros2Uwvm_groot_bridgeMsgFloatTensorWrite(element,w);
}
w.v.setUint8(w.at,value["info_is_empty"]?1:0,true);w.at+=1;
}
function Ros2Uwvm_groot_bridgeSrvInferAction_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["status"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["application_nonce"]=decoded;
}
{
{const count=r.u32();if(count>8)bad();value["action"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgFloatTensorRead(r);
value["action"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["info_is_empty"]=decoded!==0;
}
return value;}
function Ros2Uwvm_groot_bridgeSrvInferAction_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["application_nonce"];
out["application_nonce"]=field;
}
{const field=value["action"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>8)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_groot_bridgeMsgFloatTensorCapture(element);
}out["action"]=values;
}
{const field=value["info_is_empty"];
out["info_is_empty"]=field;
}
return out;}
