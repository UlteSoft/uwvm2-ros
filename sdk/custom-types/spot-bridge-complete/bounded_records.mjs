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
/** Complete ROS2 uwvm_spot_bridge/srv/Command_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvCommand_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_29996d4dacb4ea847c5d629cb634d4aa5e2451627d2469d6cdf301cc007968f8",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"budget_ns":0n};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvCommand_RequestSize(Ros2Uwvm_spot_bridgeSrvCommand_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvCommand_RequestCapture(value),n=Ros2Uwvm_spot_bridgeSrvCommand_RequestSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvCommand_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvCommand_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvCommand_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["budget_ns"],8,false);n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvCommand_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setBigUint64(w.at,value["budget_ns"],true);w.at+=8;
}
function Ros2Uwvm_spot_bridgeSrvCommand_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["budget_ns"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvCommand_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["budget_ns"];
out["budget_ns"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Command_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvCommand_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e35556e7439c8d5ca79cb0f4ad23cf73c85a8a79d1382cde398dd85082dd1733",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"error":0,"unknown":false,"native_accepted":false,"native_command_id":0};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvCommand_ResponseSize(Ros2Uwvm_spot_bridgeSrvCommand_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvCommand_ResponseCapture(value),n=Ros2Uwvm_spot_bridgeSrvCommand_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvCommand_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvCommand_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvCommand_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["error"],4,false);n=addSize(n,4);
if(typeof value["unknown"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["native_accepted"]!=="boolean")bad();n=addSize(n,1);
integer(value["native_command_id"],4,false);n=addSize(n,4);
if(n>26)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvCommand_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["error"],true);w.at+=4;
w.v.setUint8(w.at,value["unknown"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["native_accepted"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["native_command_id"],true);w.at+=4;
}
function Ros2Uwvm_spot_bridgeSrvCommand_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["unknown"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["native_accepted"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["native_command_id"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvCommand_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["error"];
out["error"]=field;
}
{const field=value["unknown"];
out["unknown"]=field;
}
{const field=value["native_accepted"];
out["native_accepted"]=field;
}
{const field=value["native_command_id"];
out["native_command_id"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Feedback_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvFeedback_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_de64ce283f49f5c324912ec262f7d15b6fd828879c01039d455c765743c45ba3",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"budget_ns":0n,"command_token":0n};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvFeedback_RequestSize(Ros2Uwvm_spot_bridgeSrvFeedback_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvFeedback_RequestCapture(value),n=Ros2Uwvm_spot_bridgeSrvFeedback_RequestSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvFeedback_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvFeedback_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvFeedback_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["budget_ns"],8,false);n=addSize(n,8);
integer(value["command_token"],8,false);n=addSize(n,8);
if(n>32)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvFeedback_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setBigUint64(w.at,value["budget_ns"],true);w.at+=8;
w.v.setBigUint64(w.at,value["command_token"],true);w.at+=8;
}
function Ros2Uwvm_spot_bridgeSrvFeedback_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["budget_ns"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["command_token"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvFeedback_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["budget_ns"];
out["budget_ns"]=field;
}
{const field=value["command_token"];
out["command_token"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Feedback_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvFeedback_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_dc801501c5f5c993eea419f4a94f5c7eb57a5cfe113d0351a3b1e3fbfd514901",maxWireBytes:16429,minWireBytes:45,cartesianMaxWireBytes:16429n,aggregateWireBytes:32768,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"error":0,"unknown":false,"native_command_id":0,"has_full_body_feedback":false,"full_body_status":0,"has_mobility_feedback":false,"mobility_status":0,"feedback_kind":0,"has_subcommand_status":false,"subcommand_status":0,"native_proto":[]};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvFeedback_ResponseSize(Ros2Uwvm_spot_bridgeSrvFeedback_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvFeedback_ResponseCapture(value),n=Ros2Uwvm_spot_bridgeSrvFeedback_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvFeedback_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16429,90);if(owned.length<45)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvFeedback_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvFeedback_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["error"],4,false);n=addSize(n,4);
if(typeof value["unknown"]!=="boolean")bad();n=addSize(n,1);
integer(value["native_command_id"],4,false);n=addSize(n,4);
if(typeof value["has_full_body_feedback"]!=="boolean")bad();n=addSize(n,1);
integer(value["full_body_status"],4,false);n=addSize(n,4);
if(typeof value["has_mobility_feedback"]!=="boolean")bad();n=addSize(n,1);
integer(value["mobility_status"],4,false);n=addSize(n,4);
integer(value["feedback_kind"],1,false);n=addSize(n,1);
if(typeof value["has_subcommand_status"]!=="boolean")bad();n=addSize(n,1);
integer(value["subcommand_status"],4,false);n=addSize(n,4);
if(!Array.isArray(value["native_proto"])||value["native_proto"].length>16384)bad();
n=addSize(n,4);
for(const element of value["native_proto"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>16429)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvFeedback_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["error"],true);w.at+=4;
w.v.setUint8(w.at,value["unknown"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["native_command_id"],true);w.at+=4;
w.v.setUint8(w.at,value["has_full_body_feedback"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["full_body_status"],true);w.at+=4;
w.v.setUint8(w.at,value["has_mobility_feedback"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["mobility_status"],true);w.at+=4;
w.v.setUint8(w.at,value["feedback_kind"],true);w.at+=1;
w.v.setUint8(w.at,value["has_subcommand_status"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["subcommand_status"],true);w.at+=4;
w.v.setUint32(w.at,value["native_proto"].length,true);w.at+=4;
for(const element of value["native_proto"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Uwvm_spot_bridgeSrvFeedback_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["unknown"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["native_command_id"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["has_full_body_feedback"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["full_body_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["has_mobility_feedback"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["mobility_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["feedback_kind"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["has_subcommand_status"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["subcommand_status"]=decoded;
}
{
{const count=r.u32();if(count>16384)bad();value["native_proto"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["native_proto"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_spot_bridgeSrvFeedback_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["error"];
out["error"]=field;
}
{const field=value["unknown"];
out["unknown"]=field;
}
{const field=value["native_command_id"];
out["native_command_id"]=field;
}
{const field=value["has_full_body_feedback"];
out["has_full_body_feedback"]=field;
}
{const field=value["full_body_status"];
out["full_body_status"]=field;
}
{const field=value["has_mobility_feedback"];
out["has_mobility_feedback"]=field;
}
{const field=value["mobility_status"];
out["mobility_status"]=field;
}
{const field=value["feedback_kind"];
out["feedback_kind"]=field;
}
{const field=value["has_subcommand_status"];
out["has_subcommand_status"]=field;
}
{const field=value["subcommand_status"];
out["subcommand_status"]=field;
}
{const field=value["native_proto"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16384)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["native_proto"]=values;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Lease_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvLease_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a9aacd78dc6ae9d18fb6bb0d185797c6463c882edadf251eb5c21e3aa3c538a1",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"budget_ns":0n};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvLease_RequestSize(Ros2Uwvm_spot_bridgeSrvLease_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvLease_RequestCapture(value),n=Ros2Uwvm_spot_bridgeSrvLease_RequestSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvLease_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvLease_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvLease_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["budget_ns"],8,false);n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvLease_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setBigUint64(w.at,value["budget_ns"],true);w.at+=8;
}
function Ros2Uwvm_spot_bridgeSrvLease_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["budget_ns"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvLease_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["budget_ns"];
out["budget_ns"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Lease_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvLease_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ce875eb701b607c01f8a19dc8fcff607829824ad99276f906c7fc8fc5567f6d3",maxWireBytes:22,minWireBytes:22,cartesianMaxWireBytes:22n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"error":0,"unknown":false,"provider_holds_body_lease":false};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvLease_ResponseSize(Ros2Uwvm_spot_bridgeSrvLease_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvLease_ResponseCapture(value),n=Ros2Uwvm_spot_bridgeSrvLease_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvLease_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,22,90);if(owned.length<22)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvLease_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvLease_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["error"],4,false);n=addSize(n,4);
if(typeof value["unknown"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["provider_holds_body_lease"]!=="boolean")bad();n=addSize(n,1);
if(n>22)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvLease_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["error"],true);w.at+=4;
w.v.setUint8(w.at,value["unknown"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["provider_holds_body_lease"]?1:0,true);w.at+=1;
}
function Ros2Uwvm_spot_bridgeSrvLease_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["unknown"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["provider_holds_body_lease"]=decoded!==0;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvLease_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["error"];
out["error"]=field;
}
{const field=value["unknown"];
out["unknown"]=field;
}
{const field=value["provider_holds_body_lease"];
out["provider_holds_body_lease"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/State_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9becf278bc168ed6d5b15fe7f1ec35b83e274de8a5173cb2335948306bc7a47e",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"budget_ns":0n};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvState_RequestSize(Ros2Uwvm_spot_bridgeSrvState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvState_RequestCapture(value),n=Ros2Uwvm_spot_bridgeSrvState_RequestSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["budget_ns"],8,false);n=addSize(n,8);
if(n>24)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvState_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setBigUint64(w.at,value["budget_ns"],true);w.at+=8;
}
function Ros2Uwvm_spot_bridgeSrvState_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["budget_ns"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["budget_ns"];
out["budget_ns"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/State_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8ffde1e9e2470975f03e038dbd51cf288c506671b4f3fe64b59223cb9711fb6c",maxWireBytes:21433,minWireBytes:57,cartesianMaxWireBytes:21433n,aggregateWireBytes:32768,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"error":0,"unknown":false,"acquisition_sec":0n,"acquisition_nsec":0,"joint_names":[],"joint_position":[],"joint_velocity":[],"joint_load":[],"motor_power_state":0,"native_proto":[]};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvState_ResponseSize(Ros2Uwvm_spot_bridgeSrvState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvState_ResponseCapture(value),n=Ros2Uwvm_spot_bridgeSrvState_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,21433,90);if(owned.length<57)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["error"],4,false);n=addSize(n,4);
if(typeof value["unknown"]!=="boolean")bad();n=addSize(n,1);
integer(value["acquisition_sec"],8,true);n=addSize(n,8);
integer(value["acquisition_nsec"],4,false);n=addSize(n,4);
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>32)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,128).length);
}
if(!Array.isArray(value["joint_position"])||value["joint_position"].length>32)bad();
n=addSize(n,4);
for(const element of value["joint_position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["joint_velocity"])||value["joint_velocity"].length>32)bad();
n=addSize(n,4);
for(const element of value["joint_velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["joint_load"])||value["joint_load"].length>32)bad();
n=addSize(n,4);
for(const element of value["joint_load"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
integer(value["motor_power_state"],4,false);n=addSize(n,4);
if(!Array.isArray(value["native_proto"])||value["native_proto"].length>16384)bad();
n=addSize(n,4);
for(const element of value["native_proto"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>21433)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvState_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["error"],true);w.at+=4;
w.v.setUint8(w.at,value["unknown"]?1:0,true);w.at+=1;
w.v.setBigInt64(w.at,value["acquisition_sec"],true);w.at+=8;
w.v.setUint32(w.at,value["acquisition_nsec"],true);w.at+=4;
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,128);
}
w.v.setUint32(w.at,value["joint_position"].length,true);w.at+=4;
for(const element of value["joint_position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["joint_velocity"].length,true);w.at+=4;
for(const element of value["joint_velocity"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["joint_load"].length,true);w.at+=4;
for(const element of value["joint_load"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["motor_power_state"],true);w.at+=4;
w.v.setUint32(w.at,value["native_proto"].length,true);w.at+=4;
for(const element of value["native_proto"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Uwvm_spot_bridgeSrvState_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["unknown"]=decoded!==0;
}
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["acquisition_sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["acquisition_nsec"]=decoded;
}
{
{const count=r.u32();if(count>32)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(128);
value["joint_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["joint_position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_position"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["joint_velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_velocity"].push(decoded);
}}
}
{
{const count=r.u32();if(count>32)bad();value["joint_load"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["joint_load"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["motor_power_state"]=decoded;
}
{
{const count=r.u32();if(count>16384)bad();value["native_proto"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["native_proto"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_spot_bridgeSrvState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["error"];
out["error"]=field;
}
{const field=value["unknown"];
out["unknown"]=field;
}
{const field=value["acquisition_sec"];
out["acquisition_sec"]=field;
}
{const field=value["acquisition_nsec"];
out["acquisition_nsec"]=field;
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["joint_position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_position"]=values;
}
{const field=value["joint_velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_velocity"]=values;
}
{const field=value["joint_load"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_load"]=values;
}
{const field=value["motor_power_state"];
out["motor_power_state"]=field;
}
{const field=value["native_proto"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16384)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["native_proto"]=values;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Velocity_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvVelocity_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ffe3b1dec91a25cd84d221ba4ebeae576bd7d6b96f258579c0b3d280cad79f8d",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"budget_ns":0n,"x_mps":0,"y_mps":0,"yaw_rps":0,"duration_ns":0n};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvVelocity_RequestSize(Ros2Uwvm_spot_bridgeSrvVelocity_RequestCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvVelocity_RequestCapture(value),n=Ros2Uwvm_spot_bridgeSrvVelocity_RequestSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvVelocity_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvVelocity_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvVelocity_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["budget_ns"],8,false);n=addSize(n,8);
if(typeof value["x_mps"]!=="number"||!Number.isFinite(value["x_mps"]))bad();n=addSize(n,8);
if(typeof value["y_mps"]!=="number"||!Number.isFinite(value["y_mps"]))bad();n=addSize(n,8);
if(typeof value["yaw_rps"]!=="number"||!Number.isFinite(value["yaw_rps"]))bad();n=addSize(n,8);
integer(value["duration_ns"],8,false);n=addSize(n,8);
if(n>56)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvVelocity_RequestWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setBigUint64(w.at,value["budget_ns"],true);w.at+=8;
w.v.setFloat64(w.at,value["x_mps"],true);w.at+=8;
w.v.setFloat64(w.at,value["y_mps"],true);w.at+=8;
w.v.setFloat64(w.at,value["yaw_rps"],true);w.at+=8;
w.v.setBigUint64(w.at,value["duration_ns"],true);w.at+=8;
}
function Ros2Uwvm_spot_bridgeSrvVelocity_RequestRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["budget_ns"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["x_mps"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["y_mps"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["yaw_rps"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["duration_ns"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvVelocity_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["budget_ns"];
out["budget_ns"]=field;
}
{const field=value["x_mps"];
out["x_mps"]=field;
}
{const field=value["y_mps"];
out["y_mps"]=field;
}
{const field=value["yaw_rps"];
out["yaw_rps"]=field;
}
{const field=value["duration_ns"];
out["duration_ns"]=field;
}
return out;}
/** Complete ROS2 uwvm_spot_bridge/srv/Velocity_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_spot_bridgeSrvVelocity_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_031f37beeb5a03c4e65d4be8fd70bc680408d110a020e066bea593d86498401a",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"token":0n,"nonce":0n,"error":0,"unknown":false,"native_accepted":false,"native_command_id":0};},
validateValue(value){Ros2Uwvm_spot_bridgeSrvVelocity_ResponseSize(Ros2Uwvm_spot_bridgeSrvVelocity_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_spot_bridgeSrvVelocity_ResponseCapture(value),n=Ros2Uwvm_spot_bridgeSrvVelocity_ResponseSize(owned),w=new Writer(n);Ros2Uwvm_spot_bridgeSrvVelocity_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Uwvm_spot_bridgeSrvVelocity_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_spot_bridgeSrvVelocity_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["token"],8,false);n=addSize(n,8);
integer(value["nonce"],8,false);n=addSize(n,8);
integer(value["error"],4,false);n=addSize(n,4);
if(typeof value["unknown"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["native_accepted"]!=="boolean")bad();n=addSize(n,1);
integer(value["native_command_id"],4,false);n=addSize(n,4);
if(n>26)bad(90);return n;}
function Ros2Uwvm_spot_bridgeSrvVelocity_ResponseWrite(value,w){
w.v.setBigUint64(w.at,value["token"],true);w.at+=8;
w.v.setBigUint64(w.at,value["nonce"],true);w.at+=8;
w.v.setUint32(w.at,value["error"],true);w.at+=4;
w.v.setUint8(w.at,value["unknown"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["native_accepted"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["native_command_id"],true);w.at+=4;
}
function Ros2Uwvm_spot_bridgeSrvVelocity_ResponseRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["token"]=decoded;
}
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["nonce"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["unknown"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["native_accepted"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["native_command_id"]=decoded;
}
return value;}
function Ros2Uwvm_spot_bridgeSrvVelocity_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["token"];
out["token"]=field;
}
{const field=value["nonce"];
out["nonce"]=field;
}
{const field=value["error"];
out["error"]=field;
}
{const field=value["unknown"];
out["unknown"]=field;
}
{const field=value["native_accepted"];
out["native_accepted"]=field;
}
{const field=value["native_command_id"];
out["native_command_id"]=field;
}
return out;}
