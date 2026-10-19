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
/** Complete ROS2 geometry_msgs/msg/TransformStamped codec; maxima are not actual lengths. */
export const Ros2Geometry_msgsMsgTransformStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_be553b5a9c73b687cde56ff1c381e3c67168fd2c0366830be3c65382c5bbd792",maxWireBytes:648,minWireBytes:72,cartesianMaxWireBytes:648n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"child_frame_id":"","transform":Ros2Geometry_msgsMsgTransformCodec.defaultValue()};},
validateValue(value){Ros2Geometry_msgsMsgTransformStampedSize(Ros2Geometry_msgsMsgTransformStampedCapture(value));return true;},
encode(value){const owned=Ros2Geometry_msgsMsgTransformStampedCapture(value),n=Ros2Geometry_msgsMsgTransformStampedSize(owned),w=new Writer(n);Ros2Geometry_msgsMsgTransformStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,648,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros2Geometry_msgsMsgTransformStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Geometry_msgsMsgTransformStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["child_frame_id"],512).length);
n=addSize(n,Ros2Geometry_msgsMsgTransformSize(value["transform"]));
if(n>648)bad(90);return n;}
function Ros2Geometry_msgsMsgTransformStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["child_frame_id"],512);
Ros2Geometry_msgsMsgTransformWrite(value["transform"],w);
}
function Ros2Geometry_msgsMsgTransformStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(512);
value["child_frame_id"]=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTransformRead(r);
value["transform"]=decoded;
}
return value;}
function Ros2Geometry_msgsMsgTransformStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["child_frame_id"];
out["child_frame_id"]=field;
}
{const field=value["transform"];
out["transform"]=Ros2Geometry_msgsMsgTransformCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/msg/DebugLevel codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgDebugLevelCodec=Object.freeze({codecIdentity:"UWVMROS1_989475cc5d981d8c5ab91fb091235871f7a1c7d87d246eb2250086e6c3c37c79",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"DEBUG":5,"INFO":4,"WARN":3,"ERROR":2,"FATAL":1,"NONE":0}),
defaultValue(){return {"val":0};},
validateValue(value){Ros2Industrial_msgsMsgDebugLevelSize(Ros2Industrial_msgsMsgDebugLevelCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgDebugLevelCapture(value),n=Ros2Industrial_msgsMsgDebugLevelSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgDebugLevelWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgDebugLevelRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgDebugLevelSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["val"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsMsgDebugLevelWrite(value,w){
w.v.setUint8(w.at,value["val"],true);w.at+=1;
}
function Ros2Industrial_msgsMsgDebugLevelRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["val"]=decoded;
}
return value;}
function Ros2Industrial_msgsMsgDebugLevelCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["val"];
out["val"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/msg/DeviceInfo codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgDeviceInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_f9a5962800e17495f34dbb73f37373df22f900e528c630497417f78cdecb283b",maxWireBytes:2580,minWireBytes:20,cartesianMaxWireBytes:2580n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"model":"","serial_number":"","hw_version":"","sw_version":"","address":""};},
validateValue(value){Ros2Industrial_msgsMsgDeviceInfoSize(Ros2Industrial_msgsMsgDeviceInfoCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgDeviceInfoCapture(value),n=Ros2Industrial_msgsMsgDeviceInfoSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgDeviceInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2580,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgDeviceInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgDeviceInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["model"],512).length);
n=addSize(n,4+textBytes(value["serial_number"],512).length);
n=addSize(n,4+textBytes(value["hw_version"],512).length);
n=addSize(n,4+textBytes(value["sw_version"],512).length);
n=addSize(n,4+textBytes(value["address"],512).length);
if(n>2580)bad(90);return n;}
function Ros2Industrial_msgsMsgDeviceInfoWrite(value,w){
w.text(value["model"],512);
w.text(value["serial_number"],512);
w.text(value["hw_version"],512);
w.text(value["sw_version"],512);
w.text(value["address"],512);
}
function Ros2Industrial_msgsMsgDeviceInfoRead(r){const value={};
{
const decoded=r.text(512);
value["model"]=decoded;
}
{
const decoded=r.text(512);
value["serial_number"]=decoded;
}
{
const decoded=r.text(512);
value["hw_version"]=decoded;
}
{
const decoded=r.text(512);
value["sw_version"]=decoded;
}
{
const decoded=r.text(512);
value["address"]=decoded;
}
return value;}
function Ros2Industrial_msgsMsgDeviceInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["model"];
out["model"]=field;
}
{const field=value["serial_number"];
out["serial_number"]=field;
}
{const field=value["hw_version"];
out["hw_version"]=field;
}
{const field=value["sw_version"];
out["sw_version"]=field;
}
{const field=value["address"];
out["address"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/msg/RobotMode codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgRobotModeCodec=Object.freeze({codecIdentity:"UWVMROS1_4f9173c5e27815ca33be603b8645a22420bda3ab308962127060d2413d092c4c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"UNKNOWN":-1,"MANUAL":1,"AUTO":2}),
defaultValue(){return {"val":0};},
validateValue(value){Ros2Industrial_msgsMsgRobotModeSize(Ros2Industrial_msgsMsgRobotModeCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgRobotModeCapture(value),n=Ros2Industrial_msgsMsgRobotModeSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgRobotModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgRobotModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgRobotModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["val"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsMsgRobotModeWrite(value,w){
w.v.setInt8(w.at,value["val"],true);w.at+=1;
}
function Ros2Industrial_msgsMsgRobotModeRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["val"]=decoded;
}
return value;}
function Ros2Industrial_msgsMsgRobotModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["val"];
out["val"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/msg/TriState codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgTriStateCodec=Object.freeze({codecIdentity:"UWVMROS1_b7ae62ce19d642ad46e962ffb50cd76c43c96ac673ac36d690f99c1dd8d2a841",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"UNKNOWN":-1,"TRUE":1,"ON":1,"ENABLED":1,"HIGH":1,"CLOSED":1,"FALSE":0,"OFF":0,"DISABLED":0,"LOW":0,"OPEN":0}),
defaultValue(){return {"val":0};},
validateValue(value){Ros2Industrial_msgsMsgTriStateSize(Ros2Industrial_msgsMsgTriStateCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgTriStateCapture(value),n=Ros2Industrial_msgsMsgTriStateSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgTriStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgTriStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgTriStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["val"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsMsgTriStateWrite(value,w){
w.v.setInt8(w.at,value["val"],true);w.at+=1;
}
function Ros2Industrial_msgsMsgTriStateRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["val"]=decoded;
}
return value;}
function Ros2Industrial_msgsMsgTriStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["val"];
out["val"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/msg/RobotStatus codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgRobotStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_1086f16ba5d614f3d87c3ab195240b5a0683ef490f2e92434e4ea3bdbe7554ba",maxWireBytes:150,minWireBytes:22,cartesianMaxWireBytes:150n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"mode":Ros2Industrial_msgsMsgRobotModeCodec.defaultValue(),"e_stopped":Ros2Industrial_msgsMsgTriStateCodec.defaultValue(),"drives_powered":Ros2Industrial_msgsMsgTriStateCodec.defaultValue(),"motion_possible":Ros2Industrial_msgsMsgTriStateCodec.defaultValue(),"in_motion":Ros2Industrial_msgsMsgTriStateCodec.defaultValue(),"in_error":Ros2Industrial_msgsMsgTriStateCodec.defaultValue(),"error_codes":[]};},
validateValue(value){Ros2Industrial_msgsMsgRobotStatusSize(Ros2Industrial_msgsMsgRobotStatusCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgRobotStatusCapture(value),n=Ros2Industrial_msgsMsgRobotStatusSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgRobotStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,150,90);if(owned.length<22)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgRobotStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgRobotStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,Ros2Industrial_msgsMsgRobotModeSize(value["mode"]));
n=addSize(n,Ros2Industrial_msgsMsgTriStateSize(value["e_stopped"]));
n=addSize(n,Ros2Industrial_msgsMsgTriStateSize(value["drives_powered"]));
n=addSize(n,Ros2Industrial_msgsMsgTriStateSize(value["motion_possible"]));
n=addSize(n,Ros2Industrial_msgsMsgTriStateSize(value["in_motion"]));
n=addSize(n,Ros2Industrial_msgsMsgTriStateSize(value["in_error"]));
if(!Array.isArray(value["error_codes"])||value["error_codes"].length>16)bad();
n=addSize(n,4);
for(const element of value["error_codes"]){
integer(element,4,true);n=addSize(n,4);
}
if(n>150)bad(90);return n;}
function Ros2Industrial_msgsMsgRobotStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
Ros2Industrial_msgsMsgRobotModeWrite(value["mode"],w);
Ros2Industrial_msgsMsgTriStateWrite(value["e_stopped"],w);
Ros2Industrial_msgsMsgTriStateWrite(value["drives_powered"],w);
Ros2Industrial_msgsMsgTriStateWrite(value["motion_possible"],w);
Ros2Industrial_msgsMsgTriStateWrite(value["in_motion"],w);
Ros2Industrial_msgsMsgTriStateWrite(value["in_error"],w);
w.v.setUint32(w.at,value["error_codes"].length,true);w.at+=4;
for(const element of value["error_codes"]){
w.v.setInt32(w.at,element,true);w.at+=4;
}
}
function Ros2Industrial_msgsMsgRobotStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgRobotModeRead(r);
value["mode"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriStateRead(r);
value["e_stopped"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriStateRead(r);
value["drives_powered"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriStateRead(r);
value["motion_possible"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriStateRead(r);
value["in_motion"]=decoded;
}
{
const decoded=Ros2Industrial_msgsMsgTriStateRead(r);
value["in_error"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["error_codes"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt32(r.take(4),true);
value["error_codes"].push(decoded);
}}
}
return value;}
function Ros2Industrial_msgsMsgRobotStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["mode"];
out["mode"]=Ros2Industrial_msgsMsgRobotModeCapture(field);
}
{const field=value["e_stopped"];
out["e_stopped"]=Ros2Industrial_msgsMsgTriStateCapture(field);
}
{const field=value["drives_powered"];
out["drives_powered"]=Ros2Industrial_msgsMsgTriStateCapture(field);
}
{const field=value["motion_possible"];
out["motion_possible"]=Ros2Industrial_msgsMsgTriStateCapture(field);
}
{const field=value["in_motion"];
out["in_motion"]=Ros2Industrial_msgsMsgTriStateCapture(field);
}
{const field=value["in_error"];
out["in_error"]=Ros2Industrial_msgsMsgTriStateCapture(field);
}
{const field=value["error_codes"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["error_codes"]=values;
}
return out;}
/** Complete ROS2 industrial_msgs/msg/ServiceReturnCode codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsMsgServiceReturnCodeCodec=Object.freeze({codecIdentity:"UWVMROS1_62eb4415a63d7686f16ea343193238838c78aa8e9b9786a65c92fb227e03d3e5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESS":1,"FAILURE":-1}),
defaultValue(){return {"val":0};},
validateValue(value){Ros2Industrial_msgsMsgServiceReturnCodeSize(Ros2Industrial_msgsMsgServiceReturnCodeCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsMsgServiceReturnCodeCapture(value),n=Ros2Industrial_msgsMsgServiceReturnCodeSize(owned),w=new Writer(n);Ros2Industrial_msgsMsgServiceReturnCodeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsMsgServiceReturnCodeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["val"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsMsgServiceReturnCodeWrite(value,w){
w.v.setInt8(w.at,value["val"],true);w.at+=1;
}
function Ros2Industrial_msgsMsgServiceReturnCodeRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["val"]=decoded;
}
return value;}
function Ros2Industrial_msgsMsgServiceReturnCodeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["val"];
out["val"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/srv/CmdJointTrajectory_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1def3934b243546a4591b7be1ef2eb27002fbd9d3725302016e3b1846805149d",maxWireBytes:4164,minWireBytes:20,cartesianMaxWireBytes:4164n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trajectory":Ros2Trajectory_msgsMsgJointTrajectoryCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvCmdJointTrajectory_RequestSize(Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCapture(value),n=Ros2Industrial_msgsSrvCmdJointTrajectory_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvCmdJointTrajectory_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4164,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvCmdJointTrajectory_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvCmdJointTrajectory_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectorySize(value["trajectory"]));
if(n>4164)bad(90);return n;}
function Ros2Industrial_msgsSrvCmdJointTrajectory_RequestWrite(value,w){
Ros2Trajectory_msgsMsgJointTrajectoryWrite(value["trajectory"],w);
}
function Ros2Industrial_msgsSrvCmdJointTrajectory_RequestRead(r){const value={};
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryRead(r);
value["trajectory"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trajectory"];
out["trajectory"]=Ros2Trajectory_msgsMsgJointTrajectoryCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/CmdJointTrajectory_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1e4d064ef659e5d1cc73bdd90f82657a16087c96e0f75b632e2be2be884bca8b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseSize(Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCapture(value),n=Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseWrite(value,w){
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/GetRobotInfo_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvGetRobotInfo_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_688a272f6ee39921013c6c39a7b0b0c4e2cda5119787088234103518ba25c1d2",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Industrial_msgsSrvGetRobotInfo_RequestSize(Ros2Industrial_msgsSrvGetRobotInfo_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvGetRobotInfo_RequestCapture(value),n=Ros2Industrial_msgsSrvGetRobotInfo_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvGetRobotInfo_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvGetRobotInfo_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvGetRobotInfo_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Industrial_msgsSrvGetRobotInfo_RequestWrite(value,w){
}
function Ros2Industrial_msgsSrvGetRobotInfo_RequestRead(r){const value={};
return value;}
function Ros2Industrial_msgsSrvGetRobotInfo_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 industrial_msgs/srv/GetRobotInfo_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvGetRobotInfo_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6d37cdcd8afc918329d7856e682d7fcdae2931832474a9194143c21079ed4152",maxWireBytes:32768,minWireBytes:25,cartesianMaxWireBytes:43865n,aggregateWireBytes:32768,constants:Object.freeze({}),
defaultValue(){return {"controller":Ros2Industrial_msgsMsgDeviceInfoCodec.defaultValue(),"robots":[],"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvGetRobotInfo_ResponseSize(Ros2Industrial_msgsSrvGetRobotInfo_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvGetRobotInfo_ResponseCapture(value),n=Ros2Industrial_msgsSrvGetRobotInfo_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvGetRobotInfo_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32768,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvGetRobotInfo_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvGetRobotInfo_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgDeviceInfoSize(value["controller"]));
if(!Array.isArray(value["robots"])||value["robots"].length>16)bad();
n=addSize(n,4);
for(const element of value["robots"]){
n=addSize(n,Ros2Industrial_msgsMsgDeviceInfoSize(element));
}
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>32768)bad(90);return n;}
function Ros2Industrial_msgsSrvGetRobotInfo_ResponseWrite(value,w){
Ros2Industrial_msgsMsgDeviceInfoWrite(value["controller"],w);
w.v.setUint32(w.at,value["robots"].length,true);w.at+=4;
for(const element of value["robots"]){
Ros2Industrial_msgsMsgDeviceInfoWrite(element,w);
}
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvGetRobotInfo_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgDeviceInfoRead(r);
value["controller"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["robots"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Industrial_msgsMsgDeviceInfoRead(r);
value["robots"].push(decoded);
}}
}
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvGetRobotInfo_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["controller"];
out["controller"]=Ros2Industrial_msgsMsgDeviceInfoCapture(field);
}
{const field=value["robots"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Industrial_msgsMsgDeviceInfoCapture(element);
}out["robots"]=values;
}
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/SetDrivePower_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvSetDrivePower_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b4fc5e109e131a97820ab5856b34839c572ac0b0c256d70457c176e401844f96",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"drive_power":false};},
validateValue(value){Ros2Industrial_msgsSrvSetDrivePower_RequestSize(Ros2Industrial_msgsSrvSetDrivePower_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvSetDrivePower_RequestCapture(value),n=Ros2Industrial_msgsSrvSetDrivePower_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvSetDrivePower_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvSetDrivePower_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvSetDrivePower_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["drive_power"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvSetDrivePower_RequestWrite(value,w){
w.v.setUint8(w.at,value["drive_power"]?1:0,true);w.at+=1;
}
function Ros2Industrial_msgsSrvSetDrivePower_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["drive_power"]=decoded!==0;
}
return value;}
function Ros2Industrial_msgsSrvSetDrivePower_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["drive_power"];
out["drive_power"]=field;
}
return out;}
/** Complete ROS2 industrial_msgs/srv/SetDrivePower_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvSetDrivePower_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_050fb9a727ebedd240e9023b9a2379a5d72e0d9fa335bead0c40d783abb22cff",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvSetDrivePower_ResponseSize(Ros2Industrial_msgsSrvSetDrivePower_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvSetDrivePower_ResponseCapture(value),n=Ros2Industrial_msgsSrvSetDrivePower_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvSetDrivePower_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvSetDrivePower_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvSetDrivePower_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvSetDrivePower_ResponseWrite(value,w){
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvSetDrivePower_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvSetDrivePower_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bbadd39c05cab90b14f4b1fad23ff05ac83325f6cc57810cd649e04fa29fe014",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"level":Ros2Industrial_msgsMsgDebugLevelCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestSize(Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCapture(value),n=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgDebugLevelSize(value["level"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestWrite(value,w){
Ros2Industrial_msgsMsgDebugLevelWrite(value["level"],w);
}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgDebugLevelRead(r);
value["level"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["level"];
out["level"]=Ros2Industrial_msgsMsgDebugLevelCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b8681d031a4bb5a67b842277af2e2156e8339bdef23be88d2b64e296a7e3d63b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseSize(Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCapture(value),n=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseWrite(value,w){
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/StartMotion_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvStartMotion_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0a96fb9489f863612a91bf82f995f2f28bcb7fb403bde74c617925c789e4dcd8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Industrial_msgsSrvStartMotion_RequestSize(Ros2Industrial_msgsSrvStartMotion_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvStartMotion_RequestCapture(value),n=Ros2Industrial_msgsSrvStartMotion_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvStartMotion_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvStartMotion_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvStartMotion_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Industrial_msgsSrvStartMotion_RequestWrite(value,w){
}
function Ros2Industrial_msgsSrvStartMotion_RequestRead(r){const value={};
return value;}
function Ros2Industrial_msgsSrvStartMotion_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 industrial_msgs/srv/StartMotion_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvStartMotion_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3bb3650fbe638a6d300bcd9fcd5acf574bd54d00a7368edd3014bf44dcf66c16",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvStartMotion_ResponseSize(Ros2Industrial_msgsSrvStartMotion_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvStartMotion_ResponseCapture(value),n=Ros2Industrial_msgsSrvStartMotion_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvStartMotion_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvStartMotion_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvStartMotion_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvStartMotion_ResponseWrite(value,w){
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvStartMotion_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvStartMotion_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 industrial_msgs/srv/StopMotion_Request codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvStopMotion_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fe5a7cf0da30270eab5cfb1ab8c1fbd37d2b4c4f91ef29662a0ddcac39e6ef49",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Industrial_msgsSrvStopMotion_RequestSize(Ros2Industrial_msgsSrvStopMotion_RequestCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvStopMotion_RequestCapture(value),n=Ros2Industrial_msgsSrvStopMotion_RequestSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvStopMotion_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvStopMotion_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvStopMotion_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Industrial_msgsSrvStopMotion_RequestWrite(value,w){
}
function Ros2Industrial_msgsSrvStopMotion_RequestRead(r){const value={};
return value;}
function Ros2Industrial_msgsSrvStopMotion_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 industrial_msgs/srv/StopMotion_Response codec; maxima are not actual lengths. */
export const Ros2Industrial_msgsSrvStopMotion_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c26bda240d86becc5b8b4ea80380eba3602092a5ec4a06818dbfbd38c24015b5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":Ros2Industrial_msgsMsgServiceReturnCodeCodec.defaultValue()};},
validateValue(value){Ros2Industrial_msgsSrvStopMotion_ResponseSize(Ros2Industrial_msgsSrvStopMotion_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Industrial_msgsSrvStopMotion_ResponseCapture(value),n=Ros2Industrial_msgsSrvStopMotion_ResponseSize(owned),w=new Writer(n);Ros2Industrial_msgsSrvStopMotion_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Industrial_msgsSrvStopMotion_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Industrial_msgsSrvStopMotion_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Industrial_msgsMsgServiceReturnCodeSize(value["code"]));
if(n>1)bad(90);return n;}
function Ros2Industrial_msgsSrvStopMotion_ResponseWrite(value,w){
Ros2Industrial_msgsMsgServiceReturnCodeWrite(value["code"],w);
}
function Ros2Industrial_msgsSrvStopMotion_ResponseRead(r){const value={};
{
const decoded=Ros2Industrial_msgsMsgServiceReturnCodeRead(r);
value["code"]=decoded;
}
return value;}
function Ros2Industrial_msgsSrvStopMotion_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=Ros2Industrial_msgsMsgServiceReturnCodeCapture(field);
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/AlarmCauseRemedy codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgAlarmCauseRemedyCodec=Object.freeze({codecIdentity:"UWVMROS1_f977609022f4ca593b9089d0038d1c28d1bc4b533e272f38c3379495f326ddf5",maxWireBytes:1032,minWireBytes:8,cartesianMaxWireBytes:1032n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cause":"","remedy":""};},
validateValue(value){Ros2Motoros2_interfacesMsgAlarmCauseRemedySize(Ros2Motoros2_interfacesMsgAlarmCauseRemedyCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgAlarmCauseRemedyCapture(value),n=Ros2Motoros2_interfacesMsgAlarmCauseRemedySize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgAlarmCauseRemedyWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1032,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgAlarmCauseRemedyRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgAlarmCauseRemedySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["cause"],512).length);
n=addSize(n,4+textBytes(value["remedy"],512).length);
if(n>1032)bad(90);return n;}
function Ros2Motoros2_interfacesMsgAlarmCauseRemedyWrite(value,w){
w.text(value["cause"],512);
w.text(value["remedy"],512);
}
function Ros2Motoros2_interfacesMsgAlarmCauseRemedyRead(r){const value={};
{
const decoded=r.text(512);
value["cause"]=decoded;
}
{
const decoded=r.text(512);
value["remedy"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgAlarmCauseRemedyCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cause"];
out["cause"]=field;
}
{const field=value["remedy"];
out["remedy"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/AlarmInfo codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgAlarmInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_7cf3fa3037f28f295dd62009e708ea3e46a80f83471d0a0f9fb55af125c0c27f",maxWireBytes:25248,minWireBytes:32,cartesianMaxWireBytes:25248n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"number":0,"message":"","sub_code":0,"datetime":Ros2Builtin_interfacesMsgTimeCodec.defaultValue(),"contents":"","description":"","help":[]};},
validateValue(value){Ros2Motoros2_interfacesMsgAlarmInfoSize(Ros2Motoros2_interfacesMsgAlarmInfoCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgAlarmInfoCapture(value),n=Ros2Motoros2_interfacesMsgAlarmInfoSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgAlarmInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25248,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgAlarmInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgAlarmInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["number"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
integer(value["sub_code"],4,true);n=addSize(n,4);
n=addSize(n,Ros2Builtin_interfacesMsgTimeSize(value["datetime"]));
n=addSize(n,4+textBytes(value["contents"],4096).length);
n=addSize(n,4+textBytes(value["description"],4096).length);
if(!Array.isArray(value["help"])||value["help"].length>16)bad();
n=addSize(n,4);
for(const element of value["help"]){
n=addSize(n,Ros2Motoros2_interfacesMsgAlarmCauseRemedySize(element));
}
if(n>25248)bad(90);return n;}
function Ros2Motoros2_interfacesMsgAlarmInfoWrite(value,w){
w.v.setUint32(w.at,value["number"],true);w.at+=4;
w.text(value["message"],512);
w.v.setInt32(w.at,value["sub_code"],true);w.at+=4;
Ros2Builtin_interfacesMsgTimeWrite(value["datetime"],w);
w.text(value["contents"],4096);
w.text(value["description"],4096);
w.v.setUint32(w.at,value["help"].length,true);w.at+=4;
for(const element of value["help"]){
Ros2Motoros2_interfacesMsgAlarmCauseRemedyWrite(element,w);
}
}
function Ros2Motoros2_interfacesMsgAlarmInfoRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["number"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sub_code"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgTimeRead(r);
value["datetime"]=decoded;
}
{
const decoded=r.text(4096);
value["contents"]=decoded;
}
{
const decoded=r.text(4096);
value["description"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["help"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgAlarmCauseRemedyRead(r);
value["help"].push(decoded);
}}
}
return value;}
function Ros2Motoros2_interfacesMsgAlarmInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["number"];
out["number"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["sub_code"];
out["sub_code"]=field;
}
{const field=value["datetime"];
out["datetime"]=Ros2Builtin_interfacesMsgTimeCapture(field);
}
{const field=value["contents"];
out["contents"]=field;
}
{const field=value["description"];
out["description"]=field;
}
{const field=value["help"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Motoros2_interfacesMsgAlarmCauseRemedyCapture(element);
}out["help"]=values;
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/ErrorInfo codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgErrorInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_1cfe60c0af6bc157db84c4d9e0b20a8f2e5c78fd4afd6908d32b3217f3734b67",maxWireBytes:532,minWireBytes:20,cartesianMaxWireBytes:532n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"number":0,"message":"","sub_code":0,"datetime":Ros2Builtin_interfacesMsgTimeCodec.defaultValue()};},
validateValue(value){Ros2Motoros2_interfacesMsgErrorInfoSize(Ros2Motoros2_interfacesMsgErrorInfoCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgErrorInfoCapture(value),n=Ros2Motoros2_interfacesMsgErrorInfoSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgErrorInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,532,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgErrorInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgErrorInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["number"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
integer(value["sub_code"],4,true);n=addSize(n,4);
n=addSize(n,Ros2Builtin_interfacesMsgTimeSize(value["datetime"]));
if(n>532)bad(90);return n;}
function Ros2Motoros2_interfacesMsgErrorInfoWrite(value,w){
w.v.setUint32(w.at,value["number"],true);w.at+=4;
w.text(value["message"],512);
w.v.setInt32(w.at,value["sub_code"],true);w.at+=4;
Ros2Builtin_interfacesMsgTimeWrite(value["datetime"],w);
}
function Ros2Motoros2_interfacesMsgErrorInfoRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["number"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["sub_code"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgTimeRead(r);
value["datetime"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgErrorInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["number"];
out["number"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["sub_code"];
out["sub_code"]=field;
}
{const field=value["datetime"];
out["datetime"]=Ros2Builtin_interfacesMsgTimeCapture(field);
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/InformJobCrudResultCodes codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgInformJobCrudResultCodesCodec=Object.freeze({codecIdentity:"UWVMROS1_f5ed037b83926b48a88fa486ed8db5f5d3e39e4a486d4a90190ffd6a70452613",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({"RC_OK":1,"STR_OK":"Success","RC_ERR_REFRESHING_JOB_LIST":10,"STR_ERR_REFRESHING_JOB_LIST":"Error refreshing job list","RC_ERR_RETRIEVING_FILE_COUNT":11,"STR_ERR_RETRIEVING_FILE_COUNT":"Error retrieving file count","RC_ERR_TOO_MANY_JOBS":12,"STR_ERR_TOO_MANY_JOBS":"Too many jobs","RC_ERR_RETRIEVING_JOB_NAME_FROM_LIST":13,"STR_ERR_RETRIEVING_JOB_NAME_FROM_LIST":"Failed retrieving job name from list","RC_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE":14,"STR_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE":"Error adding job name to service response"}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesMsgInformJobCrudResultCodesSize(Ros2Motoros2_interfacesMsgInformJobCrudResultCodesCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgInformJobCrudResultCodesCapture(value),n=Ros2Motoros2_interfacesMsgInformJobCrudResultCodesSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgInformJobCrudResultCodesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgInformJobCrudResultCodesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgInformJobCrudResultCodesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesMsgInformJobCrudResultCodesWrite(value,w){
}
function Ros2Motoros2_interfacesMsgInformJobCrudResultCodesRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesMsgInformJobCrudResultCodesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/msg/InitTrajEnum codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgInitTrajEnumCodec=Object.freeze({codecIdentity:"UWVMROS1_9279979f9e3570f48d6e23bf812a4680e3deed9af3309019efdff5464d7b7763",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({"INIT_TRAJ_OK":0,"INIT_TRAJ_OK_STR":"OK","INIT_TRAJ_UNSPECIFIED":200,"INIT_TRAJ_UNSPECIFIED_STR":"Unspecified failure","INIT_TRAJ_TOO_SMALL":201,"INIT_TRAJ_TOO_SMALL_STR":"Trajectory must contain at least two points.","INIT_TRAJ_TOO_BIG":202,"INIT_TRAJ_TOO_BIG_STR":"Trajectory contains too many points (Not enough memory).","INIT_TRAJ_ALREADY_IN_MOTION":203,"INIT_TRAJ_ALREADY_IN_MOTION_STR":"Already running a trajectory.","INIT_TRAJ_INVALID_STARTING_POS":204,"INIT_TRAJ_INVALID_STARTING_POS_STR":"The first point must match the robot's current position.","INIT_TRAJ_INVALID_VELOCITY":205,"INIT_TRAJ_INVALID_VELOCITY_STR":"The commanded velocity is too high.","INIT_TRAJ_INVALID_JOINTNAME":206,"INIT_TRAJ_INVALID_JOINTNAME_STR":"Invalid joint name specified. Check motoros2_config.yaml.","INIT_TRAJ_INCOMPLETE_JOINTLIST":207,"INIT_TRAJ_INCOMPLETE_JOINTLIST_STR":"Trajectory must contain data for all joints.","INIT_TRAJ_INVALID_TIME":208,"INIT_TRAJ_INVALID_TIME_STR":"Invalid time in trajectory.","INIT_TRAJ_WRONG_MODE":209,"INIT_TRAJ_WRONG_MODE_STR":"Must call start_traj_mode service","INIT_TRAJ_BACKWARD_TIME":210,"INIT_TRAJ_BACKWARD_TIME_STR":"Trajectory message contains waypoints that are not strictly increasing in time.","INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS":211,"INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS_STR":"Trajectory did not contain position data for all axes.","INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES":212,"INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES_STR":"Trajectory did not contain velocity data for all axes.","INIT_TRAJ_INVALID_ENDING_VELOCITY":213,"INIT_TRAJ_INVALID_ENDING_VELOCITY_STR":"The final point in the trajectory must have zero velocity.","INIT_TRAJ_INVALID_ENDING_ACCELERATION":214,"INIT_TRAJ_INVALID_ENDING_ACCELERATION_STR":"The final point in the trajectory must have zero acceleration.","INIT_TRAJ_DUPLICATE_JOINT_NAME":215,"INIT_TRAJ_DUPLICATE_JOINT_NAME_STR":"The trajectory contains duplicate joint names."}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Motoros2_interfacesMsgInitTrajEnumSize(Ros2Motoros2_interfacesMsgInitTrajEnumCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgInitTrajEnumCapture(value),n=Ros2Motoros2_interfacesMsgInitTrajEnumSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgInitTrajEnumWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgInitTrajEnumRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgInitTrajEnumSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Motoros2_interfacesMsgInitTrajEnumWrite(value,w){
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Motoros2_interfacesMsgInitTrajEnumRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgInitTrajEnumCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/IoResultCodes codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgIoResultCodesCodec=Object.freeze({codecIdentity:"UWVMROS1_ef9ab829ae79d4e58336a56259b6beb2ad26a8b1b4070013d9004ac4ab269915",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({"OK":0,"OK_STR":"Success","READ_ADDRESS_INVALID":1001,"READ_ADDRESS_INVALID_STR":"Address cannot be read from (out of readable range)","WRITE_ADDRESS_INVALID":1002,"WRITE_ADDRESS_INVALID_STR":"Address cannot be written to (out of writable range)","WRITE_VALUE_INVALID":1003,"WRITE_VALUE_INVALID_STR":"Illegal value for the type of IO element addressed","READ_API_ERROR":1004,"READ_API_ERROR_STR":"The MotoPlus function mpReadIO returned -1. No further information is available","WRITE_API_ERROR":1005,"WRITE_API_ERROR_STR":"The MotoPlus function mpWriteIO returned -1. No further information is available","UNKNOWN_API_ERROR_STR":"Unknown error accessing I/O. No further information is available"}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesMsgIoResultCodesSize(Ros2Motoros2_interfacesMsgIoResultCodesCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgIoResultCodesCapture(value),n=Ros2Motoros2_interfacesMsgIoResultCodesSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgIoResultCodesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgIoResultCodesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgIoResultCodesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesMsgIoResultCodesWrite(value,w){
}
function Ros2Motoros2_interfacesMsgIoResultCodesRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesMsgIoResultCodesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/msg/MotionReadyEnum codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgMotionReadyEnumCodec=Object.freeze({codecIdentity:"UWVMROS1_c8d4c2b0eba665e01cddc730421a3a42c5b344f4de35ae0a32204891603326d6",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({"READY":1,"READY_STR":"Ready","NOT_READY_UNSPECIFIED":100,"NOT_READY_UNSPECIFIED_STR":"Unspecified","NOT_READY_ALARM":101,"NOT_READY_ALARM_STR":"The controller has an active Alarm","NOT_READY_ERROR":102,"NOT_READY_ERROR_STR":"The controller has an active Error","NOT_READY_ESTOP":103,"NOT_READY_ESTOP_STR":"The controller is in E-Stop","NOT_READY_NOT_PLAY":104,"NOT_READY_NOT_PLAY_STR":"The teach pendant must not be in TEACH mode","NOT_READY_NOT_REMOTE":105,"NOT_READY_NOT_REMOTE_STR":"The teach pendant must be in REMOTE mode","NOT_READY_SERVO_OFF":106,"NOT_READY_SERVO_OFF_STR":"Servo power is OFF. Please call start_traj_mode or start_point_queue_mode service","NOT_READY_HOLD":107,"NOT_READY_HOLD_STR":"The controller is in HOLD","NOT_READY_JOB_NOT_STARTED":108,"NOT_READY_JOB_NOT_STARTED_STR":"The INIT_ROS job has not started. Please call start_traj_mode or start_point_queue_mode service","NOT_READY_NOT_ON_WAIT_CMD":109,"NOT_READY_NOT_ON_WAIT_CMD_STR":"INFORM job is not on a WAIT command. Check the format of INIT_ROS","NOT_READY_PFL_ACTIVE":111,"NOT_READY_PFL_ACTIVE_STR":"A PFL event has occurred. Please return the robot to a safe state","NOT_READY_INC_MOVE_ERROR":112,"NOT_READY_INC_MOVE_ERROR_STR":"The controller is in an error state. Please call reset_error to attempt to clear the error","NOT_READY_OTHER_PROGRAM_RUNNING":113,"NOT_READY_OTHER_PROGRAM_RUNNING_STR":"Controller is running another job","NOT_READY_OTHER_TRAJ_MODE_ACTIVE":114,"NOT_READY_OTHER_TRAJ_MODE_ACTIVE_STR":"Another trajectory mode is already active. Please call stop_traj_mode service","NOT_READY_NOT_CONT_CYCLE_MODE":115,"NOT_READY_NOT_CONT_CYCLE_MODE_STR":"AUTO cycle mode not set. Please set the cycle mode to AUTO","NOT_READY_MAJOR_ALARM":116,"NOT_READY_MAJOR_ALARM_STR":"Major Alarms can't be reset. Please check the teach pendant","NOT_READY_ECO_MODE":117,"NOT_READY_ECO_MODE_STR":"Couldn't disable eco mode","NOT_READY_SERVO_ON_TIMEOUT":118,"NOT_READY_SERVO_ON_TIMEOUT_STR":"Timed out while waiting for servo on"}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Motoros2_interfacesMsgMotionReadyEnumSize(Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(value),n=Ros2Motoros2_interfacesMsgMotionReadyEnumSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgMotionReadyEnumWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgMotionReadyEnumRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgMotionReadyEnumSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Motoros2_interfacesMsgMotionReadyEnumWrite(value,w){
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Motoros2_interfacesMsgMotionReadyEnumRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/QueueResultEnum codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgQueueResultEnumCodec=Object.freeze({codecIdentity:"UWVMROS1_146c6ce3f275b9e4a81622d73cca3d2298cf9f7bb95a783c0dea6279cb5b4f23",maxWireBytes:2,minWireBytes:2,cartesianMaxWireBytes:2n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESS":1,"SUCCESS_STR":"","WRONG_MODE":2,"WRONG_MODE_STR":"Must call start_point_queue_mode service","INIT_FAILURE":3,"INIT_FAILURE_STR":"Failed to initialize the streaming trajectory","BUSY":4,"BUSY_STR":"Busy processing the previous trajectory point. Please resend the next point shortly","INVALID_JOINT_LIST":5,"INVALID_JOINT_LIST_STR":"Point must contain data for all joints","UNABLE_TO_PROCESS_POINT":6,"UNABLE_TO_PROCESS_POINT_STR":"Error while processing the trajectory point. Check debug log for more details"}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Motoros2_interfacesMsgQueueResultEnumSize(Ros2Motoros2_interfacesMsgQueueResultEnumCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgQueueResultEnumCapture(value),n=Ros2Motoros2_interfacesMsgQueueResultEnumSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgQueueResultEnumWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2,90);if(owned.length<2)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgQueueResultEnumRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgQueueResultEnumSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],2,false);n=addSize(n,2);
if(n>2)bad(90);return n;}
function Ros2Motoros2_interfacesMsgQueueResultEnumWrite(value,w){
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Motoros2_interfacesMsgQueueResultEnumRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgQueueResultEnumCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/msg/SelectionResultCodes codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesMsgSelectionResultCodesCodec=Object.freeze({codecIdentity:"UWVMROS1_46168b1f851163985e75842d9d795c5715160864a7736e7f92f95818c9435602",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({"OK":0,"OK_STR":"","INVALID_CONTROLLER_STATE":400,"INVALID_CONTROLLER_STATE_STR":"Pendant is not in REMOTE mode","INVALID_CONTROL_GROUP":401,"INVALID_CONTROL_GROUP_STR":"Invalid control group","INVALID_SELECTION_INDEX":402,"INVALID_SELECTION_INDEX_STR":"Invalid tool selection"}),
defaultValue(){return {"value":0};},
validateValue(value){Ros2Motoros2_interfacesMsgSelectionResultCodesSize(Ros2Motoros2_interfacesMsgSelectionResultCodesCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesMsgSelectionResultCodesCapture(value),n=Ros2Motoros2_interfacesMsgSelectionResultCodesSize(owned),w=new Writer(n);Ros2Motoros2_interfacesMsgSelectionResultCodesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesMsgSelectionResultCodesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesMsgSelectionResultCodesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Motoros2_interfacesMsgSelectionResultCodesWrite(value,w){
w.v.setUint32(w.at,value["value"],true);w.at+=4;
}
function Ros2Motoros2_interfacesMsgSelectionResultCodesRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesMsgSelectionResultCodesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_aadcd63f3bc47424c38767afdc1589561e3c90e8aeae9bdceef88b1033d57a7e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestSize(Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCapture(value),n=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestWrite(value,w){
}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e11bdbb95b6ded88d035696260c58dad32bcf3604aa4e89dd41c0dab26541567",maxWireBytes:32768,minWireBytes:16,cartesianMaxWireBytes:413008n,aggregateWireBytes:32768,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"result_message":"","alarms":[],"errors":[]};},
validateValue(value){Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseSize(Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,32768,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["result_message"],512).length);
if(!Array.isArray(value["alarms"])||value["alarms"].length>16)bad();
n=addSize(n,4);
for(const element of value["alarms"]){
n=addSize(n,Ros2Motoros2_interfacesMsgAlarmInfoSize(element));
}
if(!Array.isArray(value["errors"])||value["errors"].length>16)bad();
n=addSize(n,4);
for(const element of value["errors"]){
n=addSize(n,Ros2Motoros2_interfacesMsgErrorInfoSize(element));
}
if(n>32768)bad(90);return n;}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["result_message"],512);
w.v.setUint32(w.at,value["alarms"].length,true);w.at+=4;
for(const element of value["alarms"]){
Ros2Motoros2_interfacesMsgAlarmInfoWrite(element,w);
}
w.v.setUint32(w.at,value["errors"].length,true);w.at+=4;
for(const element of value["errors"]){
Ros2Motoros2_interfacesMsgErrorInfoWrite(element,w);
}
}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["result_message"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["alarms"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgAlarmInfoRead(r);
value["alarms"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["errors"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Motoros2_interfacesMsgErrorInfoRead(r);
value["errors"].push(decoded);
}}
}
return value;}
function Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["result_message"];
out["result_message"]=field;
}
{const field=value["alarms"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Motoros2_interfacesMsgAlarmInfoCapture(element);
}out["alarms"]=values;
}
{const field=value["errors"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Motoros2_interfacesMsgErrorInfoCapture(element);
}out["errors"]=values;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ListInformJobs_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvListInformJobs_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e77eedadef98d98321b1757b88284022afcdb1ffef52b98820b0c4ba596d643c",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesSrvListInformJobs_RequestSize(Ros2Motoros2_interfacesSrvListInformJobs_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvListInformJobs_RequestCapture(value),n=Ros2Motoros2_interfacesSrvListInformJobs_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvListInformJobs_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvListInformJobs_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvListInformJobs_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesSrvListInformJobs_RequestWrite(value,w){
}
function Ros2Motoros2_interfacesSrvListInformJobs_RequestRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesSrvListInformJobs_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ListInformJobs_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvListInformJobs_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_49021f0abc00f8df1ef49605a244cb6bba8d8b45ece5365ae35d747d2506eadc",maxWireBytes:8780,minWireBytes:12,cartesianMaxWireBytes:8780n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","names":[]};},
validateValue(value){Ros2Motoros2_interfacesSrvListInformJobs_ResponseSize(Ros2Motoros2_interfacesSrvListInformJobs_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvListInformJobs_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvListInformJobs_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvListInformJobs_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8780,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvListInformJobs_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvListInformJobs_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(!Array.isArray(value["names"])||value["names"].length>16)bad();
n=addSize(n,4);
for(const element of value["names"]){
n=addSize(n,4+textBytes(element,512).length);
}
if(n>8780)bad(90);return n;}
function Ros2Motoros2_interfacesSrvListInformJobs_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint32(w.at,value["names"].length,true);w.at+=4;
for(const element of value["names"]){
w.text(element,512);
}
}
function Ros2Motoros2_interfacesSrvListInformJobs_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(512);
value["names"].push(decoded);
}}
}
return value;}
function Ros2Motoros2_interfacesSrvListInformJobs_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["names"]=values;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/QueueTrajPoint_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3d37f9981d6abaca33ec0b5378fa3a1ac301791cc9be94d5bdcf4ea6aa81b8d0",maxWireBytes:1228,minWireBytes:28,cartesianMaxWireBytes:1228n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_names":[],"point":Ros2Trajectory_msgsMsgJointTrajectoryPointCodec.defaultValue()};},
validateValue(value){Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestSize(Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCapture(value),n=Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1228,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>12)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
n=addSize(n,Ros2Trajectory_msgsMsgJointTrajectoryPointSize(value["point"]));
if(n>1228)bad(90);return n;}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestWrite(value,w){
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
Ros2Trajectory_msgsMsgJointTrajectoryPointWrite(value["point"],w);
}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestRead(r){const value={};
{
{const count=r.u32();if(count>12)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPointRead(r);
value["point"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["point"];
out["point"]=Ros2Trajectory_msgsMsgJointTrajectoryPointCapture(field);
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/QueueTrajPoint_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d4887ed24d39332a5ccfd4a3d072004dd8f314fe5c166bf0bd57520a826cbccd",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":Ros2Motoros2_interfacesMsgQueueResultEnumCodec.defaultValue(),"message":""};},
validateValue(value){Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseSize(Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Motoros2_interfacesMsgQueueResultEnumSize(value["result_code"]));
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseWrite(value,w){
Ros2Motoros2_interfacesMsgQueueResultEnumWrite(value["result_code"],w);
w.text(value["message"],512);
}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseRead(r){const value={};
{
const decoded=Ros2Motoros2_interfacesMsgQueueResultEnumRead(r);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=Ros2Motoros2_interfacesMsgQueueResultEnumCapture(field);
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadGroupIO_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadGroupIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6ee09ac379e169fc23b81a551e9a9ed21c3f82b5010859984e911b17152beb7f",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadGroupIO_RequestSize(Ros2Motoros2_interfacesSrvReadGroupIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadGroupIO_RequestCapture(value),n=Ros2Motoros2_interfacesSrvReadGroupIO_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadGroupIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadGroupIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadGroupIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadGroupIO_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvReadGroupIO_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadGroupIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadGroupIO_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_292fcf6e8c46a2509ee4cc5238d443157f37fed0de898367248b967e9001ea0d",maxWireBytes:522,minWireBytes:10,cartesianMaxWireBytes:522n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadGroupIO_ResponseSize(Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvReadGroupIO_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadGroupIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,522,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadGroupIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadGroupIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
integer(value["value"],1,false);n=addSize(n,1);
if(n>522)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadGroupIO_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["value"],true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvReadGroupIO_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadMRegister_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadMRegister_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f31f0983466054f5ac5dcb2ab80ab7721d606a681b833ebb376cc87f96054f6f",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadMRegister_RequestSize(Ros2Motoros2_interfacesSrvReadMRegister_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadMRegister_RequestCapture(value),n=Ros2Motoros2_interfacesSrvReadMRegister_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadMRegister_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadMRegister_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadMRegister_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadMRegister_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvReadMRegister_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadMRegister_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadMRegister_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadMRegister_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_8d6ded2876adfa2079711205cb0110a82ead6f51db05f266bf285308cd815a04",maxWireBytes:523,minWireBytes:11,cartesianMaxWireBytes:523n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadMRegister_ResponseSize(Ros2Motoros2_interfacesSrvReadMRegister_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadMRegister_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvReadMRegister_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadMRegister_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,523,90);if(owned.length<11)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadMRegister_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadMRegister_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
integer(value["value"],2,false);n=addSize(n,2);
if(n>523)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadMRegister_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Motoros2_interfacesSrvReadMRegister_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadMRegister_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadSingleIO_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadSingleIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_30e28db303b772f26b2b3cf6165a0ff79037d037d60e8115b4ee8e177f17e8bb",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadSingleIO_RequestSize(Ros2Motoros2_interfacesSrvReadSingleIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadSingleIO_RequestCapture(value),n=Ros2Motoros2_interfacesSrvReadSingleIO_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadSingleIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadSingleIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadSingleIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadSingleIO_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvReadSingleIO_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadSingleIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ReadSingleIO_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b42013571274e85769ffac4d47129de8b76e4a675bdef3efefebc9ee19e92082",maxWireBytes:525,minWireBytes:13,cartesianMaxWireBytes:525n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvReadSingleIO_ResponseSize(Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvReadSingleIO_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvReadSingleIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,525,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvReadSingleIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvReadSingleIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
integer(value["value"],4,true);n=addSize(n,4);
if(n>525)bad(90);return n;}
function Ros2Motoros2_interfacesSrvReadSingleIO_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.v.setInt32(w.at,value["value"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvReadSingleIO_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ResetError_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvResetError_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2737855d0608478946e2d9ab7f06c92585199c1082ec8406a012f50ad56beae3",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesSrvResetError_RequestSize(Ros2Motoros2_interfacesSrvResetError_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvResetError_RequestCapture(value),n=Ros2Motoros2_interfacesSrvResetError_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvResetError_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvResetError_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvResetError_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesSrvResetError_RequestWrite(value,w){
}
function Ros2Motoros2_interfacesSrvResetError_RequestRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesSrvResetError_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/srv/ResetError_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvResetError_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0af14ee02c157ecc39b143225a580e475473306389ccc3ba198cf22ca47d82f3",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":Ros2Motoros2_interfacesMsgMotionReadyEnumCodec.defaultValue(),"message":""};},
validateValue(value){Ros2Motoros2_interfacesSrvResetError_ResponseSize(Ros2Motoros2_interfacesSrvResetError_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvResetError_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvResetError_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvResetError_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvResetError_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvResetError_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Motoros2_interfacesMsgMotionReadyEnumSize(value["result_code"]));
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Motoros2_interfacesSrvResetError_ResponseWrite(value,w){
Ros2Motoros2_interfacesMsgMotionReadyEnumWrite(value["result_code"],w);
w.text(value["message"],512);
}
function Ros2Motoros2_interfacesSrvResetError_ResponseRead(r){const value={};
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnumRead(r);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvResetError_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(field);
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/SelectMotionTool_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e6c884ae0a97337c1e441efe099b7869dc95205fcbca0a8082d4d9b06f5b438c",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"group_number":0,"tool_number":0};},
validateValue(value){Ros2Motoros2_interfacesSrvSelectMotionTool_RequestSize(Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCapture(value),n=Ros2Motoros2_interfacesSrvSelectMotionTool_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvSelectMotionTool_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvSelectMotionTool_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvSelectMotionTool_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["group_number"],4,false);n=addSize(n,4);
integer(value["tool_number"],4,false);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Motoros2_interfacesSrvSelectMotionTool_RequestWrite(value,w){
w.v.setUint32(w.at,value["group_number"],true);w.at+=4;
w.v.setUint32(w.at,value["tool_number"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvSelectMotionTool_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["group_number"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["tool_number"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["group_number"];
out["group_number"]=field;
}
{const field=value["tool_number"];
out["tool_number"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/SelectMotionTool_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_088349a49c14c52282c25b428628462c48618c3b5202df560dcc5c9bed742640",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":Ros2Motoros2_interfacesMsgSelectionResultCodesCodec.defaultValue(),"message":"","success":false};},
validateValue(value){Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseSize(Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Motoros2_interfacesMsgSelectionResultCodesSize(value["result_code"]));
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseWrite(value,w){
Ros2Motoros2_interfacesMsgSelectionResultCodesWrite(value["result_code"],w);
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseRead(r){const value={};
{
const decoded=Ros2Motoros2_interfacesMsgSelectionResultCodesRead(r);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=Ros2Motoros2_interfacesMsgSelectionResultCodesCapture(field);
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/StartPointQueueMode_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_58a4827fc6535ef0ffc64ee25c5c4af75aa0fe199fa0350a88a95d2e991164a1",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestSize(Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCapture(value),n=Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestWrite(value,w){
}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/srv/StartPointQueueMode_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bc553d5640e1c4644a5d5bdc2dfcefc9dbe789ef1e2a23dd620f08963467b6ff",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":Ros2Motoros2_interfacesMsgMotionReadyEnumCodec.defaultValue(),"message":""};},
validateValue(value){Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseSize(Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Motoros2_interfacesMsgMotionReadyEnumSize(value["result_code"]));
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseWrite(value,w){
Ros2Motoros2_interfacesMsgMotionReadyEnumWrite(value["result_code"],w);
w.text(value["message"],512);
}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseRead(r){const value={};
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnumRead(r);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(field);
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/StartTrajMode_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvStartTrajMode_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0b8b2ad5db5901047035e1070b675618afc516a1ae8c3308cfdcad1174d34789",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Motoros2_interfacesSrvStartTrajMode_RequestSize(Ros2Motoros2_interfacesSrvStartTrajMode_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvStartTrajMode_RequestCapture(value),n=Ros2Motoros2_interfacesSrvStartTrajMode_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvStartTrajMode_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvStartTrajMode_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvStartTrajMode_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Motoros2_interfacesSrvStartTrajMode_RequestWrite(value,w){
}
function Ros2Motoros2_interfacesSrvStartTrajMode_RequestRead(r){const value={};
return value;}
function Ros2Motoros2_interfacesSrvStartTrajMode_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 motoros2_interfaces/srv/StartTrajMode_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_170e25737e6a2b06f5d5d9f331d748c97cf0306284271757953a4a801c722939",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":Ros2Motoros2_interfacesMsgMotionReadyEnumCodec.defaultValue(),"message":""};},
validateValue(value){Ros2Motoros2_interfacesSrvStartTrajMode_ResponseSize(Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvStartTrajMode_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvStartTrajMode_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvStartTrajMode_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvStartTrajMode_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Motoros2_interfacesMsgMotionReadyEnumSize(value["result_code"]));
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros2Motoros2_interfacesSrvStartTrajMode_ResponseWrite(value,w){
Ros2Motoros2_interfacesMsgMotionReadyEnumWrite(value["result_code"],w);
w.text(value["message"],512);
}
function Ros2Motoros2_interfacesSrvStartTrajMode_ResponseRead(r){const value={};
{
const decoded=Ros2Motoros2_interfacesMsgMotionReadyEnumRead(r);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=Ros2Motoros2_interfacesMsgMotionReadyEnumCapture(field);
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteGroupIO_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2d348ed71408a0dd1dd28ee2ed2ca1db56bb07a14546d0fe13ccf64e8553d52c",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteGroupIO_RequestSize(Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCapture(value),n=Ros2Motoros2_interfacesSrvWriteGroupIO_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteGroupIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteGroupIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteGroupIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
integer(value["value"],1,false);n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteGroupIO_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
w.v.setUint8(w.at,value["value"],true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvWriteGroupIO_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteGroupIO_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_06769934711b27fd32cdcbf849504e160189ee82ea32e0e71a2903af47c5d766",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseSize(Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteMRegister_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteMRegister_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8df4536be27f0632052725d5616fd5b047f49fc306fc7735854d9de4fb15d4ef",maxWireBytes:6,minWireBytes:6,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteMRegister_RequestSize(Ros2Motoros2_interfacesSrvWriteMRegister_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteMRegister_RequestCapture(value),n=Ros2Motoros2_interfacesSrvWriteMRegister_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteMRegister_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteMRegister_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteMRegister_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
integer(value["value"],2,false);n=addSize(n,2);
if(n>6)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteMRegister_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
w.v.setUint16(w.at,value["value"],true);w.at+=2;
}
function Ros2Motoros2_interfacesSrvWriteMRegister_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteMRegister_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteMRegister_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a61cc0596a7545ef4836bdc64dc9d6b4ed99b0abd18a726eb37e6215f51c729a",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteMRegister_ResponseSize(Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvWriteMRegister_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteMRegister_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteMRegister_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteMRegister_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteMRegister_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvWriteMRegister_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteSingleIO_Request codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6361affcd8b7ebaaa1fe6b5806e64a7c18f7a96a1178614cfce487de813a6392",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"address":0,"value":0};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteSingleIO_RequestSize(Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCapture(value),n=Ros2Motoros2_interfacesSrvWriteSingleIO_RequestSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteSingleIO_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteSingleIO_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteSingleIO_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["address"],4,false);n=addSize(n,4);
integer(value["value"],4,true);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteSingleIO_RequestWrite(value,w){
w.v.setUint32(w.at,value["address"],true);w.at+=4;
w.v.setInt32(w.at,value["value"],true);w.at+=4;
}
function Ros2Motoros2_interfacesSrvWriteSingleIO_RequestRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["address"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["value"]=decoded;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["address"];
out["address"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS2 motoros2_interfaces/srv/WriteSingleIO_Response codec; maxima are not actual lengths. */
export const Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_12328db372a572fe9b0c7207331a7971d5d7c9033376e3be410db579b85fb794",maxWireBytes:521,minWireBytes:9,cartesianMaxWireBytes:521n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":"","success":false};},
validateValue(value){Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseSize(Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCapture(value),n=Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseSize(owned),w=new Writer(n);Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,521,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],4,false);n=addSize(n,4);
n=addSize(n,4+textBytes(value["message"],512).length);
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
if(n>521)bad(90);return n;}
function Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseWrite(value,w){
w.v.setUint32(w.at,value["result_code"],true);w.at+=4;
w.text(value["message"],512);
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
}
function Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"]=decoded!==0;
}
return value;}
function Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
{const field=value["success"];
out["success"]=field;
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
/** Complete ROS2 tf2_msgs/msg/TFMessage codec; maxima are not actual lengths. */
export const Ros2Tf2_msgsMsgTFMessageCodec=Object.freeze({codecIdentity:"UWVMROS1_4428597a9c2783fd8f0f083a16e8fd1b3d286487f4800c55fcc5571e7f79e65a",maxWireBytes:10372,minWireBytes:4,cartesianMaxWireBytes:10372n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"transforms":[]};},
validateValue(value){Ros2Tf2_msgsMsgTFMessageSize(Ros2Tf2_msgsMsgTFMessageCapture(value));return true;},
encode(value){const owned=Ros2Tf2_msgsMsgTFMessageCapture(value),n=Ros2Tf2_msgsMsgTFMessageSize(owned),w=new Writer(n);Ros2Tf2_msgsMsgTFMessageWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10372,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Tf2_msgsMsgTFMessageRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Tf2_msgsMsgTFMessageSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["transforms"])||value["transforms"].length>16)bad();
n=addSize(n,4);
for(const element of value["transforms"]){
n=addSize(n,Ros2Geometry_msgsMsgTransformStampedSize(element));
}
if(n>10372)bad(90);return n;}
function Ros2Tf2_msgsMsgTFMessageWrite(value,w){
w.v.setUint32(w.at,value["transforms"].length,true);w.at+=4;
for(const element of value["transforms"]){
Ros2Geometry_msgsMsgTransformStampedWrite(element,w);
}
}
function Ros2Tf2_msgsMsgTFMessageRead(r){const value={};
{
{const count=r.u32();if(count>16)bad();value["transforms"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTransformStampedRead(r);
value["transforms"].push(decoded);
}}
}
return value;}
function Ros2Tf2_msgsMsgTFMessageCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["transforms"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Geometry_msgsMsgTransformStampedCapture(element);
}out["transforms"]=values;
}
return out;}
