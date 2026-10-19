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
/** Complete ROS2 abb_egm_msgs/msg/EGMChannelState codec; maxima are not actual lengths. */
export const Ros2Abb_egm_msgsMsgEGMChannelStateCodec=Object.freeze({codecIdentity:"UWVMROS1_fbd288b93963fccc92aa114570c0a4f6a1fdc5b473cb8e252922720f5d88b519",maxWireBytes:81,minWireBytes:17,cartesianMaxWireBytes:81n,aggregateWireBytes:0,constants:Object.freeze({"EGM_UNDEFINED":1,"EGM_ERROR":2,"EGM_STOPPED":3,"EGM_RUNNING":4,"MOTORS_UNDEFINED":1,"MOTORS_ON":2,"MOTORS_OFF":3,"RAPID_UNDEFINED":1,"RAPID_STOPPED":2,"RAPID_RUNNING":3}),
defaultValue(){return {"name":"","active":false,"egm_convergence_met":false,"egm_client_state":0,"motor_state":0,"rapid_execution_state":0,"utilization_rate":0};},
validateValue(value){Ros2Abb_egm_msgsMsgEGMChannelStateSize(Ros2Abb_egm_msgsMsgEGMChannelStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_egm_msgsMsgEGMChannelStateCapture(value),n=Ros2Abb_egm_msgsMsgEGMChannelStateSize(owned),w=new Writer(n);Ros2Abb_egm_msgsMsgEGMChannelStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,81,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Abb_egm_msgsMsgEGMChannelStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_egm_msgsMsgEGMChannelStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["active"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["egm_convergence_met"]!=="boolean")bad();n=addSize(n,1);
integer(value["egm_client_state"],1,false);n=addSize(n,1);
integer(value["motor_state"],1,false);n=addSize(n,1);
integer(value["rapid_execution_state"],1,false);n=addSize(n,1);
if(typeof value["utilization_rate"]!=="number"||!Number.isFinite(value["utilization_rate"]))bad();n=addSize(n,8);
if(n>81)bad(90);return n;}
function Ros2Abb_egm_msgsMsgEGMChannelStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["active"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["egm_convergence_met"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["egm_client_state"],true);w.at+=1;
w.v.setUint8(w.at,value["motor_state"],true);w.at+=1;
w.v.setUint8(w.at,value["rapid_execution_state"],true);w.at+=1;
w.v.setFloat64(w.at,value["utilization_rate"],true);w.at+=8;
}
function Ros2Abb_egm_msgsMsgEGMChannelStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["active"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["egm_convergence_met"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["egm_client_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["motor_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["rapid_execution_state"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["utilization_rate"]=decoded;
}
return value;}
function Ros2Abb_egm_msgsMsgEGMChannelStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["active"];
out["active"]=field;
}
{const field=value["egm_convergence_met"];
out["egm_convergence_met"]=field;
}
{const field=value["egm_client_state"];
out["egm_client_state"]=field;
}
{const field=value["motor_state"];
out["motor_state"]=field;
}
{const field=value["rapid_execution_state"];
out["rapid_execution_state"]=field;
}
{const field=value["utilization_rate"];
out["utilization_rate"]=field;
}
return out;}
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
/** Complete ROS2 std_msgs/msg/Header codec; maxima are not actual lengths. */
export const Ros2Std_msgsMsgHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396",maxWireBytes:76,minWireBytes:12,cartesianMaxWireBytes:76n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":Ros2Builtin_interfacesMsgTimeCodec.defaultValue(),"frame_id":""};},
validateValue(value){Ros2Std_msgsMsgHeaderSize(Ros2Std_msgsMsgHeaderCapture(value));return true;},
encode(value){const owned=Ros2Std_msgsMsgHeaderCapture(value),n=Ros2Std_msgsMsgHeaderSize(owned),w=new Writer(n);Ros2Std_msgsMsgHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,76,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Std_msgsMsgHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_msgsMsgHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Builtin_interfacesMsgTimeSize(value["stamp"]));
n=addSize(n,4+textBytes(value["frame_id"],64).length);
if(n>76)bad(90);return n;}
function Ros2Std_msgsMsgHeaderWrite(value,w){
Ros2Builtin_interfacesMsgTimeWrite(value["stamp"],w);
w.text(value["frame_id"],64);
}
function Ros2Std_msgsMsgHeaderRead(r){const value={};
{
const decoded=Ros2Builtin_interfacesMsgTimeRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.text(64);
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
/** Complete ROS2 abb_egm_msgs/msg/EGMState codec; maxima are not actual lengths. */
export const Ros2Abb_egm_msgsMsgEGMStateCodec=Object.freeze({codecIdentity:"UWVMROS1_5b10efb13c3be7290c7e391d55679a174969887edd1b8baca682edac311a8e5d",maxWireBytes:1376,minWireBytes:16,cartesianMaxWireBytes:1376n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"egm_channels":[]};},
validateValue(value){Ros2Abb_egm_msgsMsgEGMStateSize(Ros2Abb_egm_msgsMsgEGMStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_egm_msgsMsgEGMStateCapture(value),n=Ros2Abb_egm_msgsMsgEGMStateSize(owned),w=new Writer(n);Ros2Abb_egm_msgsMsgEGMStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1376,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_egm_msgsMsgEGMStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_egm_msgsMsgEGMStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["egm_channels"])||value["egm_channels"].length>16)bad();
n=addSize(n,4);
for(const element of value["egm_channels"]){
n=addSize(n,Ros2Abb_egm_msgsMsgEGMChannelStateSize(element));
}
if(n>1376)bad(90);return n;}
function Ros2Abb_egm_msgsMsgEGMStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["egm_channels"].length,true);w.at+=4;
for(const element of value["egm_channels"]){
Ros2Abb_egm_msgsMsgEGMChannelStateWrite(element,w);
}
}
function Ros2Abb_egm_msgsMsgEGMStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["egm_channels"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Abb_egm_msgsMsgEGMChannelStateRead(r);
value["egm_channels"].push(decoded);
}}
}
return value;}
function Ros2Abb_egm_msgsMsgEGMStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["egm_channels"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Abb_egm_msgsMsgEGMChannelStateCapture(element);
}out["egm_channels"]=values;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/ConfData codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgConfDataCodec=Object.freeze({codecIdentity:"UWVMROS1_0c22cc29db62f5001ef20ea7b131de63099e31ac5f150d3e9f0bb62120d60571",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cf1":0,"cf4":0,"cf6":0,"cfx":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgConfDataSize(Ros2Abb_rapid_msgsMsgConfDataCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgConfDataCapture(value),n=Ros2Abb_rapid_msgsMsgConfDataSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgConfDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgConfDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgConfDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["cf1"]!=="number"||!Number.isFinite(value["cf1"])||!Number.isFinite(Math.fround(value["cf1"])))bad();n=addSize(n,4);
if(typeof value["cf4"]!=="number"||!Number.isFinite(value["cf4"])||!Number.isFinite(Math.fround(value["cf4"])))bad();n=addSize(n,4);
if(typeof value["cf6"]!=="number"||!Number.isFinite(value["cf6"])||!Number.isFinite(Math.fround(value["cf6"])))bad();n=addSize(n,4);
if(typeof value["cfx"]!=="number"||!Number.isFinite(value["cfx"])||!Number.isFinite(Math.fround(value["cfx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgConfDataWrite(value,w){
w.v.setFloat32(w.at,value["cf1"],true);w.at+=4;
w.v.setFloat32(w.at,value["cf4"],true);w.at+=4;
w.v.setFloat32(w.at,value["cf6"],true);w.at+=4;
w.v.setFloat32(w.at,value["cfx"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgConfDataRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf6"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cfx"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgConfDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cf1"];
out["cf1"]=field;
}
{const field=value["cf4"];
out["cf4"]=field;
}
{const field=value["cf6"];
out["cf6"]=field;
}
{const field=value["cfx"];
out["cfx"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/ExtJoint codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgExtJointCodec=Object.freeze({codecIdentity:"UWVMROS1_8b799c6bec9e4eab0900a633a93375a7792e49fde0bf302f6752115a001a4ae2",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"eax_a":0,"eax_b":0,"eax_c":0,"eax_d":0,"eax_e":0,"eax_f":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgExtJointSize(Ros2Abb_rapid_msgsMsgExtJointCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgExtJointCapture(value),n=Ros2Abb_rapid_msgsMsgExtJointSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgExtJointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgExtJointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgExtJointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["eax_a"]!=="number"||!Number.isFinite(value["eax_a"])||!Number.isFinite(Math.fround(value["eax_a"])))bad();n=addSize(n,4);
if(typeof value["eax_b"]!=="number"||!Number.isFinite(value["eax_b"])||!Number.isFinite(Math.fround(value["eax_b"])))bad();n=addSize(n,4);
if(typeof value["eax_c"]!=="number"||!Number.isFinite(value["eax_c"])||!Number.isFinite(Math.fround(value["eax_c"])))bad();n=addSize(n,4);
if(typeof value["eax_d"]!=="number"||!Number.isFinite(value["eax_d"])||!Number.isFinite(Math.fround(value["eax_d"])))bad();n=addSize(n,4);
if(typeof value["eax_e"]!=="number"||!Number.isFinite(value["eax_e"])||!Number.isFinite(Math.fround(value["eax_e"])))bad();n=addSize(n,4);
if(typeof value["eax_f"]!=="number"||!Number.isFinite(value["eax_f"])||!Number.isFinite(Math.fround(value["eax_f"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgExtJointWrite(value,w){
w.v.setFloat32(w.at,value["eax_a"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_b"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_c"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_d"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_e"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_f"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgExtJointRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_a"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_b"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_c"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_d"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_e"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_f"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgExtJointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["eax_a"];
out["eax_a"]=field;
}
{const field=value["eax_b"];
out["eax_b"]=field;
}
{const field=value["eax_c"];
out["eax_c"]=field;
}
{const field=value["eax_d"];
out["eax_d"]=field;
}
{const field=value["eax_e"];
out["eax_e"]=field;
}
{const field=value["eax_f"];
out["eax_f"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/RobJoint codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgRobJointCodec=Object.freeze({codecIdentity:"UWVMROS1_6d842aebc0a3b222a727063a41ef7e96f2b8c14ffbba5c8d6e140ba2890e69cd",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"rax_1":0,"rax_2":0,"rax_3":0,"rax_4":0,"rax_5":0,"rax_6":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgRobJointSize(Ros2Abb_rapid_msgsMsgRobJointCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgRobJointCapture(value),n=Ros2Abb_rapid_msgsMsgRobJointSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgRobJointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgRobJointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgRobJointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["rax_1"]!=="number"||!Number.isFinite(value["rax_1"])||!Number.isFinite(Math.fround(value["rax_1"])))bad();n=addSize(n,4);
if(typeof value["rax_2"]!=="number"||!Number.isFinite(value["rax_2"])||!Number.isFinite(Math.fround(value["rax_2"])))bad();n=addSize(n,4);
if(typeof value["rax_3"]!=="number"||!Number.isFinite(value["rax_3"])||!Number.isFinite(Math.fround(value["rax_3"])))bad();n=addSize(n,4);
if(typeof value["rax_4"]!=="number"||!Number.isFinite(value["rax_4"])||!Number.isFinite(Math.fround(value["rax_4"])))bad();n=addSize(n,4);
if(typeof value["rax_5"]!=="number"||!Number.isFinite(value["rax_5"])||!Number.isFinite(Math.fround(value["rax_5"])))bad();n=addSize(n,4);
if(typeof value["rax_6"]!=="number"||!Number.isFinite(value["rax_6"])||!Number.isFinite(Math.fround(value["rax_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgRobJointWrite(value,w){
w.v.setFloat32(w.at,value["rax_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_6"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgRobJointRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_6"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgRobJointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rax_1"];
out["rax_1"]=field;
}
{const field=value["rax_2"];
out["rax_2"]=field;
}
{const field=value["rax_3"];
out["rax_3"]=field;
}
{const field=value["rax_4"];
out["rax_4"]=field;
}
{const field=value["rax_5"];
out["rax_5"]=field;
}
{const field=value["rax_6"];
out["rax_6"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/JointTarget codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgJointTargetCodec=Object.freeze({codecIdentity:"UWVMROS1_5949cf0121391646c1f04b54d8f7deaae0497f4920f7817739fc25bfddc4e540",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robax":Ros2Abb_rapid_msgsMsgRobJointCodec.defaultValue(),"extax":Ros2Abb_rapid_msgsMsgExtJointCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_msgsMsgJointTargetSize(Ros2Abb_rapid_msgsMsgJointTargetCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgJointTargetCapture(value),n=Ros2Abb_rapid_msgsMsgJointTargetSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgJointTargetWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgJointTargetRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgJointTargetSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_rapid_msgsMsgRobJointSize(value["robax"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgExtJointSize(value["extax"]));
if(n>48)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgJointTargetWrite(value,w){
Ros2Abb_rapid_msgsMsgRobJointWrite(value["robax"],w);
Ros2Abb_rapid_msgsMsgExtJointWrite(value["extax"],w);
}
function Ros2Abb_rapid_msgsMsgJointTargetRead(r){const value={};
{
const decoded=Ros2Abb_rapid_msgsMsgRobJointRead(r);
value["robax"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgExtJointRead(r);
value["extax"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgJointTargetCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robax"];
out["robax"]=Ros2Abb_rapid_msgsMsgRobJointCapture(field);
}
{const field=value["extax"];
out["extax"]=Ros2Abb_rapid_msgsMsgExtJointCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/Pos codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgPosCodec=Object.freeze({codecIdentity:"UWVMROS1_31d490d6ead5a1a78295dc4e934f0b89e74e4732da2b57d28d297477bf4cbdd2",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgPosSize(Ros2Abb_rapid_msgsMsgPosCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgPosCapture(value),n=Ros2Abb_rapid_msgsMsgPosSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgPosWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgPosRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgPosSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgPosWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgPosRead(r){const value={};
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
value["z"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgPosCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 abb_rapid_msgs/msg/Orient codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgOrientCodec=Object.freeze({codecIdentity:"UWVMROS1_821d887b88298422f9466aaf3926db580f878e964055fbd72226b7f76f2f536e",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"q1":0,"q2":0,"q3":0,"q4":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgOrientSize(Ros2Abb_rapid_msgsMsgOrientCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgOrientCapture(value),n=Ros2Abb_rapid_msgsMsgOrientSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgOrientWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgOrientRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgOrientSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["q1"]!=="number"||!Number.isFinite(value["q1"])||!Number.isFinite(Math.fround(value["q1"])))bad();n=addSize(n,4);
if(typeof value["q2"]!=="number"||!Number.isFinite(value["q2"])||!Number.isFinite(Math.fround(value["q2"])))bad();n=addSize(n,4);
if(typeof value["q3"]!=="number"||!Number.isFinite(value["q3"])||!Number.isFinite(Math.fround(value["q3"])))bad();n=addSize(n,4);
if(typeof value["q4"]!=="number"||!Number.isFinite(value["q4"])||!Number.isFinite(Math.fround(value["q4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgOrientWrite(value,w){
w.v.setFloat32(w.at,value["q1"],true);w.at+=4;
w.v.setFloat32(w.at,value["q2"],true);w.at+=4;
w.v.setFloat32(w.at,value["q3"],true);w.at+=4;
w.v.setFloat32(w.at,value["q4"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgOrientRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q4"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgOrientCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["q1"];
out["q1"]=field;
}
{const field=value["q2"];
out["q2"]=field;
}
{const field=value["q3"];
out["q3"]=field;
}
{const field=value["q4"];
out["q4"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/LoadData codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgLoadDataCodec=Object.freeze({codecIdentity:"UWVMROS1_f73e480a0e6746a4df8f4d62bc123d68d01d3dc48f0bc5f0eac51b859a0750f9",maxWireBytes:44,minWireBytes:44,cartesianMaxWireBytes:44n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"cog":Ros2Abb_rapid_msgsMsgPosCodec.defaultValue(),"aom":Ros2Abb_rapid_msgsMsgOrientCodec.defaultValue(),"ix":0,"iy":0,"iz":0};},
validateValue(value){Ros2Abb_rapid_msgsMsgLoadDataSize(Ros2Abb_rapid_msgsMsgLoadDataCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgLoadDataCapture(value),n=Ros2Abb_rapid_msgsMsgLoadDataSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgLoadDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,44,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgLoadDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgLoadDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
n=addSize(n,Ros2Abb_rapid_msgsMsgPosSize(value["cog"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgOrientSize(value["aom"]));
if(typeof value["ix"]!=="number"||!Number.isFinite(value["ix"])||!Number.isFinite(Math.fround(value["ix"])))bad();n=addSize(n,4);
if(typeof value["iy"]!=="number"||!Number.isFinite(value["iy"])||!Number.isFinite(Math.fround(value["iy"])))bad();n=addSize(n,4);
if(typeof value["iz"]!=="number"||!Number.isFinite(value["iz"])||!Number.isFinite(Math.fround(value["iz"])))bad();n=addSize(n,4);
if(n>44)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgLoadDataWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
Ros2Abb_rapid_msgsMsgPosWrite(value["cog"],w);
Ros2Abb_rapid_msgsMsgOrientWrite(value["aom"],w);
w.v.setFloat32(w.at,value["ix"],true);w.at+=4;
w.v.setFloat32(w.at,value["iy"],true);w.at+=4;
w.v.setFloat32(w.at,value["iz"],true);w.at+=4;
}
function Ros2Abb_rapid_msgsMsgLoadDataRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPosRead(r);
value["cog"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrientRead(r);
value["aom"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ix"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["iy"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["iz"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgLoadDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cog"];
out["cog"]=Ros2Abb_rapid_msgsMsgPosCapture(field);
}
{const field=value["aom"];
out["aom"]=Ros2Abb_rapid_msgsMsgOrientCapture(field);
}
{const field=value["ix"];
out["ix"]=field;
}
{const field=value["iy"];
out["iy"]=field;
}
{const field=value["iz"];
out["iz"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/Pose codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgPoseCodec=Object.freeze({codecIdentity:"UWVMROS1_8f1535fe21326706fd0b16314857162d36bca91ce8e8f6a8bec3864dafb9e2f2",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trans":Ros2Abb_rapid_msgsMsgPosCodec.defaultValue(),"rot":Ros2Abb_rapid_msgsMsgOrientCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_msgsMsgPoseSize(Ros2Abb_rapid_msgsMsgPoseCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgPoseCapture(value),n=Ros2Abb_rapid_msgsMsgPoseSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgPoseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgPoseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgPoseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_rapid_msgsMsgPosSize(value["trans"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgOrientSize(value["rot"]));
if(n>28)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgPoseWrite(value,w){
Ros2Abb_rapid_msgsMsgPosWrite(value["trans"],w);
Ros2Abb_rapid_msgsMsgOrientWrite(value["rot"],w);
}
function Ros2Abb_rapid_msgsMsgPoseRead(r){const value={};
{
const decoded=Ros2Abb_rapid_msgsMsgPosRead(r);
value["trans"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrientRead(r);
value["rot"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgPoseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trans"];
out["trans"]=Ros2Abb_rapid_msgsMsgPosCapture(field);
}
{const field=value["rot"];
out["rot"]=Ros2Abb_rapid_msgsMsgOrientCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/RobTarget codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgRobTargetCodec=Object.freeze({codecIdentity:"UWVMROS1_8decb56e8faf0a6a954af974adfea5c500aea8392e6a778ae7394174323489d8",maxWireBytes:68,minWireBytes:68,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trans":Ros2Abb_rapid_msgsMsgPosCodec.defaultValue(),"rot":Ros2Abb_rapid_msgsMsgOrientCodec.defaultValue(),"robconf":Ros2Abb_rapid_msgsMsgConfDataCodec.defaultValue(),"extax":Ros2Abb_rapid_msgsMsgExtJointCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_msgsMsgRobTargetSize(Ros2Abb_rapid_msgsMsgRobTargetCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgRobTargetCapture(value),n=Ros2Abb_rapid_msgsMsgRobTargetSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgRobTargetWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<68)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgRobTargetRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgRobTargetSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_rapid_msgsMsgPosSize(value["trans"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgOrientSize(value["rot"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgConfDataSize(value["robconf"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgExtJointSize(value["extax"]));
if(n>68)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgRobTargetWrite(value,w){
Ros2Abb_rapid_msgsMsgPosWrite(value["trans"],w);
Ros2Abb_rapid_msgsMsgOrientWrite(value["rot"],w);
Ros2Abb_rapid_msgsMsgConfDataWrite(value["robconf"],w);
Ros2Abb_rapid_msgsMsgExtJointWrite(value["extax"],w);
}
function Ros2Abb_rapid_msgsMsgRobTargetRead(r){const value={};
{
const decoded=Ros2Abb_rapid_msgsMsgPosRead(r);
value["trans"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrientRead(r);
value["rot"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgConfDataRead(r);
value["robconf"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgExtJointRead(r);
value["extax"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgRobTargetCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trans"];
out["trans"]=Ros2Abb_rapid_msgsMsgPosCapture(field);
}
{const field=value["rot"];
out["rot"]=Ros2Abb_rapid_msgsMsgOrientCapture(field);
}
{const field=value["robconf"];
out["robconf"]=Ros2Abb_rapid_msgsMsgConfDataCapture(field);
}
{const field=value["extax"];
out["extax"]=Ros2Abb_rapid_msgsMsgExtJointCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/ToolData codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgToolDataCodec=Object.freeze({codecIdentity:"UWVMROS1_948fa0bbaaf95f0d88ca03cad687f16102261f54b0da889ae8bca82042fc5237",maxWireBytes:73,minWireBytes:73,cartesianMaxWireBytes:73n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robhold":false,"tframe":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue(),"tload":Ros2Abb_rapid_msgsMsgLoadDataCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_msgsMsgToolDataSize(Ros2Abb_rapid_msgsMsgToolDataCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgToolDataCapture(value),n=Ros2Abb_rapid_msgsMsgToolDataSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgToolDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,73,90);if(owned.length<73)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgToolDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgToolDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["robhold"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["tframe"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgLoadDataSize(value["tload"]));
if(n>73)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgToolDataWrite(value,w){
w.v.setUint8(w.at,value["robhold"]?1:0,true);w.at+=1;
Ros2Abb_rapid_msgsMsgPoseWrite(value["tframe"],w);
Ros2Abb_rapid_msgsMsgLoadDataWrite(value["tload"],w);
}
function Ros2Abb_rapid_msgsMsgToolDataRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["robhold"]=decoded!==0;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["tframe"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgLoadDataRead(r);
value["tload"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgToolDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robhold"];
out["robhold"]=field;
}
{const field=value["tframe"];
out["tframe"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
{const field=value["tload"];
out["tload"]=Ros2Abb_rapid_msgsMsgLoadDataCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_msgs/msg/WObjData codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_msgsMsgWObjDataCodec=Object.freeze({codecIdentity:"UWVMROS1_b36f0448512fab6028431d19b39a8ae31b7b81b2615ffc8311139f63b5d8e7cd",maxWireBytes:574,minWireBytes:62,cartesianMaxWireBytes:574n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robhold":false,"ufprog":false,"ufmec":"","uframe":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue(),"oframe":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_msgsMsgWObjDataSize(Ros2Abb_rapid_msgsMsgWObjDataCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_msgsMsgWObjDataCapture(value),n=Ros2Abb_rapid_msgsMsgWObjDataSize(owned),w=new Writer(n);Ros2Abb_rapid_msgsMsgWObjDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,574,90);if(owned.length<62)bad();const r=new Reader(owned),value=Ros2Abb_rapid_msgsMsgWObjDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_msgsMsgWObjDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["robhold"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["ufprog"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["ufmec"],512).length);
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["uframe"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["oframe"]));
if(n>574)bad(90);return n;}
function Ros2Abb_rapid_msgsMsgWObjDataWrite(value,w){
w.v.setUint8(w.at,value["robhold"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["ufprog"]?1:0,true);w.at+=1;
w.text(value["ufmec"],512);
Ros2Abb_rapid_msgsMsgPoseWrite(value["uframe"],w);
Ros2Abb_rapid_msgsMsgPoseWrite(value["oframe"],w);
}
function Ros2Abb_rapid_msgsMsgWObjDataRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["robhold"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ufprog"]=decoded!==0;
}
{
const decoded=r.text(512);
value["ufmec"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["uframe"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["oframe"]=decoded;
}
return value;}
function Ros2Abb_rapid_msgsMsgWObjDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robhold"];
out["robhold"]=field;
}
{const field=value["ufprog"];
out["ufprog"]=field;
}
{const field=value["ufmec"];
out["ufmec"]=field;
}
{const field=value["uframe"];
out["uframe"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
{const field=value["oframe"];
out["oframe"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMActivateSettings codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_fce73aea80d9b8a299b8e2ab742602871146b3ebbd624a2e807e25c1ac107cfa",maxWireBytes:716,minWireBytes:204,cartesianMaxWireBytes:716n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tool":Ros2Abb_rapid_msgsMsgToolDataCodec.defaultValue(),"wobj":Ros2Abb_rapid_msgsMsgWObjDataCodec.defaultValue(),"correction_frame":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue(),"sensor_frame":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue(),"cond_min_max":0,"lp_filter":0,"sample_rate":0,"max_speed_deviation":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsSize(Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,716,90);if(owned.length<204)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_rapid_msgsMsgToolDataSize(value["tool"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgWObjDataSize(value["wobj"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["correction_frame"]));
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["sensor_frame"]));
if(typeof value["cond_min_max"]!=="number"||!Number.isFinite(value["cond_min_max"])||!Number.isFinite(Math.fround(value["cond_min_max"])))bad();n=addSize(n,4);
if(typeof value["lp_filter"]!=="number"||!Number.isFinite(value["lp_filter"])||!Number.isFinite(Math.fround(value["lp_filter"])))bad();n=addSize(n,4);
integer(value["sample_rate"],1,false);n=addSize(n,1);
if(typeof value["max_speed_deviation"]!=="number"||!Number.isFinite(value["max_speed_deviation"])||!Number.isFinite(Math.fround(value["max_speed_deviation"])))bad();n=addSize(n,4);
if(n>716)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsWrite(value,w){
Ros2Abb_rapid_msgsMsgToolDataWrite(value["tool"],w);
Ros2Abb_rapid_msgsMsgWObjDataWrite(value["wobj"],w);
Ros2Abb_rapid_msgsMsgPoseWrite(value["correction_frame"],w);
Ros2Abb_rapid_msgsMsgPoseWrite(value["sensor_frame"],w);
w.v.setFloat32(w.at,value["cond_min_max"],true);w.at+=4;
w.v.setFloat32(w.at,value["lp_filter"],true);w.at+=4;
w.v.setUint8(w.at,value["sample_rate"],true);w.at+=1;
w.v.setFloat32(w.at,value["max_speed_deviation"],true);w.at+=4;
}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsRead(r){const value={};
{
const decoded=Ros2Abb_rapid_msgsMsgToolDataRead(r);
value["tool"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgWObjDataRead(r);
value["wobj"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["correction_frame"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["sensor_frame"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cond_min_max"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["lp_filter"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["sample_rate"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["max_speed_deviation"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tool"];
out["tool"]=Ros2Abb_rapid_msgsMsgToolDataCapture(field);
}
{const field=value["wobj"];
out["wobj"]=Ros2Abb_rapid_msgsMsgWObjDataCapture(field);
}
{const field=value["correction_frame"];
out["correction_frame"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
{const field=value["sensor_frame"];
out["sensor_frame"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
{const field=value["cond_min_max"];
out["cond_min_max"]=field;
}
{const field=value["lp_filter"];
out["lp_filter"]=field;
}
{const field=value["sample_rate"];
out["sample_rate"]=field;
}
{const field=value["max_speed_deviation"];
out["max_speed_deviation"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMRunSettings codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_92a2a654eb2eda89077933f955f04ed87884827b8875cf563570eed5d211b0e2",maxWireBytes:40,minWireBytes:40,cartesianMaxWireBytes:40n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cond_time":0,"ramp_in_time":0,"offset":Ros2Abb_rapid_msgsMsgPoseCodec.defaultValue(),"pos_corr_gain":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsSize(Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,40,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["cond_time"]!=="number"||!Number.isFinite(value["cond_time"])||!Number.isFinite(Math.fround(value["cond_time"])))bad();n=addSize(n,4);
if(typeof value["ramp_in_time"]!=="number"||!Number.isFinite(value["ramp_in_time"])||!Number.isFinite(Math.fround(value["ramp_in_time"])))bad();n=addSize(n,4);
n=addSize(n,Ros2Abb_rapid_msgsMsgPoseSize(value["offset"]));
if(typeof value["pos_corr_gain"]!=="number"||!Number.isFinite(value["pos_corr_gain"])||!Number.isFinite(Math.fround(value["pos_corr_gain"])))bad();n=addSize(n,4);
if(n>40)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsWrite(value,w){
w.v.setFloat32(w.at,value["cond_time"],true);w.at+=4;
w.v.setFloat32(w.at,value["ramp_in_time"],true);w.at+=4;
Ros2Abb_rapid_msgsMsgPoseWrite(value["offset"],w);
w.v.setFloat32(w.at,value["pos_corr_gain"],true);w.at+=4;
}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cond_time"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ramp_in_time"]=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPoseRead(r);
value["offset"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pos_corr_gain"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cond_time"];
out["cond_time"]=field;
}
{const field=value["ramp_in_time"];
out["ramp_in_time"]=field;
}
{const field=value["offset"];
out["offset"]=Ros2Abb_rapid_msgsMsgPoseCapture(field);
}
{const field=value["pos_corr_gain"];
out["pos_corr_gain"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMSetupSettings codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_2565bed5b83b132c69f37d7294c69181800ad2974ddc54f60868da57e28e782c",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"use_filtering":false,"comm_timeout":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsSize(Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["use_filtering"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["comm_timeout"]!=="number"||!Number.isFinite(value["comm_timeout"])||!Number.isFinite(Math.fround(value["comm_timeout"])))bad();n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsWrite(value,w){
w.v.setUint8(w.at,value["use_filtering"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["comm_timeout"],true);w.at+=4;
}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_filtering"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["comm_timeout"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["use_filtering"];
out["use_filtering"]=field;
}
{const field=value["comm_timeout"];
out["comm_timeout"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMStopSettings codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_0f3f2f6034567a7d49dd140ff9ff620e8567548139597b9b823e63a947673cee",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ramp_out_time":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsSize(Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ramp_out_time"]!=="number"||!Number.isFinite(value["ramp_out_time"])||!Number.isFinite(Math.fround(value["ramp_out_time"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsWrite(value,w){
w.v.setFloat32(w.at,value["ramp_out_time"],true);w.at+=4;
}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ramp_out_time"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ramp_out_time"];
out["ramp_out_time"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMSettings codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_622d3a975eecb11f7729aecfadc161cada271208d6fe7599f455750b90144ef3",maxWireBytes:767,minWireBytes:255,cartesianMaxWireBytes:767n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"allow_egm_motions":false,"use_presync":false,"setup_uc":Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCodec.defaultValue(),"activate":Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCodec.defaultValue(),"run":Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCodec.defaultValue(),"stop":Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsSize(Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,767,90);if(owned.length<255)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["allow_egm_motions"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["use_presync"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsSize(value["setup_uc"]));
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsSize(value["activate"]));
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsSize(value["run"]));
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsSize(value["stop"]));
if(n>767)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsWrite(value,w){
w.v.setUint8(w.at,value["allow_egm_motions"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["use_presync"]?1:0,true);w.at+=1;
Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsWrite(value["setup_uc"],w);
Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsWrite(value["activate"],w);
Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsWrite(value["run"],w);
Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsWrite(value["stop"],w);
}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["allow_egm_motions"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_presync"]=decoded!==0;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsRead(r);
value["setup_uc"]=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsRead(r);
value["activate"]=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsRead(r);
value["run"]=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsRead(r);
value["stop"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["allow_egm_motions"];
out["allow_egm_motions"]=field;
}
{const field=value["use_presync"];
out["use_presync"]=field;
}
{const field=value["setup_uc"];
out["setup_uc"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCapture(field);
}
{const field=value["activate"];
out["activate"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCapture(field);
}
{const field=value["run"];
out["run"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCapture(field);
}
{const field=value["stop"];
out["stop"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/StateMachineState codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ff2a09e4b6e8ef26cbdc3ecdffdceb33d6b2c82f0d51ed2c38ce9648c6f47711",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({"SM_STATE_UNKNOWN":1,"SM_STATE_IDLE":2,"SM_STATE_INITIALIZE":3,"SM_STATE_RUN_RAPID_ROUTINE":4,"SM_STATE_RUN_EGM_ROUTINE":5,"EGM_ACTION_UNKNOWN":1,"EGM_ACTION_NONE":2,"EGM_ACTION_RUN_JOINT":3,"EGM_ACTION_RUN_POSE":4,"EGM_ACTION_STOP":5,"EGM_ACTION_START_STREAM":6,"EGM_ACTION_STOP_STREAM":7}),
defaultValue(){return {"rapid_task":"","sm_state":0,"egm_action":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateSize(Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["rapid_task"],512).length);
integer(value["sm_state"],1,false);n=addSize(n,1);
integer(value["egm_action"],1,false);n=addSize(n,1);
if(n>518)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateWrite(value,w){
w.text(value["rapid_task"],512);
w.v.setUint8(w.at,value["sm_state"],true);w.at+=1;
w.v.setUint8(w.at,value["egm_action"],true);w.at+=1;
}
function Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateRead(r){const value={};
{
const decoded=r.text(512);
value["rapid_task"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["sm_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["egm_action"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rapid_task"];
out["rapid_task"]=field;
}
{const field=value["sm_state"];
out["sm_state"]=field;
}
{const field=value["egm_action"];
out["egm_action"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/RuntimeState codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateCodec=Object.freeze({codecIdentity:"UWVMROS1_3c0f157544fa82c4e206a4fb42f8ba7f67213d19533f79b77c66dec6fd0460d9",maxWireBytes:8368,minWireBytes:16,cartesianMaxWireBytes:8368n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"state_machines":[]};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateSize(Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateCapture(value),n=Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8368,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["state_machines"])||value["state_machines"].length>16)bad();
n=addSize(n,4);
for(const element of value["state_machines"]){
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateSize(element));
}
if(n>8368)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["state_machines"].length,true);w.at+=4;
for(const element of value["state_machines"]){
Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateWrite(element,w);
}
}
function Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["state_machines"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateRead(r);
value["state_machines"].push(decoded);
}}
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["state_machines"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCapture(element);
}out["state_machines"]=values;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_363be2459fcc9c475c6305cbcfebf7689e891eb27645b7667960aace23d56e8e",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestSize(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
if(n>68)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestWrite(value,w){
w.text(value["task"],64);
}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d8f340e1c6d78fc0a782c3279efb34ae5e11df175a82f9dd5ee82e603d6b2ffb",maxWireBytes:1285,minWireBytes:261,cartesianMaxWireBytes:1285n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"settings":Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCodec.defaultValue(),"result_code":0,"message":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseSize(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1285,90);if(owned.length<261)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsSize(value["settings"]));
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1285)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseWrite(value,w){
Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsWrite(value["settings"],w);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseRead(r){const value={};
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsRead(r);
value["settings"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["settings"];
out["settings"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCapture(field);
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c2f3dd535e9475526bf18bdffa0e9a839049d315c9aebb150f8561ee3361a2bd",maxWireBytes:835,minWireBytes:259,cartesianMaxWireBytes:835n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","settings":Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCodec.defaultValue()};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestSize(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,835,90);if(owned.length<259)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsSize(value["settings"]));
if(n>835)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestWrite(value,w){
w.text(value["task"],64);
Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsWrite(value["settings"],w);
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsRead(r);
value["settings"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["settings"];
out["settings"]=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCapture(field);
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6a1adeebe31dfe101a6d57019e26ee2f1144b9a6ce30aee08d4c6fe56ea6838d",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseSize(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4561b7e5e296a1a329d5f5684bcbde1ef41324554a34e9a79a85824286dd477c",maxWireBytes:584,minWireBytes:8,cartesianMaxWireBytes:584n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","routine":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestSize(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,584,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,4+textBytes(value["routine"],512).length);
if(n>584)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestWrite(value,w){
w.text(value["task"],64);
w.text(value["routine"],512);
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.text(512);
value["routine"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["routine"];
out["routine"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_980d2a16536289f446161acdf2fc57d459baa2290d74f1dd5aaf8129126cbba1",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseSize(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5d6983aa85c2c52f8ec06cbd723d5929cfc381db2e0bfffb4c27078497d86c90",maxWireBytes:73,minWireBytes:9,cartesianMaxWireBytes:73n,aggregateWireBytes:0,constants:Object.freeze({"SG_COMMAND_UNKNOWN":1,"SG_COMMAND_NONE":2,"SG_COMMAND_INITIALIZE":3,"SG_COMMAND_CALIBRATE":4,"SG_COMMAND_MOVE_TO":5,"SG_COMMAND_GRIP_IN":6,"SG_COMMAND_GRIP_OUT":7,"SG_COMMAND_BLOW_ON_1":8,"SG_COMMAND_BLOW_ON_2":9,"SG_COMMAND_BLOW_OFF_1":10,"SG_COMMAND_BLOW_OFF_2":11,"SG_COMMAND_VACUUM_ON_1":12,"SG_COMMAND_VACUUM_ON_2":13,"SG_COMMAND_VACUUM_OFF_1":14,"SG_COMMAND_VACUUM_OFF_2":15}),
defaultValue(){return {"task":"","command":0,"target_position":0};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestSize(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,73,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
integer(value["command"],1,false);n=addSize(n,1);
if(typeof value["target_position"]!=="number"||!Number.isFinite(value["target_position"])||!Number.isFinite(Math.fround(value["target_position"])))bad();n=addSize(n,4);
if(n>73)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestWrite(value,w){
w.text(value["task"],64);
w.v.setUint8(w.at,value["command"],true);w.at+=1;
w.v.setFloat32(w.at,value["target_position"],true);w.at+=4;
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["command"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["target_position"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["command"];
out["command"]=field;
}
{const field=value["target_position"];
out["target_position"]=field;
}
return out;}
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response codec; maxima are not actual lengths. */
export const Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_87cdc41c81d49930dce7193d4b7af98add266a6e9a7cf5e69457d01ded388cf8",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseSize(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseCapture(value),n=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseSize(owned),w=new Writer(n);Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/msg/MechanicalUnitState codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsMsgMechanicalUnitStateCodec=Object.freeze({codecIdentity:"UWVMROS1_1dc7a7dcbaa19e78cd6419f8844287a7fbd9d7a839ab1a8ad97ef0be34f3a3a8",maxWireBytes:69,minWireBytes:5,cartesianMaxWireBytes:69n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","activated":false};},
validateValue(value){Ros2Abb_robot_msgsMsgMechanicalUnitStateSize(Ros2Abb_robot_msgsMsgMechanicalUnitStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsMsgMechanicalUnitStateCapture(value),n=Ros2Abb_robot_msgsMsgMechanicalUnitStateSize(owned),w=new Writer(n);Ros2Abb_robot_msgsMsgMechanicalUnitStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,69,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsMsgMechanicalUnitStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsMsgMechanicalUnitStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["activated"]!=="boolean")bad();n=addSize(n,1);
if(n>69)bad(90);return n;}
function Ros2Abb_robot_msgsMsgMechanicalUnitStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["activated"]?1:0,true);w.at+=1;
}
function Ros2Abb_robot_msgsMsgMechanicalUnitStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["activated"]=decoded!==0;
}
return value;}
function Ros2Abb_robot_msgsMsgMechanicalUnitStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["activated"];
out["activated"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/msg/RAPIDSymbolPath codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec=Object.freeze({codecIdentity:"UWVMROS1_249523197567fbf638f30487a7f5b8d8ff5ea746fcd4a65a35f2dbb18a3adf0e",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","module":"","symbol":""};},
validateValue(value){Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(value),n=Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(owned),w=new Writer(n);Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,4+textBytes(value["module"],64).length);
n=addSize(n,4+textBytes(value["symbol"],64).length);
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value,w){
w.text(value["task"],64);
w.text(value["module"],64);
w.text(value["symbol"],64);
}
function Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.text(64);
value["module"]=decoded;
}
{
const decoded=r.text(64);
value["symbol"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["module"];
out["module"]=field;
}
{const field=value["symbol"];
out["symbol"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/msg/RAPIDTaskState codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsMsgRAPIDTaskStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ff698e375b0ff33026cfd248c1ac734f939763aba25a6c4f7cac628d445422fd",maxWireBytes:71,minWireBytes:7,cartesianMaxWireBytes:71n,aggregateWireBytes:0,constants:Object.freeze({"EXECUTION_STATE_UNKNOWN":1,"EXECUTION_STATE_READY":2,"EXECUTION_STATE_STOPPED":3,"EXECUTION_STATE_STARTED":4,"EXECUTION_STATE_UNINITIALIZED":5}),
defaultValue(){return {"name":"","activated":false,"execution_state":0,"motion_task":false};},
validateValue(value){Ros2Abb_robot_msgsMsgRAPIDTaskStateSize(Ros2Abb_robot_msgsMsgRAPIDTaskStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsMsgRAPIDTaskStateCapture(value),n=Ros2Abb_robot_msgsMsgRAPIDTaskStateSize(owned),w=new Writer(n);Ros2Abb_robot_msgsMsgRAPIDTaskStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,71,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsMsgRAPIDTaskStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsMsgRAPIDTaskStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["activated"]!=="boolean")bad();n=addSize(n,1);
integer(value["execution_state"],1,false);n=addSize(n,1);
if(typeof value["motion_task"]!=="boolean")bad();n=addSize(n,1);
if(n>71)bad(90);return n;}
function Ros2Abb_robot_msgsMsgRAPIDTaskStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["activated"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["execution_state"],true);w.at+=1;
w.v.setUint8(w.at,value["motion_task"]?1:0,true);w.at+=1;
}
function Ros2Abb_robot_msgsMsgRAPIDTaskStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["activated"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["execution_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motion_task"]=decoded!==0;
}
return value;}
function Ros2Abb_robot_msgsMsgRAPIDTaskStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["activated"];
out["activated"]=field;
}
{const field=value["execution_state"];
out["execution_state"]=field;
}
{const field=value["motion_task"];
out["motion_task"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/msg/ServiceResponses codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsMsgServiceResponsesCodec=Object.freeze({codecIdentity:"UWVMROS1_4e01b972b67454e851801f28aee09c0774b67907d2241cc67cae7d045c18f646",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESS":"","RC_SUCCESS":1,"FAILED":"Service failed (e.g. timeout or resource not found, see the DEBUG log for details)","RC_FAILED":2,"SERVER_IS_BUSY":"Server is busy","RC_SERVER_IS_BUSY":1001,"EMPTY_FILENAME":"Filename is empty","RC_EMPTY_FILENAME":2001,"EMPTY_SIGNAL_NAME":"Signal name is empty","RC_EMPTY_SIGNAL_NAME":2002,"EMPTY_RAPID_TASK_NAME":"RAPID task name is empty","RC_EMPTY_RAPID_TASK_NAME":2003,"EMPTY_RAPID_MODULE_NAME":"RAPID module name is empty","RC_EMPTY_RAPID_MODULE_NAME":2004,"EMPTY_RAPID_SYMBOL_NAME":"RAPID symbol name is empty","RC_EMPTY_RAPID_SYMBOL_NAME":2005,"NOT_IN_AUTO_MODE":"Robot controller is not in AUTO mode","RC_NOT_IN_AUTO_MODE":3001,"MOTORS_ARE_OFF":"Motors are off","RC_MOTORS_ARE_OFF":3002,"MOTORS_ARE_ON":"Motors are on","RC_MOTORS_ARE_ON":3003,"RAPID_NOT_STOPPED":"RAPID has not been stopped","RC_RAPID_NOT_STOPPED":3004,"RAPID_NOT_RUNNING":"RAPID is not running","RC_RAPID_NOT_RUNNING":3005,"SM_RUNTIME_STATES_MISSING":"No runtime states received for any StateMachine Add-In instance","RC_SM_RUNTIME_STATES_MISSING":4001,"SM_UNKNOWN_RAPID_TASK":"RAPID task not found among known StateMachine Add-In instances","RC_SM_UNKNOWN_RAPID_TASK":4002,"SM_UNINITIALIZED":"StateMachine Add-In instance has not been initialized","RC_SM_UNINITIALIZED":4003}),
defaultValue(){return {};},
validateValue(value){Ros2Abb_robot_msgsMsgServiceResponsesSize(Ros2Abb_robot_msgsMsgServiceResponsesCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsMsgServiceResponsesCapture(value),n=Ros2Abb_robot_msgsMsgServiceResponsesSize(owned),w=new Writer(n);Ros2Abb_robot_msgsMsgServiceResponsesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsMsgServiceResponsesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsMsgServiceResponsesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Abb_robot_msgsMsgServiceResponsesWrite(value,w){
}
function Ros2Abb_robot_msgsMsgServiceResponsesRead(r){const value={};
return value;}
function Ros2Abb_robot_msgsMsgServiceResponsesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 abb_robot_msgs/msg/SystemState codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsMsgSystemStateCodec=Object.freeze({codecIdentity:"UWVMROS1_c1cdbf0e578fddc5bf70cc2cc181d786f3e8189afc6a67962de6eb273973a38f",maxWireBytes:2327,minWireBytes:23,cartesianMaxWireBytes:2327n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"motors_on":false,"auto_mode":false,"rapid_running":false,"rapid_tasks":[],"mechanical_units":[]};},
validateValue(value){Ros2Abb_robot_msgsMsgSystemStateSize(Ros2Abb_robot_msgsMsgSystemStateCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsMsgSystemStateCapture(value),n=Ros2Abb_robot_msgsMsgSystemStateSize(owned),w=new Writer(n);Ros2Abb_robot_msgsMsgSystemStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2327,90);if(owned.length<23)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsMsgSystemStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsMsgSystemStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["motors_on"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["auto_mode"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rapid_running"]!=="boolean")bad();n=addSize(n,1);
if(!Array.isArray(value["rapid_tasks"])||value["rapid_tasks"].length>16)bad();
n=addSize(n,4);
for(const element of value["rapid_tasks"]){
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDTaskStateSize(element));
}
if(!Array.isArray(value["mechanical_units"])||value["mechanical_units"].length>16)bad();
n=addSize(n,4);
for(const element of value["mechanical_units"]){
n=addSize(n,Ros2Abb_robot_msgsMsgMechanicalUnitStateSize(element));
}
if(n>2327)bad(90);return n;}
function Ros2Abb_robot_msgsMsgSystemStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["motors_on"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["auto_mode"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rapid_running"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["rapid_tasks"].length,true);w.at+=4;
for(const element of value["rapid_tasks"]){
Ros2Abb_robot_msgsMsgRAPIDTaskStateWrite(element,w);
}
w.v.setUint32(w.at,value["mechanical_units"].length,true);w.at+=4;
for(const element of value["mechanical_units"]){
Ros2Abb_robot_msgsMsgMechanicalUnitStateWrite(element,w);
}
}
function Ros2Abb_robot_msgsMsgSystemStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motors_on"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["auto_mode"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rapid_running"]=decoded!==0;
}
{
{const count=r.u32();if(count>16)bad();value["rapid_tasks"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Abb_robot_msgsMsgRAPIDTaskStateRead(r);
value["rapid_tasks"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["mechanical_units"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Abb_robot_msgsMsgMechanicalUnitStateRead(r);
value["mechanical_units"].push(decoded);
}}
}
return value;}
function Ros2Abb_robot_msgsMsgSystemStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["motors_on"];
out["motors_on"]=field;
}
{const field=value["auto_mode"];
out["auto_mode"]=field;
}
{const field=value["rapid_running"];
out["rapid_running"]=field;
}
{const field=value["rapid_tasks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Abb_robot_msgsMsgRAPIDTaskStateCapture(element);
}out["rapid_tasks"]=values;
}
{const field=value["mechanical_units"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Abb_robot_msgsMsgMechanicalUnitStateCapture(element);
}out["mechanical_units"]=values;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetFileContents_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetFileContents_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bdcf73a965bd05356e48019b74d633176fdeda62452bc7185d6b6ff30576016b",maxWireBytes:516,minWireBytes:4,cartesianMaxWireBytes:516n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetFileContents_RequestSize(Ros2Abb_robot_msgsSrvGetFileContents_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetFileContents_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetFileContents_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetFileContents_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,516,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetFileContents_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetFileContents_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],512).length);
if(n>516)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetFileContents_RequestWrite(value,w){
w.text(value["filename"],512);
}
function Ros2Abb_robot_msgsSrvGetFileContents_RequestRead(r){const value={};
{
const decoded=r.text(512);
value["filename"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetFileContents_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetFileContents_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetFileContents_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f05617145a40043e87cbc251f833274d2a962bd7cdaabc5f9a8e4591bd56e107",maxWireBytes:4618,minWireBytes:10,cartesianMaxWireBytes:4618n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"contents":"","result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetFileContents_ResponseSize(Ros2Abb_robot_msgsSrvGetFileContents_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetFileContents_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetFileContents_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetFileContents_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4618,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetFileContents_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetFileContents_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["contents"],4096).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>4618)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetFileContents_ResponseWrite(value,w){
w.text(value["contents"],4096);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetFileContents_ResponseRead(r){const value={};
{
const decoded=r.text(4096);
value["contents"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetFileContents_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["contents"];
out["contents"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetIOSignal_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetIOSignal_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2406d53c2fce44e72ad7e6f1c9e75ea19c4df694ff9f00917229ec758209b1f5",maxWireBytes:516,minWireBytes:4,cartesianMaxWireBytes:516n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"signal":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetIOSignal_RequestSize(Ros2Abb_robot_msgsSrvGetIOSignal_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetIOSignal_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetIOSignal_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetIOSignal_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,516,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetIOSignal_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetIOSignal_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["signal"],512).length);
if(n>516)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetIOSignal_RequestWrite(value,w){
w.text(value["signal"],512);
}
function Ros2Abb_robot_msgsSrvGetIOSignal_RequestRead(r){const value={};
{
const decoded=r.text(512);
value["signal"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetIOSignal_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["signal"];
out["signal"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetIOSignal_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetIOSignal_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1f668ef78c353f2ec8defdac619d349cda731576f2429cf07107b9b56ba9030e",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetIOSignal_ResponseSize(Ros2Abb_robot_msgsSrvGetIOSignal_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetIOSignal_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetIOSignal_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetIOSignal_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetIOSignal_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetIOSignal_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetIOSignal_ResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetIOSignal_ResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetIOSignal_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDBool_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c381080d235c78216a8e64275819ff3fb97e1b1de96e7da758c4889630b8e0ff",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestSize(Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDBool_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b853e212489fb815866fa29b165eea474a977483158b6203699c4a1293aa6acd",maxWireBytes:519,minWireBytes:7,cartesianMaxWireBytes:519n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":false,"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseSize(Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,519,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>519)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseWrite(value,w){
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_71057c2fe44bc96505ec6e05d998fcb6323b281fff40808ce990207c48626979",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestSize(Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_af0420c126368a7e45b728c7fef76062b5ef2f0adea7535ad38e4771244edd56",maxWireBytes:526,minWireBytes:14,cartesianMaxWireBytes:526n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0,"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseSize(Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,526,90);if(owned.length<14)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"]))bad();n=addSize(n,8);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>526)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseWrite(value,w){
w.v.setFloat64(w.at,value["value"],true);w.at+=8;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDNum_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_03d5da34af8d90ed8a20f8ca38fa75d9e777496828978f0f348e3c1c8f67d0a5",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestSize(Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDNum_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a0da03014dd001e58efc4a5b274cd5c62016e7fa257af1120a8142a79635220d",maxWireBytes:522,minWireBytes:10,cartesianMaxWireBytes:522n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0,"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseSize(Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,522,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>522)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDString_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDString_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c7bc9c788301e619a165c18a1cdb0aca80df8def5be31da5dbf34c0038e73596",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDString_RequestSize(Ros2Abb_robot_msgsSrvGetRAPIDString_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDString_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDString_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDString_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDString_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDString_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDString_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
}
function Ros2Abb_robot_msgsSrvGetRAPIDString_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDString_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDString_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cf7bfdb2af5b91eb58b19b909f7fa3357dc1dad02716f8fa4465b53f3f4a1458",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseSize(Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e216a54cdae70f8147489928ff05a4b172ba4fc47e9ed5df4828b070354ac469",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestSize(Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b1f6de7ad0640f5645678d327804dc7e732bdad38805e642b13ed7294c21d602",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseSize(Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_caf70e11b386a933f62d06a433ca1cedaa3c8ecc75b7e8de8c1d5ae00a29f03e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestSize(Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestWrite(value,w){
}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestRead(r){const value={};
return value;}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1af2591f035379597f376ed1e2343f73a89f9fef5c29269aa420243a82e6c368",maxWireBytes:4618,minWireBytes:10,cartesianMaxWireBytes:4618n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"description":"","result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseSize(Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4618,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["description"],4096).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>4618)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseWrite(value,w){
w.text(value["description"],4096);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseRead(r){const value={};
{
const decoded=r.text(4096);
value["description"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["description"];
out["description"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetSpeedRatio_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_54ee2c897e2bf2f39c96cc42246ffe5997c8df280280433e8748b3bcf24c299c",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestSize(Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestCapture(value),n=Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestWrite(value,w){
}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestRead(r){const value={};
return value;}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 abb_robot_msgs/srv/GetSpeedRatio_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_baa271a258c7ca274e0145fea1abb7cbc43e98c82b8e40d67c4743237a19910b",maxWireBytes:519,minWireBytes:7,cartesianMaxWireBytes:519n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_ratio":0,"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseSize(Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,519,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["speed_ratio"],1,false);n=addSize(n,1);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>519)bad(90);return n;}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseWrite(value,w){
w.v.setUint8(w.at,value["speed_ratio"],true);w.at+=1;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["speed_ratio"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_ratio"];
out["speed_ratio"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetFileContents_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetFileContents_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3bfa8ad1846fc31acae47a875d3555a78a64b777923eb9b62e87a836f041b26b",maxWireBytes:4616,minWireBytes:8,cartesianMaxWireBytes:4616n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":"","contents":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetFileContents_RequestSize(Ros2Abb_robot_msgsSrvSetFileContents_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetFileContents_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetFileContents_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetFileContents_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4616,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetFileContents_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetFileContents_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],512).length);
n=addSize(n,4+textBytes(value["contents"],4096).length);
if(n>4616)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetFileContents_RequestWrite(value,w){
w.text(value["filename"],512);
w.text(value["contents"],4096);
}
function Ros2Abb_robot_msgsSrvSetFileContents_RequestRead(r){const value={};
{
const decoded=r.text(512);
value["filename"]=decoded;
}
{
const decoded=r.text(4096);
value["contents"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetFileContents_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
{const field=value["contents"];
out["contents"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetFileContents_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetFileContents_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1586344401d43055460b49f2487567638f66838cfe5cac99a47e0a39c0b06ede",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetFileContents_ResponseSize(Ros2Abb_robot_msgsSrvSetFileContents_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetFileContents_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetFileContents_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetFileContents_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetFileContents_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetFileContents_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetFileContents_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetFileContents_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetFileContents_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetIOSignal_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetIOSignal_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2d206d0020179263e83e745a428616ed53d809b0c65a79ad495cb882ae9e49b7",maxWireBytes:1032,minWireBytes:8,cartesianMaxWireBytes:1032n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"signal":"","value":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetIOSignal_RequestSize(Ros2Abb_robot_msgsSrvSetIOSignal_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetIOSignal_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetIOSignal_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetIOSignal_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1032,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetIOSignal_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetIOSignal_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["signal"],512).length);
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>1032)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetIOSignal_RequestWrite(value,w){
w.text(value["signal"],512);
w.text(value["value"],512);
}
function Ros2Abb_robot_msgsSrvSetIOSignal_RequestRead(r){const value={};
{
const decoded=r.text(512);
value["signal"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetIOSignal_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["signal"];
out["signal"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetIOSignal_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetIOSignal_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1fa28cc3160d51f71d0740cbe145663ec360d6fa488c2a6bf14ad6ab92004a75",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetIOSignal_ResponseSize(Ros2Abb_robot_msgsSrvSetIOSignal_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetIOSignal_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetIOSignal_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetIOSignal_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetIOSignal_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetIOSignal_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetIOSignal_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetIOSignal_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetIOSignal_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDBool_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4e707e429acc860d032d216bcd2c1eb1a0f731fad7e7c0bef7beed810c6e4a52",maxWireBytes:205,minWireBytes:13,cartesianMaxWireBytes:205n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue(),"value":false};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestSize(Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,205,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
if(n>205)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDBool_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_80d2b31c93771cfa8a19ff82a3d9d63951bf9d28c039d2725017327fa3b0114a",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseSize(Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cece086311a908f12c0b0ab3988ca829e891749c74687c717d2a52bfb8ceaf76",maxWireBytes:212,minWireBytes:20,cartesianMaxWireBytes:212n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue(),"value":0};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestSize(Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,212,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"]))bad();n=addSize(n,8);
if(n>212)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
w.v.setFloat64(w.at,value["value"],true);w.at+=8;
}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e5f77fe9850f109d873a2784b9690c8aa0632b9e91d158ed4d14daf1aaf90ea6",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseSize(Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDNum_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a5bee9a64be69dc4246f68837a44362f52795ae3c6420a2df47685606283ea02",maxWireBytes:208,minWireBytes:16,cartesianMaxWireBytes:208n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue(),"value":0};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestSize(Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,208,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>208)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDNum_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_744f3c5fac58ff098c96cedcea94fe6fa87c88ceb89674449f50fbdcd1ca30bb",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseSize(Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDString_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDString_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f21c45f3f8cd4c0b50781bdd487fdc6ec0c0b35adbc7e36e9ca5b7cc47c3d461",maxWireBytes:720,minWireBytes:16,cartesianMaxWireBytes:720n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue(),"value":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDString_RequestSize(Ros2Abb_robot_msgsSrvSetRAPIDString_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDString_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDString_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDString_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,720,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDString_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDString_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>720)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDString_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
w.text(value["value"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDString_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDString_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDString_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1496ef4f4ffe48fe74b9e25b925c19f10f0e171a9d33fa2e56c15aa5cfbe4774",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseSize(Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_928d427801b79775595fed6e3779f9ba9e7e8e27f2de66cf79610cb51bb72bcf",maxWireBytes:720,minWireBytes:16,cartesianMaxWireBytes:720n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec.defaultValue(),"value":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestSize(Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,720,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Abb_robot_msgsMsgRAPIDSymbolPathSize(value["path"]));
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>720)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestWrite(value,w){
Ros2Abb_robot_msgsMsgRAPIDSymbolPathWrite(value["path"],w);
w.text(value["value"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestRead(r){const value={};
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros2Abb_robot_msgsMsgRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e156616a51bf4691a1c3853d62a8b0c43bb17d1a330cd4e7a58eceb5fe442382",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseSize(Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetSpeedRatio_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_da30970377ba1bc151b544560773f02e17f4eaed56eb4c8d791de72bdef04355",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_ratio":0};},
validateValue(value){Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestSize(Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestCapture(value),n=Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["speed_ratio"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestWrite(value,w){
w.v.setUint8(w.at,value["speed_ratio"],true);w.at+=1;
}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["speed_ratio"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_ratio"];
out["speed_ratio"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/SetSpeedRatio_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_25e6d7edaa0503a4d45d893e886db4215bfd0af8711307c141bc63552609e3a5",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseSize(Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Request codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fd1d49c3b4e489464c6662f306210137f71d15192f64cd8a8abf93acd5fbff8d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestSize(Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestCapture(value),n=Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestWrite(value,w){
}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestRead(r){const value={};
return value;}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Response codec; maxima are not actual lengths. */
export const Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e38422b2a008f07e108505ac3ce65c24189a1d6657e88a6289e17d7d4ae38d51",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseSize(Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseCapture(value),n=Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseSize(owned),w=new Writer(n);Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 builtin_interfaces/msg/Duration codec; maxima are not actual lengths. */
export const Ros2Builtin_interfacesMsgDurationCodec=Object.freeze({codecIdentity:"UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nanosec":0};},
validateValue(value){Ros2Builtin_interfacesMsgDurationSize(Ros2Builtin_interfacesMsgDurationCapture(value));return true;},
encode(value){const owned=Ros2Builtin_interfacesMsgDurationCapture(value),n=Ros2Builtin_interfacesMsgDurationSize(owned),w=new Writer(n);Ros2Builtin_interfacesMsgDurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Builtin_interfacesMsgDurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Builtin_interfacesMsgDurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nanosec"],4,false);n=addSize(n,4);
if(value["nanosec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros2Builtin_interfacesMsgDurationWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nanosec"],true);w.at+=4;
}
function Ros2Builtin_interfacesMsgDurationRead(r){const value={};
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
function Ros2Builtin_interfacesMsgDurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nanosec"];
out["nanosec"]=field;
}
return out;}
/** Complete ROS2 trajectory_msgs/msg/JointTrajectoryPoint codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgJointTrajectoryPointCodec=Object.freeze({codecIdentity:"UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5",maxWireBytes:408,minWireBytes:24,cartesianMaxWireBytes:408n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"positions":[],"velocities":[],"accelerations":[],"effort":[],"time_from_start":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Trajectory_msgsMsgJointTrajectoryPointSize(Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value),n=Ros2Trajectory_msgsMsgJointTrajectoryPointSize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,408,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["positions"])||value["positions"].length>12)bad();
n=addSize(n,4);
for(const element of value["positions"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocities"])||value["velocities"].length>12)bad();
n=addSize(n,4);
for(const element of value["velocities"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["accelerations"])||value["accelerations"].length>12)bad();
n=addSize(n,4);
for(const element of value["accelerations"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>12)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["time_from_start"]));
if(n>408)bad(90);return n;}
function Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value,w){
w.v.setUint32(w.at,value["positions"].length,true);w.at+=4;
for(const element of value["positions"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["velocities"].length,true);w.at+=4;
for(const element of value["velocities"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["accelerations"].length,true);w.at+=4;
for(const element of value["accelerations"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["effort"].length,true);w.at+=4;
for(const element of value["effort"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros2Builtin_interfacesMsgDurationWrite(value["time_from_start"],w);
}
function Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r){const value={};
{
{const count=r.u32();if(count>12)bad();value["positions"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["positions"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["velocities"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocities"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["accelerations"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["accelerations"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["effort"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["effort"].push(decoded);
}}
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["time_from_start"]=decoded;
}
return value;}
function Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["positions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["positions"]=values;
}
{const field=value["velocities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocities"]=values;
}
{const field=value["accelerations"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["accelerations"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
{const field=value["time_from_start"];
out["time_from_start"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 trajectory_msgs/msg/JointTrajectory codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgJointTrajectoryCodec=Object.freeze({codecIdentity:"UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0",maxWireBytes:4164,minWireBytes:20,cartesianMaxWireBytes:4164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_names":[],"points":[]};},
validateValue(value){Ros2Trajectory_msgsMsgJointTrajectorySize(Ros2Trajectory_msgsMsgJointTrajectoryCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgJointTrajectoryCapture(value),n=Ros2Trajectory_msgsMsgJointTrajectorySize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgJointTrajectoryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4164,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgJointTrajectoryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgJointTrajectorySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>12)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["points"])||value["points"].length>8)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(element));
}
if(n>4164)bad(90);return n;}
function Ros2Trajectory_msgsMsgJointTrajectoryWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(element,w);
}
}
function Ros2Trajectory_msgsMsgJointTrajectoryRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>8)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros2Trajectory_msgsMsgJointTrajectoryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>8)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(element);
}out["points"]=values;
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
/** Complete ROS2 geometry_msgs/msg/Transform codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTransformCodec=Object.freeze({codecIdentity:"UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b",maxWireBytes:56,minWireBytes:56,cartesianMaxWireBytes:56n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"translation":Ros2Geometry_msgsMsgVector3Codec.defaultValue(),"rotation":Ros2Geometry_msgsMsgQuaternionCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTransformSize(Ros2Geometry_msgsMsgTransformCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTransformCapture(value),n=Ros2Geometry_msgsMsgTransformSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTransformWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,56,90);if(owned.length<56)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTransformRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTransformSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Geometry_msgsMsgVector3Size(value["translation"]));
n=addSize(n,Ros2Geometry_msgsMsgQuaternionSize(value["rotation"]));
if(n>56)bad(90);return n;}
function Ros2Geometry_msgsMsgTransformWrite(value,w){
Ros2Geometry_msgsMsgVector3Write(value["translation"],w);
Ros2Geometry_msgsMsgQuaternionWrite(value["rotation"],w);
}
function Ros2Geometry_msgsMsgTransformRead(r){const value={};
{
const decoded=Ros2Geometry_msgsMsgVector3Read(r);
value["translation"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternionRead(r);
value["rotation"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTransformCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["translation"];
out["translation"]=Ros2Geometry_msgsMsgVector3Capture(field);
}
{const field=value["rotation"];
out["rotation"]=Ros2Geometry_msgsMsgQuaternionCapture(field);
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
/** Complete ROS2 trajectory_msgs/msg/MultiDOFJointTrajectoryPoint codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec=Object.freeze({codecIdentity:"UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242",maxWireBytes:324,minWireBytes:20,cartesianMaxWireBytes:324n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"transforms":[],"velocities":[],"accelerations":[],"time_from_start":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(value),n=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,324,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["transforms"])||value["transforms"].length>2)bad();
n=addSize(n,4);
for(const element of value["transforms"]){
n=addSize(n,Ros2Geometry_msgsMsgTransformSize(element));
}
if(!Array.isArray(value["velocities"])||value["velocities"].length>2)bad();
n=addSize(n,4);
for(const element of value["velocities"]){
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(element));
}
if(!Array.isArray(value["accelerations"])||value["accelerations"].length>2)bad();
n=addSize(n,4);
for(const element of value["accelerations"]){
n=addSize(n,Ros2Geometry_msgsMsgTwistSize(element));
}
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["time_from_start"]));
if(n>324)bad(90);return n;}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(value,w){
w.v.setUint32(w.at,value["transforms"].length,true);w.at+=4;
for(const element of value["transforms"]){
Ros2Geometry_msgsMsgTransformWrite(element,w);
}
w.v.setUint32(w.at,value["velocities"].length,true);w.at+=4;
for(const element of value["velocities"]){
Ros2Geometry_msgsMsgTwistWrite(element,w);
}
w.v.setUint32(w.at,value["accelerations"].length,true);w.at+=4;
for(const element of value["accelerations"]){
Ros2Geometry_msgsMsgTwistWrite(element,w);
}
Ros2Builtin_interfacesMsgDurationWrite(value["time_from_start"],w);
}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["transforms"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTransformRead(r);
value["transforms"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["velocities"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["velocities"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["accelerations"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwistRead(r);
value["accelerations"].push(decoded);
}}
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["time_from_start"]=decoded;
}
return value;}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["transforms"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgTransformCapture(element);
}out["transforms"]=values;
}
{const field=value["velocities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgTwistCapture(element);
}out["velocities"]=values;
}
{const field=value["accelerations"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgTwistCapture(element);
}out["accelerations"]=values;
}
{const field=value["time_from_start"];
out["time_from_start"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 trajectory_msgs/msg/MultiDOFJointTrajectory codec; maxima are not actual lengths. */
export const Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCodec=Object.freeze({codecIdentity:"UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401",maxWireBytes:1516,minWireBytes:20,cartesianMaxWireBytes:1516n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_names":[],"points":[]};},
validateValue(value){Ros2Trajectory_msgsMsgMultiDOFJointTrajectorySize(Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCapture(value));return true;},
encode(value){const owned=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCapture(value),n=Ros2Trajectory_msgsMsgMultiDOFJointTrajectorySize(owned),w=new Writer(n);Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1516,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectorySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>2)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["points"])||value["points"].length>4)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(element));
}
if(n>1516)bad(90);return n;}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(element,w);
}
}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS2 control_msgs/msg/JointTolerance codec; maxima are not actual lengths. */
export const Ros2Control_msgsMsgJointToleranceCodec=Object.freeze({codecIdentity:"UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2",maxWireBytes:92,minWireBytes:28,cartesianMaxWireBytes:92n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","position":0,"velocity":0,"acceleration":0};},
validateValue(value){Ros2Control_msgsMsgJointToleranceSize(Ros2Control_msgsMsgJointToleranceCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsMsgJointToleranceCapture(value),n=Ros2Control_msgsMsgJointToleranceSize(owned),w=new Writer(n);Ros2Control_msgsMsgJointToleranceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,92,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Control_msgsMsgJointToleranceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsMsgJointToleranceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"]))bad();n=addSize(n,8);
if(typeof value["acceleration"]!=="number"||!Number.isFinite(value["acceleration"]))bad();n=addSize(n,8);
if(n>92)bad(90);return n;}
function Ros2Control_msgsMsgJointToleranceWrite(value,w){
w.text(value["name"],64);
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setFloat64(w.at,value["velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["acceleration"],true);w.at+=8;
}
function Ros2Control_msgsMsgJointToleranceRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
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
return value;}
function Ros2Control_msgsMsgJointToleranceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["position"];
out["position"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["acceleration"];
out["acceleration"]=field;
}
return out;}
/** Complete ROS2 control_msgs/msg/JointComponentTolerance codec; maxima are not actual lengths. */
export const Ros2Control_msgsMsgJointComponentToleranceCodec=Object.freeze({codecIdentity:"UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342",maxWireBytes:542,minWireBytes:30,cartesianMaxWireBytes:542n,aggregateWireBytes:0,constants:Object.freeze({"X_AXIS":1,"Y_AXIS":2,"Z_AXIS":3,"TRANSLATION":4,"ROTATION":5}),
defaultValue(){return {"joint_name":"","component":0,"position":0,"velocity":0,"acceleration":0};},
validateValue(value){Ros2Control_msgsMsgJointComponentToleranceSize(Ros2Control_msgsMsgJointComponentToleranceCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsMsgJointComponentToleranceCapture(value),n=Ros2Control_msgsMsgJointComponentToleranceSize(owned),w=new Writer(n);Ros2Control_msgsMsgJointComponentToleranceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,542,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Control_msgsMsgJointComponentToleranceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsMsgJointComponentToleranceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["joint_name"],512).length);
integer(value["component"],2,false);n=addSize(n,2);
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"]))bad();n=addSize(n,8);
if(typeof value["acceleration"]!=="number"||!Number.isFinite(value["acceleration"]))bad();n=addSize(n,8);
if(n>542)bad(90);return n;}
function Ros2Control_msgsMsgJointComponentToleranceWrite(value,w){
w.text(value["joint_name"],512);
w.v.setUint16(w.at,value["component"],true);w.at+=2;
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setFloat64(w.at,value["velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["acceleration"],true);w.at+=8;
}
function Ros2Control_msgsMsgJointComponentToleranceRead(r){const value={};
{
const decoded=r.text(512);
value["joint_name"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["component"]=decoded;
}
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
return value;}
function Ros2Control_msgsMsgJointComponentToleranceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_name"];
out["joint_name"]=field;
}
{const field=value["component"];
out["component"]=field;
}
{const field=value["position"];
out["position"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["acceleration"];
out["acceleration"]=field;
}
return out;}
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Goal codec; maxima are not actual lengths. */
export const Ros2Control_msgsActionFollowJointTrajectory_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16",maxWireBytes:20920,minWireBytes:64,cartesianMaxWireBytes:20920n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trajectory":Ros2Trajectory_msgsMsgJointTrajectoryCodec.defaultValue(),"multi_dof_trajectory":Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCodec.defaultValue(),"path_tolerance":[],"component_path_tolerance":[],"goal_tolerance":[],"component_goal_tolerance":[],"goal_time_tolerance":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Control_msgsActionFollowJointTrajectory_GoalSize(Ros2Control_msgsActionFollowJointTrajectory_GoalCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsActionFollowJointTrajectory_GoalCapture(value),n=Ros2Control_msgsActionFollowJointTrajectory_GoalSize(owned),w=new Writer(n);Ros2Control_msgsActionFollowJointTrajectory_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20920,90);if(owned.length<64)bad();const r=new Reader(owned),value=Ros2Control_msgsActionFollowJointTrajectory_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsActionFollowJointTrajectory_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectorySize(value["trajectory"]));
n=addSize(n,Ros2Trajectory_msgsMsgMultiDOFJointTrajectorySize(value["multi_dof_trajectory"]));
if(!Array.isArray(value["path_tolerance"])||value["path_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["path_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointToleranceSize(element));
}
if(!Array.isArray(value["component_path_tolerance"])||value["component_path_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["component_path_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointComponentToleranceSize(element));
}
if(!Array.isArray(value["goal_tolerance"])||value["goal_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["goal_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointToleranceSize(element));
}
if(!Array.isArray(value["component_goal_tolerance"])||value["component_goal_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["component_goal_tolerance"]){
n=addSize(n,Ros2Control_msgsMsgJointComponentToleranceSize(element));
}
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["goal_time_tolerance"]));
if(n>20920)bad(90);return n;}
function Ros2Control_msgsActionFollowJointTrajectory_GoalWrite(value,w){
Ros2Trajectory_msgsMsgJointTrajectoryWrite(value["trajectory"],w);
Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryWrite(value["multi_dof_trajectory"],w);
w.v.setUint32(w.at,value["path_tolerance"].length,true);w.at+=4;
for(const element of value["path_tolerance"]){
Ros2Control_msgsMsgJointToleranceWrite(element,w);
}
w.v.setUint32(w.at,value["component_path_tolerance"].length,true);w.at+=4;
for(const element of value["component_path_tolerance"]){
Ros2Control_msgsMsgJointComponentToleranceWrite(element,w);
}
w.v.setUint32(w.at,value["goal_tolerance"].length,true);w.at+=4;
for(const element of value["goal_tolerance"]){
Ros2Control_msgsMsgJointToleranceWrite(element,w);
}
w.v.setUint32(w.at,value["component_goal_tolerance"].length,true);w.at+=4;
for(const element of value["component_goal_tolerance"]){
Ros2Control_msgsMsgJointComponentToleranceWrite(element,w);
}
Ros2Builtin_interfacesMsgDurationWrite(value["goal_time_tolerance"],w);
}
function Ros2Control_msgsActionFollowJointTrajectory_GoalRead(r){const value={};
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryRead(r);
value["trajectory"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryRead(r);
value["multi_dof_trajectory"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["path_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointToleranceRead(r);
value["path_tolerance"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["component_path_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentToleranceRead(r);
value["component_path_tolerance"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["goal_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointToleranceRead(r);
value["goal_tolerance"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["component_goal_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentToleranceRead(r);
value["component_goal_tolerance"].push(decoded);
}}
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["goal_time_tolerance"]=decoded;
}
return value;}
function Ros2Control_msgsActionFollowJointTrajectory_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trajectory"];
out["trajectory"]=Ros2Trajectory_msgsMsgJointTrajectoryCapture(field);
}
{const field=value["multi_dof_trajectory"];
out["multi_dof_trajectory"]=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCapture(field);
}
{const field=value["path_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointToleranceCapture(element);
}out["path_tolerance"]=values;
}
{const field=value["component_path_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointComponentToleranceCapture(element);
}out["component_path_tolerance"]=values;
}
{const field=value["goal_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointToleranceCapture(element);
}out["goal_tolerance"]=values;
}
{const field=value["component_goal_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Control_msgsMsgJointComponentToleranceCapture(element);
}out["component_goal_tolerance"]=values;
}
{const field=value["goal_time_tolerance"];
out["goal_time_tolerance"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Result codec; maxima are not actual lengths. */
export const Ros2Control_msgsActionFollowJointTrajectory_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70",maxWireBytes:4104,minWireBytes:8,cartesianMaxWireBytes:4104n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESSFUL":0,"INVALID_GOAL":-1,"INVALID_JOINTS":-2,"OLD_HEADER_TIMESTAMP":-3,"PATH_TOLERANCE_VIOLATED":-4,"GOAL_TOLERANCE_VIOLATED":-5}),
defaultValue(){return {"error_code":0,"error_string":""};},
validateValue(value){Ros2Control_msgsActionFollowJointTrajectory_ResultSize(Ros2Control_msgsActionFollowJointTrajectory_ResultCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsActionFollowJointTrajectory_ResultCapture(value),n=Ros2Control_msgsActionFollowJointTrajectory_ResultSize(owned),w=new Writer(n);Ros2Control_msgsActionFollowJointTrajectory_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4104,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Control_msgsActionFollowJointTrajectory_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsActionFollowJointTrajectory_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["error_code"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["error_string"],4096).length);
if(n>4104)bad(90);return n;}
function Ros2Control_msgsActionFollowJointTrajectory_ResultWrite(value,w){
w.v.setInt32(w.at,value["error_code"],true);w.at+=4;
w.text(value["error_string"],4096);
}
function Ros2Control_msgsActionFollowJointTrajectory_ResultRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=r.text(4096);
value["error_string"]=decoded;
}
return value;}
function Ros2Control_msgsActionFollowJointTrajectory_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_string"];
out["error_string"]=field;
}
return out;}
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Feedback codec; maxima are not actual lengths. */
export const Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb",maxWireBytes:4128,minWireBytes:152,cartesianMaxWireBytes:4128n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"joint_names":[],"desired":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue(),"actual":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue(),"error":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue(),"multi_dof_joint_names":[],"multi_dof_desired":Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec.defaultValue(),"multi_dof_actual":Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec.defaultValue(),"multi_dof_error":Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec.defaultValue()};},
validateValue(value){Ros2Control_msgsActionFollowJointTrajectory_FeedbackSize(Ros2Control_msgsActionFollowJointTrajectory_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Control_msgsActionFollowJointTrajectory_FeedbackCapture(value),n=Ros2Control_msgsActionFollowJointTrajectory_FeedbackSize(owned),w=new Writer(n);Ros2Control_msgsActionFollowJointTrajectory_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4128,90);if(owned.length<152)bad();const r=new Reader(owned),value=Ros2Control_msgsActionFollowJointTrajectory_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Control_msgsActionFollowJointTrajectory_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>12)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["desired"]));
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["actual"]));
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["error"]));
if(!Array.isArray(value["multi_dof_joint_names"])||value["multi_dof_joint_names"].length>2)bad();
n=addSize(n,4);
for(const element of value["multi_dof_joint_names"]){
n=addSize(n,4+textBytes(element,512).length);
}
n=addSize(n,Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(value["multi_dof_desired"]));
n=addSize(n,Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(value["multi_dof_actual"]));
n=addSize(n,Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointSize(value["multi_dof_error"]));
if(n>4128)bad(90);return n;}
function Ros2Control_msgsActionFollowJointTrajectory_FeedbackWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["desired"],w);
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["actual"],w);
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["error"],w);
w.v.setUint32(w.at,value["multi_dof_joint_names"].length,true);w.at+=4;
for(const element of value["multi_dof_joint_names"]){
w.text(element,512);
}
Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(value["multi_dof_desired"],w);
Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(value["multi_dof_actual"],w);
Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointWrite(value["multi_dof_error"],w);
}
function Ros2Control_msgsActionFollowJointTrajectory_FeedbackRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["desired"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["actual"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["error"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["multi_dof_joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(512);
value["multi_dof_joint_names"].push(decoded);
}}
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r);
value["multi_dof_desired"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r);
value["multi_dof_actual"]=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointRead(r);
value["multi_dof_error"]=decoded;
}
return value;}
function Ros2Control_msgsActionFollowJointTrajectory_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["desired"];
out["desired"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
{const field=value["actual"];
out["actual"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
{const field=value["error"];
out["error"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
{const field=value["multi_dof_joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["multi_dof_joint_names"]=values;
}
{const field=value["multi_dof_desired"];
out["multi_dof_desired"]=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(field);
}
{const field=value["multi_dof_actual"];
out["multi_dof_actual"]=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(field);
}
{const field=value["multi_dof_error"];
out["multi_dof_error"]=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCapture(field);
}
return out;}
/** Complete ROS2 sensor_msgs/msg/JointState codec; maxima are not actual lengths. */
export const Ros2Sensor_msgsMsgJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884",maxWireBytes:1196,minWireBytes:28,cartesianMaxWireBytes:1196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"name":[],"position":[],"velocity":[],"effort":[]};},
validateValue(value){Ros2Sensor_msgsMsgJointStateSize(Ros2Sensor_msgsMsgJointStateCapture(value));return true;},
encode(value){const owned=Ros2Sensor_msgsMsgJointStateCapture(value),n=Ros2Sensor_msgsMsgJointStateSize(owned),w=new Writer(n);Ros2Sensor_msgsMsgJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1196,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Sensor_msgsMsgJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Sensor_msgsMsgJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["name"])||value["name"].length>12)bad();
n=addSize(n,4);
for(const element of value["name"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["position"])||value["position"].length>12)bad();
n=addSize(n,4);
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocity"])||value["velocity"].length>12)bad();
n=addSize(n,4);
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>12)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
if(n>1196)bad(90);return n;}
function Ros2Sensor_msgsMsgJointStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["name"].length,true);w.at+=4;
for(const element of value["name"]){
w.text(element,64);
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
{const count=r.u32();if(count>12)bad();value["name"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["name"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["effort"]=[];for(let i=0;i<count;++i){
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
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["name"]=values;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
return out;}
/** Complete ROS2 std_srvs/srv/Trigger_Request codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvTrigger_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Std_srvsSrvTrigger_RequestSize(Ros2Std_srvsSrvTrigger_RequestCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvTrigger_RequestCapture(value),n=Ros2Std_srvsSrvTrigger_RequestSize(owned),w=new Writer(n);Ros2Std_srvsSrvTrigger_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvTrigger_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvTrigger_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Std_srvsSrvTrigger_RequestWrite(value,w){
}
function Ros2Std_srvsSrvTrigger_RequestRead(r){const value={};
return value;}
function Ros2Std_srvsSrvTrigger_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 std_srvs/srv/Trigger_Response codec; maxima are not actual lengths. */
export const Ros2Std_srvsSrvTrigger_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Std_srvsSrvTrigger_ResponseSize(Ros2Std_srvsSrvTrigger_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Std_srvsSrvTrigger_ResponseCapture(value),n=Ros2Std_srvsSrvTrigger_ResponseSize(owned),w=new Writer(n);Ros2Std_srvsSrvTrigger_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Std_srvsSrvTrigger_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Std_srvsSrvTrigger_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>517)bad(90);return n;}
function Ros2Std_srvsSrvTrigger_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],512);
}
function Ros2Std_srvsSrvTrigger_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Std_srvsSrvTrigger_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
