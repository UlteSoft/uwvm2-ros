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
export const Ros2Clearpath_platform_msgsMsgFansCodec=Object.freeze({codecIdentity:"UWVMROS1_addc15a9b6db4f7a0eba3545f02ece0800e48796f71e387b4d249af861f95317",maxWireBytes:6,minWireBytes:4,cartesianMaxWireBytes:6n,aggregateWireBytes:0,constants:Object.freeze({"R100_EQUIPMENT_BAY_INTAKE":0,"R100_EQUIPMENT_BAY_EXHAUST":1,"R100_CHARGER_BAY_INTAKE":2,"R100_CHARGER_BAY_EXHAUST":3,"R100_USER_BAY_INTAKE":4,"R100_USER_BAY_EXHAUST":5,"FAN_OFF":0,"FAN_ON_HIGH":1,"FAN_ON_LOW":2}),
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
export const Ros2Clearpath_platform_msgsMsgLightsCodec=Object.freeze({codecIdentity:"UWVMROS1_14da254234c9c897e39bf4ee0db8faad0798d23fe7017195f35f88fb6e0b3332",maxWireBytes:10,minWireBytes:4,cartesianMaxWireBytes:10n,aggregateWireBytes:0,constants:Object.freeze({"D100_LIGHTS_REAR_LEFT":0,"D100_LIGHTS_FRONT_LEFT":1,"D100_LIGHTS_FRONT_RIGHT":2,"D100_LIGHTS_REAR_RIGHT":3,"D150_LIGHTS_REAR_LEFT":0,"D150_LIGHTS_FRONT_LEFT":1,"D150_LIGHTS_FRONT_RIGHT":2,"D150_LIGHTS_REAR_RIGHT":3,"R100_LIGHTS_FRONT_PORT_UPPER":0,"R100_LIGHTS_FRONT_PORT_LOWER":1,"R100_LIGHTS_FRONT_STARBOARD_UPPER":2,"R100_LIGHTS_FRONT_STARBOARD_LOWER":3,"R100_LIGHTS_REAR_PORT_UPPER":4,"R100_LIGHTS_REAR_PORT_LOWER":5,"R100_LIGHTS_REAR_STARBOARD_UPPER":6,"R100_LIGHTS_REAR_STARBOARD_LOWER":7,"W200_LIGHTS_FRONT_LEFT":0,"W200_LIGHTS_FRONT_RIGHT":1,"W200_LIGHTS_REAR_LEFT":2,"W200_LIGHTS_REAR_RIGHT":3}),
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
/** Complete ROS2 clearpath_platform_msgs/msg/Power codec; maxima are not actual lengths. */
export const Ros2Clearpath_platform_msgsMsgPowerCodec=Object.freeze({codecIdentity:"UWVMROS1_e97ba89dea149ff43d0134038985ea45d2f072af592ea128aa5502a053478d22",maxWireBytes:169,minWireBytes:25,cartesianMaxWireBytes:169n,aggregateWireBytes:0,constants:Object.freeze({"NOT_APPLICABLE":-1,"J100_MEASURED_BATTERY":0,"J100_MEASURED_5V":1,"J100_MEASURED_12V":2,"D100_MEASURED_BATTERY":0,"D100_MEASURED_5V":1,"D100_MEASURED_12V":2,"D150_MEASURED_BATTERY":0,"D150_MEASURED_5V":1,"D150_MEASURED_12V":2,"W200_MEASURED_BATTERY":0,"W200_MEASURED_12V":1,"W200_MEASURED_24V":2,"W200_MEASURED_48V":3,"R100_MEASURED_BATTERY":0,"R100_MEASURED_5V":1,"R100_MEASURED_12V":2,"R100_MEASURED_INVERTER":3,"R100_MEASURED_FRONT_AXLE":4,"R100_MEASURED_REAR_AXLE":5,"R100_MEASURED_LIGHT":6,"A200_BATTERY_VOLTAGE":0,"A200_LEFT_DRIVER_VOLTAGE":1,"A200_RIGHT_DRIVER_VOLTAGE":2,"A200_VOLTAGES_SIZE":3,"J100_TOTAL_CURRENT":0,"J100_COMPUTER_CURRENT":1,"J100_DRIVE_CURRENT":2,"J100_USER_CURRENT":3,"D100_TOTAL_CURRENT":0,"D100_COMPUTER_CURRENT":1,"D150_TOTAL_CURRENT":0,"D150_COMPUTER_CURRENT":1,"W200_TOTAL_CURRENT":0,"W200_COMPUTER_CURRENT":1,"W200_12V_CURRENT":2,"W200_24V_CURRENT":3,"R100_TOTAL_CURRENT":0,"A200_MCU_AND_USER_PORT_CURRENT":0,"A200_LEFT_DRIVER_CURRENT":1,"A200_RIGHT_DRIVER_CURRENT":2,"A200_CURRENTS_SIZE":3}),
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
export const Ros2Clearpath_platform_msgsMsgStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_331afb70728e70e0391da1b2a0f2b97fdc8742a363a2b007606121ad778dd5c1",maxWireBytes:684,minWireBytes:44,cartesianMaxWireBytes:684n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"hardware_id":"","firmware_version":"","mcu_uptime":Ros2Builtin_interfacesMsgDurationCodec.defaultValue(),"connection_uptime":Ros2Builtin_interfacesMsgDurationCodec.defaultValue(),"pcb_temperature":0,"mcu_temperature":0};},
validateValue(value){Ros2Clearpath_platform_msgsMsgStatusSize(Ros2Clearpath_platform_msgsMsgStatusCapture(value));return true;},
encode(value){const owned=Ros2Clearpath_platform_msgsMsgStatusCapture(value),n=Ros2Clearpath_platform_msgsMsgStatusSize(owned),w=new Writer(n);Ros2Clearpath_platform_msgsMsgStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,684,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros2Clearpath_platform_msgsMsgStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Clearpath_platform_msgsMsgStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["hardware_id"],256).length);
n=addSize(n,4+textBytes(value["firmware_version"],256).length);
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["mcu_uptime"]));
n=addSize(n,Ros2Builtin_interfacesMsgDurationSize(value["connection_uptime"]));
if(typeof value["pcb_temperature"]!=="number"||!Number.isFinite(value["pcb_temperature"])||!Number.isFinite(Math.fround(value["pcb_temperature"])))bad();n=addSize(n,4);
if(typeof value["mcu_temperature"]!=="number"||!Number.isFinite(value["mcu_temperature"])||!Number.isFinite(Math.fround(value["mcu_temperature"])))bad();n=addSize(n,4);
if(n>684)bad(90);return n;}
function Ros2Clearpath_platform_msgsMsgStatusWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.text(value["hardware_id"],256);
w.text(value["firmware_version"],256);
Ros2Builtin_interfacesMsgDurationWrite(value["mcu_uptime"],w);
Ros2Builtin_interfacesMsgDurationWrite(value["connection_uptime"],w);
w.v.setFloat32(w.at,value["pcb_temperature"],true);w.at+=4;
w.v.setFloat32(w.at,value["mcu_temperature"],true);w.at+=4;
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
{const field=value["pcb_temperature"];
out["pcb_temperature"]=field;
}
{const field=value["mcu_temperature"];
out["mcu_temperature"]=field;
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
