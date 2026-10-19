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
/** Complete ROS1 clearpath_camera_msgs/InspectPOIGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_camera_msgsInspectPOIGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_565be10d97c4c60912d396cfb44121df60bc367458a8907d0dbccb48df3a35db",maxWireBytes:264,minWireBytes:8,cartesianMaxWireBytes:264n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"poi_uuid":"","zoom":0};},
validateValue(value){Ros1Clearpath_camera_msgsInspectPOIGoalSize(Ros1Clearpath_camera_msgsInspectPOIGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_camera_msgsInspectPOIGoalCapture(value),n=Ros1Clearpath_camera_msgsInspectPOIGoalSize(owned),w=new Writer(n);Ros1Clearpath_camera_msgsInspectPOIGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,264,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_camera_msgsInspectPOIGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_camera_msgsInspectPOIGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["poi_uuid"],256).length);
if(typeof value["zoom"]!=="number"||!Number.isFinite(value["zoom"])||!Number.isFinite(Math.fround(value["zoom"])))bad();n=addSize(n,4);
if(n>264)bad(90);return n;}
function Ros1Clearpath_camera_msgsInspectPOIGoalWrite(value,w){
w.text(value["poi_uuid"],256);
w.v.setFloat32(w.at,value["zoom"],true);w.at+=4;
}
function Ros1Clearpath_camera_msgsInspectPOIGoalRead(r){const value={};
{
const decoded=r.text(256);
value["poi_uuid"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["zoom"]=decoded;
}
return value;}
function Ros1Clearpath_camera_msgsInspectPOIGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["poi_uuid"];
out["poi_uuid"]=field;
}
{const field=value["zoom"];
out["zoom"]=field;
}
return out;}
/** Complete ROS1 clearpath_camera_msgs/InspectPOIResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_camera_msgsInspectPOIResultCodec=Object.freeze({codecIdentity:"UWVMROS1_6e3fb05c4e74fb8dd7de685bc0f774506921e3b4cf060721b2a756771403e156",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_camera_msgsInspectPOIResultSize(Ros1Clearpath_camera_msgsInspectPOIResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_camera_msgsInspectPOIResultCapture(value),n=Ros1Clearpath_camera_msgsInspectPOIResultSize(owned),w=new Writer(n);Ros1Clearpath_camera_msgsInspectPOIResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_camera_msgsInspectPOIResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_camera_msgsInspectPOIResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_camera_msgsInspectPOIResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_camera_msgsInspectPOIResultRead(r){const value={};
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
function Ros1Clearpath_camera_msgsInspectPOIResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_camera_msgs/InspectPOIFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_camera_msgsInspectPOIFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_27296847f85ab4f22770912227596b59886335e35a4c3b0313fee5bf5b386777",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_camera_msgsInspectPOIFeedbackSize(Ros1Clearpath_camera_msgsInspectPOIFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_camera_msgsInspectPOIFeedbackCapture(value),n=Ros1Clearpath_camera_msgsInspectPOIFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_camera_msgsInspectPOIFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_camera_msgsInspectPOIFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_camera_msgsInspectPOIFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_camera_msgsInspectPOIFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_camera_msgsInspectPOIFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_camera_msgsInspectPOIFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalDockGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalDockGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_45a53e58200f1fae57933c2414443a3ee7c0c80dfe5d849d59f5085030abab81",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_dock_msgsLocalDockGoalSize(Ros1Clearpath_dock_msgsLocalDockGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalDockGoalCapture(value),n=Ros1Clearpath_dock_msgsLocalDockGoalSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalDockGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalDockGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalDockGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalDockGoalWrite(value,w){
}
function Ros1Clearpath_dock_msgsLocalDockGoalRead(r){const value={};
return value;}
function Ros1Clearpath_dock_msgsLocalDockGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalDockResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalDockResultCodec=Object.freeze({codecIdentity:"UWVMROS1_eb485112962a291a8ab4d789f8d36a96e9dd45b54c35ecd070ffefcef510310b",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsLocalDockResultSize(Ros1Clearpath_dock_msgsLocalDockResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalDockResultCapture(value),n=Ros1Clearpath_dock_msgsLocalDockResultSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalDockResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalDockResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalDockResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalDockResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsLocalDockResultRead(r){const value={};
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
function Ros1Clearpath_dock_msgsLocalDockResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalDockFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalDockFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_242011c30d8e33482c33e2d379c7d70a021236a000ecf3f6df4941980cfe10e1",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsLocalDockFeedbackSize(Ros1Clearpath_dock_msgsLocalDockFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalDockFeedbackCapture(value),n=Ros1Clearpath_dock_msgsLocalDockFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalDockFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalDockFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalDockFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalDockFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsLocalDockFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsLocalDockFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalUndockGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalUndockGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_9ae8cdbd777755af0f8d92e93b3a45438af4b1b02af9fedfedf97ea45a0d81fc",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"undock_distance":0};},
validateValue(value){Ros1Clearpath_dock_msgsLocalUndockGoalSize(Ros1Clearpath_dock_msgsLocalUndockGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalUndockGoalCapture(value),n=Ros1Clearpath_dock_msgsLocalUndockGoalSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalUndockGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalUndockGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalUndockGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["undock_distance"]!=="number"||!Number.isFinite(value["undock_distance"])||!Number.isFinite(Math.fround(value["undock_distance"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalUndockGoalWrite(value,w){
w.v.setFloat32(w.at,value["undock_distance"],true);w.at+=4;
}
function Ros1Clearpath_dock_msgsLocalUndockGoalRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["undock_distance"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsLocalUndockGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["undock_distance"];
out["undock_distance"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalUndockResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalUndockResultCodec=Object.freeze({codecIdentity:"UWVMROS1_e6f44748c15f5653f3f79ac5ebbcb8067a52736c1905aeb76a8646d4d0caf6c2",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsLocalUndockResultSize(Ros1Clearpath_dock_msgsLocalUndockResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalUndockResultCapture(value),n=Ros1Clearpath_dock_msgsLocalUndockResultSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalUndockResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalUndockResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalUndockResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalUndockResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsLocalUndockResultRead(r){const value={};
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
function Ros1Clearpath_dock_msgsLocalUndockResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/LocalUndockFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsLocalUndockFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_f3d6f32b8f1735c612aba8c5bfc6bc4d45f59b80b3ed75e6fc64f20b821d534f",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsLocalUndockFeedbackSize(Ros1Clearpath_dock_msgsLocalUndockFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsLocalUndockFeedbackCapture(value),n=Ros1Clearpath_dock_msgsLocalUndockFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsLocalUndockFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsLocalUndockFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsLocalUndockFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsLocalUndockFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsLocalUndockFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsLocalUndockFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/NetworkDockGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsNetworkDockGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_d4b9ecb8f25cbbb6b0db567d93de10a739e9d4f5b5cc9a9744a45bd515de45ec",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dock_uuid":"","network_uuid":""};},
validateValue(value){Ros1Clearpath_dock_msgsNetworkDockGoalSize(Ros1Clearpath_dock_msgsNetworkDockGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsNetworkDockGoalCapture(value),n=Ros1Clearpath_dock_msgsNetworkDockGoalSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsNetworkDockGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsNetworkDockGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsNetworkDockGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["dock_uuid"],256).length);
n=addSize(n,4+textBytes(value["network_uuid"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_dock_msgsNetworkDockGoalWrite(value,w){
w.text(value["dock_uuid"],256);
w.text(value["network_uuid"],256);
}
function Ros1Clearpath_dock_msgsNetworkDockGoalRead(r){const value={};
{
const decoded=r.text(256);
value["dock_uuid"]=decoded;
}
{
const decoded=r.text(256);
value["network_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsNetworkDockGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dock_uuid"];
out["dock_uuid"]=field;
}
{const field=value["network_uuid"];
out["network_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/NetworkDockResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsNetworkDockResultCodec=Object.freeze({codecIdentity:"UWVMROS1_7f7886646fa1c9792d2ded3033e67f6476d5401a5d67536365d7b97b9318a913",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsNetworkDockResultSize(Ros1Clearpath_dock_msgsNetworkDockResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsNetworkDockResultCapture(value),n=Ros1Clearpath_dock_msgsNetworkDockResultSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsNetworkDockResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsNetworkDockResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsNetworkDockResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsNetworkDockResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsNetworkDockResultRead(r){const value={};
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
function Ros1Clearpath_dock_msgsNetworkDockResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
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
/** Complete ROS1 clearpath_navigation_msgs/Task codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsTaskCodec=Object.freeze({codecIdentity:"UWVMROS1_9800265a8c633618cc26019c169871c8f2cb53c56aebba1567b1de52f15ac658",maxWireBytes:1457,minWireBytes:25,cartesianMaxWireBytes:1457n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","uuid":"","action_server_name":"","version":"","floats":[],"strings":[],"allow_failure":false};},
validateValue(value){Ros1Clearpath_navigation_msgsTaskSize(Ros1Clearpath_navigation_msgsTaskCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsTaskCapture(value),n=Ros1Clearpath_navigation_msgsTaskSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsTaskWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1457,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsTaskRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsTaskSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["action_server_name"],256).length);
n=addSize(n,4+textBytes(value["version"],256).length);
if(!Array.isArray(value["floats"])||value["floats"].length>2)bad();
n=addSize(n,4);
for(const element of value["floats"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["strings"])||value["strings"].length>2)bad();
n=addSize(n,4);
for(const element of value["strings"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["allow_failure"]!=="boolean")bad();n=addSize(n,1);
if(n>1457)bad(90);return n;}
function Ros1Clearpath_navigation_msgsTaskWrite(value,w){
w.text(value["name"],128);
w.text(value["uuid"],256);
w.text(value["action_server_name"],256);
w.text(value["version"],256);
w.v.setUint32(w.at,value["floats"].length,true);w.at+=4;
for(const element of value["floats"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["strings"].length,true);w.at+=4;
for(const element of value["strings"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["allow_failure"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsTaskRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["action_server_name"]=decoded;
}
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["floats"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["floats"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["strings"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["strings"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["allow_failure"]=decoded!==0;
}
return value;}
function Ros1Clearpath_navigation_msgsTaskCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["action_server_name"];
out["action_server_name"]=field;
}
{const field=value["version"];
out["version"]=field;
}
{const field=value["floats"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["floats"]=values;
}
{const field=value["strings"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["strings"]=values;
}
{const field=value["allow_failure"];
out["allow_failure"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Waypoint codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsWaypointCodec=Object.freeze({codecIdentity:"UWVMROS1_68f7d2e826789aad21d6f15bd06060b6e0fb0bdac40bdc7de685c935afc93c30",maxWireBytes:3350,minWireBytes:52,cartesianMaxWireBytes:3350n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","latitude":0,"longitude":0,"heading":0,"position_tolerance":0,"yaw_tolerance":0,"tasks":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsWaypointSize(Ros1Clearpath_navigation_msgsWaypointCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsWaypointCapture(value),n=Ros1Clearpath_navigation_msgsWaypointSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsWaypointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3350,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsWaypointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsWaypointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(typeof value["position_tolerance"]!=="number"||!Number.isFinite(value["position_tolerance"]))bad();n=addSize(n,8);
if(typeof value["yaw_tolerance"]!=="number"||!Number.isFinite(value["yaw_tolerance"]))bad();n=addSize(n,8);
if(!Array.isArray(value["tasks"])||value["tasks"].length>2)bad();
n=addSize(n,4);
for(const element of value["tasks"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(n>3350)bad(90);return n;}
function Ros1Clearpath_navigation_msgsWaypointWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setFloat64(w.at,value["position_tolerance"],true);w.at+=8;
w.v.setFloat64(w.at,value["yaw_tolerance"],true);w.at+=8;
w.v.setUint32(w.at,value["tasks"].length,true);w.at+=4;
for(const element of value["tasks"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsWaypointRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position_tolerance"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["yaw_tolerance"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tasks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["tasks"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsWaypointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["position_tolerance"];
out["position_tolerance"]=field;
}
{const field=value["yaw_tolerance"];
out["yaw_tolerance"]=field;
}
{const field=value["tasks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["tasks"]=values;
}
return out;}
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
/** Complete ROS1 clearpath_navigation_msgs/NetworkMissionSegment codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkMissionSegmentCodec=Object.freeze({codecIdentity:"UWVMROS1_63c55d82f410637d740a5a5c93fe4f1e0d1539bdbce947eac474bb2472355877",maxWireBytes:3546,minWireBytes:72,cartesianMaxWireBytes:3546n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"goal":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue(),"points":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(value),n=Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3546,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["goal"]));
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Geometry_msgsPointSize(element));
}
if(n>3546)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Clearpath_navigation_msgsWaypointWrite(value["goal"],w);
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Geometry_msgsPointWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["goal"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["goal"];
out["goal"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPointCapture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/NetworkDockFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsNetworkDockFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_5b1b091da1ad18a3686cd6b3c57b2c181539a5b94e0f31dd2ddb89aa297bd6f6",maxWireBytes:3806,minWireBytes:76,cartesianMaxWireBytes:3806n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":"","network_plan":Ros1Clearpath_navigation_msgsNetworkMissionSegmentCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_dock_msgsNetworkDockFeedbackSize(Ros1Clearpath_dock_msgsNetworkDockFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsNetworkDockFeedbackCapture(value),n=Ros1Clearpath_dock_msgsNetworkDockFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsNetworkDockFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3806,90);if(owned.length<76)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsNetworkDockFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsNetworkDockFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(value["network_plan"]));
if(n>3806)bad(90);return n;}
function Ros1Clearpath_dock_msgsNetworkDockFeedbackWrite(value,w){
w.text(value["message"],256);
Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(value["network_plan"],w);
}
function Ros1Clearpath_dock_msgsNetworkDockFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);
value["network_plan"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsNetworkDockFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
{const field=value["network_plan"];
out["network_plan"]=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(field);
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/RadiusDockGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsRadiusDockGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_078d5852eab50c98283fcb512c9b9161ea1a710b5827b5738fbe0199323230a9",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dock_uuid":""};},
validateValue(value){Ros1Clearpath_dock_msgsRadiusDockGoalSize(Ros1Clearpath_dock_msgsRadiusDockGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsRadiusDockGoalCapture(value),n=Ros1Clearpath_dock_msgsRadiusDockGoalSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsRadiusDockGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsRadiusDockGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsRadiusDockGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["dock_uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsRadiusDockGoalWrite(value,w){
w.text(value["dock_uuid"],256);
}
function Ros1Clearpath_dock_msgsRadiusDockGoalRead(r){const value={};
{
const decoded=r.text(256);
value["dock_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsRadiusDockGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dock_uuid"];
out["dock_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/RadiusDockResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsRadiusDockResultCodec=Object.freeze({codecIdentity:"UWVMROS1_e4b1757e3b129f7de6427334ea3ad10d6bd7df27a0558af6130b20ab37a5c30a",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsRadiusDockResultSize(Ros1Clearpath_dock_msgsRadiusDockResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsRadiusDockResultCapture(value),n=Ros1Clearpath_dock_msgsRadiusDockResultSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsRadiusDockResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsRadiusDockResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsRadiusDockResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsRadiusDockResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsRadiusDockResultRead(r){const value={};
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
function Ros1Clearpath_dock_msgsRadiusDockResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/RadiusDockFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsRadiusDockFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_1a3467aced8c46a9d3c1146d69d9af4b1ec04849b52c2fb971ed12e6c587c048",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsRadiusDockFeedbackSize(Ros1Clearpath_dock_msgsRadiusDockFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsRadiusDockFeedbackCapture(value),n=Ros1Clearpath_dock_msgsRadiusDockFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsRadiusDockFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsRadiusDockFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsRadiusDockFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsRadiusDockFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsRadiusDockFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsRadiusDockFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SurveyBaseStationGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSurveyBaseStationGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_0cc3ffd35550c179c2cc3a7a40a6c0976ff2749c5405473d8b0504ff1bd2e904",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"number_of_desired_fixes":0};},
validateValue(value){Ros1Clearpath_localization_msgsSurveyBaseStationGoalSize(Ros1Clearpath_localization_msgsSurveyBaseStationGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSurveyBaseStationGoalCapture(value),n=Ros1Clearpath_localization_msgsSurveyBaseStationGoalSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSurveyBaseStationGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSurveyBaseStationGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSurveyBaseStationGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["number_of_desired_fixes"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Clearpath_localization_msgsSurveyBaseStationGoalWrite(value,w){
w.v.setUint32(w.at,value["number_of_desired_fixes"],true);w.at+=4;
}
function Ros1Clearpath_localization_msgsSurveyBaseStationGoalRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["number_of_desired_fixes"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsSurveyBaseStationGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["number_of_desired_fixes"];
out["number_of_desired_fixes"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SurveyBaseStationResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSurveyBaseStationResultCodec=Object.freeze({codecIdentity:"UWVMROS1_80e92d291c46cd247b66d76aef498a69a9a1289d55442d2fc170092728a1b466",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_localization_msgsSurveyBaseStationResultSize(Ros1Clearpath_localization_msgsSurveyBaseStationResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSurveyBaseStationResultCapture(value),n=Ros1Clearpath_localization_msgsSurveyBaseStationResultSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSurveyBaseStationResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSurveyBaseStationResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSurveyBaseStationResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_localization_msgsSurveyBaseStationResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_localization_msgsSurveyBaseStationResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_localization_msgsSurveyBaseStationResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SurveyBaseStationFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_d90857766b78e69fe9ccd7fcc4fc234e8411fcd162ef0d1912b3d11465ba3d72",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"percent_complete":0};},
validateValue(value){Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackSize(Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCapture(value),n=Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["percent_complete"]!=="number"||!Number.isFinite(value["percent_complete"])||!Number.isFinite(Math.fround(value["percent_complete"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackWrite(value,w){
w.v.setFloat32(w.at,value["percent_complete"],true);w.at+=4;
}
function Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["percent_complete"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["percent_complete"];
out["percent_complete"]=field;
}
return out;}
/** Complete ROS1 duration codec; maxima are not actual lengths. */
export const Ros1DurationCodec=Object.freeze({codecIdentity:"UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nsec":0};},
validateValue(value){Ros1DurationSize(Ros1DurationCapture(value));return true;},
encode(value){const owned=Ros1DurationCapture(value),n=Ros1DurationSize(owned),w=new Writer(n);Ros1DurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1DurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1DurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nsec"],4,true);n=addSize(n,4);
if(value["nsec"]<0||value["nsec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros1DurationWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setInt32(w.at,value["nsec"],true);w.at+=4;
}
function Ros1DurationRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["nsec"]=decoded;
}
if(value["nsec"]<0||value["nsec"]>=1000000000)bad();
return value;}
function Ros1DurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nsec"];
out["nsec"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/RunScheduleByUuidGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_cbe7e0318b365563a448441b29166d798a20ef92c0c65415f80c1a71a00cbf72",maxWireBytes:268,minWireBytes:12,cartesianMaxWireBytes:268n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","delay":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalSize(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCapture(value),n=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,268,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,Ros1DurationSize(value["delay"]));
if(n>268)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalWrite(value,w){
w.text(value["uuid"],256);
Ros1DurationWrite(value["delay"],w);
}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["delay"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["delay"];
out["delay"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/RunScheduleByUuidResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCodec=Object.freeze({codecIdentity:"UWVMROS1_01288b98deab00397457f14ea6817e320bff6448f839acfad0047654facb91d4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultSize(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCapture(value),n=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/RunScheduleByUuidFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_8b075ffb1cdf13580f5a4ddec2cb3d6edbeb88a9ba1e038d85a82a6029ec0f32",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_mission_uuid":"","state":""};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackSize(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCapture(value),n=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["current_mission_uuid"],256).length);
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackWrite(value,w){
w.text(value["current_mission_uuid"],256);
w.text(value["state"],256);
}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["current_mission_uuid"]=decoded;
}
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_mission_uuid"];
out["current_mission_uuid"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteMissionByUuidGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_2026328374c75acb27d33669d0e8a4aa7aa9527b90607c23e5aae986b2be7801",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","from_start":false,"start_waypoint_uuid":""};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalSize(Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCapture(value),n=Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["from_start"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["start_waypoint_uuid"],256).length);
if(n>521)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalWrite(value,w){
w.text(value["uuid"],256);
w.v.setUint8(w.at,value["from_start"]?1:0,true);w.at+=1;
w.text(value["start_waypoint_uuid"],256);
}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["from_start"]=decoded!==0;
}
{
const decoded=r.text(256);
value["start_waypoint_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["from_start"];
out["from_start"]=field;
}
{const field=value["start_waypoint_uuid"];
out["start_waypoint_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteMissionByUuidResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCodec=Object.freeze({codecIdentity:"UWVMROS1_5b93894ac84b85948b10ebe884bd74c1c0bc90031a86f407516badb1940a12f8",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultSize(Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCapture(value),n=Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteMissionByUuidFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_f86df88585d2f804900f3d362f53bcb1552c3981c806b9d5a3eb83be6023b8fb",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackSize(Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkGoToGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_0877cd8087712f31f6f7d6461f9a12eb23c62b0369b02b1363be5367e6bbe9a5",maxWireBytes:3610,minWireBytes:56,cartesianMaxWireBytes:3610n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_uuid":"","waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalSize(Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3610,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_uuid"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(n>3610)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalWrite(value,w){
w.text(value["map_uuid"],256);
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalRead(r){const value={};
{
const decoded=r.text(256);
value["map_uuid"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_uuid"];
out["map_uuid"]=field;
}
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkGoToResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCodec=Object.freeze({codecIdentity:"UWVMROS1_2b2841163f46c7b7debb7cfe15df2b60f32e103396455f68ed328a1a11cddeda",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({"UNKNOWN":0,"CANCELLED":1,"COLLISION":2,"PLANNING":3,"START_TASKS":4,"END_TASKS":5,"MISSION_TASKS":6}),
defaultValue(){return {"code":0,"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultSize(Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["code"],1,false);n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>262)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultWrite(value,w){
w.v.setUint8(w.at,value["code"],true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["code"]=decoded;
}
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
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkGoToFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_fe1de33d922bd1483c747ede295e8383a18848ae5911e66fb947fd110a196067",maxWireBytes:3806,minWireBytes:76,cartesianMaxWireBytes:3806n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":"","network_plan":Ros1Clearpath_navigation_msgsNetworkMissionSegmentCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackSize(Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3806,90);if(owned.length<76)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(value["network_plan"]));
if(n>3806)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackWrite(value,w){
w.text(value["message"],256);
Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(value["network_plan"],w);
}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);
value["network_plan"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
{const field=value["network_plan"];
out["network_plan"]=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkMissionByUuidGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_f75615051b84d90d373e7a16785d6cfaf7c48465ea3a644dc54711172a13be65",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission_uuid":"","map_uuid":""};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalSize(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["mission_uuid"],256).length);
n=addSize(n,4+textBytes(value["map_uuid"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalWrite(value,w){
w.text(value["mission_uuid"],256);
w.text(value["map_uuid"],256);
}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalRead(r){const value={};
{
const decoded=r.text(256);
value["mission_uuid"]=decoded;
}
{
const decoded=r.text(256);
value["map_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission_uuid"];
out["mission_uuid"]=field;
}
{const field=value["map_uuid"];
out["map_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkGoalState codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkGoalStateCodec=Object.freeze({codecIdentity:"UWVMROS1_2a039023aed39c0b8b3e91c1938a6b9b41c94e698639bc4b27fb377e9e5dde01",maxWireBytes:3351,minWireBytes:53,cartesianMaxWireBytes:3351n,aggregateWireBytes:0,constants:Object.freeze({"PENDING":0,"ACTIVE":1,"SUCCEEDED":2,"NAV_FAILURE":3,"TASK_WARNING":4,"TASK_FAILURE":5}),
defaultValue(){return {"state":0,"waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkGoalStateSize(Ros1Clearpath_navigation_msgsNetworkGoalStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkGoalStateCapture(value),n=Ros1Clearpath_navigation_msgsNetworkGoalStateSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkGoalStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3351,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkGoalStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkGoalStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["state"],1,false);n=addSize(n,1);
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(n>3351)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkGoalStateWrite(value,w){
w.v.setUint8(w.at,value["state"],true);w.at+=1;
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
}
function Ros1Clearpath_navigation_msgsNetworkGoalStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["state"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkGoalStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkMissionByUuidResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCodec=Object.freeze({codecIdentity:"UWVMROS1_5bdab4569bc5bb4f94910b294acf04708f4677f6a1adcc326ee0420edf6de856",maxWireBytes:6968,minWireBytes:10,cartesianMaxWireBytes:6968n,aggregateWireBytes:0,constants:Object.freeze({"UNKNOWN":0,"CANCELLED":1,"COLLISION":2,"PLANNING":3,"START_TASKS":4,"END_TASKS":5,"MISSION_TASKS":6}),
defaultValue(){return {"code":0,"success":false,"message":"","goal_states":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultSize(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6968,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["code"],1,false);n=addSize(n,1);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(!Array.isArray(value["goal_states"])||value["goal_states"].length>2)bad();
n=addSize(n,4);
for(const element of value["goal_states"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkGoalStateSize(element));
}
if(n>6968)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultWrite(value,w){
w.v.setUint8(w.at,value["code"],true);w.at+=1;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
w.v.setUint32(w.at,value["goal_states"].length,true);w.at+=4;
for(const element of value["goal_states"]){
Ros1Clearpath_navigation_msgsNetworkGoalStateWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["code"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["goal_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkGoalStateRead(r);
value["goal_states"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["goal_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkGoalStateCapture(element);
}out["goal_states"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ExecuteNetworkMissionByUuidFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_f36146854331b1ff188708a23717889180d6bda4fed7ae6c6e758782372d0927",maxWireBytes:10512,minWireBytes:80,cartesianMaxWireBytes:10512n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":"","goal_states":[],"network_plan":Ros1Clearpath_navigation_msgsNetworkMissionSegmentCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackSize(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10512,90);if(owned.length<80)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(!Array.isArray(value["goal_states"])||value["goal_states"].length>2)bad();
n=addSize(n,4);
for(const element of value["goal_states"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkGoalStateSize(element));
}
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(value["network_plan"]));
if(n>10512)bad(90);return n;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackWrite(value,w){
w.text(value["message"],256);
w.v.setUint32(w.at,value["goal_states"].length,true);w.at+=4;
for(const element of value["goal_states"]){
Ros1Clearpath_navigation_msgsNetworkGoalStateWrite(element,w);
}
Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(value["network_plan"],w);
}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["goal_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkGoalStateRead(r);
value["goal_states"].push(decoded);
}}
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);
value["network_plan"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
{const field=value["goal_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkGoalStateCapture(element);
}out["goal_states"]=values;
}
{const field=value["network_plan"];
out["network_plan"]=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(field);
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
/** Complete ROS1 clearpath_navigation_msgs/GetWaypointPathGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsGetWaypointPathGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_9f4e55ff8c5c423e58bc206b5ba8cf40f2dce89fe6b6194e3f274f33915c8373",maxWireBytes:874,minWireBytes:90,cartesianMaxWireBytes:874n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"waypoints":[],"use_start_pose":false,"start_pose":Ros1Geometry_msgsPoseStampedCodec.defaultValue(),"tolerance":0,"planner":"","concurrency_slot":0};},
validateValue(value){Ros1Clearpath_navigation_msgsGetWaypointPathGoalSize(Ros1Clearpath_navigation_msgsGetWaypointPathGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsGetWaypointPathGoalCapture(value),n=Ros1Clearpath_navigation_msgsGetWaypointPathGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsGetWaypointPathGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,874,90);if(owned.length<90)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsGetWaypointPathGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsGetWaypointPathGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(element));
}
if(typeof value["use_start_pose"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(value["start_pose"]));
if(typeof value["tolerance"]!=="number"||!Number.isFinite(value["tolerance"]))bad();n=addSize(n,8);
n=addSize(n,4+textBytes(value["planner"],256).length);
integer(value["concurrency_slot"],1,false);n=addSize(n,1);
if(n>874)bad(90);return n;}
function Ros1Clearpath_navigation_msgsGetWaypointPathGoalWrite(value,w){
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Geometry_msgsPoseStampedWrite(element,w);
}
w.v.setUint8(w.at,value["use_start_pose"]?1:0,true);w.at+=1;
Ros1Geometry_msgsPoseStampedWrite(value["start_pose"],w);
w.v.setFloat64(w.at,value["tolerance"],true);w.at+=8;
w.text(value["planner"],256);
w.v.setUint8(w.at,value["concurrency_slot"],true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsGetWaypointPathGoalRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["waypoints"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_start_pose"]=decoded!==0;
}
{
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["start_pose"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tolerance"]=decoded;
}
{
const decoded=r.text(256);
value["planner"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["concurrency_slot"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsGetWaypointPathGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPoseStampedCapture(element);
}out["waypoints"]=values;
}
{const field=value["use_start_pose"];
out["use_start_pose"]=field;
}
{const field=value["start_pose"];
out["start_pose"]=Ros1Geometry_msgsPoseStampedCapture(field);
}
{const field=value["tolerance"];
out["tolerance"]=field;
}
{const field=value["planner"];
out["planner"]=field;
}
{const field=value["concurrency_slot"];
out["concurrency_slot"]=field;
}
return out;}
/** Complete ROS1 nav_msgs/Path codec; maxima are not actual lengths. */
export const Ros1Nav_msgsPathCodec=Object.freeze({codecIdentity:"UWVMROS1_4451806e82167229ab44f7a8f9f3de7bdcec2e8b7c98e8765b029c7e76af7f60",maxWireBytes:548,minWireBytes:20,cartesianMaxWireBytes:548n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"poses":[]};},
validateValue(value){Ros1Nav_msgsPathSize(Ros1Nav_msgsPathCapture(value));return true;},
encode(value){const owned=Ros1Nav_msgsPathCapture(value),n=Ros1Nav_msgsPathSize(owned),w=new Writer(n);Ros1Nav_msgsPathWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,548,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Nav_msgsPathRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Nav_msgsPathSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["poses"])||value["poses"].length>2)bad();
n=addSize(n,4);
for(const element of value["poses"]){
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(element));
}
if(n>548)bad(90);return n;}
function Ros1Nav_msgsPathWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["poses"].length,true);w.at+=4;
for(const element of value["poses"]){
Ros1Geometry_msgsPoseStampedWrite(element,w);
}
}
function Ros1Nav_msgsPathRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["poses"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["poses"].push(decoded);
}}
}
return value;}
function Ros1Nav_msgsPathCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["poses"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPoseStampedCapture(element);
}out["poses"]=values;
}
return out;}
/** Complete ROS1 std_msgs/Duration codec; maxima are not actual lengths. */
export const Ros1Std_msgsDurationCodec=Object.freeze({codecIdentity:"UWVMROS1_195444342590aabe87833226c1cce7cc900519e5b1fe13ff460406396e3ce175",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Std_msgsDurationSize(Ros1Std_msgsDurationCapture(value));return true;},
encode(value){const owned=Ros1Std_msgsDurationCapture(value),n=Ros1Std_msgsDurationSize(owned),w=new Writer(n);Ros1Std_msgsDurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Std_msgsDurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_msgsDurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1DurationSize(value["data"]));
if(n>8)bad(90);return n;}
function Ros1Std_msgsDurationWrite(value,w){
Ros1DurationWrite(value["data"],w);
}
function Ros1Std_msgsDurationRead(r){const value={};
{
const decoded=Ros1DurationRead(r);
value["data"]=decoded;
}
return value;}
function Ros1Std_msgsDurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/GetWaypointPathResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsGetWaypointPathResultCodec=Object.freeze({codecIdentity:"UWVMROS1_e380ef98ced5ac0bb2edff0accf0df86828cc9d9a8d17c73d7b236b8f660c396",maxWireBytes:828,minWireBytes:44,cartesianMaxWireBytes:828n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESS":0,"FAILURE":50,"CANCELED":51,"INVALID_START":52,"INVALID_GOAL":53,"NO_PATH_FOUND":54,"PAT_EXCEEDED":55,"EMPTY_PATH":56,"TF_ERROR":57,"NOT_INITIALIZED":58,"INVALID_PLUGIN":59,"INTERNAL_ERROR":60,"OUT_OF_MAP":61,"MAP_ERROR":62,"STOPPED":63}),
defaultValue(){return {"outcome":0,"message":"","path":Ros1Nav_msgsPathCodec.defaultValue(),"cost":0,"planning_time":Ros1Std_msgsDurationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsGetWaypointPathResultSize(Ros1Clearpath_navigation_msgsGetWaypointPathResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsGetWaypointPathResultCapture(value),n=Ros1Clearpath_navigation_msgsGetWaypointPathResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsGetWaypointPathResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,828,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsGetWaypointPathResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsGetWaypointPathResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["outcome"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],256).length);
n=addSize(n,Ros1Nav_msgsPathSize(value["path"]));
if(typeof value["cost"]!=="number"||!Number.isFinite(value["cost"]))bad();n=addSize(n,8);
n=addSize(n,Ros1Std_msgsDurationSize(value["planning_time"]));
if(n>828)bad(90);return n;}
function Ros1Clearpath_navigation_msgsGetWaypointPathResultWrite(value,w){
w.v.setUint32(w.at,value["outcome"],true);w.at+=4;
w.text(value["message"],256);
Ros1Nav_msgsPathWrite(value["path"],w);
w.v.setFloat64(w.at,value["cost"],true);w.at+=8;
Ros1Std_msgsDurationWrite(value["planning_time"],w);
}
function Ros1Clearpath_navigation_msgsGetWaypointPathResultRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["outcome"]=decoded;
}
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
const decoded=Ros1Nav_msgsPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["cost"]=decoded;
}
{
const decoded=Ros1Std_msgsDurationRead(r);
value["planning_time"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsGetWaypointPathResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["outcome"];
out["outcome"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["path"];
out["path"]=Ros1Nav_msgsPathCapture(field);
}
{const field=value["cost"];
out["cost"]=field;
}
{const field=value["planning_time"];
out["planning_time"]=Ros1Std_msgsDurationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/GetWaypointPathFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_d1e0f84cfd174723eaae1a46d298702562ce42b19875146e18bf58ac6a1c9cec",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackSize(Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackWrite(value,w){
}
function Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackRead(r){const value={};
return value;}
function Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Mission codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMissionCodec=Object.freeze({codecIdentity:"UWVMROS1_7a2e7409a879989eedd02009b13a33cc775188ee2af9409a3a47ed8fd38d4490",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"name":"","uuid":"","waypoints":[],"on_start":[],"on_stop":[],"onav_config":""};},
validateValue(value){Ros1Clearpath_navigation_msgsMissionSize(Ros1Clearpath_navigation_msgsMissionCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMissionCapture(value),n=Ros1Clearpath_navigation_msgsMissionSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMissionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMissionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMissionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(element));
}
if(!Array.isArray(value["on_start"])||value["on_start"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(!Array.isArray(value["on_stop"])||value["on_stop"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(n>13336)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMissionWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["name"],128);
w.text(value["uuid"],256);
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Clearpath_navigation_msgsWaypointWrite(element,w);
}
w.v.setUint32(w.at,value["on_start"].length,true);w.at+=4;
for(const element of value["on_start"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
w.v.setUint32(w.at,value["on_stop"].length,true);w.at+=4;
for(const element of value["on_stop"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
w.text(value["onav_config"],256);
}
function Ros1Clearpath_navigation_msgsMissionRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoints"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["on_start"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["on_stop"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsMissionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsWaypointCapture(element);
}out["waypoints"]=values;
}
{const field=value["on_start"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["on_start"]=values;
}
{const field=value["on_stop"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["on_stop"]=values;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/MissionGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMissionGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_d763e90ea09778c5cf9d16bb789eaec779b81a3527fec14fa556b2a2fbfe41bc",maxWireBytes:16687,minWireBytes:93,cartesianMaxWireBytes:16687n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue(),"from_start":false,"start_waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsMissionGoalSize(Ros1Clearpath_navigation_msgsMissionGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMissionGoalCapture(value),n=Ros1Clearpath_navigation_msgsMissionGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMissionGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16687,90);if(owned.length<93)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMissionGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMissionGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["mission"]));
if(typeof value["from_start"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["start_waypoint"]));
if(n>16687)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMissionGoalWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["mission"],w);
w.v.setUint8(w.at,value["from_start"]?1:0,true);w.at+=1;
Ros1Clearpath_navigation_msgsWaypointWrite(value["start_waypoint"],w);
}
function Ros1Clearpath_navigation_msgsMissionGoalRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["mission"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["from_start"]=decoded!==0;
}
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["start_waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsMissionGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
{const field=value["from_start"];
out["from_start"]=field;
}
{const field=value["start_waypoint"];
out["start_waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/MissionResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMissionResultCodec=Object.freeze({codecIdentity:"UWVMROS1_9f8dcc6b43b368957630f5dd039ec3a7e3bc1091a04a3acd639a9571293628a2",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsMissionResultSize(Ros1Clearpath_navigation_msgsMissionResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMissionResultCapture(value),n=Ros1Clearpath_navigation_msgsMissionResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMissionResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMissionResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMissionResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMissionResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsMissionResultRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsMissionResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/MissionFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMissionFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_dfb40538ac8673b076a60565a1bca3b9805b7cf6a0ab5b50a46cbc6cdd59f2ef",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsMissionFeedbackSize(Ros1Clearpath_navigation_msgsMissionFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMissionFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsMissionFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMissionFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMissionFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMissionFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMissionFeedbackWrite(value,w){
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsMissionFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsMissionFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkGoToPOIGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_f8c1dddcecc439487e6811465240e02f26291fe68ae773d9cbf2a61a976de4bd",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"poi_uuid":"","map_uuid":""};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalSize(Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCapture(value),n=Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["poi_uuid"],256).length);
n=addSize(n,4+textBytes(value["map_uuid"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalWrite(value,w){
w.text(value["poi_uuid"],256);
w.text(value["map_uuid"],256);
}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalRead(r){const value={};
{
const decoded=r.text(256);
value["poi_uuid"]=decoded;
}
{
const decoded=r.text(256);
value["map_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["poi_uuid"];
out["poi_uuid"]=field;
}
{const field=value["map_uuid"];
out["map_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkGoToPOIResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCodec=Object.freeze({codecIdentity:"UWVMROS1_402e95779afd664798a412dbebce299c4a4cc332c5d01bda4b19f2c24023c548",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkGoToPOIResultSize(Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCapture(value),n=Ros1Clearpath_navigation_msgsNetworkGoToPOIResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkGoToPOIResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkGoToPOIResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkGoToPOIResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIResultRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkGoToPOIFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_77676f75545d6eb58b93904be926923f749a6ddc7332476dd23e635ed0e072e8",maxWireBytes:3806,minWireBytes:76,cartesianMaxWireBytes:3806n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"message":"","network_plan":Ros1Clearpath_navigation_msgsNetworkMissionSegmentCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackSize(Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3806,90);if(owned.length<76)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["message"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(value["network_plan"]));
if(n>3806)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackWrite(value,w){
w.text(value["message"],256);
Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(value["network_plan"],w);
}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["message"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);
value["network_plan"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["message"];
out["message"]=field;
}
{const field=value["network_plan"];
out["network_plan"]=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RecordPathGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRecordPathGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_64cc3f4f8c7cd725d8a5a1a7286cacafd7a41ab74e8d664dc10149ac92b0cf68",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"max_deviation":0,"min_distance":0};},
validateValue(value){Ros1Clearpath_navigation_msgsRecordPathGoalSize(Ros1Clearpath_navigation_msgsRecordPathGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRecordPathGoalCapture(value),n=Ros1Clearpath_navigation_msgsRecordPathGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRecordPathGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRecordPathGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRecordPathGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["max_deviation"]!=="number"||!Number.isFinite(value["max_deviation"]))bad();n=addSize(n,8);
if(typeof value["min_distance"]!=="number"||!Number.isFinite(value["min_distance"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRecordPathGoalWrite(value,w){
w.v.setFloat64(w.at,value["max_deviation"],true);w.at+=8;
w.v.setFloat64(w.at,value["min_distance"],true);w.at+=8;
}
function Ros1Clearpath_navigation_msgsRecordPathGoalRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["max_deviation"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["min_distance"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsRecordPathGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["max_deviation"];
out["max_deviation"]=field;
}
{const field=value["min_distance"];
out["min_distance"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Vector2D64 codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsVector2D64Codec=Object.freeze({codecIdentity:"UWVMROS1_13edfb447d3fdb131755cd00318a6ce89fc45096b9e41440b021231b324caee9",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"point":Array.from({length:2},()=>(0))};},
validateValue(value){Ros1Clearpath_navigation_msgsVector2D64Size(Ros1Clearpath_navigation_msgsVector2D64Capture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsVector2D64Capture(value),n=Ros1Clearpath_navigation_msgsVector2D64Size(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsVector2D64Write(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsVector2D64Read(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsVector2D64Size(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["point"])||value["point"].length!==2)bad();
for(const element of value["point"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>16)bad(90);return n;}
function Ros1Clearpath_navigation_msgsVector2D64Write(value,w){
for(const element of value["point"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Clearpath_navigation_msgsVector2D64Read(r){const value={};
{
{const count=2;if(count>2)bad();value["point"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["point"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsVector2D64Capture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["point"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["point"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RecordPathResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRecordPathResultCodec=Object.freeze({codecIdentity:"UWVMROS1_e2b420a2149b030f0aac9bba36797abfcb705d6478679dfe85b00e83be0ebe98",maxWireBytes:36,minWireBytes:4,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"points":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsRecordPathResultSize(Ros1Clearpath_navigation_msgsRecordPathResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRecordPathResultCapture(value),n=Ros1Clearpath_navigation_msgsRecordPathResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRecordPathResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRecordPathResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRecordPathResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Clearpath_navigation_msgsVector2D64Size(element));
}
if(n>36)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRecordPathResultWrite(value,w){
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Clearpath_navigation_msgsVector2D64Write(element,w);
}
}
function Ros1Clearpath_navigation_msgsRecordPathResultRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsVector2D64Read(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsRecordPathResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsVector2D64Capture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RecordPathFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRecordPathFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_3d12790afde54b03439f03f1fd1453d746fbfeddf8a8cfd2cf0af0f2a4e8ee7a",maxWireBytes:36,minWireBytes:4,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"points":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsRecordPathFeedbackSize(Ros1Clearpath_navigation_msgsRecordPathFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRecordPathFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsRecordPathFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRecordPathFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRecordPathFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRecordPathFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Clearpath_navigation_msgsVector2D64Size(element));
}
if(n>36)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRecordPathFeedbackWrite(value,w){
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Clearpath_navigation_msgsVector2D64Write(element,w);
}
}
function Ros1Clearpath_navigation_msgsRecordPathFeedbackRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsVector2D64Read(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsRecordPathFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsVector2D64Capture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkGoToPlannerGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_95c219ff4985d097b9bd8e24f7811fb1090ce3d0d977f6d53f7ad8191cfeb4dc",maxWireBytes:3610,minWireBytes:56,cartesianMaxWireBytes:3610n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_uuid":"","waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalSize(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3610,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_uuid"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(n>3610)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalWrite(value,w){
w.text(value["map_uuid"],256);
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalRead(r){const value={};
{
const decoded=r.text(256);
value["map_uuid"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_uuid"];
out["map_uuid"]=field;
}
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkGoToPlannerResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCodec=Object.freeze({codecIdentity:"UWVMROS1_74a36e238244de86de447c97874d3b22a8cea29c61c0000a38cb97981bd0ffad",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultSize(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkGoToPlannerFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_0a0c6aa1c129a3c64ab4fc70915d8d4fe2c320b24d0a127f61b09b55c091f0be",maxWireBytes:196,minWireBytes:20,cartesianMaxWireBytes:196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"points":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackSize(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,196,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Geometry_msgsPointSize(element));
}
if(n>196)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Geometry_msgsPointWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPointCapture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkMissionPreviewerGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_80e350b2bcb3fe42a76d2a717ca07e57f7d9a31ba2f21066d5f2b59822ec8966",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_uuid":"","mission_uuid":""};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalSize(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_uuid"],256).length);
n=addSize(n,4+textBytes(value["mission_uuid"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalWrite(value,w){
w.text(value["map_uuid"],256);
w.text(value["mission_uuid"],256);
}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalRead(r){const value={};
{
const decoded=r.text(256);
value["map_uuid"]=decoded;
}
{
const decoded=r.text(256);
value["mission_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_uuid"];
out["map_uuid"]=field;
}
{const field=value["mission_uuid"];
out["mission_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkMissionPreviewerResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCodec=Object.freeze({codecIdentity:"UWVMROS1_57d1973b7ae42cf0dbba42920caf934cb9ce1dee9c934e11b7044e52b4fc7b10",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultSize(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/RunNetworkMissionPreviewerFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_5e2c543ebca10579806d00dd9b7cc5c302e31a2fcf54321ff07f55fa58ec8e89",maxWireBytes:7096,minWireBytes:4,cartesianMaxWireBytes:7096n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"segments":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackSize(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7096,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["segments"])||value["segments"].length>2)bad();
n=addSize(n,4);
for(const element of value["segments"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSegmentSize(element));
}
if(n>7096)bad(90);return n;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackWrite(value,w){
w.v.setUint32(w.at,value["segments"].length,true);w.at+=4;
for(const element of value["segments"]){
Ros1Clearpath_navigation_msgsNetworkMissionSegmentWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["segments"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionSegmentRead(r);
value["segments"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["segments"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkMissionSegmentCapture(element);
}out["segments"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SmoothPathGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSmoothPathGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_bed6cefe852634fd4fa352eb0413c69583559346cc66772514345d4c71ae8a05",maxWireBytes:580,minWireBytes:52,cartesianMaxWireBytes:580n,aggregateWireBytes:0,constants:Object.freeze({"CUBIC_SPLINE_SMOOTHER":1,"DUBINS_SMOOTHER":2,"GENERIC_SMOOTHER":3}),
defaultValue(){return {"path":Ros1Nav_msgsPathCodec.defaultValue(),"type":0,"spline_points_per_unit":0,"spline_skip_points":0,"spline_use_end_conditions":false,"spline_use_middle_conditions":false,"dubins_turning_radius":0,"dubins_step_size":0,"generic_enable_refinement":false,"generic_refinement_iterations":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSmoothPathGoalSize(Ros1Clearpath_navigation_msgsSmoothPathGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSmoothPathGoalCapture(value),n=Ros1Clearpath_navigation_msgsSmoothPathGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSmoothPathGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,580,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSmoothPathGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSmoothPathGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Nav_msgsPathSize(value["path"]));
integer(value["type"],1,false);n=addSize(n,1);
integer(value["spline_points_per_unit"],4,true);n=addSize(n,4);
integer(value["spline_skip_points"],4,true);n=addSize(n,4);
if(typeof value["spline_use_end_conditions"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["spline_use_middle_conditions"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["dubins_turning_radius"]!=="number"||!Number.isFinite(value["dubins_turning_radius"]))bad();n=addSize(n,8);
if(typeof value["dubins_step_size"]!=="number"||!Number.isFinite(value["dubins_step_size"]))bad();n=addSize(n,8);
if(typeof value["generic_enable_refinement"]!=="boolean")bad();n=addSize(n,1);
integer(value["generic_refinement_iterations"],4,true);n=addSize(n,4);
if(n>580)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSmoothPathGoalWrite(value,w){
Ros1Nav_msgsPathWrite(value["path"],w);
w.v.setUint8(w.at,value["type"],true);w.at+=1;
w.v.setInt32(w.at,value["spline_points_per_unit"],true);w.at+=4;
w.v.setInt32(w.at,value["spline_skip_points"],true);w.at+=4;
w.v.setUint8(w.at,value["spline_use_end_conditions"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["spline_use_middle_conditions"]?1:0,true);w.at+=1;
w.v.setFloat64(w.at,value["dubins_turning_radius"],true);w.at+=8;
w.v.setFloat64(w.at,value["dubins_step_size"],true);w.at+=8;
w.v.setUint8(w.at,value["generic_enable_refinement"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["generic_refinement_iterations"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsSmoothPathGoalRead(r){const value={};
{
const decoded=Ros1Nav_msgsPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["type"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["spline_points_per_unit"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["spline_skip_points"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["spline_use_end_conditions"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["spline_use_middle_conditions"]=decoded!==0;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dubins_turning_radius"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dubins_step_size"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["generic_enable_refinement"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["generic_refinement_iterations"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSmoothPathGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Nav_msgsPathCapture(field);
}
{const field=value["type"];
out["type"]=field;
}
{const field=value["spline_points_per_unit"];
out["spline_points_per_unit"]=field;
}
{const field=value["spline_skip_points"];
out["spline_skip_points"]=field;
}
{const field=value["spline_use_end_conditions"];
out["spline_use_end_conditions"]=field;
}
{const field=value["spline_use_middle_conditions"];
out["spline_use_middle_conditions"]=field;
}
{const field=value["dubins_turning_radius"];
out["dubins_turning_radius"]=field;
}
{const field=value["dubins_step_size"];
out["dubins_step_size"]=field;
}
{const field=value["generic_enable_refinement"];
out["generic_enable_refinement"]=field;
}
{const field=value["generic_refinement_iterations"];
out["generic_refinement_iterations"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SmoothPathResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSmoothPathResultCodec=Object.freeze({codecIdentity:"UWVMROS1_f42205bb1dad95ed43d6d8f9159ea3803a944050dcfc5b840309e9fd104bf5f9",maxWireBytes:556,minWireBytes:28,cartesianMaxWireBytes:556n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"smoothed_path":Ros1Nav_msgsPathCodec.defaultValue(),"time":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSmoothPathResultSize(Ros1Clearpath_navigation_msgsSmoothPathResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSmoothPathResultCapture(value),n=Ros1Clearpath_navigation_msgsSmoothPathResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSmoothPathResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,556,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSmoothPathResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSmoothPathResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Nav_msgsPathSize(value["smoothed_path"]));
if(typeof value["time"]!=="number"||!Number.isFinite(value["time"]))bad();n=addSize(n,8);
if(n>556)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSmoothPathResultWrite(value,w){
Ros1Nav_msgsPathWrite(value["smoothed_path"],w);
w.v.setFloat64(w.at,value["time"],true);w.at+=8;
}
function Ros1Clearpath_navigation_msgsSmoothPathResultRead(r){const value={};
{
const decoded=Ros1Nav_msgsPathRead(r);
value["smoothed_path"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["time"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSmoothPathResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["smoothed_path"];
out["smoothed_path"]=Ros1Nav_msgsPathCapture(field);
}
{const field=value["time"];
out["time"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SmoothPathFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSmoothPathFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_f5ab7d56fd2811fd4caac7fe44c6a1c7a4f6b5593ee1903251bf312a979c09d1",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sequence":""};},
validateValue(value){Ros1Clearpath_navigation_msgsSmoothPathFeedbackSize(Ros1Clearpath_navigation_msgsSmoothPathFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSmoothPathFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsSmoothPathFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSmoothPathFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSmoothPathFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSmoothPathFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["sequence"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSmoothPathFeedbackWrite(value,w){
w.text(value["sequence"],256);
}
function Ros1Clearpath_navigation_msgsSmoothPathFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["sequence"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSmoothPathFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sequence"];
out["sequence"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/UITaskGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsUITaskGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_29c35437efebdb8c8c42b2ab4ed29f81e6f8e91c300380ba7099dd68a862ef9b",maxWireBytes:804,minWireBytes:12,cartesianMaxWireBytes:804n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version":"","floats":[],"strings":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsUITaskGoalSize(Ros1Clearpath_navigation_msgsUITaskGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsUITaskGoalCapture(value),n=Ros1Clearpath_navigation_msgsUITaskGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsUITaskGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,804,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsUITaskGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsUITaskGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["version"],256).length);
if(!Array.isArray(value["floats"])||value["floats"].length>2)bad();
n=addSize(n,4);
for(const element of value["floats"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["strings"])||value["strings"].length>2)bad();
n=addSize(n,4);
for(const element of value["strings"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>804)bad(90);return n;}
function Ros1Clearpath_navigation_msgsUITaskGoalWrite(value,w){
w.text(value["version"],256);
w.v.setUint32(w.at,value["floats"].length,true);w.at+=4;
for(const element of value["floats"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["strings"].length,true);w.at+=4;
for(const element of value["strings"]){
w.text(element,256);
}
}
function Ros1Clearpath_navigation_msgsUITaskGoalRead(r){const value={};
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["floats"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["floats"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["strings"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["strings"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsUITaskGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version"];
out["version"]=field;
}
{const field=value["floats"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["floats"]=values;
}
{const field=value["strings"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["strings"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/UITaskResult codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsUITaskResultCodec=Object.freeze({codecIdentity:"UWVMROS1_f88d16093a93f26c60a09ed08a0944ba8b3da03fda60d514dce4a8e48533b3ad",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_navigation_msgsUITaskResultSize(Ros1Clearpath_navigation_msgsUITaskResultCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsUITaskResultCapture(value),n=Ros1Clearpath_navigation_msgsUITaskResultSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsUITaskResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsUITaskResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsUITaskResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_navigation_msgsUITaskResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsUITaskResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_navigation_msgsUITaskResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/UITaskFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsUITaskFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_bb14cbf36189f1bc9759c168619ab4cd6be5d68147658ac54535f16d36f2960f",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":""};},
validateValue(value){Ros1Clearpath_navigation_msgsUITaskFeedbackSize(Ros1Clearpath_navigation_msgsUITaskFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsUITaskFeedbackCapture(value),n=Ros1Clearpath_navigation_msgsUITaskFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsUITaskFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsUITaskFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsUITaskFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_navigation_msgsUITaskFeedbackWrite(value,w){
w.text(value["state"],256);
}
function Ros1Clearpath_navigation_msgsUITaskFeedbackRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsUITaskFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 clearpath_configuration_msgs/AutonomyConfig codec; maxima are not actual lengths. */
export const Ros1Clearpath_configuration_msgsAutonomyConfigCodec=Object.freeze({codecIdentity:"UWVMROS1_94bd6352cf4c46adffcfffd54ad5ac5e1b0cb4aadc98ea00eeb9c62a8ec5be2c",maxWireBytes:268,minWireBytes:12,cartesianMaxWireBytes:268n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version":"","uptime":0};},
validateValue(value){Ros1Clearpath_configuration_msgsAutonomyConfigSize(Ros1Clearpath_configuration_msgsAutonomyConfigCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_configuration_msgsAutonomyConfigCapture(value),n=Ros1Clearpath_configuration_msgsAutonomyConfigSize(owned),w=new Writer(n);Ros1Clearpath_configuration_msgsAutonomyConfigWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,268,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_configuration_msgsAutonomyConfigRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_configuration_msgsAutonomyConfigSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["version"],256).length);
if(typeof value["uptime"]!=="number"||!Number.isFinite(value["uptime"]))bad();n=addSize(n,8);
if(n>268)bad(90);return n;}
function Ros1Clearpath_configuration_msgsAutonomyConfigWrite(value,w){
w.text(value["version"],256);
w.v.setFloat64(w.at,value["uptime"],true);w.at+=8;
}
function Ros1Clearpath_configuration_msgsAutonomyConfigRead(r){const value={};
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["uptime"]=decoded;
}
return value;}
function Ros1Clearpath_configuration_msgsAutonomyConfigCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version"];
out["version"]=field;
}
{const field=value["uptime"];
out["uptime"]=field;
}
return out;}
/** Complete ROS1 clearpath_configuration_msgs/PlatformConfig codec; maxima are not actual lengths. */
export const Ros1Clearpath_configuration_msgsPlatformConfigCodec=Object.freeze({codecIdentity:"UWVMROS1_8ec6a196a91bd5d6cd4d4ea2af14b5c549d8efe82c1da95ec08903bb4cbc5407",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_configuration_msgsPlatformConfigSize(Ros1Clearpath_configuration_msgsPlatformConfigCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_configuration_msgsPlatformConfigCapture(value),n=Ros1Clearpath_configuration_msgsPlatformConfigSize(owned),w=new Writer(n);Ros1Clearpath_configuration_msgsPlatformConfigWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_configuration_msgsPlatformConfigRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_configuration_msgsPlatformConfigSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_configuration_msgsPlatformConfigWrite(value,w){
}
function Ros1Clearpath_configuration_msgsPlatformConfigRead(r){const value={};
return value;}
function Ros1Clearpath_configuration_msgsPlatformConfigCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_control_msgs/AutonomySync codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsAutonomySyncCodec=Object.freeze({codecIdentity:"UWVMROS1_55bd50b3af5341d9a4793156e250a75451dfc293f5b18036d493c22cc23ca7a4",maxWireBytes:7,minWireBytes:5,cartesianMaxWireBytes:7n,aggregateWireBytes:0,constants:Object.freeze({"EMPTY":0,"PROCESSING":1,"PENDING_PAUSE":2,"PAUSED":3,"IDLE":4,"TEACH":5,"REPEAT":6,"MANNED":7,"UNMANNED":8}),
defaultValue(){return {"status":0,"missionType":[]};},
validateValue(value){Ros1Clearpath_control_msgsAutonomySyncSize(Ros1Clearpath_control_msgsAutonomySyncCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsAutonomySyncCapture(value),n=Ros1Clearpath_control_msgsAutonomySyncSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsAutonomySyncWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsAutonomySyncRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsAutonomySyncSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],1,false);n=addSize(n,1);
if(!Array.isArray(value["missionType"])||value["missionType"].length>2)bad();
n=addSize(n,4);
for(const element of value["missionType"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>7)bad(90);return n;}
function Ros1Clearpath_control_msgsAutonomySyncWrite(value,w){
w.v.setUint8(w.at,value["status"],true);w.at+=1;
w.v.setUint32(w.at,value["missionType"].length,true);w.at+=4;
for(const element of value["missionType"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros1Clearpath_control_msgsAutonomySyncRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["status"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["missionType"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["missionType"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_control_msgsAutonomySyncCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["missionType"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["missionType"]=values;
}
return out;}
/** Complete ROS1 clearpath_control_msgs/ControlMode codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsControlModeCodec=Object.freeze({codecIdentity:"UWVMROS1_654f836cb5e86bdc3c861109e4c976683f6368e8d856c7145642c6b2c7260cfc",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"NEUTRAL":0,"MANUAL":1,"AUTONOMY":2}),
defaultValue(){return {"mode":0};},
validateValue(value){Ros1Clearpath_control_msgsControlModeSize(Ros1Clearpath_control_msgsControlModeCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsControlModeCapture(value),n=Ros1Clearpath_control_msgsControlModeSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsControlModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsControlModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsControlModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_control_msgsControlModeWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
}
function Ros1Clearpath_control_msgsControlModeRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["mode"]=decoded;
}
return value;}
function Ros1Clearpath_control_msgsControlModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
return out;}
/** Complete ROS1 clearpath_control_msgs/ControlState codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsControlStateCodec=Object.freeze({codecIdentity:"UWVMROS1_3553232b4b685317bcb311ee342d756f535155d72b33a58b1346eb036badd860",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enabled":false,"paused":false};},
validateValue(value){Ros1Clearpath_control_msgsControlStateSize(Ros1Clearpath_control_msgsControlStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsControlStateCapture(value),n=Ros1Clearpath_control_msgsControlStateSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsControlStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsControlStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsControlStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["paused"]!=="boolean")bad();n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros1Clearpath_control_msgsControlStateWrite(value,w){
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["paused"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_control_msgsControlStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["paused"]=decoded!==0;
}
return value;}
function Ros1Clearpath_control_msgsControlStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enabled"];
out["enabled"]=field;
}
{const field=value["paused"];
out["paused"]=field;
}
return out;}
/** Complete ROS1 clearpath_control_msgs/ControlSelectionState codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsControlSelectionStateCodec=Object.freeze({codecIdentity:"UWVMROS1_1dff74daa9b8550504ebab354d18f2a40a143fcb3cfd721e3d04aab79546f994",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"autonomy":Ros1Clearpath_control_msgsControlStateCodec.defaultValue(),"mode":Ros1Clearpath_control_msgsControlModeCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_control_msgsControlSelectionStateSize(Ros1Clearpath_control_msgsControlSelectionStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsControlSelectionStateCapture(value),n=Ros1Clearpath_control_msgsControlSelectionStateSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsControlSelectionStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsControlSelectionStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsControlSelectionStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_control_msgsControlStateSize(value["autonomy"]));
n=addSize(n,Ros1Clearpath_control_msgsControlModeSize(value["mode"]));
if(n>3)bad(90);return n;}
function Ros1Clearpath_control_msgsControlSelectionStateWrite(value,w){
Ros1Clearpath_control_msgsControlStateWrite(value["autonomy"],w);
Ros1Clearpath_control_msgsControlModeWrite(value["mode"],w);
}
function Ros1Clearpath_control_msgsControlSelectionStateRead(r){const value={};
{
const decoded=Ros1Clearpath_control_msgsControlStateRead(r);
value["autonomy"]=decoded;
}
{
const decoded=Ros1Clearpath_control_msgsControlModeRead(r);
value["mode"]=decoded;
}
return value;}
function Ros1Clearpath_control_msgsControlSelectionStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["autonomy"];
out["autonomy"]=Ros1Clearpath_control_msgsControlStateCapture(field);
}
{const field=value["mode"];
out["mode"]=Ros1Clearpath_control_msgsControlModeCapture(field);
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/DockInfo codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsDockInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_d4c6b31ecbec7e01340e6693718db5de0429a897dda5caea9ee49b58a77649c7",maxWireBytes:1204,minWireBytes:52,cartesianMaxWireBytes:1204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","uuid":"","frame":"","target_template":"","last_location_set_time":"","max_predock_distance":0,"predock_distance":0,"latitude":0,"longitude":0,"heading":0};},
validateValue(value){Ros1Clearpath_dock_msgsDockInfoSize(Ros1Clearpath_dock_msgsDockInfoCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsDockInfoCapture(value),n=Ros1Clearpath_dock_msgsDockInfoSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsDockInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1204,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsDockInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsDockInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["frame"],256).length);
n=addSize(n,4+textBytes(value["target_template"],256).length);
n=addSize(n,4+textBytes(value["last_location_set_time"],256).length);
if(typeof value["max_predock_distance"]!=="number"||!Number.isFinite(value["max_predock_distance"])||!Number.isFinite(Math.fround(value["max_predock_distance"])))bad();n=addSize(n,4);
if(typeof value["predock_distance"]!=="number"||!Number.isFinite(value["predock_distance"])||!Number.isFinite(Math.fround(value["predock_distance"])))bad();n=addSize(n,4);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(n>1204)bad(90);return n;}
function Ros1Clearpath_dock_msgsDockInfoWrite(value,w){
w.text(value["name"],128);
w.text(value["uuid"],256);
w.text(value["frame"],256);
w.text(value["target_template"],256);
w.text(value["last_location_set_time"],256);
w.v.setFloat32(w.at,value["max_predock_distance"],true);w.at+=4;
w.v.setFloat32(w.at,value["predock_distance"],true);w.at+=4;
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
}
function Ros1Clearpath_dock_msgsDockInfoRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["frame"]=decoded;
}
{
const decoded=r.text(256);
value["target_template"]=decoded;
}
{
const decoded=r.text(256);
value["last_location_set_time"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["max_predock_distance"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["predock_distance"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsDockInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["frame"];
out["frame"]=field;
}
{const field=value["target_template"];
out["target_template"]=field;
}
{const field=value["last_location_set_time"];
out["last_location_set_time"]=field;
}
{const field=value["max_predock_distance"];
out["max_predock_distance"]=field;
}
{const field=value["predock_distance"];
out["predock_distance"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/DockProperties codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsDockPropertiesCodec=Object.freeze({codecIdentity:"UWVMROS1_842af6b3b69fb6e2dfe1fdaabed4e45d6a39410456960dd7a4422eb491d19a76",maxWireBytes:2412,minWireBytes:4,cartesianMaxWireBytes:2412n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"docks":[]};},
validateValue(value){Ros1Clearpath_dock_msgsDockPropertiesSize(Ros1Clearpath_dock_msgsDockPropertiesCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsDockPropertiesCapture(value),n=Ros1Clearpath_dock_msgsDockPropertiesSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsDockPropertiesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2412,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsDockPropertiesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsDockPropertiesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["docks"])||value["docks"].length>2)bad();
n=addSize(n,4);
for(const element of value["docks"]){
n=addSize(n,Ros1Clearpath_dock_msgsDockInfoSize(element));
}
if(n>2412)bad(90);return n;}
function Ros1Clearpath_dock_msgsDockPropertiesWrite(value,w){
w.v.setUint32(w.at,value["docks"].length,true);w.at+=4;
for(const element of value["docks"]){
Ros1Clearpath_dock_msgsDockInfoWrite(element,w);
}
}
function Ros1Clearpath_dock_msgsDockPropertiesRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["docks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_dock_msgsDockInfoRead(r);
value["docks"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_dock_msgsDockPropertiesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["docks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_dock_msgsDockInfoCapture(element);
}out["docks"]=values;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/DockState codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsDockStateCodec=Object.freeze({codecIdentity:"UWVMROS1_18f6c0addce5321932de99e4b80127dc1ac69f545778938e39b293be4c5e7a53",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"UNDOCKED":0,"DOCKED":1,"IN_PROGRESS":2,"FAILED":3,"CANCELLED":4}),
defaultValue(){return {"state":0};},
validateValue(value){Ros1Clearpath_dock_msgsDockStateSize(Ros1Clearpath_dock_msgsDockStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsDockStateCapture(value),n=Ros1Clearpath_dock_msgsDockStateSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsDockStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsDockStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsDockStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["state"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_dock_msgsDockStateWrite(value,w){
w.v.setUint8(w.at,value["state"],true);w.at+=1;
}
function Ros1Clearpath_dock_msgsDockStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsDockStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
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
/** Complete ROS1 geometry_msgs/PoseWithCovarianceStamped codec; maxima are not actual lengths. */
export const Ros1Geometry_msgsPoseWithCovarianceStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_6708292fe381a7f14aaf84f47a969f55af7446087bfcb309d3b86ab95f34a2d9",maxWireBytes:488,minWireBytes:360,cartesianMaxWireBytes:488n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"pose":Ros1Geometry_msgsPoseWithCovarianceCodec.defaultValue()};},
validateValue(value){Ros1Geometry_msgsPoseWithCovarianceStampedSize(Ros1Geometry_msgsPoseWithCovarianceStampedCapture(value));return true;},
encode(value){const owned=Ros1Geometry_msgsPoseWithCovarianceStampedCapture(value),n=Ros1Geometry_msgsPoseWithCovarianceStampedSize(owned),w=new Writer(n);Ros1Geometry_msgsPoseWithCovarianceStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,488,90);if(owned.length<360)bad();const r=new Reader(owned),value=Ros1Geometry_msgsPoseWithCovarianceStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Geometry_msgsPoseWithCovarianceStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Geometry_msgsPoseWithCovarianceSize(value["pose"]));
if(n>488)bad(90);return n;}
function Ros1Geometry_msgsPoseWithCovarianceStampedWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Geometry_msgsPoseWithCovarianceWrite(value["pose"],w);
}
function Ros1Geometry_msgsPoseWithCovarianceStampedRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseWithCovarianceRead(r);
value["pose"]=decoded;
}
return value;}
function Ros1Geometry_msgsPoseWithCovarianceStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseWithCovarianceCapture(field);
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/TargetTrackerState codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsTargetTrackerStateCodec=Object.freeze({codecIdentity:"UWVMROS1_22cddfc9827c9b576e8baf843379e8856efd0b22c309ae700c619945c45274a8",maxWireBytes:502,minWireBytes:374,cartesianMaxWireBytes:502n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"active":false,"target_locked":false,"target_pose":Ros1Geometry_msgsPoseWithCovarianceStampedCodec.defaultValue(),"score":0,"distance_to_target":0};},
validateValue(value){Ros1Clearpath_dock_msgsTargetTrackerStateSize(Ros1Clearpath_dock_msgsTargetTrackerStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsTargetTrackerStateCapture(value),n=Ros1Clearpath_dock_msgsTargetTrackerStateSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsTargetTrackerStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,502,90);if(owned.length<374)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsTargetTrackerStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsTargetTrackerStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["active"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["target_locked"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Geometry_msgsPoseWithCovarianceStampedSize(value["target_pose"]));
if(typeof value["score"]!=="number"||!Number.isFinite(value["score"])||!Number.isFinite(Math.fround(value["score"])))bad();n=addSize(n,4);
if(typeof value["distance_to_target"]!=="number"||!Number.isFinite(value["distance_to_target"]))bad();n=addSize(n,8);
if(n>502)bad(90);return n;}
function Ros1Clearpath_dock_msgsTargetTrackerStateWrite(value,w){
w.v.setUint8(w.at,value["active"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["target_locked"]?1:0,true);w.at+=1;
Ros1Geometry_msgsPoseWithCovarianceStampedWrite(value["target_pose"],w);
w.v.setFloat32(w.at,value["score"],true);w.at+=4;
w.v.setFloat64(w.at,value["distance_to_target"],true);w.at+=8;
}
function Ros1Clearpath_dock_msgsTargetTrackerStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["active"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["target_locked"]=decoded!==0;
}
{
const decoded=Ros1Geometry_msgsPoseWithCovarianceStampedRead(r);
value["target_pose"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["score"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["distance_to_target"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsTargetTrackerStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["active"];
out["active"]=field;
}
{const field=value["target_locked"];
out["target_locked"]=field;
}
{const field=value["target_pose"];
out["target_pose"]=Ros1Geometry_msgsPoseWithCovarianceStampedCapture(field);
}
{const field=value["score"];
out["score"]=field;
}
{const field=value["distance_to_target"];
out["distance_to_target"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/GNSSStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsGNSSStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_4bda462d76cea809faf3fad93446de6db661c1379d743f829f7bcf64ef7959f3",maxWireBytes:273,minWireBytes:17,cartesianMaxWireBytes:273n,aggregateWireBytes:0,constants:Object.freeze({"POSITION_RECEIVER":0,"HEADING_RECEIVER":1,"BASE_STATION_RECEIVER":2,"POSITION_NO_FIX":0,"POSITION_SPP":1,"POSITION_SBAS":2,"POSITION_RTK_FLOAT":3,"POSITION_RTK_FIXED":4,"HEADING_NOT_APPLICABLE":0,"HEADING_NO_SOLUTION":1,"HEADING_RTK_FLOAT":2,"HEADING_RTK_FIXED":3}),
defaultValue(){return {"receiver_type":0,"position_fix_type":0,"heading_fix_type":0,"num_connected_sats":0,"num_sats_solution":0,"cn0":[],"correction_age":0};},
validateValue(value){Ros1Clearpath_localization_msgsGNSSStatusSize(Ros1Clearpath_localization_msgsGNSSStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsGNSSStatusCapture(value),n=Ros1Clearpath_localization_msgsGNSSStatusSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsGNSSStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,273,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsGNSSStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsGNSSStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["receiver_type"],1,false);n=addSize(n,1);
integer(value["position_fix_type"],1,false);n=addSize(n,1);
integer(value["heading_fix_type"],1,false);n=addSize(n,1);
integer(value["num_connected_sats"],1,false);n=addSize(n,1);
integer(value["num_sats_solution"],1,false);n=addSize(n,1);
if(!Array.isArray(value["cn0"])||value["cn0"].length>64)bad();
n=addSize(n,4);
for(const element of value["cn0"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["correction_age"]!=="number"||!Number.isFinite(value["correction_age"]))bad();n=addSize(n,8);
if(n>273)bad(90);return n;}
function Ros1Clearpath_localization_msgsGNSSStatusWrite(value,w){
w.v.setUint8(w.at,value["receiver_type"],true);w.at+=1;
w.v.setUint8(w.at,value["position_fix_type"],true);w.at+=1;
w.v.setUint8(w.at,value["heading_fix_type"],true);w.at+=1;
w.v.setUint8(w.at,value["num_connected_sats"],true);w.at+=1;
w.v.setUint8(w.at,value["num_sats_solution"],true);w.at+=1;
w.v.setUint32(w.at,value["cn0"].length,true);w.at+=4;
for(const element of value["cn0"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat64(w.at,value["correction_age"],true);w.at+=8;
}
function Ros1Clearpath_localization_msgsGNSSStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["receiver_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["position_fix_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["heading_fix_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["num_connected_sats"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["num_sats_solution"]=decoded;
}
{
{const count=r.u32();if(count>64)bad();value["cn0"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cn0"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["correction_age"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsGNSSStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["receiver_type"];
out["receiver_type"]=field;
}
{const field=value["position_fix_type"];
out["position_fix_type"]=field;
}
{const field=value["heading_fix_type"];
out["heading_fix_type"]=field;
}
{const field=value["num_connected_sats"];
out["num_connected_sats"]=field;
}
{const field=value["num_sats_solution"];
out["num_sats_solution"]=field;
}
{const field=value["cn0"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>64)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cn0"]=values;
}
{const field=value["correction_age"];
out["correction_age"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/LocalizationStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsLocalizationStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_2dcb14d889fb5ccc69ad66860b3495b7c3eadd94fb9d814b9f475ff0f6dffc74",maxWireBytes:968,minWireBytes:72,cartesianMaxWireBytes:968n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"accuracy":0,"position_gnss":Ros1Clearpath_localization_msgsGNSSStatusCodec.defaultValue(),"heading_gnss":Ros1Clearpath_localization_msgsGNSSStatusCodec.defaultValue(),"base_station_gnss":Ros1Clearpath_localization_msgsGNSSStatusCodec.defaultValue(),"dead_reckoning":false};},
validateValue(value){Ros1Clearpath_localization_msgsLocalizationStatusSize(Ros1Clearpath_localization_msgsLocalizationStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsLocalizationStatusCapture(value),n=Ros1Clearpath_localization_msgsLocalizationStatusSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsLocalizationStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,968,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsLocalizationStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsLocalizationStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["accuracy"]!=="number"||!Number.isFinite(value["accuracy"])||!Number.isFinite(Math.fround(value["accuracy"])))bad();n=addSize(n,4);
n=addSize(n,Ros1Clearpath_localization_msgsGNSSStatusSize(value["position_gnss"]));
n=addSize(n,Ros1Clearpath_localization_msgsGNSSStatusSize(value["heading_gnss"]));
n=addSize(n,Ros1Clearpath_localization_msgsGNSSStatusSize(value["base_station_gnss"]));
if(typeof value["dead_reckoning"]!=="boolean")bad();n=addSize(n,1);
if(n>968)bad(90);return n;}
function Ros1Clearpath_localization_msgsLocalizationStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["accuracy"],true);w.at+=4;
Ros1Clearpath_localization_msgsGNSSStatusWrite(value["position_gnss"],w);
Ros1Clearpath_localization_msgsGNSSStatusWrite(value["heading_gnss"],w);
Ros1Clearpath_localization_msgsGNSSStatusWrite(value["base_station_gnss"],w);
w.v.setUint8(w.at,value["dead_reckoning"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_localization_msgsLocalizationStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["accuracy"]=decoded;
}
{
const decoded=Ros1Clearpath_localization_msgsGNSSStatusRead(r);
value["position_gnss"]=decoded;
}
{
const decoded=Ros1Clearpath_localization_msgsGNSSStatusRead(r);
value["heading_gnss"]=decoded;
}
{
const decoded=Ros1Clearpath_localization_msgsGNSSStatusRead(r);
value["base_station_gnss"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["dead_reckoning"]=decoded!==0;
}
return value;}
function Ros1Clearpath_localization_msgsLocalizationStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["accuracy"];
out["accuracy"]=field;
}
{const field=value["position_gnss"];
out["position_gnss"]=Ros1Clearpath_localization_msgsGNSSStatusCapture(field);
}
{const field=value["heading_gnss"];
out["heading_gnss"]=Ros1Clearpath_localization_msgsGNSSStatusCapture(field);
}
{const field=value["base_station_gnss"];
out["base_station_gnss"]=Ros1Clearpath_localization_msgsGNSSStatusCapture(field);
}
{const field=value["dead_reckoning"];
out["dead_reckoning"]=field;
}
return out;}
/** Complete ROS1 sensor_msgs/NavSatStatus codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsNavSatStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_a44d99b9b113b546b2f45c991925ea62a8e28ffceed7de4da2dbc81936fd2453",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({"STATUS_NO_FIX":-1,"STATUS_FIX":0,"STATUS_SBAS_FIX":1,"STATUS_GBAS_FIX":2,"SERVICE_GPS":1,"SERVICE_GLONASS":2,"SERVICE_COMPASS":4,"SERVICE_GALILEO":8}),
defaultValue(){return {"status":0,"service":0};},
validateValue(value){Ros1Sensor_msgsNavSatStatusSize(Ros1Sensor_msgsNavSatStatusCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsNavSatStatusCapture(value),n=Ros1Sensor_msgsNavSatStatusSize(owned),w=new Writer(n);Ros1Sensor_msgsNavSatStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Sensor_msgsNavSatStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsNavSatStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],1,true);n=addSize(n,1);
integer(value["service"],2,false);n=addSize(n,2);
if(n>3)bad(90);return n;}
function Ros1Sensor_msgsNavSatStatusWrite(value,w){
w.v.setInt8(w.at,value["status"],true);w.at+=1;
w.v.setUint16(w.at,value["service"],true);w.at+=2;
}
function Ros1Sensor_msgsNavSatStatusRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["status"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["service"]=decoded;
}
return value;}
function Ros1Sensor_msgsNavSatStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["service"];
out["service"]=field;
}
return out;}
/** Complete ROS1 sensor_msgs/NavSatFix codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsNavSatFixCodec=Object.freeze({codecIdentity:"UWVMROS1_40bcafbbd6e6fed3086df74b5bd06aea5c2b6bf3244fcee92ab8cfab9c3c4d4d",maxWireBytes:244,minWireBytes:116,cartesianMaxWireBytes:244n,aggregateWireBytes:0,constants:Object.freeze({"COVARIANCE_TYPE_UNKNOWN":0,"COVARIANCE_TYPE_APPROXIMATED":1,"COVARIANCE_TYPE_DIAGONAL_KNOWN":2,"COVARIANCE_TYPE_KNOWN":3}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"status":Ros1Sensor_msgsNavSatStatusCodec.defaultValue(),"latitude":0,"longitude":0,"altitude":0,"position_covariance":Array.from({length:9},()=>(0)),"position_covariance_type":0};},
validateValue(value){Ros1Sensor_msgsNavSatFixSize(Ros1Sensor_msgsNavSatFixCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsNavSatFixCapture(value),n=Ros1Sensor_msgsNavSatFixSize(owned),w=new Writer(n);Ros1Sensor_msgsNavSatFixWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,244,90);if(owned.length<116)bad();const r=new Reader(owned),value=Ros1Sensor_msgsNavSatFixRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsNavSatFixSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Sensor_msgsNavSatStatusSize(value["status"]));
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["altitude"]!=="number"||!Number.isFinite(value["altitude"]))bad();n=addSize(n,8);
if(!Array.isArray(value["position_covariance"])||value["position_covariance"].length!==9)bad();
for(const element of value["position_covariance"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
integer(value["position_covariance_type"],1,false);n=addSize(n,1);
if(n>244)bad(90);return n;}
function Ros1Sensor_msgsNavSatFixWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Sensor_msgsNavSatStatusWrite(value["status"],w);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["altitude"],true);w.at+=8;
for(const element of value["position_covariance"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint8(w.at,value["position_covariance_type"],true);w.at+=1;
}
function Ros1Sensor_msgsNavSatFixRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Sensor_msgsNavSatStatusRead(r);
value["status"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["altitude"]=decoded;
}
{
{const count=9;if(count>9)bad();value["position_covariance"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position_covariance"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["position_covariance_type"]=decoded;
}
return value;}
function Ros1Sensor_msgsNavSatFixCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["status"];
out["status"]=Ros1Sensor_msgsNavSatStatusCapture(field);
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["altitude"];
out["altitude"]=field;
}
{const field=value["position_covariance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position_covariance"]=values;
}
{const field=value["position_covariance_type"];
out["position_covariance_type"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SurveyProperties codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSurveyPropertiesCodec=Object.freeze({codecIdentity:"UWVMROS1_91f7b0f79ca81b76885c2a05a5de907620de7b5420f71e1ca08166bdb320c393",maxWireBytes:509,minWireBytes:125,cartesianMaxWireBytes:509n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"last_survey_time":"","num_samples":0,"base_location":Ros1Sensor_msgsNavSatFixCodec.defaultValue(),"success":false};},
validateValue(value){Ros1Clearpath_localization_msgsSurveyPropertiesSize(Ros1Clearpath_localization_msgsSurveyPropertiesCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSurveyPropertiesCapture(value),n=Ros1Clearpath_localization_msgsSurveyPropertiesSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSurveyPropertiesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,509,90);if(owned.length<125)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSurveyPropertiesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSurveyPropertiesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["last_survey_time"],256).length);
integer(value["num_samples"],4,false);n=addSize(n,4);
n=addSize(n,Ros1Sensor_msgsNavSatFixSize(value["base_location"]));
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>509)bad(90);return n;}
function Ros1Clearpath_localization_msgsSurveyPropertiesWrite(value,w){
w.text(value["last_survey_time"],256);
w.v.setUint32(w.at,value["num_samples"],true);w.at+=4;
Ros1Sensor_msgsNavSatFixWrite(value["base_location"],w);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_localization_msgsSurveyPropertiesRead(r){const value={};
{
const decoded=r.text(256);
value["last_survey_time"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["num_samples"]=decoded;
}
{
const decoded=Ros1Sensor_msgsNavSatFixRead(r);
value["base_location"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_localization_msgsSurveyPropertiesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["last_survey_time"];
out["last_survey_time"]=field;
}
{const field=value["num_samples"];
out["num_samples"]=field;
}
{const field=value["base_location"];
out["base_location"]=Ros1Sensor_msgsNavSatFixCapture(field);
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/XvnStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsXvnStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_16bffc785ac0a8da2af278aefa77478e1ecc986d6a505b306e6df5f9db6e7def",maxWireBytes:182,minWireBytes:54,cartesianMaxWireBytes:182n,aggregateWireBytes:0,constants:Object.freeze({"UNKNOWN":0,"VISION":1,"VISUAL_INERTIAL":2,"INERTIAL_GNSS":3,"VI_GNSS":4,"IMU_NOT_CONVERGED":0,"IMU_CONVERGED":1,"WS_NOT_USED":-1,"WS_NOT_CONVERGED":0,"WS_CONVERGED":1,"GNSS_UNKNOWN":0,"GNSS_NO_FIX":1,"GNSS_DEAD_RECKONING":2,"GNSS_TIME_FIX":3,"GNSS_2D_FIX":4,"GNSS_3D_FIX":5,"GNSS_3D_WITH_DR":6,"GNSS_RTK_FLOAT":7,"GNSS_RTK_FIX":8,"RTK_ERROR":0,"RTK_WARNING":1,"RTK_CONNECTING":2,"RTK_CONNECTED":3}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"latitude":0,"longitude":0,"yaw":0,"stddev_x":0,"stddev_y":0,"stddev_yaw":0,"fusion_status":0,"imu_status":0,"wheelspeed_status":0,"gnss1_status":0,"gnss2_status":0,"rtk_status":0};},
validateValue(value){Ros1Clearpath_localization_msgsXvnStatusSize(Ros1Clearpath_localization_msgsXvnStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsXvnStatusCapture(value),n=Ros1Clearpath_localization_msgsXvnStatusSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsXvnStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,182,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsXvnStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsXvnStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["yaw"]!=="number"||!Number.isFinite(value["yaw"])||!Number.isFinite(Math.fround(value["yaw"])))bad();n=addSize(n,4);
if(typeof value["stddev_x"]!=="number"||!Number.isFinite(value["stddev_x"])||!Number.isFinite(Math.fround(value["stddev_x"])))bad();n=addSize(n,4);
if(typeof value["stddev_y"]!=="number"||!Number.isFinite(value["stddev_y"])||!Number.isFinite(Math.fround(value["stddev_y"])))bad();n=addSize(n,4);
if(typeof value["stddev_yaw"]!=="number"||!Number.isFinite(value["stddev_yaw"])||!Number.isFinite(Math.fround(value["stddev_yaw"])))bad();n=addSize(n,4);
integer(value["fusion_status"],1,false);n=addSize(n,1);
integer(value["imu_status"],1,false);n=addSize(n,1);
integer(value["wheelspeed_status"],1,true);n=addSize(n,1);
integer(value["gnss1_status"],1,false);n=addSize(n,1);
integer(value["gnss2_status"],1,false);n=addSize(n,1);
integer(value["rtk_status"],1,false);n=addSize(n,1);
if(n>182)bad(90);return n;}
function Ros1Clearpath_localization_msgsXvnStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat32(w.at,value["yaw"],true);w.at+=4;
w.v.setFloat32(w.at,value["stddev_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["stddev_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["stddev_yaw"],true);w.at+=4;
w.v.setUint8(w.at,value["fusion_status"],true);w.at+=1;
w.v.setUint8(w.at,value["imu_status"],true);w.at+=1;
w.v.setInt8(w.at,value["wheelspeed_status"],true);w.at+=1;
w.v.setUint8(w.at,value["gnss1_status"],true);w.at+=1;
w.v.setUint8(w.at,value["gnss2_status"],true);w.at+=1;
w.v.setUint8(w.at,value["rtk_status"],true);w.at+=1;
}
function Ros1Clearpath_localization_msgsXvnStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["yaw"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["stddev_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["stddev_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["stddev_yaw"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["fusion_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["imu_status"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["wheelspeed_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gnss1_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gnss2_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["rtk_status"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsXvnStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["yaw"];
out["yaw"]=field;
}
{const field=value["stddev_x"];
out["stddev_x"]=field;
}
{const field=value["stddev_y"];
out["stddev_y"]=field;
}
{const field=value["stddev_yaw"];
out["stddev_yaw"]=field;
}
{const field=value["fusion_status"];
out["fusion_status"]=field;
}
{const field=value["imu_status"];
out["imu_status"]=field;
}
{const field=value["wheelspeed_status"];
out["wheelspeed_status"]=field;
}
{const field=value["gnss1_status"];
out["gnss1_status"]=field;
}
{const field=value["gnss2_status"];
out["gnss2_status"]=field;
}
{const field=value["rtk_status"];
out["rtk_status"]=field;
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
/** Complete ROS1 clearpath_logger_msgs/Event codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsEventCodec=Object.freeze({codecIdentity:"UWVMROS1_3ab8dbb5d488cb1c0789bbb96076f531b245703045330fb6f9b01079d78177ca",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({"TAG_MEDIA":"media","TAG_POWER":"power","TAG_LOCATION":"location","TAG_ODOMETRY":"odom","TAG_VELOCITY":"vel","TAG_ERROR":"error","TAG_AUTO_TELEMETRY":"auto_telemetry"}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"media_path":"","media_mime_type":"","frame_pose":Ros1Geometry_msgsPoseCodec.defaultValue(),"lat_lon_heading":Ros1Geometry_msgsPointCodec.defaultValue(),"odometry":Ros1Nav_msgsOdometryCodec.defaultValue(),"velocity":Ros1Geometry_msgsTwistCodec.defaultValue(),"battery_percent":0,"charging":false,"power_saver":false,"error_code":0,"error_msg":"","error_component":"","tags":[],"custom_fields_json":""};},
validateValue(value){Ros1Clearpath_logger_msgsEventSize(Ros1Clearpath_logger_msgsEventCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsEventCapture(value),n=Ros1Clearpath_logger_msgsEventSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsEventWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsEventRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsEventSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["media_path"],256).length);
n=addSize(n,4+textBytes(value["media_mime_type"],256).length);
n=addSize(n,Ros1Geometry_msgsPoseSize(value["frame_pose"]));
n=addSize(n,Ros1Geometry_msgsPointSize(value["lat_lon_heading"]));
n=addSize(n,Ros1Nav_msgsOdometrySize(value["odometry"]));
n=addSize(n,Ros1Geometry_msgsTwistSize(value["velocity"]));
if(typeof value["battery_percent"]!=="number"||!Number.isFinite(value["battery_percent"])||!Number.isFinite(Math.fround(value["battery_percent"])))bad();n=addSize(n,4);
if(typeof value["charging"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["power_saver"]!=="boolean")bad();n=addSize(n,1);
integer(value["error_code"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["error_msg"],256).length);
n=addSize(n,4+textBytes(value["error_component"],256).length);
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsEventWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["media_path"],256);
w.text(value["media_mime_type"],256);
Ros1Geometry_msgsPoseWrite(value["frame_pose"],w);
Ros1Geometry_msgsPointWrite(value["lat_lon_heading"],w);
Ros1Nav_msgsOdometryWrite(value["odometry"],w);
Ros1Geometry_msgsTwistWrite(value["velocity"],w);
w.v.setFloat32(w.at,value["battery_percent"],true);w.at+=4;
w.v.setUint8(w.at,value["charging"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["power_saver"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["error_code"],true);w.at+=4;
w.text(value["error_msg"],256);
w.text(value["error_component"],256);
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
w.text(value["custom_fields_json"],256);
}
function Ros1Clearpath_logger_msgsEventRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["media_path"]=decoded;
}
{
const decoded=r.text(256);
value["media_mime_type"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseRead(r);
value["frame_pose"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPointRead(r);
value["lat_lon_heading"]=decoded;
}
{
const decoded=Ros1Nav_msgsOdometryRead(r);
value["odometry"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistRead(r);
value["velocity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["battery_percent"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["charging"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["power_saver"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=r.text(256);
value["error_msg"]=decoded;
}
{
const decoded=r.text(256);
value["error_component"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsEventCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["media_path"];
out["media_path"]=field;
}
{const field=value["media_mime_type"];
out["media_mime_type"]=field;
}
{const field=value["frame_pose"];
out["frame_pose"]=Ros1Geometry_msgsPoseCapture(field);
}
{const field=value["lat_lon_heading"];
out["lat_lon_heading"]=Ros1Geometry_msgsPointCapture(field);
}
{const field=value["odometry"];
out["odometry"]=Ros1Nav_msgsOdometryCapture(field);
}
{const field=value["velocity"];
out["velocity"]=Ros1Geometry_msgsTwistCapture(field);
}
{const field=value["battery_percent"];
out["battery_percent"]=field;
}
{const field=value["charging"];
out["charging"]=field;
}
{const field=value["power_saver"];
out["power_saver"]=field;
}
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_msg"];
out["error_msg"]=field;
}
{const field=value["error_component"];
out["error_component"]=field;
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/EventLog codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsEventLogCodec=Object.freeze({codecIdentity:"UWVMROS1_ff2b4c89394c5f796204cf33e19c125c1affe603a96e22fd18268d75becd9c98",maxWireBytes:7064,minWireBytes:44,cartesianMaxWireBytes:7064n,aggregateWireBytes:0,constants:Object.freeze({"STATE_ACTIVE":0,"STATE_CLOSED":1,"STATE_DELETED":2}),
defaultValue(){return {"state":0,"start_time":Ros1TimeCodec.defaultValue(),"end_time":Ros1TimeCodec.defaultValue(),"uuid":"","name":"","battery_percent_used":0,"distance_driven":0,"custom_fields_json":"","events":[]};},
validateValue(value){Ros1Clearpath_logger_msgsEventLogSize(Ros1Clearpath_logger_msgsEventLogCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsEventLogCapture(value),n=Ros1Clearpath_logger_msgsEventLogSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsEventLogWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7064,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsEventLogRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsEventLogSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["state"],4,true);n=addSize(n,4);
n=addSize(n,Ros1TimeSize(value["start_time"]));
n=addSize(n,Ros1TimeSize(value["end_time"]));
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["battery_percent_used"]!=="number"||!Number.isFinite(value["battery_percent_used"])||!Number.isFinite(Math.fround(value["battery_percent_used"])))bad();n=addSize(n,4);
if(typeof value["distance_driven"]!=="number"||!Number.isFinite(value["distance_driven"])||!Number.isFinite(Math.fround(value["distance_driven"])))bad();n=addSize(n,4);
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(!Array.isArray(value["events"])||value["events"].length>2)bad();
n=addSize(n,4);
for(const element of value["events"]){
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(element));
}
if(n>7064)bad(90);return n;}
function Ros1Clearpath_logger_msgsEventLogWrite(value,w){
w.v.setInt32(w.at,value["state"],true);w.at+=4;
Ros1TimeWrite(value["start_time"],w);
Ros1TimeWrite(value["end_time"],w);
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat32(w.at,value["battery_percent_used"],true);w.at+=4;
w.v.setFloat32(w.at,value["distance_driven"],true);w.at+=4;
w.text(value["custom_fields_json"],256);
w.v.setUint32(w.at,value["events"].length,true);w.at+=4;
for(const element of value["events"]){
Ros1Clearpath_logger_msgsEventWrite(element,w);
}
}
function Ros1Clearpath_logger_msgsEventLogRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["state"]=decoded;
}
{
const decoded=Ros1TimeRead(r);
value["start_time"]=decoded;
}
{
const decoded=Ros1TimeRead(r);
value["end_time"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["battery_percent_used"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["distance_driven"]=decoded;
}
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["events"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["events"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_logger_msgsEventLogCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["start_time"];
out["start_time"]=Ros1TimeCapture(field);
}
{const field=value["end_time"];
out["end_time"]=Ros1TimeCapture(field);
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["battery_percent_used"];
out["battery_percent_used"]=field;
}
{const field=value["distance_driven"];
out["distance_driven"]=field;
}
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
{const field=value["events"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_logger_msgsEventCapture(element);
}out["events"]=values;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/EventLogs codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsEventLogsCodec=Object.freeze({codecIdentity:"UWVMROS1_0bd7c8adb2807c9a4273f01efb36309685301550a6e201f76865c6b5becabd61",maxWireBytes:14132,minWireBytes:4,cartesianMaxWireBytes:14132n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"logs":[]};},
validateValue(value){Ros1Clearpath_logger_msgsEventLogsSize(Ros1Clearpath_logger_msgsEventLogsCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsEventLogsCapture(value),n=Ros1Clearpath_logger_msgsEventLogsSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsEventLogsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,14132,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsEventLogsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsEventLogsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["logs"])||value["logs"].length>2)bad();
n=addSize(n,4);
for(const element of value["logs"]){
n=addSize(n,Ros1Clearpath_logger_msgsEventLogSize(element));
}
if(n>14132)bad(90);return n;}
function Ros1Clearpath_logger_msgsEventLogsWrite(value,w){
w.v.setUint32(w.at,value["logs"].length,true);w.at+=4;
for(const element of value["logs"]){
Ros1Clearpath_logger_msgsEventLogWrite(element,w);
}
}
function Ros1Clearpath_logger_msgsEventLogsRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["logs"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_logger_msgsEventLogRead(r);
value["logs"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_logger_msgsEventLogsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["logs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_logger_msgsEventLogCapture(element);
}out["logs"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/NetworkEdgeReq codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsNetworkEdgeReqCodec=Object.freeze({codecIdentity:"UWVMROS1_134bc2836c26c258057ae81465e06ba1f52307895c8016da1bab139a8efb9d62",maxWireBytes:528,minWireBytes:16,cartesianMaxWireBytes:528n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"start_point_id":"","end_point_id":"","speed_limit":0,"radius":0};},
validateValue(value){Ros1Clearpath_mission_manager_msgsNetworkEdgeReqSize(Ros1Clearpath_mission_manager_msgsNetworkEdgeReqCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsNetworkEdgeReqCapture(value),n=Ros1Clearpath_mission_manager_msgsNetworkEdgeReqSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsNetworkEdgeReqWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,528,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsNetworkEdgeReqRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsNetworkEdgeReqSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["start_point_id"],256).length);
n=addSize(n,4+textBytes(value["end_point_id"],256).length);
if(typeof value["speed_limit"]!=="number"||!Number.isFinite(value["speed_limit"])||!Number.isFinite(Math.fround(value["speed_limit"])))bad();n=addSize(n,4);
if(typeof value["radius"]!=="number"||!Number.isFinite(value["radius"])||!Number.isFinite(Math.fround(value["radius"])))bad();n=addSize(n,4);
if(n>528)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsNetworkEdgeReqWrite(value,w){
w.text(value["start_point_id"],256);
w.text(value["end_point_id"],256);
w.v.setFloat32(w.at,value["speed_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["radius"],true);w.at+=4;
}
function Ros1Clearpath_mission_manager_msgsNetworkEdgeReqRead(r){const value={};
{
const decoded=r.text(256);
value["start_point_id"]=decoded;
}
{
const decoded=r.text(256);
value["end_point_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["radius"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsNetworkEdgeReqCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["start_point_id"];
out["start_point_id"]=field;
}
{const field=value["end_point_id"];
out["end_point_id"]=field;
}
{const field=value["speed_limit"];
out["speed_limit"]=field;
}
{const field=value["radius"];
out["radius"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/NetworkMapState codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsNetworkMapStateCodec=Object.freeze({codecIdentity:"UWVMROS1_c87392fb395b63731581dec3a3b199c830a7ed5001c53949c3002f9062d4958c",maxWireBytes:790,minWireBytes:14,cartesianMaxWireBytes:790n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","exists":false,"num_subgraphs":0,"disconnected_point_ids":[],"size_exceeded":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsNetworkMapStateSize(Ros1Clearpath_mission_manager_msgsNetworkMapStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsNetworkMapStateCapture(value),n=Ros1Clearpath_mission_manager_msgsNetworkMapStateSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsNetworkMapStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,790,90);if(owned.length<14)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsNetworkMapStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsNetworkMapStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["exists"]!=="boolean")bad();n=addSize(n,1);
integer(value["num_subgraphs"],4,true);n=addSize(n,4);
if(!Array.isArray(value["disconnected_point_ids"])||value["disconnected_point_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["disconnected_point_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["size_exceeded"]!=="boolean")bad();n=addSize(n,1);
if(n>790)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsNetworkMapStateWrite(value,w){
w.text(value["uuid"],256);
w.v.setUint8(w.at,value["exists"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["num_subgraphs"],true);w.at+=4;
w.v.setUint32(w.at,value["disconnected_point_ids"].length,true);w.at+=4;
for(const element of value["disconnected_point_ids"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["size_exceeded"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsNetworkMapStateRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["exists"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["num_subgraphs"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["disconnected_point_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["disconnected_point_ids"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["size_exceeded"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsNetworkMapStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["exists"];
out["exists"]=field;
}
{const field=value["num_subgraphs"];
out["num_subgraphs"]=field;
}
{const field=value["disconnected_point_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["disconnected_point_ids"]=values;
}
{const field=value["size_exceeded"];
out["size_exceeded"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/NetworkMapValidityState codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateCodec=Object.freeze({codecIdentity:"UWVMROS1_12fe0cef6a55971defedc37a35a87257b71c7e3bb103b91cbbeb6bfbd30e850a",maxWireBytes:1584,minWireBytes:4,cartesianMaxWireBytes:1584n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_states":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateSize(Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateCapture(value),n=Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1584,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["map_states"])||value["map_states"].length>2)bad();
n=addSize(n,4);
for(const element of value["map_states"]){
n=addSize(n,Ros1Clearpath_mission_manager_msgsNetworkMapStateSize(element));
}
if(n>1584)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateWrite(value,w){
w.v.setUint32(w.at,value["map_states"].length,true);w.at+=4;
for(const element of value["map_states"]){
Ros1Clearpath_mission_manager_msgsNetworkMapStateWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["map_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_mission_manager_msgsNetworkMapStateRead(r);
value["map_states"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsNetworkMapValidityStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_mission_manager_msgsNetworkMapStateCapture(element);
}out["map_states"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkMission codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkMissionCodec=Object.freeze({codecIdentity:"UWVMROS1_8dff81532124b3498e9e9e82252f2ba06804a29c6ccfadb7e75b04483d850c2e",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"name":"","uuid":"","waypoints":[],"on_start":[],"on_stop":[],"return_to_dock_enabled":false,"return_to_dock_id":"","return_to_dock_battery_level":0,"resume_mission_battery_level":0,"network_replan_enabled":false,"onav_config":""};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkMissionSize(Ros1Clearpath_navigation_msgsNetworkMissionCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkMissionCapture(value),n=Ros1Clearpath_navigation_msgsNetworkMissionSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkMissionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkMissionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(element));
}
if(!Array.isArray(value["on_start"])||value["on_start"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(!Array.isArray(value["on_stop"])||value["on_stop"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(typeof value["return_to_dock_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["return_to_dock_id"],256).length);
if(typeof value["return_to_dock_battery_level"]!=="number"||!Number.isFinite(value["return_to_dock_battery_level"])||!Number.isFinite(Math.fround(value["return_to_dock_battery_level"])))bad();n=addSize(n,4);
if(typeof value["resume_mission_battery_level"]!=="number"||!Number.isFinite(value["resume_mission_battery_level"])||!Number.isFinite(Math.fround(value["resume_mission_battery_level"])))bad();n=addSize(n,4);
if(typeof value["network_replan_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(n>13606)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkMissionWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["name"],128);
w.text(value["uuid"],256);
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Clearpath_navigation_msgsWaypointWrite(element,w);
}
w.v.setUint32(w.at,value["on_start"].length,true);w.at+=4;
for(const element of value["on_start"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
w.v.setUint32(w.at,value["on_stop"].length,true);w.at+=4;
for(const element of value["on_stop"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
w.v.setUint8(w.at,value["return_to_dock_enabled"]?1:0,true);w.at+=1;
w.text(value["return_to_dock_id"],256);
w.v.setFloat32(w.at,value["return_to_dock_battery_level"],true);w.at+=4;
w.v.setFloat32(w.at,value["resume_mission_battery_level"],true);w.at+=4;
w.v.setUint8(w.at,value["network_replan_enabled"]?1:0,true);w.at+=1;
w.text(value["onav_config"],256);
}
function Ros1Clearpath_navigation_msgsNetworkMissionRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoints"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["on_start"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["on_stop"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["return_to_dock_enabled"]=decoded!==0;
}
{
const decoded=r.text(256);
value["return_to_dock_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["return_to_dock_battery_level"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["resume_mission_battery_level"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["network_replan_enabled"]=decoded!==0;
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkMissionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsWaypointCapture(element);
}out["waypoints"]=values;
}
{const field=value["on_start"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["on_start"]=values;
}
{const field=value["on_stop"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["on_stop"]=values;
}
{const field=value["return_to_dock_enabled"];
out["return_to_dock_enabled"]=field;
}
{const field=value["return_to_dock_id"];
out["return_to_dock_id"]=field;
}
{const field=value["return_to_dock_battery_level"];
out["return_to_dock_battery_level"]=field;
}
{const field=value["resume_mission_battery_level"];
out["resume_mission_battery_level"]=field;
}
{const field=value["network_replan_enabled"];
out["network_replan_enabled"]=field;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkPoint codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkPointCodec=Object.freeze({codecIdentity:"UWVMROS1_d64f71c07c2c73314a1e23969620110e52e225f9d46a745413f84e2030855014",maxWireBytes:276,minWireBytes:20,cartesianMaxWireBytes:276n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","latitude":0,"longitude":0};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkPointSize(Ros1Clearpath_navigation_msgsNetworkPointCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkPointCapture(value),n=Ros1Clearpath_navigation_msgsNetworkPointSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,276,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(n>276)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkPointWrite(value,w){
w.text(value["uuid"],256);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
}
function Ros1Clearpath_navigation_msgsNetworkPointRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkEdge codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkEdgeCodec=Object.freeze({codecIdentity:"UWVMROS1_2d67f3d8176b117f6f00334633385aee6b1fd23a24a2c8b214e40793fc1c2943",maxWireBytes:824,minWireBytes:56,cartesianMaxWireBytes:824n,aggregateWireBytes:0,constants:Object.freeze({"MIN_COST_FACTOR":0.0,"MAX_COST_FACTOR":100.0,"DEFAULT_COST_FACTOR":1.0}),
defaultValue(){return {"uuid":"","start_point":Ros1Clearpath_navigation_msgsNetworkPointCodec.defaultValue(),"end_point":Ros1Clearpath_navigation_msgsNetworkPointCodec.defaultValue(),"radius":0,"speed_limit":0,"cost_factor":0};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkEdgeSize(Ros1Clearpath_navigation_msgsNetworkEdgeCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkEdgeCapture(value),n=Ros1Clearpath_navigation_msgsNetworkEdgeSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkEdgeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,824,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkEdgeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkEdgeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(value["start_point"]));
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(value["end_point"]));
if(typeof value["radius"]!=="number"||!Number.isFinite(value["radius"])||!Number.isFinite(Math.fround(value["radius"])))bad();n=addSize(n,4);
if(typeof value["speed_limit"]!=="number"||!Number.isFinite(value["speed_limit"])||!Number.isFinite(Math.fround(value["speed_limit"])))bad();n=addSize(n,4);
if(typeof value["cost_factor"]!=="number"||!Number.isFinite(value["cost_factor"])||!Number.isFinite(Math.fround(value["cost_factor"])))bad();n=addSize(n,4);
if(n>824)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkEdgeWrite(value,w){
w.text(value["uuid"],256);
Ros1Clearpath_navigation_msgsNetworkPointWrite(value["start_point"],w);
Ros1Clearpath_navigation_msgsNetworkPointWrite(value["end_point"],w);
w.v.setFloat32(w.at,value["radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["speed_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["cost_factor"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsNetworkEdgeRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["start_point"]=decoded;
}
{
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["end_point"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cost_factor"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkEdgeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["start_point"];
out["start_point"]=Ros1Clearpath_navigation_msgsNetworkPointCapture(field);
}
{const field=value["end_point"];
out["end_point"]=Ros1Clearpath_navigation_msgsNetworkPointCapture(field);
}
{const field=value["radius"];
out["radius"]=field;
}
{const field=value["speed_limit"];
out["speed_limit"]=field;
}
{const field=value["cost_factor"];
out["cost_factor"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NetworkMap codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNetworkMapCodec=Object.freeze({codecIdentity:"UWVMROS1_3b14eff4b520d1eaeb357959f984142dcfcc1039cb1434469247f32fb16f06fe",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","uuid":"","default_radius":0,"default_speed_limit":0,"points":[],"connections":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsNetworkMapSize(Ros1Clearpath_navigation_msgsNetworkMapCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNetworkMapCapture(value),n=Ros1Clearpath_navigation_msgsNetworkMapSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNetworkMapWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNetworkMapRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNetworkMapSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["default_radius"]!=="number"||!Number.isFinite(value["default_radius"])||!Number.isFinite(Math.fround(value["default_radius"])))bad();n=addSize(n,4);
if(typeof value["default_speed_limit"]!=="number"||!Number.isFinite(value["default_speed_limit"])||!Number.isFinite(Math.fround(value["default_speed_limit"])))bad();n=addSize(n,4);
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(element));
}
if(!Array.isArray(value["connections"])||value["connections"].length>2)bad();
n=addSize(n,4);
for(const element of value["connections"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkEdgeSize(element));
}
if(n>2608)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNetworkMapWrite(value,w){
w.text(value["name"],128);
w.text(value["uuid"],256);
w.v.setFloat32(w.at,value["default_radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["default_speed_limit"],true);w.at+=4;
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Clearpath_navigation_msgsNetworkPointWrite(element,w);
}
w.v.setUint32(w.at,value["connections"].length,true);w.at+=4;
for(const element of value["connections"]){
Ros1Clearpath_navigation_msgsNetworkEdgeWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsNetworkMapRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_speed_limit"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["points"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["connections"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkEdgeRead(r);
value["connections"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsNetworkMapCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["default_radius"];
out["default_radius"]=field;
}
{const field=value["default_speed_limit"];
out["default_speed_limit"]=field;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkPointCapture(element);
}out["points"]=values;
}
{const field=value["connections"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkEdgeCapture(element);
}out["connections"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/PointOfInterest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsPointOfInterestCodec=Object.freeze({codecIdentity:"UWVMROS1_8d302ade38f6b9c0046c577a1cb4fce0dad418fc4b1aa9de496409fac4632d2c",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({"TAG_GOTO":"goto","TAG_INSPECT":"inspect"}),
defaultValue(){return {"uuid":"","name":"","latitude":0,"longitude":0,"elevation":0,"enable_heading":false,"heading":0,"tags":[],"custom_fields_json":""};},
validateValue(value){Ros1Clearpath_navigation_msgsPointOfInterestSize(Ros1Clearpath_navigation_msgsPointOfInterestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsPointOfInterestCapture(value),n=Ros1Clearpath_navigation_msgsPointOfInterestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsPointOfInterestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsPointOfInterestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["elevation"]!=="number"||!Number.isFinite(value["elevation"]))bad();n=addSize(n,8);
if(typeof value["enable_heading"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(n>1209)bad(90);return n;}
function Ros1Clearpath_navigation_msgsPointOfInterestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["elevation"],true);w.at+=8;
w.v.setUint8(w.at,value["enable_heading"]?1:0,true);w.at+=1;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
w.text(value["custom_fields_json"],256);
}
function Ros1Clearpath_navigation_msgsPointOfInterestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elevation"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_heading"]=decoded!==0;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsPointOfInterestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["elevation"];
out["elevation"]=field;
}
{const field=value["enable_heading"];
out["enable_heading"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/StorageState codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsStorageStateCodec=Object.freeze({codecIdentity:"UWVMROS1_3f23f2e49c54f2e0c5b3302d299dace8b15bcb205530b7b0f040125ed8b6e189",maxWireBytes:32768,minWireBytes:24,cartesianMaxWireBytes:71156n,aggregateWireBytes:32768,constants:Object.freeze({}),
defaultValue(){return {"missions":[],"waypoints":[],"tasks":[],"network_missions":[],"network_maps":[],"points_of_interest":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsStorageStateSize(Ros1Clearpath_mission_manager_msgsStorageStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsStorageStateCapture(value),n=Ros1Clearpath_mission_manager_msgsStorageStateSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsStorageStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32768,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsStorageStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsStorageStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["missions"])||value["missions"].length>2)bad();
n=addSize(n,4);
for(const element of value["missions"]){
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(element));
}
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(element));
}
if(!Array.isArray(value["tasks"])||value["tasks"].length>2)bad();
n=addSize(n,4);
for(const element of value["tasks"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(!Array.isArray(value["network_missions"])||value["network_missions"].length>2)bad();
n=addSize(n,4);
for(const element of value["network_missions"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(element));
}
if(!Array.isArray(value["network_maps"])||value["network_maps"].length>2)bad();
n=addSize(n,4);
for(const element of value["network_maps"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(element));
}
if(!Array.isArray(value["points_of_interest"])||value["points_of_interest"].length>2)bad();
n=addSize(n,4);
for(const element of value["points_of_interest"]){
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(element));
}
if(n>32768)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsStorageStateWrite(value,w){
w.v.setUint32(w.at,value["missions"].length,true);w.at+=4;
for(const element of value["missions"]){
Ros1Clearpath_navigation_msgsMissionWrite(element,w);
}
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Clearpath_navigation_msgsWaypointWrite(element,w);
}
w.v.setUint32(w.at,value["tasks"].length,true);w.at+=4;
for(const element of value["tasks"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
w.v.setUint32(w.at,value["network_missions"].length,true);w.at+=4;
for(const element of value["network_missions"]){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(element,w);
}
w.v.setUint32(w.at,value["network_maps"].length,true);w.at+=4;
for(const element of value["network_maps"]){
Ros1Clearpath_navigation_msgsNetworkMapWrite(element,w);
}
w.v.setUint32(w.at,value["points_of_interest"].length,true);w.at+=4;
for(const element of value["points_of_interest"]){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsStorageStateRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["missions"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["missions"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoints"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["tasks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["tasks"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["network_missions"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["network_missions"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["network_maps"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["network_maps"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["points_of_interest"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["points_of_interest"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsStorageStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["missions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsMissionCapture(element);
}out["missions"]=values;
}
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsWaypointCapture(element);
}out["waypoints"]=values;
}
{const field=value["tasks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["tasks"]=values;
}
{const field=value["network_missions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(element);
}out["network_missions"]=values;
}
{const field=value["network_maps"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkMapCapture(element);
}out["network_maps"]=values;
}
{const field=value["points_of_interest"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(element);
}out["points_of_interest"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/NextSchedule codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsNextScheduleCodec=Object.freeze({codecIdentity:"UWVMROS1_a2be03201266ebf80af5587b59ff48514b3e97a536078c6b235c434e6dc80f3a",maxWireBytes:268,minWireBytes:12,cartesianMaxWireBytes:268n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","time_to_start":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsNextScheduleSize(Ros1Clearpath_mission_scheduler_msgsNextScheduleCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsNextScheduleCapture(value),n=Ros1Clearpath_mission_scheduler_msgsNextScheduleSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsNextScheduleWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,268,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsNextScheduleRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsNextScheduleSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,Ros1DurationSize(value["time_to_start"]));
if(n>268)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsNextScheduleWrite(value,w){
w.text(value["uuid"],256);
Ros1DurationWrite(value["time_to_start"],w);
}
function Ros1Clearpath_mission_scheduler_msgsNextScheduleRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["time_to_start"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsNextScheduleCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["time_to_start"];
out["time_to_start"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/UtcTime codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsUtcTimeCodec=Object.freeze({codecIdentity:"UWVMROS1_5d8b0173e89fb44a2c6444028d65232a492255e596b8a535d2aea714fca0ab49",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"hour":0,"minute":0,"second":0,"millisecond":0};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(value),n=Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsUtcTimeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsUtcTimeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["hour"],1,false);n=addSize(n,1);
integer(value["minute"],1,false);n=addSize(n,1);
integer(value["second"],1,false);n=addSize(n,1);
integer(value["millisecond"],2,false);n=addSize(n,2);
if(n>5)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsUtcTimeWrite(value,w){
w.v.setUint8(w.at,value["hour"],true);w.at+=1;
w.v.setUint8(w.at,value["minute"],true);w.at+=1;
w.v.setUint8(w.at,value["second"],true);w.at+=1;
w.v.setUint16(w.at,value["millisecond"],true);w.at+=2;
}
function Ros1Clearpath_mission_scheduler_msgsUtcTimeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["hour"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["minute"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["second"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["millisecond"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["hour"];
out["hour"]=field;
}
{const field=value["minute"];
out["minute"]=field;
}
{const field=value["second"];
out["second"]=field;
}
{const field=value["millisecond"];
out["millisecond"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/Schedule codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsScheduleCodec=Object.freeze({codecIdentity:"UWVMROS1_552fe3494bad6517edf7d69e98021b0ebc373bf084de7335d57d1e074caa1465",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({"MODE_ONCE":0,"MODE_LOOP":1,"MODE_TRANSIENT":2}),
defaultValue(){return {"name":"","uuid":"","start_time":Ros1Clearpath_mission_scheduler_msgsUtcTimeCodec.defaultValue(),"mode":0,"loop_repeats":0,"loop_interval":Ros1DurationCodec.defaultValue(),"mission_ids":[],"min_battery":0,"timeout":Ros1DurationCodec.defaultValue(),"enabled":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsScheduleSize(Ros1Clearpath_mission_scheduler_msgsScheduleCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(value),n=Ros1Clearpath_mission_scheduler_msgsScheduleSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsScheduleWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsScheduleSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(value["start_time"]));
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["loop_repeats"],1,false);n=addSize(n,1);
n=addSize(n,Ros1DurationSize(value["loop_interval"]));
if(!Array.isArray(value["mission_ids"])||value["mission_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["mission_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["min_battery"]!=="number"||!Number.isFinite(value["min_battery"])||!Number.isFinite(Math.fround(value["min_battery"])))bad();n=addSize(n,4);
n=addSize(n,Ros1DurationSize(value["timeout"]));
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsScheduleWrite(value,w){
w.text(value["name"],128);
w.text(value["uuid"],256);
Ros1Clearpath_mission_scheduler_msgsUtcTimeWrite(value["start_time"],w);
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["loop_repeats"],true);w.at+=1;
Ros1DurationWrite(value["loop_interval"],w);
w.v.setUint32(w.at,value["mission_ids"].length,true);w.at+=4;
for(const element of value["mission_ids"]){
w.text(element,256);
}
w.v.setFloat32(w.at,value["min_battery"],true);w.at+=4;
Ros1DurationWrite(value["timeout"],w);
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsScheduleRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=Ros1Clearpath_mission_scheduler_msgsUtcTimeRead(r);
value["start_time"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["loop_repeats"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["loop_interval"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["mission_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["mission_ids"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["min_battery"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["timeout"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsScheduleCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["start_time"];
out["start_time"]=Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(field);
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["loop_repeats"];
out["loop_repeats"]=field;
}
{const field=value["loop_interval"];
out["loop_interval"]=Ros1DurationCapture(field);
}
{const field=value["mission_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mission_ids"]=values;
}
{const field=value["min_battery"];
out["min_battery"]=field;
}
{const field=value["timeout"];
out["timeout"]=Ros1DurationCapture(field);
}
{const field=value["enabled"];
out["enabled"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/StorageState codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsStorageStateCodec=Object.freeze({codecIdentity:"UWVMROS1_e2993af302bafec841a18f71a54d104c34b9c7ce91c84e3e4ee3be7f643a4f9a",maxWireBytes:1892,minWireBytes:4,cartesianMaxWireBytes:1892n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"schedules":[]};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsStorageStateSize(Ros1Clearpath_mission_scheduler_msgsStorageStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsStorageStateCapture(value),n=Ros1Clearpath_mission_scheduler_msgsStorageStateSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsStorageStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1892,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsStorageStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsStorageStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["schedules"])||value["schedules"].length>2)bad();
n=addSize(n,4);
for(const element of value["schedules"]){
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(element));
}
if(n>1892)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsStorageStateWrite(value,w){
w.v.setUint32(w.at,value["schedules"].length,true);w.at+=4;
for(const element of value["schedules"]){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(element,w);
}
}
function Ros1Clearpath_mission_scheduler_msgsStorageStateRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["schedules"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["schedules"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsStorageStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["schedules"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(element);
}out["schedules"]=values;
}
return out;}
/** Complete ROS1 std_msgs/Time codec; maxima are not actual lengths. */
export const Ros1Std_msgsTimeCodec=Object.freeze({codecIdentity:"UWVMROS1_64c147e5ad9757a29f19692f4f933be0b2079e1b8ae40566bb3c20841734a7d1",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":Ros1TimeCodec.defaultValue()};},
validateValue(value){Ros1Std_msgsTimeSize(Ros1Std_msgsTimeCapture(value));return true;},
encode(value){const owned=Ros1Std_msgsTimeCapture(value),n=Ros1Std_msgsTimeSize(owned),w=new Writer(n);Ros1Std_msgsTimeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Std_msgsTimeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_msgsTimeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1TimeSize(value["data"]));
if(n>8)bad(90);return n;}
function Ros1Std_msgsTimeWrite(value,w){
Ros1TimeWrite(value["data"],w);
}
function Ros1Std_msgsTimeRead(r){const value={};
{
const decoded=Ros1TimeRead(r);
value["data"]=decoded;
}
return value;}
function Ros1Std_msgsTimeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=Ros1TimeCapture(field);
}
return out;}
/** Complete ROS1 std_msgs/String codec; maxima are not actual lengths. */
export const Ros1Std_msgsStringCodec=Object.freeze({codecIdentity:"UWVMROS1_2e8d1356eb6d39d2456dc2744c3aab174c093deebb56384704976f0a9aae5413",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Std_msgsStringSize(Ros1Std_msgsStringCapture(value));return true;},
encode(value){const owned=Ros1Std_msgsStringCapture(value),n=Ros1Std_msgsStringSize(owned),w=new Writer(n);Ros1Std_msgsStringWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Std_msgsStringRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_msgsStringSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Std_msgsStringWrite(value,w){
w.text(value["data"],256);
}
function Ros1Std_msgsStringRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Std_msgsStringCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/AutonomyStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsAutonomyStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_69ab909d6efdf65bcd36d0f8c159047b0dc3ecd5e9f03015936b69c6397dfdaf",maxWireBytes:673,minWireBytes:33,cartesianMaxWireBytes:673n,aggregateWireBytes:0,constants:Object.freeze({"IDLE":0,"MISSION_DIRECT":1,"MISSION_BY_ID":2,"NETWORK_MISSION":3,"NETWORK_GOTO":4,"NETWORK_GOTO_POI":5,"LOCAL_DOCKING":6,"LOCAL_UNDOCKING":7,"RADIUS_DOCKING":8,"NETWORK_DOCKING":9}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"last_start":Ros1Std_msgsTimeCodec.defaultValue(),"mission_uuid":Ros1Std_msgsStringCodec.defaultValue(),"map_uuid":Ros1Std_msgsStringCodec.defaultValue(),"state":0};},
validateValue(value){Ros1Clearpath_navigation_msgsAutonomyStatusSize(Ros1Clearpath_navigation_msgsAutonomyStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsAutonomyStatusCapture(value),n=Ros1Clearpath_navigation_msgsAutonomyStatusSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsAutonomyStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,673,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsAutonomyStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsAutonomyStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Std_msgsTimeSize(value["last_start"]));
n=addSize(n,Ros1Std_msgsStringSize(value["mission_uuid"]));
n=addSize(n,Ros1Std_msgsStringSize(value["map_uuid"]));
integer(value["state"],1,false);n=addSize(n,1);
if(n>673)bad(90);return n;}
function Ros1Clearpath_navigation_msgsAutonomyStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Std_msgsTimeWrite(value["last_start"],w);
Ros1Std_msgsStringWrite(value["mission_uuid"],w);
Ros1Std_msgsStringWrite(value["map_uuid"],w);
w.v.setUint8(w.at,value["state"],true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsAutonomyStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Std_msgsTimeRead(r);
value["last_start"]=decoded;
}
{
const decoded=Ros1Std_msgsStringRead(r);
value["mission_uuid"]=decoded;
}
{
const decoded=Ros1Std_msgsStringRead(r);
value["map_uuid"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsAutonomyStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["last_start"];
out["last_start"]=Ros1Std_msgsTimeCapture(field);
}
{const field=value["mission_uuid"];
out["mission_uuid"]=Ros1Std_msgsStringCapture(field);
}
{const field=value["map_uuid"];
out["map_uuid"]=Ros1Std_msgsStringCapture(field);
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/DistanceToGoal codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsDistanceToGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_d1dec9c2784feaf6a70c2a5291ed9cb407145d6f42e2132ad71a141f5993a519",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"euclidean":0,"path":0};},
validateValue(value){Ros1Clearpath_navigation_msgsDistanceToGoalSize(Ros1Clearpath_navigation_msgsDistanceToGoalCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsDistanceToGoalCapture(value),n=Ros1Clearpath_navigation_msgsDistanceToGoalSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsDistanceToGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsDistanceToGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsDistanceToGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["euclidean"]!=="number"||!Number.isFinite(value["euclidean"])||!Number.isFinite(Math.fround(value["euclidean"])))bad();n=addSize(n,4);
if(typeof value["path"]!=="number"||!Number.isFinite(value["path"])||!Number.isFinite(Math.fround(value["path"])))bad();n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros1Clearpath_navigation_msgsDistanceToGoalWrite(value,w){
w.v.setFloat32(w.at,value["euclidean"],true);w.at+=4;
w.v.setFloat32(w.at,value["path"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsDistanceToGoalRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["euclidean"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["path"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsDistanceToGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["euclidean"];
out["euclidean"]=field;
}
{const field=value["path"];
out["path"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Metrics codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMetricsCodec=Object.freeze({codecIdentity:"UWVMROS1_6e28edfbea9de8f1e0e52e200560b25b0f636160461f1bd99d353595906d7c19",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"total_distance_travelled":0,"total_duration":0,"num_missions_sent":0n,"num_missions_completed":0n,"num_replans":0n,"num_recoveries":0n,"num_tasks_executed":0n};},
validateValue(value){Ros1Clearpath_navigation_msgsMetricsSize(Ros1Clearpath_navigation_msgsMetricsCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMetricsCapture(value),n=Ros1Clearpath_navigation_msgsMetricsSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMetricsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMetricsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMetricsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["total_distance_travelled"]!=="number"||!Number.isFinite(value["total_distance_travelled"]))bad();n=addSize(n,8);
if(typeof value["total_duration"]!=="number"||!Number.isFinite(value["total_duration"]))bad();n=addSize(n,8);
integer(value["num_missions_sent"],8,false);n=addSize(n,8);
integer(value["num_missions_completed"],8,false);n=addSize(n,8);
integer(value["num_replans"],8,false);n=addSize(n,8);
integer(value["num_recoveries"],8,false);n=addSize(n,8);
integer(value["num_tasks_executed"],8,false);n=addSize(n,8);
if(n>56)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMetricsWrite(value,w){
w.v.setFloat64(w.at,value["total_distance_travelled"],true);w.at+=8;
w.v.setFloat64(w.at,value["total_duration"],true);w.at+=8;
w.v.setBigUint64(w.at,value["num_missions_sent"],true);w.at+=8;
w.v.setBigUint64(w.at,value["num_missions_completed"],true);w.at+=8;
w.v.setBigUint64(w.at,value["num_replans"],true);w.at+=8;
w.v.setBigUint64(w.at,value["num_recoveries"],true);w.at+=8;
w.v.setBigUint64(w.at,value["num_tasks_executed"],true);w.at+=8;
}
function Ros1Clearpath_navigation_msgsMetricsRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["total_distance_travelled"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["total_duration"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["num_missions_sent"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["num_missions_completed"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["num_replans"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["num_recoveries"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["num_tasks_executed"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsMetricsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["total_distance_travelled"];
out["total_distance_travelled"]=field;
}
{const field=value["total_duration"];
out["total_duration"]=field;
}
{const field=value["num_missions_sent"];
out["num_missions_sent"]=field;
}
{const field=value["num_missions_completed"];
out["num_missions_completed"]=field;
}
{const field=value["num_replans"];
out["num_replans"]=field;
}
{const field=value["num_recoveries"];
out["num_recoveries"]=field;
}
{const field=value["num_tasks_executed"];
out["num_tasks_executed"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/WaypointStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsWaypointStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_8a69049422ce8a2ef23a319f2d3ed7783455381a91b7083fca402a3ca59c6644",maxWireBytes:3352,minWireBytes:54,cartesianMaxWireBytes:3352n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue(),"visited":false,"skipped":false};},
validateValue(value){Ros1Clearpath_navigation_msgsWaypointStatusSize(Ros1Clearpath_navigation_msgsWaypointStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsWaypointStatusCapture(value),n=Ros1Clearpath_navigation_msgsWaypointStatusSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsWaypointStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3352,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsWaypointStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsWaypointStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(typeof value["visited"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["skipped"]!=="boolean")bad();n=addSize(n,1);
if(n>3352)bad(90);return n;}
function Ros1Clearpath_navigation_msgsWaypointStatusWrite(value,w){
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
w.v.setUint8(w.at,value["visited"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["skipped"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsWaypointStatusRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["visited"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["skipped"]=decoded!==0;
}
return value;}
function Ros1Clearpath_navigation_msgsWaypointStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
{const field=value["visited"];
out["visited"]=field;
}
{const field=value["skipped"];
out["skipped"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/MissionStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMissionStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_e1ecf20b0466f7bda1f41537b80304e5c93cd0d7e599a4164da047a653edd9e4",maxWireBytes:7244,minWireBytes:28,cartesianMaxWireBytes:7244n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"name":"","uuid":"","waypoints":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsMissionStatusSize(Ros1Clearpath_navigation_msgsMissionStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMissionStatusCapture(value),n=Ros1Clearpath_navigation_msgsMissionStatusSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMissionStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7244,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMissionStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMissionStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointStatusSize(element));
}
if(n>7244)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMissionStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["name"],128);
w.text(value["uuid"],256);
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Clearpath_navigation_msgsWaypointStatusWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsMissionStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsWaypointStatusRead(r);
value["waypoints"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsMissionStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsWaypointStatusCapture(element);
}out["waypoints"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/MotionState codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsMotionStateCodec=Object.freeze({codecIdentity:"UWVMROS1_04e1a2cc31ea5b08b7f385b305b8e89acc31a7c6675b36a473930116076a1efa",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({"MOTION_NORMAL":10,"MOTION_STOPPED":11,"MOTION_REVERSE":12,"MOTION_SLOWING":13,"INDICATOR_NORMAL":20,"INDICATOR_LEFT_TURN":21,"INDICATOR_RIGHT_TURN":22,"INDICATOR_CLOCKWISE":23,"INDICATOR_COUNTERCLOCKWISE":24,"INDICATOR_HAZARD":25,"DIRECTION_FORWARD":30,"DIRECTION_REVERSE":31}),
defaultValue(){return {"motion":0,"indicator":0,"direction":0};},
validateValue(value){Ros1Clearpath_navigation_msgsMotionStateSize(Ros1Clearpath_navigation_msgsMotionStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsMotionStateCapture(value),n=Ros1Clearpath_navigation_msgsMotionStateSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsMotionStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsMotionStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsMotionStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["motion"],1,false);n=addSize(n,1);
integer(value["indicator"],1,false);n=addSize(n,1);
integer(value["direction"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Clearpath_navigation_msgsMotionStateWrite(value,w){
w.v.setUint8(w.at,value["motion"],true);w.at+=1;
w.v.setUint8(w.at,value["indicator"],true);w.at+=1;
w.v.setUint8(w.at,value["direction"],true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsMotionStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["motion"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["indicator"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["direction"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsMotionStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["motion"];
out["motion"]=field;
}
{const field=value["indicator"];
out["indicator"]=field;
}
{const field=value["direction"];
out["direction"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/NavigationState codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsNavigationStateCodec=Object.freeze({codecIdentity:"UWVMROS1_59723baf8c5871ea7bff969a09fe753455ef07d0cbc982ef23c1f55b984a7d2a",maxWireBytes:6,minWireBytes:4,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"STATE_IDLE":0,"STATE_COMPUTE_PATH":1,"STATE_EXECUTE_PATH":2,"STATE_REPLAN":3,"STATE_NAVIGATING_AROUND_OBSTACLE":4,"STATE_RECOVERY":5,"STATE_LOST":6,"STATE_DONE":7,"STATE_SAFETY_STOP":8,"STATE_PAUSE":9,"STATE_DISABLE":10}),
defaultValue(){return {"states":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsNavigationStateSize(Ros1Clearpath_navigation_msgsNavigationStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsNavigationStateCapture(value),n=Ros1Clearpath_navigation_msgsNavigationStateSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsNavigationStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsNavigationStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsNavigationStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["states"])||value["states"].length>2)bad();
n=addSize(n,4);
for(const element of value["states"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>6)bad(90);return n;}
function Ros1Clearpath_navigation_msgsNavigationStateWrite(value,w){
w.v.setUint32(w.at,value["states"].length,true);w.at+=4;
for(const element of value["states"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros1Clearpath_navigation_msgsNavigationStateRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["states"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["states"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsNavigationStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["states"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/OdomIntent codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsOdomIntentCodec=Object.freeze({codecIdentity:"UWVMROS1_84aa6a07039ac3402c2762cdf89f217053c0f7e2448f535e12282c1184635f69",maxWireBytes:2316,minWireBytes:1420,cartesianMaxWireBytes:2316n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"odom":Ros1Nav_msgsOdometryCodec.defaultValue(),"predicted_odom":Ros1Nav_msgsOdometryCodec.defaultValue(),"dt":0};},
validateValue(value){Ros1Clearpath_navigation_msgsOdomIntentSize(Ros1Clearpath_navigation_msgsOdomIntentCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsOdomIntentCapture(value),n=Ros1Clearpath_navigation_msgsOdomIntentSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsOdomIntentWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2316,90);if(owned.length<1420)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsOdomIntentRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsOdomIntentSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,Ros1Nav_msgsOdometrySize(value["odom"]));
n=addSize(n,Ros1Nav_msgsOdometrySize(value["predicted_odom"]));
if(typeof value["dt"]!=="number"||!Number.isFinite(value["dt"])||!Number.isFinite(Math.fround(value["dt"])))bad();n=addSize(n,4);
if(n>2316)bad(90);return n;}
function Ros1Clearpath_navigation_msgsOdomIntentWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
Ros1Nav_msgsOdometryWrite(value["odom"],w);
Ros1Nav_msgsOdometryWrite(value["predicted_odom"],w);
w.v.setFloat32(w.at,value["dt"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsOdomIntentRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros1Nav_msgsOdometryRead(r);
value["odom"]=decoded;
}
{
const decoded=Ros1Nav_msgsOdometryRead(r);
value["predicted_odom"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["dt"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsOdomIntentCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["odom"];
out["odom"]=Ros1Nav_msgsOdometryCapture(field);
}
{const field=value["predicted_odom"];
out["predicted_odom"]=Ros1Nav_msgsOdometryCapture(field);
}
{const field=value["dt"];
out["dt"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Vector2D codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsVector2DCodec=Object.freeze({codecIdentity:"UWVMROS1_fab839d4e160f8dca805b36fd2d93362ea5929238c33f34f0695f8edaed15047",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"point":Array.from({length:2},()=>(0))};},
validateValue(value){Ros1Clearpath_navigation_msgsVector2DSize(Ros1Clearpath_navigation_msgsVector2DCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsVector2DCapture(value),n=Ros1Clearpath_navigation_msgsVector2DSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsVector2DWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsVector2DRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsVector2DSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["point"])||value["point"].length!==2)bad();
for(const element of value["point"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>8)bad(90);return n;}
function Ros1Clearpath_navigation_msgsVector2DWrite(value,w){
for(const element of value["point"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Clearpath_navigation_msgsVector2DRead(r){const value={};
{
{const count=2;if(count>2)bad();value["point"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["point"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsVector2DCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["point"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["point"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/PointVector codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsPointVectorCodec=Object.freeze({codecIdentity:"UWVMROS1_4372dc043aaa0a9b16845e53db15405ec5dd98c4b5bc314319d185ba4557c9b8",maxWireBytes:164,minWireBytes:20,cartesianMaxWireBytes:164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"cloud":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsPointVectorSize(Ros1Clearpath_navigation_msgsPointVectorCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsPointVectorCapture(value),n=Ros1Clearpath_navigation_msgsPointVectorSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsPointVectorWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,164,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsPointVectorRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsPointVectorSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["cloud"])||value["cloud"].length>2)bad();
n=addSize(n,4);
for(const element of value["cloud"]){
n=addSize(n,Ros1Clearpath_navigation_msgsVector2DSize(element));
}
if(n>164)bad(90);return n;}
function Ros1Clearpath_navigation_msgsPointVectorWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["cloud"].length,true);w.at+=4;
for(const element of value["cloud"]){
Ros1Clearpath_navigation_msgsVector2DWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsPointVectorRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["cloud"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsVector2DRead(r);
value["cloud"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsPointVectorCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["cloud"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsVector2DCapture(element);
}out["cloud"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/Progress codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsProgressCodec=Object.freeze({codecIdentity:"UWVMROS1_a85be472664da59faec23030269b688f4afd1d071c0f8d9d02d5c10d6f7649d7",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path_progress":0,"goal_progress":0,"mission_progress":0};},
validateValue(value){Ros1Clearpath_navigation_msgsProgressSize(Ros1Clearpath_navigation_msgsProgressCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsProgressCapture(value),n=Ros1Clearpath_navigation_msgsProgressSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsProgressWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsProgressRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsProgressSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["path_progress"]!=="number"||!Number.isFinite(value["path_progress"])||!Number.isFinite(Math.fround(value["path_progress"])))bad();n=addSize(n,4);
if(typeof value["goal_progress"]!=="number"||!Number.isFinite(value["goal_progress"])||!Number.isFinite(Math.fround(value["goal_progress"])))bad();n=addSize(n,4);
if(typeof value["mission_progress"]!=="number"||!Number.isFinite(value["mission_progress"])||!Number.isFinite(Math.fround(value["mission_progress"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros1Clearpath_navigation_msgsProgressWrite(value,w){
w.v.setFloat32(w.at,value["path_progress"],true);w.at+=4;
w.v.setFloat32(w.at,value["goal_progress"],true);w.at+=4;
w.v.setFloat32(w.at,value["mission_progress"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsProgressRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["path_progress"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["goal_progress"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mission_progress"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsProgressCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path_progress"];
out["path_progress"]=field;
}
{const field=value["goal_progress"];
out["goal_progress"]=field;
}
{const field=value["mission_progress"];
out["mission_progress"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/ScanPoints codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsScanPointsCodec=Object.freeze({codecIdentity:"UWVMROS1_99d632501402a495a9337dee91bda1c0078fa379b9dbf872b45ad29af85a25b5",maxWireBytes:1000,minWireBytes:24,cartesianMaxWireBytes:1000n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"frames_vector":[],"cloud_vector":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsScanPointsSize(Ros1Clearpath_navigation_msgsScanPointsCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsScanPointsCapture(value),n=Ros1Clearpath_navigation_msgsScanPointsSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsScanPointsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1000,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsScanPointsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsScanPointsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["frames_vector"])||value["frames_vector"].length>2)bad();
n=addSize(n,4);
for(const element of value["frames_vector"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["cloud_vector"])||value["cloud_vector"].length>2)bad();
n=addSize(n,4);
for(const element of value["cloud_vector"]){
n=addSize(n,Ros1Clearpath_navigation_msgsPointVectorSize(element));
}
if(n>1000)bad(90);return n;}
function Ros1Clearpath_navigation_msgsScanPointsWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["frames_vector"].length,true);w.at+=4;
for(const element of value["frames_vector"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["cloud_vector"].length,true);w.at+=4;
for(const element of value["cloud_vector"]){
Ros1Clearpath_navigation_msgsPointVectorWrite(element,w);
}
}
function Ros1Clearpath_navigation_msgsScanPointsRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["frames_vector"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["frames_vector"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["cloud_vector"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsPointVectorRead(r);
value["cloud_vector"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsScanPointsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["frames_vector"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["frames_vector"]=values;
}
{const field=value["cloud_vector"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsPointVectorCapture(element);
}out["cloud_vector"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/TrackError codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsTrackErrorCodec=Object.freeze({codecIdentity:"UWVMROS1_ddaf051241e0ae7ffa19ed965cbaf65bb7c11a0b7feaa2e0530ca6068a633323",maxWireBytes:152,minWireBytes:24,cartesianMaxWireBytes:152n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"cross_track_error":0,"heading_error":0};},
validateValue(value){Ros1Clearpath_navigation_msgsTrackErrorSize(Ros1Clearpath_navigation_msgsTrackErrorCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsTrackErrorCapture(value),n=Ros1Clearpath_navigation_msgsTrackErrorSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsTrackErrorWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,152,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsTrackErrorRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsTrackErrorSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["cross_track_error"]!=="number"||!Number.isFinite(value["cross_track_error"])||!Number.isFinite(Math.fround(value["cross_track_error"])))bad();n=addSize(n,4);
if(typeof value["heading_error"]!=="number"||!Number.isFinite(value["heading_error"])||!Number.isFinite(Math.fround(value["heading_error"])))bad();n=addSize(n,4);
if(n>152)bad(90);return n;}
function Ros1Clearpath_navigation_msgsTrackErrorWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setFloat32(w.at,value["cross_track_error"],true);w.at+=4;
w.v.setFloat32(w.at,value["heading_error"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsTrackErrorRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cross_track_error"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["heading_error"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsTrackErrorCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["cross_track_error"];
out["cross_track_error"]=field;
}
{const field=value["heading_error"];
out["heading_error"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Drive codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsDriveCodec=Object.freeze({codecIdentity:"UWVMROS1_5189b2de776409879f0b7baa8687b3c59663b886de4a9038a9bff5b7da962ea6",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({"MODE_VELOCITY":0,"MODE_PWM":1,"MODE_NONE":-1,"LEFT":0,"RIGHT":1}),
defaultValue(){return {"mode":0,"drivers":Array.from({length:2},()=>(0))};},
validateValue(value){Ros1Clearpath_platform_msgsDriveSize(Ros1Clearpath_platform_msgsDriveCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsDriveCapture(value),n=Ros1Clearpath_platform_msgsDriveSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsDriveWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsDriveRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsDriveSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>9)bad(90);return n;}
function Ros1Clearpath_platform_msgsDriveWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
for(const element of value["drivers"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Clearpath_platform_msgsDriveRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["mode"]=decoded;
}
{
{const count=2;if(count>2)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["drivers"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_platform_msgsDriveCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["drivers"]=values;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/DriveFeedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsDriveFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_07cc1a72329f8ef223455d21ed1ac6398ea48b87c5be0b78aec5513791172c16",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current":0,"duty_cycle":0,"bridge_temperature":0,"motor_temperature":0,"measured_velocity":0,"measured_travel":0,"driver_fault":false};},
validateValue(value){Ros1Clearpath_platform_msgsDriveFeedbackSize(Ros1Clearpath_platform_msgsDriveFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsDriveFeedbackCapture(value),n=Ros1Clearpath_platform_msgsDriveFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsDriveFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsDriveFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsDriveFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["duty_cycle"]!=="number"||!Number.isFinite(value["duty_cycle"])||!Number.isFinite(Math.fround(value["duty_cycle"])))bad();n=addSize(n,4);
if(typeof value["bridge_temperature"]!=="number"||!Number.isFinite(value["bridge_temperature"])||!Number.isFinite(Math.fround(value["bridge_temperature"])))bad();n=addSize(n,4);
if(typeof value["motor_temperature"]!=="number"||!Number.isFinite(value["motor_temperature"])||!Number.isFinite(Math.fround(value["motor_temperature"])))bad();n=addSize(n,4);
if(typeof value["measured_velocity"]!=="number"||!Number.isFinite(value["measured_velocity"])||!Number.isFinite(Math.fround(value["measured_velocity"])))bad();n=addSize(n,4);
if(typeof value["measured_travel"]!=="number"||!Number.isFinite(value["measured_travel"])||!Number.isFinite(Math.fround(value["measured_travel"])))bad();n=addSize(n,4);
if(typeof value["driver_fault"]!=="boolean")bad();n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Clearpath_platform_msgsDriveFeedbackWrite(value,w){
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat32(w.at,value["duty_cycle"],true);w.at+=4;
w.v.setFloat32(w.at,value["bridge_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["motor_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_velocity"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_travel"],true);w.at+=4;
w.v.setUint8(w.at,value["driver_fault"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_platform_msgsDriveFeedbackRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["duty_cycle"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["bridge_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["motor_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_velocity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_travel"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["driver_fault"]=decoded!==0;
}
return value;}
function Ros1Clearpath_platform_msgsDriveFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current"];
out["current"]=field;
}
{const field=value["duty_cycle"];
out["duty_cycle"]=field;
}
{const field=value["bridge_temperature"];
out["bridge_temperature"]=field;
}
{const field=value["motor_temperature"];
out["motor_temperature"]=field;
}
{const field=value["measured_velocity"];
out["measured_velocity"]=field;
}
{const field=value["measured_travel"];
out["measured_travel"]=field;
}
{const field=value["driver_fault"];
out["driver_fault"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Fans codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsFansCodec=Object.freeze({codecIdentity:"UWVMROS1_5ce5485623fa8b07de5f3094110877bf5741c94e90a508fb0bb66a9552a13fb5",maxWireBytes:6,minWireBytes:4,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"RIDGEBACK_EQUIPMENT_BAY_INTAKE":0,"RIDGEBACK_EQUIPMENT_BAY_EXHAUST":1,"RIDGEBACK_CHARGER_BAY_INTAKE":2,"RIDGEBACK_CHARGER_BAY_EXHAUST":3,"RIDGEBACK_USER_BAY_INTAKE":4,"RIDGEBACK_USER_BAY_EXHAUST":5,"FAN_OFF":0,"FAN_ON_HIGH":1,"FAN_ON_LOW":2}),
defaultValue(){return {"fans":[]};},
validateValue(value){Ros1Clearpath_platform_msgsFansSize(Ros1Clearpath_platform_msgsFansCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsFansCapture(value),n=Ros1Clearpath_platform_msgsFansSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsFansWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsFansRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsFansSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["fans"])||value["fans"].length>2)bad();
n=addSize(n,4);
for(const element of value["fans"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>6)bad(90);return n;}
function Ros1Clearpath_platform_msgsFansWrite(value,w){
w.v.setUint32(w.at,value["fans"].length,true);w.at+=4;
for(const element of value["fans"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros1Clearpath_platform_msgsFansRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["fans"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["fans"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_platform_msgsFansCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["fans"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["fans"]=values;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Feedback codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_38f073bad8f592e143e0a60cb123a652bf00b72b160dd9e0a356bfc73eb0312a",maxWireBytes:196,minWireBytes:68,cartesianMaxWireBytes:196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"drivers":Array.from({length:2},()=>(Ros1Clearpath_platform_msgsDriveFeedbackCodec.defaultValue())),"commanded_mode":0,"actual_mode":0};},
validateValue(value){Ros1Clearpath_platform_msgsFeedbackSize(Ros1Clearpath_platform_msgsFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsFeedbackCapture(value),n=Ros1Clearpath_platform_msgsFeedbackSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,196,90);if(owned.length<68)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
n=addSize(n,Ros1Clearpath_platform_msgsDriveFeedbackSize(element));
}
integer(value["commanded_mode"],1,true);n=addSize(n,1);
integer(value["actual_mode"],1,true);n=addSize(n,1);
if(n>196)bad(90);return n;}
function Ros1Clearpath_platform_msgsFeedbackWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
for(const element of value["drivers"]){
Ros1Clearpath_platform_msgsDriveFeedbackWrite(element,w);
}
w.v.setInt8(w.at,value["commanded_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["actual_mode"],true);w.at+=1;
}
function Ros1Clearpath_platform_msgsFeedbackRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=2;if(count>2)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_platform_msgsDriveFeedbackRead(r);
value["drivers"].push(decoded);
}}
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["commanded_mode"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["actual_mode"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_platform_msgsDriveFeedbackCapture(element);
}out["drivers"]=values;
}
{const field=value["commanded_mode"];
out["commanded_mode"]=field;
}
{const field=value["actual_mode"];
out["actual_mode"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/RGB codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsRGBCodec=Object.freeze({codecIdentity:"UWVMROS1_8c848638975b7775b8ce0ff10d07c7d79c856f800745d30af6051c574c019056",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"red":0,"green":0,"blue":0};},
validateValue(value){Ros1Clearpath_platform_msgsRGBSize(Ros1Clearpath_platform_msgsRGBCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsRGBCapture(value),n=Ros1Clearpath_platform_msgsRGBSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsRGBWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsRGBRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsRGBSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["red"],1,false);n=addSize(n,1);
integer(value["green"],1,false);n=addSize(n,1);
integer(value["blue"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Clearpath_platform_msgsRGBWrite(value,w){
w.v.setUint8(w.at,value["red"],true);w.at+=1;
w.v.setUint8(w.at,value["green"],true);w.at+=1;
w.v.setUint8(w.at,value["blue"],true);w.at+=1;
}
function Ros1Clearpath_platform_msgsRGBRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["red"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["green"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["blue"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsRGBCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["red"];
out["red"]=field;
}
{const field=value["green"];
out["green"]=field;
}
{const field=value["blue"];
out["blue"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Lights codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsLightsCodec=Object.freeze({codecIdentity:"UWVMROS1_21d6dc7f4b57ae3468a7ee08945ee60f8d6bc9cbf24217f5ff48e55a4ce33c4b",maxWireBytes:10,minWireBytes:4,cartesianMaxWireBytes:10n,aggregateWireBytes:0,constants:Object.freeze({"DINGO_LIGHTS_REAR_LEFT":0,"DINGO_LIGHTS_FRONT_LEFT":1,"DINGO_LIGHTS_FRONT_RIGHT":2,"DINGO_LIGHTS_REAR_RIGHT":3,"RIDGEBACK_LIGHTS_FRONT_PORT_UPPER":0,"RIDGEBACK_LIGHTS_FRONT_PORT_LOWER":1,"RIDGEBACK_LIGHTS_FRONT_STARBOARD_UPPER":2,"RIDGEBACK_LIGHTS_FRONT_STARBOARD_LOWER":3,"RIDGEBACK_LIGHTS_REAR_PORT_UPPER":4,"RIDGEBACK_LIGHTS_REAR_PORT_LOWER":5,"RIDGEBACK_LIGHTS_REAR_STARBOARD_UPPER":6,"RIDGEBACK_LIGHTS_REAR_STARBOARD_LOWER":7,"WARTHOG_LIGHTS_FRONT_LEFT":0,"WARTHOG_LIGHTS_FRONT_RIGHT":1,"WARTHOG_LIGHTS_REAR_LEFT":2,"WARTHOG_LIGHTS_REAR_RIGHT":3}),
defaultValue(){return {"lights":[]};},
validateValue(value){Ros1Clearpath_platform_msgsLightsSize(Ros1Clearpath_platform_msgsLightsCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsLightsCapture(value),n=Ros1Clearpath_platform_msgsLightsSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsLightsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsLightsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsLightsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["lights"])||value["lights"].length>2)bad();
n=addSize(n,4);
for(const element of value["lights"]){
n=addSize(n,Ros1Clearpath_platform_msgsRGBSize(element));
}
if(n>10)bad(90);return n;}
function Ros1Clearpath_platform_msgsLightsWrite(value,w){
w.v.setUint32(w.at,value["lights"].length,true);w.at+=4;
for(const element of value["lights"]){
Ros1Clearpath_platform_msgsRGBWrite(element,w);
}
}
function Ros1Clearpath_platform_msgsLightsRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["lights"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_platform_msgsRGBRead(r);
value["lights"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_platform_msgsLightsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lights"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_platform_msgsRGBCapture(element);
}out["lights"]=values;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/PlatformID codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsPlatformIDCodec=Object.freeze({codecIdentity:"UWVMROS1_dc63cb65c84ae77ef0a36e938c77beeb234ad1e63acf11270ea4de337d5b2b07",maxWireBytes:1040,minWireBytes:16,cartesianMaxWireBytes:1040n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"model":"","serial_id":"","hardware_revision":"","firmware_revision":""};},
validateValue(value){Ros1Clearpath_platform_msgsPlatformIDSize(Ros1Clearpath_platform_msgsPlatformIDCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsPlatformIDCapture(value),n=Ros1Clearpath_platform_msgsPlatformIDSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsPlatformIDWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1040,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsPlatformIDRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsPlatformIDSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["model"],256).length);
n=addSize(n,4+textBytes(value["serial_id"],256).length);
n=addSize(n,4+textBytes(value["hardware_revision"],256).length);
n=addSize(n,4+textBytes(value["firmware_revision"],256).length);
if(n>1040)bad(90);return n;}
function Ros1Clearpath_platform_msgsPlatformIDWrite(value,w){
w.text(value["model"],256);
w.text(value["serial_id"],256);
w.text(value["hardware_revision"],256);
w.text(value["firmware_revision"],256);
}
function Ros1Clearpath_platform_msgsPlatformIDRead(r){const value={};
{
const decoded=r.text(256);
value["model"]=decoded;
}
{
const decoded=r.text(256);
value["serial_id"]=decoded;
}
{
const decoded=r.text(256);
value["hardware_revision"]=decoded;
}
{
const decoded=r.text(256);
value["firmware_revision"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsPlatformIDCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["model"];
out["model"]=field;
}
{const field=value["serial_id"];
out["serial_id"]=field;
}
{const field=value["hardware_revision"];
out["hardware_revision"]=field;
}
{const field=value["firmware_revision"];
out["firmware_revision"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Power codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsPowerCodec=Object.freeze({codecIdentity:"UWVMROS1_59c3c92a37e301e2fdfc43e0d1585138e26cda4c7821dab3085869c1c9115efc",maxWireBytes:172,minWireBytes:28,cartesianMaxWireBytes:172n,aggregateWireBytes:0,constants:Object.freeze({"NOT_APPLICABLE":-1,"JACKAL_MEASURED_BATTERY":0,"JACKAL_MEASURED_5V":1,"JACKAL_MEASURED_12V":2,"DINGO_MEASURED_BATTERY":0,"DINGO_MEASURED_5V":1,"DINGO_MEASURED_12V":2,"WARTHOG_MEASURED_BATTERY":0,"WARTHOG_MEASURED_12V":1,"WARTHOG_MEASURED_24V":2,"WARTHOG_MEASURED_48V":3,"RIDGEBACK_MEASURED_BATTERY":0,"RIDGEBACK_MEASURED_5V":1,"RIDGEBACK_MEASURED_12V":2,"RIDGEBACK_MEASURED_INVERTER":3,"RIDGEBACK_MEASURED_FRONT_AXLE":4,"RIDGEBACK_MEASURED_REAR_AXLE":5,"RIDGEBACK_MEASURED_LIGHT":6,"JACKAL_TOTAL_CURRENT":0,"JACKAL_COMPUTER_CURRENT":1,"JACKAL_DRIVE_CURRENT":2,"JACKAL_USER_CURRENT":3,"DINGO_TOTAL_CURRENT":0,"DINGO_COMPUTER_CURRENT":1,"WARTHOG_TOTAL_CURRENT":0,"WARTHOG_COMPUTER_CURRENT":1,"WARTHOG_12V_CURRENT":2,"WARTHOG_24V_CURRENT":3,"WARTHOG_48V_CURRENT":4,"RIDGEBACK_TOTAL_CURRENT":0}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"shore_power_connected":0,"battery_connected":0,"power_12v_user_nominal":0,"charging_complete":0,"measured_voltages":[],"measured_currents":[]};},
validateValue(value){Ros1Clearpath_platform_msgsPowerSize(Ros1Clearpath_platform_msgsPowerCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsPowerCapture(value),n=Ros1Clearpath_platform_msgsPowerSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsPowerWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,172,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsPowerRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsPowerSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
integer(value["shore_power_connected"],1,true);n=addSize(n,1);
integer(value["battery_connected"],1,true);n=addSize(n,1);
integer(value["power_12v_user_nominal"],1,true);n=addSize(n,1);
integer(value["charging_complete"],1,true);n=addSize(n,1);
if(!Array.isArray(value["measured_voltages"])||value["measured_voltages"].length>2)bad();
n=addSize(n,4);
for(const element of value["measured_voltages"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["measured_currents"])||value["measured_currents"].length>2)bad();
n=addSize(n,4);
for(const element of value["measured_currents"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>172)bad(90);return n;}
function Ros1Clearpath_platform_msgsPowerWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["shore_power_connected"],true);w.at+=1;
w.v.setInt8(w.at,value["battery_connected"],true);w.at+=1;
w.v.setInt8(w.at,value["power_12v_user_nominal"],true);w.at+=1;
w.v.setInt8(w.at,value["charging_complete"],true);w.at+=1;
w.v.setUint32(w.at,value["measured_voltages"].length,true);w.at+=4;
for(const element of value["measured_voltages"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["measured_currents"].length,true);w.at+=4;
for(const element of value["measured_currents"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Clearpath_platform_msgsPowerRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["shore_power_connected"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["battery_connected"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["power_12v_user_nominal"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["charging_complete"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["measured_voltages"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_voltages"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["measured_currents"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_currents"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_platform_msgsPowerCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["shore_power_connected"];
out["shore_power_connected"]=field;
}
{const field=value["battery_connected"];
out["battery_connected"]=field;
}
{const field=value["power_12v_user_nominal"];
out["power_12v_user_nominal"]=field;
}
{const field=value["charging_complete"];
out["charging_complete"]=field;
}
{const field=value["measured_voltages"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["measured_voltages"]=values;
}
{const field=value["measured_currents"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["measured_currents"]=values;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/RYG codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsRYGCodec=Object.freeze({codecIdentity:"UWVMROS1_7a5c0869aad03d552e5029582d0a63d70723954f84b63ab459f45121ea1da8df",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"red":0,"yellow":0,"green":0};},
validateValue(value){Ros1Clearpath_platform_msgsRYGSize(Ros1Clearpath_platform_msgsRYGCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsRYGCapture(value),n=Ros1Clearpath_platform_msgsRYGSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsRYGWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsRYGRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsRYGSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["red"],1,false);n=addSize(n,1);
integer(value["yellow"],1,false);n=addSize(n,1);
integer(value["green"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Clearpath_platform_msgsRYGWrite(value,w){
w.v.setUint8(w.at,value["red"],true);w.at+=1;
w.v.setUint8(w.at,value["yellow"],true);w.at+=1;
w.v.setUint8(w.at,value["green"],true);w.at+=1;
}
function Ros1Clearpath_platform_msgsRYGRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["red"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["yellow"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["green"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsRYGCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["red"];
out["red"]=field;
}
{const field=value["yellow"];
out["yellow"]=field;
}
{const field=value["green"];
out["green"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/StackLightState codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsStackLightStateCodec=Object.freeze({codecIdentity:"UWVMROS1_508b54b7050d7598a79aa238e96f210280796c177de357d9b969df4437d969e1",maxWireBytes:264,minWireBytes:8,cartesianMaxWireBytes:264n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"color":Ros1Clearpath_platform_msgsRYGCodec.defaultValue(),"strobe_on":false,"message":""};},
validateValue(value){Ros1Clearpath_platform_msgsStackLightStateSize(Ros1Clearpath_platform_msgsStackLightStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsStackLightStateCapture(value),n=Ros1Clearpath_platform_msgsStackLightStateSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsStackLightStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,264,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsStackLightStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsStackLightStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_platform_msgsRYGSize(value["color"]));
if(typeof value["strobe_on"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>264)bad(90);return n;}
function Ros1Clearpath_platform_msgsStackLightStateWrite(value,w){
Ros1Clearpath_platform_msgsRYGWrite(value["color"],w);
w.v.setUint8(w.at,value["strobe_on"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_platform_msgsStackLightStateRead(r){const value={};
{
const decoded=Ros1Clearpath_platform_msgsRYGRead(r);
value["color"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["strobe_on"]=decoded!==0;
}
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsStackLightStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["color"];
out["color"]=Ros1Clearpath_platform_msgsRYGCapture(field);
}
{const field=value["strobe_on"];
out["strobe_on"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/Status codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_9db960375fb63d011c0c038266421414c4c5f4ce1909ff5105e63dd2e1289a33",maxWireBytes:688,minWireBytes:48,cartesianMaxWireBytes:688n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"hardware_id":"","firmware_commit":"","mcu_uptime":Ros1Std_msgsDurationCodec.defaultValue(),"connection_uptime":Ros1Std_msgsDurationCodec.defaultValue(),"pcb_temperature":0,"mcu_temperature":0};},
validateValue(value){Ros1Clearpath_platform_msgsStatusSize(Ros1Clearpath_platform_msgsStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsStatusCapture(value),n=Ros1Clearpath_platform_msgsStatusSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,688,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["hardware_id"],256).length);
n=addSize(n,4+textBytes(value["firmware_commit"],256).length);
n=addSize(n,Ros1Std_msgsDurationSize(value["mcu_uptime"]));
n=addSize(n,Ros1Std_msgsDurationSize(value["connection_uptime"]));
if(typeof value["pcb_temperature"]!=="number"||!Number.isFinite(value["pcb_temperature"])||!Number.isFinite(Math.fround(value["pcb_temperature"])))bad();n=addSize(n,4);
if(typeof value["mcu_temperature"]!=="number"||!Number.isFinite(value["mcu_temperature"])||!Number.isFinite(Math.fround(value["mcu_temperature"])))bad();n=addSize(n,4);
if(n>688)bad(90);return n;}
function Ros1Clearpath_platform_msgsStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["hardware_id"],256);
w.text(value["firmware_commit"],256);
Ros1Std_msgsDurationWrite(value["mcu_uptime"],w);
Ros1Std_msgsDurationWrite(value["connection_uptime"],w);
w.v.setFloat32(w.at,value["pcb_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["mcu_temperature"],true);w.at+=4;
}
function Ros1Clearpath_platform_msgsStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["hardware_id"]=decoded;
}
{
const decoded=r.text(256);
value["firmware_commit"]=decoded;
}
{
const decoded=Ros1Std_msgsDurationRead(r);
value["mcu_uptime"]=decoded;
}
{
const decoded=Ros1Std_msgsDurationRead(r);
value["connection_uptime"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pcb_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mcu_temperature"]=decoded;
}
return value;}
function Ros1Clearpath_platform_msgsStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["hardware_id"];
out["hardware_id"]=field;
}
{const field=value["firmware_commit"];
out["firmware_commit"]=field;
}
{const field=value["mcu_uptime"];
out["mcu_uptime"]=Ros1Std_msgsDurationCapture(field);
}
{const field=value["connection_uptime"];
out["connection_uptime"]=Ros1Std_msgsDurationCapture(field);
}
{const field=value["pcb_temperature"];
out["pcb_temperature"]=field;
}
{const field=value["mcu_temperature"];
out["mcu_temperature"]=field;
}
return out;}
/** Complete ROS1 clearpath_platform_msgs/StopStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_platform_msgsStopStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_e16505686f22cd4183a6eb89e53c3b4184ccd58a15b1bc46efa656d50b0b3a18",maxWireBytes:146,minWireBytes:18,cartesianMaxWireBytes:146n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"stop_power_status":false,"external_stop_present":false};},
validateValue(value){Ros1Clearpath_platform_msgsStopStatusSize(Ros1Clearpath_platform_msgsStopStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_platform_msgsStopStatusCapture(value),n=Ros1Clearpath_platform_msgsStopStatusSize(owned),w=new Writer(n);Ros1Clearpath_platform_msgsStopStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,146,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros1Clearpath_platform_msgsStopStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_platform_msgsStopStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["stop_power_status"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["external_stop_present"]!=="boolean")bad();n=addSize(n,1);
if(n>146)bad(90);return n;}
function Ros1Clearpath_platform_msgsStopStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["stop_power_status"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["external_stop_present"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_platform_msgsStopStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["stop_power_status"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["external_stop_present"]=decoded!==0;
}
return value;}
function Ros1Clearpath_platform_msgsStopStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["stop_power_status"];
out["stop_power_status"]=field;
}
{const field=value["external_stop_present"];
out["external_stop_present"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AssistedTeleopState codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAssistedTeleopStateCodec=Object.freeze({codecIdentity:"UWVMROS1_e502d2181d51f1de7976b35a95ea2463644d374becce390d668a81e549fad82e",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({"STATE_ACTIVE":0,"STATE_DISABLED":1,"STATE_COLLISION_IMMINENT":2,"STATE_STOPPED":3,"STATE_COLLISION":4,"STATE_ERROR":5,"ASSIST_ON":100,"ASSIST_OFF_SERVICE":101,"ASSIST_OFF_JOYSTICK_OVERRIDE":102,"ASSIST_OFF_SENSOR_TIMEOUT":103,"ASSIST_OFF_REMOTE_CONTROLLER_OVERRIDE":104}),
defaultValue(){return {"state":0,"assist_off_trigger":0,"error_message":""};},
validateValue(value){Ros1Clearpath_safety_msgsAssistedTeleopStateSize(Ros1Clearpath_safety_msgsAssistedTeleopStateCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAssistedTeleopStateCapture(value),n=Ros1Clearpath_safety_msgsAssistedTeleopStateSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAssistedTeleopStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAssistedTeleopStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAssistedTeleopStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["state"],1,false);n=addSize(n,1);
integer(value["assist_off_trigger"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["error_message"],256).length);
if(n>262)bad(90);return n;}
function Ros1Clearpath_safety_msgsAssistedTeleopStateWrite(value,w){
w.v.setUint8(w.at,value["state"],true);w.at+=1;
w.v.setUint8(w.at,value["assist_off_trigger"],true);w.at+=1;
w.text(value["error_message"],256);
}
function Ros1Clearpath_safety_msgsAssistedTeleopStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["assist_off_trigger"]=decoded;
}
{
const decoded=r.text(256);
value["error_message"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAssistedTeleopStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["assist_off_trigger"];
out["assist_off_trigger"]=field;
}
{const field=value["error_message"];
out["error_message"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/ObstacleMap codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsObstacleMapCodec=Object.freeze({codecIdentity:"UWVMROS1_e137bca7b7ac8e14451792df0b9347910c5ff236087eb0f5db872c5a0f3e0f93",maxWireBytes:12,minWireBytes:4,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"threat_level":[]};},
validateValue(value){Ros1Clearpath_safety_msgsObstacleMapSize(Ros1Clearpath_safety_msgsObstacleMapCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsObstacleMapCapture(value),n=Ros1Clearpath_safety_msgsObstacleMapSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsObstacleMapWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsObstacleMapRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsObstacleMapSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["threat_level"])||value["threat_level"].length>2)bad();
n=addSize(n,4);
for(const element of value["threat_level"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>12)bad(90);return n;}
function Ros1Clearpath_safety_msgsObstacleMapWrite(value,w){
w.v.setUint32(w.at,value["threat_level"].length,true);w.at+=4;
for(const element of value["threat_level"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Clearpath_safety_msgsObstacleMapRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["threat_level"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["threat_level"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_safety_msgsObstacleMapCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["threat_level"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["threat_level"]=values;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/Safety codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsSafetyCodec=Object.freeze({codecIdentity:"UWVMROS1_033ee9ca48e803538ac663cdd476b65fdc5e29359be86b8ea6251cfaaa85e9ce",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({"SAFETY_ESTOP":0,"SAFETY_SENSOR_TIMEOUT":1}),
defaultValue(){return {"safety_monitor_triggered":false,"trigger_type":0,"message":""};},
validateValue(value){Ros1Clearpath_safety_msgsSafetySize(Ros1Clearpath_safety_msgsSafetyCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsSafetyCapture(value),n=Ros1Clearpath_safety_msgsSafetySize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsSafetyWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsSafetyRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsSafetySize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["safety_monitor_triggered"]!=="boolean")bad();n=addSize(n,1);
integer(value["trigger_type"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>262)bad(90);return n;}
function Ros1Clearpath_safety_msgsSafetyWrite(value,w){
w.v.setUint8(w.at,value["safety_monitor_triggered"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["trigger_type"],true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_safety_msgsSafetyRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["safety_monitor_triggered"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["trigger_type"]=decoded;
}
{
const decoded=r.text(256);
value["message"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsSafetyCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["safety_monitor_triggered"];
out["safety_monitor_triggered"]=field;
}
{const field=value["trigger_type"];
out["trigger_type"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigCommon codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec=Object.freeze({codecIdentity:"UWVMROS1_508d2bd15b0b42a056f3ef82fd04799bd2b6d8d4d6d03dc571e151f5c30c8844",maxWireBytes:1177,minWireBytes:17,cartesianMaxWireBytes:1177n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","type":"","enabled":false,"components":[],"on_trigger_action":""};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1177,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["type"],256).length);
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(!Array.isArray(value["components"])||value["components"].length>2)bad();
n=addSize(n,4);
for(const element of value["components"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["on_trigger_action"],256).length);
if(n>1177)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value,w){
w.text(value["name"],128);
w.text(value["type"],256);
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["components"].length,true);w.at+=4;
for(const element of value["components"]){
w.text(element,256);
}
w.text(value["on_trigger_action"],256);
}
function Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
{
{const count=r.u32();if(count>2)bad();value["components"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["components"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["on_trigger_action"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["type"];
out["type"]=field;
}
{const field=value["enabled"];
out["enabled"]=field;
}
{const field=value["components"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["components"]=values;
}
{const field=value["on_trigger_action"];
out["on_trigger_action"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigCommunication codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCodec=Object.freeze({codecIdentity:"UWVMROS1_be40239142afe7ffedef0d13cc13f093e1c6c5f49b73074cfa5c9ccaa0106446",maxWireBytes:1961,minWireBytes:33,cartesianMaxWireBytes:1961n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"protocol":"","ip":"","interface":"","timeout":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigCommunicationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1961,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["protocol"],256).length);
n=addSize(n,4+textBytes(value["ip"],256).length);
n=addSize(n,4+textBytes(value["interface"],256).length);
if(typeof value["timeout"]!=="number"||!Number.isFinite(value["timeout"])||!Number.isFinite(Math.fround(value["timeout"])))bad();n=addSize(n,4);
if(n>1961)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigCommunicationWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["protocol"],256);
w.text(value["ip"],256);
w.text(value["interface"],256);
w.v.setFloat32(w.at,value["timeout"],true);w.at+=4;
}
function Ros1Clearpath_safety_msgsWatchdogConfigCommunicationRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["protocol"]=decoded;
}
{
const decoded=r.text(256);
value["ip"]=decoded;
}
{
const decoded=r.text(256);
value["interface"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["timeout"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["protocol"];
out["protocol"]=field;
}
{const field=value["ip"];
out["ip"]=field;
}
{const field=value["interface"];
out["interface"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigHeartbeat codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCodec=Object.freeze({codecIdentity:"UWVMROS1_66f859db1405ce185c91bd0e04b20901c12cb4e66deb3c2fff3a7085647a2cc2",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"topic":"","timeout":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["topic"],256).length);
if(typeof value["timeout"]!=="number"||!Number.isFinite(value["timeout"])||!Number.isFinite(Math.fround(value["timeout"])))bad();n=addSize(n,4);
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["topic"],256);
w.v.setFloat32(w.at,value["timeout"],true);w.at+=4;
}
function Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["topic"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["timeout"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["topic"];
out["topic"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigInclination codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigInclinationCodec=Object.freeze({codecIdentity:"UWVMROS1_55022d7e5888b670bce29de4cc9c54da784e26b21d8843770de5a9db75f78d77",maxWireBytes:1449,minWireBytes:33,cartesianMaxWireBytes:1449n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"roll_limit":0,"pitch_limit":0,"delay":0,"topic":""};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigInclinationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1449,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigInclinationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
if(typeof value["roll_limit"]!=="number"||!Number.isFinite(value["roll_limit"])||!Number.isFinite(Math.fround(value["roll_limit"])))bad();n=addSize(n,4);
if(typeof value["pitch_limit"]!=="number"||!Number.isFinite(value["pitch_limit"])||!Number.isFinite(Math.fround(value["pitch_limit"])))bad();n=addSize(n,4);
if(typeof value["delay"]!=="number"||!Number.isFinite(value["delay"])||!Number.isFinite(Math.fround(value["delay"])))bad();n=addSize(n,4);
n=addSize(n,4+textBytes(value["topic"],256).length);
if(n>1449)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigInclinationWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.v.setFloat32(w.at,value["roll_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["pitch_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["delay"],true);w.at+=4;
w.text(value["topic"],256);
}
function Ros1Clearpath_safety_msgsWatchdogConfigInclinationRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["roll_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pitch_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["delay"]=decoded;
}
{
const decoded=r.text(256);
value["topic"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["roll_limit"];
out["roll_limit"]=field;
}
{const field=value["pitch_limit"];
out["pitch_limit"]=field;
}
{const field=value["delay"];
out["delay"]=field;
}
{const field=value["topic"];
out["topic"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigNodeStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_b8efb3fe844ee27f21c9f77dcb71be6e818931c2d927b163c70fd988c371f0f9",maxWireBytes:1438,minWireBytes:22,cartesianMaxWireBytes:1438n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"node_name":"","num_requests":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1438,90);if(owned.length<22)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["node_name"],256).length);
integer(value["num_requests"],1,false);n=addSize(n,1);
if(n>1438)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["node_name"],256);
w.v.setUint8(w.at,value["num_requests"],true);w.at+=1;
}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["node_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["num_requests"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["node_name"];
out["node_name"]=field;
}
{const field=value["num_requests"];
out["num_requests"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigNodeletStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_170ba32f5c32e65fd02acf9aee6715014a6e3ed471fc416f1181e2759182ba5f",maxWireBytes:1698,minWireBytes:26,cartesianMaxWireBytes:1698n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"manager":"","nodelet":"","num_requests":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1698,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["manager"],256).length);
n=addSize(n,4+textBytes(value["nodelet"],256).length);
integer(value["num_requests"],1,false);n=addSize(n,1);
if(n>1698)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["manager"],256);
w.text(value["nodelet"],256);
w.v.setUint8(w.at,value["num_requests"],true);w.at+=1;
}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["manager"]=decoded;
}
{
const decoded=r.text(256);
value["nodelet"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["num_requests"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["manager"];
out["manager"]=field;
}
{const field=value["nodelet"];
out["nodelet"]=field;
}
{const field=value["num_requests"];
out["num_requests"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigNumPoints codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCodec=Object.freeze({codecIdentity:"UWVMROS1_ea9a231a5a1f6471b20965acd6b2ec11ece3d76dd510649ac2e93b33c5235b85",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"topic":"","threshold":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigNumPointsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["topic"],256).length);
integer(value["threshold"],4,true);n=addSize(n,4);
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigNumPointsWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["topic"],256);
w.v.setInt32(w.at,value["threshold"],true);w.at+=4;
}
function Ros1Clearpath_safety_msgsWatchdogConfigNumPointsRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["topic"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["threshold"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["topic"];
out["topic"]=field;
}
{const field=value["threshold"];
out["threshold"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigOdomCovariance codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCodec=Object.freeze({codecIdentity:"UWVMROS1_16d77772c5fed2351e0d79f6ab3d2e80bd2ac3c8d96fa16234d6148baa66c813",maxWireBytes:1449,minWireBytes:33,cartesianMaxWireBytes:1449n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"topic":"","xy_threshold":0,"yaw_threshold":0,"timeout":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1449,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["topic"],256).length);
if(typeof value["xy_threshold"]!=="number"||!Number.isFinite(value["xy_threshold"])||!Number.isFinite(Math.fround(value["xy_threshold"])))bad();n=addSize(n,4);
if(typeof value["yaw_threshold"]!=="number"||!Number.isFinite(value["yaw_threshold"])||!Number.isFinite(Math.fround(value["yaw_threshold"])))bad();n=addSize(n,4);
if(typeof value["timeout"]!=="number"||!Number.isFinite(value["timeout"])||!Number.isFinite(Math.fround(value["timeout"])))bad();n=addSize(n,4);
if(n>1449)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["topic"],256);
w.v.setFloat32(w.at,value["xy_threshold"],true);w.at+=4;
w.v.setFloat32(w.at,value["yaw_threshold"],true);w.at+=4;
w.v.setFloat32(w.at,value["timeout"],true);w.at+=4;
}
function Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["topic"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["xy_threshold"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["yaw_threshold"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["timeout"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["topic"];
out["topic"]=field;
}
{const field=value["xy_threshold"];
out["xy_threshold"]=field;
}
{const field=value["yaw_threshold"];
out["yaw_threshold"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigRtkFix codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCodec=Object.freeze({codecIdentity:"UWVMROS1_c73d2db18765570053184bef37ec0519c43d8265f73dae48a18e209577190bc2",maxWireBytes:1443,minWireBytes:27,cartesianMaxWireBytes:1443n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"rtk_heading_required":false,"rtk_position_required":false,"timeout":0,"gnss_type":""};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigRtkFixWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1443,90);if(owned.length<27)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
if(typeof value["rtk_heading_required"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rtk_position_required"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["timeout"]!=="number"||!Number.isFinite(value["timeout"])||!Number.isFinite(Math.fround(value["timeout"])))bad();n=addSize(n,4);
n=addSize(n,4+textBytes(value["gnss_type"],256).length);
if(n>1443)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigRtkFixWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.v.setUint8(w.at,value["rtk_heading_required"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rtk_position_required"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["timeout"],true);w.at+=4;
w.text(value["gnss_type"],256);
}
function Ros1Clearpath_safety_msgsWatchdogConfigRtkFixRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rtk_heading_required"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rtk_position_required"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["timeout"]=decoded;
}
{
const decoded=r.text(256);
value["gnss_type"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["rtk_heading_required"];
out["rtk_heading_required"]=field;
}
{const field=value["rtk_position_required"];
out["rtk_position_required"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
{const field=value["gnss_type"];
out["gnss_type"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigTopicData codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCodec=Object.freeze({codecIdentity:"UWVMROS1_1bf0a230aef86f09014852b1be0cf0d5998d4e8a69e4630a9e387b10b470ad8f",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"base_config":Ros1Clearpath_safety_msgsWatchdogConfigCommonCodec.defaultValue(),"topic":"","timeout":0};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigTopicDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommonSize(value["base_config"]));
n=addSize(n,4+textBytes(value["topic"],256).length);
if(typeof value["timeout"]!=="number"||!Number.isFinite(value["timeout"])||!Number.isFinite(Math.fround(value["timeout"])))bad();n=addSize(n,4);
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigTopicDataWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommonWrite(value["base_config"],w);
w.text(value["topic"],256);
w.v.setFloat32(w.at,value["timeout"],true);w.at+=4;
}
function Ros1Clearpath_safety_msgsWatchdogConfigTopicDataRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommonRead(r);
value["base_config"]=decoded;
}
{
const decoded=r.text(256);
value["topic"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["timeout"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["base_config"];
out["base_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommonCapture(field);
}
{const field=value["topic"];
out["topic"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogConfigArray codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogConfigArrayCodec=Object.freeze({codecIdentity:"UWVMROS1_5395da95a24a49234ff91d9149558ae13d621524f233b286744dcc28e6a558d3",maxWireBytes:27558,minWireBytes:36,cartesianMaxWireBytes:27558n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"communication_config":[],"heartbeat_config":[],"inclination_config":[],"node_status_config":[],"nodelet_status_config":[],"num_points_config":[],"odom_covariance_config":[],"rtk_fix_config":[],"topic_data_config":[]};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogConfigArraySize(Ros1Clearpath_safety_msgsWatchdogConfigArrayCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogConfigArrayCapture(value),n=Ros1Clearpath_safety_msgsWatchdogConfigArraySize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogConfigArrayWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,27558,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogConfigArrayRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogConfigArraySize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["communication_config"])||value["communication_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["communication_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(element));
}
if(!Array.isArray(value["heartbeat_config"])||value["heartbeat_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["heartbeat_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(element));
}
if(!Array.isArray(value["inclination_config"])||value["inclination_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["inclination_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(element));
}
if(!Array.isArray(value["node_status_config"])||value["node_status_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["node_status_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(element));
}
if(!Array.isArray(value["nodelet_status_config"])||value["nodelet_status_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["nodelet_status_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(element));
}
if(!Array.isArray(value["num_points_config"])||value["num_points_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["num_points_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(element));
}
if(!Array.isArray(value["odom_covariance_config"])||value["odom_covariance_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["odom_covariance_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(element));
}
if(!Array.isArray(value["rtk_fix_config"])||value["rtk_fix_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["rtk_fix_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(element));
}
if(!Array.isArray(value["topic_data_config"])||value["topic_data_config"].length>2)bad();
n=addSize(n,4);
for(const element of value["topic_data_config"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(element));
}
if(n>27558)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogConfigArrayWrite(value,w){
w.v.setUint32(w.at,value["communication_config"].length,true);w.at+=4;
for(const element of value["communication_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigCommunicationWrite(element,w);
}
w.v.setUint32(w.at,value["heartbeat_config"].length,true);w.at+=4;
for(const element of value["heartbeat_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatWrite(element,w);
}
w.v.setUint32(w.at,value["inclination_config"].length,true);w.at+=4;
for(const element of value["inclination_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigInclinationWrite(element,w);
}
w.v.setUint32(w.at,value["node_status_config"].length,true);w.at+=4;
for(const element of value["node_status_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusWrite(element,w);
}
w.v.setUint32(w.at,value["nodelet_status_config"].length,true);w.at+=4;
for(const element of value["nodelet_status_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusWrite(element,w);
}
w.v.setUint32(w.at,value["num_points_config"].length,true);w.at+=4;
for(const element of value["num_points_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigNumPointsWrite(element,w);
}
w.v.setUint32(w.at,value["odom_covariance_config"].length,true);w.at+=4;
for(const element of value["odom_covariance_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceWrite(element,w);
}
w.v.setUint32(w.at,value["rtk_fix_config"].length,true);w.at+=4;
for(const element of value["rtk_fix_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigRtkFixWrite(element,w);
}
w.v.setUint32(w.at,value["topic_data_config"].length,true);w.at+=4;
for(const element of value["topic_data_config"]){
Ros1Clearpath_safety_msgsWatchdogConfigTopicDataWrite(element,w);
}
}
function Ros1Clearpath_safety_msgsWatchdogConfigArrayRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["communication_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationRead(r);
value["communication_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["heartbeat_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatRead(r);
value["heartbeat_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["inclination_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigInclinationRead(r);
value["inclination_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["node_status_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusRead(r);
value["node_status_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["nodelet_status_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusRead(r);
value["nodelet_status_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["num_points_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsRead(r);
value["num_points_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["odom_covariance_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceRead(r);
value["odom_covariance_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["rtk_fix_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixRead(r);
value["rtk_fix_config"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["topic_data_config"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataRead(r);
value["topic_data_config"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogConfigArrayCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["communication_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(element);
}out["communication_config"]=values;
}
{const field=value["heartbeat_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(element);
}out["heartbeat_config"]=values;
}
{const field=value["inclination_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(element);
}out["inclination_config"]=values;
}
{const field=value["node_status_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(element);
}out["node_status_config"]=values;
}
{const field=value["nodelet_status_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(element);
}out["nodelet_status_config"]=values;
}
{const field=value["num_points_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(element);
}out["num_points_config"]=values;
}
{const field=value["odom_covariance_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(element);
}out["odom_covariance_config"]=values;
}
{const field=value["rtk_fix_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(element);
}out["rtk_fix_config"]=values;
}
{const field=value["topic_data_config"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(element);
}out["topic_data_config"]=values;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogStatus codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_823af4aa5da6787f2014faecb325f5c78d28f593409fc73f3237eb43ed01b5cc",maxWireBytes:653,minWireBytes:13,cartesianMaxWireBytes:653n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","triggered":false,"trigger_message":"","action":""};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogStatusSize(Ros1Clearpath_safety_msgsWatchdogStatusCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogStatusCapture(value),n=Ros1Clearpath_safety_msgsWatchdogStatusSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,653,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["triggered"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["trigger_message"],256).length);
n=addSize(n,4+textBytes(value["action"],256).length);
if(n>653)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogStatusWrite(value,w){
w.text(value["name"],128);
w.v.setUint8(w.at,value["triggered"]?1:0,true);w.at+=1;
w.text(value["trigger_message"],256);
w.text(value["action"],256);
}
function Ros1Clearpath_safety_msgsWatchdogStatusRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["triggered"]=decoded!==0;
}
{
const decoded=r.text(256);
value["trigger_message"]=decoded;
}
{
const decoded=r.text(256);
value["action"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["triggered"];
out["triggered"]=field;
}
{const field=value["trigger_message"];
out["trigger_message"]=field;
}
{const field=value["action"];
out["action"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/WatchdogStatusArray codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsWatchdogStatusArrayCodec=Object.freeze({codecIdentity:"UWVMROS1_744df181cbf598672b82290fc5bac35af5fb6c4e62b55e5afed820a7cf270f96",maxWireBytes:1454,minWireBytes:20,cartesianMaxWireBytes:1454n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"statuses":[]};},
validateValue(value){Ros1Clearpath_safety_msgsWatchdogStatusArraySize(Ros1Clearpath_safety_msgsWatchdogStatusArrayCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsWatchdogStatusArrayCapture(value),n=Ros1Clearpath_safety_msgsWatchdogStatusArraySize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsWatchdogStatusArrayWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1454,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsWatchdogStatusArrayRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsWatchdogStatusArraySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["statuses"])||value["statuses"].length>2)bad();
n=addSize(n,4);
for(const element of value["statuses"]){
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogStatusSize(element));
}
if(n>1454)bad(90);return n;}
function Ros1Clearpath_safety_msgsWatchdogStatusArrayWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["statuses"].length,true);w.at+=4;
for(const element of value["statuses"]){
Ros1Clearpath_safety_msgsWatchdogStatusWrite(element,w);
}
}
function Ros1Clearpath_safety_msgsWatchdogStatusArrayRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["statuses"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_safety_msgsWatchdogStatusRead(r);
value["statuses"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_safety_msgsWatchdogStatusArrayCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["statuses"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_safety_msgsWatchdogStatusCapture(element);
}out["statuses"]=values;
}
return out;}
/** Complete ROS1 husky_msgs/HuskyStatus codec; maxima are not actual lengths. */
export const Ros1Husky_msgsHuskyStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_4aaed6c015314bd743406086571c12760cb41e446d5248d26537e6fc22c40a86",maxWireBytes:252,minWireBytes:124,cartesianMaxWireBytes:252n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"uptime":0,"ros_control_loop_freq":0,"mcu_and_user_port_current":0,"left_driver_current":0,"right_driver_current":0,"battery_voltage":0,"left_driver_voltage":0,"right_driver_voltage":0,"left_driver_temp":0,"right_driver_temp":0,"left_motor_temp":0,"right_motor_temp":0,"capacity_estimate":0,"charge_estimate":0,"timeout":false,"lockout":false,"e_stop":false,"ros_pause":false,"no_battery":false,"current_limit":false};},
validateValue(value){Ros1Husky_msgsHuskyStatusSize(Ros1Husky_msgsHuskyStatusCapture(value));return true;},
encode(value){const owned=Ros1Husky_msgsHuskyStatusCapture(value),n=Ros1Husky_msgsHuskyStatusSize(owned),w=new Writer(n);Ros1Husky_msgsHuskyStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,252,90);if(owned.length<124)bad();const r=new Reader(owned),value=Ros1Husky_msgsHuskyStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Husky_msgsHuskyStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
integer(value["uptime"],4,false);n=addSize(n,4);
if(typeof value["ros_control_loop_freq"]!=="number"||!Number.isFinite(value["ros_control_loop_freq"]))bad();n=addSize(n,8);
if(typeof value["mcu_and_user_port_current"]!=="number"||!Number.isFinite(value["mcu_and_user_port_current"]))bad();n=addSize(n,8);
if(typeof value["left_driver_current"]!=="number"||!Number.isFinite(value["left_driver_current"]))bad();n=addSize(n,8);
if(typeof value["right_driver_current"]!=="number"||!Number.isFinite(value["right_driver_current"]))bad();n=addSize(n,8);
if(typeof value["battery_voltage"]!=="number"||!Number.isFinite(value["battery_voltage"]))bad();n=addSize(n,8);
if(typeof value["left_driver_voltage"]!=="number"||!Number.isFinite(value["left_driver_voltage"]))bad();n=addSize(n,8);
if(typeof value["right_driver_voltage"]!=="number"||!Number.isFinite(value["right_driver_voltage"]))bad();n=addSize(n,8);
if(typeof value["left_driver_temp"]!=="number"||!Number.isFinite(value["left_driver_temp"]))bad();n=addSize(n,8);
if(typeof value["right_driver_temp"]!=="number"||!Number.isFinite(value["right_driver_temp"]))bad();n=addSize(n,8);
if(typeof value["left_motor_temp"]!=="number"||!Number.isFinite(value["left_motor_temp"]))bad();n=addSize(n,8);
if(typeof value["right_motor_temp"]!=="number"||!Number.isFinite(value["right_motor_temp"]))bad();n=addSize(n,8);
integer(value["capacity_estimate"],2,false);n=addSize(n,2);
if(typeof value["charge_estimate"]!=="number"||!Number.isFinite(value["charge_estimate"]))bad();n=addSize(n,8);
if(typeof value["timeout"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["lockout"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["e_stop"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["ros_pause"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["no_battery"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["current_limit"]!=="boolean")bad();n=addSize(n,1);
if(n>252)bad(90);return n;}
function Ros1Husky_msgsHuskyStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["uptime"],true);w.at+=4;
w.v.setFloat64(w.at,value["ros_control_loop_freq"],true);w.at+=8;
w.v.setFloat64(w.at,value["mcu_and_user_port_current"],true);w.at+=8;
w.v.setFloat64(w.at,value["left_driver_current"],true);w.at+=8;
w.v.setFloat64(w.at,value["right_driver_current"],true);w.at+=8;
w.v.setFloat64(w.at,value["battery_voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["left_driver_voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["right_driver_voltage"],true);w.at+=8;
w.v.setFloat64(w.at,value["left_driver_temp"],true);w.at+=8;
w.v.setFloat64(w.at,value["right_driver_temp"],true);w.at+=8;
w.v.setFloat64(w.at,value["left_motor_temp"],true);w.at+=8;
w.v.setFloat64(w.at,value["right_motor_temp"],true);w.at+=8;
w.v.setUint16(w.at,value["capacity_estimate"],true);w.at+=2;
w.v.setFloat64(w.at,value["charge_estimate"],true);w.at+=8;
w.v.setUint8(w.at,value["timeout"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["lockout"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["e_stop"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["ros_pause"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["no_battery"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["current_limit"]?1:0,true);w.at+=1;
}
function Ros1Husky_msgsHuskyStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["uptime"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ros_control_loop_freq"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["mcu_and_user_port_current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["left_driver_current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["right_driver_current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["battery_voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["left_driver_voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["right_driver_voltage"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["left_driver_temp"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["right_driver_temp"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["left_motor_temp"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["right_motor_temp"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["capacity_estimate"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["charge_estimate"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["timeout"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["lockout"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["e_stop"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ros_pause"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["no_battery"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["current_limit"]=decoded!==0;
}
return value;}
function Ros1Husky_msgsHuskyStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["uptime"];
out["uptime"]=field;
}
{const field=value["ros_control_loop_freq"];
out["ros_control_loop_freq"]=field;
}
{const field=value["mcu_and_user_port_current"];
out["mcu_and_user_port_current"]=field;
}
{const field=value["left_driver_current"];
out["left_driver_current"]=field;
}
{const field=value["right_driver_current"];
out["right_driver_current"]=field;
}
{const field=value["battery_voltage"];
out["battery_voltage"]=field;
}
{const field=value["left_driver_voltage"];
out["left_driver_voltage"]=field;
}
{const field=value["right_driver_voltage"];
out["right_driver_voltage"]=field;
}
{const field=value["left_driver_temp"];
out["left_driver_temp"]=field;
}
{const field=value["right_driver_temp"];
out["right_driver_temp"]=field;
}
{const field=value["left_motor_temp"];
out["left_motor_temp"]=field;
}
{const field=value["right_motor_temp"];
out["right_motor_temp"]=field;
}
{const field=value["capacity_estimate"];
out["capacity_estimate"]=field;
}
{const field=value["charge_estimate"];
out["charge_estimate"]=field;
}
{const field=value["timeout"];
out["timeout"]=field;
}
{const field=value["lockout"];
out["lockout"]=field;
}
{const field=value["e_stop"];
out["e_stop"]=field;
}
{const field=value["ros_pause"];
out["ros_pause"]=field;
}
{const field=value["no_battery"];
out["no_battery"]=field;
}
{const field=value["current_limit"];
out["current_limit"]=field;
}
return out;}
/** Complete ROS1 jackal_msgs/Drive codec; maxima are not actual lengths. */
export const Ros1Jackal_msgsDriveCodec=Object.freeze({codecIdentity:"UWVMROS1_a1991d124f321c1aaa7e02647dfdd7114d5c389197d621ddc8213b240b3541e6",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({"MODE_VELOCITY":0,"MODE_PWM":1,"MODE_EFFORT":2,"MODE_NONE":-1,"LEFT":0,"RIGHT":1}),
defaultValue(){return {"mode":0,"drivers":Array.from({length:2},()=>(0))};},
validateValue(value){Ros1Jackal_msgsDriveSize(Ros1Jackal_msgsDriveCapture(value));return true;},
encode(value){const owned=Ros1Jackal_msgsDriveCapture(value),n=Ros1Jackal_msgsDriveSize(owned),w=new Writer(n);Ros1Jackal_msgsDriveWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Jackal_msgsDriveRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Jackal_msgsDriveSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>9)bad(90);return n;}
function Ros1Jackal_msgsDriveWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
for(const element of value["drivers"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Jackal_msgsDriveRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["mode"]=decoded;
}
{
{const count=2;if(count>2)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["drivers"].push(decoded);
}}
}
return value;}
function Ros1Jackal_msgsDriveCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["drivers"]=values;
}
return out;}
/** Complete ROS1 jackal_msgs/DriveFeedback codec; maxima are not actual lengths. */
export const Ros1Jackal_msgsDriveFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_491d1150e13f8a9985582d3d80ccaadfb11feb4686624beb32689b1ff4b096d1",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current":0,"duty_cycle":0,"bridge_temperature":0,"motor_temperature":0,"measured_velocity":0,"measured_travel":0,"driver_fault":false};},
validateValue(value){Ros1Jackal_msgsDriveFeedbackSize(Ros1Jackal_msgsDriveFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Jackal_msgsDriveFeedbackCapture(value),n=Ros1Jackal_msgsDriveFeedbackSize(owned),w=new Writer(n);Ros1Jackal_msgsDriveFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Jackal_msgsDriveFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Jackal_msgsDriveFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["duty_cycle"]!=="number"||!Number.isFinite(value["duty_cycle"])||!Number.isFinite(Math.fround(value["duty_cycle"])))bad();n=addSize(n,4);
if(typeof value["bridge_temperature"]!=="number"||!Number.isFinite(value["bridge_temperature"])||!Number.isFinite(Math.fround(value["bridge_temperature"])))bad();n=addSize(n,4);
if(typeof value["motor_temperature"]!=="number"||!Number.isFinite(value["motor_temperature"])||!Number.isFinite(Math.fround(value["motor_temperature"])))bad();n=addSize(n,4);
if(typeof value["measured_velocity"]!=="number"||!Number.isFinite(value["measured_velocity"])||!Number.isFinite(Math.fround(value["measured_velocity"])))bad();n=addSize(n,4);
if(typeof value["measured_travel"]!=="number"||!Number.isFinite(value["measured_travel"])||!Number.isFinite(Math.fround(value["measured_travel"])))bad();n=addSize(n,4);
if(typeof value["driver_fault"]!=="boolean")bad();n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Jackal_msgsDriveFeedbackWrite(value,w){
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat32(w.at,value["duty_cycle"],true);w.at+=4;
w.v.setFloat32(w.at,value["bridge_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["motor_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_velocity"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_travel"],true);w.at+=4;
w.v.setUint8(w.at,value["driver_fault"]?1:0,true);w.at+=1;
}
function Ros1Jackal_msgsDriveFeedbackRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["duty_cycle"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["bridge_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["motor_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_velocity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_travel"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["driver_fault"]=decoded!==0;
}
return value;}
function Ros1Jackal_msgsDriveFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current"];
out["current"]=field;
}
{const field=value["duty_cycle"];
out["duty_cycle"]=field;
}
{const field=value["bridge_temperature"];
out["bridge_temperature"]=field;
}
{const field=value["motor_temperature"];
out["motor_temperature"]=field;
}
{const field=value["measured_velocity"];
out["measured_velocity"]=field;
}
{const field=value["measured_travel"];
out["measured_travel"]=field;
}
{const field=value["driver_fault"];
out["driver_fault"]=field;
}
return out;}
/** Complete ROS1 jackal_msgs/Feedback codec; maxima are not actual lengths. */
export const Ros1Jackal_msgsFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_5d574303e5854771234ad6b215d5923df4319cb597acc70ff00a6c73c27ccb89",maxWireBytes:204,minWireBytes:76,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"drivers":Array.from({length:2},()=>(Ros1Jackal_msgsDriveFeedbackCodec.defaultValue())),"pcb_temperature":0,"mcu_temperature":0,"commanded_mode":0,"actual_mode":0};},
validateValue(value){Ros1Jackal_msgsFeedbackSize(Ros1Jackal_msgsFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Jackal_msgsFeedbackCapture(value),n=Ros1Jackal_msgsFeedbackSize(owned),w=new Writer(n);Ros1Jackal_msgsFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<76)bad();const r=new Reader(owned),value=Ros1Jackal_msgsFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Jackal_msgsFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
n=addSize(n,Ros1Jackal_msgsDriveFeedbackSize(element));
}
if(typeof value["pcb_temperature"]!=="number"||!Number.isFinite(value["pcb_temperature"])||!Number.isFinite(Math.fround(value["pcb_temperature"])))bad();n=addSize(n,4);
if(typeof value["mcu_temperature"]!=="number"||!Number.isFinite(value["mcu_temperature"])||!Number.isFinite(Math.fround(value["mcu_temperature"])))bad();n=addSize(n,4);
integer(value["commanded_mode"],1,true);n=addSize(n,1);
integer(value["actual_mode"],1,true);n=addSize(n,1);
if(n>204)bad(90);return n;}
function Ros1Jackal_msgsFeedbackWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
for(const element of value["drivers"]){
Ros1Jackal_msgsDriveFeedbackWrite(element,w);
}
w.v.setFloat32(w.at,value["pcb_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["mcu_temperature"],true);w.at+=4;
w.v.setInt8(w.at,value["commanded_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["actual_mode"],true);w.at+=1;
}
function Ros1Jackal_msgsFeedbackRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=2;if(count>2)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Jackal_msgsDriveFeedbackRead(r);
value["drivers"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pcb_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mcu_temperature"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["commanded_mode"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["actual_mode"]=decoded;
}
return value;}
function Ros1Jackal_msgsFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Jackal_msgsDriveFeedbackCapture(element);
}out["drivers"]=values;
}
{const field=value["pcb_temperature"];
out["pcb_temperature"]=field;
}
{const field=value["mcu_temperature"];
out["mcu_temperature"]=field;
}
{const field=value["commanded_mode"];
out["commanded_mode"]=field;
}
{const field=value["actual_mode"];
out["actual_mode"]=field;
}
return out;}
/** Complete ROS1 jackal_msgs/Status codec; maxima are not actual lengths. */
export const Ros1Jackal_msgsStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_e340e68eb138003c1a4f99aa0defbe651fe573681fda03dff451e1c5d73d48af",maxWireBytes:463,minWireBytes:79,cartesianMaxWireBytes:463n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"hardware_id":"","mcu_uptime":Ros1DurationCodec.defaultValue(),"connection_uptime":Ros1DurationCodec.defaultValue(),"drivers_active":false,"driver_external_stop_present":false,"driver_external_stop_stopped":false,"measured_battery":0,"measured_12v":0,"measured_5v":0,"drive_current":0,"user_current":0,"computer_current":0,"total_current":0,"total_current_peak":0,"total_power_consumed":0};},
validateValue(value){Ros1Jackal_msgsStatusSize(Ros1Jackal_msgsStatusCapture(value));return true;},
encode(value){const owned=Ros1Jackal_msgsStatusCapture(value),n=Ros1Jackal_msgsStatusSize(owned),w=new Writer(n);Ros1Jackal_msgsStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,463,90);if(owned.length<79)bad();const r=new Reader(owned),value=Ros1Jackal_msgsStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Jackal_msgsStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["hardware_id"],256).length);
n=addSize(n,Ros1DurationSize(value["mcu_uptime"]));
n=addSize(n,Ros1DurationSize(value["connection_uptime"]));
if(typeof value["drivers_active"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["driver_external_stop_present"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["driver_external_stop_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["measured_battery"]!=="number"||!Number.isFinite(value["measured_battery"])||!Number.isFinite(Math.fround(value["measured_battery"])))bad();n=addSize(n,4);
if(typeof value["measured_12v"]!=="number"||!Number.isFinite(value["measured_12v"])||!Number.isFinite(Math.fround(value["measured_12v"])))bad();n=addSize(n,4);
if(typeof value["measured_5v"]!=="number"||!Number.isFinite(value["measured_5v"])||!Number.isFinite(Math.fround(value["measured_5v"])))bad();n=addSize(n,4);
if(typeof value["drive_current"]!=="number"||!Number.isFinite(value["drive_current"])||!Number.isFinite(Math.fround(value["drive_current"])))bad();n=addSize(n,4);
if(typeof value["user_current"]!=="number"||!Number.isFinite(value["user_current"])||!Number.isFinite(Math.fround(value["user_current"])))bad();n=addSize(n,4);
if(typeof value["computer_current"]!=="number"||!Number.isFinite(value["computer_current"])||!Number.isFinite(Math.fround(value["computer_current"])))bad();n=addSize(n,4);
if(typeof value["total_current"]!=="number"||!Number.isFinite(value["total_current"])||!Number.isFinite(Math.fround(value["total_current"])))bad();n=addSize(n,4);
if(typeof value["total_current_peak"]!=="number"||!Number.isFinite(value["total_current_peak"])||!Number.isFinite(Math.fround(value["total_current_peak"])))bad();n=addSize(n,4);
if(typeof value["total_power_consumed"]!=="number"||!Number.isFinite(value["total_power_consumed"]))bad();n=addSize(n,8);
if(n>463)bad(90);return n;}
function Ros1Jackal_msgsStatusWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.text(value["hardware_id"],256);
Ros1DurationWrite(value["mcu_uptime"],w);
Ros1DurationWrite(value["connection_uptime"],w);
w.v.setUint8(w.at,value["drivers_active"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["driver_external_stop_present"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["driver_external_stop_stopped"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["measured_battery"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_12v"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_5v"],true);w.at+=4;
w.v.setFloat32(w.at,value["drive_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["user_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["computer_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["total_current"],true);w.at+=4;
w.v.setFloat32(w.at,value["total_current_peak"],true);w.at+=4;
w.v.setFloat64(w.at,value["total_power_consumed"],true);w.at+=8;
}
function Ros1Jackal_msgsStatusRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["hardware_id"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["mcu_uptime"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["connection_uptime"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["drivers_active"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["driver_external_stop_present"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["driver_external_stop_stopped"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_battery"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_12v"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["measured_5v"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["drive_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["user_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["computer_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["total_current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["total_current_peak"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["total_power_consumed"]=decoded;
}
return value;}
function Ros1Jackal_msgsStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["hardware_id"];
out["hardware_id"]=field;
}
{const field=value["mcu_uptime"];
out["mcu_uptime"]=Ros1DurationCapture(field);
}
{const field=value["connection_uptime"];
out["connection_uptime"]=Ros1DurationCapture(field);
}
{const field=value["drivers_active"];
out["drivers_active"]=field;
}
{const field=value["driver_external_stop_present"];
out["driver_external_stop_present"]=field;
}
{const field=value["driver_external_stop_stopped"];
out["driver_external_stop_stopped"]=field;
}
{const field=value["measured_battery"];
out["measured_battery"]=field;
}
{const field=value["measured_12v"];
out["measured_12v"]=field;
}
{const field=value["measured_5v"];
out["measured_5v"]=field;
}
{const field=value["drive_current"];
out["drive_current"]=field;
}
{const field=value["user_current"];
out["user_current"]=field;
}
{const field=value["computer_current"];
out["computer_current"]=field;
}
{const field=value["total_current"];
out["total_current"]=field;
}
{const field=value["total_current_peak"];
out["total_current_peak"]=field;
}
{const field=value["total_power_consumed"];
out["total_power_consumed"]=field;
}
return out;}
/** Complete ROS1 clearpath_camera_msgs/q62AreaZoomRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_camera_msgsQ62AreaZoomRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f1c8e9c75b7792b47733669fc00a61bbb2340e058450e72fcbaab9211b33362f",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"zoom_factor":0};},
validateValue(value){Ros1Clearpath_camera_msgsQ62AreaZoomRequestSize(Ros1Clearpath_camera_msgsQ62AreaZoomRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_camera_msgsQ62AreaZoomRequestCapture(value),n=Ros1Clearpath_camera_msgsQ62AreaZoomRequestSize(owned),w=new Writer(n);Ros1Clearpath_camera_msgsQ62AreaZoomRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_camera_msgsQ62AreaZoomRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_camera_msgsQ62AreaZoomRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["zoom_factor"]!=="number"||!Number.isFinite(value["zoom_factor"])||!Number.isFinite(Math.fround(value["zoom_factor"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros1Clearpath_camera_msgsQ62AreaZoomRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["zoom_factor"],true);w.at+=4;
}
function Ros1Clearpath_camera_msgsQ62AreaZoomRequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["zoom_factor"]=decoded;
}
return value;}
function Ros1Clearpath_camera_msgsQ62AreaZoomRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["zoom_factor"];
out["zoom_factor"]=field;
}
return out;}
/** Complete ROS1 clearpath_camera_msgs/q62AreaZoomResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_camera_msgsQ62AreaZoomResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_af1389573cb858990ad84dde13231fba6e758b46629c100bb8ae30fbf141ae3b",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_camera_msgsQ62AreaZoomResponseSize(Ros1Clearpath_camera_msgsQ62AreaZoomResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_camera_msgsQ62AreaZoomResponseCapture(value),n=Ros1Clearpath_camera_msgsQ62AreaZoomResponseSize(owned),w=new Writer(n);Ros1Clearpath_camera_msgsQ62AreaZoomResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_camera_msgsQ62AreaZoomResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_camera_msgsQ62AreaZoomResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_camera_msgsQ62AreaZoomResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_camera_msgsQ62AreaZoomResponseRead(r){const value={};
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
function Ros1Clearpath_camera_msgsQ62AreaZoomResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_control_msgs/SetControlModeRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsSetControlModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_80b29d151ddf41bb49d6deffc3b119e278ad7893d3a63f976f21f86ffc21a23f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":Ros1Clearpath_control_msgsControlModeCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_control_msgsSetControlModeRequestSize(Ros1Clearpath_control_msgsSetControlModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsSetControlModeRequestCapture(value),n=Ros1Clearpath_control_msgsSetControlModeRequestSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsSetControlModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsSetControlModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsSetControlModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_control_msgsControlModeSize(value["mode"]));
if(n>1)bad(90);return n;}
function Ros1Clearpath_control_msgsSetControlModeRequestWrite(value,w){
Ros1Clearpath_control_msgsControlModeWrite(value["mode"],w);
}
function Ros1Clearpath_control_msgsSetControlModeRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_control_msgsControlModeRead(r);
value["mode"]=decoded;
}
return value;}
function Ros1Clearpath_control_msgsSetControlModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=Ros1Clearpath_control_msgsControlModeCapture(field);
}
return out;}
/** Complete ROS1 clearpath_control_msgs/SetControlModeResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_control_msgsSetControlModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9aeb21dcef1fa6b6c12300e9c93cb7859090a5a96e58c9d2dd6ed00af3049734",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_control_msgsSetControlModeResponseSize(Ros1Clearpath_control_msgsSetControlModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_control_msgsSetControlModeResponseCapture(value),n=Ros1Clearpath_control_msgsSetControlModeResponseSize(owned),w=new Writer(n);Ros1Clearpath_control_msgsSetControlModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_control_msgsSetControlModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_control_msgsSetControlModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_control_msgsSetControlModeResponseWrite(value,w){
}
function Ros1Clearpath_control_msgsSetControlModeResponseRead(r){const value={};
return value;}
function Ros1Clearpath_control_msgsSetControlModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_dock_msgs/AddDockRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsAddDockRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d779d320a6f98a81388a91a74c0199608b11f794ded24f3863a42b38b9a2d340",maxWireBytes:1204,minWireBytes:52,cartesianMaxWireBytes:1204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dock":Ros1Clearpath_dock_msgsDockInfoCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_dock_msgsAddDockRequestSize(Ros1Clearpath_dock_msgsAddDockRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsAddDockRequestCapture(value),n=Ros1Clearpath_dock_msgsAddDockRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsAddDockRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1204,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsAddDockRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsAddDockRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_dock_msgsDockInfoSize(value["dock"]));
if(n>1204)bad(90);return n;}
function Ros1Clearpath_dock_msgsAddDockRequestWrite(value,w){
Ros1Clearpath_dock_msgsDockInfoWrite(value["dock"],w);
}
function Ros1Clearpath_dock_msgsAddDockRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_dock_msgsDockInfoRead(r);
value["dock"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsAddDockRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dock"];
out["dock"]=Ros1Clearpath_dock_msgsDockInfoCapture(field);
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/AddDockResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsAddDockResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ba1e0ca5cef0dbfbc42d0a22fc241f044fd90ad8d5fff3f8b94be57dae26d08f",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsAddDockResponseSize(Ros1Clearpath_dock_msgsAddDockResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsAddDockResponseCapture(value),n=Ros1Clearpath_dock_msgsAddDockResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsAddDockResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsAddDockResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsAddDockResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsAddDockResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsAddDockResponseRead(r){const value={};
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
function Ros1Clearpath_dock_msgsAddDockResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/ExportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsExportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5921228fb0be54a809ad2ca87d1a56293cd455f080d90800880d147617044827",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_dock_msgsExportDataRequestSize(Ros1Clearpath_dock_msgsExportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsExportDataRequestCapture(value),n=Ros1Clearpath_dock_msgsExportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsExportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsExportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsExportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_dock_msgsExportDataRequestWrite(value,w){
}
function Ros1Clearpath_dock_msgsExportDataRequestRead(r){const value={};
return value;}
function Ros1Clearpath_dock_msgsExportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_dock_msgs/ExportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsExportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6fc247ff50ba6c24159cd8f308df22b541e86ebc56f8aa27207d5fcdfaf9cb2c",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_dock_msgsExportDataResponseSize(Ros1Clearpath_dock_msgsExportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsExportDataResponseCapture(value),n=Ros1Clearpath_dock_msgsExportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsExportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsExportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsExportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsExportDataResponseWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_dock_msgsExportDataResponseRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsExportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/GetDockByUuidRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsGetDockByUuidRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a54abd75f8db68630e6964a3f5f7ea520f5ef37103e2ba07c44ba85f519b339c",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dock_uuid":""};},
validateValue(value){Ros1Clearpath_dock_msgsGetDockByUuidRequestSize(Ros1Clearpath_dock_msgsGetDockByUuidRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsGetDockByUuidRequestCapture(value),n=Ros1Clearpath_dock_msgsGetDockByUuidRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsGetDockByUuidRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsGetDockByUuidRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsGetDockByUuidRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["dock_uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsGetDockByUuidRequestWrite(value,w){
w.text(value["dock_uuid"],256);
}
function Ros1Clearpath_dock_msgsGetDockByUuidRequestRead(r){const value={};
{
const decoded=r.text(256);
value["dock_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsGetDockByUuidRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dock_uuid"];
out["dock_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/GetDockByUuidResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsGetDockByUuidResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e574e53eca717e5855217d0829d238a7604133fa944db368b39a2660acdeef54",maxWireBytes:1316,minWireBytes:164,cartesianMaxWireBytes:1316n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"dock_pose":Ros1Geometry_msgsPoseCodec.defaultValue(),"predock_pose":Ros1Geometry_msgsPoseCodec.defaultValue(),"info":Ros1Clearpath_dock_msgsDockInfoCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_dock_msgsGetDockByUuidResponseSize(Ros1Clearpath_dock_msgsGetDockByUuidResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsGetDockByUuidResponseCapture(value),n=Ros1Clearpath_dock_msgsGetDockByUuidResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsGetDockByUuidResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1316,90);if(owned.length<164)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsGetDockByUuidResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsGetDockByUuidResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPoseSize(value["dock_pose"]));
n=addSize(n,Ros1Geometry_msgsPoseSize(value["predock_pose"]));
n=addSize(n,Ros1Clearpath_dock_msgsDockInfoSize(value["info"]));
if(n>1316)bad(90);return n;}
function Ros1Clearpath_dock_msgsGetDockByUuidResponseWrite(value,w){
Ros1Geometry_msgsPoseWrite(value["dock_pose"],w);
Ros1Geometry_msgsPoseWrite(value["predock_pose"],w);
Ros1Clearpath_dock_msgsDockInfoWrite(value["info"],w);
}
function Ros1Clearpath_dock_msgsGetDockByUuidResponseRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPoseRead(r);
value["dock_pose"]=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseRead(r);
value["predock_pose"]=decoded;
}
{
const decoded=Ros1Clearpath_dock_msgsDockInfoRead(r);
value["info"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsGetDockByUuidResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["dock_pose"];
out["dock_pose"]=Ros1Geometry_msgsPoseCapture(field);
}
{const field=value["predock_pose"];
out["predock_pose"]=Ros1Geometry_msgsPoseCapture(field);
}
{const field=value["info"];
out["info"]=Ros1Clearpath_dock_msgsDockInfoCapture(field);
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/ImportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsImportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a9ffc12e91eac79296a09b8a795b665e0bb176380d9ec48b598c2bffde5ad9ae",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_dock_msgsImportDataRequestSize(Ros1Clearpath_dock_msgsImportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsImportDataRequestCapture(value),n=Ros1Clearpath_dock_msgsImportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsImportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsImportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsImportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsImportDataRequestWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_dock_msgsImportDataRequestRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsImportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/ImportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsImportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_582614b533e6d2f79eb08cdd1f8f7786a98428d13ca197c967b87a169f847e5d",maxWireBytes:2412,minWireBytes:4,cartesianMaxWireBytes:2412n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"docks":[]};},
validateValue(value){Ros1Clearpath_dock_msgsImportDataResponseSize(Ros1Clearpath_dock_msgsImportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsImportDataResponseCapture(value),n=Ros1Clearpath_dock_msgsImportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsImportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2412,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsImportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsImportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["docks"])||value["docks"].length>2)bad();
n=addSize(n,4);
for(const element of value["docks"]){
n=addSize(n,Ros1Clearpath_dock_msgsDockInfoSize(element));
}
if(n>2412)bad(90);return n;}
function Ros1Clearpath_dock_msgsImportDataResponseWrite(value,w){
w.v.setUint32(w.at,value["docks"].length,true);w.at+=4;
for(const element of value["docks"]){
Ros1Clearpath_dock_msgsDockInfoWrite(element,w);
}
}
function Ros1Clearpath_dock_msgsImportDataResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["docks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_dock_msgsDockInfoRead(r);
value["docks"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_dock_msgsImportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["docks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_dock_msgsDockInfoCapture(element);
}out["docks"]=values;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/RemoveDockRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsRemoveDockRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_eac82f7b602d69a732174a7a43c45550e91e299973d0d6de9a8d5a58d261058e",maxWireBytes:392,minWireBytes:8,cartesianMaxWireBytes:392n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":""};},
validateValue(value){Ros1Clearpath_dock_msgsRemoveDockRequestSize(Ros1Clearpath_dock_msgsRemoveDockRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsRemoveDockRequestCapture(value),n=Ros1Clearpath_dock_msgsRemoveDockRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsRemoveDockRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,392,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsRemoveDockRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsRemoveDockRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(n>392)bad(90);return n;}
function Ros1Clearpath_dock_msgsRemoveDockRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
}
function Ros1Clearpath_dock_msgsRemoveDockRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsRemoveDockRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/RemoveDockResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsRemoveDockResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_64707a9a3b2ad309850d39b89a8ae0f0f26a5f823dc142f2ee3e4ccb4769574f",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsRemoveDockResponseSize(Ros1Clearpath_dock_msgsRemoveDockResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsRemoveDockResponseCapture(value),n=Ros1Clearpath_dock_msgsRemoveDockResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsRemoveDockResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsRemoveDockResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsRemoveDockResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsRemoveDockResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsRemoveDockResponseRead(r){const value={};
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
function Ros1Clearpath_dock_msgsRemoveDockResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/SetDockLocationByIdRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_581419b14cf665be4a9d3dda8ba2c4233ebf084576d8c9a659158c3dc63d3c6b",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_dock_msgsSetDockLocationByIdRequestSize(Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCapture(value),n=Ros1Clearpath_dock_msgsSetDockLocationByIdRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsSetDockLocationByIdRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsSetDockLocationByIdRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsSetDockLocationByIdRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_dock_msgsSetDockLocationByIdRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_dock_msgsSetDockLocationByIdRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/SetDockLocationByIdResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c3a384f22b47a30e4ca35aff300960108f5feced16af4bffeee342fcd3b07b12",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsSetDockLocationByIdResponseSize(Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCapture(value),n=Ros1Clearpath_dock_msgsSetDockLocationByIdResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsSetDockLocationByIdResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsSetDockLocationByIdResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsSetDockLocationByIdResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsSetDockLocationByIdResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsSetDockLocationByIdResponseRead(r){const value={};
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
function Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/SetDockLocationByNameRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4751614c464a74f577b3da7c8224def643d79dfcdfdb250deb6aa4dd5ffd3773",maxWireBytes:132,minWireBytes:4,cartesianMaxWireBytes:132n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":""};},
validateValue(value){Ros1Clearpath_dock_msgsSetDockLocationByNameRequestSize(Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCapture(value),n=Ros1Clearpath_dock_msgsSetDockLocationByNameRequestSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsSetDockLocationByNameRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,132,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsSetDockLocationByNameRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsSetDockLocationByNameRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(n>132)bad(90);return n;}
function Ros1Clearpath_dock_msgsSetDockLocationByNameRequestWrite(value,w){
w.text(value["name"],128);
}
function Ros1Clearpath_dock_msgsSetDockLocationByNameRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
return value;}
function Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
return out;}
/** Complete ROS1 clearpath_dock_msgs/SetDockLocationByNameResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_596351bd90396d0cfd1f195b82dcae64edc481f8e9fba25dc9548942df2aca6e",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_dock_msgsSetDockLocationByNameResponseSize(Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCapture(value),n=Ros1Clearpath_dock_msgsSetDockLocationByNameResponseSize(owned),w=new Writer(n);Ros1Clearpath_dock_msgsSetDockLocationByNameResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_dock_msgsSetDockLocationByNameResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_dock_msgsSetDockLocationByNameResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_dock_msgsSetDockLocationByNameResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_dock_msgsSetDockLocationByNameResponseRead(r){const value={};
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
function Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertCartesianToLatLonRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c2cd021cd86fca2e45b47022987f7de44d14d010abf5fcf31c326310abd19305",maxWireBytes:200,minWireBytes:72,cartesianMaxWireBytes:200n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":Ros1Geometry_msgsPoseStampedCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestSize(Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCapture(value),n=Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,200,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(value["pose"]));
if(n>200)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestWrite(value,w){
Ros1Geometry_msgsPoseStampedWrite(value["pose"],w);
}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["pose"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseStampedCapture(field);
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertCartesianToLatLonResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4de91da28a53ef701e8d063d74fb527d296118d5b7bb9255ee027057f7f35312",maxWireBytes:244,minWireBytes:116,cartesianMaxWireBytes:244n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":Ros1Sensor_msgsNavSatFixCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseSize(Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCapture(value),n=Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,244,90);if(owned.length<116)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Sensor_msgsNavSatFixSize(value["msg"]));
if(n>244)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseWrite(value,w){
Ros1Sensor_msgsNavSatFixWrite(value["msg"],w);
}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseRead(r){const value={};
{
const decoded=Ros1Sensor_msgsNavSatFixRead(r);
value["msg"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
out["msg"]=Ros1Sensor_msgsNavSatFixCapture(field);
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertCartesianToLatLonArrayRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_03e1cf82239cc5336b5b7b54c69c267afc70318c1e221ef30913f9add2a0aa70",maxWireBytes:404,minWireBytes:4,cartesianMaxWireBytes:404n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":[]};},
validateValue(value){Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestSize(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCapture(value),n=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,404,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["pose"])||value["pose"].length>2)bad();
n=addSize(n,4);
for(const element of value["pose"]){
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(element));
}
if(n>404)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestWrite(value,w){
w.v.setUint32(w.at,value["pose"].length,true);w.at+=4;
for(const element of value["pose"]){
Ros1Geometry_msgsPoseStampedWrite(element,w);
}
}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["pose"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["pose"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPoseStampedCapture(element);
}out["pose"]=values;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertCartesianToLatLonArrayResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_403b1732b1772db7eee605e8e7560f923756d1d43c9ed036705455bd0f33498b",maxWireBytes:492,minWireBytes:4,cartesianMaxWireBytes:492n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":[]};},
validateValue(value){Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseSize(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCapture(value),n=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,492,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["msg"])||value["msg"].length>2)bad();
n=addSize(n,4);
for(const element of value["msg"]){
n=addSize(n,Ros1Sensor_msgsNavSatFixSize(element));
}
if(n>492)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseWrite(value,w){
w.v.setUint32(w.at,value["msg"].length,true);w.at+=4;
for(const element of value["msg"]){
Ros1Sensor_msgsNavSatFixWrite(element,w);
}
}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["msg"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Sensor_msgsNavSatFixRead(r);
value["msg"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Sensor_msgsNavSatFixCapture(element);
}out["msg"]=values;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertLatLonToCartesianRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e1aed3afdc81e8fb0a2539f2b46971fe70f67bafaf6660dcc96e575bcb92d95b",maxWireBytes:244,minWireBytes:116,cartesianMaxWireBytes:244n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":Ros1Sensor_msgsNavSatFixCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestSize(Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCapture(value),n=Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,244,90);if(owned.length<116)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Sensor_msgsNavSatFixSize(value["msg"]));
if(n>244)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestWrite(value,w){
Ros1Sensor_msgsNavSatFixWrite(value["msg"],w);
}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestRead(r){const value={};
{
const decoded=Ros1Sensor_msgsNavSatFixRead(r);
value["msg"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
out["msg"]=Ros1Sensor_msgsNavSatFixCapture(field);
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertLatLonToCartesianResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_252d8d183faa2f20cfa6b10d35deeee44a2c2b2ae04ef01e20e556426a63ce76",maxWireBytes:200,minWireBytes:72,cartesianMaxWireBytes:200n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":Ros1Geometry_msgsPoseStampedCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseSize(Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCapture(value),n=Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,200,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(value["pose"]));
if(n>200)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseWrite(value,w){
Ros1Geometry_msgsPoseStampedWrite(value["pose"],w);
}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseRead(r){const value={};
{
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["pose"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
out["pose"]=Ros1Geometry_msgsPoseStampedCapture(field);
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertLatLonToCartesianArrayRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_20251e11541b312f114d6a6c0c007086369ed272952a4144e459af9d37472e9a",maxWireBytes:492,minWireBytes:4,cartesianMaxWireBytes:492n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":[]};},
validateValue(value){Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestSize(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCapture(value),n=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,492,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["msg"])||value["msg"].length>2)bad();
n=addSize(n,4);
for(const element of value["msg"]){
n=addSize(n,Ros1Sensor_msgsNavSatFixSize(element));
}
if(n>492)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestWrite(value,w){
w.v.setUint32(w.at,value["msg"].length,true);w.at+=4;
for(const element of value["msg"]){
Ros1Sensor_msgsNavSatFixWrite(element,w);
}
}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["msg"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Sensor_msgsNavSatFixRead(r);
value["msg"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Sensor_msgsNavSatFixCapture(element);
}out["msg"]=values;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ConvertLatLonToCartesianArrayResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b2b9af859bba03c260b29eea8953b0a1c232e8c482353188a19db61a40480ae3",maxWireBytes:404,minWireBytes:4,cartesianMaxWireBytes:404n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pose":[]};},
validateValue(value){Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseSize(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCapture(value),n=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,404,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["pose"])||value["pose"].length>2)bad();
n=addSize(n,4);
for(const element of value["pose"]){
n=addSize(n,Ros1Geometry_msgsPoseStampedSize(element));
}
if(n>404)bad(90);return n;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseWrite(value,w){
w.v.setUint32(w.at,value["pose"].length,true);w.at+=4;
for(const element of value["pose"]){
Ros1Geometry_msgsPoseStampedWrite(element,w);
}
}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["pose"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Geometry_msgsPoseStampedRead(r);
value["pose"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pose"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Geometry_msgsPoseStampedCapture(element);
}out["pose"]=values;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ResetLocalizationRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsResetLocalizationRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_22fbc4612b0b68bd20e0018476f5ed22276b22cc677fbfdb7645f6e242a8c707",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"require_gnss":false,"gnss_samples":0};},
validateValue(value){Ros1Clearpath_localization_msgsResetLocalizationRequestSize(Ros1Clearpath_localization_msgsResetLocalizationRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsResetLocalizationRequestCapture(value),n=Ros1Clearpath_localization_msgsResetLocalizationRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsResetLocalizationRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsResetLocalizationRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsResetLocalizationRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["require_gnss"]!=="boolean")bad();n=addSize(n,1);
integer(value["gnss_samples"],4,false);n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros1Clearpath_localization_msgsResetLocalizationRequestWrite(value,w){
w.v.setUint8(w.at,value["require_gnss"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["gnss_samples"],true);w.at+=4;
}
function Ros1Clearpath_localization_msgsResetLocalizationRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["require_gnss"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["gnss_samples"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsResetLocalizationRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["require_gnss"];
out["require_gnss"]=field;
}
{const field=value["gnss_samples"];
out["gnss_samples"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/ResetLocalizationResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsResetLocalizationResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6c613160101b7598e26b7243f7e756c3f55aecad15725e5d68f64540ef5f28ab",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_localization_msgsResetLocalizationResponseSize(Ros1Clearpath_localization_msgsResetLocalizationResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsResetLocalizationResponseCapture(value),n=Ros1Clearpath_localization_msgsResetLocalizationResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsResetLocalizationResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsResetLocalizationResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsResetLocalizationResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_localization_msgsResetLocalizationResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_localization_msgsResetLocalizationResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_localization_msgsResetLocalizationResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SetDatumRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSetDatumRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c1e4818a01522e1c73da13bf605db0464c245908182f728b016d436fdf4a6574",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lat":0,"lon":0};},
validateValue(value){Ros1Clearpath_localization_msgsSetDatumRequestSize(Ros1Clearpath_localization_msgsSetDatumRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSetDatumRequestCapture(value),n=Ros1Clearpath_localization_msgsSetDatumRequestSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSetDatumRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSetDatumRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSetDatumRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["lat"]!=="number"||!Number.isFinite(value["lat"])||!Number.isFinite(Math.fround(value["lat"])))bad();n=addSize(n,4);
if(typeof value["lon"]!=="number"||!Number.isFinite(value["lon"])||!Number.isFinite(Math.fround(value["lon"])))bad();n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros1Clearpath_localization_msgsSetDatumRequestWrite(value,w){
w.v.setFloat32(w.at,value["lat"],true);w.at+=4;
w.v.setFloat32(w.at,value["lon"],true);w.at+=4;
}
function Ros1Clearpath_localization_msgsSetDatumRequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["lat"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["lon"]=decoded;
}
return value;}
function Ros1Clearpath_localization_msgsSetDatumRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lat"];
out["lat"]=field;
}
{const field=value["lon"];
out["lon"]=field;
}
return out;}
/** Complete ROS1 clearpath_localization_msgs/SetDatumResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_localization_msgsSetDatumResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_db90d1e0168fb29d9563692c1b5d86c1a30b2cf4bb1186b370ed177842acc6f2",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_localization_msgsSetDatumResponseSize(Ros1Clearpath_localization_msgsSetDatumResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_localization_msgsSetDatumResponseCapture(value),n=Ros1Clearpath_localization_msgsSetDatumResponseSize(owned),w=new Writer(n);Ros1Clearpath_localization_msgsSetDatumResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_localization_msgsSetDatumResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_localization_msgsSetDatumResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_localization_msgsSetDatumResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_localization_msgsSetDatumResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_localization_msgsSetDatumResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/DeleteLogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsDeleteLogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b54559708072a689b0944885be1d6627b37bbee14845982890968cb51be523da",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","delete_media":false,"purge_record":false};},
validateValue(value){Ros1Clearpath_logger_msgsDeleteLogRequestSize(Ros1Clearpath_logger_msgsDeleteLogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsDeleteLogRequestCapture(value),n=Ros1Clearpath_logger_msgsDeleteLogRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsDeleteLogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsDeleteLogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsDeleteLogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["delete_media"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["purge_record"]!=="boolean")bad();n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros1Clearpath_logger_msgsDeleteLogRequestWrite(value,w){
w.text(value["uuid"],256);
w.v.setUint8(w.at,value["delete_media"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["purge_record"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_logger_msgsDeleteLogRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["delete_media"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["purge_record"]=decoded!==0;
}
return value;}
function Ros1Clearpath_logger_msgsDeleteLogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["delete_media"];
out["delete_media"]=field;
}
{const field=value["purge_record"];
out["purge_record"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/DeleteLogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsDeleteLogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d78cf7d455fda53e3ab5882e1990e75da7f93546f9e59e6e3b2325c738cabc49",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_logger_msgsDeleteLogResponseSize(Ros1Clearpath_logger_msgsDeleteLogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsDeleteLogResponseCapture(value),n=Ros1Clearpath_logger_msgsDeleteLogResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsDeleteLogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsDeleteLogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsDeleteLogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_logger_msgsDeleteLogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_logger_msgsDeleteLogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_logger_msgsDeleteLogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/DownloadLogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsDownloadLogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_92e66dfc586d2444e0314d6eb77604c23354c0f1922b50f88b84d970217d5e64",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"log_uuid":""};},
validateValue(value){Ros1Clearpath_logger_msgsDownloadLogRequestSize(Ros1Clearpath_logger_msgsDownloadLogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsDownloadLogRequestCapture(value),n=Ros1Clearpath_logger_msgsDownloadLogRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsDownloadLogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsDownloadLogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsDownloadLogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["log_uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_logger_msgsDownloadLogRequestWrite(value,w){
w.text(value["log_uuid"],256);
}
function Ros1Clearpath_logger_msgsDownloadLogRequestRead(r){const value={};
{
const decoded=r.text(256);
value["log_uuid"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsDownloadLogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["log_uuid"];
out["log_uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/DownloadLogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsDownloadLogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_02df348a428fed4659fff462c547d41fa0aaa1daa0101a2eee25d0634924bcf8",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":"","media_missing":false};},
validateValue(value){Ros1Clearpath_logger_msgsDownloadLogResponseSize(Ros1Clearpath_logger_msgsDownloadLogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsDownloadLogResponseCapture(value),n=Ros1Clearpath_logger_msgsDownloadLogResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsDownloadLogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsDownloadLogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsDownloadLogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["path"],256).length);
if(typeof value["media_missing"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros1Clearpath_logger_msgsDownloadLogResponseWrite(value,w){
w.text(value["path"],256);
w.v.setUint8(w.at,value["media_missing"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_logger_msgsDownloadLogResponseRead(r){const value={};
{
const decoded=r.text(256);
value["path"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["media_missing"]=decoded!==0;
}
return value;}
function Ros1Clearpath_logger_msgsDownloadLogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=field;
}
{const field=value["media_missing"];
out["media_missing"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/GetAllLogsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsGetAllLogsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_68d786f935d2fd1912ac01f0196841b491f5c6d1b6e319ab22bdca371993fbb6",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_logger_msgsGetAllLogsRequestSize(Ros1Clearpath_logger_msgsGetAllLogsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsGetAllLogsRequestCapture(value),n=Ros1Clearpath_logger_msgsGetAllLogsRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsGetAllLogsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsGetAllLogsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsGetAllLogsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_logger_msgsGetAllLogsRequestWrite(value,w){
}
function Ros1Clearpath_logger_msgsGetAllLogsRequestRead(r){const value={};
return value;}
function Ros1Clearpath_logger_msgsGetAllLogsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_logger_msgs/GetAllLogsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsGetAllLogsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3c4c4948dd5425a9ca6a1042f07aa926138e21ce73c01ead9fe7471f27690149",maxWireBytes:14132,minWireBytes:4,cartesianMaxWireBytes:14132n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"logs":[]};},
validateValue(value){Ros1Clearpath_logger_msgsGetAllLogsResponseSize(Ros1Clearpath_logger_msgsGetAllLogsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsGetAllLogsResponseCapture(value),n=Ros1Clearpath_logger_msgsGetAllLogsResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsGetAllLogsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,14132,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsGetAllLogsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsGetAllLogsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["logs"])||value["logs"].length>2)bad();
n=addSize(n,4);
for(const element of value["logs"]){
n=addSize(n,Ros1Clearpath_logger_msgsEventLogSize(element));
}
if(n>14132)bad(90);return n;}
function Ros1Clearpath_logger_msgsGetAllLogsResponseWrite(value,w){
w.v.setUint32(w.at,value["logs"].length,true);w.at+=4;
for(const element of value["logs"]){
Ros1Clearpath_logger_msgsEventLogWrite(element,w);
}
}
function Ros1Clearpath_logger_msgsGetAllLogsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["logs"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_logger_msgsEventLogRead(r);
value["logs"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_logger_msgsGetAllLogsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["logs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_logger_msgsEventLogCapture(element);
}out["logs"]=values;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordCustomEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordCustomEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_63b69b7bad4a34c168013248ed6c6e7dede8c588750a66d78e174f13ed0f89c5",maxWireBytes:916,minWireBytes:12,cartesianMaxWireBytes:916n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"custom_fields_json":"","tags":[],"frame_id":""};},
validateValue(value){Ros1Clearpath_logger_msgsRecordCustomEventRequestSize(Ros1Clearpath_logger_msgsRecordCustomEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordCustomEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordCustomEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordCustomEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,916,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordCustomEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordCustomEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(n>916)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordCustomEventRequestWrite(value,w){
w.text(value["custom_fields_json"],256);
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
w.text(value["frame_id"],128);
}
function Ros1Clearpath_logger_msgsRecordCustomEventRequestRead(r){const value={};
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
{
const decoded=r.text(128);
value["frame_id"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordCustomEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordCustomEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordCustomEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2c722c562ef52b0eea7c7888b494cc98bb79947cde164074b4cbed645caa93cd",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordCustomEventResponseSize(Ros1Clearpath_logger_msgsRecordCustomEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordCustomEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordCustomEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordCustomEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordCustomEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordCustomEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordCustomEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordCustomEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordCustomEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordErrorEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordErrorEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a9cdf939aa39c10e714f29245790b1ec112d2e0ee5ec7629ee81df1effbfbc78",maxWireBytes:524,minWireBytes:12,cartesianMaxWireBytes:524n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"error_code":0,"error_msg":"","error_component":""};},
validateValue(value){Ros1Clearpath_logger_msgsRecordErrorEventRequestSize(Ros1Clearpath_logger_msgsRecordErrorEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordErrorEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordErrorEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordErrorEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,524,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordErrorEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordErrorEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["error_code"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["error_msg"],256).length);
n=addSize(n,4+textBytes(value["error_component"],256).length);
if(n>524)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordErrorEventRequestWrite(value,w){
w.v.setInt32(w.at,value["error_code"],true);w.at+=4;
w.text(value["error_msg"],256);
w.text(value["error_component"],256);
}
function Ros1Clearpath_logger_msgsRecordErrorEventRequestRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=r.text(256);
value["error_msg"]=decoded;
}
{
const decoded=r.text(256);
value["error_component"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordErrorEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_msg"];
out["error_msg"]=field;
}
{const field=value["error_component"];
out["error_component"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordErrorEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordErrorEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d8134e572a44c305c69f2ae1da4f37a6d9920cd762bbe9a90dc15e9ef7981b54",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordErrorEventResponseSize(Ros1Clearpath_logger_msgsRecordErrorEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordErrorEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordErrorEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordErrorEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordErrorEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordErrorEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordErrorEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordErrorEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordErrorEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_05f725bbe2f4fbd2976bf766a0e60e6a5566326b27dd09f774a096c0864f677e",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordEventRequestSize(Ros1Clearpath_logger_msgsRecordEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordEventRequestWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordEventRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5b1e7fb4032c38c3b4a7ab8477019fc0290c643e9f1cff449d007d33c9a22961",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordEventResponseSize(Ros1Clearpath_logger_msgsRecordEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordLocationEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordLocationEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_066b7fbb43be3ddb476c6542478e70c253dad5bb6930d5b757f2e578a9a8e884",maxWireBytes:1680,minWireBytes:776,cartesianMaxWireBytes:1680n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"latitude":0,"longitude":0,"heading":0,"velocity":Ros1Geometry_msgsTwistCodec.defaultValue(),"odometry":Ros1Nav_msgsOdometryCodec.defaultValue(),"tags":[]};},
validateValue(value){Ros1Clearpath_logger_msgsRecordLocationEventRequestSize(Ros1Clearpath_logger_msgsRecordLocationEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordLocationEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordLocationEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordLocationEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1680,90);if(owned.length<776)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordLocationEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordLocationEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
n=addSize(n,Ros1Geometry_msgsTwistSize(value["velocity"]));
n=addSize(n,Ros1Nav_msgsOdometrySize(value["odometry"]));
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>1680)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordLocationEventRequestWrite(value,w){
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
Ros1Geometry_msgsTwistWrite(value["velocity"],w);
Ros1Nav_msgsOdometryWrite(value["odometry"],w);
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
}
function Ros1Clearpath_logger_msgsRecordLocationEventRequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistRead(r);
value["velocity"]=decoded;
}
{
const decoded=Ros1Nav_msgsOdometryRead(r);
value["odometry"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_logger_msgsRecordLocationEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["velocity"];
out["velocity"]=Ros1Geometry_msgsTwistCapture(field);
}
{const field=value["odometry"];
out["odometry"]=Ros1Nav_msgsOdometryCapture(field);
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordLocationEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordLocationEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_707fdc02afa7c63796a952af43697c371d530d5976661c42e1867e3fe5c75e3d",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordLocationEventResponseSize(Ros1Clearpath_logger_msgsRecordLocationEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordLocationEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordLocationEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordLocationEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordLocationEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordLocationEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordLocationEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordLocationEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordLocationEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordMediaEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordMediaEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_df1b4ba70aa6a687d2be8935aaed1ccadade1ac77a148db72a563a74b300bda5",maxWireBytes:652,minWireBytes:12,cartesianMaxWireBytes:652n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"media_path":"","mime_type":"","frame_id":""};},
validateValue(value){Ros1Clearpath_logger_msgsRecordMediaEventRequestSize(Ros1Clearpath_logger_msgsRecordMediaEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordMediaEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordMediaEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordMediaEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,652,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordMediaEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordMediaEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["media_path"],256).length);
n=addSize(n,4+textBytes(value["mime_type"],256).length);
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(n>652)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordMediaEventRequestWrite(value,w){
w.text(value["media_path"],256);
w.text(value["mime_type"],256);
w.text(value["frame_id"],128);
}
function Ros1Clearpath_logger_msgsRecordMediaEventRequestRead(r){const value={};
{
const decoded=r.text(256);
value["media_path"]=decoded;
}
{
const decoded=r.text(256);
value["mime_type"]=decoded;
}
{
const decoded=r.text(128);
value["frame_id"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordMediaEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["media_path"];
out["media_path"]=field;
}
{const field=value["mime_type"];
out["mime_type"]=field;
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordMediaEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordMediaEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7f8ccc45f90b3c2df031bb6aef4c00e7e35ddf620bbc899a9c229d453eb275ae",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordMediaEventResponseSize(Ros1Clearpath_logger_msgsRecordMediaEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordMediaEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordMediaEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordMediaEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordMediaEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordMediaEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordMediaEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordMediaEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordMediaEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordPowerEventRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordPowerEventRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_82706bac64cc379346fc004d2c86b8d466e976a3bb676dea2e8ae7ed1947d974",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"battery_percent":0,"charging":false};},
validateValue(value){Ros1Clearpath_logger_msgsRecordPowerEventRequestSize(Ros1Clearpath_logger_msgsRecordPowerEventRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordPowerEventRequestCapture(value),n=Ros1Clearpath_logger_msgsRecordPowerEventRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordPowerEventRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordPowerEventRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordPowerEventRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["battery_percent"]!=="number"||!Number.isFinite(value["battery_percent"])||!Number.isFinite(Math.fround(value["battery_percent"])))bad();n=addSize(n,4);
if(typeof value["charging"]!=="boolean")bad();n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordPowerEventRequestWrite(value,w){
w.v.setFloat32(w.at,value["battery_percent"],true);w.at+=4;
w.v.setUint8(w.at,value["charging"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_logger_msgsRecordPowerEventRequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["battery_percent"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["charging"]=decoded!==0;
}
return value;}
function Ros1Clearpath_logger_msgsRecordPowerEventRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["battery_percent"];
out["battery_percent"]=field;
}
{const field=value["charging"];
out["charging"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/RecordPowerEventResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsRecordPowerEventResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4d15a62946c8e9941fc9760d43e8eebcc7e9125561e01e646c6fb6edf7aaf866",maxWireBytes:3190,minWireBytes:878,cartesianMaxWireBytes:3190n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":Ros1Clearpath_logger_msgsEventCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsRecordPowerEventResponseSize(Ros1Clearpath_logger_msgsRecordPowerEventResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsRecordPowerEventResponseCapture(value),n=Ros1Clearpath_logger_msgsRecordPowerEventResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsRecordPowerEventResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3190,90);if(owned.length<878)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsRecordPowerEventResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsRecordPowerEventResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventSize(value["event"]));
if(n>3190)bad(90);return n;}
function Ros1Clearpath_logger_msgsRecordPowerEventResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventWrite(value["event"],w);
}
function Ros1Clearpath_logger_msgsRecordPowerEventResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventRead(r);
value["event"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsRecordPowerEventResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=Ros1Clearpath_logger_msgsEventCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/StartRecordingRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsStartRecordingRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_eb42cefac21b32710fd09c9fb1629f55a334eabc10cc958e0d98911e786ca42f",maxWireBytes:132,minWireBytes:4,cartesianMaxWireBytes:132n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":""};},
validateValue(value){Ros1Clearpath_logger_msgsStartRecordingRequestSize(Ros1Clearpath_logger_msgsStartRecordingRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsStartRecordingRequestCapture(value),n=Ros1Clearpath_logger_msgsStartRecordingRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsStartRecordingRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,132,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsStartRecordingRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsStartRecordingRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(n>132)bad(90);return n;}
function Ros1Clearpath_logger_msgsStartRecordingRequestWrite(value,w){
w.text(value["name"],128);
}
function Ros1Clearpath_logger_msgsStartRecordingRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsStartRecordingRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/StartRecordingResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsStartRecordingResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2220536f6a937fb7bb198bfdbd288855bf294a4b3404e702349f9ff83428af31",maxWireBytes:7064,minWireBytes:44,cartesianMaxWireBytes:7064n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"log":Ros1Clearpath_logger_msgsEventLogCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsStartRecordingResponseSize(Ros1Clearpath_logger_msgsStartRecordingResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsStartRecordingResponseCapture(value),n=Ros1Clearpath_logger_msgsStartRecordingResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsStartRecordingResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7064,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsStartRecordingResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsStartRecordingResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventLogSize(value["log"]));
if(n>7064)bad(90);return n;}
function Ros1Clearpath_logger_msgsStartRecordingResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventLogWrite(value["log"],w);
}
function Ros1Clearpath_logger_msgsStartRecordingResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventLogRead(r);
value["log"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsStartRecordingResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["log"];
out["log"]=Ros1Clearpath_logger_msgsEventLogCapture(field);
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/StopRecordingRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsStopRecordingRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ece885864f4cc93062aec96411096358e1d2c4885f30431b762b4732d3ff44b5",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"custom_fields_json":""};},
validateValue(value){Ros1Clearpath_logger_msgsStopRecordingRequestSize(Ros1Clearpath_logger_msgsStopRecordingRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsStopRecordingRequestCapture(value),n=Ros1Clearpath_logger_msgsStopRecordingRequestSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsStopRecordingRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsStopRecordingRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsStopRecordingRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_logger_msgsStopRecordingRequestWrite(value,w){
w.text(value["custom_fields_json"],256);
}
function Ros1Clearpath_logger_msgsStopRecordingRequestRead(r){const value={};
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsStopRecordingRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
return out;}
/** Complete ROS1 clearpath_logger_msgs/StopRecordingResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_logger_msgsStopRecordingResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bc3f620603442ee53fa1b62618eb73ca1fa68347be928b6b38acc65d63295eca",maxWireBytes:7064,minWireBytes:44,cartesianMaxWireBytes:7064n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"log":Ros1Clearpath_logger_msgsEventLogCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_logger_msgsStopRecordingResponseSize(Ros1Clearpath_logger_msgsStopRecordingResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_logger_msgsStopRecordingResponseCapture(value),n=Ros1Clearpath_logger_msgsStopRecordingResponseSize(owned),w=new Writer(n);Ros1Clearpath_logger_msgsStopRecordingResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7064,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Clearpath_logger_msgsStopRecordingResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_logger_msgsStopRecordingResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_logger_msgsEventLogSize(value["log"]));
if(n>7064)bad(90);return n;}
function Ros1Clearpath_logger_msgsStopRecordingResponseWrite(value,w){
Ros1Clearpath_logger_msgsEventLogWrite(value["log"],w);
}
function Ros1Clearpath_logger_msgsStopRecordingResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_logger_msgsEventLogRead(r);
value["log"]=decoded;
}
return value;}
function Ros1Clearpath_logger_msgsStopRecordingResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["log"];
out["log"]=Ros1Clearpath_logger_msgsEventLogCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkEdgeRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c2ad8aa1d5988fb0397e85e15aa37211b7041a2ecdc67e5c89b53a38431e56f3",maxWireBytes:793,minWireBytes:25,cartesianMaxWireBytes:793n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","start_point_id":"","end_point_id":"","radius":0,"speed_limit":0,"cost_factor":0,"symmetrical":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestSize(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,793,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
n=addSize(n,4+textBytes(value["start_point_id"],256).length);
n=addSize(n,4+textBytes(value["end_point_id"],256).length);
if(typeof value["radius"]!=="number"||!Number.isFinite(value["radius"])||!Number.isFinite(Math.fround(value["radius"])))bad();n=addSize(n,4);
if(typeof value["speed_limit"]!=="number"||!Number.isFinite(value["speed_limit"])||!Number.isFinite(Math.fround(value["speed_limit"])))bad();n=addSize(n,4);
if(typeof value["cost_factor"]!=="number"||!Number.isFinite(value["cost_factor"])||!Number.isFinite(Math.fround(value["cost_factor"])))bad();n=addSize(n,4);
if(typeof value["symmetrical"]!=="boolean")bad();n=addSize(n,1);
if(n>793)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestWrite(value,w){
w.text(value["map_id"],256);
w.text(value["start_point_id"],256);
w.text(value["end_point_id"],256);
w.v.setFloat32(w.at,value["radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["speed_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["cost_factor"],true);w.at+=4;
w.v.setUint8(w.at,value["symmetrical"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
const decoded=r.text(256);
value["start_point_id"]=decoded;
}
{
const decoded=r.text(256);
value["end_point_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cost_factor"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["symmetrical"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["start_point_id"];
out["start_point_id"]=field;
}
{const field=value["end_point_id"];
out["end_point_id"]=field;
}
{const field=value["radius"];
out["radius"]=field;
}
{const field=value["speed_limit"];
out["speed_limit"]=field;
}
{const field=value["cost_factor"];
out["cost_factor"]=field;
}
{const field=value["symmetrical"];
out["symmetrical"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkEdgeResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2e14500e273e2a953389948d0c7533b096e7559f41d17e518f9eec6e93b7df4f",maxWireBytes:1652,minWireBytes:4,cartesianMaxWireBytes:1652n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"edges":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseSize(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1652,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["edges"])||value["edges"].length>2)bad();
n=addSize(n,4);
for(const element of value["edges"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkEdgeSize(element));
}
if(n>1652)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseWrite(value,w){
w.v.setUint32(w.at,value["edges"].length,true);w.at+=4;
for(const element of value["edges"]){
Ros1Clearpath_navigation_msgsNetworkEdgeWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["edges"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkEdgeRead(r);
value["edges"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["edges"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkEdgeCapture(element);
}out["edges"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkPathRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0c8717a0058d4fbdf9927579f6bb29d84e32cadcaa811b2d3aa02464807c55d0",maxWireBytes:301,minWireBytes:13,cartesianMaxWireBytes:301n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"latitudes":[],"longitudes":[],"map_id":"","symmetrical":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestSize(Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,301,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["latitudes"])||value["latitudes"].length>2)bad();
n=addSize(n,4);
for(const element of value["latitudes"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["longitudes"])||value["longitudes"].length>2)bad();
n=addSize(n,4);
for(const element of value["longitudes"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,4+textBytes(value["map_id"],256).length);
if(typeof value["symmetrical"]!=="boolean")bad();n=addSize(n,1);
if(n>301)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestWrite(value,w){
w.v.setUint32(w.at,value["latitudes"].length,true);w.at+=4;
for(const element of value["latitudes"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["longitudes"].length,true);w.at+=4;
for(const element of value["longitudes"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.text(value["map_id"],256);
w.v.setUint8(w.at,value["symmetrical"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["latitudes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitudes"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["longitudes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitudes"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["symmetrical"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["latitudes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["latitudes"]=values;
}
{const field=value["longitudes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["longitudes"]=values;
}
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["symmetrical"];
out["symmetrical"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkPathResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ec9794b231eb22dfd44263164562c393069793f34f603bc35181ef358f26782c",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseSize(Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseRead(r){const value={};
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
function Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkPointRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c1eb740c2558280db5b6fbe8be5cb67ba3c1538d2e9dd19d7fb5b38df316f909",maxWireBytes:1325,minWireBytes:29,cartesianMaxWireBytes:1325n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"latitude":0,"longitude":0,"map_id":"","connect_from":[],"connect_to":[],"symmetrical":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestSize(Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1325,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
n=addSize(n,4+textBytes(value["map_id"],256).length);
if(!Array.isArray(value["connect_from"])||value["connect_from"].length>2)bad();
n=addSize(n,4);
for(const element of value["connect_from"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["connect_to"])||value["connect_to"].length>2)bad();
n=addSize(n,4);
for(const element of value["connect_to"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["symmetrical"]!=="boolean")bad();n=addSize(n,1);
if(n>1325)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestWrite(value,w){
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.text(value["map_id"],256);
w.v.setUint32(w.at,value["connect_from"].length,true);w.at+=4;
for(const element of value["connect_from"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["connect_to"].length,true);w.at+=4;
for(const element of value["connect_to"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["symmetrical"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["connect_from"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["connect_from"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["connect_to"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["connect_to"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["symmetrical"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["connect_from"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["connect_from"]=values;
}
{const field=value["connect_to"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["connect_to"]=values;
}
{const field=value["symmetrical"];
out["symmetrical"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddNetworkPointResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_adf7216e0cef0cf515203d17726e6c5acf46c03601cd931a4a1e59cdeca313be",maxWireBytes:276,minWireBytes:20,cartesianMaxWireBytes:276n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"point":Ros1Clearpath_navigation_msgsNetworkPointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseSize(Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,276,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(value["point"]));
if(n>276)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkPointWrite(value["point"],w);
}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["point"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["point"];
out["point"]=Ros1Clearpath_navigation_msgsNetworkPointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddRemoveByIdRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3eb2a9e36f226c731491dcb174d1af9bf5bcfe131206dcea36d0d2b42cfc028e",maxWireBytes:524,minWireBytes:12,cartesianMaxWireBytes:524n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","parent_uuid":"","position":0};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestSize(Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,524,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["parent_uuid"],256).length);
integer(value["position"],4,true);n=addSize(n,4);
if(n>524)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["parent_uuid"],256);
w.v.setInt32(w.at,value["position"],true);w.at+=4;
}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["parent_uuid"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["position"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["parent_uuid"];
out["parent_uuid"]=field;
}
{const field=value["position"];
out["position"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddRemoveByIdResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a81b57d98295b7ad37f1bcf070b016e40ea73d802ae5c113822b8341891c12ea",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ok":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseSize(Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ok"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseWrite(value,w){
w.v.setUint8(w.at,value["ok"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ok"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ok"];
out["ok"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddRemoveTagRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_23ac2c4784aae50c56d78d68ee369ee365113dad9331c6224673b9c53576f2f6",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","tag":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestSize(Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["tag"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["tag"],256);
}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["tag"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["tag"];
out["tag"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/AddRemoveTagResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f2514cde45093626fc3ec068327521d1e077279df21ca36f3812abc6bcd316c6",maxWireBytes:524,minWireBytes:4,cartesianMaxWireBytes:524n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tags":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseSize(Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,524,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>524)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseWrite(value,w){
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7c3d4053b272033a901c1fbe5c7eecaf16cd7bd7e28e2d5c6369183a7b6d62d4",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","new_name":"","reverse":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneMissionRequestSize(Ros1Clearpath_mission_manager_msgsCloneMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["new_name"],256).length);
if(typeof value["reverse"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneMissionRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["new_name"],256);
w.v.setUint8(w.at,value["reverse"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsCloneMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["new_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["reverse"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["new_name"];
out["new_name"]=field;
}
{const field=value["reverse"];
out["reverse"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_88894721fb1457e6aafd8b131c6a3b26af8d96ee27a8224e3bb1277bc0f987cc",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneMissionResponseSize(Ros1Clearpath_mission_manager_msgsCloneMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["mission"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsCloneMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2f74e78c9bc6bff2eb44665cdb4b516c6fdafa209addd1a4d4f3fa3c1976a5f5",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","new_name":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["new_name"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["new_name"],256);
}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["new_name"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["new_name"];
out["new_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1143b6aaee73251732ae77f676ba29ca561a47c11d5443d56e74326689a38ffb",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["map"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["map"],w);
}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["map"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map"];
out["map"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneNetworkMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_46048ce502c1e4aa8629826444bf77de18318f16c9398768ea75283549b802b4",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","new_name":"","reverse":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestSize(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["new_name"],256).length);
if(typeof value["reverse"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["new_name"],256);
w.v.setUint8(w.at,value["reverse"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["new_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["reverse"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["new_name"];
out["new_name"]=field;
}
{const field=value["reverse"];
out["reverse"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CloneNetworkMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7aa5c1e17efce3523e3c751e6900a63eacea43b47b6c1e8987fce3d98462b63d",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseSize(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["mission"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ClonePointOfInterestRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ffa35600ea2a044f4ef3388627f8f34323407b6dfdf314d5e68e8a997f95cc69",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","new_name":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestSize(Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["new_name"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["new_name"],256);
}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["new_name"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["new_name"];
out["new_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ClonePointOfInterestResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_97edd93b8afb8681c2c0755f0313129e0ea8bea5384235a03b3b48c39e6438d0",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsPointOfInterestCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseSize(Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(value["result"]));
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseWrite(value,w){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_caa700efafa4dbdb056e78e08af082f2f4d985f1b431b5bfb2ae019542f3925f",maxWireBytes:1964,minWireBytes:20,cartesianMaxWireBytes:1964n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","onav_config":"","waypoint_ids":[],"on_start_ids":[],"on_stop_ids":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateMissionRequestSize(Ros1Clearpath_mission_manager_msgsCreateMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1964,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(!Array.isArray(value["waypoint_ids"])||value["waypoint_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoint_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_start_ids"])||value["on_start_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_stop_ids"])||value["on_stop_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>1964)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateMissionRequestWrite(value,w){
w.text(value["name"],128);
w.text(value["onav_config"],256);
w.v.setUint32(w.at,value["waypoint_ids"].length,true);w.at+=4;
for(const element of value["waypoint_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_start_ids"].length,true);w.at+=4;
for(const element of value["on_start_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_stop_ids"].length,true);w.at+=4;
for(const element of value["on_stop_ids"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsCreateMissionRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoint_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["waypoint_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_start_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_stop_ids"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
{const field=value["waypoint_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["waypoint_ids"]=values;
}
{const field=value["on_start_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_start_ids"]=values;
}
{const field=value["on_stop_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_stop_ids"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ce4a3a3cc959015bffb0a98373399a9d1063d5dee0baa4b4e685c6fde879bc01",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateMissionResponseSize(Ros1Clearpath_mission_manager_msgsCreateMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["result"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_339dba93c2db9e3f5280805772411b26292f0587a465f95a6369828c556d9f3a",maxWireBytes:1756,minWireBytes:20,cartesianMaxWireBytes:1756n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","default_radius":0,"default_speed_limit":0,"points":[],"connections":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1756,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["default_radius"]!=="number"||!Number.isFinite(value["default_radius"])||!Number.isFinite(Math.fround(value["default_radius"])))bad();n=addSize(n,4);
if(typeof value["default_speed_limit"]!=="number"||!Number.isFinite(value["default_speed_limit"])||!Number.isFinite(Math.fround(value["default_speed_limit"])))bad();n=addSize(n,4);
if(!Array.isArray(value["points"])||value["points"].length>2)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(element));
}
if(!Array.isArray(value["connections"])||value["connections"].length>2)bad();
n=addSize(n,4);
for(const element of value["connections"]){
n=addSize(n,Ros1Clearpath_mission_manager_msgsNetworkEdgeReqSize(element));
}
if(n>1756)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestWrite(value,w){
w.text(value["name"],128);
w.v.setFloat32(w.at,value["default_radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["default_speed_limit"],true);w.at+=4;
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Clearpath_navigation_msgsNetworkPointWrite(element,w);
}
w.v.setUint32(w.at,value["connections"].length,true);w.at+=4;
for(const element of value["connections"]){
Ros1Clearpath_mission_manager_msgsNetworkEdgeReqWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_speed_limit"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["points"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["connections"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_mission_manager_msgsNetworkEdgeReqRead(r);
value["connections"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["default_radius"];
out["default_radius"]=field;
}
{const field=value["default_speed_limit"];
out["default_speed_limit"]=field;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkPointCapture(element);
}out["points"]=values;
}
{const field=value["connections"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_mission_manager_msgsNetworkEdgeReqCapture(element);
}out["connections"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cc887ebcf4ab3232cff47e7c47c90a3458fd83ca6c252d6942d4d7e2b9a4b5cb",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["result"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateNetworkMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7f681e1a88063ba1d41a6e6156a499e7dc0cc68f656c09931589b8a1532e95bf",maxWireBytes:2234,minWireBytes:34,cartesianMaxWireBytes:2234n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","onav_config":"","waypoint_ids":[],"on_start_ids":[],"on_stop_ids":[],"return_to_dock_enabled":false,"return_to_dock_id":"","return_to_dock_battery_level":0,"resume_mission_battery_level":0,"network_replan_enabled":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestSize(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2234,90);if(owned.length<34)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(!Array.isArray(value["waypoint_ids"])||value["waypoint_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoint_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_start_ids"])||value["on_start_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_stop_ids"])||value["on_stop_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["return_to_dock_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["return_to_dock_id"],256).length);
if(typeof value["return_to_dock_battery_level"]!=="number"||!Number.isFinite(value["return_to_dock_battery_level"])||!Number.isFinite(Math.fround(value["return_to_dock_battery_level"])))bad();n=addSize(n,4);
if(typeof value["resume_mission_battery_level"]!=="number"||!Number.isFinite(value["resume_mission_battery_level"])||!Number.isFinite(Math.fround(value["resume_mission_battery_level"])))bad();n=addSize(n,4);
if(typeof value["network_replan_enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>2234)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestWrite(value,w){
w.text(value["name"],128);
w.text(value["onav_config"],256);
w.v.setUint32(w.at,value["waypoint_ids"].length,true);w.at+=4;
for(const element of value["waypoint_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_start_ids"].length,true);w.at+=4;
for(const element of value["on_start_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_stop_ids"].length,true);w.at+=4;
for(const element of value["on_stop_ids"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["return_to_dock_enabled"]?1:0,true);w.at+=1;
w.text(value["return_to_dock_id"],256);
w.v.setFloat32(w.at,value["return_to_dock_battery_level"],true);w.at+=4;
w.v.setFloat32(w.at,value["resume_mission_battery_level"],true);w.at+=4;
w.v.setUint8(w.at,value["network_replan_enabled"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoint_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["waypoint_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_start_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_stop_ids"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["return_to_dock_enabled"]=decoded!==0;
}
{
const decoded=r.text(256);
value["return_to_dock_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["return_to_dock_battery_level"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["resume_mission_battery_level"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["network_replan_enabled"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
{const field=value["waypoint_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["waypoint_ids"]=values;
}
{const field=value["on_start_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_start_ids"]=values;
}
{const field=value["on_stop_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_stop_ids"]=values;
}
{const field=value["return_to_dock_enabled"];
out["return_to_dock_enabled"]=field;
}
{const field=value["return_to_dock_id"];
out["return_to_dock_id"]=field;
}
{const field=value["return_to_dock_battery_level"];
out["return_to_dock_battery_level"]=field;
}
{const field=value["resume_mission_battery_level"];
out["resume_mission_battery_level"]=field;
}
{const field=value["network_replan_enabled"];
out["network_replan_enabled"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateNetworkMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7795dd8fd48439f93f4ff545ed7ecdb4948692d973e4d688d197c7a2c8afb374",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseSize(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["result"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreatePointOfInterestRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_46db8c6cd151bb2b514dbbb6ca3c5638186e78ed8c0a5b587c63d263dbf4e707",maxWireBytes:949,minWireBytes:45,cartesianMaxWireBytes:949n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","latitude":0,"longitude":0,"elevation":0,"enable_heading":false,"heading":0,"tags":[],"custom_fields_json":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestSize(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,949,90);if(owned.length<45)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["elevation"]!=="number"||!Number.isFinite(value["elevation"]))bad();n=addSize(n,8);
if(typeof value["enable_heading"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(n>949)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestWrite(value,w){
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["elevation"],true);w.at+=8;
w.v.setUint8(w.at,value["enable_heading"]?1:0,true);w.at+=1;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
w.text(value["custom_fields_json"],256);
}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elevation"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_heading"]=decoded!==0;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["elevation"];
out["elevation"]=field;
}
{const field=value["enable_heading"];
out["enable_heading"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreatePointOfInterestResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0a515e14a911f9bbb05e1b44fccbe91b8ae9c5d83e6bf3b3055bdd46c95ab82e",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsPointOfInterestCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseSize(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(value["result"]));
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseWrite(value,w){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateTaskRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateTaskRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b6d1b102bd25c79e40c1c8f009688a8a183d7fbebb3983b97ab8ecb219e4eab3",maxWireBytes:2769,minWireBytes:33,cartesianMaxWireBytes:2769n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","action_server_name":"","version":"","floats":[],"strings":[],"allow_failure":false,"assign_to_wp":[],"assign_on_start":[],"assign_on_stop":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateTaskRequestSize(Ros1Clearpath_mission_manager_msgsCreateTaskRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateTaskRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateTaskRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateTaskRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2769,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateTaskRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateTaskRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["action_server_name"],256).length);
n=addSize(n,4+textBytes(value["version"],256).length);
if(!Array.isArray(value["floats"])||value["floats"].length>2)bad();
n=addSize(n,4);
for(const element of value["floats"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["strings"])||value["strings"].length>2)bad();
n=addSize(n,4);
for(const element of value["strings"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["allow_failure"]!=="boolean")bad();n=addSize(n,1);
if(!Array.isArray(value["assign_to_wp"])||value["assign_to_wp"].length>2)bad();
n=addSize(n,4);
for(const element of value["assign_to_wp"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["assign_on_start"])||value["assign_on_start"].length>2)bad();
n=addSize(n,4);
for(const element of value["assign_on_start"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["assign_on_stop"])||value["assign_on_stop"].length>2)bad();
n=addSize(n,4);
for(const element of value["assign_on_stop"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>2769)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateTaskRequestWrite(value,w){
w.text(value["name"],128);
w.text(value["action_server_name"],256);
w.text(value["version"],256);
w.v.setUint32(w.at,value["floats"].length,true);w.at+=4;
for(const element of value["floats"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["strings"].length,true);w.at+=4;
for(const element of value["strings"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["allow_failure"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["assign_to_wp"].length,true);w.at+=4;
for(const element of value["assign_to_wp"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["assign_on_start"].length,true);w.at+=4;
for(const element of value["assign_on_start"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["assign_on_stop"].length,true);w.at+=4;
for(const element of value["assign_on_stop"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsCreateTaskRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["action_server_name"]=decoded;
}
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["floats"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["floats"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["strings"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["strings"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["allow_failure"]=decoded!==0;
}
{
{const count=r.u32();if(count>2)bad();value["assign_to_wp"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["assign_to_wp"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["assign_on_start"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["assign_on_start"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["assign_on_stop"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["assign_on_stop"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateTaskRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["action_server_name"];
out["action_server_name"]=field;
}
{const field=value["version"];
out["version"]=field;
}
{const field=value["floats"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["floats"]=values;
}
{const field=value["strings"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["strings"]=values;
}
{const field=value["allow_failure"];
out["allow_failure"]=field;
}
{const field=value["assign_to_wp"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["assign_to_wp"]=values;
}
{const field=value["assign_on_start"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["assign_on_start"]=values;
}
{const field=value["assign_on_stop"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["assign_on_stop"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateTaskResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateTaskResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1fab04f16b13985ec3cffd38cace15e459fbb8de05d7283917df6ac0e7e30c8a",maxWireBytes:1457,minWireBytes:25,cartesianMaxWireBytes:1457n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsTaskCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateTaskResponseSize(Ros1Clearpath_mission_manager_msgsCreateTaskResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateTaskResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateTaskResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateTaskResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1457,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateTaskResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateTaskResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(value["result"]));
if(n>1457)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateTaskResponseWrite(value,w){
Ros1Clearpath_navigation_msgsTaskWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateTaskResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateTaskResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsTaskCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateWaypointRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0779feb60308617994f391072eba0fb734890defcf0b05974cf4a54485586188",maxWireBytes:1220,minWireBytes:52,cartesianMaxWireBytes:1220n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","latitude":0,"longitude":0,"heading":0,"position_tolerance":0,"yaw_tolerance":0,"task_ids":[],"assign_to":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateWaypointRequestSize(Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateWaypointRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateWaypointRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1220,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateWaypointRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateWaypointRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(typeof value["position_tolerance"]!=="number"||!Number.isFinite(value["position_tolerance"]))bad();n=addSize(n,8);
if(typeof value["yaw_tolerance"]!=="number"||!Number.isFinite(value["yaw_tolerance"]))bad();n=addSize(n,8);
if(!Array.isArray(value["task_ids"])||value["task_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["task_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["assign_to"])||value["assign_to"].length>2)bad();
n=addSize(n,4);
for(const element of value["assign_to"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>1220)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointRequestWrite(value,w){
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setFloat64(w.at,value["position_tolerance"],true);w.at+=8;
w.v.setFloat64(w.at,value["yaw_tolerance"],true);w.at+=8;
w.v.setUint32(w.at,value["task_ids"].length,true);w.at+=4;
for(const element of value["task_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["assign_to"].length,true);w.at+=4;
for(const element of value["assign_to"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsCreateWaypointRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position_tolerance"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["yaw_tolerance"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["task_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["task_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["assign_to"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["assign_to"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["position_tolerance"];
out["position_tolerance"]=field;
}
{const field=value["yaw_tolerance"];
out["yaw_tolerance"]=field;
}
{const field=value["task_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["task_ids"]=values;
}
{const field=value["assign_to"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["assign_to"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateWaypointResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ecf69fcfa5288295555651d9e8215b75b0b368a019447a7f50f25390eaf73a37",maxWireBytes:3350,minWireBytes:52,cartesianMaxWireBytes:3350n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateWaypointResponseSize(Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateWaypointResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateWaypointResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3350,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateWaypointResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateWaypointResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["result"]));
if(n>3350)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointResponseWrite(value,w){
Ros1Clearpath_navigation_msgsWaypointWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateWaypointResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateWaypointFromPoiRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_792f6ca7e821aa73a48aafcec57ba7b6c729115e7d2fbd4b4ac25412e1d76522",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"point_of_interest":Ros1Clearpath_navigation_msgsPointOfInterestCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestSize(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(value["point_of_interest"]));
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestWrite(value,w){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(value["point_of_interest"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["point_of_interest"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["point_of_interest"];
out["point_of_interest"]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/CreateWaypointFromPoiResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_39d1b867f9aa0a0fa17ceb11ab8b301a42a96c3156a97a8258d2a775853549ac",maxWireBytes:3350,minWireBytes:52,cartesianMaxWireBytes:3350n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseSize(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3350,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(n>3350)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseWrite(value,w){
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteByIdRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_489c2a214ef86b4b1b60e32ee19f71e11e7d000a3293ae747d48c20b746527f2",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteByIdRequestSize(Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteByIdRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteByIdRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteByIdRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteByIdRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteByIdRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsDeleteByIdRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteByIdResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a93b6bdc62702c29ed1db507504586a1b651ba32f98725326faf10b14e5abfcf",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ok":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteByIdResponseSize(Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteByIdResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteByIdResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteByIdResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteByIdResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ok"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteByIdResponseWrite(value,w){
w.v.setUint8(w.at,value["ok"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsDeleteByIdResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ok"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ok"];
out["ok"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteEverythingRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a0b2722e0ba476021308ba2d3dc2a6ef9843dbcf607c0e6ca18d8c4de60256f8",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"yes_i_am_absolutely_sure_i_want_to_do_this":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestSize(Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["yes_i_am_absolutely_sure_i_want_to_do_this"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestWrite(value,w){
w.v.setUint8(w.at,value["yes_i_am_absolutely_sure_i_want_to_do_this"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["yes_i_am_absolutely_sure_i_want_to_do_this"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["yes_i_am_absolutely_sure_i_want_to_do_this"];
out["yes_i_am_absolutely_sure_i_want_to_do_this"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteEverythingResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cd73a92e3a56214ea6fd445e5f7b20aab1a91945875774af4db25426f4a67a4d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ok":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseSize(Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ok"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseWrite(value,w){
w.v.setUint8(w.at,value["ok"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ok"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ok"];
out["ok"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteMultipleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_93d59bed5a06ad0d5d7c19f7c2881255ab70f4136e8d9633c6619e7060d2c2df",maxWireBytes:784,minWireBytes:8,cartesianMaxWireBytes:784n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuids":[],"parent_id":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestSize(Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,784,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["uuids"])||value["uuids"].length>2)bad();
n=addSize(n,4);
for(const element of value["uuids"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["parent_id"],256).length);
if(n>784)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestWrite(value,w){
w.v.setUint32(w.at,value["uuids"].length,true);w.at+=4;
for(const element of value["uuids"]){
w.text(element,256);
}
w.text(value["parent_id"],256);
}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["uuids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["uuids"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["parent_id"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["uuids"]=values;
}
{const field=value["parent_id"];
out["parent_id"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/DeleteMultipleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ddfebd2869fb25d1e6d51221ab483db5d1015d44607cfbf7d45fb2c28cd4cb60",maxWireBytes:524,minWireBytes:4,cartesianMaxWireBytes:524n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"failed_uuids":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseSize(Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,524,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["failed_uuids"])||value["failed_uuids"].length>2)bad();
n=addSize(n,4);
for(const element of value["failed_uuids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>524)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseWrite(value,w){
w.v.setUint32(w.at,value["failed_uuids"].length,true);w.at+=4;
for(const element of value["failed_uuids"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["failed_uuids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["failed_uuids"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["failed_uuids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["failed_uuids"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ExportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsExportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_273ca86bcaa10fda29a72290dd5b605569d6c59562e935e4779deee94249e5f4",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsExportDataRequestSize(Ros1Clearpath_mission_manager_msgsExportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsExportDataRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsExportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsExportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsExportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsExportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsExportDataRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsExportDataRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsExportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ExportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsExportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_497c67643bdb2dfb03f415e5af67a58b3dcfe439842441973fece1df28e56dbb",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsExportDataResponseSize(Ros1Clearpath_mission_manager_msgsExportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsExportDataResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsExportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsExportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsExportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsExportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsExportDataResponseWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_mission_manager_msgsExportDataResponseRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsExportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllMissionsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9b2758f137a20a5a8d0c798befe715f0d834c3fa912fddd5005a88c95340fb45",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestSize(Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllMissionsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b3936fa567f9f915b83d8933633c280b42684da025f096333fd133772642649b",maxWireBytes:26676,minWireBytes:4,cartesianMaxWireBytes:26676n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"missions":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseSize(Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26676,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["missions"])||value["missions"].length>2)bad();
n=addSize(n,4);
for(const element of value["missions"]){
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(element));
}
if(n>26676)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseWrite(value,w){
w.v.setUint32(w.at,value["missions"].length,true);w.at+=4;
for(const element of value["missions"]){
Ros1Clearpath_navigation_msgsMissionWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["missions"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["missions"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["missions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsMissionCapture(element);
}out["missions"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllNetworkMapsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b366fcf604486b90ff0de6d4cdf601f179509337b308b423d51a941857b964ed",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestSize(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllNetworkMapsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ac5719469bbfc8fbc6f0dd9a897cea857ecde83b65f7d2d6b19ac1b2c3905f3d",maxWireBytes:5220,minWireBytes:4,cartesianMaxWireBytes:5220n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"maps":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseSize(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5220,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["maps"])||value["maps"].length>2)bad();
n=addSize(n,4);
for(const element of value["maps"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(element));
}
if(n>5220)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseWrite(value,w){
w.v.setUint32(w.at,value["maps"].length,true);w.at+=4;
for(const element of value["maps"]){
Ros1Clearpath_navigation_msgsNetworkMapWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["maps"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["maps"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["maps"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkMapCapture(element);
}out["maps"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllNetworkMissionsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a1aed2728f1c66328d9c6f15d5866d67b379abfe7afd71b03c02ce69ef4fd11f",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestSize(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllNetworkMissionsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e0f273f3e31d98804e84ffe011f5060be89d009d790fb34992de6aaa346ef0e2",maxWireBytes:27216,minWireBytes:4,cartesianMaxWireBytes:27216n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"missions":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseSize(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,27216,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["missions"])||value["missions"].length>2)bad();
n=addSize(n,4);
for(const element of value["missions"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(element));
}
if(n>27216)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseWrite(value,w){
w.v.setUint32(w.at,value["missions"].length,true);w.at+=4;
for(const element of value["missions"]){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["missions"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["missions"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["missions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(element);
}out["missions"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllPointsOfInterestRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1774ba572ac91b4246b91b9e006829ff4f38a195ae67b23f5ed461574d72ccc9",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestSize(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllPointsOfInterestResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8105af95e78f446b3f73eb05379d4633513a51c29ea157adf3fb24ebed014fe4",maxWireBytes:2422,minWireBytes:4,cartesianMaxWireBytes:2422n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"points_of_interest":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseSize(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2422,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["points_of_interest"])||value["points_of_interest"].length>2)bad();
n=addSize(n,4);
for(const element of value["points_of_interest"]){
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(element));
}
if(n>2422)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseWrite(value,w){
w.v.setUint32(w.at,value["points_of_interest"].length,true);w.at+=4;
for(const element of value["points_of_interest"]){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["points_of_interest"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["points_of_interest"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["points_of_interest"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(element);
}out["points_of_interest"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllTasksRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e345153b30f5709bffaea462092527b89285ab0902c8b7c78f759f92d823c74e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllTasksRequestSize(Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllTasksRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllTasksRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllTasksRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllTasksRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllTasksRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllTasksRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllTasksResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_fed546c0b2b8ea2db1ca9e8e13e918144b436e4a2a818e724fadfe5a3775c148",maxWireBytes:2918,minWireBytes:4,cartesianMaxWireBytes:2918n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tasks":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllTasksResponseSize(Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllTasksResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllTasksResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2918,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllTasksResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllTasksResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["tasks"])||value["tasks"].length>2)bad();
n=addSize(n,4);
for(const element of value["tasks"]){
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(element));
}
if(n>2918)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllTasksResponseWrite(value,w){
w.v.setUint32(w.at,value["tasks"].length,true);w.at+=4;
for(const element of value["tasks"]){
Ros1Clearpath_navigation_msgsTaskWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllTasksResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["tasks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["tasks"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tasks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsTaskCapture(element);
}out["tasks"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllWaypointsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d5a22af162ba215c95bddbd955eac6aba558134ee4a5c988a13b6819fc5e22c7",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestSize(Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetAllWaypointsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_daacee9b652bc78820380eaf5d515c8813f8387fc7d2a3b934d7961fd1f46208",maxWireBytes:6704,minWireBytes:4,cartesianMaxWireBytes:6704n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"waypoints":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseSize(Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6704,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["waypoints"])||value["waypoints"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoints"]){
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(element));
}
if(n>6704)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseWrite(value,w){
w.v.setUint32(w.at,value["waypoints"].length,true);w.at+=4;
for(const element of value["waypoints"]){
Ros1Clearpath_navigation_msgsWaypointWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["waypoints"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoints"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["waypoints"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsWaypointCapture(element);
}out["waypoints"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetEverythingRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetEverythingRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6be79503b5553344dcbc97ef60843035d672883c3431f33b86a6e367bff35783",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetEverythingRequestSize(Ros1Clearpath_mission_manager_msgsGetEverythingRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetEverythingRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetEverythingRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetEverythingRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetEverythingRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetEverythingRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetEverythingRequestWrite(value,w){
}
function Ros1Clearpath_mission_manager_msgsGetEverythingRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_manager_msgsGetEverythingRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetEverythingResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetEverythingResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_135f1c7e2aea6353167ae048ae6afdc57fb6614c64a1a135321015734fc79069",maxWireBytes:32768,minWireBytes:24,cartesianMaxWireBytes:71156n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros1Clearpath_mission_manager_msgsStorageStateCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetEverythingResponseSize(Ros1Clearpath_mission_manager_msgsGetEverythingResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetEverythingResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetEverythingResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetEverythingResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32768,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetEverythingResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetEverythingResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_manager_msgsStorageStateSize(value["state"]));
if(n>32768)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetEverythingResponseWrite(value,w){
Ros1Clearpath_mission_manager_msgsStorageStateWrite(value["state"],w);
}
function Ros1Clearpath_mission_manager_msgsGetEverythingResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_manager_msgsStorageStateRead(r);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetEverythingResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros1Clearpath_mission_manager_msgsStorageStateCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c1587963d27ada2cb3d8fbb4ccad1faa2f7bdbe2cb88081b76e573843f51051f",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetMissionRequestSize(Ros1Clearpath_mission_manager_msgsGetMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetMissionRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ed24abebf6a3316c8583a2d6d9e62f7eca8ea493607c7772130b057468aa483f",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetMissionResponseSize(Ros1Clearpath_mission_manager_msgsGetMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["mission"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsGetMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_67930348022f20b977b129f0a583130b9664cf9c45d26bcfaf5a5af087e185b7",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0a25742662a313fe30697c86f1463304d54a7bfc06e86b6e6a2c9881c09dc775",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["map"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["map"],w);
}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["map"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map"];
out["map"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetNetworkMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a69493f449f860b33c6c6c510b7a23b015ce5c3566ac13aae7d2d42b4a0652b7",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestSize(Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetNetworkMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9ec509115489f3011105522578f549180057312e652eb9bf1a1c3f1dbee2025a",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseSize(Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["mission"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetPointOfInterestRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a6b60dbc44cf3fdfc7deea9ce5d8826b9bfb17cfe3bdf6e5a82ae485d51fa9a7",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestSize(Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetPointOfInterestResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_711a556a81c17e244578495cac6e6721c5e7b81dadf505784ea191cc1f0b9a13",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"point_of_interest":Ros1Clearpath_navigation_msgsPointOfInterestCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseSize(Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(value["point_of_interest"]));
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseWrite(value,w){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(value["point_of_interest"],w);
}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["point_of_interest"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["point_of_interest"];
out["point_of_interest"]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetTaskRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetTaskRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_854685ce6f5dc28caf2c8e11d82813aa8f0611945d5c76d29bb2ca11461a94dc",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetTaskRequestSize(Ros1Clearpath_mission_manager_msgsGetTaskRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetTaskRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetTaskRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetTaskRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetTaskRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetTaskRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetTaskRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetTaskRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetTaskRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetTaskResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetTaskResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7ddc2f1b761f344a7765b4f54d9a15a91a4097e715f0610c741fc2a16dba947f",maxWireBytes:1457,minWireBytes:25,cartesianMaxWireBytes:1457n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":Ros1Clearpath_navigation_msgsTaskCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetTaskResponseSize(Ros1Clearpath_mission_manager_msgsGetTaskResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetTaskResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetTaskResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetTaskResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1457,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetTaskResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetTaskResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(value["task"]));
if(n>1457)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetTaskResponseWrite(value,w){
Ros1Clearpath_navigation_msgsTaskWrite(value["task"],w);
}
function Ros1Clearpath_mission_manager_msgsGetTaskResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["task"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetTaskResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=Ros1Clearpath_navigation_msgsTaskCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetWaypointRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetWaypointRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d3530712135cd3a80f746e233124cb3e79ef8d8ef69ca6fe1f26c750a3314539",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetWaypointRequestSize(Ros1Clearpath_mission_manager_msgsGetWaypointRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetWaypointRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsGetWaypointRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetWaypointRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetWaypointRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetWaypointRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetWaypointRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsGetWaypointRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetWaypointRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/GetWaypointResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsGetWaypointResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8226d0597a337e9b43babecca8bf4ba09274c53802cd87ece58feea9c0954c66",maxWireBytes:3350,minWireBytes:52,cartesianMaxWireBytes:3350n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"waypoint":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsGetWaypointResponseSize(Ros1Clearpath_mission_manager_msgsGetWaypointResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsGetWaypointResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsGetWaypointResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsGetWaypointResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3350,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsGetWaypointResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsGetWaypointResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["waypoint"]));
if(n>3350)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsGetWaypointResponseWrite(value,w){
Ros1Clearpath_navigation_msgsWaypointWrite(value["waypoint"],w);
}
function Ros1Clearpath_mission_manager_msgsGetWaypointResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["waypoint"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsGetWaypointResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["waypoint"];
out["waypoint"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5e0a54e52dea50a99d18f8e82ded58dafd203c692fd467c0123e0fb057e37d25",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportDataRequestSize(Ros1Clearpath_mission_manager_msgsImportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportDataRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsImportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportDataRequestWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_mission_manager_msgsImportDataRequestRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_16cd6e810a8c986454949c165ec577118481f2557bf2d1620fc098108b806488",maxWireBytes:32768,minWireBytes:24,cartesianMaxWireBytes:71156n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros1Clearpath_mission_manager_msgsStorageStateCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportDataResponseSize(Ros1Clearpath_mission_manager_msgsImportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportDataResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsImportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32768,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_manager_msgsStorageStateSize(value["state"]));
if(n>32768)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportDataResponseWrite(value,w){
Ros1Clearpath_mission_manager_msgsStorageStateWrite(value["state"],w);
}
function Ros1Clearpath_mission_manager_msgsImportDataResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_manager_msgsStorageStateRead(r);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros1Clearpath_mission_manager_msgsStorageStateCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4eb653a68629b1c4461091d9ab12f90d26ed5e8b063d13b3cbbe1a2944f3fb53",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportMissionRequestSize(Ros1Clearpath_mission_manager_msgsImportMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsImportMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["mission"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportMissionRequestWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsImportMissionRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8881f8b70cb092d74936eb93345036049e07cc11cd5c55e52b2b3f7f36d07e7d",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportMissionResponseSize(Ros1Clearpath_mission_manager_msgsImportMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsImportMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["mission"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsImportMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ca063451548e0a85552a3069cccf7b86524d6cb30c5dd6c788d44ba0fc508978",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["map"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["map"],w);
}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["map"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map"];
out["map"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b53c8f06e58b7dde2c5127df98451db37c1825fe667cf8eb816c0cacf93ecf3f",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["map"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["map"],w);
}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["map"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map"];
out["map"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportNetworkMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_03140f9073798b8de77e3ba9b8fc8f048b0ab43fa5d17c17bb767c23dfcf1a39",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestSize(Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["mission"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ImportNetworkMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2f4b00981cea86beab4c6872e8184ad90257612396bb44d0a120290b8404c7d8",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mission":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseSize(Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["mission"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["mission"],w);
}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["mission"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mission"];
out["mission"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/SplitNetworkEdgeRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6d083d95fc9720932b330979fdf997abeaa7e463265226bf7ae09fec1bb97be5",maxWireBytes:537,minWireBytes:25,cartesianMaxWireBytes:537n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","edge_id":"","new_point_latitude":0,"new_point_longitude":0,"symmetrical":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestSize(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,537,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
n=addSize(n,4+textBytes(value["edge_id"],256).length);
if(typeof value["new_point_latitude"]!=="number"||!Number.isFinite(value["new_point_latitude"]))bad();n=addSize(n,8);
if(typeof value["new_point_longitude"]!=="number"||!Number.isFinite(value["new_point_longitude"]))bad();n=addSize(n,8);
if(typeof value["symmetrical"]!=="boolean")bad();n=addSize(n,1);
if(n>537)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestWrite(value,w){
w.text(value["map_id"],256);
w.text(value["edge_id"],256);
w.v.setFloat64(w.at,value["new_point_latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["new_point_longitude"],true);w.at+=8;
w.v.setUint8(w.at,value["symmetrical"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
const decoded=r.text(256);
value["edge_id"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["new_point_latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["new_point_longitude"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["symmetrical"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["edge_id"];
out["edge_id"]=field;
}
{const field=value["new_point_latitude"];
out["new_point_latitude"]=field;
}
{const field=value["new_point_longitude"];
out["new_point_longitude"]=field;
}
{const field=value["symmetrical"];
out["symmetrical"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/SplitNetworkEdgeResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_55fdb27f06984817af8a20e3e0bcc8dd85c33db0fbc65f1881822eabc6ab14b3",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseSize(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["result"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3320e01dc2228359832eb75a54c3df011870d017f0f8f862fe2a68d2cb56a3d3",maxWireBytes:2224,minWireBytes:24,cartesianMaxWireBytes:2224n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","onav_config":"","waypoint_ids":[],"on_start_ids":[],"on_stop_ids":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateMissionRequestSize(Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2224,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(!Array.isArray(value["waypoint_ids"])||value["waypoint_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoint_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_start_ids"])||value["on_start_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_stop_ids"])||value["on_stop_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>2224)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateMissionRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.text(value["onav_config"],256);
w.v.setUint32(w.at,value["waypoint_ids"].length,true);w.at+=4;
for(const element of value["waypoint_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_start_ids"].length,true);w.at+=4;
for(const element of value["on_start_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_stop_ids"].length,true);w.at+=4;
for(const element of value["on_stop_ids"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsUpdateMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoint_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["waypoint_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_start_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_stop_ids"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
{const field=value["waypoint_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["waypoint_ids"]=values;
}
{const field=value["on_start_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_start_ids"]=values;
}
{const field=value["on_stop_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_stop_ids"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1d43fee27013957863e01990de9010ebb6b8a7a8668cf6b3916835aaa92e18a9",maxWireBytes:13336,minWireBytes:40,cartesianMaxWireBytes:13336n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateMissionResponseSize(Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13336,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsMissionSize(value["result"]));
if(n>13336)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsMissionWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsMissionRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkEdgeRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6f6eb31133df261e82cf165b1e93ae4d853c4a9de1a928a612ac35115d0bd563",maxWireBytes:1052,minWireBytes:28,cartesianMaxWireBytes:1052n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","uuid":"","start_point_id":"","end_point_id":"","radius":0,"speed_limit":0,"cost_factor":0};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1052,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["start_point_id"],256).length);
n=addSize(n,4+textBytes(value["end_point_id"],256).length);
if(typeof value["radius"]!=="number"||!Number.isFinite(value["radius"])||!Number.isFinite(Math.fround(value["radius"])))bad();n=addSize(n,4);
if(typeof value["speed_limit"]!=="number"||!Number.isFinite(value["speed_limit"])||!Number.isFinite(Math.fround(value["speed_limit"])))bad();n=addSize(n,4);
if(typeof value["cost_factor"]!=="number"||!Number.isFinite(value["cost_factor"])||!Number.isFinite(Math.fround(value["cost_factor"])))bad();n=addSize(n,4);
if(n>1052)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestWrite(value,w){
w.text(value["map_id"],256);
w.text(value["uuid"],256);
w.text(value["start_point_id"],256);
w.text(value["end_point_id"],256);
w.v.setFloat32(w.at,value["radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["speed_limit"],true);w.at+=4;
w.v.setFloat32(w.at,value["cost_factor"],true);w.at+=4;
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["start_point_id"]=decoded;
}
{
const decoded=r.text(256);
value["end_point_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_limit"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cost_factor"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["start_point_id"];
out["start_point_id"]=field;
}
{const field=value["end_point_id"];
out["end_point_id"]=field;
}
{const field=value["radius"];
out["radius"]=field;
}
{const field=value["speed_limit"];
out["speed_limit"]=field;
}
{const field=value["cost_factor"];
out["cost_factor"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkEdgeResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4534230375b42cc0956c57d587baac63882da426edfe851fccc4b95a42d59c01",maxWireBytes:824,minWireBytes:56,cartesianMaxWireBytes:824n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkEdgeCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,824,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkEdgeSize(value["result"]));
if(n>824)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkEdgeWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkEdgeRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkEdgeCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkEdgesRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9126cc96d7840c27a84dbb4144580cb9a76a7c77aa1fefea23b6b965e6d2d7f4",maxWireBytes:820,minWireBytes:20,cartesianMaxWireBytes:820n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","uuids":[],"radii":[],"speed_limits":[],"cost_factors":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,820,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
if(!Array.isArray(value["uuids"])||value["uuids"].length>2)bad();
n=addSize(n,4);
for(const element of value["uuids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["radii"])||value["radii"].length>2)bad();
n=addSize(n,4);
for(const element of value["radii"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["speed_limits"])||value["speed_limits"].length>2)bad();
n=addSize(n,4);
for(const element of value["speed_limits"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["cost_factors"])||value["cost_factors"].length>2)bad();
n=addSize(n,4);
for(const element of value["cost_factors"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>820)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestWrite(value,w){
w.text(value["map_id"],256);
w.v.setUint32(w.at,value["uuids"].length,true);w.at+=4;
for(const element of value["uuids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["radii"].length,true);w.at+=4;
for(const element of value["radii"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["speed_limits"].length,true);w.at+=4;
for(const element of value["speed_limits"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["cost_factors"].length,true);w.at+=4;
for(const element of value["cost_factors"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["uuids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["uuids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["radii"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["radii"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["speed_limits"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_limits"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["cost_factors"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cost_factors"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["uuids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["uuids"]=values;
}
{const field=value["radii"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["radii"]=values;
}
{const field=value["speed_limits"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["speed_limits"]=values;
}
{const field=value["cost_factors"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cost_factors"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkEdgesResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0a5fee23a1ef3a75299f92c499ef73bc22f39f15932a9c383d7406445cb89b0c",maxWireBytes:1652,minWireBytes:4,cartesianMaxWireBytes:1652n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1652,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["result"])||value["result"].length>2)bad();
n=addSize(n,4);
for(const element of value["result"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkEdgeSize(element));
}
if(n>1652)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseWrite(value,w){
w.v.setUint32(w.at,value["result"].length,true);w.at+=4;
for(const element of value["result"]){
Ros1Clearpath_navigation_msgsNetworkEdgeWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["result"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkEdgeRead(r);
value["result"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkEdgeCapture(element);
}out["result"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_527641cfaf5b2283be483c5fceb31d82cdcbe01dd9199343033b910c1864e012",maxWireBytes:400,minWireBytes:16,cartesianMaxWireBytes:400n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","default_radius":0,"default_speed_limit":0};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,400,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["default_radius"]!=="number"||!Number.isFinite(value["default_radius"])||!Number.isFinite(Math.fround(value["default_radius"])))bad();n=addSize(n,4);
if(typeof value["default_speed_limit"]!=="number"||!Number.isFinite(value["default_speed_limit"])||!Number.isFinite(Math.fround(value["default_speed_limit"])))bad();n=addSize(n,4);
if(n>400)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat32(w.at,value["default_radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["default_speed_limit"],true);w.at+=4;
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["default_speed_limit"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["default_radius"];
out["default_radius"]=field;
}
{const field=value["default_speed_limit"];
out["default_speed_limit"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1ea078b7625f7a8593ddbd8df3f158972cfcc973bb956f041d36e66734a03924",maxWireBytes:2608,minWireBytes:24,cartesianMaxWireBytes:2608n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkMapCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2608,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMapSize(value["result"]));
if(n>2608)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMapWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMapRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkMapCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkMissionRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e3f246a1753021aee8ab1d6a7792a878fda832ee63d797b52ecabff917a41d95",maxWireBytes:2494,minWireBytes:38,cartesianMaxWireBytes:2494n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","onav_config":"","waypoint_ids":[],"on_start_ids":[],"on_stop_ids":[],"return_to_dock_enabled":false,"return_to_dock_id":"","return_to_dock_battery_level":0,"resume_mission_battery_level":0,"network_replan_enabled":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2494,90);if(owned.length<38)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["onav_config"],256).length);
if(!Array.isArray(value["waypoint_ids"])||value["waypoint_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["waypoint_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_start_ids"])||value["on_start_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_start_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["on_stop_ids"])||value["on_stop_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["on_stop_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["return_to_dock_enabled"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["return_to_dock_id"],256).length);
if(typeof value["return_to_dock_battery_level"]!=="number"||!Number.isFinite(value["return_to_dock_battery_level"])||!Number.isFinite(Math.fround(value["return_to_dock_battery_level"])))bad();n=addSize(n,4);
if(typeof value["resume_mission_battery_level"]!=="number"||!Number.isFinite(value["resume_mission_battery_level"])||!Number.isFinite(Math.fround(value["resume_mission_battery_level"])))bad();n=addSize(n,4);
if(typeof value["network_replan_enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>2494)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.text(value["onav_config"],256);
w.v.setUint32(w.at,value["waypoint_ids"].length,true);w.at+=4;
for(const element of value["waypoint_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_start_ids"].length,true);w.at+=4;
for(const element of value["on_start_ids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["on_stop_ids"].length,true);w.at+=4;
for(const element of value["on_stop_ids"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["return_to_dock_enabled"]?1:0,true);w.at+=1;
w.text(value["return_to_dock_id"],256);
w.v.setFloat32(w.at,value["return_to_dock_battery_level"],true);w.at+=4;
w.v.setFloat32(w.at,value["resume_mission_battery_level"],true);w.at+=4;
w.v.setUint8(w.at,value["network_replan_enabled"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["onav_config"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["waypoint_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["waypoint_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_start_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_start_ids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["on_stop_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["on_stop_ids"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["return_to_dock_enabled"]=decoded!==0;
}
{
const decoded=r.text(256);
value["return_to_dock_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["return_to_dock_battery_level"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["resume_mission_battery_level"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["network_replan_enabled"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["onav_config"];
out["onav_config"]=field;
}
{const field=value["waypoint_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["waypoint_ids"]=values;
}
{const field=value["on_start_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_start_ids"]=values;
}
{const field=value["on_stop_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["on_stop_ids"]=values;
}
{const field=value["return_to_dock_enabled"];
out["return_to_dock_enabled"]=field;
}
{const field=value["return_to_dock_id"];
out["return_to_dock_id"]=field;
}
{const field=value["return_to_dock_battery_level"];
out["return_to_dock_battery_level"]=field;
}
{const field=value["resume_mission_battery_level"];
out["resume_mission_battery_level"]=field;
}
{const field=value["network_replan_enabled"];
out["network_replan_enabled"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkMissionResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_223f3be41c5e6b365f0aed33952e2b53619fdb700b6249cd79cdb835a66317f3",maxWireBytes:13606,minWireBytes:54,cartesianMaxWireBytes:13606n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkMissionCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13606,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkMissionSize(value["result"]));
if(n>13606)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkMissionWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkMissionRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkMissionCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkPointRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ced6a1449a9a8f8c92cec59f865adbb3563e6840d992c977ca124e018668fd85",maxWireBytes:536,minWireBytes:24,cartesianMaxWireBytes:536n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","uuid":"","latitude":0,"longitude":0};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,536,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(n>536)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestWrite(value,w){
w.text(value["map_id"],256);
w.text(value["uuid"],256);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkPointResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7015a36b6124bdbb162c38c33a1a107431c3b2050cdd48d33707d4a27317d2ee",maxWireBytes:276,minWireBytes:20,cartesianMaxWireBytes:276n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsNetworkPointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,276,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(value["result"]));
if(n>276)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseWrite(value,w){
Ros1Clearpath_navigation_msgsNetworkPointWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsNetworkPointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkPointsRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cd684b3f95616f37e2620228a24e41609f3c102f286797d0d49069ad6f8fb7d9",maxWireBytes:824,minWireBytes:16,cartesianMaxWireBytes:824n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"map_id":"","uuids":[],"latitudes":[],"longitudes":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,824,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["map_id"],256).length);
if(!Array.isArray(value["uuids"])||value["uuids"].length>2)bad();
n=addSize(n,4);
for(const element of value["uuids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(!Array.isArray(value["latitudes"])||value["latitudes"].length>2)bad();
n=addSize(n,4);
for(const element of value["latitudes"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["longitudes"])||value["longitudes"].length>2)bad();
n=addSize(n,4);
for(const element of value["longitudes"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>824)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestWrite(value,w){
w.text(value["map_id"],256);
w.v.setUint32(w.at,value["uuids"].length,true);w.at+=4;
for(const element of value["uuids"]){
w.text(element,256);
}
w.v.setUint32(w.at,value["latitudes"].length,true);w.at+=4;
for(const element of value["latitudes"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["longitudes"].length,true);w.at+=4;
for(const element of value["longitudes"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestRead(r){const value={};
{
const decoded=r.text(256);
value["map_id"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["uuids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["uuids"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["latitudes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitudes"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["longitudes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitudes"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["map_id"];
out["map_id"]=field;
}
{const field=value["uuids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["uuids"]=values;
}
{const field=value["latitudes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["latitudes"]=values;
}
{const field=value["longitudes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["longitudes"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateNetworkPointsResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_867e62ceec9cffd73f6bc697994998578c0d848b489c7fb1536ff29cc2ed740d",maxWireBytes:556,minWireBytes:4,cartesianMaxWireBytes:556n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseSize(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,556,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["result"])||value["result"].length>2)bad();
n=addSize(n,4);
for(const element of value["result"]){
n=addSize(n,Ros1Clearpath_navigation_msgsNetworkPointSize(element));
}
if(n>556)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseWrite(value,w){
w.v.setUint32(w.at,value["result"].length,true);w.at+=4;
for(const element of value["result"]){
Ros1Clearpath_navigation_msgsNetworkPointWrite(element,w);
}
}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["result"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_navigation_msgsNetworkPointRead(r);
value["result"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_navigation_msgsNetworkPointCapture(element);
}out["result"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdatePointOfInterestRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e1dc3af1f989ce0787973a31ea9f5fc4e4be8c2a23cc73e258ceb5f5e503ff51",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","latitude":0,"longitude":0,"elevation":0,"enable_heading":false,"heading":0,"tags":[],"custom_fields_json":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestSize(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["elevation"]!=="number"||!Number.isFinite(value["elevation"]))bad();n=addSize(n,8);
if(typeof value["enable_heading"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(!Array.isArray(value["tags"])||value["tags"].length>2)bad();
n=addSize(n,4);
for(const element of value["tags"]){
n=addSize(n,4+textBytes(element,256).length);
}
n=addSize(n,4+textBytes(value["custom_fields_json"],256).length);
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["elevation"],true);w.at+=8;
w.v.setUint8(w.at,value["enable_heading"]?1:0,true);w.at+=1;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setUint32(w.at,value["tags"].length,true);w.at+=4;
for(const element of value["tags"]){
w.text(element,256);
}
w.text(value["custom_fields_json"],256);
}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elevation"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_heading"]=decoded!==0;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["tags"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["tags"].push(decoded);
}}
}
{
const decoded=r.text(256);
value["custom_fields_json"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["elevation"];
out["elevation"]=field;
}
{const field=value["enable_heading"];
out["enable_heading"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["tags"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tags"]=values;
}
{const field=value["custom_fields_json"];
out["custom_fields_json"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdatePointOfInterestResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4bfcd0f85d450d766dc190745d8abdddb2320a4b8990e02cf1fce80aff304e05",maxWireBytes:1209,minWireBytes:49,cartesianMaxWireBytes:1209n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsPointOfInterestCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseSize(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1209,90);if(owned.length<49)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsPointOfInterestSize(value["result"]));
if(n>1209)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseWrite(value,w){
Ros1Clearpath_navigation_msgsPointOfInterestWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsPointOfInterestRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsPointOfInterestCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateTaskRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a5844fedb72c67d92446e533cd49d4e18fb1cec1aecc522646fc0fb5834fde15",maxWireBytes:1457,minWireBytes:25,cartesianMaxWireBytes:1457n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","action_server_name":"","version":"","floats":[],"strings":[],"allow_failure":false};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateTaskRequestSize(Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateTaskRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateTaskRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1457,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateTaskRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateTaskRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,4+textBytes(value["action_server_name"],256).length);
n=addSize(n,4+textBytes(value["version"],256).length);
if(!Array.isArray(value["floats"])||value["floats"].length>2)bad();
n=addSize(n,4);
for(const element of value["floats"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["strings"])||value["strings"].length>2)bad();
n=addSize(n,4);
for(const element of value["strings"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["allow_failure"]!=="boolean")bad();n=addSize(n,1);
if(n>1457)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateTaskRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.text(value["action_server_name"],256);
w.text(value["version"],256);
w.v.setUint32(w.at,value["floats"].length,true);w.at+=4;
for(const element of value["floats"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["strings"].length,true);w.at+=4;
for(const element of value["strings"]){
w.text(element,256);
}
w.v.setUint8(w.at,value["allow_failure"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_manager_msgsUpdateTaskRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.text(256);
value["action_server_name"]=decoded;
}
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["floats"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["floats"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["strings"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["strings"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["allow_failure"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["action_server_name"];
out["action_server_name"]=field;
}
{const field=value["version"];
out["version"]=field;
}
{const field=value["floats"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["floats"]=values;
}
{const field=value["strings"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["strings"]=values;
}
{const field=value["allow_failure"];
out["allow_failure"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateTaskResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9e2be799a216403d720d9aac572b8faf20a4cb81abd299fa6f845798cb64e86d",maxWireBytes:1457,minWireBytes:25,cartesianMaxWireBytes:1457n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsTaskCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateTaskResponseSize(Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateTaskResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateTaskResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1457,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateTaskResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateTaskResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsTaskSize(value["result"]));
if(n>1457)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateTaskResponseWrite(value,w){
Ros1Clearpath_navigation_msgsTaskWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateTaskResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsTaskRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsTaskCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateWaypointRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_79fb82878e0bc290e0975c281962165c5ea99dd8cf550607896633a9a2a3215a",maxWireBytes:956,minWireBytes:52,cartesianMaxWireBytes:956n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","latitude":0,"longitude":0,"heading":0,"position_tolerance":0,"yaw_tolerance":0,"task_ids":[]};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestSize(Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,956,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
if(typeof value["latitude"]!=="number"||!Number.isFinite(value["latitude"]))bad();n=addSize(n,8);
if(typeof value["longitude"]!=="number"||!Number.isFinite(value["longitude"]))bad();n=addSize(n,8);
if(typeof value["heading"]!=="number"||!Number.isFinite(value["heading"]))bad();n=addSize(n,8);
if(typeof value["position_tolerance"]!=="number"||!Number.isFinite(value["position_tolerance"]))bad();n=addSize(n,8);
if(typeof value["yaw_tolerance"]!=="number"||!Number.isFinite(value["yaw_tolerance"]))bad();n=addSize(n,8);
if(!Array.isArray(value["task_ids"])||value["task_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["task_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>956)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
w.v.setFloat64(w.at,value["latitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["longitude"],true);w.at+=8;
w.v.setFloat64(w.at,value["heading"],true);w.at+=8;
w.v.setFloat64(w.at,value["position_tolerance"],true);w.at+=8;
w.v.setFloat64(w.at,value["yaw_tolerance"],true);w.at+=8;
w.v.setUint32(w.at,value["task_ids"].length,true);w.at+=4;
for(const element of value["task_ids"]){
w.text(element,256);
}
}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["latitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["longitude"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["heading"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position_tolerance"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["yaw_tolerance"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["task_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["task_ids"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["latitude"];
out["latitude"]=field;
}
{const field=value["longitude"];
out["longitude"]=field;
}
{const field=value["heading"];
out["heading"]=field;
}
{const field=value["position_tolerance"];
out["position_tolerance"]=field;
}
{const field=value["yaw_tolerance"];
out["yaw_tolerance"]=field;
}
{const field=value["task_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["task_ids"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/UpdateWaypointResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7488513078df8a3a9b1b3bfcca432aee912b3f7bf028f9d772c67f91e26c850c",maxWireBytes:3350,minWireBytes:52,cartesianMaxWireBytes:3350n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_navigation_msgsWaypointCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseSize(Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3350,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_navigation_msgsWaypointSize(value["result"]));
if(n>3350)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseWrite(value,w){
Ros1Clearpath_navigation_msgsWaypointWrite(value["result"],w);
}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_navigation_msgsWaypointRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_navigation_msgsWaypointCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ValidateNetworkMapRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6924bb2508845236c9ef6c5ef4d71881eed30960550df58105c3b9e91fa45041",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestSize(Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCapture(value),n=Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_manager_msgs/ValidateNetworkMapResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c213df6efac189646a82ff69eb42d36321e72356308da44743cce592910c4bd8",maxWireBytes:790,minWireBytes:14,cartesianMaxWireBytes:790n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros1Clearpath_mission_manager_msgsNetworkMapStateCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseSize(Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCapture(value),n=Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,790,90);if(owned.length<14)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_manager_msgsNetworkMapStateSize(value["state"]));
if(n>790)bad(90);return n;}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseWrite(value,w){
Ros1Clearpath_mission_manager_msgsNetworkMapStateWrite(value["state"],w);
}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_manager_msgsNetworkMapStateRead(r);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros1Clearpath_mission_manager_msgsNetworkMapStateCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/CloneScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8387bc2882c59ffb6124127d6a5dd54bc7e2685fd5a0aa3453389612e50b3216",maxWireBytes:520,minWireBytes:8,cartesianMaxWireBytes:520n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","new_name":""};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,520,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["new_name"],256).length);
if(n>520)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["new_name"],256);
}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(256);
value["new_name"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["new_name"];
out["new_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/CloneScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2245dcf9c323b09f914e5ff952fadd117c5ae6a3c62e31d2b7faa4b723be27a0",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"schedule":Ros1Clearpath_mission_scheduler_msgsScheduleCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(value["schedule"]));
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseWrite(value,w){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(value["schedule"],w);
}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["schedule"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["schedule"];
out["schedule"]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/CreateScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5c2188fde6641a0d1d38f8c03f09f2feef33a0c6938310fb2233adef32719b53",maxWireBytes:684,minWireBytes:36,cartesianMaxWireBytes:684n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","start_time":Ros1Clearpath_mission_scheduler_msgsUtcTimeCodec.defaultValue(),"mode":0,"loop_repeats":0,"loop_interval":Ros1DurationCodec.defaultValue(),"mission_ids":[],"min_battery":0,"timeout":Ros1DurationCodec.defaultValue(),"enabled":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,684,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(value["start_time"]));
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["loop_repeats"],1,false);n=addSize(n,1);
n=addSize(n,Ros1DurationSize(value["loop_interval"]));
if(!Array.isArray(value["mission_ids"])||value["mission_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["mission_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["min_battery"]!=="number"||!Number.isFinite(value["min_battery"])||!Number.isFinite(Math.fround(value["min_battery"])))bad();n=addSize(n,4);
n=addSize(n,Ros1DurationSize(value["timeout"]));
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>684)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestWrite(value,w){
w.text(value["name"],128);
Ros1Clearpath_mission_scheduler_msgsUtcTimeWrite(value["start_time"],w);
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["loop_repeats"],true);w.at+=1;
Ros1DurationWrite(value["loop_interval"],w);
w.v.setUint32(w.at,value["mission_ids"].length,true);w.at+=4;
for(const element of value["mission_ids"]){
w.text(element,256);
}
w.v.setFloat32(w.at,value["min_battery"],true);w.at+=4;
Ros1DurationWrite(value["timeout"],w);
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=Ros1Clearpath_mission_scheduler_msgsUtcTimeRead(r);
value["start_time"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["loop_repeats"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["loop_interval"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["mission_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["mission_ids"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["min_battery"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["timeout"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["start_time"];
out["start_time"]=Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(field);
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["loop_repeats"];
out["loop_repeats"]=field;
}
{const field=value["loop_interval"];
out["loop_interval"]=Ros1DurationCapture(field);
}
{const field=value["mission_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mission_ids"]=values;
}
{const field=value["min_battery"];
out["min_battery"]=field;
}
{const field=value["timeout"];
out["timeout"]=Ros1DurationCapture(field);
}
{const field=value["enabled"];
out["enabled"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/CreateScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9859bffd0fb72f93e6692c733cf9160af07d648b317f8bf9fdc34f6a7dc7b24c",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_mission_scheduler_msgsScheduleCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(value["result"]));
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseWrite(value,w){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(value["result"],w);
}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/EnableScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_26ad79f2ef123281877514e3bc65268a180f01441229ccc08c907bc76f16e953",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","enable":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(typeof value["enable"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestWrite(value,w){
w.text(value["uuid"],256);
w.v.setUint8(w.at,value["enable"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["enable"];
out["enable"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/EnableScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4ca5afdf4ac83e2d03d94b02e3bd156bfaec1835922a6f3d719ea700251324b2",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ok":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ok"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseWrite(value,w){
w.v.setUint8(w.at,value["ok"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ok"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ok"];
out["ok"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/ExportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsExportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8ad06898b5c892ea73d57aa1db2f7b8b0a8e435d05c8a614c6d76d08ac598c54",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsExportDataRequestSize(Ros1Clearpath_mission_scheduler_msgsExportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsExportDataRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsExportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsExportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsExportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsExportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsExportDataRequestWrite(value,w){
}
function Ros1Clearpath_mission_scheduler_msgsExportDataRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_scheduler_msgsExportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/ExportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsExportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b3ee9c9b3c93a5e1c1df96c51d33fc883713fc6dc1657e6c9c8a567be368847b",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsExportDataResponseSize(Ros1Clearpath_mission_scheduler_msgsExportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsExportDataResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsExportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsExportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsExportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsExportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsExportDataResponseWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_mission_scheduler_msgsExportDataResponseRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsExportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetAllSchedulesRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cdf67fd2aa2ebffb723416c55f3aff5700dfad53b2464ba446fd115342da54c1",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestSize(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestWrite(value,w){
}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetAllSchedulesResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_79b76d2badd9a5a0ab76421b3f992b0c90552d8c879c2a4727966ca749014834",maxWireBytes:1892,minWireBytes:4,cartesianMaxWireBytes:1892n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"schedules":[]};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseSize(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1892,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["schedules"])||value["schedules"].length>2)bad();
n=addSize(n,4);
for(const element of value["schedules"]){
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(element));
}
if(n>1892)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseWrite(value,w){
w.v.setUint32(w.at,value["schedules"].length,true);w.at+=4;
for(const element of value["schedules"]){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(element,w);
}
}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["schedules"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["schedules"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["schedules"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(element);
}out["schedules"]=values;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetNextScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3c10499a950f4c7431e738bf8e1d56a6407f06eec6b570b10ef5532f67ad0e31",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestWrite(value,w){
}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestRead(r){const value={};
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetNextScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_eb0d25c6d02bf163d55bf197402c0818a96089948b79be7747bdb4a9d97089fa",maxWireBytes:268,minWireBytes:12,cartesianMaxWireBytes:268n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","time_to_start":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,268,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,Ros1DurationSize(value["time_to_start"]));
if(n>268)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseWrite(value,w){
w.text(value["uuid"],256);
Ros1DurationWrite(value["time_to_start"],w);
}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["time_to_start"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["time_to_start"];
out["time_to_start"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_631d2966a939673a63824e4aab11b1dc1b7f6df6446e64307151b9826264800a",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":""};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestWrite(value,w){
w.text(value["uuid"],256);
}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/GetScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c731f817524bcf55c9efd68faeb390dc88a160f2efb2fa472d81a8aaff2cd970",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"schedule":Ros1Clearpath_mission_scheduler_msgsScheduleCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(value["schedule"]));
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseWrite(value,w){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(value["schedule"],w);
}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["schedule"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["schedule"];
out["schedule"]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/ImportDataRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsImportDataRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7c1b0e75e3d4dacd5f9fc3d82b7ddab8629686215f1c52d7be83c802a1316561",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"data":""};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsImportDataRequestSize(Ros1Clearpath_mission_scheduler_msgsImportDataRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsImportDataRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsImportDataRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsImportDataRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsImportDataRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsImportDataRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["data"],256).length);
if(n>260)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsImportDataRequestWrite(value,w){
w.text(value["data"],256);
}
function Ros1Clearpath_mission_scheduler_msgsImportDataRequestRead(r){const value={};
{
const decoded=r.text(256);
value["data"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsImportDataRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["data"];
out["data"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/ImportDataResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsImportDataResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2a5e62341836cc9171f7c292463a116fc84ddb7b2a8600107e3c5fb2ce5a004f",maxWireBytes:1892,minWireBytes:4,cartesianMaxWireBytes:1892n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":Ros1Clearpath_mission_scheduler_msgsStorageStateCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsImportDataResponseSize(Ros1Clearpath_mission_scheduler_msgsImportDataResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsImportDataResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsImportDataResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsImportDataResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1892,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsImportDataResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsImportDataResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsStorageStateSize(value["state"]));
if(n>1892)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsImportDataResponseWrite(value,w){
Ros1Clearpath_mission_scheduler_msgsStorageStateWrite(value["state"],w);
}
function Ros1Clearpath_mission_scheduler_msgsImportDataResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_scheduler_msgsStorageStateRead(r);
value["state"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsImportDataResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=Ros1Clearpath_mission_scheduler_msgsStorageStateCapture(field);
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/UpdateScheduleRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_24baafba1e1af2d47b61f21aa7f2c5795f298477a90181b97e3d942f8da25b8f",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","name":"","start_time":Ros1Clearpath_mission_scheduler_msgsUtcTimeCodec.defaultValue(),"mode":0,"loop_repeats":0,"loop_interval":Ros1DurationCodec.defaultValue(),"mission_ids":[],"min_battery":0,"timeout":Ros1DurationCodec.defaultValue(),"enabled":false};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestSize(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCapture(value),n=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],256).length);
n=addSize(n,4+textBytes(value["name"],128).length);
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsUtcTimeSize(value["start_time"]));
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["loop_repeats"],1,false);n=addSize(n,1);
n=addSize(n,Ros1DurationSize(value["loop_interval"]));
if(!Array.isArray(value["mission_ids"])||value["mission_ids"].length>2)bad();
n=addSize(n,4);
for(const element of value["mission_ids"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(typeof value["min_battery"]!=="number"||!Number.isFinite(value["min_battery"])||!Number.isFinite(Math.fround(value["min_battery"])))bad();n=addSize(n,4);
n=addSize(n,Ros1DurationSize(value["timeout"]));
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestWrite(value,w){
w.text(value["uuid"],256);
w.text(value["name"],128);
Ros1Clearpath_mission_scheduler_msgsUtcTimeWrite(value["start_time"],w);
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["loop_repeats"],true);w.at+=1;
Ros1DurationWrite(value["loop_interval"],w);
w.v.setUint32(w.at,value["mission_ids"].length,true);w.at+=4;
for(const element of value["mission_ids"]){
w.text(element,256);
}
w.v.setFloat32(w.at,value["min_battery"],true);w.at+=4;
Ros1DurationWrite(value["timeout"],w);
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestRead(r){const value={};
{
const decoded=r.text(256);
value["uuid"]=decoded;
}
{
const decoded=r.text(128);
value["name"]=decoded;
}
{
const decoded=Ros1Clearpath_mission_scheduler_msgsUtcTimeRead(r);
value["start_time"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["loop_repeats"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["loop_interval"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["mission_ids"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["mission_ids"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["min_battery"]=decoded;
}
{
const decoded=Ros1DurationRead(r);
value["timeout"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["name"];
out["name"]=field;
}
{const field=value["start_time"];
out["start_time"]=Ros1Clearpath_mission_scheduler_msgsUtcTimeCapture(field);
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["loop_repeats"];
out["loop_repeats"]=field;
}
{const field=value["loop_interval"];
out["loop_interval"]=Ros1DurationCapture(field);
}
{const field=value["mission_ids"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mission_ids"]=values;
}
{const field=value["min_battery"];
out["min_battery"]=field;
}
{const field=value["timeout"];
out["timeout"]=Ros1DurationCapture(field);
}
{const field=value["enabled"];
out["enabled"]=field;
}
return out;}
/** Complete ROS1 clearpath_mission_scheduler_msgs/UpdateScheduleResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9b32778480e054c6aa5d6100fab9bc44efde5fec5456e05af13618e53b410853",maxWireBytes:944,minWireBytes:40,cartesianMaxWireBytes:944n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":Ros1Clearpath_mission_scheduler_msgsScheduleCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseSize(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCapture(value),n=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseSize(owned),w=new Writer(n);Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,944,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_mission_scheduler_msgsScheduleSize(value["result"]));
if(n>944)bad(90);return n;}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseWrite(value,w){
Ros1Clearpath_mission_scheduler_msgsScheduleWrite(value["result"],w);
}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseRead(r){const value={};
{
const decoded=Ros1Clearpath_mission_scheduler_msgsScheduleRead(r);
value["result"]=decoded;
}
return value;}
function Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=Ros1Clearpath_mission_scheduler_msgsScheduleCapture(field);
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/AutonomyTaskSrvRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e8a7610c646bb07a6de057f8882dd431405a8ce00f8f5ee11679a383d73cb9d6",maxWireBytes:804,minWireBytes:12,cartesianMaxWireBytes:804n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version":"","floats":[],"strings":[]};},
validateValue(value){Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestSize(Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCapture(value),n=Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,804,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["version"],256).length);
if(!Array.isArray(value["floats"])||value["floats"].length>2)bad();
n=addSize(n,4);
for(const element of value["floats"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["strings"])||value["strings"].length>2)bad();
n=addSize(n,4);
for(const element of value["strings"]){
n=addSize(n,4+textBytes(element,256).length);
}
if(n>804)bad(90);return n;}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestWrite(value,w){
w.text(value["version"],256);
w.v.setUint32(w.at,value["floats"].length,true);w.at+=4;
for(const element of value["floats"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["strings"].length,true);w.at+=4;
for(const element of value["strings"]){
w.text(element,256);
}
}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestRead(r){const value={};
{
const decoded=r.text(256);
value["version"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["floats"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["floats"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["strings"]=[];for(let i=0;i<count;++i){
const decoded=r.text(256);
value["strings"].push(decoded);
}}
}
return value;}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version"];
out["version"]=field;
}
{const field=value["floats"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["floats"]=values;
}
{const field=value["strings"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["strings"]=values;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/AutonomyTaskSrvResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f1c6ef16107712804760b090620b57e8bb60dd19ccfbb516630ed916acf42e8c",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({"NOGPS":0,"NOHEADING_RTK":1,"ROBOT_IS_MOVING":2,"ERROR":3,"SERVICE_NOT_AVAILABLE":4,"PENDING":5,"COMPLETED":6}),
defaultValue(){return {"success":false,"status":0};},
validateValue(value){Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseSize(Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCapture(value),n=Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
integer(value["status"],1,true);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["status"],true);w.at+=1;
}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["status"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetDelayCompensationRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ec8f99417bf6bf770da1acdebb86acc69ec41b9dd523e235688c4ccc0fff453e",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enable_delay_compensation":false,"delay":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSetDelayCompensationRequestSize(Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCapture(value),n=Ros1Clearpath_navigation_msgsSetDelayCompensationRequestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetDelayCompensationRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetDelayCompensationRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetDelayCompensationRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enable_delay_compensation"]!=="boolean")bad();n=addSize(n,1);
integer(value["delay"],2,false);n=addSize(n,2);
if(n>3)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetDelayCompensationRequestWrite(value,w){
w.v.setUint8(w.at,value["enable_delay_compensation"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["delay"],true);w.at+=2;
}
function Ros1Clearpath_navigation_msgsSetDelayCompensationRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_delay_compensation"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["delay"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enable_delay_compensation"];
out["enable_delay_compensation"]=field;
}
{const field=value["delay"];
out["delay"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetDelayCompensationResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9fc1ca659396456af96f2abb819cec8217992f10b6dd3b0e0f94baf37e70e6bc",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsSetDelayCompensationResponseSize(Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCapture(value),n=Ros1Clearpath_navigation_msgsSetDelayCompensationResponseSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetDelayCompensationResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetDelayCompensationResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetDelayCompensationResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetDelayCompensationResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsSetDelayCompensationResponseRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetPathShifterRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetPathShifterRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_66a02e58f180dfdb2a32bd6162bf329004e3dd5a91232507dffc15fe4ba0bd6c",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enable_path_shifter":false,"path_shift_offset":0,"path_shift_time":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSetPathShifterRequestSize(Ros1Clearpath_navigation_msgsSetPathShifterRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetPathShifterRequestCapture(value),n=Ros1Clearpath_navigation_msgsSetPathShifterRequestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetPathShifterRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetPathShifterRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetPathShifterRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enable_path_shifter"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["path_shift_offset"]!=="number"||!Number.isFinite(value["path_shift_offset"])||!Number.isFinite(Math.fround(value["path_shift_offset"])))bad();n=addSize(n,4);
if(typeof value["path_shift_time"]!=="number"||!Number.isFinite(value["path_shift_time"])||!Number.isFinite(Math.fround(value["path_shift_time"])))bad();n=addSize(n,4);
if(n>9)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetPathShifterRequestWrite(value,w){
w.v.setUint8(w.at,value["enable_path_shifter"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["path_shift_offset"],true);w.at+=4;
w.v.setFloat32(w.at,value["path_shift_time"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsSetPathShifterRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_path_shifter"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["path_shift_offset"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["path_shift_time"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSetPathShifterRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enable_path_shifter"];
out["enable_path_shifter"]=field;
}
{const field=value["path_shift_offset"];
out["path_shift_offset"]=field;
}
{const field=value["path_shift_time"];
out["path_shift_time"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetPathShifterResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetPathShifterResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_64e3a6c0fe9c4303436ad8400fb0c5a74b47b226162648e2dad5e19db256ac2e",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsSetPathShifterResponseSize(Ros1Clearpath_navigation_msgsSetPathShifterResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetPathShifterResponseCapture(value),n=Ros1Clearpath_navigation_msgsSetPathShifterResponseSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetPathShifterResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetPathShifterResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetPathShifterResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetPathShifterResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsSetPathShifterResponseRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsSetPathShifterResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetPathSmootherRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetPathSmootherRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_13ca562ab68dfd839bd370ac19cd4eec02c7a855876b5e9ec6e5b00f968b111f",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enable_path_smoother":false,"turn_radius":0,"step_size":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSetPathSmootherRequestSize(Ros1Clearpath_navigation_msgsSetPathSmootherRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetPathSmootherRequestCapture(value),n=Ros1Clearpath_navigation_msgsSetPathSmootherRequestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetPathSmootherRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetPathSmootherRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetPathSmootherRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enable_path_smoother"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["turn_radius"]!=="number"||!Number.isFinite(value["turn_radius"])||!Number.isFinite(Math.fround(value["turn_radius"])))bad();n=addSize(n,4);
if(typeof value["step_size"]!=="number"||!Number.isFinite(value["step_size"])||!Number.isFinite(Math.fround(value["step_size"])))bad();n=addSize(n,4);
if(n>9)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetPathSmootherRequestWrite(value,w){
w.v.setUint8(w.at,value["enable_path_smoother"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["turn_radius"],true);w.at+=4;
w.v.setFloat32(w.at,value["step_size"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsSetPathSmootherRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_path_smoother"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["turn_radius"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["step_size"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSetPathSmootherRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enable_path_smoother"];
out["enable_path_smoother"]=field;
}
{const field=value["turn_radius"];
out["turn_radius"]=field;
}
{const field=value["step_size"];
out["step_size"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetPathSmootherResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetPathSmootherResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4317fa93c867dc6ef5a4c5bdaece4bdad2ab9d888d707202f65a23d15ce8f0d1",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsSetPathSmootherResponseSize(Ros1Clearpath_navigation_msgsSetPathSmootherResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetPathSmootherResponseCapture(value),n=Ros1Clearpath_navigation_msgsSetPathSmootherResponseSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetPathSmootherResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetPathSmootherResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetPathSmootherResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetPathSmootherResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsSetPathSmootherResponseRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsSetPathSmootherResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetStopDistanceRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetStopDistanceRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_66c986cb752a0c4990e08b616557fc67e8400f3f4a38a5603d6e5cd5c5b69b12",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enable_stop_distance":false,"stop_distance":0};},
validateValue(value){Ros1Clearpath_navigation_msgsSetStopDistanceRequestSize(Ros1Clearpath_navigation_msgsSetStopDistanceRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetStopDistanceRequestCapture(value),n=Ros1Clearpath_navigation_msgsSetStopDistanceRequestSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetStopDistanceRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetStopDistanceRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetStopDistanceRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["enable_stop_distance"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["stop_distance"]!=="number"||!Number.isFinite(value["stop_distance"])||!Number.isFinite(Math.fround(value["stop_distance"])))bad();n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetStopDistanceRequestWrite(value,w){
w.v.setUint8(w.at,value["enable_stop_distance"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["stop_distance"],true);w.at+=4;
}
function Ros1Clearpath_navigation_msgsSetStopDistanceRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enable_stop_distance"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["stop_distance"]=decoded;
}
return value;}
function Ros1Clearpath_navigation_msgsSetStopDistanceRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enable_stop_distance"];
out["enable_stop_distance"]=field;
}
{const field=value["stop_distance"];
out["stop_distance"]=field;
}
return out;}
/** Complete ROS1 clearpath_navigation_msgs/SetStopDistanceResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_navigation_msgsSetStopDistanceResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9bb0632e92b3f8733eec16b7915dea0f19749b7020466e9a9391365f87d24d19",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros1Clearpath_navigation_msgsSetStopDistanceResponseSize(Ros1Clearpath_navigation_msgsSetStopDistanceResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_navigation_msgsSetStopDistanceResponseCapture(value),n=Ros1Clearpath_navigation_msgsSetStopDistanceResponseSize(owned),w=new Writer(n);Ros1Clearpath_navigation_msgsSetStopDistanceResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Clearpath_navigation_msgsSetStopDistanceResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_navigation_msgsSetStopDistanceResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros1Clearpath_navigation_msgsSetStopDistanceResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros1Clearpath_navigation_msgsSetStopDistanceResponseRead(r){const value={};
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
function Ros1Clearpath_navigation_msgsSetStopDistanceResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddCommunicationWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9a1cdf2a02fa873143b12cf9225980c0fd4ba681c8c600fcd11705503a85834d",maxWireBytes:1961,minWireBytes:33,cartesianMaxWireBytes:1961n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestSize(Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1961,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(value["config"]));
if(n>1961)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommunicationWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddCommunicationWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cc124f08c384c233c2239b1c5cd634d70becf8ed0f556aaeec16011c2e09f159",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseSize(Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddHeartbeatWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_58a8482be436683e3b1def8c5cfe6b974e63ce9e129d3de2c88531a3112580b4",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestSize(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(value["config"]));
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddHeartbeatWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4c15aaf99a3ceab29b595b76df4d641282de57182511447b584126119265b7ce",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseSize(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddInclinationWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1463748d4c746f08f9347ca0d94fc3860009fff7de2b24862d1fc2b35c328470",maxWireBytes:1449,minWireBytes:33,cartesianMaxWireBytes:1449n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigInclinationCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestSize(Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1449,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(value["config"]));
if(n>1449)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigInclinationWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigInclinationRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddInclinationWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_366c7d76dcd82a989d1dd1453d5441e57ff229795b6f978a6a8aa1b1f00cf2d2",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseSize(Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNodeStatusWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_418c383184ef1e70c02fabf773dd943f5afc14b581ba07253eb9c285d0ba7836",maxWireBytes:1438,minWireBytes:22,cartesianMaxWireBytes:1438n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestSize(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1438,90);if(owned.length<22)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(value["config"]));
if(n>1438)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNodeStatusWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_08c4dd8bd289e405d27a981707ebc959c5c786640ac50c6bbeda628245644cfc",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseSize(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNodeletStatusWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9d7b4e34a74a50528b1e2c61db3fc36eef9d2e93ccd03815ed8c770526070e8c",maxWireBytes:1698,minWireBytes:26,cartesianMaxWireBytes:1698n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestSize(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1698,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(value["config"]));
if(n>1698)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNodeletStatusWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e1f797bf8282c0cc3cdfb105c0545438a3ac9f12e7027231001ed87b2e62dba6",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseSize(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNumPointsWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4303f93fc59680bbd04437b832060a2f2d3f93fe4cb06c5fb42ebba9736f1f73",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestSize(Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(value["config"]));
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNumPointsWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddNumPointsWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d51e4b3bfb779b244d3f2bb6979db7bb8144de7c5fafc9eef0b49a4c1f8030ef",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseSize(Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddOdomCovarianceWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_807e8d5088be66039e0e9264f239a0a8f1ca0f067cb2c7173e62b1a5ff9d49b7",maxWireBytes:1449,minWireBytes:33,cartesianMaxWireBytes:1449n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestSize(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1449,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(value["config"]));
if(n>1449)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddOdomCovarianceWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2af9a9a057aa4368b076c14bfcef6e7b0bae09a5727ab0e0ad79a6f2c5a21b06",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseSize(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddRtkFixWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_debc849b5738a49920e4adea8e76f75a55f071c2d971a048fbd639c5b3ababe4",maxWireBytes:1443,minWireBytes:27,cartesianMaxWireBytes:1443n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestSize(Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1443,90);if(owned.length<27)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(value["config"]));
if(n>1443)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigRtkFixWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddRtkFixWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1f680adab1e07f42b3d76200810f330ad5e70dcf350484bf5aca0f4dbd32c535",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseSize(Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddTopicDataWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0bdeed1e711b9da18829c30d7499ea3eda6afa5538db74d2bb0e88385b0bc1fd",maxWireBytes:1441,minWireBytes:25,cartesianMaxWireBytes:1441n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"config":Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCodec.defaultValue()};},
validateValue(value){Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestSize(Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1441,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(value["config"]));
if(n>1441)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigTopicDataWrite(value["config"],w);
}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataRead(r);
value["config"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["config"];
out["config"]=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(field);
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/AddTopicDataWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9fea6483689717396e22f81e995067ccb427a3a2ac18b48ddd91b72246e8621d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseSize(Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/RemoveWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsRemoveWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_87a214c09e13bf5a9627f6be0b7d59c473fdbb7680791e4f96a580253f820a45",maxWireBytes:132,minWireBytes:4,cartesianMaxWireBytes:132n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":""};},
validateValue(value){Ros1Clearpath_safety_msgsRemoveWatchdogRequestSize(Ros1Clearpath_safety_msgsRemoveWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsRemoveWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsRemoveWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsRemoveWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,132,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsRemoveWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsRemoveWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],128).length);
if(n>132)bad(90);return n;}
function Ros1Clearpath_safety_msgsRemoveWatchdogRequestWrite(value,w){
w.text(value["name"],128);
}
function Ros1Clearpath_safety_msgsRemoveWatchdogRequestRead(r){const value={};
{
const decoded=r.text(128);
value["name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsRemoveWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/RemoveWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsRemoveWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cb5e504b035471d2b78f9574fa83b212689a4e68f197881aa787bfb13defe8a1",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsRemoveWatchdogResponseSize(Ros1Clearpath_safety_msgsRemoveWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsRemoveWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsRemoveWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsRemoveWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsRemoveWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsRemoveWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsRemoveWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsRemoveWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsRemoveWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateCommunicationWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_28311849018f0a63f18add101832a0f70fe8a3cb50455b598f9a4fdea9f0a172",maxWireBytes:2221,minWireBytes:37,cartesianMaxWireBytes:2221n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2221,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigCommunicationSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>2221)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigCommunicationWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigCommunicationCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateCommunicationWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_16bf85e3159ebef01b8aa74c8b2dd8bd4037ea3b98723ce5feeaadaee1086f08",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateHeartbeatWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_38f7e38b9de37001d85a0d0642420bcedabc3d16e5477c3c0e80cdd3c01c33dc",maxWireBytes:1701,minWireBytes:29,cartesianMaxWireBytes:1701n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1701,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1701)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigHeartbeatCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateHeartbeatWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5102240f548751135043fe3e20cc41d8ed189ed39f75f5c8328e169e3bf5c4fc",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateInclinationWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cab6e7cbad06eb1c7692a64f0b7870ec5e3e6e56172916597f551a6dfb6a442c",maxWireBytes:1709,minWireBytes:37,cartesianMaxWireBytes:1709n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigInclinationCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1709,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigInclinationSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1709)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigInclinationWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigInclinationRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigInclinationCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateInclinationWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_822cddd0fc632d1140f7f32b375eee38348748c4eba1ce79cb271ffdf100e1fb",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNodeStatusWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_962c4c93dd43f25968ee1cae173f614573c8f488bb58e4d254f8e2f9a8ee20cb",maxWireBytes:1698,minWireBytes:26,cartesianMaxWireBytes:1698n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1698,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1698)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigNodeStatusCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNodeStatusWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4c78b86fb01b4f645285f53ba1f03f7992db7aa21928129d3e436885ee241b07",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNodeletStatusWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_696685f347ed5da62811b33ff4bc7d5a368b71347e72ff96ae20852c404e5a7b",maxWireBytes:1958,minWireBytes:30,cartesianMaxWireBytes:1958n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1958,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1958)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigNodeletStatusCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNodeletStatusWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7948d56be62c8db523533c6de69635c32bb16b9ee1f46c5ab2150a148e8951d3",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNumPointsWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b6990dab39039686f9f0ca55bc832b07f299522b0c5a94d441cebcfb78f8da61",maxWireBytes:1701,minWireBytes:29,cartesianMaxWireBytes:1701n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1701,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigNumPointsSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1701)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigNumPointsWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigNumPointsCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateNumPointsWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_edb0c4ab7f230636492b5e9ddae9d0717f37a042167aed3d1473489321fa5b96",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateOdomCovarianceWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_884e8b91e3bcb9b559ca03b46c268eebbda100aa40c92c62f805c29f69fae628",maxWireBytes:1709,minWireBytes:37,cartesianMaxWireBytes:1709n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1709,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1709)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigOdomCovarianceCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateOdomCovarianceWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_36ab644fd1658850c45accb7a3443210e51420d9bb30200566ab1444de17de20",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateRtkFixWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_346015ad9d35ab1f670410d69707f5e7464bc961c610ad5f03e1a072bf270e71",maxWireBytes:1703,minWireBytes:31,cartesianMaxWireBytes:1703n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1703,90);if(owned.length<31)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigRtkFixSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1703)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigRtkFixWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigRtkFixCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateRtkFixWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8011c3715440e0b385c2700ba784d7e144a885141644b6446a2bb0d170c02572",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateTopicDataWatchdogRequest codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d5e48e1a2fdc42e8a2e69d6e3052128b040476caf17478fc14cdec784addcc39",maxWireBytes:1701,minWireBytes:29,cartesianMaxWireBytes:1701n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"new_config":Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCodec.defaultValue(),"current_name":""};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestSize(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCapture(value),n=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1701,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Clearpath_safety_msgsWatchdogConfigTopicDataSize(value["new_config"]));
n=addSize(n,4+textBytes(value["current_name"],256).length);
if(n>1701)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestWrite(value,w){
Ros1Clearpath_safety_msgsWatchdogConfigTopicDataWrite(value["new_config"],w);
w.text(value["current_name"],256);
}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestRead(r){const value={};
{
const decoded=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataRead(r);
value["new_config"]=decoded;
}
{
const decoded=r.text(256);
value["current_name"]=decoded;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["new_config"];
out["new_config"]=Ros1Clearpath_safety_msgsWatchdogConfigTopicDataCapture(field);
}
{const field=value["current_name"];
out["current_name"]=field;
}
return out;}
/** Complete ROS1 clearpath_safety_msgs/UpdateTopicDataWatchdogResponse codec; maxima are not actual lengths. */
export const Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_491070db415651a6f37bedaa7c23caceda299a84fa956b272dd21fb062684f85",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseSize(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCapture(value));return true;},
encode(value){const owned=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCapture(value),n=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseSize(owned),w=new Writer(n);Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
