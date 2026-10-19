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
/** Complete ROS1 franka_example_controllers/JointTorqueComparison codec; maxima are not actual lengths. */
export const Ros1Franka_example_controllersJointTorqueComparisonCodec=Object.freeze({codecIdentity:"UWVMROS1_dde6eaf404002642e64af59eb5367948b00bc07c0b2fd8a57e359e9df3084254",maxWireBytes:176,minWireBytes:176,cartesianMaxWireBytes:176n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tau_error":Array.from({length:7},()=>(0)),"tau_commanded":Array.from({length:7},()=>(0)),"tau_measured":Array.from({length:7},()=>(0)),"root_mean_square_error":0};},
validateValue(value){Ros1Franka_example_controllersJointTorqueComparisonSize(Ros1Franka_example_controllersJointTorqueComparisonCapture(value));return true;},
encode(value){const owned=Ros1Franka_example_controllersJointTorqueComparisonCapture(value),n=Ros1Franka_example_controllersJointTorqueComparisonSize(owned),w=new Writer(n);Ros1Franka_example_controllersJointTorqueComparisonWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,176,90);if(owned.length<176)bad();const r=new Reader(owned),value=Ros1Franka_example_controllersJointTorqueComparisonRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_example_controllersJointTorqueComparisonSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["tau_error"])||value["tau_error"].length!==7)bad();
for(const element of value["tau_error"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tau_commanded"])||value["tau_commanded"].length!==7)bad();
for(const element of value["tau_commanded"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tau_measured"])||value["tau_measured"].length!==7)bad();
for(const element of value["tau_measured"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["root_mean_square_error"]!=="number"||!Number.isFinite(value["root_mean_square_error"]))bad();n=addSize(n,8);
if(n>176)bad(90);return n;}
function Ros1Franka_example_controllersJointTorqueComparisonWrite(value,w){
for(const element of value["tau_error"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["tau_commanded"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["tau_measured"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["root_mean_square_error"],true);w.at+=8;
}
function Ros1Franka_example_controllersJointTorqueComparisonRead(r){const value={};
{
{const count=7;if(count>7)bad();value["tau_error"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_error"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["tau_commanded"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_commanded"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["tau_measured"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_measured"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["root_mean_square_error"]=decoded;
}
return value;}
function Ros1Franka_example_controllersJointTorqueComparisonCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tau_error"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_error"]=values;
}
{const field=value["tau_commanded"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_commanded"]=values;
}
{const field=value["tau_measured"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_measured"]=values;
}
{const field=value["root_mean_square_error"];
out["root_mean_square_error"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/GraspEpsilon codec; maxima are not actual lengths. */
export const Ros1Franka_gripperGraspEpsilonCodec=Object.freeze({codecIdentity:"UWVMROS1_538ac801e9874841f232909d2d9a16893a8d29c8380cd623c00c408690f978f9",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"inner":0,"outer":0};},
validateValue(value){Ros1Franka_gripperGraspEpsilonSize(Ros1Franka_gripperGraspEpsilonCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperGraspEpsilonCapture(value),n=Ros1Franka_gripperGraspEpsilonSize(owned),w=new Writer(n);Ros1Franka_gripperGraspEpsilonWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Franka_gripperGraspEpsilonRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperGraspEpsilonSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["inner"]!=="number"||!Number.isFinite(value["inner"]))bad();n=addSize(n,8);
if(typeof value["outer"]!=="number"||!Number.isFinite(value["outer"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros1Franka_gripperGraspEpsilonWrite(value,w){
w.v.setFloat64(w.at,value["inner"],true);w.at+=8;
w.v.setFloat64(w.at,value["outer"],true);w.at+=8;
}
function Ros1Franka_gripperGraspEpsilonRead(r){const value={};
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
function Ros1Franka_gripperGraspEpsilonCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["inner"];
out["inner"]=field;
}
{const field=value["outer"];
out["outer"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/GraspGoal codec; maxima are not actual lengths. */
export const Ros1Franka_gripperGraspGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_d3dd0d3f47e49e06a95d907a6219335b35234749518ab645e102257595ed7bcf",maxWireBytes:40,minWireBytes:40,cartesianMaxWireBytes:40n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"width":0,"epsilon":Ros1Franka_gripperGraspEpsilonCodec.defaultValue(),"speed":0,"force":0};},
validateValue(value){Ros1Franka_gripperGraspGoalSize(Ros1Franka_gripperGraspGoalCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperGraspGoalCapture(value),n=Ros1Franka_gripperGraspGoalSize(owned),w=new Writer(n);Ros1Franka_gripperGraspGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,40,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Franka_gripperGraspGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperGraspGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["width"]!=="number"||!Number.isFinite(value["width"]))bad();n=addSize(n,8);
n=addSize(n,Ros1Franka_gripperGraspEpsilonSize(value["epsilon"]));
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"]))bad();n=addSize(n,8);
if(typeof value["force"]!=="number"||!Number.isFinite(value["force"]))bad();n=addSize(n,8);
if(n>40)bad(90);return n;}
function Ros1Franka_gripperGraspGoalWrite(value,w){
w.v.setFloat64(w.at,value["width"],true);w.at+=8;
Ros1Franka_gripperGraspEpsilonWrite(value["epsilon"],w);
w.v.setFloat64(w.at,value["speed"],true);w.at+=8;
w.v.setFloat64(w.at,value["force"],true);w.at+=8;
}
function Ros1Franka_gripperGraspGoalRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["width"]=decoded;
}
{
const decoded=Ros1Franka_gripperGraspEpsilonRead(r);
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
function Ros1Franka_gripperGraspGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["width"];
out["width"]=field;
}
{const field=value["epsilon"];
out["epsilon"]=Ros1Franka_gripperGraspEpsilonCapture(field);
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["force"];
out["force"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/GraspResult codec; maxima are not actual lengths. */
export const Ros1Franka_gripperGraspResultCodec=Object.freeze({codecIdentity:"UWVMROS1_9ce1e4f95f16fd6c1603f0b30a9fcc33e4f8436cdb96710ea91eb20b38a1e02e",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_gripperGraspResultSize(Ros1Franka_gripperGraspResultCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperGraspResultCapture(value),n=Ros1Franka_gripperGraspResultSize(owned),w=new Writer(n);Ros1Franka_gripperGraspResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_gripperGraspResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperGraspResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_gripperGraspResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_gripperGraspResultRead(r){const value={};
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
function Ros1Franka_gripperGraspResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/GraspFeedback codec; maxima are not actual lengths. */
export const Ros1Franka_gripperGraspFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_24ca23625df0bd9fb984e65c8ecd7a3af162a3db00c15951225df5e18d3a4d0d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperGraspFeedbackSize(Ros1Franka_gripperGraspFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperGraspFeedbackCapture(value),n=Ros1Franka_gripperGraspFeedbackSize(owned),w=new Writer(n);Ros1Franka_gripperGraspFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperGraspFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperGraspFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperGraspFeedbackWrite(value,w){
}
function Ros1Franka_gripperGraspFeedbackRead(r){const value={};
return value;}
function Ros1Franka_gripperGraspFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_gripper/HomingGoal codec; maxima are not actual lengths. */
export const Ros1Franka_gripperHomingGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_4b4f861fa9ff44afda4217cf613720e90353b0d43c1fc3425d5937157353910f",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperHomingGoalSize(Ros1Franka_gripperHomingGoalCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperHomingGoalCapture(value),n=Ros1Franka_gripperHomingGoalSize(owned),w=new Writer(n);Ros1Franka_gripperHomingGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperHomingGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperHomingGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperHomingGoalWrite(value,w){
}
function Ros1Franka_gripperHomingGoalRead(r){const value={};
return value;}
function Ros1Franka_gripperHomingGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_gripper/HomingResult codec; maxima are not actual lengths. */
export const Ros1Franka_gripperHomingResultCodec=Object.freeze({codecIdentity:"UWVMROS1_1ddf202b4e7dbf100d304b4eb32aa0ce8dca6ffadcf720820f2d7932af056477",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_gripperHomingResultSize(Ros1Franka_gripperHomingResultCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperHomingResultCapture(value),n=Ros1Franka_gripperHomingResultSize(owned),w=new Writer(n);Ros1Franka_gripperHomingResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_gripperHomingResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperHomingResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_gripperHomingResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_gripperHomingResultRead(r){const value={};
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
function Ros1Franka_gripperHomingResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/HomingFeedback codec; maxima are not actual lengths. */
export const Ros1Franka_gripperHomingFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_4a1118d8ab544b2bc99ff878356aae49b4d3ecb4cb5ba7e2f93f7c0c7a29512e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperHomingFeedbackSize(Ros1Franka_gripperHomingFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperHomingFeedbackCapture(value),n=Ros1Franka_gripperHomingFeedbackSize(owned),w=new Writer(n);Ros1Franka_gripperHomingFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperHomingFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperHomingFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperHomingFeedbackWrite(value,w){
}
function Ros1Franka_gripperHomingFeedbackRead(r){const value={};
return value;}
function Ros1Franka_gripperHomingFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_gripper/MoveGoal codec; maxima are not actual lengths. */
export const Ros1Franka_gripperMoveGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_c83296f2540854e78cc159691c229f5d50267b3fdb31789e325a55fcd7d6830a",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"width":0,"speed":0};},
validateValue(value){Ros1Franka_gripperMoveGoalSize(Ros1Franka_gripperMoveGoalCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperMoveGoalCapture(value),n=Ros1Franka_gripperMoveGoalSize(owned),w=new Writer(n);Ros1Franka_gripperMoveGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Franka_gripperMoveGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperMoveGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["width"]!=="number"||!Number.isFinite(value["width"]))bad();n=addSize(n,8);
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"]))bad();n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros1Franka_gripperMoveGoalWrite(value,w){
w.v.setFloat64(w.at,value["width"],true);w.at+=8;
w.v.setFloat64(w.at,value["speed"],true);w.at+=8;
}
function Ros1Franka_gripperMoveGoalRead(r){const value={};
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
function Ros1Franka_gripperMoveGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["width"];
out["width"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/MoveResult codec; maxima are not actual lengths. */
export const Ros1Franka_gripperMoveResultCodec=Object.freeze({codecIdentity:"UWVMROS1_527adf0720dabcc91866a0858cddfbfae1ed4203d4eb34a3965ebe085d0ac3b3",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_gripperMoveResultSize(Ros1Franka_gripperMoveResultCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperMoveResultCapture(value),n=Ros1Franka_gripperMoveResultSize(owned),w=new Writer(n);Ros1Franka_gripperMoveResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_gripperMoveResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperMoveResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_gripperMoveResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_gripperMoveResultRead(r){const value={};
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
function Ros1Franka_gripperMoveResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/MoveFeedback codec; maxima are not actual lengths. */
export const Ros1Franka_gripperMoveFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_8e02ca1d66ae38a5d71ca5f708aa2c57bb3df573d37b692e24986dba384c5f80",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperMoveFeedbackSize(Ros1Franka_gripperMoveFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperMoveFeedbackCapture(value),n=Ros1Franka_gripperMoveFeedbackSize(owned),w=new Writer(n);Ros1Franka_gripperMoveFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperMoveFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperMoveFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperMoveFeedbackWrite(value,w){
}
function Ros1Franka_gripperMoveFeedbackRead(r){const value={};
return value;}
function Ros1Franka_gripperMoveFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_gripper/StopGoal codec; maxima are not actual lengths. */
export const Ros1Franka_gripperStopGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_ecb09ccc03cb2140a457a11c174a614392ce1f9263e46c5b82427a99f7ff0245",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperStopGoalSize(Ros1Franka_gripperStopGoalCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperStopGoalCapture(value),n=Ros1Franka_gripperStopGoalSize(owned),w=new Writer(n);Ros1Franka_gripperStopGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperStopGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperStopGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperStopGoalWrite(value,w){
}
function Ros1Franka_gripperStopGoalRead(r){const value={};
return value;}
function Ros1Franka_gripperStopGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_gripper/StopResult codec; maxima are not actual lengths. */
export const Ros1Franka_gripperStopResultCodec=Object.freeze({codecIdentity:"UWVMROS1_343e16d9b839b61ed8f8c1ce93005cb3a21511a220095200eb075dfb62a40c61",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_gripperStopResultSize(Ros1Franka_gripperStopResultCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperStopResultCapture(value),n=Ros1Franka_gripperStopResultSize(owned),w=new Writer(n);Ros1Franka_gripperStopResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_gripperStopResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperStopResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_gripperStopResultWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_gripperStopResultRead(r){const value={};
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
function Ros1Franka_gripperStopResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_gripper/StopFeedback codec; maxima are not actual lengths. */
export const Ros1Franka_gripperStopFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_62b637081e95f59df491f71b0ca84394a7e1f36137ca389a8f6d0d3a0f19038d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_gripperStopFeedbackSize(Ros1Franka_gripperStopFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Franka_gripperStopFeedbackCapture(value),n=Ros1Franka_gripperStopFeedbackSize(owned),w=new Writer(n);Ros1Franka_gripperStopFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_gripperStopFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_gripperStopFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_gripperStopFeedbackWrite(value,w){
}
function Ros1Franka_gripperStopFeedbackRead(r){const value={};
return value;}
function Ros1Franka_gripperStopFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_msgs/ErrorRecoveryGoal codec; maxima are not actual lengths. */
export const Ros1Franka_msgsErrorRecoveryGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_b4f11555bc4919bf59aa548bd21046bdc7b1e2ed5cce6dc46298eb7486ef7b64",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_msgsErrorRecoveryGoalSize(Ros1Franka_msgsErrorRecoveryGoalCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsErrorRecoveryGoalCapture(value),n=Ros1Franka_msgsErrorRecoveryGoalSize(owned),w=new Writer(n);Ros1Franka_msgsErrorRecoveryGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_msgsErrorRecoveryGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsErrorRecoveryGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_msgsErrorRecoveryGoalWrite(value,w){
}
function Ros1Franka_msgsErrorRecoveryGoalRead(r){const value={};
return value;}
function Ros1Franka_msgsErrorRecoveryGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_msgs/ErrorRecoveryResult codec; maxima are not actual lengths. */
export const Ros1Franka_msgsErrorRecoveryResultCodec=Object.freeze({codecIdentity:"UWVMROS1_6f06297f59f06dc46e3f983b72101ae3c25cd054bca44cea1f6dd11a9aaa7794",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_msgsErrorRecoveryResultSize(Ros1Franka_msgsErrorRecoveryResultCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsErrorRecoveryResultCapture(value),n=Ros1Franka_msgsErrorRecoveryResultSize(owned),w=new Writer(n);Ros1Franka_msgsErrorRecoveryResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_msgsErrorRecoveryResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsErrorRecoveryResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_msgsErrorRecoveryResultWrite(value,w){
}
function Ros1Franka_msgsErrorRecoveryResultRead(r){const value={};
return value;}
function Ros1Franka_msgsErrorRecoveryResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_msgs/ErrorRecoveryFeedback codec; maxima are not actual lengths. */
export const Ros1Franka_msgsErrorRecoveryFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_3faee6d8afdc0524598cec0fe30fd51bd723e8e44849f035368723e59be87a21",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Franka_msgsErrorRecoveryFeedbackSize(Ros1Franka_msgsErrorRecoveryFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsErrorRecoveryFeedbackCapture(value),n=Ros1Franka_msgsErrorRecoveryFeedbackSize(owned),w=new Writer(n);Ros1Franka_msgsErrorRecoveryFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Franka_msgsErrorRecoveryFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsErrorRecoveryFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Franka_msgsErrorRecoveryFeedbackWrite(value,w){
}
function Ros1Franka_msgsErrorRecoveryFeedbackRead(r){const value={};
return value;}
function Ros1Franka_msgsErrorRecoveryFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 franka_msgs/Errors codec; maxima are not actual lengths. */
export const Ros1Franka_msgsErrorsCodec=Object.freeze({codecIdentity:"UWVMROS1_80e3d1157d53bf80d4db3621f9f22b35a1ef665382974599ff016709c6a629ca",maxWireBytes:41,minWireBytes:41,cartesianMaxWireBytes:41n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_position_limits_violation":false,"cartesian_position_limits_violation":false,"self_collision_avoidance_violation":false,"joint_velocity_violation":false,"cartesian_velocity_violation":false,"force_control_safety_violation":false,"joint_reflex":false,"cartesian_reflex":false,"max_goal_pose_deviation_violation":false,"max_path_pose_deviation_violation":false,"cartesian_velocity_profile_safety_violation":false,"joint_position_motion_generator_start_pose_invalid":false,"joint_motion_generator_position_limits_violation":false,"joint_motion_generator_velocity_limits_violation":false,"joint_motion_generator_velocity_discontinuity":false,"joint_motion_generator_acceleration_discontinuity":false,"cartesian_position_motion_generator_start_pose_invalid":false,"cartesian_motion_generator_elbow_limit_violation":false,"cartesian_motion_generator_velocity_limits_violation":false,"cartesian_motion_generator_velocity_discontinuity":false,"cartesian_motion_generator_acceleration_discontinuity":false,"cartesian_motion_generator_elbow_sign_inconsistent":false,"cartesian_motion_generator_start_elbow_invalid":false,"cartesian_motion_generator_joint_position_limits_violation":false,"cartesian_motion_generator_joint_velocity_limits_violation":false,"cartesian_motion_generator_joint_velocity_discontinuity":false,"cartesian_motion_generator_joint_acceleration_discontinuity":false,"cartesian_position_motion_generator_invalid_frame":false,"force_controller_desired_force_tolerance_violation":false,"controller_torque_discontinuity":false,"start_elbow_sign_inconsistent":false,"communication_constraints_violation":false,"power_limit_violation":false,"joint_p2p_insufficient_torque_for_planning":false,"tau_j_range_violation":false,"instability_detected":false,"joint_move_in_wrong_direction":false,"cartesian_spline_motion_generator_violation":false,"joint_via_motion_generator_planning_joint_limit_violation":false,"base_acceleration_initialization_timeout":false,"base_acceleration_invalid_reading":false};},
validateValue(value){Ros1Franka_msgsErrorsSize(Ros1Franka_msgsErrorsCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsErrorsCapture(value),n=Ros1Franka_msgsErrorsSize(owned),w=new Writer(n);Ros1Franka_msgsErrorsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,41,90);if(owned.length<41)bad();const r=new Reader(owned),value=Ros1Franka_msgsErrorsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsErrorsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
if(typeof value["joint_move_in_wrong_direction"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["cartesian_spline_motion_generator_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["joint_via_motion_generator_planning_joint_limit_violation"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["base_acceleration_initialization_timeout"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["base_acceleration_invalid_reading"]!=="boolean")bad();n=addSize(n,1);
if(n>41)bad(90);return n;}
function Ros1Franka_msgsErrorsWrite(value,w){
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
w.v.setUint8(w.at,value["joint_move_in_wrong_direction"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["cartesian_spline_motion_generator_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["joint_via_motion_generator_planning_joint_limit_violation"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["base_acceleration_initialization_timeout"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["base_acceleration_invalid_reading"]?1:0,true);w.at+=1;
}
function Ros1Franka_msgsErrorsRead(r){const value={};
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
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_move_in_wrong_direction"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["cartesian_spline_motion_generator_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["joint_via_motion_generator_planning_joint_limit_violation"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["base_acceleration_initialization_timeout"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["base_acceleration_invalid_reading"]=decoded!==0;
}
return value;}
function Ros1Franka_msgsErrorsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["joint_move_in_wrong_direction"];
out["joint_move_in_wrong_direction"]=field;
}
{const field=value["cartesian_spline_motion_generator_violation"];
out["cartesian_spline_motion_generator_violation"]=field;
}
{const field=value["joint_via_motion_generator_planning_joint_limit_violation"];
out["joint_via_motion_generator_planning_joint_limit_violation"]=field;
}
{const field=value["base_acceleration_initialization_timeout"];
out["base_acceleration_initialization_timeout"]=field;
}
{const field=value["base_acceleration_invalid_reading"];
out["base_acceleration_invalid_reading"]=field;
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
/** Complete ROS1 franka_msgs/FrankaState codec; maxima are not actual lengths. */
export const Ros1Franka_msgsFrankaStateCodec=Object.freeze({codecIdentity:"UWVMROS1_79f22f36ce2c11f519ee29e65a248ec1b67240e27712b2667e9b803a2562ee71",maxWireBytes:2619,minWireBytes:2491,cartesianMaxWireBytes:2619n,aggregateWireBytes:0,constants:Object.freeze({"ROBOT_MODE_OTHER":0,"ROBOT_MODE_IDLE":1,"ROBOT_MODE_MOVE":2,"ROBOT_MODE_GUIDING":3,"ROBOT_MODE_REFLEX":4,"ROBOT_MODE_USER_STOPPED":5,"ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY":6}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"cartesian_collision":Array.from({length:6},()=>(0)),"cartesian_contact":Array.from({length:6},()=>(0)),"q":Array.from({length:7},()=>(0)),"q_d":Array.from({length:7},()=>(0)),"dq":Array.from({length:7},()=>(0)),"dq_d":Array.from({length:7},()=>(0)),"ddq_d":Array.from({length:7},()=>(0)),"theta":Array.from({length:7},()=>(0)),"dtheta":Array.from({length:7},()=>(0)),"tau_J":Array.from({length:7},()=>(0)),"dtau_J":Array.from({length:7},()=>(0)),"tau_J_d":Array.from({length:7},()=>(0)),"K_F_ext_hat_K":Array.from({length:6},()=>(0)),"elbow":Array.from({length:2},()=>(0)),"elbow_d":Array.from({length:2},()=>(0)),"elbow_c":Array.from({length:2},()=>(0)),"delbow_c":Array.from({length:2},()=>(0)),"ddelbow_c":Array.from({length:2},()=>(0)),"joint_collision":Array.from({length:7},()=>(0)),"joint_contact":Array.from({length:7},()=>(0)),"O_F_ext_hat_K":Array.from({length:6},()=>(0)),"O_dP_EE_d":Array.from({length:6},()=>(0)),"O_ddP_O":Array.from({length:3},()=>(0)),"O_dP_EE_c":Array.from({length:6},()=>(0)),"O_ddP_EE_c":Array.from({length:6},()=>(0)),"tau_ext_hat_filtered":Array.from({length:7},()=>(0)),"m_ee":0,"F_x_Cee":Array.from({length:3},()=>(0)),"I_ee":Array.from({length:9},()=>(0)),"m_load":0,"F_x_Cload":Array.from({length:3},()=>(0)),"I_load":Array.from({length:9},()=>(0)),"m_total":0,"F_x_Ctotal":Array.from({length:3},()=>(0)),"I_total":Array.from({length:9},()=>(0)),"O_T_EE":Array.from({length:16},()=>(0)),"O_T_EE_d":Array.from({length:16},()=>(0)),"O_T_EE_c":Array.from({length:16},()=>(0)),"F_T_EE":Array.from({length:16},()=>(0)),"F_T_NE":Array.from({length:16},()=>(0)),"NE_T_EE":Array.from({length:16},()=>(0)),"EE_T_K":Array.from({length:16},()=>(0)),"time":0,"control_command_success_rate":0,"robot_mode":0,"current_errors":Ros1Franka_msgsErrorsCodec.defaultValue(),"last_motion_errors":Ros1Franka_msgsErrorsCodec.defaultValue()};},
validateValue(value){Ros1Franka_msgsFrankaStateSize(Ros1Franka_msgsFrankaStateCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsFrankaStateCapture(value),n=Ros1Franka_msgsFrankaStateSize(owned),w=new Writer(n);Ros1Franka_msgsFrankaStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2619,90);if(owned.length<2491)bad();const r=new Reader(owned),value=Ros1Franka_msgsFrankaStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsFrankaStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["cartesian_collision"])||value["cartesian_collision"].length!==6)bad();
for(const element of value["cartesian_collision"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["cartesian_contact"])||value["cartesian_contact"].length!==6)bad();
for(const element of value["cartesian_contact"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["q"])||value["q"].length!==7)bad();
for(const element of value["q"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["q_d"])||value["q_d"].length!==7)bad();
for(const element of value["q_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["dq"])||value["dq"].length!==7)bad();
for(const element of value["dq"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["dq_d"])||value["dq_d"].length!==7)bad();
for(const element of value["dq_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["ddq_d"])||value["ddq_d"].length!==7)bad();
for(const element of value["ddq_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["theta"])||value["theta"].length!==7)bad();
for(const element of value["theta"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["dtheta"])||value["dtheta"].length!==7)bad();
for(const element of value["dtheta"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tau_J"])||value["tau_J"].length!==7)bad();
for(const element of value["tau_J"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["dtau_J"])||value["dtau_J"].length!==7)bad();
for(const element of value["dtau_J"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tau_J_d"])||value["tau_J_d"].length!==7)bad();
for(const element of value["tau_J_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["K_F_ext_hat_K"])||value["K_F_ext_hat_K"].length!==6)bad();
for(const element of value["K_F_ext_hat_K"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["elbow"])||value["elbow"].length!==2)bad();
for(const element of value["elbow"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["elbow_d"])||value["elbow_d"].length!==2)bad();
for(const element of value["elbow_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["elbow_c"])||value["elbow_c"].length!==2)bad();
for(const element of value["elbow_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["delbow_c"])||value["delbow_c"].length!==2)bad();
for(const element of value["delbow_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["ddelbow_c"])||value["ddelbow_c"].length!==2)bad();
for(const element of value["ddelbow_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["joint_collision"])||value["joint_collision"].length!==7)bad();
for(const element of value["joint_collision"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["joint_contact"])||value["joint_contact"].length!==7)bad();
for(const element of value["joint_contact"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_F_ext_hat_K"])||value["O_F_ext_hat_K"].length!==6)bad();
for(const element of value["O_F_ext_hat_K"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_dP_EE_d"])||value["O_dP_EE_d"].length!==6)bad();
for(const element of value["O_dP_EE_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_ddP_O"])||value["O_ddP_O"].length!==3)bad();
for(const element of value["O_ddP_O"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_dP_EE_c"])||value["O_dP_EE_c"].length!==6)bad();
for(const element of value["O_dP_EE_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_ddP_EE_c"])||value["O_ddP_EE_c"].length!==6)bad();
for(const element of value["O_ddP_EE_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["tau_ext_hat_filtered"])||value["tau_ext_hat_filtered"].length!==7)bad();
for(const element of value["tau_ext_hat_filtered"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["m_ee"]!=="number"||!Number.isFinite(value["m_ee"]))bad();n=addSize(n,8);
if(!Array.isArray(value["F_x_Cee"])||value["F_x_Cee"].length!==3)bad();
for(const element of value["F_x_Cee"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["I_ee"])||value["I_ee"].length!==9)bad();
for(const element of value["I_ee"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["m_load"]!=="number"||!Number.isFinite(value["m_load"]))bad();n=addSize(n,8);
if(!Array.isArray(value["F_x_Cload"])||value["F_x_Cload"].length!==3)bad();
for(const element of value["F_x_Cload"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["I_load"])||value["I_load"].length!==9)bad();
for(const element of value["I_load"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["m_total"]!=="number"||!Number.isFinite(value["m_total"]))bad();n=addSize(n,8);
if(!Array.isArray(value["F_x_Ctotal"])||value["F_x_Ctotal"].length!==3)bad();
for(const element of value["F_x_Ctotal"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["I_total"])||value["I_total"].length!==9)bad();
for(const element of value["I_total"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_T_EE"])||value["O_T_EE"].length!==16)bad();
for(const element of value["O_T_EE"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_T_EE_d"])||value["O_T_EE_d"].length!==16)bad();
for(const element of value["O_T_EE_d"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["O_T_EE_c"])||value["O_T_EE_c"].length!==16)bad();
for(const element of value["O_T_EE_c"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["F_T_EE"])||value["F_T_EE"].length!==16)bad();
for(const element of value["F_T_EE"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["F_T_NE"])||value["F_T_NE"].length!==16)bad();
for(const element of value["F_T_NE"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["NE_T_EE"])||value["NE_T_EE"].length!==16)bad();
for(const element of value["NE_T_EE"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["EE_T_K"])||value["EE_T_K"].length!==16)bad();
for(const element of value["EE_T_K"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(typeof value["time"]!=="number"||!Number.isFinite(value["time"]))bad();n=addSize(n,8);
if(typeof value["control_command_success_rate"]!=="number"||!Number.isFinite(value["control_command_success_rate"]))bad();n=addSize(n,8);
integer(value["robot_mode"],1,false);n=addSize(n,1);
n=addSize(n,Ros1Franka_msgsErrorsSize(value["current_errors"]));
n=addSize(n,Ros1Franka_msgsErrorsSize(value["last_motion_errors"]));
if(n>2619)bad(90);return n;}
function Ros1Franka_msgsFrankaStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
for(const element of value["cartesian_collision"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["cartesian_contact"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["q"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["q_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["dq"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["dq_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["ddq_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["theta"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["dtheta"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["tau_J"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["dtau_J"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["tau_J_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["K_F_ext_hat_K"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["elbow"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["elbow_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["elbow_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["delbow_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["ddelbow_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["joint_collision"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["joint_contact"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_F_ext_hat_K"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_dP_EE_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_ddP_O"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_dP_EE_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_ddP_EE_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["tau_ext_hat_filtered"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["m_ee"],true);w.at+=8;
for(const element of value["F_x_Cee"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["I_ee"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["m_load"],true);w.at+=8;
for(const element of value["F_x_Cload"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["I_load"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["m_total"],true);w.at+=8;
for(const element of value["F_x_Ctotal"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["I_total"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_T_EE"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_T_EE_d"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["O_T_EE_c"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["F_T_EE"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["F_T_NE"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["NE_T_EE"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["EE_T_K"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setFloat64(w.at,value["time"],true);w.at+=8;
w.v.setFloat64(w.at,value["control_command_success_rate"],true);w.at+=8;
w.v.setUint8(w.at,value["robot_mode"],true);w.at+=1;
Ros1Franka_msgsErrorsWrite(value["current_errors"],w);
Ros1Franka_msgsErrorsWrite(value["last_motion_errors"],w);
}
function Ros1Franka_msgsFrankaStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=6;if(count>6)bad();value["cartesian_collision"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["cartesian_collision"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["cartesian_contact"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["cartesian_contact"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["q"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["q"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["q_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["q_d"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["dq"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dq"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["dq_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dq_d"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["ddq_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ddq_d"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["theta"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["theta"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["dtheta"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dtheta"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["tau_J"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_J"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["dtau_J"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["dtau_J"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["tau_J_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_J_d"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["K_F_ext_hat_K"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["K_F_ext_hat_K"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["elbow"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elbow"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["elbow_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elbow_d"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["elbow_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["elbow_c"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["delbow_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["delbow_c"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["ddelbow_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["ddelbow_c"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["joint_collision"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_collision"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["joint_contact"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_contact"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["O_F_ext_hat_K"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_F_ext_hat_K"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["O_dP_EE_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_dP_EE_d"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["O_ddP_O"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_ddP_O"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["O_dP_EE_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_dP_EE_c"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["O_ddP_EE_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_ddP_EE_c"].push(decoded);
}}
}
{
{const count=7;if(count>7)bad();value["tau_ext_hat_filtered"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tau_ext_hat_filtered"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["m_ee"]=decoded;
}
{
{const count=3;if(count>3)bad();value["F_x_Cee"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_x_Cee"].push(decoded);
}}
}
{
{const count=9;if(count>9)bad();value["I_ee"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["I_ee"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["m_load"]=decoded;
}
{
{const count=3;if(count>3)bad();value["F_x_Cload"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_x_Cload"].push(decoded);
}}
}
{
{const count=9;if(count>9)bad();value["I_load"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["I_load"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["m_total"]=decoded;
}
{
{const count=3;if(count>3)bad();value["F_x_Ctotal"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_x_Ctotal"].push(decoded);
}}
}
{
{const count=9;if(count>9)bad();value["I_total"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["I_total"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["O_T_EE"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_T_EE"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["O_T_EE_d"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_T_EE_d"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["O_T_EE_c"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["O_T_EE_c"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["F_T_EE"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_T_EE"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["F_T_NE"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_T_NE"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["NE_T_EE"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["NE_T_EE"].push(decoded);
}}
}
{
{const count=16;if(count>16)bad();value["EE_T_K"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["EE_T_K"].push(decoded);
}}
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
const decoded=Ros1Franka_msgsErrorsRead(r);
value["current_errors"]=decoded;
}
{
const decoded=Ros1Franka_msgsErrorsRead(r);
value["last_motion_errors"]=decoded;
}
return value;}
function Ros1Franka_msgsFrankaStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["cartesian_collision"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cartesian_collision"]=values;
}
{const field=value["cartesian_contact"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cartesian_contact"]=values;
}
{const field=value["q"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q"]=values;
}
{const field=value["q_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["q_d"]=values;
}
{const field=value["dq"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["dq"]=values;
}
{const field=value["dq_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["dq_d"]=values;
}
{const field=value["ddq_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["ddq_d"]=values;
}
{const field=value["theta"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["theta"]=values;
}
{const field=value["dtheta"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["dtheta"]=values;
}
{const field=value["tau_J"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_J"]=values;
}
{const field=value["dtau_J"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["dtau_J"]=values;
}
{const field=value["tau_J_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_J_d"]=values;
}
{const field=value["K_F_ext_hat_K"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["K_F_ext_hat_K"]=values;
}
{const field=value["elbow"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["elbow"]=values;
}
{const field=value["elbow_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["elbow_d"]=values;
}
{const field=value["elbow_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["elbow_c"]=values;
}
{const field=value["delbow_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["delbow_c"]=values;
}
{const field=value["ddelbow_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["ddelbow_c"]=values;
}
{const field=value["joint_collision"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_collision"]=values;
}
{const field=value["joint_contact"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_contact"]=values;
}
{const field=value["O_F_ext_hat_K"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_F_ext_hat_K"]=values;
}
{const field=value["O_dP_EE_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_dP_EE_d"]=values;
}
{const field=value["O_ddP_O"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_ddP_O"]=values;
}
{const field=value["O_dP_EE_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_dP_EE_c"]=values;
}
{const field=value["O_ddP_EE_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_ddP_EE_c"]=values;
}
{const field=value["tau_ext_hat_filtered"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["tau_ext_hat_filtered"]=values;
}
{const field=value["m_ee"];
out["m_ee"]=field;
}
{const field=value["F_x_Cee"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_x_Cee"]=values;
}
{const field=value["I_ee"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["I_ee"]=values;
}
{const field=value["m_load"];
out["m_load"]=field;
}
{const field=value["F_x_Cload"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_x_Cload"]=values;
}
{const field=value["I_load"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["I_load"]=values;
}
{const field=value["m_total"];
out["m_total"]=field;
}
{const field=value["F_x_Ctotal"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_x_Ctotal"]=values;
}
{const field=value["I_total"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["I_total"]=values;
}
{const field=value["O_T_EE"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_T_EE"]=values;
}
{const field=value["O_T_EE_d"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_T_EE_d"]=values;
}
{const field=value["O_T_EE_c"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["O_T_EE_c"]=values;
}
{const field=value["F_T_EE"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_T_EE"]=values;
}
{const field=value["F_T_NE"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_T_NE"]=values;
}
{const field=value["NE_T_EE"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["NE_T_EE"]=values;
}
{const field=value["EE_T_K"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["EE_T_K"]=values;
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
out["current_errors"]=Ros1Franka_msgsErrorsCapture(field);
}
{const field=value["last_motion_errors"];
out["last_motion_errors"]=Ros1Franka_msgsErrorsCapture(field);
}
return out;}
/** Complete ROS1 franka_msgs/SetCartesianImpedanceRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetCartesianImpedanceRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ff604aae9ab1ef36b5af107966e64ee2115ee17618a4ece6f15997c889cacbdf",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cartesian_stiffness":Array.from({length:6},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetCartesianImpedanceRequestSize(Ros1Franka_msgsSetCartesianImpedanceRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetCartesianImpedanceRequestCapture(value),n=Ros1Franka_msgsSetCartesianImpedanceRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetCartesianImpedanceRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetCartesianImpedanceRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetCartesianImpedanceRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["cartesian_stiffness"])||value["cartesian_stiffness"].length!==6)bad();
for(const element of value["cartesian_stiffness"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>48)bad(90);return n;}
function Ros1Franka_msgsSetCartesianImpedanceRequestWrite(value,w){
for(const element of value["cartesian_stiffness"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Franka_msgsSetCartesianImpedanceRequestRead(r){const value={};
{
{const count=6;if(count>6)bad();value["cartesian_stiffness"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["cartesian_stiffness"].push(decoded);
}}
}
return value;}
function Ros1Franka_msgsSetCartesianImpedanceRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cartesian_stiffness"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cartesian_stiffness"]=values;
}
return out;}
/** Complete ROS1 franka_msgs/SetCartesianImpedanceResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetCartesianImpedanceResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b091d2fe98c84528460ba3d84524c38355e725c161456edf0034e62c6f63ab5b",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetCartesianImpedanceResponseSize(Ros1Franka_msgsSetCartesianImpedanceResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetCartesianImpedanceResponseCapture(value),n=Ros1Franka_msgsSetCartesianImpedanceResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetCartesianImpedanceResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetCartesianImpedanceResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetCartesianImpedanceResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetCartesianImpedanceResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetCartesianImpedanceResponseRead(r){const value={};
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
function Ros1Franka_msgsSetCartesianImpedanceResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetEEFrameRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetEEFrameRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1bc740d3998a14dd6c0604d5024ebda2b4916a8497950841b14a8073c6c1ecfc",maxWireBytes:128,minWireBytes:128,cartesianMaxWireBytes:128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"NE_T_EE":Array.from({length:16},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetEEFrameRequestSize(Ros1Franka_msgsSetEEFrameRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetEEFrameRequestCapture(value),n=Ros1Franka_msgsSetEEFrameRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetEEFrameRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,128,90);if(owned.length<128)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetEEFrameRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetEEFrameRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["NE_T_EE"])||value["NE_T_EE"].length!==16)bad();
for(const element of value["NE_T_EE"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>128)bad(90);return n;}
function Ros1Franka_msgsSetEEFrameRequestWrite(value,w){
for(const element of value["NE_T_EE"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Franka_msgsSetEEFrameRequestRead(r){const value={};
{
{const count=16;if(count>16)bad();value["NE_T_EE"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["NE_T_EE"].push(decoded);
}}
}
return value;}
function Ros1Franka_msgsSetEEFrameRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["NE_T_EE"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["NE_T_EE"]=values;
}
return out;}
/** Complete ROS1 franka_msgs/SetEEFrameResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetEEFrameResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b40c9595d57d6ed52ea4eca78e21f2c3d81f5a0afaad2420511211931f7d1c67",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetEEFrameResponseSize(Ros1Franka_msgsSetEEFrameResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetEEFrameResponseCapture(value),n=Ros1Franka_msgsSetEEFrameResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetEEFrameResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetEEFrameResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetEEFrameResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetEEFrameResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetEEFrameResponseRead(r){const value={};
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
function Ros1Franka_msgsSetEEFrameResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetForceTorqueCollisionBehaviorRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2513f0ea74bf6a871a85ce00c0f822891f2c0bed890b87e35d25a637aa62c4b5",maxWireBytes:208,minWireBytes:208,cartesianMaxWireBytes:208n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lower_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"upper_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"lower_force_thresholds_nominal":Array.from({length:6},()=>(0)),"upper_force_thresholds_nominal":Array.from({length:6},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestSize(Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCapture(value),n=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,208,90);if(owned.length<208)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestWrite(value,w){
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
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestRead(r){const value={};
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
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 franka_msgs/SetForceTorqueCollisionBehaviorResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_72418d73d504ae385d7ddce7e3cef859a104346dbfea7fc9da82e0f7e33ca2f5",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseSize(Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCapture(value),n=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseRead(r){const value={};
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
function Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetFullCollisionBehaviorRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetFullCollisionBehaviorRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_47a86620ec5abdd6cc820b095d8f5f4214c1d9af00ac1ff0740137bbdda7217c",maxWireBytes:416,minWireBytes:416,cartesianMaxWireBytes:416n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lower_torque_thresholds_acceleration":Array.from({length:7},()=>(0)),"upper_torque_thresholds_acceleration":Array.from({length:7},()=>(0)),"lower_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"upper_torque_thresholds_nominal":Array.from({length:7},()=>(0)),"lower_force_thresholds_acceleration":Array.from({length:6},()=>(0)),"upper_force_thresholds_acceleration":Array.from({length:6},()=>(0)),"lower_force_thresholds_nominal":Array.from({length:6},()=>(0)),"upper_force_thresholds_nominal":Array.from({length:6},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetFullCollisionBehaviorRequestSize(Ros1Franka_msgsSetFullCollisionBehaviorRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetFullCollisionBehaviorRequestCapture(value),n=Ros1Franka_msgsSetFullCollisionBehaviorRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetFullCollisionBehaviorRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,416,90);if(owned.length<416)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetFullCollisionBehaviorRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetFullCollisionBehaviorRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros1Franka_msgsSetFullCollisionBehaviorRequestWrite(value,w){
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
function Ros1Franka_msgsSetFullCollisionBehaviorRequestRead(r){const value={};
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
function Ros1Franka_msgsSetFullCollisionBehaviorRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 franka_msgs/SetFullCollisionBehaviorResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetFullCollisionBehaviorResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_26f46e286c42d51a8ef45db7c8b3bf315fb02faf7b2be5d3741563d43169a0e2",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetFullCollisionBehaviorResponseSize(Ros1Franka_msgsSetFullCollisionBehaviorResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetFullCollisionBehaviorResponseCapture(value),n=Ros1Franka_msgsSetFullCollisionBehaviorResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetFullCollisionBehaviorResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetFullCollisionBehaviorResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetFullCollisionBehaviorResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetFullCollisionBehaviorResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetFullCollisionBehaviorResponseRead(r){const value={};
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
function Ros1Franka_msgsSetFullCollisionBehaviorResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetJointImpedanceRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetJointImpedanceRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_29b5419f698b39452d38fbc58a0e3c572b8d285b70c8acf7d326c440eb417c53",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_stiffness":Array.from({length:7},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetJointImpedanceRequestSize(Ros1Franka_msgsSetJointImpedanceRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetJointImpedanceRequestCapture(value),n=Ros1Franka_msgsSetJointImpedanceRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetJointImpedanceRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetJointImpedanceRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetJointImpedanceRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["joint_stiffness"])||value["joint_stiffness"].length!==7)bad();
for(const element of value["joint_stiffness"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>56)bad(90);return n;}
function Ros1Franka_msgsSetJointImpedanceRequestWrite(value,w){
for(const element of value["joint_stiffness"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Franka_msgsSetJointImpedanceRequestRead(r){const value={};
{
{const count=7;if(count>7)bad();value["joint_stiffness"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_stiffness"].push(decoded);
}}
}
return value;}
function Ros1Franka_msgsSetJointImpedanceRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_stiffness"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==7)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_stiffness"]=values;
}
return out;}
/** Complete ROS1 franka_msgs/SetJointImpedanceResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetJointImpedanceResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1ae0cd7c4ad89de09ccffd6c4999435fa6965509e99a9adb98b7f3ebb3ea4c55",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetJointImpedanceResponseSize(Ros1Franka_msgsSetJointImpedanceResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetJointImpedanceResponseCapture(value),n=Ros1Franka_msgsSetJointImpedanceResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetJointImpedanceResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetJointImpedanceResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetJointImpedanceResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetJointImpedanceResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetJointImpedanceResponseRead(r){const value={};
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
function Ros1Franka_msgsSetJointImpedanceResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetKFrameRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetKFrameRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_36afcdc6402259fb5a71ae1b27be0bb8623a64dd93caffa376adea7862c86446",maxWireBytes:128,minWireBytes:128,cartesianMaxWireBytes:128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"EE_T_K":Array.from({length:16},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetKFrameRequestSize(Ros1Franka_msgsSetKFrameRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetKFrameRequestCapture(value),n=Ros1Franka_msgsSetKFrameRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetKFrameRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,128,90);if(owned.length<128)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetKFrameRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetKFrameRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["EE_T_K"])||value["EE_T_K"].length!==16)bad();
for(const element of value["EE_T_K"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>128)bad(90);return n;}
function Ros1Franka_msgsSetKFrameRequestWrite(value,w){
for(const element of value["EE_T_K"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Franka_msgsSetKFrameRequestRead(r){const value={};
{
{const count=16;if(count>16)bad();value["EE_T_K"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["EE_T_K"].push(decoded);
}}
}
return value;}
function Ros1Franka_msgsSetKFrameRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["EE_T_K"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["EE_T_K"]=values;
}
return out;}
/** Complete ROS1 franka_msgs/SetKFrameResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetKFrameResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4ea77d3e827a9b9eaf3c3f9805f78c38bfe91789555d0469cffd4ff4266cd443",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetKFrameResponseSize(Ros1Franka_msgsSetKFrameResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetKFrameResponseCapture(value),n=Ros1Franka_msgsSetKFrameResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetKFrameResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetKFrameResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetKFrameResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetKFrameResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetKFrameResponseRead(r){const value={};
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
function Ros1Franka_msgsSetKFrameResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
/** Complete ROS1 franka_msgs/SetLoadRequest codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetLoadRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d7c08dd9105cb7b83cb251e80c7d2dd1e9f7bb2a43f614bff6d00d808cccbcb5",maxWireBytes:104,minWireBytes:104,cartesianMaxWireBytes:104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"F_x_center_load":Array.from({length:3},()=>(0)),"load_inertia":Array.from({length:9},()=>(0))};},
validateValue(value){Ros1Franka_msgsSetLoadRequestSize(Ros1Franka_msgsSetLoadRequestCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetLoadRequestCapture(value),n=Ros1Franka_msgsSetLoadRequestSize(owned),w=new Writer(n);Ros1Franka_msgsSetLoadRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,104,90);if(owned.length<104)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetLoadRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetLoadRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"]))bad();n=addSize(n,8);
if(!Array.isArray(value["F_x_center_load"])||value["F_x_center_load"].length!==3)bad();
for(const element of value["F_x_center_load"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["load_inertia"])||value["load_inertia"].length!==9)bad();
for(const element of value["load_inertia"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(n>104)bad(90);return n;}
function Ros1Franka_msgsSetLoadRequestWrite(value,w){
w.v.setFloat64(w.at,value["mass"],true);w.at+=8;
for(const element of value["F_x_center_load"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
for(const element of value["load_inertia"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Franka_msgsSetLoadRequestRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
{const count=3;if(count>3)bad();value["F_x_center_load"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["F_x_center_load"].push(decoded);
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
function Ros1Franka_msgsSetLoadRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["F_x_center_load"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["F_x_center_load"]=values;
}
{const field=value["load_inertia"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["load_inertia"]=values;
}
return out;}
/** Complete ROS1 franka_msgs/SetLoadResponse codec; maxima are not actual lengths. */
export const Ros1Franka_msgsSetLoadResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f18952407bf9e2813deeaa3734a781e90b74cb79892919c1218812ad71255cf3",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"error":""};},
validateValue(value){Ros1Franka_msgsSetLoadResponseSize(Ros1Franka_msgsSetLoadResponseCapture(value));return true;},
encode(value){const owned=Ros1Franka_msgsSetLoadResponseCapture(value),n=Ros1Franka_msgsSetLoadResponseSize(owned),w=new Writer(n);Ros1Franka_msgsSetLoadResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Franka_msgsSetLoadResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Franka_msgsSetLoadResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["error"],256).length);
if(n>261)bad(90);return n;}
function Ros1Franka_msgsSetLoadResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["error"],256);
}
function Ros1Franka_msgsSetLoadResponseRead(r){const value={};
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
function Ros1Franka_msgsSetLoadResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["error"];
out["error"]=field;
}
return out;}
