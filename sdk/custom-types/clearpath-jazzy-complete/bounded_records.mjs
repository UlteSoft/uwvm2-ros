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
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Goal codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_61b01a6234b41f96464de9449b3e8d1b955decf6afa12aa3ba94062c98cec681",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalSize(Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalCapture(value),n=Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalWrite(value,w){
}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalRead(r){const value={};
return value;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Result codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_686450363b8b15eba1e9e9e9ff05c9aacfb95bb0d113eaa7c443e4c70d55d41f",maxWireBytes:12,minWireBytes:4,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"offset":[]};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultSize(Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultCapture(value),n=Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["offset"])||value["offset"].length>2)bad();
n=addSize(n,4);
for(const element of value["offset"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>12)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultWrite(value,w){
w.v.setUint32(w.at,value["offset"].length,true);w.at+=4;
for(const element of value["offset"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["offset"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["offset"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["offset"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["offset"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Feedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_ebe7badd3f72982079911d8aa72c4ddd27ff0a12d2f948e5970649317a592b53",maxWireBytes:8,minWireBytes:4,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"iteration":[]};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackSize(Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackCapture(value),n=Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["iteration"])||value["iteration"].length>2)bad();
n=addSize(n,4);
for(const element of value["iteration"]){
integer(element,2,false);n=addSize(n,2);
}
if(n>8)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackWrite(value,w){
w.v.setUint32(w.at,value["iteration"].length,true);w.at+=4;
for(const element of value["iteration"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["iteration"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["iteration"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["iteration"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["iteration"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Goal codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxUpdate_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_f0362f8a90391e8ffda2e8f3b798769b644091058361882c4e9457196f007565",maxWireBytes:260,minWireBytes:4,cartesianMaxWireBytes:260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"file":""};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxUpdate_GoalSize(Ros2Clearpath_motor_msgsActionLynxUpdate_GoalCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxUpdate_GoalCapture(value),n=Ros2Clearpath_motor_msgsActionLynxUpdate_GoalSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxUpdate_GoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,260,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxUpdate_GoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxUpdate_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["file"],256).length);
if(n>260)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_GoalWrite(value,w){
w.text(value["file"],256);
}
function Ros2Clearpath_motor_msgsActionLynxUpdate_GoalRead(r){const value={};
{
const decoded=r.text(256);
value["file"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_GoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["file"];
out["file"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Result codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxUpdate_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_126fe871dbcb036e485d20e8ccdc8e9fb7be01757f539bfba895aabf44d341d8",maxWireBytes:6,minWireBytes:4,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":[]};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxUpdate_ResultSize(Ros2Clearpath_motor_msgsActionLynxUpdate_ResultCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxUpdate_ResultCapture(value),n=Ros2Clearpath_motor_msgsActionLynxUpdate_ResultSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxUpdate_ResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxUpdate_ResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxUpdate_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["success"])||value["success"].length>2)bad();
n=addSize(n,4);
for(const element of value["success"]){
if(typeof element!=="boolean")bad();n=addSize(n,1);
}
if(n>6)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_ResultWrite(value,w){
w.v.setUint32(w.at,value["success"].length,true);w.at+=4;
for(const element of value["success"]){
w.v.setUint8(w.at,element?1:0,true);w.at+=1;
}
}
function Ros2Clearpath_motor_msgsActionLynxUpdate_ResultRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["success"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["success"].push(decoded!==0);
}}
}
return value;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_ResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["success"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Feedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_2179ec9dee627234a1a31e9681ff214cc45ca837349cd8e407e019a354121b19",maxWireBytes:12,minWireBytes:4,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"progress":[]};},
validateValue(value){Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackSize(Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackCapture(value),n=Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["progress"])||value["progress"].length>2)bad();
n=addSize(n,4);
for(const element of value["progress"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>12)bad(90);return n;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackWrite(value,w){
w.v.setUint32(w.at,value["progress"].length,true);w.at+=4;
for(const element of value["progress"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["progress"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["progress"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["progress"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["progress"]=values;
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
/** Complete ROS2 clearpath_control_msgs/msg/DeviceInfo codec; maxima are not actual lengths. */
export const Ros2Clearpath_control_msgsMsgDeviceInfoCodec=Object.freeze({codecIdentity:"UWVMROS1_9111f5d4b5324c36eb85f01e6fdbc3c589479ff5b64a8fa7b9d467aa14675c3e",maxWireBytes:1820,minWireBytes:28,cartesianMaxWireBytes:1820n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"device_id":"","device_class":"","model":"","manufacturer":"","firmware_version":"","hardware_version":"","serial_number":""};},
validateValue(value){Ros2Clearpath_control_msgsMsgDeviceInfoSize(Ros2Clearpath_control_msgsMsgDeviceInfoCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_control_msgsMsgDeviceInfoCapture(value),n=Ros2Clearpath_control_msgsMsgDeviceInfoSize(owned),w=new Writer(n);Ros2Clearpath_control_msgsMsgDeviceInfoWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1820,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Clearpath_control_msgsMsgDeviceInfoRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_control_msgsMsgDeviceInfoSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["device_id"],256).length);
n=addSize(n,4+textBytes(value["device_class"],256).length);
n=addSize(n,4+textBytes(value["model"],256).length);
n=addSize(n,4+textBytes(value["manufacturer"],256).length);
n=addSize(n,4+textBytes(value["firmware_version"],256).length);
n=addSize(n,4+textBytes(value["hardware_version"],256).length);
n=addSize(n,4+textBytes(value["serial_number"],256).length);
if(n>1820)bad(90);return n;}
function Ros2Clearpath_control_msgsMsgDeviceInfoWrite(value,w){
w.text(value["device_id"],256);
w.text(value["device_class"],256);
w.text(value["model"],256);
w.text(value["manufacturer"],256);
w.text(value["firmware_version"],256);
w.text(value["hardware_version"],256);
w.text(value["serial_number"],256);
}
function Ros2Clearpath_control_msgsMsgDeviceInfoRead(r){const value={};
{
const decoded=r.text(256);
value["device_id"]=decoded;
}
{
const decoded=r.text(256);
value["device_class"]=decoded;
}
{
const decoded=r.text(256);
value["model"]=decoded;
}
{
const decoded=r.text(256);
value["manufacturer"]=decoded;
}
{
const decoded=r.text(256);
value["firmware_version"]=decoded;
}
{
const decoded=r.text(256);
value["hardware_version"]=decoded;
}
{
const decoded=r.text(256);
value["serial_number"]=decoded;
}
return value;}
function Ros2Clearpath_control_msgsMsgDeviceInfoCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["device_id"];
out["device_id"]=field;
}
{const field=value["device_class"];
out["device_class"]=field;
}
{const field=value["model"];
out["model"]=field;
}
{const field=value["manufacturer"];
out["manufacturer"]=field;
}
{const field=value["firmware_version"];
out["firmware_version"]=field;
}
{const field=value["hardware_version"];
out["hardware_version"]=field;
}
{const field=value["serial_number"];
out["serial_number"]=field;
}
return out;}
/** Complete ROS2 clearpath_control_msgs/msg/StreamControl codec; maxima are not actual lengths. */
export const Ros2Clearpath_control_msgsMsgStreamControlCodec=Object.freeze({codecIdentity:"UWVMROS1_26208e266c9185bfb0e715407ab0fe313f2d47e6bc9f54594a102c3a09d34734",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stream":"","enabled":false};},
validateValue(value){Ros2Clearpath_control_msgsMsgStreamControlSize(Ros2Clearpath_control_msgsMsgStreamControlCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_control_msgsMsgStreamControlCapture(value),n=Ros2Clearpath_control_msgsMsgStreamControlSize(owned),w=new Writer(n);Ros2Clearpath_control_msgsMsgStreamControlWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_control_msgsMsgStreamControlRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_control_msgsMsgStreamControlSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["stream"],256).length);
if(typeof value["enabled"]!=="boolean")bad();n=addSize(n,1);
if(n>261)bad(90);return n;}
function Ros2Clearpath_control_msgsMsgStreamControlWrite(value,w){
w.text(value["stream"],256);
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros2Clearpath_control_msgsMsgStreamControlRead(r){const value={};
{
const decoded=r.text(256);
value["stream"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
function Ros2Clearpath_control_msgsMsgStreamControlCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["stream"];
out["stream"]=field;
}
{const field=value["enabled"];
out["enabled"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxDebug codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxDebugCodec=Object.freeze({codecIdentity:"UWVMROS1_0341ae040f1e8a1f980bde84498c4a4f8b9c55c3806839a73727c1579ab4416e",maxWireBytes:291,minWireBytes:19,cartesianMaxWireBytes:291n,aggregateWireBytes:0,constants:Object.freeze({"ACCUMULATOR_FLAG_ABOVE_RATED":0,"ACCUMULATOR_FLAG_CONTINUOUS_1_1":1,"ACCUMULATOR_FLAG_CONTINUOUS_1_2":2,"ACCUMULATOR_FLAG_CONTINUOUS_1_3":3,"ACCUMULATOR_FLAG_CONTINUOUS_1_4":4,"ACCUMULATOR_FLAG_CONTINUOUS_1_5":5,"ACCUMULATOR_FLAG_CONTINUOUS_1_6":6,"ACCUMULATOR_FLAG_CONTINUOUS_1_7":7,"ACCUMULATOR_FLAG_CONTINUOUS_1_8":8,"ACCUMULATOR_FLAG_CONTINUOUS_1_9":9,"ACCUMULATOR_FLAG_CONTINUOUS_2_0":10,"ACCUMULATOR_FLAG_CONTINUOUS_2_1":11,"ACCUMULATOR_FLAG_CONTINUOUS_2_2":12,"ACCUMULATOR_FLAG_CONTINUOUS_2_3":13,"ACCUMULATOR_FLAG_CONTINUOUS_2_4":14,"ACCUMULATOR_FLAG_CONTINUOUS_2_5":15,"ACCUMULATOR_FLAG_CONTINUOUS_2_6":16,"ACCUMULATOR_FLAG_CONTINUOUS_2_7":17,"ACCUMULATOR_FLAG_CONTINUOUS_2_8":18,"ACCUMULATOR_FLAG_CONTINUOUS_2_9":19,"ACCUMULATOR_FLAG_CONTINUOUS_3_0":20,"ACCUMULATOR_ABOVE_RATED":0,"ACCUMULATOR_CONTINUOUS_1_1":1,"ACCUMULATOR_CONTINUOUS_1_5":2,"ACCUMULATOR_CONTINUOUS_2_0":3,"ACCUMULATOR_CONTINUOUS_2_5":4,"ACCUMULATOR_CONTINUOUS_3_0":5,"CURRENT_ID_FB":0,"CURRENT_ID_FF":1,"CURRENT_ID_REF":2,"CURRENT_IQ_FB":3,"CURRENT_IQ_FF":4,"CURRENT_IQ_REF":5}),
defaultValue(){return {"can_id":0,"joint_name":"","fault_frequency":0,"accumulator_flags":0,"accumulators":[],"currents":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxDebugSize(Ros2Clearpath_motor_msgsMsgLynxDebugCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxDebugCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxDebugSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxDebugWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,291,90);if(owned.length<19)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxDebugRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxDebugSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["can_id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["joint_name"],256).length);
integer(value["fault_frequency"],2,false);n=addSize(n,2);
integer(value["accumulator_flags"],4,false);n=addSize(n,4);
if(!Array.isArray(value["accumulators"])||value["accumulators"].length>2)bad();
n=addSize(n,4);
for(const element of value["accumulators"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["currents"])||value["currents"].length>2)bad();
n=addSize(n,4);
for(const element of value["currents"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>291)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxDebugWrite(value,w){
w.v.setUint8(w.at,value["can_id"],true);w.at+=1;
w.text(value["joint_name"],256);
w.v.setUint16(w.at,value["fault_frequency"],true);w.at+=2;
w.v.setUint32(w.at,value["accumulator_flags"],true);w.at+=4;
w.v.setUint32(w.at,value["accumulators"].length,true);w.at+=4;
for(const element of value["accumulators"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["currents"].length,true);w.at+=4;
for(const element of value["currents"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_motor_msgsMsgLynxDebugRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["can_id"]=decoded;
}
{
const decoded=r.text(256);
value["joint_name"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["fault_frequency"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["accumulator_flags"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["accumulators"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["accumulators"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2)bad();value["currents"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["currents"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxDebugCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["can_id"];
out["can_id"]=field;
}
{const field=value["joint_name"];
out["joint_name"]=field;
}
{const field=value["fault_frequency"];
out["fault_frequency"]=field;
}
{const field=value["accumulator_flags"];
out["accumulator_flags"]=field;
}
{const field=value["accumulators"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["accumulators"]=values;
}
{const field=value["currents"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["currents"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxFeedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_5d92c7085019ebf00ba93788b5b2f4055ad40b2c6714575a66fb75cf314429eb",maxWireBytes:277,minWireBytes:21,cartesianMaxWireBytes:277n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"can_id":0,"joint_name":"","current":0,"voltage":0,"velocity":0,"travel":0};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxFeedbackSize(Ros2Clearpath_motor_msgsMsgLynxFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxFeedbackCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,277,90);if(owned.length<21)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["can_id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["joint_name"],256).length);
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["voltage"]!=="number"||!Number.isFinite(value["voltage"])||!Number.isFinite(Math.fround(value["voltage"])))bad();n=addSize(n,4);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"])||!Number.isFinite(Math.fround(value["velocity"])))bad();n=addSize(n,4);
if(typeof value["travel"]!=="number"||!Number.isFinite(value["travel"])||!Number.isFinite(Math.fround(value["travel"])))bad();n=addSize(n,4);
if(n>277)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxFeedbackWrite(value,w){
w.v.setUint8(w.at,value["can_id"],true);w.at+=1;
w.text(value["joint_name"],256);
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat32(w.at,value["voltage"],true);w.at+=4;
w.v.setFloat32(w.at,value["velocity"],true);w.at+=4;
w.v.setFloat32(w.at,value["travel"],true);w.at+=4;
}
function Ros2Clearpath_motor_msgsMsgLynxFeedbackRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["can_id"]=decoded;
}
{
const decoded=r.text(256);
value["joint_name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["velocity"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["travel"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["can_id"];
out["can_id"]=field;
}
{const field=value["joint_name"];
out["joint_name"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["voltage"];
out["voltage"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["travel"];
out["travel"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMotorProtection codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCodec=Object.freeze({codecIdentity:"UWVMROS1_f824abfa97f6f24934e04094d66170167f19a14fb933f325cbef4ed6fca16630",maxWireBytes:262,minWireBytes:6,cartesianMaxWireBytes:262n,aggregateWireBytes:0,constants:Object.freeze({"NORMAL":0,"THROTTLED":1,"OVERHEATED":2,"ERROR":3}),
defaultValue(){return {"can_id":0,"joint_name":"","state":0};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxMotorProtectionSize(Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxMotorProtectionSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxMotorProtectionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,262,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxMotorProtectionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxMotorProtectionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["can_id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["joint_name"],256).length);
integer(value["state"],1,false);n=addSize(n,1);
if(n>262)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxMotorProtectionWrite(value,w){
w.v.setUint8(w.at,value["can_id"],true);w.at+=1;
w.text(value["joint_name"],256);
w.v.setUint8(w.at,value["state"],true);w.at+=1;
}
function Ros2Clearpath_motor_msgsMsgLynxMotorProtectionRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["can_id"]=decoded;
}
{
const decoded=r.text(256);
value["joint_name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["state"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["can_id"];
out["can_id"]=field;
}
{const field=value["joint_name"];
out["joint_name"]=field;
}
{const field=value["state"];
out["state"]=field;
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
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiDebug codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxMultiDebugCodec=Object.freeze({codecIdentity:"UWVMROS1_056abd2153e7f1a6df223bf5daba581e6c218bb7ef365444fc5c3171dfc62e95",maxWireBytes:1308,minWireBytes:16,cartesianMaxWireBytes:1308n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxMultiDebugSize(Ros2Clearpath_motor_msgsMsgLynxMultiDebugCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxMultiDebugCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxMultiDebugSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxMultiDebugWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1308,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxMultiDebugRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxMultiDebugSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length>4)bad();
n=addSize(n,4);
for(const element of value["drivers"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgLynxDebugSize(element));
}
if(n>1308)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxMultiDebugWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["drivers"].length,true);w.at+=4;
for(const element of value["drivers"]){
Ros2Clearpath_motor_msgsMsgLynxDebugWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgLynxMultiDebugRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>4)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxDebugRead(r);
value["drivers"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxMultiDebugCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgLynxDebugCapture(element);
}out["drivers"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiFeedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_2c83cade6bec4d766fd2cff97338642b3d2065d41e9968ccf5054c6f231242c7",maxWireBytes:1252,minWireBytes:16,cartesianMaxWireBytes:1252n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackSize(Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1252,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length>4)bad();
n=addSize(n,4);
for(const element of value["drivers"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgLynxFeedbackSize(element));
}
if(n>1252)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["drivers"].length,true);w.at+=4;
for(const element of value["drivers"]){
Ros2Clearpath_motor_msgsMsgLynxFeedbackWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>4)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxFeedbackRead(r);
value["drivers"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgLynxFeedbackCapture(element);
}out["drivers"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_b68a7ab9bc5312c59d2ef86e49313468b631e8cacdf52468374b67f4eced5c6d",maxWireBytes:546,minWireBytes:34,cartesianMaxWireBytes:546n,aggregateWireBytes:0,constants:Object.freeze({"STATUS_FLAG_ADC_CALIBRATED":0,"STATUS_FLAG_FOC_ENABLED":1,"STATUS_FLAG_CALIBRATION_REQUESTED":2,"STATUS_FLAG_CALIBRATION_CANCELLED":3,"STATUS_FLAG_ESTOPPED":4,"WARNING_FLAG_MOTOR_THERMISTOR":0,"WARNING_FLAG_PCB_THERMISTOR":1,"WARNING_FLAG_PHASE":2,"WARNING_FLAG_PHASE_A":3,"WARNING_FLAG_PHASE_B":4,"WARNING_FLAG_PHASE_C":5,"WARNING_FLAG_ENCODER_INDEX":6,"WARNING_FLAG_ENCODER_OUTPUT_A":7,"WARNING_FLAG_ENCODER_OUTPUT_B":8,"ERROR_FLAG_NOT_CALIBRATED":0,"ERROR_FLAG_MOTOR_FAULT":1,"ERROR_FLAG_MOTOR_STALLING":2,"ERROR_FLAG_MOTOR_THERMISTOR":3,"ERROR_FLAG_PCB_THERMISTOR":4,"ERROR_FLAG_PHASE":5,"ERROR_FLAG_PHASE_A":6,"ERROR_FLAG_PHASE_B":7,"ERROR_FLAG_PHASE_C":8,"ERROR_FLAG_ENCODER_POWER":9,"ERROR_FLAG_ENCODER_INDEX":10,"ERROR_FLAG_ENCODER_OUTPUT_A":11,"ERROR_FLAG_ENCODER_OUTPUT_B":12}),
defaultValue(){return {"can_id":0,"joint_name":"","firmware_version":"","motor_temperature":0,"mcu_temperature":0,"pcb_temperature":0,"stopped":false,"status_flags":0,"warning_flags":0,"error_flags":0};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxStatusSize(Ros2Clearpath_motor_msgsMsgLynxStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxStatusCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxStatusSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,546,90);if(owned.length<34)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["can_id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["joint_name"],256).length);
n=addSize(n,4+textBytes(value["firmware_version"],256).length);
if(typeof value["motor_temperature"]!=="number"||!Number.isFinite(value["motor_temperature"])||!Number.isFinite(Math.fround(value["motor_temperature"])))bad();n=addSize(n,4);
if(typeof value["mcu_temperature"]!=="number"||!Number.isFinite(value["mcu_temperature"])||!Number.isFinite(Math.fround(value["mcu_temperature"])))bad();n=addSize(n,4);
if(typeof value["pcb_temperature"]!=="number"||!Number.isFinite(value["pcb_temperature"])||!Number.isFinite(Math.fround(value["pcb_temperature"])))bad();n=addSize(n,4);
if(typeof value["stopped"]!=="boolean")bad();n=addSize(n,1);
integer(value["status_flags"],4,false);n=addSize(n,4);
integer(value["warning_flags"],4,false);n=addSize(n,4);
integer(value["error_flags"],4,false);n=addSize(n,4);
if(n>546)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxStatusWrite(value,w){
w.v.setUint8(w.at,value["can_id"],true);w.at+=1;
w.text(value["joint_name"],256);
w.text(value["firmware_version"],256);
w.v.setFloat32(w.at,value["motor_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["mcu_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["pcb_temperature"],true);w.at+=4;
w.v.setUint8(w.at,value["stopped"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["status_flags"],true);w.at+=4;
w.v.setUint32(w.at,value["warning_flags"],true);w.at+=4;
w.v.setUint32(w.at,value["error_flags"],true);w.at+=4;
}
function Ros2Clearpath_motor_msgsMsgLynxStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["can_id"]=decoded;
}
{
const decoded=r.text(256);
value["joint_name"]=decoded;
}
{
const decoded=r.text(256);
value["firmware_version"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["motor_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mcu_temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pcb_temperature"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["stopped"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["status_flags"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["warning_flags"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error_flags"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["can_id"];
out["can_id"]=field;
}
{const field=value["joint_name"];
out["joint_name"]=field;
}
{const field=value["firmware_version"];
out["firmware_version"]=field;
}
{const field=value["motor_temperature"];
out["motor_temperature"]=field;
}
{const field=value["mcu_temperature"];
out["mcu_temperature"]=field;
}
{const field=value["pcb_temperature"];
out["pcb_temperature"]=field;
}
{const field=value["stopped"];
out["stopped"]=field;
}
{const field=value["status_flags"];
out["status_flags"]=field;
}
{const field=value["warning_flags"];
out["warning_flags"]=field;
}
{const field=value["error_flags"];
out["error_flags"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxMultiStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_595cd0847b4332e14cc067b916166e9a6e1d1e12a7a68cdb7e1cd414a9ea9ce1",maxWireBytes:2328,minWireBytes:16,cartesianMaxWireBytes:2328n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxMultiStatusSize(Ros2Clearpath_motor_msgsMsgLynxMultiStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxMultiStatusCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxMultiStatusSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxMultiStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2328,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxMultiStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxMultiStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length>4)bad();
n=addSize(n,4);
for(const element of value["drivers"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgLynxStatusSize(element));
}
if(n>2328)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxMultiStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["drivers"].length,true);w.at+=4;
for(const element of value["drivers"]){
Ros2Clearpath_motor_msgsMsgLynxStatusWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgLynxMultiStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>4)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxStatusRead(r);
value["drivers"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxMultiStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgLynxStatusCapture(element);
}out["drivers"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/LynxSystemProtection codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgLynxSystemProtectionCodec=Object.freeze({codecIdentity:"UWVMROS1_4d662de03c34bb633a122399be1a1aaea6187550c83231bf6d8a06b95fa82d9f",maxWireBytes:669,minWireBytes:17,cartesianMaxWireBytes:669n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"system_state":0,"motor_states":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgLynxSystemProtectionSize(Ros2Clearpath_motor_msgsMsgLynxSystemProtectionCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgLynxSystemProtectionCapture(value),n=Ros2Clearpath_motor_msgsMsgLynxSystemProtectionSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgLynxSystemProtectionWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,669,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgLynxSystemProtectionRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgLynxSystemProtectionSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["system_state"],1,false);n=addSize(n,1);
if(!Array.isArray(value["motor_states"])||value["motor_states"].length>2)bad();
n=addSize(n,4);
for(const element of value["motor_states"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgLynxMotorProtectionSize(element));
}
if(n>669)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgLynxSystemProtectionWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["system_state"],true);w.at+=1;
w.v.setUint32(w.at,value["motor_states"].length,true);w.at+=4;
for(const element of value["motor_states"]){
Ros2Clearpath_motor_msgsMsgLynxMotorProtectionWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgLynxSystemProtectionRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["system_state"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["motor_states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxMotorProtectionRead(r);
value["motor_states"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgLynxSystemProtectionCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["system_state"];
out["system_state"]=field;
}
{const field=value["motor_states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCapture(element);
}out["motor_states"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/PumaFeedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgPumaFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_c020162b07a55ac5f41df6021c0d3053d7302863f4e544a738b6cc5bc4dc9659",maxWireBytes:293,minWireBytes:37,cartesianMaxWireBytes:293n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"device_number":0,"device_name":"","duty_cycle":0,"current":0,"travel":0,"speed":0,"setpoint":0};},
validateValue(value){Ros2Clearpath_motor_msgsMsgPumaFeedbackSize(Ros2Clearpath_motor_msgsMsgPumaFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgPumaFeedbackCapture(value),n=Ros2Clearpath_motor_msgsMsgPumaFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgPumaFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,293,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgPumaFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgPumaFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["device_number"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["device_name"],256).length);
if(typeof value["duty_cycle"]!=="number"||!Number.isFinite(value["duty_cycle"])||!Number.isFinite(Math.fround(value["duty_cycle"])))bad();n=addSize(n,4);
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["travel"]!=="number"||!Number.isFinite(value["travel"]))bad();n=addSize(n,8);
if(typeof value["speed"]!=="number"||!Number.isFinite(value["speed"]))bad();n=addSize(n,8);
if(typeof value["setpoint"]!=="number"||!Number.isFinite(value["setpoint"]))bad();n=addSize(n,8);
if(n>293)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgPumaFeedbackWrite(value,w){
w.v.setUint8(w.at,value["device_number"],true);w.at+=1;
w.text(value["device_name"],256);
w.v.setFloat32(w.at,value["duty_cycle"],true);w.at+=4;
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat64(w.at,value["travel"],true);w.at+=8;
w.v.setFloat64(w.at,value["speed"],true);w.at+=8;
w.v.setFloat64(w.at,value["setpoint"],true);w.at+=8;
}
function Ros2Clearpath_motor_msgsMsgPumaFeedbackRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["device_number"]=decoded;
}
{
const decoded=r.text(256);
value["device_name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["duty_cycle"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["current"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["travel"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["speed"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["setpoint"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsMsgPumaFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["device_number"];
out["device_number"]=field;
}
{const field=value["device_name"];
out["device_name"]=field;
}
{const field=value["duty_cycle"];
out["duty_cycle"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["travel"];
out["travel"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["setpoint"];
out["setpoint"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/PumaMultiFeedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_638309564580ad23d585799dcd46156e1e4261ebf9febfbb91754c250907c947",maxWireBytes:1316,minWireBytes:16,cartesianMaxWireBytes:1316n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers_feedback":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackSize(Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackCapture(value),n=Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1316,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers_feedback"])||value["drivers_feedback"].length>4)bad();
n=addSize(n,4);
for(const element of value["drivers_feedback"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgPumaFeedbackSize(element));
}
if(n>1316)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["drivers_feedback"].length,true);w.at+=4;
for(const element of value["drivers_feedback"]){
Ros2Clearpath_motor_msgsMsgPumaFeedbackWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>4)bad();value["drivers_feedback"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgPumaFeedbackRead(r);
value["drivers_feedback"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers_feedback"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgPumaFeedbackCapture(element);
}out["drivers_feedback"]=values;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/PumaStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgPumaStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_a027229db2c8500f707d97064548b27e77e24ae4840ad9722d2dc74726099da8",maxWireBytes:279,minWireBytes:23,cartesianMaxWireBytes:279n,aggregateWireBytes:0,constants:Object.freeze({"MODE_VOLTAGE":0,"MODE_CURRENT":1,"MODE_SPEED":2,"MODE_POSITION":3,"MODE_VCOMP":4,"FAULT_CURRENT":1,"FAULT_TEMPERATURE":2,"FAULT_BUS_VOLTAGE":4,"FAULT_BRIDGE_DRIVER":8}),
defaultValue(){return {"device_number":0,"device_name":"","bus_voltage":0,"temperature":0,"output_voltage":0,"analog_input":0,"mode":0,"fault":0};},
validateValue(value){Ros2Clearpath_motor_msgsMsgPumaStatusSize(Ros2Clearpath_motor_msgsMsgPumaStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgPumaStatusCapture(value),n=Ros2Clearpath_motor_msgsMsgPumaStatusSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgPumaStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,279,90);if(owned.length<23)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgPumaStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgPumaStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["device_number"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["device_name"],256).length);
if(typeof value["bus_voltage"]!=="number"||!Number.isFinite(value["bus_voltage"])||!Number.isFinite(Math.fround(value["bus_voltage"])))bad();n=addSize(n,4);
if(typeof value["temperature"]!=="number"||!Number.isFinite(value["temperature"])||!Number.isFinite(Math.fround(value["temperature"])))bad();n=addSize(n,4);
if(typeof value["output_voltage"]!=="number"||!Number.isFinite(value["output_voltage"])||!Number.isFinite(Math.fround(value["output_voltage"])))bad();n=addSize(n,4);
if(typeof value["analog_input"]!=="number"||!Number.isFinite(value["analog_input"])||!Number.isFinite(Math.fround(value["analog_input"])))bad();n=addSize(n,4);
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["fault"],1,false);n=addSize(n,1);
if(n>279)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgPumaStatusWrite(value,w){
w.v.setUint8(w.at,value["device_number"],true);w.at+=1;
w.text(value["device_name"],256);
w.v.setFloat32(w.at,value["bus_voltage"],true);w.at+=4;
w.v.setFloat32(w.at,value["temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["output_voltage"],true);w.at+=4;
w.v.setFloat32(w.at,value["analog_input"],true);w.at+=4;
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["fault"],true);w.at+=1;
}
function Ros2Clearpath_motor_msgsMsgPumaStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["device_number"]=decoded;
}
{
const decoded=r.text(256);
value["device_name"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["bus_voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["temperature"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["output_voltage"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["analog_input"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["fault"]=decoded;
}
return value;}
function Ros2Clearpath_motor_msgsMsgPumaStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["device_number"];
out["device_number"]=field;
}
{const field=value["device_name"];
out["device_name"]=field;
}
{const field=value["bus_voltage"];
out["bus_voltage"]=field;
}
{const field=value["temperature"];
out["temperature"]=field;
}
{const field=value["output_voltage"];
out["output_voltage"]=field;
}
{const field=value["analog_input"];
out["analog_input"]=field;
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["fault"];
out["fault"]=field;
}
return out;}
/** Complete ROS2 clearpath_motor_msgs/msg/PumaMultiStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_motor_msgsMsgPumaMultiStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_30a0210fea4fffe07371c898c365ef9696501a7541935dd80770671a7163c8f2",maxWireBytes:1260,minWireBytes:16,cartesianMaxWireBytes:1260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers":[]};},
validateValue(value){Ros2Clearpath_motor_msgsMsgPumaMultiStatusSize(Ros2Clearpath_motor_msgsMsgPumaMultiStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_motor_msgsMsgPumaMultiStatusCapture(value),n=Ros2Clearpath_motor_msgsMsgPumaMultiStatusSize(owned),w=new Writer(n);Ros2Clearpath_motor_msgsMsgPumaMultiStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1260,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_motor_msgsMsgPumaMultiStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_motor_msgsMsgPumaMultiStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length>4)bad();
n=addSize(n,4);
for(const element of value["drivers"]){
n=addSize(n,Ros2Clearpath_motor_msgsMsgPumaStatusSize(element));
}
if(n>1260)bad(90);return n;}
function Ros2Clearpath_motor_msgsMsgPumaMultiStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["drivers"].length,true);w.at+=4;
for(const element of value["drivers"]){
Ros2Clearpath_motor_msgsMsgPumaStatusWrite(element,w);
}
}
function Ros2Clearpath_motor_msgsMsgPumaMultiStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>4)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgPumaStatusRead(r);
value["drivers"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_motor_msgsMsgPumaMultiStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_motor_msgsMsgPumaStatusCapture(element);
}out["drivers"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/DisplayStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgDisplayStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_e4585117414581491304a26d528b44c770799f622c3f1cde037078f5cb2462c2",maxWireBytes:106,minWireBytes:8,cartesianMaxWireBytes:106n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"string1":"","string2":""};},
validateValue(value){Ros2Clearpath_platform_msgsMsgDisplayStatusSize(Ros2Clearpath_platform_msgsMsgDisplayStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgDisplayStatusCapture(value),n=Ros2Clearpath_platform_msgsMsgDisplayStatusSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgDisplayStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,106,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgDisplayStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgDisplayStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["string1"],49).length);
n=addSize(n,4+textBytes(value["string2"],49).length);
if(n>106)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgDisplayStatusWrite(value,w){
w.text(value["string1"],49);
w.text(value["string2"],49);
}
function Ros2Clearpath_platform_msgsMsgDisplayStatusRead(r){const value={};
{
const decoded=r.text(49);
value["string1"]=decoded;
}
{
const decoded=r.text(49);
value["string2"]=decoded;
}
return value;}
function Ros2Clearpath_platform_msgsMsgDisplayStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["string1"];
out["string1"]=field;
}
{const field=value["string2"];
out["string2"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/Drive codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgDriveCodec=Object.freeze({codecIdentity:"UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785",maxWireBytes:9,minWireBytes:9,cartesianMaxWireBytes:9n,aggregateWireBytes:0,constants:Object.freeze({"MODE_VELOCITY":0,"MODE_PWM":1,"MODE_NONE":-1,"LEFT":0,"RIGHT":1}),
defaultValue(){return {"mode":0,"drivers":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Clearpath_platform_msgsMsgDriveSize(Ros2Clearpath_platform_msgsMsgDriveCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgDriveCapture(value),n=Ros2Clearpath_platform_msgsMsgDriveSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgDriveWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,9,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgDriveRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgDriveSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>9)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgDriveWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
for(const element of value["drivers"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_platform_msgsMsgDriveRead(r){const value={};
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
function Ros2Clearpath_platform_msgsMsgDriveCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["drivers"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/DriveFeedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgDriveFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"current":0,"duty_cycle":0,"bridge_temperature":0,"motor_temperature":0,"measured_velocity":0,"measured_travel":0,"driver_fault":false};},
validateValue(value){Ros2Clearpath_platform_msgsMsgDriveFeedbackSize(Ros2Clearpath_platform_msgsMsgDriveFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgDriveFeedbackCapture(value),n=Ros2Clearpath_platform_msgsMsgDriveFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgDriveFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgDriveFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgDriveFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["current"]!=="number"||!Number.isFinite(value["current"])||!Number.isFinite(Math.fround(value["current"])))bad();n=addSize(n,4);
if(typeof value["duty_cycle"]!=="number"||!Number.isFinite(value["duty_cycle"])||!Number.isFinite(Math.fround(value["duty_cycle"])))bad();n=addSize(n,4);
if(typeof value["bridge_temperature"]!=="number"||!Number.isFinite(value["bridge_temperature"])||!Number.isFinite(Math.fround(value["bridge_temperature"])))bad();n=addSize(n,4);
if(typeof value["motor_temperature"]!=="number"||!Number.isFinite(value["motor_temperature"])||!Number.isFinite(Math.fround(value["motor_temperature"])))bad();n=addSize(n,4);
if(typeof value["measured_velocity"]!=="number"||!Number.isFinite(value["measured_velocity"])||!Number.isFinite(Math.fround(value["measured_velocity"])))bad();n=addSize(n,4);
if(typeof value["measured_travel"]!=="number"||!Number.isFinite(value["measured_travel"])||!Number.isFinite(Math.fround(value["measured_travel"])))bad();n=addSize(n,4);
if(typeof value["driver_fault"]!=="boolean")bad();n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgDriveFeedbackWrite(value,w){
w.v.setFloat32(w.at,value["current"],true);w.at+=4;
w.v.setFloat32(w.at,value["duty_cycle"],true);w.at+=4;
w.v.setFloat32(w.at,value["bridge_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["motor_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_velocity"],true);w.at+=4;
w.v.setFloat32(w.at,value["measured_travel"],true);w.at+=4;
w.v.setUint8(w.at,value["driver_fault"]?1:0,true);w.at+=1;
}
function Ros2Clearpath_platform_msgsMsgDriveFeedbackRead(r){const value={};
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
function Ros2Clearpath_platform_msgsMsgDriveFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 clearpath_platform_msgs/msg/Fans codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgFansCodec=Object.freeze({codecIdentity:"UWVMROS1_a7d96f2242d992baf96f31bd8f93088547f92c72912c241c2d237f7714800cc8",maxWireBytes:6,minWireBytes:4,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"CC01_FAN1":0,"CC01_FAN2":1,"CC01_FAN3":2,"CC01_FAN4":3,"CC01_FAN5":4,"CC01_FAN6":5,"CC01_FAN7":6,"CC01_FAN8":7,"R100_EQUIPMENT_BAY_INTAKE":0,"R100_EQUIPMENT_BAY_EXHAUST":1,"R100_CHARGER_BAY_INTAKE":2,"R100_CHARGER_BAY_EXHAUST":3,"R100_USER_BAY_INTAKE":4,"R100_USER_BAY_EXHAUST":5,"FAN_OFF":0,"FAN_ON_HIGH":1,"FAN_ON_LOW":2}),
defaultValue(){return {"fans":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgFansSize(Ros2Clearpath_platform_msgsMsgFansCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgFansCapture(value),n=Ros2Clearpath_platform_msgsMsgFansSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgFansWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgFansRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgFansSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["fans"])||value["fans"].length>2)bad();
n=addSize(n,4);
for(const element of value["fans"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>6)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgFansWrite(value,w){
w.v.setUint32(w.at,value["fans"].length,true);w.at+=4;
for(const element of value["fans"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Clearpath_platform_msgsMsgFansRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["fans"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["fans"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_platform_msgsMsgFansCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["fans"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["fans"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/Feedback codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442",maxWireBytes:192,minWireBytes:64,cartesianMaxWireBytes:192n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"drivers":Array.from({length:2},()=>(Ros2Clearpath_platform_msgsMsgDriveFeedbackCodec.defaultValue())),"commanded_mode":0,"actual_mode":0};},
validateValue(value){Ros2Clearpath_platform_msgsMsgFeedbackSize(Ros2Clearpath_platform_msgsMsgFeedbackCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgFeedbackCapture(value),n=Ros2Clearpath_platform_msgsMsgFeedbackSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,192,90);if(owned.length<64)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["drivers"])||value["drivers"].length!==2)bad();
for(const element of value["drivers"]){
n=addSize(n,Ros2Clearpath_platform_msgsMsgDriveFeedbackSize(element));
}
integer(value["commanded_mode"],1,true);n=addSize(n,1);
integer(value["actual_mode"],1,true);n=addSize(n,1);
if(n>192)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgFeedbackWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
for(const element of value["drivers"]){
Ros2Clearpath_platform_msgsMsgDriveFeedbackWrite(element,w);
}
w.v.setInt8(w.at,value["commanded_mode"],true);w.at+=1;
w.v.setInt8(w.at,value["actual_mode"],true);w.at+=1;
}
function Ros2Clearpath_platform_msgsMsgFeedbackRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=2;if(count>2)bad();value["drivers"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_platform_msgsMsgDriveFeedbackRead(r);
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
function Ros2Clearpath_platform_msgsMsgFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["drivers"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_platform_msgsMsgDriveFeedbackCapture(element);
}out["drivers"]=values;
}
{const field=value["commanded_mode"];
out["commanded_mode"]=field;
}
{const field=value["actual_mode"];
out["actual_mode"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/RGB codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgRGBCodec=Object.freeze({codecIdentity:"UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"red":0,"green":0,"blue":0};},
validateValue(value){Ros2Clearpath_platform_msgsMsgRGBSize(Ros2Clearpath_platform_msgsMsgRGBCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgRGBCapture(value),n=Ros2Clearpath_platform_msgsMsgRGBSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgRGBWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgRGBRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgRGBSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["red"],1,false);n=addSize(n,1);
integer(value["green"],1,false);n=addSize(n,1);
integer(value["blue"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgRGBWrite(value,w){
w.v.setUint8(w.at,value["red"],true);w.at+=1;
w.v.setUint8(w.at,value["green"],true);w.at+=1;
w.v.setUint8(w.at,value["blue"],true);w.at+=1;
}
function Ros2Clearpath_platform_msgsMsgRGBRead(r){const value={};
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
function Ros2Clearpath_platform_msgsMsgRGBCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 clearpath_platform_msgs/msg/Lights codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgLightsCodec=Object.freeze({codecIdentity:"UWVMROS1_3f975a3ec1fca4f8add9523d4f9f99963be520c0e2982e644551b71cbd4b8a27",maxWireBytes:10,minWireBytes:4,cartesianMaxWireBytes:10n,aggregateWireBytes:0,constants:Object.freeze({"D100_LIGHTS_REAR_LEFT":0,"D100_LIGHTS_FRONT_LEFT":1,"D100_LIGHTS_FRONT_RIGHT":2,"D100_LIGHTS_REAR_RIGHT":3,"D150_LIGHTS_REAR_LEFT":0,"D150_LIGHTS_FRONT_LEFT":1,"D150_LIGHTS_FRONT_RIGHT":2,"D150_LIGHTS_REAR_RIGHT":3,"R100_LIGHTS_FRONT_PORT_UPPER":0,"R100_LIGHTS_FRONT_PORT_LOWER":1,"R100_LIGHTS_FRONT_STARBOARD_UPPER":2,"R100_LIGHTS_FRONT_STARBOARD_LOWER":3,"R100_LIGHTS_REAR_PORT_UPPER":4,"R100_LIGHTS_REAR_PORT_LOWER":5,"R100_LIGHTS_REAR_STARBOARD_UPPER":6,"R100_LIGHTS_REAR_STARBOARD_LOWER":7,"W200_LIGHTS_FRONT_LEFT":0,"W200_LIGHTS_FRONT_RIGHT":1,"W200_LIGHTS_REAR_LEFT":2,"W200_LIGHTS_REAR_RIGHT":3,"A300_LIGHTS_FRONT_RIGHT":0,"A300_LIGHTS_FRONT_LEFT":1,"A300_LIGHTS_REAR_LEFT":2,"A300_LIGHTS_REAR_RIGHT":3}),
defaultValue(){return {"lights":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgLightsSize(Ros2Clearpath_platform_msgsMsgLightsCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgLightsCapture(value),n=Ros2Clearpath_platform_msgsMsgLightsSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgLightsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,10,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgLightsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgLightsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["lights"])||value["lights"].length>2)bad();
n=addSize(n,4);
for(const element of value["lights"]){
n=addSize(n,Ros2Clearpath_platform_msgsMsgRGBSize(element));
}
if(n>10)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgLightsWrite(value,w){
w.v.setUint32(w.at,value["lights"].length,true);w.at+=4;
for(const element of value["lights"]){
Ros2Clearpath_platform_msgsMsgRGBWrite(element,w);
}
}
function Ros2Clearpath_platform_msgsMsgLightsRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["lights"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Clearpath_platform_msgsMsgRGBRead(r);
value["lights"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_platform_msgsMsgLightsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lights"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Clearpath_platform_msgsMsgRGBCapture(element);
}out["lights"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/PinoutCommand codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgPinoutCommandCodec=Object.freeze({codecIdentity:"UWVMROS1_b26c2baf1e64cefa889c4ac6bd6b7f4647d9247eca0626b83cd726547a4e80ff",maxWireBytes:18,minWireBytes:8,cartesianMaxWireBytes:18n,aggregateWireBytes:0,constants:Object.freeze({"CC01_USER_PWR_CTRL":0,"D100_AUX_OUT1":0,"D100_AUX_OUT2":1,"D100_AUX_OUT3":2,"D150_AUX_OUT1":0,"D150_AUX_OUT2":1,"D150_AUX_OUT3":2,"CC01_AUX_OUT1":0,"CC01_AUX_OUT2":1,"CC01_AUX_OUT3":2,"CC01_GPO0":3,"CC01_GPO1":4,"CC01_GPO2":5,"CC01_GPO3":6}),
defaultValue(){return {"rails":[],"outputs":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgPinoutCommandSize(Ros2Clearpath_platform_msgsMsgPinoutCommandCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgPinoutCommandCapture(value),n=Ros2Clearpath_platform_msgsMsgPinoutCommandSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgPinoutCommandWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,18,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgPinoutCommandRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgPinoutCommandSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["rails"])||value["rails"].length>2)bad();
n=addSize(n,4);
for(const element of value["rails"]){
if(typeof element!=="boolean")bad();n=addSize(n,1);
}
if(!Array.isArray(value["outputs"])||value["outputs"].length>2)bad();
n=addSize(n,4);
for(const element of value["outputs"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>18)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgPinoutCommandWrite(value,w){
w.v.setUint32(w.at,value["rails"].length,true);w.at+=4;
for(const element of value["rails"]){
w.v.setUint8(w.at,element?1:0,true);w.at+=1;
}
w.v.setUint32(w.at,value["outputs"].length,true);w.at+=4;
for(const element of value["outputs"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_platform_msgsMsgPinoutCommandRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["rails"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rails"].push(decoded!==0);
}}
}
{
{const count=r.u32();if(count>2)bad();value["outputs"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["outputs"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_platform_msgsMsgPinoutCommandCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rails"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["rails"]=values;
}
{const field=value["outputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["outputs"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/PinoutState codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgPinoutStateCodec=Object.freeze({codecIdentity:"UWVMROS1_a936c6d91440709d50fa9b0be8fb567a9ce793e84f57e5379f6a160c8cbf73e7",maxWireBytes:170,minWireBytes:28,cartesianMaxWireBytes:170n,aggregateWireBytes:0,constants:Object.freeze({"CC01_USER_PWR_CTRL":0,"D100_AUX_IN1":0,"D100_AUX_IN2":1,"D100_AUX_IN3":2,"D150_AUX_IN1":0,"D150_AUX_IN2":1,"D150_AUX_IN3":2,"CC01_AUX_IN1":0,"CC01_AUX_IN2":1,"CC01_AUX_IN3":2,"CC01_GPI0":3,"CC01_GPI1":4,"CC01_GPI2":5,"CC01_GPI3":6,"D100_AUX_OUT1":0,"D100_AUX_OUT2":1,"D100_AUX_OUT3":2,"D150_AUX_OUT1":0,"D150_AUX_OUT2":1,"D150_AUX_OUT3":2,"CC01_AUX_OUT1":0,"CC01_AUX_OUT2":1,"CC01_AUX_OUT3":2,"CC01_GPO0":3,"CC01_GPO1":4,"CC01_GPO2":5,"CC01_GPO3":6}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"rails":[],"inputs":[],"outputs":[],"output_periods":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgPinoutStateSize(Ros2Clearpath_platform_msgsMsgPinoutStateCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgPinoutStateCapture(value),n=Ros2Clearpath_platform_msgsMsgPinoutStateSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgPinoutStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,170,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgPinoutStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgPinoutStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["rails"])||value["rails"].length>2)bad();
n=addSize(n,4);
for(const element of value["rails"]){
if(typeof element!=="boolean")bad();n=addSize(n,1);
}
if(!Array.isArray(value["inputs"])||value["inputs"].length>2)bad();
n=addSize(n,4);
for(const element of value["inputs"]){
if(typeof element!=="boolean")bad();n=addSize(n,1);
}
if(!Array.isArray(value["outputs"])||value["outputs"].length>2)bad();
n=addSize(n,4);
for(const element of value["outputs"]){
if(typeof element!=="boolean")bad();n=addSize(n,1);
}
if(!Array.isArray(value["output_periods"])||value["output_periods"].length>2)bad();
n=addSize(n,4);
for(const element of value["output_periods"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>170)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgPinoutStateWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["rails"].length,true);w.at+=4;
for(const element of value["rails"]){
w.v.setUint8(w.at,element?1:0,true);w.at+=1;
}
w.v.setUint32(w.at,value["inputs"].length,true);w.at+=4;
for(const element of value["inputs"]){
w.v.setUint8(w.at,element?1:0,true);w.at+=1;
}
w.v.setUint32(w.at,value["outputs"].length,true);w.at+=4;
for(const element of value["outputs"]){
w.v.setUint8(w.at,element?1:0,true);w.at+=1;
}
w.v.setUint32(w.at,value["output_periods"].length,true);w.at+=4;
for(const element of value["output_periods"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_platform_msgsMsgPinoutStateRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["rails"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rails"].push(decoded!==0);
}}
}
{
{const count=r.u32();if(count>2)bad();value["inputs"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["inputs"].push(decoded!==0);
}}
}
{
{const count=r.u32();if(count>2)bad();value["outputs"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["outputs"].push(decoded!==0);
}}
}
{
{const count=r.u32();if(count>2)bad();value["output_periods"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["output_periods"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_platform_msgsMsgPinoutStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["rails"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["rails"]=values;
}
{const field=value["inputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["inputs"]=values;
}
{const field=value["outputs"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["outputs"]=values;
}
{const field=value["output_periods"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["output_periods"]=values;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/Power codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgPowerCodec=Object.freeze({codecIdentity:"UWVMROS1_3fc241d6b81da2f80bfce22933b81786b5e582715dc66cd238fa40b239e7aecc",maxWireBytes:169,minWireBytes:25,cartesianMaxWireBytes:169n,aggregateWireBytes:0,constants:Object.freeze({"NOT_APPLICABLE":-1,"J100_MEASURED_BATTERY":0,"J100_MEASURED_5V":1,"J100_MEASURED_12V":2,"D100_MEASURED_BATTERY":0,"D100_MEASURED_5V":1,"D100_MEASURED_12V":2,"D150_MEASURED_BATTERY":0,"D150_MEASURED_5V":1,"D150_MEASURED_12V":2,"W200_MEASURED_BATTERY":0,"W200_MEASURED_12V":1,"W200_MEASURED_24V":2,"W200_MEASURED_48V":3,"R100_MEASURED_BATTERY":0,"R100_MEASURED_5V":1,"R100_MEASURED_12V":2,"R100_MEASURED_INVERTER":3,"R100_MEASURED_FRONT_AXLE":4,"R100_MEASURED_REAR_AXLE":5,"R100_MEASURED_LIGHT":6,"A200_BATTERY_VOLTAGE":0,"A200_LEFT_DRIVER_VOLTAGE":1,"A200_RIGHT_DRIVER_VOLTAGE":2,"A200_VOLTAGES_SIZE":3,"CC01_BATTERY_VOLTAGE":0,"CC01_USER_BATTERY_VOLTAGE":1,"CC01_USER_24V_VOLTAGE":2,"CC01_USER_12VB_VOLTAGE":3,"CC01_SYSTEM_12V_VOLTAGE":4,"CC01_EXPANSION_VOLTAGE":5,"CC01_BREAKOUT_24V_AUX_VOLTAGE":6,"CC01_BREAKOUT_12V_AUX_VOLTAGE":7,"CC01_BREAKOUT_USER_12VA_VOLTAGE":8,"CC01_BREAKOUT_LYNX1_VOLTAGE":9,"CC01_BREAKOUT_LYNX2_VOLTAGE":10,"CC01_BREAKOUT_LYNX3_VOLTAGE":11,"CC01_BREAKOUT_LYNX4_VOLTAGE":12,"J100_TOTAL_CURRENT":0,"J100_COMPUTER_CURRENT":1,"J100_DRIVE_CURRENT":2,"J100_USER_CURRENT":3,"D100_TOTAL_CURRENT":0,"D100_COMPUTER_CURRENT":1,"D150_TOTAL_CURRENT":0,"D150_COMPUTER_CURRENT":1,"W200_TOTAL_CURRENT":0,"W200_COMPUTER_CURRENT":1,"W200_12V_CURRENT":2,"W200_24V_CURRENT":3,"R100_TOTAL_CURRENT":0,"A200_MCU_AND_USER_PORT_CURRENT":0,"A200_LEFT_DRIVER_CURRENT":1,"A200_RIGHT_DRIVER_CURRENT":2,"A200_CURRENTS_SIZE":3,"CC01_BATTERY_PACK_CURRENT":0,"CC01_USER_BATTERY_CURRENT":1,"CC01_AUX_HES_CURRENT":2,"CC01_SYSTEM_12V_CURRENT":3,"CC01_24V_AUX_CURRENT":4,"CC01_12V_AUX_CURRENT":5,"CC01_BREAKOUT_USER_24V_CURRENT":6,"CC01_BREAKOUT_USER_12VA_CURRENT":7,"CC01_BREAKOUT_USER_12VB_CURRENT":8,"CC01_BREAKOUT_USER_LYNX1_CURRENT":9,"CC01_BREAKOUT_USER_LYNX2_CURRENT":10,"CC01_BREAKOUT_USER_LYNX3_CURRENT":11,"CC01_BREAKOUT_USER_LYNX4_CURRENT":12}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"shore_power_connected":0,"battery_connected":0,"power_12v_user_nominal":0,"charger_connected":0,"charging_complete":0,"measured_voltages":[],"measured_currents":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgPowerSize(Ros2Clearpath_platform_msgsMsgPowerCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgPowerCapture(value),n=Ros2Clearpath_platform_msgsMsgPowerSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgPowerWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,169,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgPowerRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgPowerSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
integer(value["shore_power_connected"],1,true);n=addSize(n,1);
integer(value["battery_connected"],1,true);n=addSize(n,1);
integer(value["power_12v_user_nominal"],1,true);n=addSize(n,1);
integer(value["charger_connected"],1,true);n=addSize(n,1);
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
if(n>169)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgPowerWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setInt8(w.at,value["shore_power_connected"],true);w.at+=1;
w.v.setInt8(w.at,value["battery_connected"],true);w.at+=1;
w.v.setInt8(w.at,value["power_12v_user_nominal"],true);w.at+=1;
w.v.setInt8(w.at,value["charger_connected"],true);w.at+=1;
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
function Ros2Clearpath_platform_msgsMsgPowerRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
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
value["charger_connected"]=decoded;
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
function Ros2Clearpath_platform_msgsMsgPowerCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
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
{const field=value["charger_connected"];
out["charger_connected"]=field;
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
/** Complete ROS2 clearpath_platform_msgs/msg/Status codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_1bd99cc11ea9efcb51eeba8fc1d599960c16e958a8adf33a56d47fe27be87782",maxWireBytes:676,minWireBytes:36,cartesianMaxWireBytes:676n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"hardware_id":"","firmware_version":"","mcu_uptime":Ros2Builtin_interfacesMsgDurationCodec.defaultValue(),"connection_uptime":Ros2Builtin_interfacesMsgDurationCodec.defaultValue()};},
validateValue(value){Ros2Clearpath_platform_msgsMsgStatusSize(Ros2Clearpath_platform_msgsMsgStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgStatusCapture(value),n=Ros2Clearpath_platform_msgsMsgStatusSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,676,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["hardware_id"],256).length);
n=addSize(n,4+textBytes(value["firmware_version"],256).length);
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["mcu_uptime"]));
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["connection_uptime"]));
if(n>676)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["hardware_id"],256);
w.text(value["firmware_version"],256);
Ros2Builtin_interfacesMsgDurationWrite(value["mcu_uptime"],w);
Ros2Builtin_interfacesMsgDurationWrite(value["connection_uptime"],w);
}
function Ros2Clearpath_platform_msgsMsgStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(256);
value["hardware_id"]=decoded;
}
{
const decoded=r.text(256);
value["firmware_version"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["mcu_uptime"]=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDurationRead(r);
value["connection_uptime"]=decoded;
}
return value;}
function Ros2Clearpath_platform_msgsMsgStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["hardware_id"];
out["hardware_id"]=field;
}
{const field=value["firmware_version"];
out["firmware_version"]=field;
}
{const field=value["mcu_uptime"];
out["mcu_uptime"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
{const field=value["connection_uptime"];
out["connection_uptime"]=Ros2Builtin_interfacesMsgDurationCapture(field);
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/StopStatus codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgStopStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2",maxWireBytes:143,minWireBytes:15,cartesianMaxWireBytes:143n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"stop_power_status":false,"external_stop_present":false,"needs_reset":false};},
validateValue(value){Ros2Clearpath_platform_msgsMsgStopStatusSize(Ros2Clearpath_platform_msgsMsgStopStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgStopStatusCapture(value),n=Ros2Clearpath_platform_msgsMsgStopStatusSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgStopStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,143,90);if(owned.length<15)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgStopStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgStopStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(typeof value["stop_power_status"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["external_stop_present"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["needs_reset"]!=="boolean")bad();n=addSize(n,1);
if(n>143)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgStopStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["stop_power_status"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["external_stop_present"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["needs_reset"]?1:0,true);w.at+=1;
}
function Ros2Clearpath_platform_msgsMsgStopStatusRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
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
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["needs_reset"]=decoded!==0;
}
return value;}
function Ros2Clearpath_platform_msgsMsgStopStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["stop_power_status"];
out["stop_power_status"]=field;
}
{const field=value["external_stop_present"];
out["external_stop_present"]=field;
}
{const field=value["needs_reset"];
out["needs_reset"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/msg/Temperature codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgTemperatureCodec=Object.freeze({codecIdentity:"UWVMROS1_b4ae4d6ace9045ce96b0931f08d3b2c7ec358ab9c6da7022afd9ea489f9b0466",maxWireBytes:152,minWireBytes:16,cartesianMaxWireBytes:152n,aggregateWireBytes:0,constants:Object.freeze({"J100_MCU":0,"J100_PCB":1,"D100_MCU":0,"D100_PCB":1,"D150_MCU":0,"D150_PCB":1,"W200_MCU":0,"R100_MCU":0,"R100_PCB":1,"CC01_MCU":0,"CC01_FAN1":1,"CC01_FAN2":2,"CC01_FAN3":3,"CC01_FAN4":4,"CC01_FAN5":5,"CC01_FAN6":6,"CC01_FAN7":7,"CC01_FAN8":8,"CC01_5V_INDUCTOR":9,"CC01_MAIN_GND_LUG":10,"CC01_24V_DCDC":11,"CC01_12V_DCDC":12,"CC01_EXT_1":13,"CC01_EXT_2":14,"CC01_EXT_3":15,"CC01_EXT_4":16,"CC01_BREAKOUT_1":17,"CC01_BREAKOUT_2":18}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"temperatures":[]};},
validateValue(value){Ros2Clearpath_platform_msgsMsgTemperatureSize(Ros2Clearpath_platform_msgsMsgTemperatureCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgTemperatureCapture(value),n=Ros2Clearpath_platform_msgsMsgTemperatureSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgTemperatureWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,152,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgTemperatureRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgTemperatureSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["temperatures"])||value["temperatures"].length>2)bad();
n=addSize(n,4);
for(const element of value["temperatures"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>152)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgTemperatureWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["temperatures"].length,true);w.at+=4;
for(const element of value["temperatures"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Clearpath_platform_msgsMsgTemperatureRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2)bad();value["temperatures"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["temperatures"].push(decoded);
}}
}
return value;}
function Ros2Clearpath_platform_msgsMsgTemperatureCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["temperatures"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["temperatures"]=values;
}
return out;}
/** Complete ROS2 clearpath_control_msgs/srv/PowerControl_Request codec; maxima are not actual lengths. */
export const Ros2Clearpath_control_msgsSrvPowerControl_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_1ad49d42d8895b3bd219799ab9cc080cf09d23aa4bca4c1172d19bc18fa416c4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"power_on":false};},
validateValue(value){Ros2Clearpath_control_msgsSrvPowerControl_RequestSize(Ros2Clearpath_control_msgsSrvPowerControl_RequestCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_control_msgsSrvPowerControl_RequestCapture(value),n=Ros2Clearpath_control_msgsSrvPowerControl_RequestSize(owned),w=new Writer(n);Ros2Clearpath_control_msgsSrvPowerControl_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Clearpath_control_msgsSrvPowerControl_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_control_msgsSrvPowerControl_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["power_on"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Clearpath_control_msgsSrvPowerControl_RequestWrite(value,w){
w.v.setUint8(w.at,value["power_on"]?1:0,true);w.at+=1;
}
function Ros2Clearpath_control_msgsSrvPowerControl_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["power_on"]=decoded!==0;
}
return value;}
function Ros2Clearpath_control_msgsSrvPowerControl_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["power_on"];
out["power_on"]=field;
}
return out;}
/** Complete ROS2 clearpath_control_msgs/srv/PowerControl_Response codec; maxima are not actual lengths. */
export const Ros2Clearpath_control_msgsSrvPowerControl_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6d74d07d6a8ff1c41ed4ca13939d54e9e642f423dc2ff6598665238ebbf10a8b",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Clearpath_control_msgsSrvPowerControl_ResponseSize(Ros2Clearpath_control_msgsSrvPowerControl_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_control_msgsSrvPowerControl_ResponseCapture(value),n=Ros2Clearpath_control_msgsSrvPowerControl_ResponseSize(owned),w=new Writer(n);Ros2Clearpath_control_msgsSrvPowerControl_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_control_msgsSrvPowerControl_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_control_msgsSrvPowerControl_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Clearpath_control_msgsSrvPowerControl_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Clearpath_control_msgsSrvPowerControl_ResponseRead(r){const value={};
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
function Ros2Clearpath_control_msgsSrvPowerControl_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Request codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"domain_id":0,"robot_namespace":""};},
validateValue(value){Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestSize(Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestCapture(value),n=Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["domain_id"],1,false);n=addSize(n,1);
n=addSize(n,4+textBytes(value["robot_namespace"],256).length);
if(n>261)bad(90);return n;}
function Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestWrite(value,w){
w.v.setUint8(w.at,value["domain_id"],true);w.at+=1;
w.text(value["robot_namespace"],256);
}
function Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["domain_id"]=decoded;
}
{
const decoded=r.text(256);
value["robot_namespace"]=decoded;
}
return value;}
function Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["domain_id"];
out["domain_id"]=field;
}
{const field=value["robot_namespace"];
out["robot_namespace"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Response codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseSize(Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseCapture(value),n=Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseRead(r){const value={};
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
function Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/srv/SetPinout_Request codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsSrvSetPinout_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3a3a2f8f16279c2e7a057353cd1f7f11e1a65e887dd3ce0a4b7f584708afb238",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"state":false,"toggle_period":0};},
validateValue(value){Ros2Clearpath_platform_msgsSrvSetPinout_RequestSize(Ros2Clearpath_platform_msgsSrvSetPinout_RequestCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsSrvSetPinout_RequestCapture(value),n=Ros2Clearpath_platform_msgsSrvSetPinout_RequestSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsSrvSetPinout_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsSrvSetPinout_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsSrvSetPinout_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["state"]!=="boolean")bad();n=addSize(n,1);
integer(value["toggle_period"],4,false);n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros2Clearpath_platform_msgsSrvSetPinout_RequestWrite(value,w){
w.v.setUint8(w.at,value["state"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["toggle_period"],true);w.at+=4;
}
function Ros2Clearpath_platform_msgsSrvSetPinout_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["state"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["toggle_period"]=decoded;
}
return value;}
function Ros2Clearpath_platform_msgsSrvSetPinout_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["state"];
out["state"]=field;
}
{const field=value["toggle_period"];
out["toggle_period"]=field;
}
return out;}
/** Complete ROS2 clearpath_platform_msgs/srv/SetPinout_Response codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsSrvSetPinout_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b643ef5de2dfb725906ef8f11d79601400953211b47220be7a6fcc05d1be8ee3",maxWireBytes:261,minWireBytes:5,cartesianMaxWireBytes:261n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"success":false,"message":""};},
validateValue(value){Ros2Clearpath_platform_msgsSrvSetPinout_ResponseSize(Ros2Clearpath_platform_msgsSrvSetPinout_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsSrvSetPinout_ResponseCapture(value),n=Ros2Clearpath_platform_msgsSrvSetPinout_ResponseSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsSrvSetPinout_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,261,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsSrvSetPinout_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsSrvSetPinout_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["success"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["message"],256).length);
if(n>261)bad(90);return n;}
function Ros2Clearpath_platform_msgsSrvSetPinout_ResponseWrite(value,w){
w.v.setUint8(w.at,value["success"]?1:0,true);w.at+=1;
w.text(value["message"],256);
}
function Ros2Clearpath_platform_msgsSrvSetPinout_ResponseRead(r){const value={};
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
function Ros2Clearpath_platform_msgsSrvSetPinout_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["success"];
out["success"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
