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
/** Complete ROS2 kuka_driver_interfaces/msg/FRIState codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgFRIStateCodec=Object.freeze({codecIdentity:"UWVMROS1_27be8431e4a8cda3bb4d6d5a8a69fd86a0d07a1d920799c232d63a4941ee6716",maxWireBytes:40,minWireBytes:40,cartesianMaxWireBytes:40n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"session_state":0,"connection_quality":0,"safety_state":0,"command_mode":0,"control_mode":0,"operation_mode":0,"drive_state":0,"overlay_type":0,"tracking_performance":0};},
validateValue(value){Ros2Kuka_driver_interfacesMsgFRIStateSize(Ros2Kuka_driver_interfacesMsgFRIStateCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgFRIStateCapture(value),n=Ros2Kuka_driver_interfacesMsgFRIStateSize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgFRIStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,40,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgFRIStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgFRIStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["session_state"],4,true);n=addSize(n,4);
integer(value["connection_quality"],4,true);n=addSize(n,4);
integer(value["safety_state"],4,true);n=addSize(n,4);
integer(value["command_mode"],4,true);n=addSize(n,4);
integer(value["control_mode"],4,true);n=addSize(n,4);
integer(value["operation_mode"],4,true);n=addSize(n,4);
integer(value["drive_state"],4,true);n=addSize(n,4);
integer(value["overlay_type"],4,true);n=addSize(n,4);
if(typeof value["tracking_performance"]!=="number"||!Number.isFinite(value["tracking_performance"]))bad();n=addSize(n,8);
if(n>40)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgFRIStateWrite(value,w){
w.v.setInt32(w.at,value["session_state"],true);w.at+=4;
w.v.setInt32(w.at,value["connection_quality"],true);w.at+=4;
w.v.setInt32(w.at,value["safety_state"],true);w.at+=4;
w.v.setInt32(w.at,value["command_mode"],true);w.at+=4;
w.v.setInt32(w.at,value["control_mode"],true);w.at+=4;
w.v.setInt32(w.at,value["operation_mode"],true);w.at+=4;
w.v.setInt32(w.at,value["drive_state"],true);w.at+=4;
w.v.setInt32(w.at,value["overlay_type"],true);w.at+=4;
w.v.setFloat64(w.at,value["tracking_performance"],true);w.at+=8;
}
function Ros2Kuka_driver_interfacesMsgFRIStateRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["session_state"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["connection_quality"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["safety_state"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["command_mode"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["control_mode"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["operation_mode"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["drive_state"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["overlay_type"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["tracking_performance"]=decoded;
}
return value;}
function Ros2Kuka_driver_interfacesMsgFRIStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["session_state"];
out["session_state"]=field;
}
{const field=value["connection_quality"];
out["connection_quality"]=field;
}
{const field=value["safety_state"];
out["safety_state"]=field;
}
{const field=value["command_mode"];
out["command_mode"]=field;
}
{const field=value["control_mode"];
out["control_mode"]=field;
}
{const field=value["operation_mode"];
out["operation_mode"]=field;
}
{const field=value["drive_state"];
out["drive_state"]=field;
}
{const field=value["overlay_type"];
out["overlay_type"]=field;
}
{const field=value["tracking_performance"];
out["tracking_performance"]=field;
}
return out;}
/** Complete ROS2 kuka_driver_interfaces/msg/FRIStateArray codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgFRIStateArrayCodec=Object.freeze({codecIdentity:"UWVMROS1_4312aa4103fd0c92fe7cd9d1201c40e327b05812cdc60130d11b8c2e0eb47449",maxWireBytes:1736,minWireBytes:8,cartesianMaxWireBytes:1736n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robot_names":[],"states":[]};},
validateValue(value){Ros2Kuka_driver_interfacesMsgFRIStateArraySize(Ros2Kuka_driver_interfacesMsgFRIStateArrayCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgFRIStateArrayCapture(value),n=Ros2Kuka_driver_interfacesMsgFRIStateArraySize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgFRIStateArrayWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1736,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgFRIStateArrayRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgFRIStateArraySize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["robot_names"])||value["robot_names"].length>16)bad();
n=addSize(n,4);
for(const element of value["robot_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["states"])||value["states"].length>16)bad();
n=addSize(n,4);
for(const element of value["states"]){
n=addSize(n,Ros2Kuka_driver_interfacesMsgFRIStateSize(element));
}
if(n>1736)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgFRIStateArrayWrite(value,w){
w.v.setUint32(w.at,value["robot_names"].length,true);w.at+=4;
for(const element of value["robot_names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["states"].length,true);w.at+=4;
for(const element of value["states"]){
Ros2Kuka_driver_interfacesMsgFRIStateWrite(element,w);
}
}
function Ros2Kuka_driver_interfacesMsgFRIStateArrayRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["robot_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["robot_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Kuka_driver_interfacesMsgFRIStateRead(r);
value["states"].push(decoded);
}}
}
return value;}
function Ros2Kuka_driver_interfacesMsgFRIStateArrayCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robot_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["robot_names"]=values;
}
{const field=value["states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Kuka_driver_interfacesMsgFRIStateCapture(element);
}out["states"]=values;
}
return out;}
/** Complete ROS2 kuka_driver_interfaces/msg/FriConfiguration codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgFriConfigurationCodec=Object.freeze({codecIdentity:"UWVMROS1_a60335766ad606adc5699c833cc6bc43adc3067691bcdbf587bbfb56e8a7127e",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"receive_multiplier":0,"send_period_ms":0};},
validateValue(value){Ros2Kuka_driver_interfacesMsgFriConfigurationSize(Ros2Kuka_driver_interfacesMsgFriConfigurationCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgFriConfigurationCapture(value),n=Ros2Kuka_driver_interfacesMsgFriConfigurationSize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgFriConfigurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgFriConfigurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgFriConfigurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["receive_multiplier"],4,true);n=addSize(n,4);
integer(value["send_period_ms"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgFriConfigurationWrite(value,w){
w.v.setInt32(w.at,value["receive_multiplier"],true);w.at+=4;
w.v.setInt32(w.at,value["send_period_ms"],true);w.at+=4;
}
function Ros2Kuka_driver_interfacesMsgFriConfigurationRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["receive_multiplier"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["send_period_ms"]=decoded;
}
return value;}
function Ros2Kuka_driver_interfacesMsgFriConfigurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["receive_multiplier"];
out["receive_multiplier"]=field;
}
{const field=value["send_period_ms"];
out["send_period_ms"]=field;
}
return out;}
/** Complete ROS2 kuka_driver_interfaces/msg/HardwareEvent codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgHardwareEventCodec=Object.freeze({codecIdentity:"UWVMROS1_cb542e9311957712257c6d8ae273b3d2d2339760978e11e3798982c84fbc5e68",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robot_name":"","event":0};},
validateValue(value){Ros2Kuka_driver_interfacesMsgHardwareEventSize(Ros2Kuka_driver_interfacesMsgHardwareEventCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgHardwareEventCapture(value),n=Ros2Kuka_driver_interfacesMsgHardwareEventSize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgHardwareEventWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgHardwareEventRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgHardwareEventSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["robot_name"],512).length);
integer(value["event"],1,false);n=addSize(n,1);
if(n>517)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgHardwareEventWrite(value,w){
w.text(value["robot_name"],512);
w.v.setUint8(w.at,value["event"],true);w.at+=1;
}
function Ros2Kuka_driver_interfacesMsgHardwareEventRead(r){const value={};
{
const decoded=r.text(512);
value["robot_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["event"]=decoded;
}
return value;}
function Ros2Kuka_driver_interfacesMsgHardwareEventCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robot_name"];
out["robot_name"]=field;
}
{const field=value["event"];
out["event"]=field;
}
return out;}
/** Complete ROS2 kuka_driver_interfaces/msg/KssStatus codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgKssStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_b28bc2f19a0f410f0cd1d5b1d59daf1d09acb3e8312b4267f622a4d119f16b82",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({"UNSPECIFIED":0,"JOINT_POSITION_CONTROL":1,"CARTESIAN_POSITION_CONTROL":5,"RSI_4MS":1,"RSI_12MS":2,"T1":1,"T2":2,"AUT":3,"EXT":4}),
defaultValue(){return {"control_mode":0,"cycle_time":0,"drives_powered":false,"emergency_stop":false,"guard_stop":false,"in_motion":false,"motion_possible":false,"operation_mode":0,"robot_stopped":false};},
validateValue(value){Ros2Kuka_driver_interfacesMsgKssStatusSize(Ros2Kuka_driver_interfacesMsgKssStatusCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgKssStatusCapture(value),n=Ros2Kuka_driver_interfacesMsgKssStatusSize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgKssStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgKssStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgKssStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["control_mode"],1,false);n=addSize(n,1);
integer(value["cycle_time"],1,false);n=addSize(n,1);
if(typeof value["drives_powered"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["emergency_stop"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["guard_stop"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["in_motion"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["motion_possible"]!=="boolean")bad();n=addSize(n,1);
integer(value["operation_mode"],1,false);n=addSize(n,1);
if(typeof value["robot_stopped"]!=="boolean")bad();n=addSize(n,1);
if(n>9)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgKssStatusWrite(value,w){
w.v.setUint8(w.at,value["control_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["cycle_time"],true);w.at+=1;
w.v.setUint8(w.at,value["drives_powered"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["emergency_stop"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["guard_stop"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["in_motion"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["motion_possible"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["operation_mode"],true);w.at+=1;
w.v.setUint8(w.at,value["robot_stopped"]?1:0,true);w.at+=1;
}
function Ros2Kuka_driver_interfacesMsgKssStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["control_mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["cycle_time"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["drives_powered"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["emergency_stop"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["guard_stop"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["in_motion"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motion_possible"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["operation_mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["robot_stopped"]=decoded!==0;
}
return value;}
function Ros2Kuka_driver_interfacesMsgKssStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["control_mode"];
out["control_mode"]=field;
}
{const field=value["cycle_time"];
out["cycle_time"]=field;
}
{const field=value["drives_powered"];
out["drives_powered"]=field;
}
{const field=value["emergency_stop"];
out["emergency_stop"]=field;
}
{const field=value["guard_stop"];
out["guard_stop"]=field;
}
{const field=value["in_motion"];
out["in_motion"]=field;
}
{const field=value["motion_possible"];
out["motion_possible"]=field;
}
{const field=value["operation_mode"];
out["operation_mode"]=field;
}
{const field=value["robot_stopped"];
out["robot_stopped"]=field;
}
return out;}
/** Complete ROS2 kuka_driver_interfaces/msg/KssStatusArray codec; maxima are not actual lengths. */
export const Ros2Kuka_driver_interfacesMsgKssStatusArrayCodec=Object.freeze({codecIdentity:"UWVMROS1_3c8e93b369eefea89c0aa56ab657721edd041ac66e18cfbf4fc4201df60dbebd",maxWireBytes:1240,minWireBytes:8,cartesianMaxWireBytes:1240n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robot_names":[],"statuses":[]};},
validateValue(value){Ros2Kuka_driver_interfacesMsgKssStatusArraySize(Ros2Kuka_driver_interfacesMsgKssStatusArrayCapture(value));return true;},
encode(value){const owned=Ros2Kuka_driver_interfacesMsgKssStatusArrayCapture(value),n=Ros2Kuka_driver_interfacesMsgKssStatusArraySize(owned),w=new Writer(n);Ros2Kuka_driver_interfacesMsgKssStatusArrayWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1240,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Kuka_driver_interfacesMsgKssStatusArrayRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Kuka_driver_interfacesMsgKssStatusArraySize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["robot_names"])||value["robot_names"].length>16)bad();
n=addSize(n,4);
for(const element of value["robot_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["statuses"])||value["statuses"].length>16)bad();
n=addSize(n,4);
for(const element of value["statuses"]){
n=addSize(n,Ros2Kuka_driver_interfacesMsgKssStatusSize(element));
}
if(n>1240)bad(90);return n;}
function Ros2Kuka_driver_interfacesMsgKssStatusArrayWrite(value,w){
w.v.setUint32(w.at,value["robot_names"].length,true);w.at+=4;
for(const element of value["robot_names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["statuses"].length,true);w.at+=4;
for(const element of value["statuses"]){
Ros2Kuka_driver_interfacesMsgKssStatusWrite(element,w);
}
}
function Ros2Kuka_driver_interfacesMsgKssStatusArrayRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["robot_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["robot_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["statuses"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Kuka_driver_interfacesMsgKssStatusRead(r);
value["statuses"].push(decoded);
}}
}
return value;}
function Ros2Kuka_driver_interfacesMsgKssStatusArrayCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robot_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["robot_names"]=values;
}
{const field=value["statuses"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Kuka_driver_interfacesMsgKssStatusCapture(element);
}out["statuses"]=values;
}
return out;}
/** Complete ROS2 lifecycle_msgs/msg/State codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsMsgStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ebd49e5f87ddaa204cc4072ceef9840676c6f65f67448a027e26570dae0ded94",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({"PRIMARY_STATE_UNKNOWN":0,"PRIMARY_STATE_UNCONFIGURED":1,"PRIMARY_STATE_INACTIVE":2,"PRIMARY_STATE_ACTIVE":3,"PRIMARY_STATE_FINALIZED":4,"TRANSITION_STATE_CONFIGURING":10,"TRANSITION_STATE_CLEANINGUP":11,"TRANSITION_STATE_SHUTTINGDOWN":12,"TRANSITION_STATE_ACTIVATING":13,"TRANSITION_STATE_DEACTIVATING":14,"TRANSITION_STATE_ERRORPROCESSING":15}),
defaultValue(){return {"id":0,"label":""};},
validateValue(value){Ros2Lifecycle_msgsMsgStateSize(Ros2Lifecycle_msgsMsgStateCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsMsgStateCapture(value),n=Ros2Lifecycle_msgsMsgStateSize(owned),w=new Writer(n);Ros2Lifecycle_msgsMsgStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsMsgStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsMsgStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["label"],512).length);
if(n>517)bad(90);return n;}
function Ros2Lifecycle_msgsMsgStateWrite(value,w){
w.v.setUint8(w.at,value["id"],true);w.at+=1;
w.text(value["label"],512);
}
function Ros2Lifecycle_msgsMsgStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["id"]=decoded;
}
{
const decoded=r.text(512);
value["label"]=decoded;
}
return value;}
function Ros2Lifecycle_msgsMsgStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["id"];
out["id"]=field;
}
{const field=value["label"];
out["label"]=field;
}
return out;}
/** Complete ROS2 lifecycle_msgs/msg/Transition codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsMsgTransitionCodec=Object.freeze({codecIdentity:"UWVMROS1_1f9bb864622854b446a37cf3c6e29e9cf41f8efa37375c5a56205b4c02ed10cb",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({"TRANSITION_CREATE":0,"TRANSITION_CONFIGURE":1,"TRANSITION_CLEANUP":2,"TRANSITION_ACTIVATE":3,"TRANSITION_DEACTIVATE":4,"TRANSITION_UNCONFIGURED_SHUTDOWN":5,"TRANSITION_INACTIVE_SHUTDOWN":6,"TRANSITION_ACTIVE_SHUTDOWN":7,"TRANSITION_DESTROY":8,"TRANSITION_ON_CONFIGURE_SUCCESS":10,"TRANSITION_ON_CONFIGURE_FAILURE":11,"TRANSITION_ON_CONFIGURE_ERROR":12,"TRANSITION_ON_CLEANUP_SUCCESS":20,"TRANSITION_ON_CLEANUP_FAILURE":21,"TRANSITION_ON_CLEANUP_ERROR":22,"TRANSITION_ON_ACTIVATE_SUCCESS":30,"TRANSITION_ON_ACTIVATE_FAILURE":31,"TRANSITION_ON_ACTIVATE_ERROR":32,"TRANSITION_ON_DEACTIVATE_SUCCESS":40,"TRANSITION_ON_DEACTIVATE_FAILURE":41,"TRANSITION_ON_DEACTIVATE_ERROR":42,"TRANSITION_ON_SHUTDOWN_SUCCESS":50,"TRANSITION_ON_SHUTDOWN_FAILURE":51,"TRANSITION_ON_SHUTDOWN_ERROR":52,"TRANSITION_ON_ERROR_SUCCESS":60,"TRANSITION_ON_ERROR_FAILURE":61,"TRANSITION_ON_ERROR_ERROR":62,"TRANSITION_CALLBACK_SUCCESS":97,"TRANSITION_CALLBACK_FAILURE":98,"TRANSITION_CALLBACK_ERROR":99}),
defaultValue(){return {"id":0,"label":""};},
validateValue(value){Ros2Lifecycle_msgsMsgTransitionSize(Ros2Lifecycle_msgsMsgTransitionCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsMsgTransitionCapture(value),n=Ros2Lifecycle_msgsMsgTransitionSize(owned),w=new Writer(n);Ros2Lifecycle_msgsMsgTransitionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsMsgTransitionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsMsgTransitionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["label"],512).length);
if(n>517)bad(90);return n;}
function Ros2Lifecycle_msgsMsgTransitionWrite(value,w){
w.v.setUint8(w.at,value["id"],true);w.at+=1;
w.text(value["label"],512);
}
function Ros2Lifecycle_msgsMsgTransitionRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["id"]=decoded;
}
{
const decoded=r.text(512);
value["label"]=decoded;
}
return value;}
function Ros2Lifecycle_msgsMsgTransitionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["id"];
out["id"]=field;
}
{const field=value["label"];
out["label"]=field;
}
return out;}
/** Complete ROS2 lifecycle_msgs/srv/ChangeState_Request codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsSrvChangeState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b04270f03616b8e581036d327d18bde62d38627ae8acf88fb77d0ed7b3b3d3d1",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"transition":Ros2Lifecycle_msgsMsgTransitionCodec.defaultValue()};},
validateValue(value){Ros2Lifecycle_msgsSrvChangeState_RequestSize(Ros2Lifecycle_msgsSrvChangeState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsSrvChangeState_RequestCapture(value),n=Ros2Lifecycle_msgsSrvChangeState_RequestSize(owned),w=new Writer(n);Ros2Lifecycle_msgsSrvChangeState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsSrvChangeState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsSrvChangeState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Lifecycle_msgsMsgTransitionSize(value["transition"]));
if(n>517)bad(90);return n;}
function Ros2Lifecycle_msgsSrvChangeState_RequestWrite(value,w){
Ros2Lifecycle_msgsMsgTransitionWrite(value["transition"],w);
}
function Ros2Lifecycle_msgsSrvChangeState_RequestRead(r){const value={};
{
const decoded=Ros2Lifecycle_msgsMsgTransitionRead(r);
value["transition"]=decoded;
}
return value;}
function Ros2Lifecycle_msgsSrvChangeState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["transition"];
out["transition"]=Ros2Lifecycle_msgsMsgTransitionCapture(field);
}
return out;}
/** Complete ROS2 lifecycle_msgs/srv/ChangeState_Response codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsSrvChangeState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d2dc541d6995976a2a1a2c03d499aa9dccaf487736b11704d778c86fddaf5032",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false};},
validateValue(value){Ros2Lifecycle_msgsSrvChangeState_ResponseSize(Ros2Lifecycle_msgsSrvChangeState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsSrvChangeState_ResponseCapture(value),n=Ros2Lifecycle_msgsSrvChangeState_ResponseSize(owned),w=new Writer(n);Ros2Lifecycle_msgsSrvChangeState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsSrvChangeState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsSrvChangeState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Lifecycle_msgsSrvChangeState_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Lifecycle_msgsSrvChangeState_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Lifecycle_msgsSrvChangeState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 lifecycle_msgs/srv/GetState_Request codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsSrvGetState_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_86a3d813de32971a271e062acfaf201eb718aff8d68a4e343d6fceaae2b5098d",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Lifecycle_msgsSrvGetState_RequestSize(Ros2Lifecycle_msgsSrvGetState_RequestCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsSrvGetState_RequestCapture(value),n=Ros2Lifecycle_msgsSrvGetState_RequestSize(owned),w=new Writer(n);Ros2Lifecycle_msgsSrvGetState_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsSrvGetState_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsSrvGetState_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Lifecycle_msgsSrvGetState_RequestWrite(value,w){
}
function Ros2Lifecycle_msgsSrvGetState_RequestRead(r){const value={};
return value;}
function Ros2Lifecycle_msgsSrvGetState_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 lifecycle_msgs/srv/GetState_Response codec; maxima are not actual lengths. */
export const Ros2Lifecycle_msgsSrvGetState_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_697418ec4095e98c855b1f9d238a3bd74a59ccbbc3eb31981c4df3c3b829ea39",maxWireBytes:517,minWireBytes:5,cartesianMaxWireBytes:517n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current_state":Ros2Lifecycle_msgsMsgStateCodec.defaultValue()};},
validateValue(value){Ros2Lifecycle_msgsSrvGetState_ResponseSize(Ros2Lifecycle_msgsSrvGetState_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Lifecycle_msgsSrvGetState_ResponseCapture(value),n=Ros2Lifecycle_msgsSrvGetState_ResponseSize(owned),w=new Writer(n);Ros2Lifecycle_msgsSrvGetState_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,517,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Lifecycle_msgsSrvGetState_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Lifecycle_msgsSrvGetState_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Lifecycle_msgsMsgStateSize(value["current_state"]));
if(n>517)bad(90);return n;}
function Ros2Lifecycle_msgsSrvGetState_ResponseWrite(value,w){
Ros2Lifecycle_msgsMsgStateWrite(value["current_state"],w);
}
function Ros2Lifecycle_msgsSrvGetState_ResponseRead(r){const value={};
{
const decoded=Ros2Lifecycle_msgsMsgStateRead(r);
value["current_state"]=decoded;
}
return value;}
function Ros2Lifecycle_msgsSrvGetState_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["current_state"];
out["current_state"]=Ros2Lifecycle_msgsMsgStateCapture(field);
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
