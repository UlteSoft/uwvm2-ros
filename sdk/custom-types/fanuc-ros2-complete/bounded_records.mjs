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
/** Complete ROS2 fanuc_msgs/msg/IOType codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgIOTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_1815aa6b1bcab1efe21de0e31570ca650553b5c70f0793301f82c0ca7934c586",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({"AI":"AI","AO":"AO","DI":"DI","DO":"DO","RI":"RI","RO":"RO","SI":"SI","SO":"SO","WI":"WI","WO":"WO","UI":"UI","UO":"UO","WSI":"WSI","WSO":"WSO","F":"F","M":"M","GI":"GI","GO":"GO"}),
defaultValue(){return {"type":""};},
validateValue(value){Ros2Fanuc_msgsMsgIOTypeSize(Ros2Fanuc_msgsMsgIOTypeCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgIOTypeCapture(value),n=Ros2Fanuc_msgsMsgIOTypeSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgIOTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgIOTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgIOTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
if(n>68)bad(90);return n;}
function Ros2Fanuc_msgsMsgIOTypeWrite(value,w){
w.text(value["type"],64);
}
function Ros2Fanuc_msgsMsgIOTypeRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgIOTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/AnalogIO codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgAnalogIOCodec=Object.freeze({codecIdentity:"UWVMROS1_d62097527ba99bf6e803283350161cd1415173a22eb3cb20331a232366076d00",maxWireBytes:74,minWireBytes:10,cartesianMaxWireBytes:74n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsMsgAnalogIOSize(Ros2Fanuc_msgsMsgAnalogIOCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgAnalogIOCapture(value),n=Ros2Fanuc_msgsMsgAnalogIOSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgAnalogIOWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,74,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgAnalogIORead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgAnalogIOSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>74)bad(90);return n;}
function Ros2Fanuc_msgsMsgAnalogIOWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsMsgAnalogIORead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgAnalogIOCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/AnalogIOCmd codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgAnalogIOCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_62f38a088ed23f8daf7163f26514ae1aabd7b1b64845ccdbc281cb7839d857ba",maxWireBytes:2372,minWireBytes:4,cartesianMaxWireBytes:2372n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgAnalogIOCmdSize(Ros2Fanuc_msgsMsgAnalogIOCmdCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgAnalogIOCmdCapture(value),n=Ros2Fanuc_msgsMsgAnalogIOCmdSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgAnalogIOCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2372,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgAnalogIOCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgAnalogIOCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgAnalogIOSize(element));
}
if(n>2372)bad(90);return n;}
function Ros2Fanuc_msgsMsgAnalogIOCmdWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgAnalogIOWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgAnalogIOCmdRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgAnalogIORead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgAnalogIOCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgAnalogIOCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/AnalogIOState codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgAnalogIOStateCodec=Object.freeze({codecIdentity:"UWVMROS1_820b41fecbf6db8e2b2e67d60cd1167cf474b22ca6c489f4d2d64dd33eaac9a4",maxWireBytes:2372,minWireBytes:4,cartesianMaxWireBytes:2372n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgAnalogIOStateSize(Ros2Fanuc_msgsMsgAnalogIOStateCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgAnalogIOStateCapture(value),n=Ros2Fanuc_msgsMsgAnalogIOStateSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgAnalogIOStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2372,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgAnalogIOStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgAnalogIOStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgAnalogIOSize(element));
}
if(n>2372)bad(90);return n;}
function Ros2Fanuc_msgsMsgAnalogIOStateWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgAnalogIOWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgAnalogIOStateRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgAnalogIORead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgAnalogIOStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgAnalogIOCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/BoolIO codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgBoolIOCodec=Object.freeze({codecIdentity:"UWVMROS1_9209ae1b8f5407a31beface0a3286f58bd823aabdc560c2a4bf4a9b1ca125aa7",maxWireBytes:71,minWireBytes:7,cartesianMaxWireBytes:71n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0,"value":false};},
validateValue(value){Ros2Fanuc_msgsMsgBoolIOSize(Ros2Fanuc_msgsMsgBoolIOCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgBoolIOCapture(value),n=Ros2Fanuc_msgsMsgBoolIOSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgBoolIOWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,71,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgBoolIORead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgBoolIOSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
if(n>71)bad(90);return n;}
function Ros2Fanuc_msgsMsgBoolIOWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
}
function Ros2Fanuc_msgsMsgBoolIORead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
return value;}
function Ros2Fanuc_msgsMsgBoolIOCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/CollaborativeSpeedScaling codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgCollaborativeSpeedScalingCodec=Object.freeze({codecIdentity:"UWVMROS1_693a3b581527d2eaf0f697f31e87ca2da428d1f51b821d42d6b06af387a2fb45",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"collaborative_speed_scaling":0};},
validateValue(value){Ros2Fanuc_msgsMsgCollaborativeSpeedScalingSize(Ros2Fanuc_msgsMsgCollaborativeSpeedScalingCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgCollaborativeSpeedScalingCapture(value),n=Ros2Fanuc_msgsMsgCollaborativeSpeedScalingSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgCollaborativeSpeedScalingWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgCollaborativeSpeedScalingRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgCollaborativeSpeedScalingSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["collaborative_speed_scaling"]!=="number"||!Number.isFinite(value["collaborative_speed_scaling"]))bad();n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Fanuc_msgsMsgCollaborativeSpeedScalingWrite(value,w){
w.v.setFloat64(w.at,value["collaborative_speed_scaling"],true);w.at+=8;
}
function Ros2Fanuc_msgsMsgCollaborativeSpeedScalingRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["collaborative_speed_scaling"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgCollaborativeSpeedScalingCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["collaborative_speed_scaling"];
out["collaborative_speed_scaling"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/ConnectionStatus codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgConnectionStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_8ce24fb87538922b1f3c0ae856eb223d716143064eb46a6cfce5fec01b45b078",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"is_connected":false};},
validateValue(value){Ros2Fanuc_msgsMsgConnectionStatusSize(Ros2Fanuc_msgsMsgConnectionStatusCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgConnectionStatusCapture(value),n=Ros2Fanuc_msgsMsgConnectionStatusSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgConnectionStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgConnectionStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgConnectionStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["is_connected"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsMsgConnectionStatusWrite(value,w){
w.v.setUint8(w.at,value["is_connected"]?1:0,true);w.at+=1;
}
function Ros2Fanuc_msgsMsgConnectionStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_connected"]=decoded!==0;
}
return value;}
function Ros2Fanuc_msgsMsgConnectionStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["is_connected"];
out["is_connected"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/ForceSensor codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgForceSensorCodec=Object.freeze({codecIdentity:"UWVMROS1_06c99b43d7347646179850f3ba79192da7ce71296667d93d88659efbc1004032",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"force_x":0,"force_y":0,"force_z":0,"moment_x":0,"moment_y":0,"moment_z":0,"fs_type":0};},
validateValue(value){Ros2Fanuc_msgsMsgForceSensorSize(Ros2Fanuc_msgsMsgForceSensorCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgForceSensorCapture(value),n=Ros2Fanuc_msgsMsgForceSensorSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgForceSensorWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgForceSensorRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgForceSensorSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["force_x"]!=="number"||!Number.isFinite(value["force_x"])||!Number.isFinite(Math.fround(value["force_x"])))bad();n=addSize(n,4);
if(typeof value["force_y"]!=="number"||!Number.isFinite(value["force_y"])||!Number.isFinite(Math.fround(value["force_y"])))bad();n=addSize(n,4);
if(typeof value["force_z"]!=="number"||!Number.isFinite(value["force_z"])||!Number.isFinite(Math.fround(value["force_z"])))bad();n=addSize(n,4);
if(typeof value["moment_x"]!=="number"||!Number.isFinite(value["moment_x"])||!Number.isFinite(Math.fround(value["moment_x"])))bad();n=addSize(n,4);
if(typeof value["moment_y"]!=="number"||!Number.isFinite(value["moment_y"])||!Number.isFinite(Math.fround(value["moment_y"])))bad();n=addSize(n,4);
if(typeof value["moment_z"]!=="number"||!Number.isFinite(value["moment_z"])||!Number.isFinite(Math.fround(value["moment_z"])))bad();n=addSize(n,4);
integer(value["fs_type"],4,false);n=addSize(n,4);
if(n>28)bad(90);return n;}
function Ros2Fanuc_msgsMsgForceSensorWrite(value,w){
w.v.setFloat32(w.at,value["force_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["force_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["force_z"],true);w.at+=4;
w.v.setFloat32(w.at,value["moment_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["moment_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["moment_z"],true);w.at+=4;
w.v.setUint32(w.at,value["fs_type"],true);w.at+=4;
}
function Ros2Fanuc_msgsMsgForceSensorRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["force_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["force_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["force_z"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["moment_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["moment_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["moment_z"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["fs_type"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgForceSensorCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["force_x"];
out["force_x"]=field;
}
{const field=value["force_y"];
out["force_y"]=field;
}
{const field=value["force_z"];
out["force_z"]=field;
}
{const field=value["moment_x"];
out["moment_x"]=field;
}
{const field=value["moment_y"];
out["moment_y"]=field;
}
{const field=value["moment_z"];
out["moment_z"]=field;
}
{const field=value["fs_type"];
out["fs_type"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/Frame codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgFrameCodec=Object.freeze({codecIdentity:"UWVMROS1_609d1b7e49315f7f0a0fe86fa997673acd0d009e57b563690f36624b59b9effb",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"w":0,"p":0,"r":0};},
validateValue(value){Ros2Fanuc_msgsMsgFrameSize(Ros2Fanuc_msgsMsgFrameCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgFrameCapture(value),n=Ros2Fanuc_msgsMsgFrameSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgFrameWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgFrameRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgFrameSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"])||!Number.isFinite(Math.fround(value["w"])))bad();n=addSize(n,4);
if(typeof value["p"]!=="number"||!Number.isFinite(value["p"])||!Number.isFinite(Math.fround(value["p"])))bad();n=addSize(n,4);
if(typeof value["r"]!=="number"||!Number.isFinite(value["r"])||!Number.isFinite(Math.fround(value["r"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Fanuc_msgsMsgFrameWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["w"],true);w.at+=4;
w.v.setFloat32(w.at,value["p"],true);w.at+=4;
w.v.setFloat32(w.at,value["r"],true);w.at+=4;
}
function Ros2Fanuc_msgsMsgFrameRead(r){const value={};
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
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["p"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["r"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgFrameCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["p"];
out["p"]=field;
}
{const field=value["r"];
out["r"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/IOCmd codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgIOCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_462552bbaa3c9ed4caaf3108960d514e4ab61fcf2d6a84f0465b485bec1de45d",maxWireBytes:2276,minWireBytes:4,cartesianMaxWireBytes:2276n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgIOCmdSize(Ros2Fanuc_msgsMsgIOCmdCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgIOCmdCapture(value),n=Ros2Fanuc_msgsMsgIOCmdSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgIOCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2276,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgIOCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgIOCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgBoolIOSize(element));
}
if(n>2276)bad(90);return n;}
function Ros2Fanuc_msgsMsgIOCmdWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgBoolIOWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgIOCmdRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgBoolIORead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgIOCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgBoolIOCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/IOState codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgIOStateCodec=Object.freeze({codecIdentity:"UWVMROS1_332533445014c03d626a2c4dbdffd9421748aa6b73f49d407441fd6b9f2b24d8",maxWireBytes:2276,minWireBytes:4,cartesianMaxWireBytes:2276n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgIOStateSize(Ros2Fanuc_msgsMsgIOStateCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgIOStateCapture(value),n=Ros2Fanuc_msgsMsgIOStateSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgIOStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2276,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgIOStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgIOStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgBoolIOSize(element));
}
if(n>2276)bad(90);return n;}
function Ros2Fanuc_msgsMsgIOStateWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgBoolIOWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgIOStateRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgBoolIORead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgIOStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgBoolIOCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/NumReg codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgNumRegCodec=Object.freeze({codecIdentity:"UWVMROS1_0fb9344c5768ea52ab072939f368f1e73dcc0ff83e74b4cb6d35fca47c1d29cc",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"index":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsMsgNumRegSize(Ros2Fanuc_msgsMsgNumRegCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgNumRegCapture(value),n=Ros2Fanuc_msgsMsgNumRegSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgNumRegWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgNumRegRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgNumRegSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2Fanuc_msgsMsgNumRegWrite(value,w){
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsMsgNumRegRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgNumRegCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/NumRegCmd codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgNumRegCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_9c76500073889377525a0802c46bd96589a2639dd9534cf7ad2b9956cd95b5b6",maxWireBytes:196,minWireBytes:4,cartesianMaxWireBytes:196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgNumRegCmdSize(Ros2Fanuc_msgsMsgNumRegCmdCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgNumRegCmdCapture(value),n=Ros2Fanuc_msgsMsgNumRegCmdSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgNumRegCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,196,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgNumRegCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgNumRegCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgNumRegSize(element));
}
if(n>196)bad(90);return n;}
function Ros2Fanuc_msgsMsgNumRegCmdWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgNumRegWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgNumRegCmdRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgNumRegRead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgNumRegCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgNumRegCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/NumRegState codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgNumRegStateCodec=Object.freeze({codecIdentity:"UWVMROS1_9c8ff4917a6dd033e6a6776ae8525e93c2924d776f05fd54e7e8d2251fc48b3a",maxWireBytes:196,minWireBytes:4,cartesianMaxWireBytes:196n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"values":[]};},
validateValue(value){Ros2Fanuc_msgsMsgNumRegStateSize(Ros2Fanuc_msgsMsgNumRegStateCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgNumRegStateCapture(value),n=Ros2Fanuc_msgsMsgNumRegStateSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgNumRegStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,196,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgNumRegStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgNumRegStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["values"])||value["values"].length>32)bad();
n=addSize(n,4);
for(const element of value["values"]){
n=addSize(n,Ros2Fanuc_msgsMsgNumRegSize(element));
}
if(n>196)bad(90);return n;}
function Ros2Fanuc_msgsMsgNumRegStateWrite(value,w){
w.v.setUint32(w.at,value["values"].length,true);w.at+=4;
for(const element of value["values"]){
Ros2Fanuc_msgsMsgNumRegWrite(element,w);
}
}
function Ros2Fanuc_msgsMsgNumRegStateRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["values"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Fanuc_msgsMsgNumRegRead(r);
value["values"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsMsgNumRegStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["values"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Fanuc_msgsMsgNumRegCapture(element);
}out["values"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/RMIStatus codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgRMIStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_dc2e7eaabf44807142581905838f8951653682a6c6d61ebcf56494b16a10d5e8",maxWireBytes:1048,minWireBytes:24,cartesianMaxWireBytes:1048n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"program_status":0,"next_sequence":0,"last_sequence":0,"last_sequence_error":0,"last_sequence_instruction":"","last_sequence_error_message":""};},
validateValue(value){Ros2Fanuc_msgsMsgRMIStatusSize(Ros2Fanuc_msgsMsgRMIStatusCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgRMIStatusCapture(value),n=Ros2Fanuc_msgsMsgRMIStatusSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgRMIStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1048,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgRMIStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgRMIStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["program_status"],4,false);n=addSize(n,4);
integer(value["next_sequence"],4,false);n=addSize(n,4);
integer(value["last_sequence"],4,false);n=addSize(n,4);
integer(value["last_sequence_error"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["last_sequence_instruction"],512).length);
n=addSize(n,4+textBytes(value["last_sequence_error_message"],512).length);
if(n>1048)bad(90);return n;}
function Ros2Fanuc_msgsMsgRMIStatusWrite(value,w){
w.v.setUint32(w.at,value["program_status"],true);w.at+=4;
w.v.setUint32(w.at,value["next_sequence"],true);w.at+=4;
w.v.setUint32(w.at,value["last_sequence"],true);w.at+=4;
w.v.setInt32(w.at,value["last_sequence_error"],true);w.at+=4;
w.text(value["last_sequence_instruction"],512);
w.text(value["last_sequence_error_message"],512);
}
function Ros2Fanuc_msgsMsgRMIStatusRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["program_status"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["next_sequence"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["last_sequence"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["last_sequence_error"]=decoded;
}
{
const decoded=r.text(512);
value["last_sequence_instruction"]=decoded;
}
{
const decoded=r.text(512);
value["last_sequence_error_message"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgRMIStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["program_status"];
out["program_status"]=field;
}
{const field=value["next_sequence"];
out["next_sequence"]=field;
}
{const field=value["last_sequence"];
out["last_sequence"]=field;
}
{const field=value["last_sequence_error"];
out["last_sequence_error"]=field;
}
{const field=value["last_sequence_instruction"];
out["last_sequence_instruction"]=field;
}
{const field=value["last_sequence_error_message"];
out["last_sequence_error_message"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/RobotStatus codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgRobotStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_3f31ee3631dc6f4c90eb5ded6209ccebf0f4193ddfe5f5032bdd2bcd71f8cc31",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({"CONTACT_STOP_MODE_NONE":0,"CONTACT_STOP_MODE_SAFE":1,"CONTACT_STOP_MODE_STOP":2,"CONTACT_STOP_MODE_DSBL":3,"CONTACT_STOP_MODE_ESCP":4}),
defaultValue(){return {"in_error":false,"tp_enabled":false,"e_stopped":false,"motion_possible":false,"contact_stop_mode":0};},
validateValue(value){Ros2Fanuc_msgsMsgRobotStatusSize(Ros2Fanuc_msgsMsgRobotStatusCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgRobotStatusCapture(value),n=Ros2Fanuc_msgsMsgRobotStatusSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgRobotStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgRobotStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgRobotStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["in_error"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["tp_enabled"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["e_stopped"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["motion_possible"]!=="boolean")bad();n=addSize(n,1);
integer(value["contact_stop_mode"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Fanuc_msgsMsgRobotStatusWrite(value,w){
w.v.setUint8(w.at,value["in_error"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["tp_enabled"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["e_stopped"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["motion_possible"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["contact_stop_mode"],true);w.at+=1;
}
function Ros2Fanuc_msgsMsgRobotStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["in_error"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["tp_enabled"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["e_stopped"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motion_possible"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["contact_stop_mode"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgRobotStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["in_error"];
out["in_error"]=field;
}
{const field=value["tp_enabled"];
out["tp_enabled"]=field;
}
{const field=value["e_stopped"];
out["e_stopped"]=field;
}
{const field=value["motion_possible"];
out["motion_possible"]=field;
}
{const field=value["contact_stop_mode"];
out["contact_stop_mode"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/msg/RobotStatusExt codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsMsgRobotStatusExtCodec=Object.freeze({codecIdentity:"UWVMROS1_8ba47a11bfbe02026fe79b32e1ed02e5f609499ea55bb57c9f09af9fba389648",maxWireBytes:1042,minWireBytes:18,cartesianMaxWireBytes:1042n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"error_code":"","in_motion":false,"drives_powered":false,"gen_override":0,"speed_clamp_limit":0,"control_mode":""};},
validateValue(value){Ros2Fanuc_msgsMsgRobotStatusExtSize(Ros2Fanuc_msgsMsgRobotStatusExtCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsMsgRobotStatusExtCapture(value),n=Ros2Fanuc_msgsMsgRobotStatusExtSize(owned),w=new Writer(n);Ros2Fanuc_msgsMsgRobotStatusExtWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1042,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsMsgRobotStatusExtRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsMsgRobotStatusExtSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["error_code"],512).length);
if(typeof value["in_motion"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["drives_powered"]!=="boolean")bad();n=addSize(n,1);
integer(value["gen_override"],4,true);n=addSize(n,4);
if(typeof value["speed_clamp_limit"]!=="number"||!Number.isFinite(value["speed_clamp_limit"])||!Number.isFinite(Math.fround(value["speed_clamp_limit"])))bad();n=addSize(n,4);
n=addSize(n,4+textBytes(value["control_mode"],512).length);
if(n>1042)bad(90);return n;}
function Ros2Fanuc_msgsMsgRobotStatusExtWrite(value,w){
w.text(value["error_code"],512);
w.v.setUint8(w.at,value["in_motion"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["drives_powered"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["gen_override"],true);w.at+=4;
w.v.setFloat32(w.at,value["speed_clamp_limit"],true);w.at+=4;
w.text(value["control_mode"],512);
}
function Ros2Fanuc_msgsMsgRobotStatusExtRead(r){const value={};
{
const decoded=r.text(512);
value["error_code"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["in_motion"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["drives_powered"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["gen_override"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed_clamp_limit"]=decoded;
}
{
const decoded=r.text(512);
value["control_mode"]=decoded;
}
return value;}
function Ros2Fanuc_msgsMsgRobotStatusExtCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["in_motion"];
out["in_motion"]=field;
}
{const field=value["drives_powered"];
out["drives_powered"]=field;
}
{const field=value["gen_override"];
out["gen_override"]=field;
}
{const field=value["speed_clamp_limit"];
out["speed_clamp_limit"]=field;
}
{const field=value["control_mode"];
out["control_mode"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/CfgForceSensor_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvCfgForceSensor_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_01973ed21546cb747ad2ce2a990a8ec4e70efb66e1c8fc8856cf1082870984ff",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"do_reset":0,"fs_type":0};},
validateValue(value){Ros2Fanuc_msgsSrvCfgForceSensor_RequestSize(Ros2Fanuc_msgsSrvCfgForceSensor_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvCfgForceSensor_RequestCapture(value),n=Ros2Fanuc_msgsSrvCfgForceSensor_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvCfgForceSensor_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvCfgForceSensor_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvCfgForceSensor_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["do_reset"],4,false);n=addSize(n,4);
integer(value["fs_type"],4,false);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Fanuc_msgsSrvCfgForceSensor_RequestWrite(value,w){
w.v.setUint32(w.at,value["do_reset"],true);w.at+=4;
w.v.setUint32(w.at,value["fs_type"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvCfgForceSensor_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["do_reset"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["fs_type"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvCfgForceSensor_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["do_reset"];
out["do_reset"]=field;
}
{const field=value["fs_type"];
out["fs_type"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/CfgForceSensor_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_41799ce44c937b1524889391db61e2b4d35795055cb4bbacfe5cc8bbd5b9a37b",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvCfgForceSensor_ResponseSize(Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCapture(value),n=Ros2Fanuc_msgsSrvCfgForceSensor_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvCfgForceSensor_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvCfgForceSensor_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvCfgForceSensor_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvCfgForceSensor_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvCfgForceSensor_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetAnalogIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetAnalogIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cd567f97de2de581975c3d4f280eec545dea37c941d316fc3708bbc4d3af94d9",maxWireBytes:70,minWireBytes:6,cartesianMaxWireBytes:70n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetAnalogIO_RequestSize(Ros2Fanuc_msgsSrvGetAnalogIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetAnalogIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetAnalogIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetAnalogIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,70,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetAnalogIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetAnalogIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(n>70)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetAnalogIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetAnalogIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetAnalogIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetAnalogIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3b53d3ce95d95454a782778c160fcc102ead869c5681c9c67224c373fef1b217",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetAnalogIO_ResponseSize(Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetAnalogIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetAnalogIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetAnalogIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetAnalogIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetAnalogIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetAnalogIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetBoolIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetBoolIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ce8c4b1efd2828e92656e897baf041e8965367a236079231101807c898d4b50c",maxWireBytes:70,minWireBytes:6,cartesianMaxWireBytes:70n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetBoolIO_RequestSize(Ros2Fanuc_msgsSrvGetBoolIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetBoolIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetBoolIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetBoolIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,70,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetBoolIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetBoolIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(n>70)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetBoolIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetBoolIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetBoolIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetBoolIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetBoolIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7dbdcdde8d76c4ef422ea158551dd912273220c25a08f28fc8d19fdf8bc8a01c",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"value":false};},
validateValue(value){Ros2Fanuc_msgsSrvGetBoolIO_ResponseSize(Ros2Fanuc_msgsSrvGetBoolIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetBoolIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetBoolIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetBoolIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetBoolIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetBoolIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetBoolIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetBoolIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
return value;}
function Ros2Fanuc_msgsSrvGetBoolIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetCartesianPosition_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a694a17bde88131efcadac4f134464a7777beab4b09e17624ae7b173ab62c3e4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetCartesianPosition_RequestSize(Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetCartesianPosition_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetCartesianPosition_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetCartesianPosition_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetCartesianPosition_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["group"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetCartesianPosition_RequestWrite(value,w){
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetCartesianPosition_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetCartesianPosition_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0516f0145717b06550674e02f0b9c47f9b312f4f361c791f56257fbb79e67329",maxWireBytes:54,minWireBytes:54,cartesianMaxWireBytes:54n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"timetag":0,"utool":0,"uframe":0,"front":0,"up":0,"left":0,"flip":0,"turn4":0,"turn5":0,"turn6":0,"x":0,"y":0,"z":0,"w":0,"p":0,"r":0,"ext1":0,"ext2":0,"ext3":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseSize(Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,54,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["timetag"],4,true);n=addSize(n,4);
integer(value["utool"],1,false);n=addSize(n,1);
integer(value["uframe"],1,false);n=addSize(n,1);
integer(value["front"],1,false);n=addSize(n,1);
integer(value["up"],1,false);n=addSize(n,1);
integer(value["left"],1,false);n=addSize(n,1);
integer(value["flip"],1,false);n=addSize(n,1);
integer(value["turn4"],1,true);n=addSize(n,1);
integer(value["turn5"],1,true);n=addSize(n,1);
integer(value["turn6"],1,true);n=addSize(n,1);
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"])||!Number.isFinite(Math.fround(value["w"])))bad();n=addSize(n,4);
if(typeof value["p"]!=="number"||!Number.isFinite(value["p"])||!Number.isFinite(Math.fround(value["p"])))bad();n=addSize(n,4);
if(typeof value["r"]!=="number"||!Number.isFinite(value["r"])||!Number.isFinite(Math.fround(value["r"])))bad();n=addSize(n,4);
if(typeof value["ext1"]!=="number"||!Number.isFinite(value["ext1"])||!Number.isFinite(Math.fround(value["ext1"])))bad();n=addSize(n,4);
if(typeof value["ext2"]!=="number"||!Number.isFinite(value["ext2"])||!Number.isFinite(Math.fround(value["ext2"])))bad();n=addSize(n,4);
if(typeof value["ext3"]!=="number"||!Number.isFinite(value["ext3"])||!Number.isFinite(Math.fround(value["ext3"])))bad();n=addSize(n,4);
integer(value["group"],1,false);n=addSize(n,1);
if(n>54)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["timetag"],true);w.at+=4;
w.v.setUint8(w.at,value["utool"],true);w.at+=1;
w.v.setUint8(w.at,value["uframe"],true);w.at+=1;
w.v.setUint8(w.at,value["front"],true);w.at+=1;
w.v.setUint8(w.at,value["up"],true);w.at+=1;
w.v.setUint8(w.at,value["left"],true);w.at+=1;
w.v.setUint8(w.at,value["flip"],true);w.at+=1;
w.v.setInt8(w.at,value["turn4"],true);w.at+=1;
w.v.setInt8(w.at,value["turn5"],true);w.at+=1;
w.v.setInt8(w.at,value["turn6"],true);w.at+=1;
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["w"],true);w.at+=4;
w.v.setFloat32(w.at,value["p"],true);w.at+=4;
w.v.setFloat32(w.at,value["r"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext1"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext2"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext3"],true);w.at+=4;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["timetag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["up"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["left"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["flip"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn5"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn6"]=decoded;
}
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
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["p"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["r"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext3"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["timetag"];
out["timetag"]=field;
}
{const field=value["utool"];
out["utool"]=field;
}
{const field=value["uframe"];
out["uframe"]=field;
}
{const field=value["front"];
out["front"]=field;
}
{const field=value["up"];
out["up"]=field;
}
{const field=value["left"];
out["left"]=field;
}
{const field=value["flip"];
out["flip"]=field;
}
{const field=value["turn4"];
out["turn4"]=field;
}
{const field=value["turn5"];
out["turn5"]=field;
}
{const field=value["turn6"];
out["turn6"]=field;
}
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
{const field=value["p"];
out["p"]=field;
}
{const field=value["r"];
out["r"]=field;
}
{const field=value["ext1"];
out["ext1"]=field;
}
{const field=value["ext2"];
out["ext2"]=field;
}
{const field=value["ext3"];
out["ext3"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetGroupIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetGroupIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_dbd2e8e791e8debc745843827ed296b51bf84159f4c4fe5d912c231d621de216",maxWireBytes:70,minWireBytes:6,cartesianMaxWireBytes:70n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetGroupIO_RequestSize(Ros2Fanuc_msgsSrvGetGroupIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetGroupIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetGroupIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetGroupIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,70,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetGroupIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetGroupIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(n>70)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetGroupIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetGroupIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetGroupIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetGroupIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetGroupIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9e131b91f36badc90ddae54a9855095241ab86c51b419b684d62879fae0bda7f",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetGroupIO_ResponseSize(Ros2Fanuc_msgsSrvGetGroupIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetGroupIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetGroupIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetGroupIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetGroupIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetGroupIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["value"],2,false);n=addSize(n,2);
if(n>6)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetGroupIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetGroupIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetGroupIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetNumReg_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetNumReg_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_727b5ff4ab9c8976280df12e80d3aee03b31288087adb94ee82161574fc8dcb5",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"index":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetNumReg_RequestSize(Ros2Fanuc_msgsSrvGetNumReg_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetNumReg_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetNumReg_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetNumReg_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetNumReg_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetNumReg_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["index"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetNumReg_RequestWrite(value,w){
w.v.setUint16(w.at,value["index"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetNumReg_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetNumReg_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["index"];
out["index"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetNumReg_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetNumReg_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_78ae7ff82b3b2cd7d512fee8335f14a0759a71425fcf3d1b150dbe5e2a0179b3",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetNumReg_ResponseSize(Ros2Fanuc_msgsSrvGetNumReg_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetNumReg_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetNumReg_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetNumReg_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetNumReg_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetNumReg_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetNumReg_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetNumReg_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetNumReg_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadComp_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadComp_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d48b88619f4a68ceeed02336e9ef078c27905707a21d977448fbbfbd560f398d",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadComp_RequestSize(Ros2Fanuc_msgsSrvGetPayloadComp_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadComp_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadComp_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadComp_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadComp_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadComp_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadComp_RequestWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetPayloadComp_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPayloadComp_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadComp_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_012b216c949f99755a36678b6be28d032f698b472ce4fa9ba975bc6d453ae4e4",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"cg_x":0,"cg_y":0,"cg_z":0,"in_x":0,"in_y":0,"in_z":0,"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadComp_ResponseSize(Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadComp_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadComp_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadComp_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadComp_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
if(typeof value["cg_x"]!=="number"||!Number.isFinite(value["cg_x"])||!Number.isFinite(Math.fround(value["cg_x"])))bad();n=addSize(n,4);
if(typeof value["cg_y"]!=="number"||!Number.isFinite(value["cg_y"])||!Number.isFinite(Math.fround(value["cg_y"])))bad();n=addSize(n,4);
if(typeof value["cg_z"]!=="number"||!Number.isFinite(value["cg_z"])||!Number.isFinite(Math.fround(value["cg_z"])))bad();n=addSize(n,4);
if(typeof value["in_x"]!=="number"||!Number.isFinite(value["in_x"])||!Number.isFinite(Math.fround(value["in_x"])))bad();n=addSize(n,4);
if(typeof value["in_y"]!=="number"||!Number.isFinite(value["in_y"])||!Number.isFinite(Math.fround(value["in_y"])))bad();n=addSize(n,4);
if(typeof value["in_z"]!=="number"||!Number.isFinite(value["in_z"])||!Number.isFinite(Math.fround(value["in_z"])))bad();n=addSize(n,4);
integer(value["result"],4,true);n=addSize(n,4);
if(n>32)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadComp_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_z"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_z"],true);w.at+=4;
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetPayloadComp_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_z"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_z"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cg_x"];
out["cg_x"]=field;
}
{const field=value["cg_y"];
out["cg_y"]=field;
}
{const field=value["cg_z"];
out["cg_z"]=field;
}
{const field=value["in_x"];
out["in_x"]=field;
}
{const field=value["in_y"];
out["in_y"]=field;
}
{const field=value["in_z"];
out["in_z"]=field;
}
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadID_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadID_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6c943659905c8adc7aa4b73832584994c83bfe009d18dc30a11d99b9fc0d6eef",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadID_RequestSize(Ros2Fanuc_msgsSrvGetPayloadID_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadID_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadID_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadID_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadID_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadID_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadID_RequestWrite(value,w){
}
function Ros2Fanuc_msgsSrvGetPayloadID_RequestRead(r){const value={};
return value;}
function Ros2Fanuc_msgsSrvGetPayloadID_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadID_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadID_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5551f4eab590c87fd7679d593c9c446903bdaa02aa1ed2512b146768122a950c",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0,"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadID_ResponseSize(Ros2Fanuc_msgsSrvGetPayloadID_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadID_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadID_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadID_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadID_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadID_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
integer(value["result"],4,true);n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadID_ResponseWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetPayloadID_ResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPayloadID_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadValue_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadValue_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d82667bd644873ae4fa5de0289ed94791eb2a7af49c1b8f86ed7f91cf875407a",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadValue_RequestSize(Ros2Fanuc_msgsSrvGetPayloadValue_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadValue_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadValue_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadValue_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadValue_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadValue_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadValue_RequestWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetPayloadValue_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPayloadValue_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadValue_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c3361ceec2ccd5b4d3ef566cbd885f8cddba75b4f1dc1712144147827689f06d",maxWireBytes:32,minWireBytes:32,cartesianMaxWireBytes:32n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"cg_x":0,"cg_y":0,"cg_z":0,"in_x":0,"in_y":0,"in_z":0,"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPayloadValue_ResponseSize(Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetPayloadValue_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPayloadValue_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPayloadValue_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPayloadValue_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
if(typeof value["cg_x"]!=="number"||!Number.isFinite(value["cg_x"])||!Number.isFinite(Math.fround(value["cg_x"])))bad();n=addSize(n,4);
if(typeof value["cg_y"]!=="number"||!Number.isFinite(value["cg_y"])||!Number.isFinite(Math.fround(value["cg_y"])))bad();n=addSize(n,4);
if(typeof value["cg_z"]!=="number"||!Number.isFinite(value["cg_z"])||!Number.isFinite(Math.fround(value["cg_z"])))bad();n=addSize(n,4);
if(typeof value["in_x"]!=="number"||!Number.isFinite(value["in_x"])||!Number.isFinite(Math.fround(value["in_x"])))bad();n=addSize(n,4);
if(typeof value["in_y"]!=="number"||!Number.isFinite(value["in_y"])||!Number.isFinite(Math.fround(value["in_y"])))bad();n=addSize(n,4);
if(typeof value["in_z"]!=="number"||!Number.isFinite(value["in_z"])||!Number.isFinite(Math.fround(value["in_z"])))bad();n=addSize(n,4);
integer(value["result"],4,true);n=addSize(n,4);
if(n>32)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPayloadValue_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_z"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_z"],true);w.at+=4;
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetPayloadValue_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_z"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_z"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cg_x"];
out["cg_x"]=field;
}
{const field=value["cg_y"];
out["cg_y"]=field;
}
{const field=value["cg_z"];
out["cg_z"]=field;
}
{const field=value["in_x"];
out["in_x"]=field;
}
{const field=value["in_y"];
out["in_y"]=field;
}
{const field=value["in_z"];
out["in_z"]=field;
}
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPosReg_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPosReg_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f7f1391c5ea385bff615d95c5e38f01a809828b142112829dc436916b063505a",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"index":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPosReg_RequestSize(Ros2Fanuc_msgsSrvGetPosReg_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPosReg_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetPosReg_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPosReg_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPosReg_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPosReg_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["index"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPosReg_RequestWrite(value,w){
w.v.setUint16(w.at,value["index"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvGetPosReg_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPosReg_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["index"];
out["index"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetPosReg_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetPosReg_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e540225b0d532c460905582ad99ee175dd771e971c7e75f5dfb9325d39ed6b27",maxWireBytes:601,minWireBytes:89,cartesianMaxWireBytes:601n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"representation":"","utool":0,"uframe":0,"front":0,"up":0,"left":0,"flip":0,"turn4":0,"turn5":0,"turn6":0,"x":0,"y":0,"z":0,"w":0,"p":0,"r":0,"ext1":0,"ext2":0,"ext3":0,"j1":0,"j2":0,"j3":0,"j4":0,"j5":0,"j6":0,"j7":0,"j8":0,"j9":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetPosReg_ResponseSize(Ros2Fanuc_msgsSrvGetPosReg_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetPosReg_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetPosReg_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetPosReg_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,601,90);if(owned.length<89)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetPosReg_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetPosReg_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["representation"],512).length);
integer(value["utool"],1,false);n=addSize(n,1);
integer(value["uframe"],1,false);n=addSize(n,1);
integer(value["front"],1,false);n=addSize(n,1);
integer(value["up"],1,false);n=addSize(n,1);
integer(value["left"],1,false);n=addSize(n,1);
integer(value["flip"],1,false);n=addSize(n,1);
integer(value["turn4"],1,true);n=addSize(n,1);
integer(value["turn5"],1,true);n=addSize(n,1);
integer(value["turn6"],1,true);n=addSize(n,1);
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"])||!Number.isFinite(Math.fround(value["w"])))bad();n=addSize(n,4);
if(typeof value["p"]!=="number"||!Number.isFinite(value["p"])||!Number.isFinite(Math.fround(value["p"])))bad();n=addSize(n,4);
if(typeof value["r"]!=="number"||!Number.isFinite(value["r"])||!Number.isFinite(Math.fround(value["r"])))bad();n=addSize(n,4);
if(typeof value["ext1"]!=="number"||!Number.isFinite(value["ext1"])||!Number.isFinite(Math.fround(value["ext1"])))bad();n=addSize(n,4);
if(typeof value["ext2"]!=="number"||!Number.isFinite(value["ext2"])||!Number.isFinite(Math.fround(value["ext2"])))bad();n=addSize(n,4);
if(typeof value["ext3"]!=="number"||!Number.isFinite(value["ext3"])||!Number.isFinite(Math.fround(value["ext3"])))bad();n=addSize(n,4);
if(typeof value["j1"]!=="number"||!Number.isFinite(value["j1"])||!Number.isFinite(Math.fround(value["j1"])))bad();n=addSize(n,4);
if(typeof value["j2"]!=="number"||!Number.isFinite(value["j2"])||!Number.isFinite(Math.fround(value["j2"])))bad();n=addSize(n,4);
if(typeof value["j3"]!=="number"||!Number.isFinite(value["j3"])||!Number.isFinite(Math.fround(value["j3"])))bad();n=addSize(n,4);
if(typeof value["j4"]!=="number"||!Number.isFinite(value["j4"])||!Number.isFinite(Math.fround(value["j4"])))bad();n=addSize(n,4);
if(typeof value["j5"]!=="number"||!Number.isFinite(value["j5"])||!Number.isFinite(Math.fround(value["j5"])))bad();n=addSize(n,4);
if(typeof value["j6"]!=="number"||!Number.isFinite(value["j6"])||!Number.isFinite(Math.fround(value["j6"])))bad();n=addSize(n,4);
if(typeof value["j7"]!=="number"||!Number.isFinite(value["j7"])||!Number.isFinite(Math.fround(value["j7"])))bad();n=addSize(n,4);
if(typeof value["j8"]!=="number"||!Number.isFinite(value["j8"])||!Number.isFinite(Math.fround(value["j8"])))bad();n=addSize(n,4);
if(typeof value["j9"]!=="number"||!Number.isFinite(value["j9"])||!Number.isFinite(Math.fround(value["j9"])))bad();n=addSize(n,4);
if(n>601)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetPosReg_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.text(value["representation"],512);
w.v.setUint8(w.at,value["utool"],true);w.at+=1;
w.v.setUint8(w.at,value["uframe"],true);w.at+=1;
w.v.setUint8(w.at,value["front"],true);w.at+=1;
w.v.setUint8(w.at,value["up"],true);w.at+=1;
w.v.setUint8(w.at,value["left"],true);w.at+=1;
w.v.setUint8(w.at,value["flip"],true);w.at+=1;
w.v.setInt8(w.at,value["turn4"],true);w.at+=1;
w.v.setInt8(w.at,value["turn5"],true);w.at+=1;
w.v.setInt8(w.at,value["turn6"],true);w.at+=1;
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["w"],true);w.at+=4;
w.v.setFloat32(w.at,value["p"],true);w.at+=4;
w.v.setFloat32(w.at,value["r"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext1"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext2"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext3"],true);w.at+=4;
w.v.setFloat32(w.at,value["j1"],true);w.at+=4;
w.v.setFloat32(w.at,value["j2"],true);w.at+=4;
w.v.setFloat32(w.at,value["j3"],true);w.at+=4;
w.v.setFloat32(w.at,value["j4"],true);w.at+=4;
w.v.setFloat32(w.at,value["j5"],true);w.at+=4;
w.v.setFloat32(w.at,value["j6"],true);w.at+=4;
w.v.setFloat32(w.at,value["j7"],true);w.at+=4;
w.v.setFloat32(w.at,value["j8"],true);w.at+=4;
w.v.setFloat32(w.at,value["j9"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvGetPosReg_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.text(512);
value["representation"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["up"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["left"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["flip"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn5"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn6"]=decoded;
}
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
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["p"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["r"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j6"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j9"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetPosReg_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["representation"];
out["representation"]=field;
}
{const field=value["utool"];
out["utool"]=field;
}
{const field=value["uframe"];
out["uframe"]=field;
}
{const field=value["front"];
out["front"]=field;
}
{const field=value["up"];
out["up"]=field;
}
{const field=value["left"];
out["left"]=field;
}
{const field=value["flip"];
out["flip"]=field;
}
{const field=value["turn4"];
out["turn4"]=field;
}
{const field=value["turn5"];
out["turn5"]=field;
}
{const field=value["turn6"];
out["turn6"]=field;
}
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
{const field=value["p"];
out["p"]=field;
}
{const field=value["r"];
out["r"]=field;
}
{const field=value["ext1"];
out["ext1"]=field;
}
{const field=value["ext2"];
out["ext2"]=field;
}
{const field=value["ext3"];
out["ext3"]=field;
}
{const field=value["j1"];
out["j1"]=field;
}
{const field=value["j2"];
out["j2"]=field;
}
{const field=value["j3"];
out["j3"]=field;
}
{const field=value["j4"];
out["j4"]=field;
}
{const field=value["j5"];
out["j5"]=field;
}
{const field=value["j6"];
out["j6"]=field;
}
{const field=value["j7"];
out["j7"]=field;
}
{const field=value["j8"];
out["j8"]=field;
}
{const field=value["j9"];
out["j9"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetTCPSpeed_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6bdfd44465b75bbd4e232f802dbd3c0bfa28e90a93f9a28b3c15fb150bce7ac7",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetTCPSpeed_RequestSize(Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetTCPSpeed_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetTCPSpeed_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetTCPSpeed_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetTCPSpeed_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["group"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetTCPSpeed_RequestWrite(value,w){
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetTCPSpeed_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetTCPSpeed_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3ec07d99342f275506452f32076f9e1ea01bf32f43662311708a9b44ca951147",maxWireBytes:13,minWireBytes:13,cartesianMaxWireBytes:13n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"timetag":0,"speed":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseSize(Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,13,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["timetag"],4,true);n=addSize(n,4);
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"])||!Number.isFinite(Math.fround(value["speed"])))bad();n=addSize(n,4);
integer(value["group"],1,false);n=addSize(n,1);
if(n>13)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["timetag"],true);w.at+=4;
w.v.setFloat32(w.at,value["speed"],true);w.at+=4;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["timetag"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["speed"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["timetag"];
out["timetag"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameData_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUFrameData_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c1ef7077fd7f61ed8ff46931c8e39c5950658cfe1d36cffac10083ab304307be",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uframe_number":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUFrameData_RequestSize(Ros2Fanuc_msgsSrvGetUFrameData_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUFrameData_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetUFrameData_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUFrameData_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUFrameData_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUFrameData_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["uframe_number"],1,false);n=addSize(n,1);
integer(value["group"],1,false);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUFrameData_RequestWrite(value,w){
w.v.setUint8(w.at,value["uframe_number"],true);w.at+=1;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUFrameData_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUFrameData_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uframe_number"];
out["uframe_number"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameData_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUFrameData_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f956e1447bd4a15f547fdf5d6775836018d50153270fe8863c4b30d83422fd2e",maxWireBytes:30,minWireBytes:30,cartesianMaxWireBytes:30n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"uframe_number":0,"frame":Ros2Fanuc_msgsMsgFrameCodec.defaultValue(),"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUFrameData_ResponseSize(Ros2Fanuc_msgsSrvGetUFrameData_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUFrameData_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetUFrameData_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUFrameData_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,30,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUFrameData_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUFrameData_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["uframe_number"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Fanuc_msgsMsgFrameSize(value["frame"]));
integer(value["group"],1,false);n=addSize(n,1);
if(n>30)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUFrameData_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["uframe_number"],true);w.at+=1;
Ros2Fanuc_msgsMsgFrameWrite(value["frame"],w);
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUFrameData_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe_number"]=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrameRead(r);
value["frame"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUFrameData_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["uframe_number"];
out["uframe_number"]=field;
}
{const field=value["frame"];
out["frame"]=Ros2Fanuc_msgsMsgFrameCapture(field);
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameUTool_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3d57da6f8003b33b2a079d4f284eb0f182d7e088074112b7f4ee25d3113bd443",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUFrameUTool_RequestSize(Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetUFrameUTool_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUFrameUTool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUFrameUTool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUFrameUTool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["group"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUFrameUTool_RequestWrite(value,w){
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUFrameUTool_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameUTool_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f9cb64b89f17b6811e4073b9f94d0fd6844d21ec4b846f78bfd13cb14a35a6b3",maxWireBytes:7,minWireBytes:7,cartesianMaxWireBytes:7n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"uframe_number":0,"utool_number":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseSize(Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,7,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["uframe_number"],1,false);n=addSize(n,1);
integer(value["utool_number"],1,false);n=addSize(n,1);
integer(value["group"],1,false);n=addSize(n,1);
if(n>7)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["uframe_number"],true);w.at+=1;
w.v.setUint8(w.at,value["utool_number"],true);w.at+=1;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["uframe_number"];
out["uframe_number"]=field;
}
{const field=value["utool_number"];
out["utool_number"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUToolData_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUToolData_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_10c3799dcb1b09dbcb6d1b794a00ecfaf69a8f457d95eeb92855f79dba7be1bc",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"utool_number":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUToolData_RequestSize(Ros2Fanuc_msgsSrvGetUToolData_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUToolData_RequestCapture(value),n=Ros2Fanuc_msgsSrvGetUToolData_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUToolData_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUToolData_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUToolData_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["utool_number"],1,false);n=addSize(n,1);
integer(value["group"],1,false);n=addSize(n,1);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUToolData_RequestWrite(value,w){
w.v.setUint8(w.at,value["utool_number"],true);w.at+=1;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUToolData_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUToolData_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["utool_number"];
out["utool_number"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/GetUToolData_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvGetUToolData_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f3a2b94f337cec9c58156b803675f38885350cc3fc3171234c0d361afc800d81",maxWireBytes:30,minWireBytes:30,cartesianMaxWireBytes:30n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"utool_number":0,"frame":Ros2Fanuc_msgsMsgFrameCodec.defaultValue(),"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvGetUToolData_ResponseSize(Ros2Fanuc_msgsSrvGetUToolData_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvGetUToolData_ResponseCapture(value),n=Ros2Fanuc_msgsSrvGetUToolData_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvGetUToolData_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,30,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvGetUToolData_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvGetUToolData_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["utool_number"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Fanuc_msgsMsgFrameSize(value["frame"]));
integer(value["group"],1,false);n=addSize(n,1);
if(n>30)bad(90);return n;}
function Ros2Fanuc_msgsSrvGetUToolData_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["utool_number"],true);w.at+=1;
Ros2Fanuc_msgsMsgFrameWrite(value["frame"],w);
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvGetUToolData_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool_number"]=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrameRead(r);
value["frame"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvGetUToolData_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["utool_number"];
out["utool_number"]=field;
}
{const field=value["frame"];
out["frame"]=Ros2Fanuc_msgsMsgFrameCapture(field);
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/ReadError_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvReadError_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4b67ed8e2ecccefb6bdead80249f5916e6ea192d822183292db43351839fb6f9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"count":1};},
validateValue(value){Ros2Fanuc_msgsSrvReadError_RequestSize(Ros2Fanuc_msgsSrvReadError_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvReadError_RequestCapture(value),n=Ros2Fanuc_msgsSrvReadError_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvReadError_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvReadError_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvReadError_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["count"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsSrvReadError_RequestWrite(value,w){
w.v.setUint8(w.at,value["count"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvReadError_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["count"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvReadError_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["count"];
out["count"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/ReadError_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvReadError_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6e28db10c7afb06cb1256f6d74a43e26c5efa6c13fec8826ae0e623a5eb954b6",maxWireBytes:2585,minWireBytes:25,cartesianMaxWireBytes:2585n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"count":0,"error_data":Array.from({length:5},()=>(""))};},
validateValue(value){Ros2Fanuc_msgsSrvReadError_ResponseSize(Ros2Fanuc_msgsSrvReadError_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvReadError_ResponseCapture(value),n=Ros2Fanuc_msgsSrvReadError_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvReadError_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2585,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvReadError_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvReadError_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["count"],1,false);n=addSize(n,1);
if(!Array.isArray(value["error_data"])||value["error_data"].length!==5)bad();
for(const element of value["error_data"]){
n=addSize(n,4+textBytes(element,512).length);
}
if(n>2585)bad(90);return n;}
function Ros2Fanuc_msgsSrvReadError_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["count"],true);w.at+=1;
for(const element of value["error_data"]){
w.text(element,512);
}
}
function Ros2Fanuc_msgsSrvReadError_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["count"]=decoded;
}
{
{const count=5;if(count>5)bad();value["error_data"]=[];for(let i=0;i<count;++i){
const decoded=r.text(512);
value["error_data"].push(decoded);
}}
}
return value;}
function Ros2Fanuc_msgsSrvReadError_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["count"];
out["count"]=field;
}
{const field=value["error_data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==5)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["error_data"]=values;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/Reset_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvReset_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b73f661d569f6af8dad1cd6625f4c0c55ef2c14c79b3e9af0f17fdc02736abc4",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Fanuc_msgsSrvReset_RequestSize(Ros2Fanuc_msgsSrvReset_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvReset_RequestCapture(value),n=Ros2Fanuc_msgsSrvReset_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvReset_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvReset_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvReset_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Fanuc_msgsSrvReset_RequestWrite(value,w){
}
function Ros2Fanuc_msgsSrvReset_RequestRead(r){const value={};
return value;}
function Ros2Fanuc_msgsSrvReset_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 fanuc_msgs/srv/Reset_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvReset_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5e9b92e5a112b4934ce0893afaa09156fdb47c26b0aa6ae9a27d61920d308ab8",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvReset_ResponseSize(Ros2Fanuc_msgsSrvReset_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvReset_ResponseCapture(value),n=Ros2Fanuc_msgsSrvReset_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvReset_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvReset_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvReset_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvReset_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvReset_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvReset_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetAnalogIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetAnalogIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a6bb4acfae23db79464faff849f359657537026d7c79f8a6c60259caec5e87f4",maxWireBytes:74,minWireBytes:10,cartesianMaxWireBytes:74n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetAnalogIO_RequestSize(Ros2Fanuc_msgsSrvSetAnalogIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetAnalogIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetAnalogIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetAnalogIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,74,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetAnalogIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetAnalogIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>74)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetAnalogIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetAnalogIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetAnalogIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetAnalogIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_aeac383e2c4a292f1237494bff5b261cd0197542e00456b4ee0edf9b7fe8a39b",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetAnalogIO_ResponseSize(Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetAnalogIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetAnalogIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetAnalogIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetAnalogIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetAnalogIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetAnalogIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetBoolIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetBoolIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0e8ace58c769670a2519e733d020a3ca902bff99982514d5cd9ecda391c1a4cf",maxWireBytes:71,minWireBytes:7,cartesianMaxWireBytes:71n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0,"value":false};},
validateValue(value){Ros2Fanuc_msgsSrvSetBoolIO_RequestSize(Ros2Fanuc_msgsSrvSetBoolIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetBoolIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetBoolIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetBoolIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,71,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetBoolIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetBoolIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
if(n>71)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetBoolIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetBoolIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
return value;}
function Ros2Fanuc_msgsSrvSetBoolIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetBoolIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetBoolIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c9c68ca577dd6e4d1743385d9c69c8f1c6e0780ae247877d823ab97a68e675a0",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetBoolIO_ResponseSize(Ros2Fanuc_msgsSrvSetBoolIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetBoolIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetBoolIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetBoolIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetBoolIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetBoolIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetBoolIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetBoolIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetBoolIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetGenOverride_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetGenOverride_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7d321e33aaf6f58013c33ffc012c1ddbd27aefff3f21d7456030a1d590362039",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetGenOverride_RequestSize(Ros2Fanuc_msgsSrvSetGenOverride_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetGenOverride_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetGenOverride_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetGenOverride_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetGenOverride_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetGenOverride_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetGenOverride_RequestWrite(value,w){
w.v.setInt8(w.at,value["value"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetGenOverride_RequestRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetGenOverride_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetGenOverride_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetGenOverride_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1ffcb25cad7dbf51c216753dac2c117a191fb081a5a562e63d34d447e61f0a59",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetGenOverride_ResponseSize(Ros2Fanuc_msgsSrvSetGenOverride_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetGenOverride_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetGenOverride_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetGenOverride_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetGenOverride_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetGenOverride_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetGenOverride_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetGenOverride_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetGenOverride_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetGroupIO_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetGroupIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_42a76aec283e26e3a2c99bb27a23778e34effb6c3328754793f783038a2b84bd",maxWireBytes:72,minWireBytes:8,cartesianMaxWireBytes:72n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"io_type":Ros2Fanuc_msgsMsgIOTypeCodec.defaultValue(),"index":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetGroupIO_RequestSize(Ros2Fanuc_msgsSrvSetGroupIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetGroupIO_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetGroupIO_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetGroupIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,72,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetGroupIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetGroupIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Fanuc_msgsMsgIOTypeSize(value["io_type"]));
integer(value["index"],2,false);n=addSize(n,2);
integer(value["value"],2,false);n=addSize(n,2);
if(n>72)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetGroupIO_RequestWrite(value,w){
Ros2Fanuc_msgsMsgIOTypeWrite(value["io_type"],w);
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvSetGroupIO_RequestRead(r){const value={};
{
const decoded=Ros2Fanuc_msgsMsgIOTypeRead(r);
value["io_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetGroupIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["io_type"];
out["io_type"]=Ros2Fanuc_msgsMsgIOTypeCapture(field);
}
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetGroupIO_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetGroupIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_aa6d2c99e8817273f8f2ab179df61d60efd8ee2cee0e1ec76c647956e22bff17",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetGroupIO_ResponseSize(Ros2Fanuc_msgsSrvSetGroupIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetGroupIO_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetGroupIO_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetGroupIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetGroupIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetGroupIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetGroupIO_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetGroupIO_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetGroupIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetNumReg_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetNumReg_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7e362b671b8770db33f3876308c41a930863a0805771fa05fcbb170782207e36",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"index":0,"value":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetNumReg_RequestSize(Ros2Fanuc_msgsSrvSetNumReg_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetNumReg_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetNumReg_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetNumReg_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetNumReg_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetNumReg_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["index"],2,false);n=addSize(n,2);
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>6)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetNumReg_RequestWrite(value,w){
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetNumReg_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetNumReg_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["index"];
out["index"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetNumReg_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetNumReg_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7c7850ad8bb5c861d9b436311a8cf985e2e029d362877be80c5a6cb19021e482",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetNumReg_ResponseSize(Ros2Fanuc_msgsSrvSetNumReg_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetNumReg_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetNumReg_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetNumReg_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetNumReg_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetNumReg_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetNumReg_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetNumReg_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetNumReg_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadComp_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadComp_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_92c495d71657249e15c172e850aa91869929a25817437567778df1b01d9037a8",maxWireBytes:30,minWireBytes:30,cartesianMaxWireBytes:30n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0,"mass":0,"cg_x":0,"cg_y":0,"cg_z":0,"in_x":0,"in_y":0,"in_z":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadComp_RequestSize(Ros2Fanuc_msgsSrvSetPayloadComp_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadComp_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadComp_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadComp_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,30,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadComp_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadComp_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
if(typeof value["cg_x"]!=="number"||!Number.isFinite(value["cg_x"])||!Number.isFinite(Math.fround(value["cg_x"])))bad();n=addSize(n,4);
if(typeof value["cg_y"]!=="number"||!Number.isFinite(value["cg_y"])||!Number.isFinite(Math.fround(value["cg_y"])))bad();n=addSize(n,4);
if(typeof value["cg_z"]!=="number"||!Number.isFinite(value["cg_z"])||!Number.isFinite(Math.fround(value["cg_z"])))bad();n=addSize(n,4);
if(typeof value["in_x"]!=="number"||!Number.isFinite(value["in_x"])||!Number.isFinite(Math.fround(value["in_x"])))bad();n=addSize(n,4);
if(typeof value["in_y"]!=="number"||!Number.isFinite(value["in_y"])||!Number.isFinite(Math.fround(value["in_y"])))bad();n=addSize(n,4);
if(typeof value["in_z"]!=="number"||!Number.isFinite(value["in_z"])||!Number.isFinite(Math.fround(value["in_z"])))bad();n=addSize(n,4);
if(n>30)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadComp_RequestWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_z"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_z"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPayloadComp_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_z"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_z"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadComp_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cg_x"];
out["cg_x"]=field;
}
{const field=value["cg_y"];
out["cg_y"]=field;
}
{const field=value["cg_z"];
out["cg_z"]=field;
}
{const field=value["in_x"];
out["in_x"]=field;
}
{const field=value["in_y"];
out["in_y"]=field;
}
{const field=value["in_z"];
out["in_z"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadComp_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_37a9876e2a9c514fa8c4337ff1351f778d350318bdc6d8aa5d569b4039a2f76b",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadComp_ResponseSize(Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadComp_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadComp_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadComp_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadComp_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadComp_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPayloadComp_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadID_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadID_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_aa15b59db17d2afb7ef9e74676a9b744763954ae063efafb91a3ab3c9852a1c0",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadID_RequestSize(Ros2Fanuc_msgsSrvSetPayloadID_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadID_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadID_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadID_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadID_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadID_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadID_RequestWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
}
function Ros2Fanuc_msgsSrvSetPayloadID_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadID_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadID_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadID_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7b704b36da10dc71b4cfaad92ea02ed22e4966077dc0940b03b8b8e49c5769c7",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadID_ResponseSize(Ros2Fanuc_msgsSrvSetPayloadID_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadID_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadID_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadID_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadID_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadID_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadID_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPayloadID_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadID_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadValue_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadValue_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5d56ce8f2f8df9c3450d6a5dc77e9e77861471805aa6b9c6f8815c2e7c324974",maxWireBytes:31,minWireBytes:31,cartesianMaxWireBytes:31n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"payload_schedule_id":0,"mass":0,"cg_x":0,"cg_y":0,"cg_z":0,"use_in":false,"in_x":0,"in_y":0,"in_z":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadValue_RequestSize(Ros2Fanuc_msgsSrvSetPayloadValue_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadValue_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadValue_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadValue_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,31,90);if(owned.length<31)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadValue_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadValue_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["payload_schedule_id"],2,false);n=addSize(n,2);
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
if(typeof value["cg_x"]!=="number"||!Number.isFinite(value["cg_x"])||!Number.isFinite(Math.fround(value["cg_x"])))bad();n=addSize(n,4);
if(typeof value["cg_y"]!=="number"||!Number.isFinite(value["cg_y"])||!Number.isFinite(Math.fround(value["cg_y"])))bad();n=addSize(n,4);
if(typeof value["cg_z"]!=="number"||!Number.isFinite(value["cg_z"])||!Number.isFinite(Math.fround(value["cg_z"])))bad();n=addSize(n,4);
if(typeof value["use_in"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["in_x"]!=="number"||!Number.isFinite(value["in_x"])||!Number.isFinite(Math.fround(value["in_x"])))bad();n=addSize(n,4);
if(typeof value["in_y"]!=="number"||!Number.isFinite(value["in_y"])||!Number.isFinite(Math.fround(value["in_y"])))bad();n=addSize(n,4);
if(typeof value["in_z"]!=="number"||!Number.isFinite(value["in_z"])||!Number.isFinite(Math.fround(value["in_z"])))bad();n=addSize(n,4);
if(n>31)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadValue_RequestWrite(value,w){
w.v.setUint16(w.at,value["payload_schedule_id"],true);w.at+=2;
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["cg_z"],true);w.at+=4;
w.v.setUint8(w.at,value["use_in"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["in_x"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_y"],true);w.at+=4;
w.v.setFloat32(w.at,value["in_z"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPayloadValue_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["payload_schedule_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cg_z"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_in"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_x"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_y"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["in_z"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadValue_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["payload_schedule_id"];
out["payload_schedule_id"]=field;
}
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cg_x"];
out["cg_x"]=field;
}
{const field=value["cg_y"];
out["cg_y"]=field;
}
{const field=value["cg_z"];
out["cg_z"]=field;
}
{const field=value["use_in"];
out["use_in"]=field;
}
{const field=value["in_x"];
out["in_x"]=field;
}
{const field=value["in_y"];
out["in_y"]=field;
}
{const field=value["in_z"];
out["in_z"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadValue_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_09f30071b49ac57353429e8a2862ee80d3dcab9237e8192d008c12a3628cc6ab",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPayloadValue_ResponseSize(Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetPayloadValue_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPayloadValue_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPayloadValue_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPayloadValue_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPayloadValue_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPayloadValue_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPosReg_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPosReg_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f9af742e55a044a1a363a45cbe4a0a8ce4ca62fcaf43759041cef89149d96d30",maxWireBytes:599,minWireBytes:87,cartesianMaxWireBytes:599n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"index":0,"representation":"","utool":0,"uframe":0,"front":0,"up":0,"left":0,"flip":0,"turn4":0,"turn5":0,"turn6":0,"x":0,"y":0,"z":0,"w":0,"p":0,"r":0,"ext1":0,"ext2":0,"ext3":0,"j1":0,"j2":0,"j3":0,"j4":0,"j5":0,"j6":0,"j7":0,"j8":0,"j9":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPosReg_RequestSize(Ros2Fanuc_msgsSrvSetPosReg_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPosReg_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetPosReg_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPosReg_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,599,90);if(owned.length<87)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPosReg_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPosReg_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["index"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["representation"],512).length);
integer(value["utool"],1,false);n=addSize(n,1);
integer(value["uframe"],1,false);n=addSize(n,1);
integer(value["front"],1,false);n=addSize(n,1);
integer(value["up"],1,false);n=addSize(n,1);
integer(value["left"],1,false);n=addSize(n,1);
integer(value["flip"],1,false);n=addSize(n,1);
integer(value["turn4"],1,true);n=addSize(n,1);
integer(value["turn5"],1,true);n=addSize(n,1);
integer(value["turn6"],1,true);n=addSize(n,1);
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"])||!Number.isFinite(Math.fround(value["w"])))bad();n=addSize(n,4);
if(typeof value["p"]!=="number"||!Number.isFinite(value["p"])||!Number.isFinite(Math.fround(value["p"])))bad();n=addSize(n,4);
if(typeof value["r"]!=="number"||!Number.isFinite(value["r"])||!Number.isFinite(Math.fround(value["r"])))bad();n=addSize(n,4);
if(typeof value["ext1"]!=="number"||!Number.isFinite(value["ext1"])||!Number.isFinite(Math.fround(value["ext1"])))bad();n=addSize(n,4);
if(typeof value["ext2"]!=="number"||!Number.isFinite(value["ext2"])||!Number.isFinite(Math.fround(value["ext2"])))bad();n=addSize(n,4);
if(typeof value["ext3"]!=="number"||!Number.isFinite(value["ext3"])||!Number.isFinite(Math.fround(value["ext3"])))bad();n=addSize(n,4);
if(typeof value["j1"]!=="number"||!Number.isFinite(value["j1"])||!Number.isFinite(Math.fround(value["j1"])))bad();n=addSize(n,4);
if(typeof value["j2"]!=="number"||!Number.isFinite(value["j2"])||!Number.isFinite(Math.fround(value["j2"])))bad();n=addSize(n,4);
if(typeof value["j3"]!=="number"||!Number.isFinite(value["j3"])||!Number.isFinite(Math.fround(value["j3"])))bad();n=addSize(n,4);
if(typeof value["j4"]!=="number"||!Number.isFinite(value["j4"])||!Number.isFinite(Math.fround(value["j4"])))bad();n=addSize(n,4);
if(typeof value["j5"]!=="number"||!Number.isFinite(value["j5"])||!Number.isFinite(Math.fround(value["j5"])))bad();n=addSize(n,4);
if(typeof value["j6"]!=="number"||!Number.isFinite(value["j6"])||!Number.isFinite(Math.fround(value["j6"])))bad();n=addSize(n,4);
if(typeof value["j7"]!=="number"||!Number.isFinite(value["j7"])||!Number.isFinite(Math.fround(value["j7"])))bad();n=addSize(n,4);
if(typeof value["j8"]!=="number"||!Number.isFinite(value["j8"])||!Number.isFinite(Math.fround(value["j8"])))bad();n=addSize(n,4);
if(typeof value["j9"]!=="number"||!Number.isFinite(value["j9"])||!Number.isFinite(Math.fround(value["j9"])))bad();n=addSize(n,4);
if(n>599)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPosReg_RequestWrite(value,w){
w.v.setUint16(w.at,value["index"],true);w.at+=2;
w.text(value["representation"],512);
w.v.setUint8(w.at,value["utool"],true);w.at+=1;
w.v.setUint8(w.at,value["uframe"],true);w.at+=1;
w.v.setUint8(w.at,value["front"],true);w.at+=1;
w.v.setUint8(w.at,value["up"],true);w.at+=1;
w.v.setUint8(w.at,value["left"],true);w.at+=1;
w.v.setUint8(w.at,value["flip"],true);w.at+=1;
w.v.setInt8(w.at,value["turn4"],true);w.at+=1;
w.v.setInt8(w.at,value["turn5"],true);w.at+=1;
w.v.setInt8(w.at,value["turn6"],true);w.at+=1;
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["w"],true);w.at+=4;
w.v.setFloat32(w.at,value["p"],true);w.at+=4;
w.v.setFloat32(w.at,value["r"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext1"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext2"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext3"],true);w.at+=4;
w.v.setFloat32(w.at,value["j1"],true);w.at+=4;
w.v.setFloat32(w.at,value["j2"],true);w.at+=4;
w.v.setFloat32(w.at,value["j3"],true);w.at+=4;
w.v.setFloat32(w.at,value["j4"],true);w.at+=4;
w.v.setFloat32(w.at,value["j5"],true);w.at+=4;
w.v.setFloat32(w.at,value["j6"],true);w.at+=4;
w.v.setFloat32(w.at,value["j7"],true);w.at+=4;
w.v.setFloat32(w.at,value["j8"],true);w.at+=4;
w.v.setFloat32(w.at,value["j9"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPosReg_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["index"]=decoded;
}
{
const decoded=r.text(512);
value["representation"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["up"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["left"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["flip"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn5"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn6"]=decoded;
}
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
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["p"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["r"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j6"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["j9"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPosReg_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["index"];
out["index"]=field;
}
{const field=value["representation"];
out["representation"]=field;
}
{const field=value["utool"];
out["utool"]=field;
}
{const field=value["uframe"];
out["uframe"]=field;
}
{const field=value["front"];
out["front"]=field;
}
{const field=value["up"];
out["up"]=field;
}
{const field=value["left"];
out["left"]=field;
}
{const field=value["flip"];
out["flip"]=field;
}
{const field=value["turn4"];
out["turn4"]=field;
}
{const field=value["turn5"];
out["turn5"]=field;
}
{const field=value["turn6"];
out["turn6"]=field;
}
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
{const field=value["p"];
out["p"]=field;
}
{const field=value["r"];
out["r"]=field;
}
{const field=value["ext1"];
out["ext1"]=field;
}
{const field=value["ext2"];
out["ext2"]=field;
}
{const field=value["ext3"];
out["ext3"]=field;
}
{const field=value["j1"];
out["j1"]=field;
}
{const field=value["j2"];
out["j2"]=field;
}
{const field=value["j3"];
out["j3"]=field;
}
{const field=value["j4"];
out["j4"]=field;
}
{const field=value["j5"];
out["j5"]=field;
}
{const field=value["j6"];
out["j6"]=field;
}
{const field=value["j7"];
out["j7"]=field;
}
{const field=value["j8"];
out["j8"]=field;
}
{const field=value["j9"];
out["j9"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetPosReg_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetPosReg_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_fb6f889d30c921df31f55cdc760346eb0d655816e71b52d4aac7a1e56d4e9cb2",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetPosReg_ResponseSize(Ros2Fanuc_msgsSrvSetPosReg_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetPosReg_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetPosReg_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetPosReg_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetPosReg_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetPosReg_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetPosReg_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSetPosReg_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetPosReg_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameData_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUFrameData_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a067808d8fe5ca1418e18ac492313ad9211cc8f3bf402ad0ac51a2850a2bc169",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uframe_number":0,"frame":Ros2Fanuc_msgsMsgFrameCodec.defaultValue(),"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUFrameData_RequestSize(Ros2Fanuc_msgsSrvSetUFrameData_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUFrameData_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetUFrameData_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUFrameData_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUFrameData_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUFrameData_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["uframe_number"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Fanuc_msgsMsgFrameSize(value["frame"]));
integer(value["group"],1,false);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUFrameData_RequestWrite(value,w){
w.v.setUint8(w.at,value["uframe_number"],true);w.at+=1;
Ros2Fanuc_msgsMsgFrameWrite(value["frame"],w);
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUFrameData_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe_number"]=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrameRead(r);
value["frame"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUFrameData_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uframe_number"];
out["uframe_number"]=field;
}
{const field=value["frame"];
out["frame"]=Ros2Fanuc_msgsMsgFrameCapture(field);
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameData_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUFrameData_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f436e4fe5bd06b048f4f53d2df4933ac8eac5a784f31b42ccf13f5eaf1696eee",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUFrameData_ResponseSize(Ros2Fanuc_msgsSrvSetUFrameData_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUFrameData_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetUFrameData_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUFrameData_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUFrameData_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUFrameData_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["group"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUFrameData_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUFrameData_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUFrameData_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameUTool_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0b86b0a37e4dc23e0ff651df867f5958e7440bf4712b47779d69ca699af4c09f",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uframe_number":0,"utool_number":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUFrameUTool_RequestSize(Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetUFrameUTool_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUFrameUTool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUFrameUTool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUFrameUTool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["uframe_number"],1,false);n=addSize(n,1);
integer(value["utool_number"],1,false);n=addSize(n,1);
integer(value["group"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUFrameUTool_RequestWrite(value,w){
w.v.setUint8(w.at,value["uframe_number"],true);w.at+=1;
w.v.setUint8(w.at,value["utool_number"],true);w.at+=1;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUFrameUTool_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["uframe_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uframe_number"];
out["uframe_number"]=field;
}
{const field=value["utool_number"];
out["utool_number"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameUTool_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b77837445c9663f29f23465cc7178a183d245486f74bcff732fc2f4025b82a67",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseSize(Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["group"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUToolData_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUToolData_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7d5b911133645744c8def79bc16c8b7c4ee4e87a057ea33741031d6a94e620ab",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"utool_number":0,"frame":Ros2Fanuc_msgsMsgFrameCodec.defaultValue(),"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUToolData_RequestSize(Ros2Fanuc_msgsSrvSetUToolData_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUToolData_RequestCapture(value),n=Ros2Fanuc_msgsSrvSetUToolData_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUToolData_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUToolData_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUToolData_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["utool_number"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Fanuc_msgsMsgFrameSize(value["frame"]));
integer(value["group"],1,false);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUToolData_RequestWrite(value,w){
w.v.setUint8(w.at,value["utool_number"],true);w.at+=1;
Ros2Fanuc_msgsMsgFrameWrite(value["frame"],w);
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUToolData_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["utool_number"]=decoded;
}
{
const decoded=Ros2Fanuc_msgsMsgFrameRead(r);
value["frame"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUToolData_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["utool_number"];
out["utool_number"]=field;
}
{const field=value["frame"];
out["frame"]=Ros2Fanuc_msgsMsgFrameCapture(field);
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SetUToolData_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSetUToolData_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b33d265f275dfcb5fe20432d5e38b55574c85260135bded477c2d5fa2c85c7cb",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"group":0};},
validateValue(value){Ros2Fanuc_msgsSrvSetUToolData_ResponseSize(Ros2Fanuc_msgsSrvSetUToolData_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSetUToolData_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSetUToolData_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSetUToolData_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSetUToolData_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSetUToolData_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["group"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Fanuc_msgsSrvSetUToolData_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setUint8(w.at,value["group"],true);w.at+=1;
}
function Ros2Fanuc_msgsSrvSetUToolData_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSetUToolData_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["group"];
out["group"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SwitchControlState_Request codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSwitchControlState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b485b7071fe2737c7dc8f0360a15b7be30f997430142d8260454fd423172cc21",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({"STOP":0,"START":1}),
defaultValue(){return {"status":0};},
validateValue(value){Ros2Fanuc_msgsSrvSwitchControlState_RequestSize(Ros2Fanuc_msgsSrvSwitchControlState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSwitchControlState_RequestCapture(value),n=Ros2Fanuc_msgsSrvSwitchControlState_RequestSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSwitchControlState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSwitchControlState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSwitchControlState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSwitchControlState_RequestWrite(value,w){
w.v.setUint32(w.at,value["status"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSwitchControlState_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["status"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSwitchControlState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 fanuc_msgs/srv/SwitchControlState_Response codec; maxima are not actual lengths. */
export const Ros2Fanuc_msgsSrvSwitchControlState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8a3d7a3c1037174f4118843e34222ef8811eff3cc885a0c688d98071e1a9496c",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Fanuc_msgsSrvSwitchControlState_ResponseSize(Ros2Fanuc_msgsSrvSwitchControlState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Fanuc_msgsSrvSwitchControlState_ResponseCapture(value),n=Ros2Fanuc_msgsSrvSwitchControlState_ResponseSize(owned),w=new Writer(n);Ros2Fanuc_msgsSrvSwitchControlState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Fanuc_msgsSrvSwitchControlState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Fanuc_msgsSrvSwitchControlState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Fanuc_msgsSrvSwitchControlState_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Fanuc_msgsSrvSwitchControlState_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Fanuc_msgsSrvSwitchControlState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/CallParam codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgCallParamCodec=Object.freeze({codecIdentity:"UWVMROS1_ed625d80db6c4314b8373f2d205344d6ab8d71be46182dab5a497d36564a2255",maxWireBytes:592,minWireBytes:16,cartesianMaxWireBytes:592n,aggregateWireBytes:0,constants:Object.freeze({"AR":"AR","PR":"PR","SR":"SR","R":"R","P":"P","INT":"INT","INTEGER":"INT","FLOAT":"FLOAT","STRING":"String","STR":"String"}),
defaultValue(){return {"type":"","int_value":0,"float_value":0,"string_value":""};},
validateValue(value){Ros2Rmi_msgsMsgCallParamSize(Ros2Rmi_msgsMsgCallParamCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgCallParamCapture(value),n=Ros2Rmi_msgsMsgCallParamSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgCallParamWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,592,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgCallParamRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgCallParamSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
integer(value["int_value"],4,true);n=addSize(n,4);
if(typeof value["float_value"]!=="number"||!Number.isFinite(value["float_value"])||!Number.isFinite(Math.fround(value["float_value"])))bad();n=addSize(n,4);
n=addSize(n,4+textBytes(value["string_value"],512).length);
if(n>592)bad(90);return n;}
function Ros2Rmi_msgsMsgCallParamWrite(value,w){
w.text(value["type"],64);
w.v.setInt32(w.at,value["int_value"],true);w.at+=4;
w.v.setFloat32(w.at,value["float_value"],true);w.at+=4;
w.text(value["string_value"],512);
}
function Ros2Rmi_msgsMsgCallParamRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["int_value"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["float_value"]=decoded;
}
{
const decoded=r.text(512);
value["string_value"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgCallParamCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
{const field=value["int_value"];
out["int_value"]=field;
}
{const field=value["float_value"];
out["float_value"]=field;
}
{const field=value["string_value"];
out["string_value"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/Cartesian codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgCartesianCodec=Object.freeze({codecIdentity:"UWVMROS1_2a0e1fff119007499a63d55c9722f666be1436a9212d9e43b556af347e8e4b40",maxWireBytes:45,minWireBytes:45,cartesianMaxWireBytes:45n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"utool":1,"uframe":0,"front":1,"up":1,"left":0,"flip":0,"turn4":0,"turn5":0,"turn6":0,"x":0,"y":0,"z":0,"w":0,"p":0,"r":0,"ext1":0,"ext2":0,"ext3":0};},
validateValue(value){Ros2Rmi_msgsMsgCartesianSize(Ros2Rmi_msgsMsgCartesianCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgCartesianCapture(value),n=Ros2Rmi_msgsMsgCartesianSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgCartesianWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,45,90);if(owned.length<45)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgCartesianRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgCartesianSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["utool"],1,true);n=addSize(n,1);
integer(value["uframe"],1,true);n=addSize(n,1);
integer(value["front"],1,false);n=addSize(n,1);
integer(value["up"],1,false);n=addSize(n,1);
integer(value["left"],1,false);n=addSize(n,1);
integer(value["flip"],1,false);n=addSize(n,1);
integer(value["turn4"],1,true);n=addSize(n,1);
integer(value["turn5"],1,true);n=addSize(n,1);
integer(value["turn6"],1,true);n=addSize(n,1);
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["w"]!=="number"||!Number.isFinite(value["w"])||!Number.isFinite(Math.fround(value["w"])))bad();n=addSize(n,4);
if(typeof value["p"]!=="number"||!Number.isFinite(value["p"])||!Number.isFinite(Math.fround(value["p"])))bad();n=addSize(n,4);
if(typeof value["r"]!=="number"||!Number.isFinite(value["r"])||!Number.isFinite(Math.fround(value["r"])))bad();n=addSize(n,4);
if(typeof value["ext1"]!=="number"||!Number.isFinite(value["ext1"])||!Number.isFinite(Math.fround(value["ext1"])))bad();n=addSize(n,4);
if(typeof value["ext2"]!=="number"||!Number.isFinite(value["ext2"])||!Number.isFinite(Math.fround(value["ext2"])))bad();n=addSize(n,4);
if(typeof value["ext3"]!=="number"||!Number.isFinite(value["ext3"])||!Number.isFinite(Math.fround(value["ext3"])))bad();n=addSize(n,4);
if(n>45)bad(90);return n;}
function Ros2Rmi_msgsMsgCartesianWrite(value,w){
w.v.setInt8(w.at,value["utool"],true);w.at+=1;
w.v.setInt8(w.at,value["uframe"],true);w.at+=1;
w.v.setUint8(w.at,value["front"],true);w.at+=1;
w.v.setUint8(w.at,value["up"],true);w.at+=1;
w.v.setUint8(w.at,value["left"],true);w.at+=1;
w.v.setUint8(w.at,value["flip"],true);w.at+=1;
w.v.setInt8(w.at,value["turn4"],true);w.at+=1;
w.v.setInt8(w.at,value["turn5"],true);w.at+=1;
w.v.setInt8(w.at,value["turn6"],true);w.at+=1;
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["w"],true);w.at+=4;
w.v.setFloat32(w.at,value["p"],true);w.at+=4;
w.v.setFloat32(w.at,value["r"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext1"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext2"],true);w.at+=4;
w.v.setFloat32(w.at,value["ext3"],true);w.at+=4;
}
function Ros2Rmi_msgsMsgCartesianRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["utool"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["uframe"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["front"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["up"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["left"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["flip"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn5"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["turn6"]=decoded;
}
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
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["w"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["p"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["r"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ext3"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgCartesianCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["utool"];
out["utool"]=field;
}
{const field=value["uframe"];
out["uframe"]=field;
}
{const field=value["front"];
out["front"]=field;
}
{const field=value["up"];
out["up"]=field;
}
{const field=value["left"];
out["left"]=field;
}
{const field=value["flip"];
out["flip"]=field;
}
{const field=value["turn4"];
out["turn4"]=field;
}
{const field=value["turn5"];
out["turn5"]=field;
}
{const field=value["turn6"];
out["turn6"]=field;
}
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
{const field=value["p"];
out["p"]=field;
}
{const field=value["r"];
out["r"]=field;
}
{const field=value["ext1"];
out["ext1"]=field;
}
{const field=value["ext2"];
out["ext2"]=field;
}
{const field=value["ext3"];
out["ext3"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/Command codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgCommandCodec=Object.freeze({codecIdentity:"UWVMROS1_d2741856b8cdf57aa716566edfc705957611557b7da5b6099b173903bdd9960c",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({"INITIAZLIE":"INITIALIZE","INIT":"INITIALIZE","ABORT":"ABORT","PAUSE":"PAUSE","RESUME":"RESUME"}),
defaultValue(){return {"type":""};},
validateValue(value){Ros2Rmi_msgsMsgCommandSize(Ros2Rmi_msgsMsgCommandCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgCommandCapture(value),n=Ros2Rmi_msgsMsgCommandSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgCommandWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgCommandRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgCommandSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
if(n>68)bad(90);return n;}
function Ros2Rmi_msgsMsgCommandWrite(value,w){
w.text(value["type"],64);
}
function Ros2Rmi_msgsMsgCommandRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgCommandCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/CommandOptions codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgCommandOptionsCodec=Object.freeze({codecIdentity:"UWVMROS1_0be0c508035725bf9686807cb8b5267e23f709b7aa34b0910a44ff3604670147",maxWireBytes:1036,minWireBytes:12,cartesianMaxWireBytes:1036n,aggregateWireBytes:0,constants:Object.freeze({"RTSA_ON":"ON","RTSA_OFF":"OFF","PLTZ_ZERODN":"ZERODN","PLTZ_ZEROUP":"ZEROUP","PLTZ_PSPIDN":"PSPIDN","PLTZ_PSPIUP":"PSPIUP","PLTZ_MSPIDN":"MSPIDN","PLTZ_MSPIUP":"MSPIUP"}),
defaultValue(){return {"use_group_mask":false,"group_mask":1,"use_rtsa":false,"rtsa":"ON","use_pltzmode":false,"pltzmode":"ZERODN"};},
validateValue(value){Ros2Rmi_msgsMsgCommandOptionsSize(Ros2Rmi_msgsMsgCommandOptionsCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgCommandOptionsCapture(value),n=Ros2Rmi_msgsMsgCommandOptionsSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgCommandOptionsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1036,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgCommandOptionsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgCommandOptionsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["use_group_mask"]!=="boolean")bad();n=addSize(n,1);
integer(value["group_mask"],1,false);n=addSize(n,1);
if(typeof value["use_rtsa"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["rtsa"],512).length);
if(typeof value["use_pltzmode"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["pltzmode"],512).length);
if(n>1036)bad(90);return n;}
function Ros2Rmi_msgsMsgCommandOptionsWrite(value,w){
w.v.setUint8(w.at,value["use_group_mask"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["group_mask"],true);w.at+=1;
w.v.setUint8(w.at,value["use_rtsa"]?1:0,true);w.at+=1;
w.text(value["rtsa"],512);
w.v.setUint8(w.at,value["use_pltzmode"]?1:0,true);w.at+=1;
w.text(value["pltzmode"],512);
}
function Ros2Rmi_msgsMsgCommandOptionsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_group_mask"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["group_mask"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_rtsa"]=decoded!==0;
}
{
const decoded=r.text(512);
value["rtsa"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_pltzmode"]=decoded!==0;
}
{
const decoded=r.text(512);
value["pltzmode"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgCommandOptionsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["use_group_mask"];
out["use_group_mask"]=field;
}
{const field=value["group_mask"];
out["group_mask"]=field;
}
{const field=value["use_rtsa"];
out["use_rtsa"]=field;
}
{const field=value["rtsa"];
out["rtsa"]=field;
}
{const field=value["use_pltzmode"];
out["use_pltzmode"]=field;
}
{const field=value["pltzmode"];
out["pltzmode"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/Joint codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgJointCodec=Object.freeze({codecIdentity:"UWVMROS1_0ba90adf6cc287bc306fd2a77c52c2e4d62731cfac37bb2cc116ee846249a42d",maxWireBytes:36,minWireBytes:36,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"positions":Array.from({length:9},()=>(0))};},
validateValue(value){Ros2Rmi_msgsMsgJointSize(Ros2Rmi_msgsMsgJointCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgJointCapture(value),n=Ros2Rmi_msgsMsgJointSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgJointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgJointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgJointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["positions"])||value["positions"].length!==9)bad();
for(const element of value["positions"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>36)bad(90);return n;}
function Ros2Rmi_msgsMsgJointWrite(value,w){
for(const element of value["positions"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Rmi_msgsMsgJointRead(r){const value={};
{
{const count=9;if(count>9)bad();value["positions"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["positions"].push(decoded);
}}
}
return value;}
function Ros2Rmi_msgsMsgJointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["positions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==9)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["positions"]=values;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/MotionInstructionOptions codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgMotionInstructionOptionsCodec=Object.freeze({codecIdentity:"UWVMROS1_f8d9ed2e083c577b0cae8965df71aa99643d12ce5a5a6ce558894edfb756bcc6",maxWireBytes:2612,minWireBytes:52,cartesianMaxWireBytes:2612n,aggregateWireBytes:0,constants:Object.freeze({"LCB_TYPE_TB":"TB","LCB_TYPE_TA":"TA","LCB_TYPE_DB":"DB","PORT_TYPE_DOUT":1,"PORT_TYPE_DO":1,"PORT_TYPE_ROUT":2,"PORT_TYPE_RO":2}),
defaultValue(){return {"use_acc":false,"acc":0,"use_offset_pr_number":false,"offset_pr_number":0,"use_vision_pr_number":false,"vision_pr_number":0,"use_wrist_joint":false,"wrist_joint":"ON","use_mrot":false,"mrot":"ON","use_lcb_type":false,"lcb_type":"","use_lcb_value":false,"lcb_value":0,"use_port_type":false,"port_type":0,"use_port_number":false,"port_number":0,"use_port_value":false,"port_value":"","use_tool_offset_pr_number":false,"tool_offset_pr_number":0,"use_alim":false,"alim":0,"use_alim_reg":false,"alim_reg":0,"use_no_blend":false,"no_blend":"ON"};},
validateValue(value){Ros2Rmi_msgsMsgMotionInstructionOptionsSize(Ros2Rmi_msgsMsgMotionInstructionOptionsCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgMotionInstructionOptionsCapture(value),n=Ros2Rmi_msgsMsgMotionInstructionOptionsSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgMotionInstructionOptionsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2612,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgMotionInstructionOptionsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgMotionInstructionOptionsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["use_acc"]!=="boolean")bad();n=addSize(n,1);
integer(value["acc"],1,false);n=addSize(n,1);
if(typeof value["use_offset_pr_number"]!=="boolean")bad();n=addSize(n,1);
integer(value["offset_pr_number"],2,false);n=addSize(n,2);
if(typeof value["use_vision_pr_number"]!=="boolean")bad();n=addSize(n,1);
integer(value["vision_pr_number"],2,false);n=addSize(n,2);
if(typeof value["use_wrist_joint"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["wrist_joint"],512).length);
if(typeof value["use_mrot"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["mrot"],512).length);
if(typeof value["use_lcb_type"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["lcb_type"],512).length);
if(typeof value["use_lcb_value"]!=="boolean")bad();n=addSize(n,1);
integer(value["lcb_value"],2,false);n=addSize(n,2);
if(typeof value["use_port_type"]!=="boolean")bad();n=addSize(n,1);
integer(value["port_type"],1,false);n=addSize(n,1);
if(typeof value["use_port_number"]!=="boolean")bad();n=addSize(n,1);
integer(value["port_number"],2,false);n=addSize(n,2);
if(typeof value["use_port_value"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["port_value"],512).length);
if(typeof value["use_tool_offset_pr_number"]!=="boolean")bad();n=addSize(n,1);
integer(value["tool_offset_pr_number"],2,false);n=addSize(n,2);
if(typeof value["use_alim"]!=="boolean")bad();n=addSize(n,1);
integer(value["alim"],4,true);n=addSize(n,4);
if(typeof value["use_alim_reg"]!=="boolean")bad();n=addSize(n,1);
integer(value["alim_reg"],2,false);n=addSize(n,2);
if(typeof value["use_no_blend"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["no_blend"],512).length);
if(n>2612)bad(90);return n;}
function Ros2Rmi_msgsMsgMotionInstructionOptionsWrite(value,w){
w.v.setUint8(w.at,value["use_acc"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["acc"],true);w.at+=1;
w.v.setUint8(w.at,value["use_offset_pr_number"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["offset_pr_number"],true);w.at+=2;
w.v.setUint8(w.at,value["use_vision_pr_number"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["vision_pr_number"],true);w.at+=2;
w.v.setUint8(w.at,value["use_wrist_joint"]?1:0,true);w.at+=1;
w.text(value["wrist_joint"],512);
w.v.setUint8(w.at,value["use_mrot"]?1:0,true);w.at+=1;
w.text(value["mrot"],512);
w.v.setUint8(w.at,value["use_lcb_type"]?1:0,true);w.at+=1;
w.text(value["lcb_type"],512);
w.v.setUint8(w.at,value["use_lcb_value"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["lcb_value"],true);w.at+=2;
w.v.setUint8(w.at,value["use_port_type"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["port_type"],true);w.at+=1;
w.v.setUint8(w.at,value["use_port_number"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["port_number"],true);w.at+=2;
w.v.setUint8(w.at,value["use_port_value"]?1:0,true);w.at+=1;
w.text(value["port_value"],512);
w.v.setUint8(w.at,value["use_tool_offset_pr_number"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["tool_offset_pr_number"],true);w.at+=2;
w.v.setUint8(w.at,value["use_alim"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["alim"],true);w.at+=4;
w.v.setUint8(w.at,value["use_alim_reg"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["alim_reg"],true);w.at+=2;
w.v.setUint8(w.at,value["use_no_blend"]?1:0,true);w.at+=1;
w.text(value["no_blend"],512);
}
function Ros2Rmi_msgsMsgMotionInstructionOptionsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_acc"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["acc"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_offset_pr_number"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["offset_pr_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_vision_pr_number"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["vision_pr_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_wrist_joint"]=decoded!==0;
}
{
const decoded=r.text(512);
value["wrist_joint"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_mrot"]=decoded!==0;
}
{
const decoded=r.text(512);
value["mrot"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_lcb_type"]=decoded!==0;
}
{
const decoded=r.text(512);
value["lcb_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_lcb_value"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["lcb_value"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_port_type"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["port_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_port_number"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["port_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_port_value"]=decoded!==0;
}
{
const decoded=r.text(512);
value["port_value"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_tool_offset_pr_number"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["tool_offset_pr_number"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_alim"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["alim"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_alim_reg"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["alim_reg"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_no_blend"]=decoded!==0;
}
{
const decoded=r.text(512);
value["no_blend"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgMotionInstructionOptionsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["use_acc"];
out["use_acc"]=field;
}
{const field=value["acc"];
out["acc"]=field;
}
{const field=value["use_offset_pr_number"];
out["use_offset_pr_number"]=field;
}
{const field=value["offset_pr_number"];
out["offset_pr_number"]=field;
}
{const field=value["use_vision_pr_number"];
out["use_vision_pr_number"]=field;
}
{const field=value["vision_pr_number"];
out["vision_pr_number"]=field;
}
{const field=value["use_wrist_joint"];
out["use_wrist_joint"]=field;
}
{const field=value["wrist_joint"];
out["wrist_joint"]=field;
}
{const field=value["use_mrot"];
out["use_mrot"]=field;
}
{const field=value["mrot"];
out["mrot"]=field;
}
{const field=value["use_lcb_type"];
out["use_lcb_type"]=field;
}
{const field=value["lcb_type"];
out["lcb_type"]=field;
}
{const field=value["use_lcb_value"];
out["use_lcb_value"]=field;
}
{const field=value["lcb_value"];
out["lcb_value"]=field;
}
{const field=value["use_port_type"];
out["use_port_type"]=field;
}
{const field=value["port_type"];
out["port_type"]=field;
}
{const field=value["use_port_number"];
out["use_port_number"]=field;
}
{const field=value["port_number"];
out["port_number"]=field;
}
{const field=value["use_port_value"];
out["use_port_value"]=field;
}
{const field=value["port_value"];
out["port_value"]=field;
}
{const field=value["use_tool_offset_pr_number"];
out["use_tool_offset_pr_number"]=field;
}
{const field=value["tool_offset_pr_number"];
out["tool_offset_pr_number"]=field;
}
{const field=value["use_alim"];
out["use_alim"]=field;
}
{const field=value["alim"];
out["alim"]=field;
}
{const field=value["use_alim_reg"];
out["use_alim_reg"]=field;
}
{const field=value["alim_reg"];
out["alim_reg"]=field;
}
{const field=value["use_no_blend"];
out["use_no_blend"]=field;
}
{const field=value["no_blend"];
out["no_blend"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/MotionType codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgMotionTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_c51cc7a32f6853d13bedaf3306dd79e445de6c0d16ae0ec39034ce0ae6d19c8a",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({"J":"J","JOINT":"J","L":"L","LINEAR":"L","C":"C","CIRCULAR":"C","S":"S","SPLINE":"S"}),
defaultValue(){return {"type":"J"};},
validateValue(value){Ros2Rmi_msgsMsgMotionTypeSize(Ros2Rmi_msgsMsgMotionTypeCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgMotionTypeCapture(value),n=Ros2Rmi_msgsMsgMotionTypeSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgMotionTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgMotionTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgMotionTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
if(n>68)bad(90);return n;}
function Ros2Rmi_msgsMsgMotionTypeWrite(value,w){
w.text(value["type"],64);
}
function Ros2Rmi_msgsMsgMotionTypeRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgMotionTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/Representation codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgRepresentationCodec=Object.freeze({codecIdentity:"UWVMROS1_b439506f2dc2135bb484b3abb308dfdf5c07367af5a4d6d0f5435ad9bba8ea6f",maxWireBytes:516,minWireBytes:4,cartesianMaxWireBytes:516n,aggregateWireBytes:0,constants:Object.freeze({"JOINT":"JOINT","J":"JOINT","CARTESIAN":"CARTESIAN","CART":"CARTESIAN","C":"CARTESIAN"}),
defaultValue(){return {"representation":"JOINT"};},
validateValue(value){Ros2Rmi_msgsMsgRepresentationSize(Ros2Rmi_msgsMsgRepresentationCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgRepresentationCapture(value),n=Ros2Rmi_msgsMsgRepresentationSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgRepresentationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,516,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgRepresentationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgRepresentationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["representation"],512).length);
if(n>516)bad(90);return n;}
function Ros2Rmi_msgsMsgRepresentationWrite(value,w){
w.text(value["representation"],512);
}
function Ros2Rmi_msgsMsgRepresentationRead(r){const value={};
{
const decoded=r.text(512);
value["representation"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgRepresentationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["representation"];
out["representation"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/SpeedType codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgSpeedTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_e4f4dbda84d8d8cd4d026b0beb52005663c7c4ee602c8d1e4d5853589777c282",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({"PERCENT":"Percent","PERCENTAGE":"Percent","MMSEC":"mmSec","INCHMIN":"InchMin","TIME":"Time","SECONDS":"Time","SEC":"Time","MSEC":"mSec"}),
defaultValue(){return {"type":"Percent"};},
validateValue(value){Ros2Rmi_msgsMsgSpeedTypeSize(Ros2Rmi_msgsMsgSpeedTypeCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgSpeedTypeCapture(value),n=Ros2Rmi_msgsMsgSpeedTypeSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgSpeedTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgSpeedTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgSpeedTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
if(n>68)bad(90);return n;}
function Ros2Rmi_msgsMsgSpeedTypeWrite(value,w){
w.text(value["type"],64);
}
function Ros2Rmi_msgsMsgSpeedTypeRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgSpeedTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/msg/TermType codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsMsgTermTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_5ddbb483126e0ab5799ed865480da457179d83a271581c1471aec940bedc4535",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({"FINE":"FINE","CNT":"CNT","CR":"CR","CD":"CD"}),
defaultValue(){return {"type":"FINE"};},
validateValue(value){Ros2Rmi_msgsMsgTermTypeSize(Ros2Rmi_msgsMsgTermTypeCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsMsgTermTypeCapture(value),n=Ros2Rmi_msgsMsgTermTypeSize(owned),w=new Writer(n);Ros2Rmi_msgsMsgTermTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsMsgTermTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsMsgTermTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["type"],64).length);
if(n>68)bad(90);return n;}
function Ros2Rmi_msgsMsgTermTypeWrite(value,w){
w.text(value["type"],64);
}
function Ros2Rmi_msgsMsgTermTypeRead(r){const value={};
{
const decoded=r.text(64);
value["type"]=decoded;
}
return value;}
function Ros2Rmi_msgsMsgTermTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["type"];
out["type"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddCall_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddCall_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4d652cfbaef29487a0a688932ee926d7a4026c5e7ac8a0eace583b503c37bcd8",maxWireBytes:9992,minWireBytes:8,cartesianMaxWireBytes:9992n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"program_name":"","params":[]};},
validateValue(value){Ros2Rmi_msgsSrvAddCall_RequestSize(Ros2Rmi_msgsSrvAddCall_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddCall_RequestCapture(value),n=Ros2Rmi_msgsSrvAddCall_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddCall_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9992,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddCall_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddCall_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["program_name"],512).length);
if(!Array.isArray(value["params"])||value["params"].length>16)bad();
n=addSize(n,4);
for(const element of value["params"]){
n=addSize(n,Ros2Rmi_msgsMsgCallParamSize(element));
}
if(n>9992)bad(90);return n;}
function Ros2Rmi_msgsSrvAddCall_RequestWrite(value,w){
w.text(value["program_name"],512);
w.v.setUint32(w.at,value["params"].length,true);w.at+=4;
for(const element of value["params"]){
Ros2Rmi_msgsMsgCallParamWrite(element,w);
}
}
function Ros2Rmi_msgsSrvAddCall_RequestRead(r){const value={};
{
const decoded=r.text(512);
value["program_name"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["params"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Rmi_msgsMsgCallParamRead(r);
value["params"].push(decoded);
}}
}
return value;}
function Ros2Rmi_msgsSrvAddCall_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["program_name"];
out["program_name"]=field;
}
{const field=value["params"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Rmi_msgsMsgCallParamCapture(element);
}out["params"]=values;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddCall_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddCall_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4f8495dfc3c69ec640444fd24843a7bb3e30008d90945bdd64817d8acb7964e1",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddCall_ResponseSize(Ros2Rmi_msgsSrvAddCall_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddCall_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddCall_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddCall_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddCall_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddCall_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddCall_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddCall_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddCall_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddMotionInstruction_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddMotionInstruction_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3f22c49cb2d32e6a8f3954f3d3cbf0021c87e0363b1f2ded4ac20a97a5b59391",maxWireBytes:4014,minWireBytes:238,cartesianMaxWireBytes:4014n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"motion_type":Ros2Rmi_msgsMsgMotionTypeCodec.defaultValue(),"incremental":false,"representation":Ros2Rmi_msgsMsgRepresentationCodec.defaultValue(),"joint":Ros2Rmi_msgsMsgJointCodec.defaultValue(),"cartesian":Ros2Rmi_msgsMsgCartesianCodec.defaultValue(),"speed_type":Ros2Rmi_msgsMsgSpeedTypeCodec.defaultValue(),"speed_value":0,"term_type":Ros2Rmi_msgsMsgTermTypeCodec.defaultValue(),"term_value":0,"via_representation":Ros2Rmi_msgsMsgRepresentationCodec.defaultValue(),"via_joint":Ros2Rmi_msgsMsgJointCodec.defaultValue(),"via_cartesian":Ros2Rmi_msgsMsgCartesianCodec.defaultValue(),"options":Ros2Rmi_msgsMsgMotionInstructionOptionsCodec.defaultValue()};},
validateValue(value){Ros2Rmi_msgsSrvAddMotionInstruction_RequestSize(Ros2Rmi_msgsSrvAddMotionInstruction_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddMotionInstruction_RequestCapture(value),n=Ros2Rmi_msgsSrvAddMotionInstruction_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddMotionInstruction_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4014,90);if(owned.length<238)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddMotionInstruction_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddMotionInstruction_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Rmi_msgsMsgMotionTypeSize(value["motion_type"]));
if(typeof value["incremental"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros2Rmi_msgsMsgRepresentationSize(value["representation"]));
n=addSize(n,Ros2Rmi_msgsMsgJointSize(value["joint"]));
n=addSize(n,Ros2Rmi_msgsMsgCartesianSize(value["cartesian"]));
n=addSize(n,Ros2Rmi_msgsMsgSpeedTypeSize(value["speed_type"]));
integer(value["speed_value"],2,false);n=addSize(n,2);
n=addSize(n,Ros2Rmi_msgsMsgTermTypeSize(value["term_type"]));
integer(value["term_value"],1,false);n=addSize(n,1);
n=addSize(n,Ros2Rmi_msgsMsgRepresentationSize(value["via_representation"]));
n=addSize(n,Ros2Rmi_msgsMsgJointSize(value["via_joint"]));
n=addSize(n,Ros2Rmi_msgsMsgCartesianSize(value["via_cartesian"]));
n=addSize(n,Ros2Rmi_msgsMsgMotionInstructionOptionsSize(value["options"]));
if(n>4014)bad(90);return n;}
function Ros2Rmi_msgsSrvAddMotionInstruction_RequestWrite(value,w){
Ros2Rmi_msgsMsgMotionTypeWrite(value["motion_type"],w);
w.v.setUint8(w.at,value["incremental"]?1:0,true);w.at+=1;
Ros2Rmi_msgsMsgRepresentationWrite(value["representation"],w);
Ros2Rmi_msgsMsgJointWrite(value["joint"],w);
Ros2Rmi_msgsMsgCartesianWrite(value["cartesian"],w);
Ros2Rmi_msgsMsgSpeedTypeWrite(value["speed_type"],w);
w.v.setUint16(w.at,value["speed_value"],true);w.at+=2;
Ros2Rmi_msgsMsgTermTypeWrite(value["term_type"],w);
w.v.setUint8(w.at,value["term_value"],true);w.at+=1;
Ros2Rmi_msgsMsgRepresentationWrite(value["via_representation"],w);
Ros2Rmi_msgsMsgJointWrite(value["via_joint"],w);
Ros2Rmi_msgsMsgCartesianWrite(value["via_cartesian"],w);
Ros2Rmi_msgsMsgMotionInstructionOptionsWrite(value["options"],w);
}
function Ros2Rmi_msgsSrvAddMotionInstruction_RequestRead(r){const value={};
{
const decoded=Ros2Rmi_msgsMsgMotionTypeRead(r);
value["motion_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["incremental"]=decoded!==0;
}
{
const decoded=Ros2Rmi_msgsMsgRepresentationRead(r);
value["representation"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgJointRead(r);
value["joint"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCartesianRead(r);
value["cartesian"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgSpeedTypeRead(r);
value["speed_type"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["speed_value"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgTermTypeRead(r);
value["term_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["term_value"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgRepresentationRead(r);
value["via_representation"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgJointRead(r);
value["via_joint"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCartesianRead(r);
value["via_cartesian"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgMotionInstructionOptionsRead(r);
value["options"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddMotionInstruction_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["motion_type"];
out["motion_type"]=Ros2Rmi_msgsMsgMotionTypeCapture(field);
}
{const field=value["incremental"];
out["incremental"]=field;
}
{const field=value["representation"];
out["representation"]=Ros2Rmi_msgsMsgRepresentationCapture(field);
}
{const field=value["joint"];
out["joint"]=Ros2Rmi_msgsMsgJointCapture(field);
}
{const field=value["cartesian"];
out["cartesian"]=Ros2Rmi_msgsMsgCartesianCapture(field);
}
{const field=value["speed_type"];
out["speed_type"]=Ros2Rmi_msgsMsgSpeedTypeCapture(field);
}
{const field=value["speed_value"];
out["speed_value"]=field;
}
{const field=value["term_type"];
out["term_type"]=Ros2Rmi_msgsMsgTermTypeCapture(field);
}
{const field=value["term_value"];
out["term_value"]=field;
}
{const field=value["via_representation"];
out["via_representation"]=Ros2Rmi_msgsMsgRepresentationCapture(field);
}
{const field=value["via_joint"];
out["via_joint"]=Ros2Rmi_msgsMsgJointCapture(field);
}
{const field=value["via_cartesian"];
out["via_cartesian"]=Ros2Rmi_msgsMsgCartesianCapture(field);
}
{const field=value["options"];
out["options"]=Ros2Rmi_msgsMsgMotionInstructionOptionsCapture(field);
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddMotionInstruction_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_79fee3e965fda63c3a53ad2178ff48614878acc9f6af74cc27a8c2812b68ac6b",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddMotionInstruction_ResponseSize(Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddMotionInstruction_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddMotionInstruction_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddMotionInstruction_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddMotionInstruction_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddMotionInstruction_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddMotionInstruction_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetPayload_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetPayload_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_45da9f81e8c26f49490a4e6eba5c8be0ee67285124540eb1c3e1f2c3e0fd821b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"schedule_number":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetPayload_RequestSize(Ros2Rmi_msgsSrvAddSetPayload_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetPayload_RequestCapture(value),n=Ros2Rmi_msgsSrvAddSetPayload_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetPayload_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetPayload_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetPayload_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["schedule_number"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetPayload_RequestWrite(value,w){
w.v.setUint8(w.at,value["schedule_number"],true);w.at+=1;
}
function Ros2Rmi_msgsSrvAddSetPayload_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["schedule_number"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetPayload_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["schedule_number"];
out["schedule_number"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetPayload_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetPayload_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bfc5bc7b92f292c2980496cb18fbe953ea1b49d45f7950f0125bbbf880bdf72a",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetPayload_ResponseSize(Ros2Rmi_msgsSrvAddSetPayload_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetPayload_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddSetPayload_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetPayload_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetPayload_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetPayload_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetPayload_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddSetPayload_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetPayload_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetUFrame_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetUFrame_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_64c83dd2be89b4b6dcf3b3c2ea70184971e6a359914ff1a5cd26d4b114d35475",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"frame_number":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetUFrame_RequestSize(Ros2Rmi_msgsSrvAddSetUFrame_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetUFrame_RequestCapture(value),n=Ros2Rmi_msgsSrvAddSetUFrame_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetUFrame_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetUFrame_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetUFrame_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["frame_number"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetUFrame_RequestWrite(value,w){
w.v.setUint8(w.at,value["frame_number"],true);w.at+=1;
}
function Ros2Rmi_msgsSrvAddSetUFrame_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["frame_number"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetUFrame_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["frame_number"];
out["frame_number"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetUFrame_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetUFrame_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e344ef6efacc8d569111d1c6ef4028e1a5bb54af49d3e8f486ae89b83c674647",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetUFrame_ResponseSize(Ros2Rmi_msgsSrvAddSetUFrame_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetUFrame_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddSetUFrame_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetUFrame_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetUFrame_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetUFrame_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetUFrame_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddSetUFrame_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetUFrame_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetUTool_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetUTool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d1327022326cd41caed6001449517a41f589e148c38d6db30923922175f05c46",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tool_number":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetUTool_RequestSize(Ros2Rmi_msgsSrvAddSetUTool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetUTool_RequestCapture(value),n=Ros2Rmi_msgsSrvAddSetUTool_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetUTool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetUTool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetUTool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["tool_number"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetUTool_RequestWrite(value,w){
w.v.setUint8(w.at,value["tool_number"],true);w.at+=1;
}
function Ros2Rmi_msgsSrvAddSetUTool_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["tool_number"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetUTool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tool_number"];
out["tool_number"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddSetUTool_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddSetUTool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_dd4c36458027b87559200cf5f6aa72a60af2d103f663f2fa9daca47ce8020ff6",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddSetUTool_ResponseSize(Ros2Rmi_msgsSrvAddSetUTool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddSetUTool_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddSetUTool_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddSetUTool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddSetUTool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddSetUTool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddSetUTool_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddSetUTool_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddSetUTool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddWaitDIN_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddWaitDIN_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c32214f1ff1b0b6980583fc3088f412e856efb5eaa9c336cf798ef8ad85c61ab",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"port_number":0,"port_value":""};},
validateValue(value){Ros2Rmi_msgsSrvAddWaitDIN_RequestSize(Ros2Rmi_msgsSrvAddWaitDIN_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddWaitDIN_RequestCapture(value),n=Ros2Rmi_msgsSrvAddWaitDIN_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddWaitDIN_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddWaitDIN_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddWaitDIN_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["port_number"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["port_value"],512).length);
if(n>518)bad(90);return n;}
function Ros2Rmi_msgsSrvAddWaitDIN_RequestWrite(value,w){
w.v.setUint16(w.at,value["port_number"],true);w.at+=2;
w.text(value["port_value"],512);
}
function Ros2Rmi_msgsSrvAddWaitDIN_RequestRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["port_number"]=decoded;
}
{
const decoded=r.text(512);
value["port_value"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddWaitDIN_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["port_number"];
out["port_number"]=field;
}
{const field=value["port_value"];
out["port_value"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddWaitDIN_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddWaitDIN_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7d3933f573619ba49f19df7c11f1d73ef211c17581f660f8c0fbcc152cd519d4",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddWaitDIN_ResponseSize(Ros2Rmi_msgsSrvAddWaitDIN_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddWaitDIN_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddWaitDIN_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddWaitDIN_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddWaitDIN_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddWaitDIN_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddWaitDIN_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddWaitDIN_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddWaitDIN_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddWaitTime_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddWaitTime_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8487f4b7dcfb044c72f7fb62a31ffe9cdc4321a9ebdaa0550a363e32acbadc46",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"time":0};},
validateValue(value){Ros2Rmi_msgsSrvAddWaitTime_RequestSize(Ros2Rmi_msgsSrvAddWaitTime_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddWaitTime_RequestCapture(value),n=Ros2Rmi_msgsSrvAddWaitTime_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddWaitTime_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddWaitTime_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddWaitTime_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["time"]!=="number"||!Number.isFinite(value["time"])||!Number.isFinite(Math.fround(value["time"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Rmi_msgsSrvAddWaitTime_RequestWrite(value,w){
w.v.setFloat32(w.at,value["time"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddWaitTime_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["time"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddWaitTime_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["time"];
out["time"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/AddWaitTime_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvAddWaitTime_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f9578c08b971f32f91d1c94b8713cafe648e25147394e20c85bb335254fa20c8",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0,"sequence_id":0};},
validateValue(value){Ros2Rmi_msgsSrvAddWaitTime_ResponseSize(Ros2Rmi_msgsSrvAddWaitTime_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvAddWaitTime_ResponseCapture(value),n=Ros2Rmi_msgsSrvAddWaitTime_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvAddWaitTime_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvAddWaitTime_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvAddWaitTime_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
integer(value["sequence_id"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Rmi_msgsSrvAddWaitTime_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
w.v.setInt32(w.at,value["sequence_id"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvAddWaitTime_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sequence_id"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvAddWaitTime_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
}
{const field=value["sequence_id"];
out["sequence_id"]=field;
}
return out;}
/** Complete ROS2 rmi_msgs/srv/CallCommand_Request codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvCallCommand_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d1f890a2beec0ca9294ff4f641d1738855d576a917ed05eb34517886c73f381c",maxWireBytes:1104,minWireBytes:16,cartesianMaxWireBytes:1104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"command":Ros2Rmi_msgsMsgCommandCodec.defaultValue(),"options":Ros2Rmi_msgsMsgCommandOptionsCodec.defaultValue()};},
validateValue(value){Ros2Rmi_msgsSrvCallCommand_RequestSize(Ros2Rmi_msgsSrvCallCommand_RequestCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvCallCommand_RequestCapture(value),n=Ros2Rmi_msgsSrvCallCommand_RequestSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvCallCommand_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1104,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvCallCommand_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvCallCommand_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Rmi_msgsMsgCommandSize(value["command"]));
n=addSize(n,Ros2Rmi_msgsMsgCommandOptionsSize(value["options"]));
if(n>1104)bad(90);return n;}
function Ros2Rmi_msgsSrvCallCommand_RequestWrite(value,w){
Ros2Rmi_msgsMsgCommandWrite(value["command"],w);
Ros2Rmi_msgsMsgCommandOptionsWrite(value["options"],w);
}
function Ros2Rmi_msgsSrvCallCommand_RequestRead(r){const value={};
{
const decoded=Ros2Rmi_msgsMsgCommandRead(r);
value["command"]=decoded;
}
{
const decoded=Ros2Rmi_msgsMsgCommandOptionsRead(r);
value["options"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvCallCommand_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["command"];
out["command"]=Ros2Rmi_msgsMsgCommandCapture(field);
}
{const field=value["options"];
out["options"]=Ros2Rmi_msgsMsgCommandOptionsCapture(field);
}
return out;}
/** Complete ROS2 rmi_msgs/srv/CallCommand_Response codec; maxima are not actual lengths. */
export const Ros2Rmi_msgsSrvCallCommand_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_64d92e6fadc651073469318ad15d2e98ec3d059c629838cefc5820b17ab6f1bc",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result":0};},
validateValue(value){Ros2Rmi_msgsSrvCallCommand_ResponseSize(Ros2Rmi_msgsSrvCallCommand_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Rmi_msgsSrvCallCommand_ResponseCapture(value),n=Ros2Rmi_msgsSrvCallCommand_ResponseSize(owned),w=new Writer(n);Ros2Rmi_msgsSrvCallCommand_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Rmi_msgsSrvCallCommand_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Rmi_msgsSrvCallCommand_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Rmi_msgsSrvCallCommand_ResponseWrite(value,w){
w.v.setInt32(w.at,value["result"],true);w.at+=4;
}
function Ros2Rmi_msgsSrvCallCommand_ResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["result"]=decoded;
}
return value;}
function Ros2Rmi_msgsSrvCallCommand_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result"];
out["result"]=field;
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
