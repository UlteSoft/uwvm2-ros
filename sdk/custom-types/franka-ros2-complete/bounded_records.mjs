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
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionErrorRecovery_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_2caee3e8d5c04f12239a71f6e9a9259b818b9f1ad75103442bfc9d2343a3a070",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_msgsActionErrorRecovery_GoalSize(Ros2Franka_msgsActionErrorRecovery_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionErrorRecovery_GoalCapture(value),n=Ros2Franka_msgsActionErrorRecovery_GoalSize(owned),w=new Writer(n);Ros2Franka_msgsActionErrorRecovery_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionErrorRecovery_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionErrorRecovery_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_msgsActionErrorRecovery_GoalWrite(value,w){
}
function Ros2Franka_msgsActionErrorRecovery_GoalRead(r){const value={};
return value;}
function Ros2Franka_msgsActionErrorRecovery_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Result codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionErrorRecovery_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_9112b16c42034737b96214f23249d499ab6a9f0aee529cfec34c67cdc76809e9",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_msgsActionErrorRecovery_ResultSize(Ros2Franka_msgsActionErrorRecovery_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionErrorRecovery_ResultCapture(value),n=Ros2Franka_msgsActionErrorRecovery_ResultSize(owned),w=new Writer(n);Ros2Franka_msgsActionErrorRecovery_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionErrorRecovery_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionErrorRecovery_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_msgsActionErrorRecovery_ResultWrite(value,w){
}
function Ros2Franka_msgsActionErrorRecovery_ResultRead(r){const value={};
return value;}
function Ros2Franka_msgsActionErrorRecovery_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionErrorRecovery_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_cc817bcbe5790860e83c71e50a8c3610cf3965e34036b3099712cdcdb426127f",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_msgsActionErrorRecovery_FeedbackSize(Ros2Franka_msgsActionErrorRecovery_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionErrorRecovery_FeedbackCapture(value),n=Ros2Franka_msgsActionErrorRecovery_FeedbackSize(owned),w=new Writer(n);Ros2Franka_msgsActionErrorRecovery_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionErrorRecovery_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionErrorRecovery_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_msgsActionErrorRecovery_FeedbackWrite(value,w){
}
function Ros2Franka_msgsActionErrorRecovery_FeedbackRead(r){const value={};
return value;}
function Ros2Franka_msgsActionErrorRecovery_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_msgs/msg/GraspEpsilon codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgGraspEpsilonCodec=Object.freeze({codecIdentity:"UWVMROS1_21787ae5929dc83b9f644f4914e22da9ddd3e4f7ad3d8c6cda6bbcdfeba20c38",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"inner":0.005,"outer":0.005};},
validateValue(value){Ros2Franka_msgsMsgGraspEpsilonSize(Ros2Franka_msgsMsgGraspEpsilonCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgGraspEpsilonCapture(value),n=Ros2Franka_msgsMsgGraspEpsilonSize(owned),w=new Writer(n);Ros2Franka_msgsMsgGraspEpsilonWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgGraspEpsilonRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgGraspEpsilonSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["inner"]!=="number"||!Number.isFinite(value["inner"]))bad();n=addSize(n,8);
if(typeof value["outer"]!=="number"||!Number.isFinite(value["outer"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros2Franka_msgsMsgGraspEpsilonWrite(value,w){
w.v.setFloat64(w.at,value["inner"],true);w.at+=8;
w.v.setFloat64(w.at,value["outer"],true);w.at+=8;
}
function Ros2Franka_msgsMsgGraspEpsilonRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["inner"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["outer"]=decoded;
}
return value;}
function Ros2Franka_msgsMsgGraspEpsilonCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["inner"];
out["inner"]=field;
}
{const field=value["outer"];
out["outer"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Grasp_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionGrasp_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_b7ef701e9fd93b5da9894499234d3647ecc4cea1936f637105c172f4579b6e1f",maxWireBytes:40,minWireBytes:40,cartesianMaxWireBytes:40n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"width":0,"epsilon":Ros2Franka_msgsMsgGraspEpsilonCodec.defaultValue(),"speed":0,"force":0};},
validateValue(value){Ros2Franka_msgsActionGrasp_GoalSize(Ros2Franka_msgsActionGrasp_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionGrasp_GoalCapture(value),n=Ros2Franka_msgsActionGrasp_GoalSize(owned),w=new Writer(n);Ros2Franka_msgsActionGrasp_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,40,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionGrasp_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionGrasp_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["width"]!=="number"||!Number.isFinite(value["width"]))bad();n=addSize(n,8);
n=addSize(n,Ros2Franka_msgsMsgGraspEpsilonSize(value["epsilon"]));
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"]))bad();n=addSize(n,8);
if(typeof value["force"]!=="number"||!Number.isFinite(value["force"]))bad();n=addSize(n,8);
if(n>40)bad(90);return n;}
function Ros2Franka_msgsActionGrasp_GoalWrite(value,w){
w.v.setFloat64(w.at,value["width"],true);w.at+=8;
Ros2Franka_msgsMsgGraspEpsilonWrite(value["epsilon"],w);
w.v.setFloat64(w.at,value["speed"],true);w.at+=8;
w.v.setFloat64(w.at,value["force"],true);w.at+=8;
}
function Ros2Franka_msgsActionGrasp_GoalRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["width"]=decoded;
}
{
const decoded=Ros2Franka_msgsMsgGraspEpsilonRead(r);
value["epsilon"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["speed"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["force"]=decoded;
}
return value;}
function Ros2Franka_msgsActionGrasp_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["width"];
out["width"]=field;
}
{const field=value["epsilon"];
out["epsilon"]=Ros2Franka_msgsMsgGraspEpsilonCapture(field);
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["force"];
out["force"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Grasp_Result codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionGrasp_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_9a4c3150c0c2d5eb8ff3dad403473a9546ee37cc3086a55a628a1a85b905b733",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsActionGrasp_ResultSize(Ros2Franka_msgsActionGrasp_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionGrasp_ResultCapture(value),n=Ros2Franka_msgsActionGrasp_ResultSize(owned),w=new Writer(n);Ros2Franka_msgsActionGrasp_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionGrasp_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionGrasp_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsActionGrasp_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsActionGrasp_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsActionGrasp_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Grasp_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionGrasp_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_2e7697286c002e2d891e9c3167aa0dd1664acfc66c649fff6e8f9101c42703fe",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_width":0};},
validateValue(value){Ros2Franka_msgsActionGrasp_FeedbackSize(Ros2Franka_msgsActionGrasp_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionGrasp_FeedbackCapture(value),n=Ros2Franka_msgsActionGrasp_FeedbackSize(owned),w=new Writer(n);Ros2Franka_msgsActionGrasp_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionGrasp_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionGrasp_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current_width"]!=="number"||!Number.isFinite(value["current_width"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Franka_msgsActionGrasp_FeedbackWrite(value,w){
w.v.setFloat64(w.at,value["current_width"],true);w.at+=8;
}
function Ros2Franka_msgsActionGrasp_FeedbackRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current_width"]=decoded;
}
return value;}
function Ros2Franka_msgsActionGrasp_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_width"];
out["current_width"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Homing_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionHoming_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_8008426ce3fa8c8d62133704f8738201b8154b324ba07e7b6f605cd3132a885d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_msgsActionHoming_GoalSize(Ros2Franka_msgsActionHoming_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionHoming_GoalCapture(value),n=Ros2Franka_msgsActionHoming_GoalSize(owned),w=new Writer(n);Ros2Franka_msgsActionHoming_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionHoming_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionHoming_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_msgsActionHoming_GoalWrite(value,w){
}
function Ros2Franka_msgsActionHoming_GoalRead(r){const value={};
return value;}
function Ros2Franka_msgsActionHoming_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_msgs/action/Homing_Result codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionHoming_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_74a9677213ffa9ba3a26b3053956350b8f75d5d1769152e5e3d4d26370bcd77c",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsActionHoming_ResultSize(Ros2Franka_msgsActionHoming_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionHoming_ResultCapture(value),n=Ros2Franka_msgsActionHoming_ResultSize(owned),w=new Writer(n);Ros2Franka_msgsActionHoming_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionHoming_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionHoming_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsActionHoming_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsActionHoming_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsActionHoming_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Homing_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionHoming_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_2260e092d369a0d2e4bf97ca2dfa78c02b6836b8167e71d63e76b5c0c40a6b46",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_width":0};},
validateValue(value){Ros2Franka_msgsActionHoming_FeedbackSize(Ros2Franka_msgsActionHoming_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionHoming_FeedbackCapture(value),n=Ros2Franka_msgsActionHoming_FeedbackSize(owned),w=new Writer(n);Ros2Franka_msgsActionHoming_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionHoming_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionHoming_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current_width"]!=="number"||!Number.isFinite(value["current_width"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Franka_msgsActionHoming_FeedbackWrite(value,w){
w.v.setFloat64(w.at,value["current_width"],true);w.at+=8;
}
function Ros2Franka_msgsActionHoming_FeedbackRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current_width"]=decoded;
}
return value;}
function Ros2Franka_msgsActionHoming_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_width"];
out["current_width"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Move_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionMove_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_9f9e308ebd034344ee73b22a07d005944975c037943279504b2414bdd9ad206f",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"width":0,"speed":0};},
validateValue(value){Ros2Franka_msgsActionMove_GoalSize(Ros2Franka_msgsActionMove_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionMove_GoalCapture(value),n=Ros2Franka_msgsActionMove_GoalSize(owned),w=new Writer(n);Ros2Franka_msgsActionMove_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionMove_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionMove_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["width"]!=="number"||!Number.isFinite(value["width"]))bad();n=addSize(n,8);
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros2Franka_msgsActionMove_GoalWrite(value,w){
w.v.setFloat64(w.at,value["width"],true);w.at+=8;
w.v.setFloat64(w.at,value["speed"],true);w.at+=8;
}
function Ros2Franka_msgsActionMove_GoalRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["width"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["speed"]=decoded;
}
return value;}
function Ros2Franka_msgsActionMove_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["width"];
out["width"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Move_Result codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionMove_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_66aa505c035357274c68be773e1b502c7d77f195f3dca848e818811ce95dd266",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsActionMove_ResultSize(Ros2Franka_msgsActionMove_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionMove_ResultCapture(value),n=Ros2Franka_msgsActionMove_ResultSize(owned),w=new Writer(n);Ros2Franka_msgsActionMove_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionMove_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionMove_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsActionMove_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsActionMove_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsActionMove_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/Move_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionMove_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_18d0e658f109492380c135e787adbdcc026b051e9a547fcea70453bbd9750596",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_width":0};},
validateValue(value){Ros2Franka_msgsActionMove_FeedbackSize(Ros2Franka_msgsActionMove_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionMove_FeedbackCapture(value),n=Ros2Franka_msgsActionMove_FeedbackSize(owned),w=new Writer(n);Ros2Franka_msgsActionMove_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionMove_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionMove_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current_width"]!=="number"||!Number.isFinite(value["current_width"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Franka_msgsActionMove_FeedbackWrite(value,w){
w.v.setFloat64(w.at,value["current_width"],true);w.at+=8;
}
function Ros2Franka_msgsActionMove_FeedbackRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current_width"]=decoded;
}
return value;}
function Ros2Franka_msgsActionMove_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_width"];
out["current_width"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/PTPMotion_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionPTPMotion_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_7e1b02257a7be37e7bccd0870ebced1e1560e7250eaeaa4547836e721f2ea2cc",maxWireBytes:128,minWireBytes:16,cartesianMaxWireBytes:128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"goal_joint_configuration":[],"maximum_joint_velocities":[],"goal_tolerance":0};},
validateValue(value){Ros2Franka_msgsActionPTPMotion_GoalSize(Ros2Franka_msgsActionPTPMotion_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionPTPMotion_GoalCapture(value),n=Ros2Franka_msgsActionPTPMotion_GoalSize(owned),w=new Writer(n);Ros2Franka_msgsActionPTPMotion_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,128,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionPTPMotion_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionPTPMotion_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["goal_joint_configuration"])||value["goal_joint_configuration"].length>7)bad();
n=addSize(n,4);
for(const element of value["goal_joint_configuration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["maximum_joint_velocities"])||value["maximum_joint_velocities"].length>7)bad();
n=addSize(n,4);
for(const element of value["maximum_joint_velocities"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["goal_tolerance"]!=="number"||!Number.isFinite(value["goal_tolerance"]))bad();n=addSize(n,8);
if(n>128)bad(90);return n;}
function Ros2Franka_msgsActionPTPMotion_GoalWrite(value,w){
w.v.setUint32(w.at,value["goal_joint_configuration"].length,true);w.at+=4;
for(const element of value["goal_joint_configuration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["maximum_joint_velocities"].length,true);w.at+=4;
for(const element of value["maximum_joint_velocities"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["goal_tolerance"],true);w.at+=8;
}
function Ros2Franka_msgsActionPTPMotion_GoalRead(r){const value={};
{
{const count=r.u32();if(count>7)bad();value["goal_joint_configuration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["goal_joint_configuration"].push(decoded);
}}
}
{
{const count=r.u32();if(count>7)bad();value["maximum_joint_velocities"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["maximum_joint_velocities"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["goal_tolerance"]=decoded;
}
return value;}
function Ros2Franka_msgsActionPTPMotion_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["goal_joint_configuration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["goal_joint_configuration"]=values;
}
{const field=value["maximum_joint_velocities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["maximum_joint_velocities"]=values;
}
{const field=value["goal_tolerance"];
out["goal_tolerance"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/msg/TargetStatus codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgTargetStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_df49eedc13cd8be2055361d8640c519bfa018ed2a421aa2b59b35c8828eac585",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"IDLE":0,"EXECUTING":1,"TARGET_REACHED":2,"ABORTED":3}),
defaultValue(){return {"status":0};},
validateValue(value){Ros2Franka_msgsMsgTargetStatusSize(Ros2Franka_msgsMsgTargetStatusCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgTargetStatusCapture(value),n=Ros2Franka_msgsMsgTargetStatusSize(owned),w=new Writer(n);Ros2Franka_msgsMsgTargetStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgTargetStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgTargetStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Franka_msgsMsgTargetStatusWrite(value,w){
w.v.setUint8(w.at,value["status"],true);w.at+=1;
}
function Ros2Franka_msgsMsgTargetStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["status"]=decoded;
}
return value;}
function Ros2Franka_msgsMsgTargetStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/PTPMotion_Result codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionPTPMotion_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_b61fabf13a3d8d910a35a5145e87b4dd80180b0678ef5580784f461e850fa4c1",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_status":Ros2Franka_msgsMsgTargetStatusCodec.defaultValue(),"error_message":""};},
validateValue(value){Ros2Franka_msgsActionPTPMotion_ResultSize(Ros2Franka_msgsActionPTPMotion_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionPTPMotion_ResultCapture(value),n=Ros2Franka_msgsActionPTPMotion_ResultSize(owned),w=new Writer(n);Ros2Franka_msgsActionPTPMotion_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionPTPMotion_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionPTPMotion_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Franka_msgsMsgTargetStatusSize(value["target_status"]));
n=addSize(n,4+textBytes(value["error_message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsActionPTPMotion_ResultWrite(value,w){
Ros2Franka_msgsMsgTargetStatusWrite(value["target_status"],w);
w.text(value["error_message"],256);
}
function Ros2Franka_msgsActionPTPMotion_ResultRead(r){const value={};
{
const decoded=Ros2Franka_msgsMsgTargetStatusRead(r);
value["target_status"]=decoded;
}
{
const decoded=r.text(256);
value["error_message"]=decoded;
}
return value;}
function Ros2Franka_msgsActionPTPMotion_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_status"];
out["target_status"]=Ros2Franka_msgsMsgTargetStatusCapture(field);
}
{const field=value["error_message"];
out["error_message"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/action/PTPMotion_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_msgsActionPTPMotion_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_c4dcfeb668595ebc341d76b5cbc606c1ae4c38b28a1b329f8436e61f506a0300",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"target_status":Ros2Franka_msgsMsgTargetStatusCodec.defaultValue()};},
validateValue(value){Ros2Franka_msgsActionPTPMotion_FeedbackSize(Ros2Franka_msgsActionPTPMotion_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsActionPTPMotion_FeedbackCapture(value),n=Ros2Franka_msgsActionPTPMotion_FeedbackSize(owned),w=new Writer(n);Ros2Franka_msgsActionPTPMotion_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Franka_msgsActionPTPMotion_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsActionPTPMotion_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Franka_msgsMsgTargetStatusSize(value["target_status"]));
if(n>1)bad(90);return n;}
function Ros2Franka_msgsActionPTPMotion_FeedbackWrite(value,w){
Ros2Franka_msgsMsgTargetStatusWrite(value["target_status"],w);
}
function Ros2Franka_msgsActionPTPMotion_FeedbackRead(r){const value={};
{
const decoded=Ros2Franka_msgsMsgTargetStatusRead(r);
value["target_status"]=decoded;
}
return value;}
function Ros2Franka_msgsActionPTPMotion_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["target_status"];
out["target_status"]=Ros2Franka_msgsMsgTargetStatusCapture(field);
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
/** Complete ROS2 franka_msgs/msg/CollisionIndicators codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgCollisionIndicatorsCodec=Object.freeze({codecIdentity:"UWVMROS1_2bdfb3ece01b47b07fae84a998b23c56fc9482a0518969e43304b318d4ee0ce1",maxWireBytes:208,minWireBytes:208,cartesianMaxWireBytes:208n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"is_cartesian_linear_collision":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"is_cartesian_angular_collision":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"is_cartesian_linear_contact":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"is_cartesian_angular_contact":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"is_joint_collision":Array.from({length:7},()=>(0)),"is_joint_contact":Array.from({length:7},()=>(0))};},
validateValue(value){Ros2Franka_msgsMsgCollisionIndicatorsSize(Ros2Franka_msgsMsgCollisionIndicatorsCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgCollisionIndicatorsCapture(value),n=Ros2Franka_msgsMsgCollisionIndicatorsSize(owned),w=new Writer(n);Ros2Franka_msgsMsgCollisionIndicatorsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,208,90);if(owned.length<208)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgCollisionIndicatorsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgCollisionIndicatorsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["is_cartesian_linear_collision"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["is_cartesian_angular_collision"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["is_cartesian_linear_contact"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["is_cartesian_angular_contact"]));
if(!Array.isArray(value["is_joint_collision"])||value["is_joint_collision"].length!==7)bad();
for(const element of value["is_joint_collision"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["is_joint_contact"])||value["is_joint_contact"].length!==7)bad();
for(const element of value["is_joint_contact"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>208)bad(90);return n;}
function Ros2Franka_msgsMsgCollisionIndicatorsWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["is_cartesian_linear_collision"],w);
Ros2Geometry_msgsMsgVector3Write(value["is_cartesian_angular_collision"],w);
Ros2Geometry_msgsMsgVector3Write(value["is_cartesian_linear_contact"],w);
Ros2Geometry_msgsMsgVector3Write(value["is_cartesian_angular_contact"],w);
for(const element of value["is_joint_collision"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["is_joint_contact"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsMsgCollisionIndicatorsRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["is_cartesian_linear_collision"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["is_cartesian_angular_collision"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["is_cartesian_linear_contact"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["is_cartesian_angular_contact"]=decoded;
}
{
{const count=7;if(count>7)bad();value["is_joint_collision"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["is_joint_collision"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["is_joint_contact"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["is_joint_contact"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsMsgCollisionIndicatorsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["is_cartesian_linear_collision"];
out["is_cartesian_linear_collision"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["is_cartesian_angular_collision"];
out["is_cartesian_angular_collision"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["is_cartesian_linear_contact"];
out["is_cartesian_linear_contact"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["is_cartesian_angular_contact"];
out["is_cartesian_angular_contact"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["is_joint_collision"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["is_joint_collision"]=values;
}
{const field=value["is_joint_contact"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["is_joint_contact"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/msg/Elbow codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgElbowCodec=Object.freeze({codecIdentity:"UWVMROS1_686cf2da29007046f9855ffeecac304c159245ec3da153990ab83be4ffce53b6",maxWireBytes:80,minWireBytes:80,cartesianMaxWireBytes:80n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"position":Array.from({length:2},()=>(0)),"desired_position":Array.from({length:2},()=>(0)),"commanded_position":Array.from({length:2},()=>(0)),"commanded_velocity":Array.from({length:2},()=>(0)),"commanded_acceleration":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Franka_msgsMsgElbowSize(Ros2Franka_msgsMsgElbowCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgElbowCapture(value),n=Ros2Franka_msgsMsgElbowSize(owned),w=new Writer(n);Ros2Franka_msgsMsgElbowWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,80,90);if(owned.length<80)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgElbowRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgElbowSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["position"])||value["position"].length!==2)bad();
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["desired_position"])||value["desired_position"].length!==2)bad();
for(const element of value["desired_position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["commanded_position"])||value["commanded_position"].length!==2)bad();
for(const element of value["commanded_position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["commanded_velocity"])||value["commanded_velocity"].length!==2)bad();
for(const element of value["commanded_velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["commanded_acceleration"])||value["commanded_acceleration"].length!==2)bad();
for(const element of value["commanded_acceleration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>80)bad(90);return n;}
function Ros2Franka_msgsMsgElbowWrite(value,w){
for(const element of value["position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["desired_position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["commanded_position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["commanded_velocity"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["commanded_acceleration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsMsgElbowRead(r){const value={};
{
{const count=2;if(count>2)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["desired_position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["desired_position"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["commanded_position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["commanded_position"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["commanded_velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["commanded_velocity"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["commanded_acceleration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["commanded_acceleration"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsMsgElbowCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["desired_position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["desired_position"]=values;
}
{const field=value["commanded_position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["commanded_position"]=values;
}
{const field=value["commanded_velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["commanded_velocity"]=values;
}
{const field=value["commanded_acceleration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["commanded_acceleration"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/msg/Errors codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgErrorsCodec=Object.freeze({codecIdentity:"UWVMROS1_c7927af1abe7710bb686574ad6f2459f8425af1d6cdb3189160bb732b5a6bc8c",maxWireBytes:36,minWireBytes:36,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_position_limits_violation":false,"cartesian_position_limits_violation":false,"self_collision_avoidance_violation":false,"joint_velocity_violation":false,"cartesian_velocity_violation":false,"force_control_safety_violation":false,"joint_reflex":false,"cartesian_reflex":false,"max_goal_pose_deviation_violation":false,"max_path_pose_deviation_violation":false,"cartesian_velocity_profile_safety_violation":false,"joint_position_motion_generator_start_pose_invalid":false,"joint_motion_generator_position_limits_violation":false,"joint_motion_generator_velocity_limits_violation":false,"joint_motion_generator_velocity_discontinuity":false,"joint_motion_generator_acceleration_discontinuity":false,"cartesian_position_motion_generator_start_pose_invalid":false,"cartesian_motion_generator_elbow_limit_violation":false,"cartesian_motion_generator_velocity_limits_violation":false,"cartesian_motion_generator_velocity_discontinuity":false,"cartesian_motion_generator_acceleration_discontinuity":false,"cartesian_motion_generator_elbow_sign_inconsistent":false,"cartesian_motion_generator_start_elbow_invalid":false,"cartesian_motion_generator_joint_position_limits_violation":false,"cartesian_motion_generator_joint_velocity_limits_violation":false,"cartesian_motion_generator_joint_velocity_discontinuity":false,"cartesian_motion_generator_joint_acceleration_discontinuity":false,"cartesian_position_motion_generator_invalid_frame":false,"force_controller_desired_force_tolerance_violation":false,"controller_torque_discontinuity":false,"start_elbow_sign_inconsistent":false,"communication_constraints_violation":false,"power_limit_violation":false,"joint_p2p_insufficient_torque_for_planning":false,"tau_j_range_violation":false,"instability_detected":false};},
validateValue(value){Ros2Franka_msgsMsgErrorsSize(Ros2Franka_msgsMsgErrorsCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgErrorsCapture(value),n=Ros2Franka_msgsMsgErrorsSize(owned),w=new Writer(n);Ros2Franka_msgsMsgErrorsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgErrorsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgErrorsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_position_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_position_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["self_collision_avoidance_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_velocity_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_velocity_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["force_control_safety_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_reflex"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_reflex"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["max_goal_pose_deviation_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["max_path_pose_deviation_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_velocity_profile_safety_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_position_motion_generator_start_pose_invalid"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_motion_generator_position_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_motion_generator_velocity_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_motion_generator_velocity_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_motion_generator_acceleration_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_position_motion_generator_start_pose_invalid"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_elbow_limit_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_velocity_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_velocity_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_acceleration_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_elbow_sign_inconsistent"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_start_elbow_invalid"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_joint_position_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_joint_velocity_limits_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_joint_velocity_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_motion_generator_joint_acceleration_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_position_motion_generator_invalid_frame"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["force_controller_desired_force_tolerance_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["controller_torque_discontinuity"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["start_elbow_sign_inconsistent"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["communication_constraints_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["power_limit_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_p2p_insufficient_torque_for_planning"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["tau_j_range_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["instability_detected"]!=="boolean")bad();n=addSize(n,1);
if(n>36)bad(90);return n;}
function Ros2Franka_msgsMsgErrorsWrite(value,w){
w.v.setUint8(w.at,value["joint_position_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_position_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["self_collision_avoidance_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_velocity_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_velocity_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["force_control_safety_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_reflex"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_reflex"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["max_goal_pose_deviation_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["max_path_pose_deviation_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_velocity_profile_safety_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_position_motion_generator_start_pose_invalid"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_motion_generator_position_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_motion_generator_velocity_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_motion_generator_velocity_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_motion_generator_acceleration_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_position_motion_generator_start_pose_invalid"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_elbow_limit_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_velocity_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_velocity_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_acceleration_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_elbow_sign_inconsistent"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_start_elbow_invalid"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_joint_position_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_joint_velocity_limits_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_joint_velocity_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_motion_generator_joint_acceleration_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_position_motion_generator_invalid_frame"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["force_controller_desired_force_tolerance_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["controller_torque_discontinuity"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["start_elbow_sign_inconsistent"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["communication_constraints_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["power_limit_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_p2p_insufficient_torque_for_planning"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["tau_j_range_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["instability_detected"]?1:0,true);w.at+=1;
}
function Ros2Franka_msgsMsgErrorsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_position_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_position_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["self_collision_avoidance_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_velocity_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_velocity_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["force_control_safety_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_reflex"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_reflex"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["max_goal_pose_deviation_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["max_path_pose_deviation_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_velocity_profile_safety_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_position_motion_generator_start_pose_invalid"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_motion_generator_position_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_motion_generator_velocity_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_motion_generator_velocity_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_motion_generator_acceleration_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_position_motion_generator_start_pose_invalid"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_elbow_limit_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_velocity_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_velocity_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_acceleration_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_elbow_sign_inconsistent"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_start_elbow_invalid"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_joint_position_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_joint_velocity_limits_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_joint_velocity_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_motion_generator_joint_acceleration_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_position_motion_generator_invalid_frame"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["force_controller_desired_force_tolerance_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["controller_torque_discontinuity"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["start_elbow_sign_inconsistent"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["communication_constraints_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["power_limit_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_p2p_insufficient_torque_for_planning"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["tau_j_range_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["instability_detected"]=decoded!==0;
}
return value;}
function Ros2Franka_msgsMsgErrorsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_position_limits_violation"];
out["joint_position_limits_violation"]=field;
}
{const field=value["cartesian_position_limits_violation"];
out["cartesian_position_limits_violation"]=field;
}
{const field=value["self_collision_avoidance_violation"];
out["self_collision_avoidance_violation"]=field;
}
{const field=value["joint_velocity_violation"];
out["joint_velocity_violation"]=field;
}
{const field=value["cartesian_velocity_violation"];
out["cartesian_velocity_violation"]=field;
}
{const field=value["force_control_safety_violation"];
out["force_control_safety_violation"]=field;
}
{const field=value["joint_reflex"];
out["joint_reflex"]=field;
}
{const field=value["cartesian_reflex"];
out["cartesian_reflex"]=field;
}
{const field=value["max_goal_pose_deviation_violation"];
out["max_goal_pose_deviation_violation"]=field;
}
{const field=value["max_path_pose_deviation_violation"];
out["max_path_pose_deviation_violation"]=field;
}
{const field=value["cartesian_velocity_profile_safety_violation"];
out["cartesian_velocity_profile_safety_violation"]=field;
}
{const field=value["joint_position_motion_generator_start_pose_invalid"];
out["joint_position_motion_generator_start_pose_invalid"]=field;
}
{const field=value["joint_motion_generator_position_limits_violation"];
out["joint_motion_generator_position_limits_violation"]=field;
}
{const field=value["joint_motion_generator_velocity_limits_violation"];
out["joint_motion_generator_velocity_limits_violation"]=field;
}
{const field=value["joint_motion_generator_velocity_discontinuity"];
out["joint_motion_generator_velocity_discontinuity"]=field;
}
{const field=value["joint_motion_generator_acceleration_discontinuity"];
out["joint_motion_generator_acceleration_discontinuity"]=field;
}
{const field=value["cartesian_position_motion_generator_start_pose_invalid"];
out["cartesian_position_motion_generator_start_pose_invalid"]=field;
}
{const field=value["cartesian_motion_generator_elbow_limit_violation"];
out["cartesian_motion_generator_elbow_limit_violation"]=field;
}
{const field=value["cartesian_motion_generator_velocity_limits_violation"];
out["cartesian_motion_generator_velocity_limits_violation"]=field;
}
{const field=value["cartesian_motion_generator_velocity_discontinuity"];
out["cartesian_motion_generator_velocity_discontinuity"]=field;
}
{const field=value["cartesian_motion_generator_acceleration_discontinuity"];
out["cartesian_motion_generator_acceleration_discontinuity"]=field;
}
{const field=value["cartesian_motion_generator_elbow_sign_inconsistent"];
out["cartesian_motion_generator_elbow_sign_inconsistent"]=field;
}
{const field=value["cartesian_motion_generator_start_elbow_invalid"];
out["cartesian_motion_generator_start_elbow_invalid"]=field;
}
{const field=value["cartesian_motion_generator_joint_position_limits_violation"];
out["cartesian_motion_generator_joint_position_limits_violation"]=field;
}
{const field=value["cartesian_motion_generator_joint_velocity_limits_violation"];
out["cartesian_motion_generator_joint_velocity_limits_violation"]=field;
}
{const field=value["cartesian_motion_generator_joint_velocity_discontinuity"];
out["cartesian_motion_generator_joint_velocity_discontinuity"]=field;
}
{const field=value["cartesian_motion_generator_joint_acceleration_discontinuity"];
out["cartesian_motion_generator_joint_acceleration_discontinuity"]=field;
}
{const field=value["cartesian_position_motion_generator_invalid_frame"];
out["cartesian_position_motion_generator_invalid_frame"]=field;
}
{const field=value["force_controller_desired_force_tolerance_violation"];
out["force_controller_desired_force_tolerance_violation"]=field;
}
{const field=value["controller_torque_discontinuity"];
out["controller_torque_discontinuity"]=field;
}
{const field=value["start_elbow_sign_inconsistent"];
out["start_elbow_sign_inconsistent"]=field;
}
{const field=value["communication_constraints_violation"];
out["communication_constraints_violation"]=field;
}
{const field=value["power_limit_violation"];
out["power_limit_violation"]=field;
}
{const field=value["joint_p2p_insufficient_torque_for_planning"];
out["joint_p2p_insufficient_torque_for_planning"]=field;
}
{const field=value["tau_j_range_violation"];
out["tau_j_range_violation"]=field;
}
{const field=value["instability_detected"];
out["instability_detected"]=field;
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
/** Complete ROS2 sensor_msgs/msg/JointState codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_912ba4261e2dfbec0e4a543ea8e5d308ef1d26877b4a7d71162a981ac64c40a3",maxWireBytes:1248,minWireBytes:28,cartesianMaxWireBytes:1248n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"name":[],"position":[],"velocity":[],"effort":[]};},
validateValue(value){Ros2Sensor_msgsMsgJointStateSize(Ros2Sensor_msgsMsgJointStateCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgJointStateCapture(value),n=Ros2Sensor_msgsMsgJointStateSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1248,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["name"])||value["name"].length>7)bad();
n=addSize(n,4);
for(const element of value["name"]){
n=addSize(n,4+textBytes(element,128).length);
}
if(!Array.isArray(value["position"])||value["position"].length>7)bad();
n=addSize(n,4);
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocity"])||value["velocity"].length>7)bad();
n=addSize(n,4);
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>7)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
if(n>1248)bad(90);return n;}
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
{const count=r.u32();if(count>7)bad();value["name"]=[];for(let i=0;i<count;++i){
const decoded=r.text(128);
value["name"].push(decoded);
}}
}
{
{const count=r.u32();if(count>7)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=r.u32();if(count>7)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
{const count=r.u32();if(count>7)bad();value["effort"]=[];for(let i=0;i<count;++i){
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
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["name"]=values;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
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
/** Complete ROS2 geometry_msgs/msg/WrenchStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgWrenchStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_e23600874a626ba9d52f043e14ed04beb3e316d47c3bca8bd87cfeed3f5d9d66",maxWireBytes:188,minWireBytes:60,cartesianMaxWireBytes:188n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"wrench":Ros2Geometry_msgsMsgWrenchCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgWrenchStampedSize(Ros2Geometry_msgsMsgWrenchStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgWrenchStampedCapture(value),n=Ros2Geometry_msgsMsgWrenchStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgWrenchStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,188,90);if(owned.length<60)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgWrenchStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgWrenchStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgWrenchSize(value["wrench"]));
if(n>188)bad(90);return n;}
function Ros2Geometry_msgsMsgWrenchStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgWrenchWrite(value["wrench"],w);
}
function Ros2Geometry_msgsMsgWrenchStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchRead(r);
value["wrench"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgWrenchStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["wrench"];
out["wrench"]=Ros2Geometry_msgsMsgWrenchCapture(field);
}
return out;}
/** Complete ROS2 geometry_msgs/msg/Inertia codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgInertiaCodec=Object.freeze({codecIdentity:"UWVMROS1_4a1b7a6a2516724a9a12e5eb166a85cd73d33fc6075c8d517ff2a279fd29e12a",maxWireBytes:80,minWireBytes:80,cartesianMaxWireBytes:80n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"m":0,"com":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"ixx":0,"ixy":0,"ixz":0,"iyy":0,"iyz":0,"izz":0};},
validateValue(value){Ros2Geometry_msgsMsgInertiaSize(Ros2Geometry_msgsMsgInertiaCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgInertiaCapture(value),n=Ros2Geometry_msgsMsgInertiaSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgInertiaWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,80,90);if(owned.length<80)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgInertiaRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgInertiaSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["m"]!=="number"||!Number.isFinite(value["m"]))bad();n=addSize(n,8);
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["com"]));
if(typeof value["ixx"]!=="number"||!Number.isFinite(value["ixx"]))bad();n=addSize(n,8);
if(typeof value["ixy"]!=="number"||!Number.isFinite(value["ixy"]))bad();n=addSize(n,8);
if(typeof value["ixz"]!=="number"||!Number.isFinite(value["ixz"]))bad();n=addSize(n,8);
if(typeof value["iyy"]!=="number"||!Number.isFinite(value["iyy"]))bad();n=addSize(n,8);
if(typeof value["iyz"]!=="number"||!Number.isFinite(value["iyz"]))bad();n=addSize(n,8);
if(typeof value["izz"]!=="number"||!Number.isFinite(value["izz"]))bad();n=addSize(n,8);
if(n>80)bad(90);return n;}
function Ros2Geometry_msgsMsgInertiaWrite(value,w){
w.v.setFloat64(w.at,value["m"],true);w.at+=8;
Ros2Geometry_msgsMsgVector3Write(value["com"],w);
w.v.setFloat64(w.at,value["ixx"],true);w.at+=8;
w.v.setFloat64(w.at,value["ixy"],true);w.at+=8;
w.v.setFloat64(w.at,value["ixz"],true);w.at+=8;
w.v.setFloat64(w.at,value["iyy"],true);w.at+=8;
w.v.setFloat64(w.at,value["iyz"],true);w.at+=8;
w.v.setFloat64(w.at,value["izz"],true);w.at+=8;
}
function Ros2Geometry_msgsMsgInertiaRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["m"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["com"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ixx"]=decoded;
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
value["iyy"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["iyz"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["izz"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgInertiaCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["m"];
out["m"]=field;
}
{const field=value["com"];
out["com"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["ixx"];
out["ixx"]=field;
}
{const field=value["ixy"];
out["ixy"]=field;
}
{const field=value["ixz"];
out["ixz"]=field;
}
{const field=value["iyy"];
out["iyy"]=field;
}
{const field=value["iyz"];
out["iyz"]=field;
}
{const field=value["izz"];
out["izz"]=field;
}
return out;}
/** Complete ROS2 geometry_msgs/msg/InertiaStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgInertiaStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_19b0b442c5858ff1eee20e4658ecc3a2b76fdd64c4a21a1642fae5ab38198576",maxWireBytes:220,minWireBytes:92,cartesianMaxWireBytes:220n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"inertia":Ros2Geometry_msgsMsgInertiaCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgInertiaStampedSize(Ros2Geometry_msgsMsgInertiaStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgInertiaStampedCapture(value),n=Ros2Geometry_msgsMsgInertiaStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgInertiaStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,220,90);if(owned.length<92)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgInertiaStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgInertiaStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgInertiaSize(value["inertia"]));
if(n>220)bad(90);return n;}
function Ros2Geometry_msgsMsgInertiaStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgInertiaWrite(value["inertia"],w);
}
function Ros2Geometry_msgsMsgInertiaStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaRead(r);
value["inertia"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgInertiaStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["inertia"];
out["inertia"]=Ros2Geometry_msgsMsgInertiaCapture(field);
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
/** Complete ROS2 geometry_msgs/msg/Accel codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgAccelCodec=Object.freeze({codecIdentity:"UWVMROS1_87264cb87970315248345c43cc629bbc0c9f9b2a8c6c4bc089da81c7af552fde",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"linear":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"angular":Ros2Geometry_msgsMsgVector3Codec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgAccelSize(Ros2Geometry_msgsMsgAccelCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgAccelCapture(value),n=Ros2Geometry_msgsMsgAccelSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgAccelWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgAccelRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgAccelSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["linear"]));
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["angular"]));
if(n>48)bad(90);return n;}
function Ros2Geometry_msgsMsgAccelWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["linear"],w);
Ros2Geometry_msgsMsgVector3Write(value["angular"],w);
}
function Ros2Geometry_msgsMsgAccelRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["linear"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["angular"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgAccelCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["linear"];
out["linear"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["angular"];
out["angular"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
return out;}
/** Complete ROS2 geometry_msgs/msg/AccelStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgAccelStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_c04d3ae310bd82f7c4c9f30c2ff185ab54989aa3adcc759c7d3eb19853819899",maxWireBytes:188,minWireBytes:60,cartesianMaxWireBytes:188n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"accel":Ros2Geometry_msgsMsgAccelCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgAccelStampedSize(Ros2Geometry_msgsMsgAccelStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgAccelStampedCapture(value),n=Ros2Geometry_msgsMsgAccelStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgAccelStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,188,90);if(owned.length<60)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgAccelStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgAccelStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Geometry_msgsMsgAccelSize(value["accel"]));
if(n>188)bad(90);return n;}
function Ros2Geometry_msgsMsgAccelStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Geometry_msgsMsgAccelWrite(value["accel"],w);
}
function Ros2Geometry_msgsMsgAccelStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgAccelRead(r);
value["accel"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgAccelStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["accel"];
out["accel"]=Ros2Geometry_msgsMsgAccelCapture(field);
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
/** Complete ROS2 franka_msgs/msg/FrankaRobotState codec; maxima are not actual lengths. */
export const Ros2Franka_msgsMsgFrankaRobotStateCodec=Object.freeze({codecIdentity:"UWVMROS1_0fdc5c99a098f4c891956bb6cd81505995d5dda41a740f88ee456f48f6e07282",maxWireBytes:10169,minWireBytes:1961,cartesianMaxWireBytes:10169n,aggregateWireBytes:0,constants:Object.freeze({"ROBOT_MODE_OTHER":0,"ROBOT_MODE_IDLE":1,"ROBOT_MODE_MOVE":2,"ROBOT_MODE_GUIDING":3,"ROBOT_MODE_REFLEX":4,"ROBOT_MODE_USER_STOPPED":5,"ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY":6}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"collision_indicators":Ros2Franka_msgsMsgCollisionIndicatorsCodec.defaultValue(),"measured_joint_state":Ros2Sensor_msgsMsgJointStateCodec.defaultValue(),"desired_joint_state":Ros2Sensor_msgsMsgJointStateCodec.defaultValue(),"measured_joint_motor_state":Ros2Sensor_msgsMsgJointStateCodec.defaultValue(),"ddq_d":Array.from({length:7},()=>(0)),"dtau_j":Array.from({length:7},()=>(0)),"tau_ext_hat_filtered":Ros2Sensor_msgsMsgJointStateCodec.defaultValue(),"elbow":Ros2Franka_msgsMsgElbowCodec.defaultValue(),"k_f_ext_hat_k":Ros2Geometry_msgsMsgWrenchStampedCodec.defaultValue(),"o_f_ext_hat_k":Ros2Geometry_msgsMsgWrenchStampedCodec.defaultValue(),"inertia_ee":Ros2Geometry_msgsMsgInertiaStampedCodec.defaultValue(),"inertia_load":Ros2Geometry_msgsMsgInertiaStampedCodec.defaultValue(),"inertia_total":Ros2Geometry_msgsMsgInertiaStampedCodec.defaultValue(),"o_t_ee":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"o_t_ee_d":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"o_t_ee_c":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"f_t_ee":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"ee_t_k":Ros2Geometry_msgsMsgPoseStampedCodec.defaultValue(),"o_dp_ee_d":Ros2Geometry_msgsMsgTwistStampedCodec.defaultValue(),"o_dp_ee_c":Ros2Geometry_msgsMsgTwistStampedCodec.defaultValue(),"o_ddp_ee_c":Ros2Geometry_msgsMsgAccelStampedCodec.defaultValue(),"time":0,"control_command_success_rate":0,"robot_mode":0,"current_errors":Ros2Franka_msgsMsgErrorsCodec.defaultValue(),"last_motion_errors":Ros2Franka_msgsMsgErrorsCodec.defaultValue(),"accelerometer_top":Array.from({length:6},()=>(Ros2Geometry_msgsMsgVector3StampedCodec.defaultValue())),"accelerometer_bottom":Array.from({length:6},()=>(Ros2Geometry_msgsMsgVector3StampedCodec.defaultValue()))};},
validateValue(value){Ros2Franka_msgsMsgFrankaRobotStateSize(Ros2Franka_msgsMsgFrankaRobotStateCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsMsgFrankaRobotStateCapture(value),n=Ros2Franka_msgsMsgFrankaRobotStateSize(owned),w=new Writer(n);Ros2Franka_msgsMsgFrankaRobotStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10169,90);if(owned.length<1961)bad();const r=new Reader(owned),value=Ros2Franka_msgsMsgFrankaRobotStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsMsgFrankaRobotStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Franka_msgsMsgCollisionIndicatorsSize(value["collision_indicators"]));
n=addSize(n,Ros2Sensor_msgsMsgJointStateSize(value["measured_joint_state"]));
n=addSize(n,Ros2Sensor_msgsMsgJointStateSize(value["desired_joint_state"]));
n=addSize(n,Ros2Sensor_msgsMsgJointStateSize(value["measured_joint_motor_state"]));
if(!Array.isArray(value["ddq_d"])||value["ddq_d"].length!==7)bad();
for(const element of value["ddq_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["dtau_j"])||value["dtau_j"].length!==7)bad();
for(const element of value["dtau_j"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros2Sensor_msgsMsgJointStateSize(value["tau_ext_hat_filtered"]));
n=addSize(n,Ros2Franka_msgsMsgElbowSize(value["elbow"]));
n=addSize(n,Ros2Geometry_msgsMsgWrenchStampedSize(value["k_f_ext_hat_k"]));
n=addSize(n,Ros2Geometry_msgsMsgWrenchStampedSize(value["o_f_ext_hat_k"]));
n=addSize(n,Ros2Geometry_msgsMsgInertiaStampedSize(value["inertia_ee"]));
n=addSize(n,Ros2Geometry_msgsMsgInertiaStampedSize(value["inertia_load"]));
n=addSize(n,Ros2Geometry_msgsMsgInertiaStampedSize(value["inertia_total"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["o_t_ee"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["o_t_ee_d"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["o_t_ee_c"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["f_t_ee"]));
n=addSize(n,Ros2Geometry_msgsMsgPoseStampedSize(value["ee_t_k"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistStampedSize(value["o_dp_ee_d"]));
n=addSize(n,Ros2Geometry_msgsMsgTwistStampedSize(value["o_dp_ee_c"]));
n=addSize(n,Ros2Geometry_msgsMsgAccelStampedSize(value["o_ddp_ee_c"]));
if(typeof value["time"]!=="number"||!Number.isFinite(value["time"]))bad();n=addSize(n,8);
if(typeof value["control_command_success_rate"]!=="number"||!Number.isFinite(value["control_command_success_rate"]))bad();n=addSize(n,8);
integer(value["robot_mode"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Franka_msgsMsgErrorsSize(value["current_errors"]));
n=addSize(n,Ros2Franka_msgsMsgErrorsSize(value["last_motion_errors"]));
if(!Array.isArray(value["accelerometer_top"])||value["accelerometer_top"].length!==6)bad();
for(const element of value["accelerometer_top"]){
n=addSize(n,Ros2Geometry_msgsMsgVector3StampedSize(element));
}
if(!Array.isArray(value["accelerometer_bottom"])||value["accelerometer_bottom"].length!==6)bad();
for(const element of value["accelerometer_bottom"]){
n=addSize(n,Ros2Geometry_msgsMsgVector3StampedSize(element));
}
if(n>10169)bad(90);return n;}
function Ros2Franka_msgsMsgFrankaRobotStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Franka_msgsMsgCollisionIndicatorsWrite(value["collision_indicators"],w);
Ros2Sensor_msgsMsgJointStateWrite(value["measured_joint_state"],w);
Ros2Sensor_msgsMsgJointStateWrite(value["desired_joint_state"],w);
Ros2Sensor_msgsMsgJointStateWrite(value["measured_joint_motor_state"],w);
for(const element of value["ddq_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["dtau_j"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros2Sensor_msgsMsgJointStateWrite(value["tau_ext_hat_filtered"],w);
Ros2Franka_msgsMsgElbowWrite(value["elbow"],w);
Ros2Geometry_msgsMsgWrenchStampedWrite(value["k_f_ext_hat_k"],w);
Ros2Geometry_msgsMsgWrenchStampedWrite(value["o_f_ext_hat_k"],w);
Ros2Geometry_msgsMsgInertiaStampedWrite(value["inertia_ee"],w);
Ros2Geometry_msgsMsgInertiaStampedWrite(value["inertia_load"],w);
Ros2Geometry_msgsMsgInertiaStampedWrite(value["inertia_total"],w);
Ros2Geometry_msgsMsgPoseStampedWrite(value["o_t_ee"],w);
Ros2Geometry_msgsMsgPoseStampedWrite(value["o_t_ee_d"],w);
Ros2Geometry_msgsMsgPoseStampedWrite(value["o_t_ee_c"],w);
Ros2Geometry_msgsMsgPoseStampedWrite(value["f_t_ee"],w);
Ros2Geometry_msgsMsgPoseStampedWrite(value["ee_t_k"],w);
Ros2Geometry_msgsMsgTwistStampedWrite(value["o_dp_ee_d"],w);
Ros2Geometry_msgsMsgTwistStampedWrite(value["o_dp_ee_c"],w);
Ros2Geometry_msgsMsgAccelStampedWrite(value["o_ddp_ee_c"],w);
w.v.setFloat64(w.at,value["time"],true);w.at+=8;
w.v.setFloat64(w.at,value["control_command_success_rate"],true);w.at+=8;
w.v.setUint8(w.at,value["robot_mode"],true);w.at+=1;
Ros2Franka_msgsMsgErrorsWrite(value["current_errors"],w);
Ros2Franka_msgsMsgErrorsWrite(value["last_motion_errors"],w);
for(const element of value["accelerometer_top"]){
Ros2Geometry_msgsMsgVector3StampedWrite(element,w);
}
for(const element of value["accelerometer_bottom"]){
Ros2Geometry_msgsMsgVector3StampedWrite(element,w);
}
}
function Ros2Franka_msgsMsgFrankaRobotStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Franka_msgsMsgCollisionIndicatorsRead(r);
value["collision_indicators"]=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointStateRead(r);
value["measured_joint_state"]=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointStateRead(r);
value["desired_joint_state"]=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointStateRead(r);
value["measured_joint_motor_state"]=decoded;
}
{
{const count=7;if(count>7)bad();value["ddq_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ddq_d"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["dtau_j"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dtau_j"].push(decoded);
}}
}
{
const decoded=Ros2Sensor_msgsMsgJointStateRead(r);
value["tau_ext_hat_filtered"]=decoded;
}
{
const decoded=Ros2Franka_msgsMsgElbowRead(r);
value["elbow"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchStampedRead(r);
value["k_f_ext_hat_k"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchStampedRead(r);
value["o_f_ext_hat_k"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStampedRead(r);
value["inertia_ee"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStampedRead(r);
value["inertia_load"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStampedRead(r);
value["inertia_total"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["o_t_ee"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["o_t_ee_d"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["o_t_ee_c"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["f_t_ee"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStampedRead(r);
value["ee_t_k"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistStampedRead(r);
value["o_dp_ee_d"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistStampedRead(r);
value["o_dp_ee_c"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgAccelStampedRead(r);
value["o_ddp_ee_c"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["time"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["control_command_success_rate"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["robot_mode"]=decoded;
}
{
const decoded=Ros2Franka_msgsMsgErrorsRead(r);
value["current_errors"]=decoded;
}
{
const decoded=Ros2Franka_msgsMsgErrorsRead(r);
value["last_motion_errors"]=decoded;
}
{
{const count=6;if(count>6)bad();value["accelerometer_top"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgVector3StampedRead(r);
value["accelerometer_top"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["accelerometer_bottom"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgVector3StampedRead(r);
value["accelerometer_bottom"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsMsgFrankaRobotStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["collision_indicators"];
out["collision_indicators"]=Ros2Franka_msgsMsgCollisionIndicatorsCapture(field);
}
{const field=value["measured_joint_state"];
out["measured_joint_state"]=Ros2Sensor_msgsMsgJointStateCapture(field);
}
{const field=value["desired_joint_state"];
out["desired_joint_state"]=Ros2Sensor_msgsMsgJointStateCapture(field);
}
{const field=value["measured_joint_motor_state"];
out["measured_joint_motor_state"]=Ros2Sensor_msgsMsgJointStateCapture(field);
}
{const field=value["ddq_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["ddq_d"]=values;
}
{const field=value["dtau_j"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["dtau_j"]=values;
}
{const field=value["tau_ext_hat_filtered"];
out["tau_ext_hat_filtered"]=Ros2Sensor_msgsMsgJointStateCapture(field);
}
{const field=value["elbow"];
out["elbow"]=Ros2Franka_msgsMsgElbowCapture(field);
}
{const field=value["k_f_ext_hat_k"];
out["k_f_ext_hat_k"]=Ros2Geometry_msgsMsgWrenchStampedCapture(field);
}
{const field=value["o_f_ext_hat_k"];
out["o_f_ext_hat_k"]=Ros2Geometry_msgsMsgWrenchStampedCapture(field);
}
{const field=value["inertia_ee"];
out["inertia_ee"]=Ros2Geometry_msgsMsgInertiaStampedCapture(field);
}
{const field=value["inertia_load"];
out["inertia_load"]=Ros2Geometry_msgsMsgInertiaStampedCapture(field);
}
{const field=value["inertia_total"];
out["inertia_total"]=Ros2Geometry_msgsMsgInertiaStampedCapture(field);
}
{const field=value["o_t_ee"];
out["o_t_ee"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["o_t_ee_d"];
out["o_t_ee_d"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["o_t_ee_c"];
out["o_t_ee_c"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["f_t_ee"];
out["f_t_ee"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["ee_t_k"];
out["ee_t_k"]=Ros2Geometry_msgsMsgPoseStampedCapture(field);
}
{const field=value["o_dp_ee_d"];
out["o_dp_ee_d"]=Ros2Geometry_msgsMsgTwistStampedCapture(field);
}
{const field=value["o_dp_ee_c"];
out["o_dp_ee_c"]=Ros2Geometry_msgsMsgTwistStampedCapture(field);
}
{const field=value["o_ddp_ee_c"];
out["o_ddp_ee_c"]=Ros2Geometry_msgsMsgAccelStampedCapture(field);
}
{const field=value["time"];
out["time"]=field;
}
{const field=value["control_command_success_rate"];
out["control_command_success_rate"]=field;
}
{const field=value["robot_mode"];
out["robot_mode"]=field;
}
{const field=value["current_errors"];
out["current_errors"]=Ros2Franka_msgsMsgErrorsCapture(field);
}
{const field=value["last_motion_errors"];
out["last_motion_errors"]=Ros2Franka_msgsMsgErrorsCapture(field);
}
{const field=value["accelerometer_top"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgVector3StampedCapture(element);
}out["accelerometer_top"]=values;
}
{const field=value["accelerometer_bottom"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgVector3StampedCapture(element);
}out["accelerometer_bottom"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetCartesianStiffness_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetCartesianStiffness_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ae78b9887a5419225cfe0727bac83d8c53f789acd53bd47f024485b02aa98260",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cartesian_stiffness":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetCartesianStiffness_RequestSize(Ros2Franka_msgsSrvSetCartesianStiffness_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetCartesianStiffness_RequestCapture(value),n=Ros2Franka_msgsSrvSetCartesianStiffness_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetCartesianStiffness_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetCartesianStiffness_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetCartesianStiffness_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["cartesian_stiffness"])||value["cartesian_stiffness"].length!==6)bad();
for(const element of value["cartesian_stiffness"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>48)bad(90);return n;}
function Ros2Franka_msgsSrvSetCartesianStiffness_RequestWrite(value,w){
for(const element of value["cartesian_stiffness"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetCartesianStiffness_RequestRead(r){const value={};
{
{const count=6;if(count>6)bad();value["cartesian_stiffness"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["cartesian_stiffness"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetCartesianStiffness_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cartesian_stiffness"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cartesian_stiffness"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetCartesianStiffness_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3194f7469bbf228e653f5a93336fd5de4a1b2d7c1fe9542f3fadc8ad4fd51afa",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetCartesianStiffness_ResponseSize(Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCapture(value),n=Ros2Franka_msgsSrvSetCartesianStiffness_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetCartesianStiffness_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetCartesianStiffness_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetCartesianStiffness_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetCartesianStiffness_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetCartesianStiffness_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_829642a9ea418a53f7252f7489bd7f5ed976bf3689443012daf216db895cf99c",maxWireBytes:208,minWireBytes:208,cartesianMaxWireBytes:208n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lower_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"upper_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"lower_force_thresholds_nominal":Array.from({length:6},()=>(0)),"upper_force_thresholds_nominal":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestSize(Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCapture(value),n=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,208,90);if(owned.length<208)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["lower_torque_thresholds_nominal"])||value["lower_torque_thresholds_nominal"].length!==7)bad();
for(const element of value["lower_torque_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_torque_thresholds_nominal"])||value["upper_torque_thresholds_nominal"].length!==7)bad();
for(const element of value["upper_torque_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["lower_force_thresholds_nominal"])||value["lower_force_thresholds_nominal"].length!==6)bad();
for(const element of value["lower_force_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_force_thresholds_nominal"])||value["upper_force_thresholds_nominal"].length!==6)bad();
for(const element of value["upper_force_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>208)bad(90);return n;}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestWrite(value,w){
for(const element of value["lower_torque_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_torque_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["lower_force_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_force_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestRead(r){const value={};
{
{const count=7;if(count>7)bad();value["lower_torque_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_torque_thresholds_nominal"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["upper_torque_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_torque_thresholds_nominal"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["lower_force_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_force_thresholds_nominal"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["upper_force_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_force_thresholds_nominal"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lower_torque_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_torque_thresholds_nominal"]=values;
}
{const field=value["upper_torque_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_torque_thresholds_nominal"]=values;
}
{const field=value["lower_force_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_force_thresholds_nominal"]=values;
}
{const field=value["upper_force_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_force_thresholds_nominal"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7d5640c34446e4262a8c00f0b242b68c0095688f0208fd692f21d855003eb47a",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseSize(Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCapture(value),n=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetFullCollisionBehavior_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d6898555d931aee3a5b4f5f97830cb9982f2e378c59c40bcaa0201840d2f2824",maxWireBytes:416,minWireBytes:416,cartesianMaxWireBytes:416n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lower_torque_thresholds_acceleration":Array.from({length:7},()=>(0)),"upper_torque_thresholds_acceleration":Array.from({length:7},()=>(0)),"lower_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"upper_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"lower_force_thresholds_acceleration":Array.from({length:6},()=>(0)),"upper_force_thresholds_acceleration":Array.from({length:6},()=>(0)),"lower_force_thresholds_nominal":Array.from({length:6},()=>(0)),"upper_force_thresholds_nominal":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestSize(Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCapture(value),n=Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,416,90);if(owned.length<416)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["lower_torque_thresholds_acceleration"])||value["lower_torque_thresholds_acceleration"].length!==7)bad();
for(const element of value["lower_torque_thresholds_acceleration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_torque_thresholds_acceleration"])||value["upper_torque_thresholds_acceleration"].length!==7)bad();
for(const element of value["upper_torque_thresholds_acceleration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["lower_torque_thresholds_nominal"])||value["lower_torque_thresholds_nominal"].length!==7)bad();
for(const element of value["lower_torque_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_torque_thresholds_nominal"])||value["upper_torque_thresholds_nominal"].length!==7)bad();
for(const element of value["upper_torque_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["lower_force_thresholds_acceleration"])||value["lower_force_thresholds_acceleration"].length!==6)bad();
for(const element of value["lower_force_thresholds_acceleration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_force_thresholds_acceleration"])||value["upper_force_thresholds_acceleration"].length!==6)bad();
for(const element of value["upper_force_thresholds_acceleration"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["lower_force_thresholds_nominal"])||value["lower_force_thresholds_nominal"].length!==6)bad();
for(const element of value["lower_force_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["upper_force_thresholds_nominal"])||value["upper_force_thresholds_nominal"].length!==6)bad();
for(const element of value["upper_force_thresholds_nominal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>416)bad(90);return n;}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestWrite(value,w){
for(const element of value["lower_torque_thresholds_acceleration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_torque_thresholds_acceleration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["lower_torque_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_torque_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["lower_force_thresholds_acceleration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_force_thresholds_acceleration"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["lower_force_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["upper_force_thresholds_nominal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestRead(r){const value={};
{
{const count=7;if(count>7)bad();value["lower_torque_thresholds_acceleration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_torque_thresholds_acceleration"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["upper_torque_thresholds_acceleration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_torque_thresholds_acceleration"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["lower_torque_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_torque_thresholds_nominal"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["upper_torque_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_torque_thresholds_nominal"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["lower_force_thresholds_acceleration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_force_thresholds_acceleration"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["upper_force_thresholds_acceleration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_force_thresholds_acceleration"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["lower_force_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_force_thresholds_nominal"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["upper_force_thresholds_nominal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_force_thresholds_nominal"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lower_torque_thresholds_acceleration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_torque_thresholds_acceleration"]=values;
}
{const field=value["upper_torque_thresholds_acceleration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_torque_thresholds_acceleration"]=values;
}
{const field=value["lower_torque_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_torque_thresholds_nominal"]=values;
}
{const field=value["upper_torque_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_torque_thresholds_nominal"]=values;
}
{const field=value["lower_force_thresholds_acceleration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_force_thresholds_acceleration"]=values;
}
{const field=value["upper_force_thresholds_acceleration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_force_thresholds_acceleration"]=values;
}
{const field=value["lower_force_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["lower_force_thresholds_nominal"]=values;
}
{const field=value["upper_force_thresholds_nominal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["upper_force_thresholds_nominal"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetFullCollisionBehavior_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_76430e384e9ad8b3f3c937e3004e8dc15a51d28abecd108c698e60d39c54f300",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseSize(Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCapture(value),n=Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetJointStiffness_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetJointStiffness_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_65b5d6599d1b97a0356f599102c22e0ae8c33b85f14c5a49b6c097eb227d334c",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_stiffness":Array.from({length:7},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetJointStiffness_RequestSize(Ros2Franka_msgsSrvSetJointStiffness_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetJointStiffness_RequestCapture(value),n=Ros2Franka_msgsSrvSetJointStiffness_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetJointStiffness_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetJointStiffness_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetJointStiffness_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["joint_stiffness"])||value["joint_stiffness"].length!==7)bad();
for(const element of value["joint_stiffness"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>56)bad(90);return n;}
function Ros2Franka_msgsSrvSetJointStiffness_RequestWrite(value,w){
for(const element of value["joint_stiffness"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetJointStiffness_RequestRead(r){const value={};
{
{const count=7;if(count>7)bad();value["joint_stiffness"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_stiffness"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetJointStiffness_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_stiffness"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_stiffness"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetJointStiffness_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetJointStiffness_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1a857ab809079e0eec664250a9521ddc1cdf66e61aa2e0f22eae8442f5231c4d",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetJointStiffness_ResponseSize(Ros2Franka_msgsSrvSetJointStiffness_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetJointStiffness_ResponseCapture(value),n=Ros2Franka_msgsSrvSetJointStiffness_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetJointStiffness_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetJointStiffness_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetJointStiffness_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetJointStiffness_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetJointStiffness_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetJointStiffness_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetLoad_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetLoad_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a616a844864b7bbf4ee18ac481ddd8406bec47354b4519a0e17f8a305635b7f8",maxWireBytes:104,minWireBytes:104,cartesianMaxWireBytes:104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"center_of_mass":Array.from({length:3},()=>(0)),"load_inertia":Array.from({length:9},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetLoad_RequestSize(Ros2Franka_msgsSrvSetLoad_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetLoad_RequestCapture(value),n=Ros2Franka_msgsSrvSetLoad_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetLoad_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,104,90);if(owned.length<104)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetLoad_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetLoad_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"]))bad();n=addSize(n,8);
if(!Array.isArray(value["center_of_mass"])||value["center_of_mass"].length!==3)bad();
for(const element of value["center_of_mass"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["load_inertia"])||value["load_inertia"].length!==9)bad();
for(const element of value["load_inertia"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>104)bad(90);return n;}
function Ros2Franka_msgsSrvSetLoad_RequestWrite(value,w){
w.v.setFloat64(w.at,value["mass"],true);w.at+=8;
for(const element of value["center_of_mass"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["load_inertia"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetLoad_RequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
{const count=3;if(count>3)bad();value["center_of_mass"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["center_of_mass"].push(decoded);
}}
}
{
{const count=9;if(count>9)bad();value["load_inertia"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["load_inertia"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetLoad_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["center_of_mass"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["center_of_mass"]=values;
}
{const field=value["load_inertia"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["load_inertia"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetLoad_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetLoad_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_991ff575719ef9442063a13c45a9660c672f5607e6e40cf6ab5c7a3173fa00bf",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetLoad_ResponseSize(Ros2Franka_msgsSrvSetLoad_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetLoad_ResponseCapture(value),n=Ros2Franka_msgsSrvSetLoad_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetLoad_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetLoad_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetLoad_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetLoad_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetLoad_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetLoad_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetStiffnessFrame_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetStiffnessFrame_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f8ee91daba1642286b63178c18bf2a005597e94951f11a185241193132c9f402",maxWireBytes:128,minWireBytes:128,cartesianMaxWireBytes:128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"transformation":Array.from({length:16},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetStiffnessFrame_RequestSize(Ros2Franka_msgsSrvSetStiffnessFrame_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetStiffnessFrame_RequestCapture(value),n=Ros2Franka_msgsSrvSetStiffnessFrame_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetStiffnessFrame_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,128,90);if(owned.length<128)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetStiffnessFrame_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetStiffnessFrame_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["transformation"])||value["transformation"].length!==16)bad();
for(const element of value["transformation"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>128)bad(90);return n;}
function Ros2Franka_msgsSrvSetStiffnessFrame_RequestWrite(value,w){
for(const element of value["transformation"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetStiffnessFrame_RequestRead(r){const value={};
{
{const count=16;if(count>16)bad();value["transformation"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["transformation"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetStiffnessFrame_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["transformation"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["transformation"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetStiffnessFrame_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e32cb83b1f0f3cda363566525c0b1fe367190f2e66f68282b330f4517f6ea744",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetStiffnessFrame_ResponseSize(Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCapture(value),n=Ros2Franka_msgsSrvSetStiffnessFrame_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetStiffnessFrame_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetStiffnessFrame_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetStiffnessFrame_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetStiffnessFrame_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetStiffnessFrame_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetTCPFrame_Request codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetTCPFrame_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2ef445bfa494a6bc50ee3337695562255c06890216c30ba17df95e691871441f",maxWireBytes:128,minWireBytes:128,cartesianMaxWireBytes:128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"transformation":Array.from({length:16},()=>(0))};},
validateValue(value){Ros2Franka_msgsSrvSetTCPFrame_RequestSize(Ros2Franka_msgsSrvSetTCPFrame_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetTCPFrame_RequestCapture(value),n=Ros2Franka_msgsSrvSetTCPFrame_RequestSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetTCPFrame_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,128,90);if(owned.length<128)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetTCPFrame_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetTCPFrame_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["transformation"])||value["transformation"].length!==16)bad();
for(const element of value["transformation"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>128)bad(90);return n;}
function Ros2Franka_msgsSrvSetTCPFrame_RequestWrite(value,w){
for(const element of value["transformation"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros2Franka_msgsSrvSetTCPFrame_RequestRead(r){const value={};
{
{const count=16;if(count>16)bad();value["transformation"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["transformation"].push(decoded);
}}
}
return value;}
function Ros2Franka_msgsSrvSetTCPFrame_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["transformation"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["transformation"]=values;
}
return out;}
/** Complete ROS2 franka_msgs/srv/SetTCPFrame_Response codec; maxima are not actual lengths. */
export const Ros2Franka_msgsSrvSetTCPFrame_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_87e4f5556c292289fe17898a40f1f5dd6161e7cc87ca570c86e9f69080d1df38",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros2Franka_msgsSrvSetTCPFrame_ResponseSize(Ros2Franka_msgsSrvSetTCPFrame_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_msgsSrvSetTCPFrame_ResponseCapture(value),n=Ros2Franka_msgsSrvSetTCPFrame_ResponseSize(owned),w=new Writer(n);Ros2Franka_msgsSrvSetTCPFrame_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Franka_msgsSrvSetTCPFrame_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_msgsSrvSetTCPFrame_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros2Franka_msgsSrvSetTCPFrame_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros2Franka_msgsSrvSetTCPFrame_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_msgsSrvSetTCPFrame_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Goal codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsActionMoveAbsolute_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_a0c2f9037fd6730641b19b6da6ce1c6ca03bbd517fdd791c159effc93290cf13",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"position":0,"velocity":0,"acceleration":0,"deceleration":0};},
validateValue(value){Ros2Franka_spine_msgsActionMoveAbsolute_GoalSize(Ros2Franka_spine_msgsActionMoveAbsolute_GoalCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsActionMoveAbsolute_GoalCapture(value),n=Ros2Franka_spine_msgsActionMoveAbsolute_GoalSize(owned),w=new Writer(n);Ros2Franka_spine_msgsActionMoveAbsolute_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsActionMoveAbsolute_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsActionMoveAbsolute_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"]))bad();n=addSize(n,8);
if(typeof value["acceleration"]!=="number"||!Number.isFinite(value["acceleration"]))bad();n=addSize(n,8);
if(typeof value["deceleration"]!=="number"||!Number.isFinite(value["deceleration"]))bad();n=addSize(n,8);
if(n>32)bad(90);return n;}
function Ros2Franka_spine_msgsActionMoveAbsolute_GoalWrite(value,w){
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setFloat64(w.at,value["velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["acceleration"],true);w.at+=8;
w.v.setFloat64(w.at,value["deceleration"],true);w.at+=8;
}
function Ros2Franka_spine_msgsActionMoveAbsolute_GoalRead(r){const value={};
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
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["deceleration"]=decoded;
}
return value;}
function Ros2Franka_spine_msgsActionMoveAbsolute_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["position"];
out["position"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["acceleration"];
out["acceleration"]=field;
}
{const field=value["deceleration"];
out["deceleration"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Result codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsActionMoveAbsolute_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_86b5c823e6e6604595462a76cd259daef420b2dff169f6dbe00f742f079155d5",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"stop_by":"","error":""};},
validateValue(value){Ros2Franka_spine_msgsActionMoveAbsolute_ResultSize(Ros2Franka_spine_msgsActionMoveAbsolute_ResultCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsActionMoveAbsolute_ResultCapture(value),n=Ros2Franka_spine_msgsActionMoveAbsolute_ResultSize(owned),w=new Writer(n);Ros2Franka_spine_msgsActionMoveAbsolute_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsActionMoveAbsolute_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsActionMoveAbsolute_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["stop_by"],256).length);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>521)bad(90);return n;}
function Ros2Franka_spine_msgsActionMoveAbsolute_ResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["stop_by"],256);
w.text(value["error"],256);
}
function Ros2Franka_spine_msgsActionMoveAbsolute_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(256);
value["stop_by"]=decoded;
}
{
const decoded=r.text(256);
value["error"]=decoded;
}
return value;}
function Ros2Franka_spine_msgsActionMoveAbsolute_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["stop_by"];
out["stop_by"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Feedback codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_cf2f760313ab8c6cc01dd79271619926320021746451f376e56d18f63ffad929",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_position":0};},
validateValue(value){Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackSize(Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCapture(value),n=Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackSize(owned),w=new Writer(n);Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current_position"]!=="number"||!Number.isFinite(value["current_position"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackWrite(value,w){
w.v.setFloat64(w.at,value["current_position"],true);w.at+=8;
}
function Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["current_position"]=decoded;
}
return value;}
function Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_position"];
out["current_position"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/msg/UserLimits codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsMsgUserLimitsCodec=Object.freeze({codecIdentity:"UWVMROS1_20256e37f0ae710b5cb9ab9438b5ff8144ce2ab6fc606c5335d0f5de5384e368",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lower_limit":0,"upper_limit":0};},
validateValue(value){Ros2Franka_spine_msgsMsgUserLimitsSize(Ros2Franka_spine_msgsMsgUserLimitsCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsMsgUserLimitsCapture(value),n=Ros2Franka_spine_msgsMsgUserLimitsSize(owned),w=new Writer(n);Ros2Franka_spine_msgsMsgUserLimitsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsMsgUserLimitsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsMsgUserLimitsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["lower_limit"]!=="number"||!Number.isFinite(value["lower_limit"]))bad();n=addSize(n,8);
if(typeof value["upper_limit"]!=="number"||!Number.isFinite(value["upper_limit"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros2Franka_spine_msgsMsgUserLimitsWrite(value,w){
w.v.setFloat64(w.at,value["lower_limit"],true);w.at+=8;
w.v.setFloat64(w.at,value["upper_limit"],true);w.at+=8;
}
function Ros2Franka_spine_msgsMsgUserLimitsRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["lower_limit"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["upper_limit"]=decoded;
}
return value;}
function Ros2Franka_spine_msgsMsgUserLimitsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lower_limit"];
out["lower_limit"]=field;
}
{const field=value["upper_limit"];
out["upper_limit"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/msg/SpineParameters codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsMsgSpineParametersCodec=Object.freeze({codecIdentity:"UWVMROS1_4d4c45cfccf278c99a189e92d438d0b11b2b693012cb0f76b65b6822eb816223",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"user_limits":Ros2Franka_spine_msgsMsgUserLimitsCodec.defaultValue()};},
validateValue(value){Ros2Franka_spine_msgsMsgSpineParametersSize(Ros2Franka_spine_msgsMsgSpineParametersCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsMsgSpineParametersCapture(value),n=Ros2Franka_spine_msgsMsgSpineParametersSize(owned),w=new Writer(n);Ros2Franka_spine_msgsMsgSpineParametersWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsMsgSpineParametersRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsMsgSpineParametersSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Franka_spine_msgsMsgUserLimitsSize(value["user_limits"]));
if(n>16)bad(90);return n;}
function Ros2Franka_spine_msgsMsgSpineParametersWrite(value,w){
Ros2Franka_spine_msgsMsgUserLimitsWrite(value["user_limits"],w);
}
function Ros2Franka_spine_msgsMsgSpineParametersRead(r){const value={};
{
const decoded=Ros2Franka_spine_msgsMsgUserLimitsRead(r);
value["user_limits"]=decoded;
}
return value;}
function Ros2Franka_spine_msgsMsgSpineParametersCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["user_limits"];
out["user_limits"]=Ros2Franka_spine_msgsMsgUserLimitsCapture(field);
}
return out;}
/** Complete ROS2 franka_spine_msgs/msg/SpinePosition codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsMsgSpinePositionCodec=Object.freeze({codecIdentity:"UWVMROS1_71be15c40f39074dfdbe79a3587847ae512c5a3c8df23cc9415d5bfe59d05775",maxWireBytes:149,minWireBytes:21,cartesianMaxWireBytes:149n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"position":0,"valid":false};},
validateValue(value){Ros2Franka_spine_msgsMsgSpinePositionSize(Ros2Franka_spine_msgsMsgSpinePositionCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsMsgSpinePositionCapture(value),n=Ros2Franka_spine_msgsMsgSpinePositionSize(owned),w=new Writer(n);Ros2Franka_spine_msgsMsgSpinePositionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,149,90);if(owned.length<21)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsMsgSpinePositionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsMsgSpinePositionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["valid"]!=="boolean")bad();n=addSize(n,1);
if(n>149)bad(90);return n;}
function Ros2Franka_spine_msgsMsgSpinePositionWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setUint8(w.at,value["valid"]?1:0,true);w.at+=1;
}
function Ros2Franka_spine_msgsMsgSpinePositionRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["valid"]=decoded!==0;
}
return value;}
function Ros2Franka_spine_msgsMsgSpinePositionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["position"];
out["position"]=field;
}
{const field=value["valid"];
out["valid"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/msg/SpineState codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsMsgSpineStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ad0b5dc5d79956da854582765c147d3d33edab569c4829e69cc7990edfb89441",maxWireBytes:921,minWireBytes:25,cartesianMaxWireBytes:921n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"state":"","error_code":"","error_description":"","valid":false};},
validateValue(value){Ros2Franka_spine_msgsMsgSpineStateSize(Ros2Franka_spine_msgsMsgSpineStateCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsMsgSpineStateCapture(value),n=Ros2Franka_spine_msgsMsgSpineStateSize(owned),w=new Writer(n);Ros2Franka_spine_msgsMsgSpineStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,921,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsMsgSpineStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsMsgSpineStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["state"],256).length);
n=addSize(n,4+textBytes(value["error_code"],256).length);
n=addSize(n,4+textBytes(value["error_description"],256).length);
if(typeof value["valid"]!=="boolean")bad();n=addSize(n,1);
if(n>921)bad(90);return n;}
function Ros2Franka_spine_msgsMsgSpineStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["state"],256);
w.text(value["error_code"],256);
w.text(value["error_description"],256);
w.v.setUint8(w.at,value["valid"]?1:0,true);w.at+=1;
}
function Ros2Franka_spine_msgsMsgSpineStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["state"]=decoded;
}
{
const decoded=r.text(256);
value["error_code"]=decoded;
}
{
const decoded=r.text(256);
value["error_description"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["valid"]=decoded!==0;
}
return value;}
function Ros2Franka_spine_msgsMsgSpineStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["state"];
out["state"]=field;
}
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_description"];
out["error_description"]=field;
}
{const field=value["valid"];
out["valid"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/FaultReset_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvFaultReset_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0f836229e8e8a1713b9e9a8d7f0f43677ad93380e1f7350ff1597d25067b7f33",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvFaultReset_RequestSize(Ros2Franka_spine_msgsSrvFaultReset_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvFaultReset_RequestCapture(value),n=Ros2Franka_spine_msgsSrvFaultReset_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvFaultReset_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvFaultReset_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvFaultReset_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvFaultReset_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvFaultReset_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvFaultReset_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/FaultReset_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvFaultReset_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1ebeded0dbe4a4197a52de3fa43dc57fa372bd4e6ef5e2ded2558ccbf691410c",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":"","success":false,"message":""};},
validateValue(value){Ros2Franka_spine_msgsSrvFaultReset_ResponseSize(Ros2Franka_spine_msgsSrvFaultReset_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvFaultReset_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvFaultReset_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvFaultReset_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvFaultReset_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvFaultReset_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>521)bad(90);return n;}
function Ros2Franka_spine_msgsSrvFaultReset_ResponseWrite(value,w){
w.text(value["state"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Franka_spine_msgsSrvFaultReset_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
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
function Ros2Franka_spine_msgsSrvFaultReset_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetParameters_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetParameters_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_80ef5d2ef6ff2df9d14f29417d1928429315c0b7be9ec59513b9b3943a970b14",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvGetParameters_RequestSize(Ros2Franka_spine_msgsSrvGetParameters_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetParameters_RequestCapture(value),n=Ros2Franka_spine_msgsSrvGetParameters_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetParameters_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetParameters_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetParameters_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetParameters_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvGetParameters_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvGetParameters_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetParameters_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetParameters_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8e648792534d7f4996179f31d353d2594cf63031edef4451fd20a3838dc896ed",maxWireBytes:277,minWireBytes:21,cartesianMaxWireBytes:277n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"parameters":Ros2Franka_spine_msgsMsgSpineParametersCodec.defaultValue(),"success":false,"message":""};},
validateValue(value){Ros2Franka_spine_msgsSrvGetParameters_ResponseSize(Ros2Franka_spine_msgsSrvGetParameters_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetParameters_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvGetParameters_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetParameters_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,277,90);if(owned.length<21)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetParameters_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetParameters_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Franka_spine_msgsMsgSpineParametersSize(value["parameters"]));
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>277)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetParameters_ResponseWrite(value,w){
Ros2Franka_spine_msgsMsgSpineParametersWrite(value["parameters"],w);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Franka_spine_msgsSrvGetParameters_ResponseRead(r){const value={};
{
const decoded=Ros2Franka_spine_msgsMsgSpineParametersRead(r);
value["parameters"]=decoded;
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
function Ros2Franka_spine_msgsSrvGetParameters_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["parameters"];
out["parameters"]=Ros2Franka_spine_msgsMsgSpineParametersCapture(field);
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetPosition_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetPosition_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_66b05d1f08331161cafed301b3ab95a60dbb16aeb1a930ee780db163cb37c6c2",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvGetPosition_RequestSize(Ros2Franka_spine_msgsSrvGetPosition_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetPosition_RequestCapture(value),n=Ros2Franka_spine_msgsSrvGetPosition_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetPosition_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetPosition_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetPosition_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetPosition_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvGetPosition_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvGetPosition_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetPosition_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetPosition_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_18d9af57cc9f82c3c03d01e850da254e9431820b8291f03dafd2e8f74e849ab8",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"position":0,"success":false};},
validateValue(value){Ros2Franka_spine_msgsSrvGetPosition_ResponseSize(Ros2Franka_spine_msgsSrvGetPosition_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetPosition_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvGetPosition_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetPosition_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetPosition_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetPosition_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>9)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetPosition_ResponseWrite(value,w){
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Franka_spine_msgsSrvGetPosition_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Franka_spine_msgsSrvGetPosition_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["position"];
out["position"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetSpineState_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetSpineState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c4c456174d7b6647d09d986d37c041b942418941a5e01c447486727e919b8a67",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvGetSpineState_RequestSize(Ros2Franka_spine_msgsSrvGetSpineState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetSpineState_RequestCapture(value),n=Ros2Franka_spine_msgsSrvGetSpineState_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetSpineState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetSpineState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetSpineState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetSpineState_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvGetSpineState_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvGetSpineState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/GetSpineState_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvGetSpineState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_297d70e2c5c24dcea4bdf8c5f9e5c8c32256e8f35f1900d8afd914ab2acfdcd1",maxWireBytes:781,minWireBytes:13,cartesianMaxWireBytes:781n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":"","error_code":"","error_description":"","success":false};},
validateValue(value){Ros2Franka_spine_msgsSrvGetSpineState_ResponseSize(Ros2Franka_spine_msgsSrvGetSpineState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvGetSpineState_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvGetSpineState_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvGetSpineState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,781,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvGetSpineState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvGetSpineState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
n=addSize(n,4+textBytes(value["error_code"],256).length);
n=addSize(n,4+textBytes(value["error_description"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>781)bad(90);return n;}
function Ros2Franka_spine_msgsSrvGetSpineState_ResponseWrite(value,w){
w.text(value["state"],256);
w.text(value["error_code"],256);
w.text(value["error_description"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Franka_spine_msgsSrvGetSpineState_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
}
{
const decoded=r.text(256);
value["error_code"]=decoded;
}
{
const decoded=r.text(256);
value["error_description"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Franka_spine_msgsSrvGetSpineState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_description"];
out["error_description"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/Halt_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvHalt_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_50af7625abf7ec20a30ec49b01e8d32de2ee0d4710820c8de659df211be8c111",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvHalt_RequestSize(Ros2Franka_spine_msgsSrvHalt_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvHalt_RequestCapture(value),n=Ros2Franka_spine_msgsSrvHalt_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvHalt_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvHalt_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvHalt_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvHalt_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvHalt_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvHalt_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/Halt_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvHalt_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_150e1d90b485a6ddf79f771379a0e1d3f99a77f88ab1cf1f569d614a63ac3d77",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":"","success":false,"message":""};},
validateValue(value){Ros2Franka_spine_msgsSrvHalt_ResponseSize(Ros2Franka_spine_msgsSrvHalt_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvHalt_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvHalt_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvHalt_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvHalt_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvHalt_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>521)bad(90);return n;}
function Ros2Franka_spine_msgsSrvHalt_ResponseWrite(value,w){
w.text(value["state"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Franka_spine_msgsSrvHalt_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
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
function Ros2Franka_spine_msgsSrvHalt_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOff_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvSwitchOff_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_097f00adfdc8d1a49615792158a36052917c41385683b39de6686f591964fc16",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvSwitchOff_RequestSize(Ros2Franka_spine_msgsSrvSwitchOff_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvSwitchOff_RequestCapture(value),n=Ros2Franka_spine_msgsSrvSwitchOff_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvSwitchOff_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvSwitchOff_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvSwitchOff_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvSwitchOff_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvSwitchOff_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvSwitchOff_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOff_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvSwitchOff_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d3ab8516e62428b2caf71673f398ccc7199b7b25aac4c37b42c15a79878e54ad",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":"","success":false,"message":""};},
validateValue(value){Ros2Franka_spine_msgsSrvSwitchOff_ResponseSize(Ros2Franka_spine_msgsSrvSwitchOff_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvSwitchOff_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvSwitchOff_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvSwitchOff_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvSwitchOff_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvSwitchOff_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>521)bad(90);return n;}
function Ros2Franka_spine_msgsSrvSwitchOff_ResponseWrite(value,w){
w.text(value["state"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Franka_spine_msgsSrvSwitchOff_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
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
function Ros2Franka_spine_msgsSrvSwitchOff_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOn_Request codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvSwitchOn_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_840d12dbe4d4a8558e72c2facbccfc547de35536f749573b6de60ce9a9238a56",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Franka_spine_msgsSrvSwitchOn_RequestSize(Ros2Franka_spine_msgsSrvSwitchOn_RequestCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvSwitchOn_RequestCapture(value),n=Ros2Franka_spine_msgsSrvSwitchOn_RequestSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvSwitchOn_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvSwitchOn_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvSwitchOn_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Franka_spine_msgsSrvSwitchOn_RequestWrite(value,w){
}
function Ros2Franka_spine_msgsSrvSwitchOn_RequestRead(r){const value={};
return value;}
function Ros2Franka_spine_msgsSrvSwitchOn_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOn_Response codec; maxima are not actual lengths. */
export const Ros2Franka_spine_msgsSrvSwitchOn_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8c58d63385a5fc6f160585504b3c9c5ca5375c6187f7a494c3db696c5cc7a37d",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":"","success":false,"message":""};},
validateValue(value){Ros2Franka_spine_msgsSrvSwitchOn_ResponseSize(Ros2Franka_spine_msgsSrvSwitchOn_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Franka_spine_msgsSrvSwitchOn_ResponseCapture(value),n=Ros2Franka_spine_msgsSrvSwitchOn_ResponseSize(owned),w=new Writer(n);Ros2Franka_spine_msgsSrvSwitchOn_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Franka_spine_msgsSrvSwitchOn_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Franka_spine_msgsSrvSwitchOn_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["state"],256).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>521)bad(90);return n;}
function Ros2Franka_spine_msgsSrvSwitchOn_ResponseWrite(value,w){
w.text(value["state"],256);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Franka_spine_msgsSrvSwitchOn_ResponseRead(r){const value={};
{
const decoded=r.text(256);
value["state"]=decoded;
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
function Ros2Franka_spine_msgsSrvSwitchOn_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
