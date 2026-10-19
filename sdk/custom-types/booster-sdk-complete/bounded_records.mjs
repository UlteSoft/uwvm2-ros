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
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/ImuState codec; maxima are not actual lengths. */
export const Ros2Uwvm_booster_sdk_bridgeMsgImuStateCodec=Object.freeze({codecIdentity:"UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04",maxWireBytes:36,minWireBytes:36,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"rpy":Array.from({length:3},()=>(0)),"gyro":Array.from({length:3},()=>(0)),"acc":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Uwvm_booster_sdk_bridgeMsgImuStateSize(Ros2Uwvm_booster_sdk_bridgeMsgImuStateCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_booster_sdk_bridgeMsgImuStateCapture(value),n=Ros2Uwvm_booster_sdk_bridgeMsgImuStateSize(owned),w=new Writer(n);Ros2Uwvm_booster_sdk_bridgeMsgImuStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Uwvm_booster_sdk_bridgeMsgImuStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_booster_sdk_bridgeMsgImuStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["rpy"])||value["rpy"].length!==3)bad();
for(const element of value["rpy"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["gyro"])||value["gyro"].length!==3)bad();
for(const element of value["gyro"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["acc"])||value["acc"].length!==3)bad();
for(const element of value["acc"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>36)bad(90);return n;}
function Ros2Uwvm_booster_sdk_bridgeMsgImuStateWrite(value,w){
for(const element of value["rpy"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["gyro"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["acc"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Uwvm_booster_sdk_bridgeMsgImuStateRead(r){const value={};
{
{const count=3;if(count>3)bad();value["rpy"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rpy"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["gyro"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["gyro"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["acc"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["acc"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_booster_sdk_bridgeMsgImuStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rpy"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["rpy"]=values;
}
{const field=value["gyro"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["gyro"]=values;
}
{const field=value["acc"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["acc"]=values;
}
return out;}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/MotorCmd codec; maxima are not actual lengths. */
export const Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"tau":0,"kp":0,"kd":0,"weight":0};},
validateValue(value){Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdSize(Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCapture(value),n=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdSize(owned),w=new Writer(n);Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["tau"]!=="number"||!Number.isFinite(value["tau"])||!Number.isFinite(Math.fround(value["tau"])))bad();n=addSize(n,4);
if(typeof value["kp"]!=="number"||!Number.isFinite(value["kp"])||!Number.isFinite(Math.fround(value["kp"])))bad();n=addSize(n,4);
if(typeof value["kd"]!=="number"||!Number.isFinite(value["kd"])||!Number.isFinite(Math.fround(value["kd"])))bad();n=addSize(n,4);
if(typeof value["weight"]!=="number"||!Number.isFinite(value["weight"])||!Number.isFinite(Math.fround(value["weight"])))bad();n=addSize(n,4);
if(n>25)bad(90);return n;}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau"],true);w.at+=4;
w.v.setFloat32(w.at,value["kp"],true);w.at+=4;
w.v.setFloat32(w.at,value["kd"],true);w.at+=4;
w.v.setFloat32(w.at,value["weight"],true);w.at+=4;
}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["dq"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tau"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kp"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["kd"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["weight"]=decoded;
}
return value;}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["q"];
out["q"]=field;
}
{const field=value["dq"];
out["dq"]=field;
}
{const field=value["tau"];
out["tau"]=field;
}
{const field=value["kp"];
out["kp"]=field;
}
{const field=value["kd"];
out["kd"]=field;
}
{const field=value["weight"];
out["weight"]=field;
}
return out;}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/MotorState codec; maxima are not actual lengths. */
export const Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af",maxWireBytes:30,minWireBytes:30,cartesianMaxWireBytes:30n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"ddq":0,"tau_est":0,"temperature":0,"lost":0,"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Uwvm_booster_sdk_bridgeMsgMotorStateSize(Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCapture(value),n=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateSize(owned),w=new Writer(n);Ros2Uwvm_booster_sdk_bridgeMsgMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,30,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_booster_sdk_bridgeMsgMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["ddq"]!=="number"||!Number.isFinite(value["ddq"])||!Number.isFinite(Math.fround(value["ddq"])))bad();n=addSize(n,4);
if(typeof value["tau_est"]!=="number"||!Number.isFinite(value["tau_est"])||!Number.isFinite(Math.fround(value["tau_est"])))bad();n=addSize(n,4);
integer(value["temperature"],1,false);n=addSize(n,1);
integer(value["lost"],4,false);n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>30)bad(90);return n;}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau_est"],true);w.at+=4;
w.v.setUint8(w.at,value["temperature"],true);w.at+=1;
w.v.setUint32(w.at,value["lost"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["dq"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ddq"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tau_est"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["temperature"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["lost"]=decoded;
}
{
{const count=2;if(count>2)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["q"];
out["q"]=field;
}
{const field=value["dq"];
out["dq"]=field;
}
{const field=value["ddq"];
out["ddq"]=field;
}
{const field=value["tau_est"];
out["tau_est"]=field;
}
{const field=value["temperature"];
out["temperature"]=field;
}
{const field=value["lost"];
out["lost"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/LowCmd codec; maxima are not actual lengths. */
export const Ros2Uwvm_booster_sdk_bridgeMsgLowCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a",maxWireBytes:733,minWireBytes:8,cartesianMaxWireBytes:733n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cmd_type":0,"motor_cmd":[]};},
validateValue(value){Ros2Uwvm_booster_sdk_bridgeMsgLowCmdSize(Ros2Uwvm_booster_sdk_bridgeMsgLowCmdCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_booster_sdk_bridgeMsgLowCmdCapture(value),n=Ros2Uwvm_booster_sdk_bridgeMsgLowCmdSize(owned),w=new Writer(n);Ros2Uwvm_booster_sdk_bridgeMsgLowCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,733,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Uwvm_booster_sdk_bridgeMsgLowCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_booster_sdk_bridgeMsgLowCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["cmd_type"],4,true);n=addSize(n,4);
if(!Array.isArray(value["motor_cmd"])||value["motor_cmd"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_cmd"]){
n=addSize(n,Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdSize(element));
}
if(n>733)bad(90);return n;}
function Ros2Uwvm_booster_sdk_bridgeMsgLowCmdWrite(value,w){
w.v.setInt32(w.at,value["cmd_type"],true);w.at+=4;
w.v.setUint32(w.at,value["motor_cmd"].length,true);w.at+=4;
for(const element of value["motor_cmd"]){
Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdWrite(element,w);
}
}
function Ros2Uwvm_booster_sdk_bridgeMsgLowCmdRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["cmd_type"]=decoded;
}
{
{const count=r.u32();if(count>29)bad();value["motor_cmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdRead(r);
value["motor_cmd"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_booster_sdk_bridgeMsgLowCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cmd_type"];
out["cmd_type"]=field;
}
{const field=value["motor_cmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCapture(element);
}out["motor_cmd"]=values;
}
return out;}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/LowState codec; maxima are not actual lengths. */
export const Ros2Uwvm_booster_sdk_bridgeMsgLowStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58",maxWireBytes:1784,minWireBytes:44,cartesianMaxWireBytes:1784n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"imu_state":Ros2Uwvm_booster_sdk_bridgeMsgImuStateCodec.defaultValue(),"motor_state_parallel":[],"motor_state_serial":[]};},
validateValue(value){Ros2Uwvm_booster_sdk_bridgeMsgLowStateSize(Ros2Uwvm_booster_sdk_bridgeMsgLowStateCapture(value));return true;},
encode(value){const owned=Ros2Uwvm_booster_sdk_bridgeMsgLowStateCapture(value),n=Ros2Uwvm_booster_sdk_bridgeMsgLowStateSize(owned),w=new Writer(n);Ros2Uwvm_booster_sdk_bridgeMsgLowStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1784,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros2Uwvm_booster_sdk_bridgeMsgLowStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_booster_sdk_bridgeMsgLowStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Uwvm_booster_sdk_bridgeMsgImuStateSize(value["imu_state"]));
if(!Array.isArray(value["motor_state_parallel"])||value["motor_state_parallel"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_state_parallel"]){
n=addSize(n,Ros2Uwvm_booster_sdk_bridgeMsgMotorStateSize(element));
}
if(!Array.isArray(value["motor_state_serial"])||value["motor_state_serial"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_state_serial"]){
n=addSize(n,Ros2Uwvm_booster_sdk_bridgeMsgMotorStateSize(element));
}
if(n>1784)bad(90);return n;}
function Ros2Uwvm_booster_sdk_bridgeMsgLowStateWrite(value,w){
Ros2Uwvm_booster_sdk_bridgeMsgImuStateWrite(value["imu_state"],w);
w.v.setUint32(w.at,value["motor_state_parallel"].length,true);w.at+=4;
for(const element of value["motor_state_parallel"]){
Ros2Uwvm_booster_sdk_bridgeMsgMotorStateWrite(element,w);
}
w.v.setUint32(w.at,value["motor_state_serial"].length,true);w.at+=4;
for(const element of value["motor_state_serial"]){
Ros2Uwvm_booster_sdk_bridgeMsgMotorStateWrite(element,w);
}
}
function Ros2Uwvm_booster_sdk_bridgeMsgLowStateRead(r){const value={};
{
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgImuStateRead(r);
value["imu_state"]=decoded;
}
{
{const count=r.u32();if(count>29)bad();value["motor_state_parallel"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateRead(r);
value["motor_state_parallel"].push(decoded);
}}
}
{
{const count=r.u32();if(count>29)bad();value["motor_state_serial"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateRead(r);
value["motor_state_serial"].push(decoded);
}}
}
return value;}
function Ros2Uwvm_booster_sdk_bridgeMsgLowStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["imu_state"];
out["imu_state"]=Ros2Uwvm_booster_sdk_bridgeMsgImuStateCapture(field);
}
{const field=value["motor_state_parallel"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCapture(element);
}out["motor_state_parallel"]=values;
}
{const field=value["motor_state_serial"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCapture(element);
}out["motor_state_serial"]=values;
}
return out;}
