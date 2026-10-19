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
/** Complete ROS2 mecharm_interfaces/msg/MecharmAngles codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_37d12e0fc3b7bbb50bdcee0ca624a5c637f9554f3c64d640f538aeda57417331",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmAnglesSize(Ros2Mecharm_interfacesMsgMecharmAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmAnglesCapture(value),n=Ros2Mecharm_interfacesMsgMecharmAnglesSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mecharm_interfacesMsgMecharmAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmCoords codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_e1bb96b1908c796e2a89eb4dd2402a6cc02bcc5077c52268bb378a52dd3029ed",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmCoordsSize(Ros2Mecharm_interfacesMsgMecharmCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmCoordsCapture(value),n=Ros2Mecharm_interfacesMsgMecharmCoordsSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mecharm_interfacesMsgMecharmCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmGripperStatus codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_2123b61d7b9ec218ee074473c26e5ec82fde818f40796047fc369cb84516e046",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmGripperStatusSize(Ros2Mecharm_interfacesMsgMecharmGripperStatusCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmGripperStatusCapture(value),n=Ros2Mecharm_interfacesMsgMecharmGripperStatusSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesMsgMecharmGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmPumpStatus codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_4d44c84c1fdd3f4db92d9975545348bfd727320e63c9fbfde5adf0b81ca04add",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmPumpStatusSize(Ros2Mecharm_interfacesMsgMecharmPumpStatusCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmPumpStatusCapture(value),n=Ros2Mecharm_interfacesMsgMecharmPumpStatusSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mecharm_interfacesMsgMecharmPumpStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmSetAngles codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_54b634e4c5d0bc9fdf04a98d31816e24d3a9c5eb497bb38fb16e2de593cf81f5",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmSetAnglesSize(Ros2Mecharm_interfacesMsgMecharmSetAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmSetAnglesCapture(value),n=Ros2Mecharm_interfacesMsgMecharmSetAnglesSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mecharm_interfacesMsgMecharmSetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmSetCoords codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesMsgMecharmSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_44944ee22cd4254167083cfdcd7377ee2f59dbe27414932c980670a471a6e7c2",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros2Mecharm_interfacesMsgMecharmSetCoordsSize(Ros2Mecharm_interfacesMsgMecharmSetCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesMsgMecharmSetCoordsCapture(value),n=Ros2Mecharm_interfacesMsgMecharmSetCoordsSize(owned),w=new Writer(n);Ros2Mecharm_interfacesMsgMecharmSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesMsgMecharmSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesMsgMecharmSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Mecharm_interfacesMsgMecharmSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros2Mecharm_interfacesMsgMecharmSetCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesMsgMecharmSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_95b1d7786c03c6fec98fec0706088481fe9c7aa48c9f33f4325031d0437d97df",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mecharm_interfacesSrvGetAngles_RequestSize(Ros2Mecharm_interfacesSrvGetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGetAngles_RequestCapture(value),n=Ros2Mecharm_interfacesSrvGetAngles_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGetAngles_RequestWrite(value,w){
}
function Ros2Mecharm_interfacesSrvGetAngles_RequestRead(r){const value={};
return value;}
function Ros2Mecharm_interfacesSrvGetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b1a035847ac7a888053ba8de7494abb6d2cdc01f61e4d0a51cb3ef6f479adf61",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mecharm_interfacesSrvGetAngles_ResponseSize(Ros2Mecharm_interfacesSrvGetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGetAngles_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvGetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGetAngles_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mecharm_interfacesSrvGetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesSrvGetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_353827e7125cfa6c829f1138cc33901099a26cf73a8d045cf806b427994ccef1",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mecharm_interfacesSrvGetCoords_RequestSize(Ros2Mecharm_interfacesSrvGetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGetCoords_RequestCapture(value),n=Ros2Mecharm_interfacesSrvGetCoords_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGetCoords_RequestWrite(value,w){
}
function Ros2Mecharm_interfacesSrvGetCoords_RequestRead(r){const value={};
return value;}
function Ros2Mecharm_interfacesSrvGetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f7fcb8c4fdb47c06e93a8f67613490f7ab0ef2af7efb02092874722670b36c75",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mecharm_interfacesSrvGetCoords_ResponseSize(Ros2Mecharm_interfacesSrvGetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGetCoords_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvGetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGetCoords_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mecharm_interfacesSrvGetCoords_ResponseRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesSrvGetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GripperStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGripperStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_044bf47303f5da0d0e17e795b8567b714d55680e8144c8e2e71ff7032859dfbb",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mecharm_interfacesSrvGripperStatus_RequestSize(Ros2Mecharm_interfacesSrvGripperStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGripperStatus_RequestCapture(value),n=Ros2Mecharm_interfacesSrvGripperStatus_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGripperStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGripperStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGripperStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGripperStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvGripperStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesSrvGripperStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/GripperStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvGripperStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_5aeee3c79bb6ebe4a4cb77f54552e97dcc155c65151a5965761a360d41acbea3",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mecharm_interfacesSrvGripperStatus_ResponseSize(Ros2Mecharm_interfacesSrvGripperStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvGripperStatus_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvGripperStatus_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvGripperStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvGripperStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvGripperStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesSrvGripperStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvGripperStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesSrvGripperStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/PumpStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvPumpStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f80774682b7b1d447ff2f8dd7f6ac2f917dd4f71ccd3b1685a57b579fa41d6b9",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mecharm_interfacesSrvPumpStatus_RequestSize(Ros2Mecharm_interfacesSrvPumpStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvPumpStatus_RequestCapture(value),n=Ros2Mecharm_interfacesSrvPumpStatus_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvPumpStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvPumpStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvPumpStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mecharm_interfacesSrvPumpStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvPumpStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesSrvPumpStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/PumpStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvPumpStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_935c0e2aaca3f1e8aad8e4487484292b3d30ccdff180a9aaa50f454f75ecff1b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mecharm_interfacesSrvPumpStatus_ResponseSize(Ros2Mecharm_interfacesSrvPumpStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvPumpStatus_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvPumpStatus_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvPumpStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvPumpStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvPumpStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesSrvPumpStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvPumpStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesSrvPumpStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/SetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvSetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_86897875ef9a609098bc1eabba63aec7d960de380db98370e7a67626b2253eeb",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mecharm_interfacesSrvSetAngles_RequestSize(Ros2Mecharm_interfacesSrvSetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvSetAngles_RequestCapture(value),n=Ros2Mecharm_interfacesSrvSetAngles_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvSetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvSetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvSetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mecharm_interfacesSrvSetAngles_RequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvSetAngles_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesSrvSetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/SetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvSetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4782fe6a137dee35bf39d863be0aea118f61dcef092c11b013c44c6f4c129620",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mecharm_interfacesSrvSetAngles_ResponseSize(Ros2Mecharm_interfacesSrvSetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvSetAngles_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvSetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvSetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvSetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvSetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesSrvSetAngles_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvSetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesSrvSetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/SetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvSetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a0daefe14076b73f1a64924ffc15a6b9647abe1b1887377e642f41b1a7920916",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros2Mecharm_interfacesSrvSetCoords_RequestSize(Ros2Mecharm_interfacesSrvSetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvSetCoords_RequestCapture(value),n=Ros2Mecharm_interfacesSrvSetCoords_RequestSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvSetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvSetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvSetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Mecharm_interfacesSrvSetCoords_RequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvSetCoords_RequestRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros2Mecharm_interfacesSrvSetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS2 mecharm_interfaces/srv/SetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mecharm_interfacesSrvSetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_43d02cfe61b6fb08f035e1f8ea78496cdcb488ed4c916f528415dce4ab810b7f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mecharm_interfacesSrvSetCoords_ResponseSize(Ros2Mecharm_interfacesSrvSetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mecharm_interfacesSrvSetCoords_ResponseCapture(value),n=Ros2Mecharm_interfacesSrvSetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mecharm_interfacesSrvSetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mecharm_interfacesSrvSetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mecharm_interfacesSrvSetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mecharm_interfacesSrvSetCoords_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mecharm_interfacesSrvSetCoords_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mecharm_interfacesSrvSetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotAngles codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_38f099339ed5ee9fe56a5b44c3f0849e2ff7ff721b2a95b5cf49e3b92f6a9d82",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotAnglesSize(Ros2Mycobot_interfacesMsgMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotAnglesCapture(value),n=Ros2Mycobot_interfacesMsgMycobotAnglesSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mycobot_interfacesMsgMycobotAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotCoords codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_8d9606c561db7680e816f5a19fd148c374f271acee8b261f7d060f370e0399d7",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotCoordsSize(Ros2Mycobot_interfacesMsgMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotCoordsCapture(value),n=Ros2Mycobot_interfacesMsgMycobotCoordsSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mycobot_interfacesMsgMycobotCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_4dc3ab413af3da01e26a3289ef9e5ef39488d00483631d701b4d617d9c9ba33d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotGripperStatusSize(Ros2Mycobot_interfacesMsgMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotGripperStatusCapture(value),n=Ros2Mycobot_interfacesMsgMycobotGripperStatusSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesMsgMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotPumpStatus codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_860004dc3975ab44b5a1b0a77a9ff01ab58bfc71d7142d842d3bf9c636365e58",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotPumpStatusSize(Ros2Mycobot_interfacesMsgMycobotPumpStatusCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotPumpStatusCapture(value),n=Ros2Mycobot_interfacesMsgMycobotPumpStatusSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mycobot_interfacesMsgMycobotPumpStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_798576c5f0b03e46e5cc5d2edf81bc7042042482ef574643ca136621346d731e",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotSetAnglesSize(Ros2Mycobot_interfacesMsgMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotSetAnglesCapture(value),n=Ros2Mycobot_interfacesMsgMycobotSetAnglesSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_interfacesMsgMycobotSetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/msg/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesMsgMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_462b7629172037a6959d08984896980c8c07ae5b6364c6d848b172dea23cab49",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros2Mycobot_interfacesMsgMycobotSetCoordsSize(Ros2Mycobot_interfacesMsgMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesMsgMycobotSetCoordsCapture(value),n=Ros2Mycobot_interfacesMsgMycobotSetCoordsSize(owned),w=new Writer(n);Ros2Mycobot_interfacesMsgMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesMsgMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesMsgMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Mycobot_interfacesMsgMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros2Mycobot_interfacesMsgMycobotSetCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesMsgMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ea433bdb6e3cb77923720a56bec24aaec62c88480248a4ed425370092474d38e",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mycobot_interfacesSrvGetAngles_RequestSize(Ros2Mycobot_interfacesSrvGetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGetAngles_RequestCapture(value),n=Ros2Mycobot_interfacesSrvGetAngles_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGetAngles_RequestWrite(value,w){
}
function Ros2Mycobot_interfacesSrvGetAngles_RequestRead(r){const value={};
return value;}
function Ros2Mycobot_interfacesSrvGetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_14d6e2d8a6f7234719fae06d8807c651965387e252160df555ede0a070123b8a",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mycobot_interfacesSrvGetAngles_ResponseSize(Ros2Mycobot_interfacesSrvGetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGetAngles_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvGetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGetAngles_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mycobot_interfacesSrvGetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesSrvGetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_31ef03eefc143e9d040572e378a5803f44f68bac6d03fbb44ca27f87fb5fb076",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mycobot_interfacesSrvGetCoords_RequestSize(Ros2Mycobot_interfacesSrvGetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGetCoords_RequestCapture(value),n=Ros2Mycobot_interfacesSrvGetCoords_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGetCoords_RequestWrite(value,w){
}
function Ros2Mycobot_interfacesSrvGetCoords_RequestRead(r){const value={};
return value;}
function Ros2Mycobot_interfacesSrvGetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c9886b5cc5f3c48116a1c884f75ab4af49be46e2c0476ffc3aa8167ff96f7787",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mycobot_interfacesSrvGetCoords_ResponseSize(Ros2Mycobot_interfacesSrvGetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGetCoords_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvGetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGetCoords_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mycobot_interfacesSrvGetCoords_ResponseRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesSrvGetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GripperStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGripperStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6c5ae66e66f12834b7d623e07a5b3ef2b8127cd9cd83cb36d6793a68d921fcd5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mycobot_interfacesSrvGripperStatus_RequestSize(Ros2Mycobot_interfacesSrvGripperStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGripperStatus_RequestCapture(value),n=Ros2Mycobot_interfacesSrvGripperStatus_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGripperStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGripperStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGripperStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGripperStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvGripperStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesSrvGripperStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/GripperStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvGripperStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_530f1e1acaa8cda6b43f8a8c0a38f5376eaebce3e99f7fb194a28143bed2d1ab",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_interfacesSrvGripperStatus_ResponseSize(Ros2Mycobot_interfacesSrvGripperStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvGripperStatus_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvGripperStatus_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvGripperStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvGripperStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvGripperStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesSrvGripperStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvGripperStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesSrvGripperStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/PumpStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvPumpStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_50b7fa285b93dd9e6b3b0eafda0afeb5911ab6e3b79d58f03f1cf1f76e732cd0",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mycobot_interfacesSrvPumpStatus_RequestSize(Ros2Mycobot_interfacesSrvPumpStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvPumpStatus_RequestCapture(value),n=Ros2Mycobot_interfacesSrvPumpStatus_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvPumpStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvPumpStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvPumpStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mycobot_interfacesSrvPumpStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvPumpStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesSrvPumpStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/PumpStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvPumpStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b347313e1d4430f4e78a72fb8c35d53a39d1d391290373f84f1f8b2b44375eb0",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_interfacesSrvPumpStatus_ResponseSize(Ros2Mycobot_interfacesSrvPumpStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvPumpStatus_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvPumpStatus_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvPumpStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvPumpStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvPumpStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesSrvPumpStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvPumpStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesSrvPumpStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/SetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvSetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a06efd8b6af878d55c26fda8a7177a0b6c8b687f7e1b9f2e17e2ecd19c440310",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mycobot_interfacesSrvSetAngles_RequestSize(Ros2Mycobot_interfacesSrvSetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvSetAngles_RequestCapture(value),n=Ros2Mycobot_interfacesSrvSetAngles_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvSetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvSetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvSetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_interfacesSrvSetAngles_RequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvSetAngles_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesSrvSetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/SetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvSetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_af4daae72e9ec28c8848b76b556c9387e65ba9698d8f0f6e5ba17522472ab665",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_interfacesSrvSetAngles_ResponseSize(Ros2Mycobot_interfacesSrvSetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvSetAngles_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvSetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvSetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvSetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvSetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesSrvSetAngles_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvSetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesSrvSetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/SetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvSetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_76a98d5db6f98255046f13855dc7468049894805d83e3d3a7fbdaace40292213",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros2Mycobot_interfacesSrvSetCoords_RequestSize(Ros2Mycobot_interfacesSrvSetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvSetCoords_RequestCapture(value),n=Ros2Mycobot_interfacesSrvSetCoords_RequestSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvSetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvSetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvSetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros2Mycobot_interfacesSrvSetCoords_RequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvSetCoords_RequestRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros2Mycobot_interfacesSrvSetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS2 mycobot_interfaces/srv/SetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_interfacesSrvSetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_75e3ae0e6db60a275b08e450c420969aa1ee05d0f34864e8fb6535d2d13613b9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_interfacesSrvSetCoords_ResponseSize(Ros2Mycobot_interfacesSrvSetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_interfacesSrvSetCoords_ResponseCapture(value),n=Ros2Mycobot_interfacesSrvSetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_interfacesSrvSetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_interfacesSrvSetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_interfacesSrvSetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_interfacesSrvSetCoords_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_interfacesSrvSetCoords_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_interfacesSrvSetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotAngles codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesMsgMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_2351398be27155ac63b8a3d15ffd0d3088dd7cf284182a9b494791b7780aa6da",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesMsgMycobotAnglesSize(Ros2Mycobot_pro450_interfacesMsgMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesMsgMycobotAnglesCapture(value),n=Ros2Mycobot_pro450_interfacesMsgMycobotAnglesSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesMsgMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesMsgMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesMsgMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesMsgMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mycobot_pro450_interfacesMsgMycobotAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesMsgMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotCoords codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesMsgMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_b8513545b0975757c6b7f756a5cedfe8aa0c5d28d0e573adfde001b452e923de",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesMsgMycobotCoordsSize(Ros2Mycobot_pro450_interfacesMsgMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesMsgMycobotCoordsCapture(value),n=Ros2Mycobot_pro450_interfacesMsgMycobotCoordsSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesMsgMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesMsgMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesMsgMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesMsgMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mycobot_pro450_interfacesMsgMycobotCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesMsgMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_1d097932f2e6b9b4e73ff838b5223b585b230e4edc5c229b310eb00404f6e784",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusSize(Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusCapture(value),n=Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_23d208d7fa8f7b7488be7c7db487698db4b227ef65e99c55d996901d27c3e591",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesSize(Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesCapture(value),n=Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_60f40a2f6a577e5785fd69e0cbe9128b301aeaec6b70fd678c71e7f9a5426cea",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsSize(Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsCapture(value),n=Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d2d8197675b9dadd50ae1a22b165a84e2e76b293b28cc70bec211225c60626c4",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestSize(Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestWrite(value,w){
}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestRead(r){const value={};
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_13056bbe3bdef4feb8488783e1c5cf725d1fb94006f36f2221f61081084227f8",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseSize(Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bda408ece90e671ce58ae428f79f01c1429c3b3aa3c1ef2efe9dc045faa9e3c2",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestSize(Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestWrite(value,w){
}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestRead(r){const value={};
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_01e8a3ef4c048100af32d92cd027769e32eacd82dba36dc3bde30c70cb44afdf",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseSize(Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_17cb5e3826409b661c2ad4bad8fde2b4c826dde95d8600e1a17bcf4908eca125",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestSize(Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestWrite(value,w){
}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestRead(r){const value={};
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_6b210e18d0696735196fb2e166b7168165dba6f03b97759a437fc8dc08ab2676",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseSize(Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseWrite(value,w){
w.v.setInt8(w.at,value["gripper_angle"],true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_edc3352d37effe3923c55a024d6acac85a3cb49350974ea35934efa41555ccac",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestSize(Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_65166c2943fd319555cc1a2dcdaf4475b1e7779960907fe5ab6d1b5dc5536fc2",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseSize(Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ec2659577304938ca4f78c1b4add50190766d8d1b551397668ef7ad03e2ccb09",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestSize(Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_6"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["joint_5"];
out["joint_5"]=field;
}
{const field=value["joint_6"];
out["joint_6"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_28dc59c06324913c1ccbe286ca05b9f47585653a8f086fc5d99a01f6e9ee08b4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseSize(Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e0c550200472f77aa582a46a5149c05c95648615a306f254599f820992890c81",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestSize(Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCapture(value),n=Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ry"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rz"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["rz"];
out["rz"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9a5ee934233519095e1b28568490f8d958c319e2544976fae0b7de53a54a3f51",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseSize(Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCapture(value),n=Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalAngles codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_0a2dc324515448310b0f026fb26c6c5c6da452e5e7067096e315abe3ab86a35d",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalAnglesSize(Ros2Mypalletizer_interfacesMsgMypalAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalAnglesCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalAnglesSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros2Mypalletizer_interfacesMsgMypalAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalCoords codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_839491548a07528380403fa31c89889ce82f28b2f82e6fd9474835659714b4f7",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalCoordsSize(Ros2Mypalletizer_interfacesMsgMypalCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalCoordsCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalCoordsSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros2Mypalletizer_interfacesMsgMypalCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalGripperStatus codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_f6e4ad6193272d6401bbbfcc20c0f38b6a147a81d9de1ad956d7c154f29daeb0",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalGripperStatusSize(Ros2Mypalletizer_interfacesMsgMypalGripperStatusCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalGripperStatusCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalGripperStatusSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesMsgMypalGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalPumpStatus codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_008dfd13cad42029330909fb7dc68392ba7f6678ad1baa14a41aad1e759f523c",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalPumpStatusSize(Ros2Mypalletizer_interfacesMsgMypalPumpStatusCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalPumpStatusCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalPumpStatusSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesMsgMypalPumpStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalSetAngles codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_b9fdd6ac956fdeb4934ff41982e616f7c0c45b11169b6d60ba74fbcc61458bb5",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalSetAnglesSize(Ros2Mypalletizer_interfacesMsgMypalSetAnglesCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalSetAnglesCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalSetAnglesSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesMsgMypalSetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalSetCoords codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesMsgMypalSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_0c1b65f543cdd239a5a56a7839ac607c5310cd4145ef6a6da5639a9af8ba4c79",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros2Mypalletizer_interfacesMsgMypalSetCoordsSize(Ros2Mypalletizer_interfacesMsgMypalSetCoordsCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesMsgMypalSetCoordsCapture(value),n=Ros2Mypalletizer_interfacesMsgMypalSetCoordsSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesMsgMypalSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesMsgMypalSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesMsgMypalSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros2Mypalletizer_interfacesMsgMypalSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesMsgMypalSetCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesMsgMypalSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c8124a1063d47b24dae2e279f51d4e48ebf9936a5e305b81b53387a4be344f78",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGetAngles_RequestSize(Ros2Mypalletizer_interfacesSrvGetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGetAngles_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvGetAngles_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGetAngles_RequestWrite(value,w){
}
function Ros2Mypalletizer_interfacesSrvGetAngles_RequestRead(r){const value={};
return value;}
function Ros2Mypalletizer_interfacesSrvGetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_33812a320c14891a5bc76a86ccbb0a5507cfbcc3706e01bc304263edabc6f75d",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGetAngles_ResponseSize(Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvGetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGetAngles_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros2Mypalletizer_interfacesSrvGetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b3db3a0e7d2aed097027798a897caf833f9cc1b8bb027f98c0db016968fa4440",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGetCoords_RequestSize(Ros2Mypalletizer_interfacesSrvGetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGetCoords_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvGetCoords_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGetCoords_RequestWrite(value,w){
}
function Ros2Mypalletizer_interfacesSrvGetCoords_RequestRead(r){const value={};
return value;}
function Ros2Mypalletizer_interfacesSrvGetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9d237cfa03c4b3c17d99fe0a9c08d1cafa3e8d0e404191e2169eea8683c54c84",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGetCoords_ResponseSize(Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvGetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGetCoords_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros2Mypalletizer_interfacesSrvGetCoords_ResponseRead(r){const value={};
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
value["rx"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GripperStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b6d4c5991599ec4ac56e5f65dacd5a1ab89f073e9f07dc2b03692f4289fdcc9d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGripperStatus_RequestSize(Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvGripperStatus_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGripperStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGripperStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGripperStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGripperStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvGripperStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/GripperStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bd0c427dc564ff7201019c6d33c7d0c743a65d263c67eb420b5e653d48e4daf4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseSize(Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/PumpStatus_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6b14af554cb5c65c9411b2c6a76d49cb7cbe06dedcd26d47804d00d12ccf2e2f",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false,"pin1":0,"pin2":0};},
validateValue(value){Ros2Mypalletizer_interfacesSrvPumpStatus_RequestSize(Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvPumpStatus_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvPumpStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvPumpStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvPumpStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
integer(value["pin1"],1,true);n=addSize(n,1);
integer(value["pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvPumpStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["pin2"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvPumpStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["pin2"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["pin1"];
out["pin1"]=field;
}
{const field=value["pin2"];
out["pin2"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/PumpStatus_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_187a703ce648bc82a276961d82ed8abd5365d8db7af3bb211e590620e1bb2b45",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseSize(Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/SetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvSetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3997bb19c183b59fbca1675695ea5ee548a247a34f9682dae3fb68fdf1a155ec",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros2Mypalletizer_interfacesSrvSetAngles_RequestSize(Ros2Mypalletizer_interfacesSrvSetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvSetAngles_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvSetAngles_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvSetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvSetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvSetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvSetAngles_RequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvSetAngles_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesSrvSetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/SetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a7952f549b35aa140adac4f18bbe8538c0f6230aa884459091cd74720ab757c9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mypalletizer_interfacesSrvSetAngles_ResponseSize(Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvSetAngles_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvSetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvSetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvSetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvSetAngles_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvSetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/SetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvSetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c72f4f1cb71f1ad1076bcc89499abbcfc5eab173fadd19c9a266baf1c32d1541",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros2Mypalletizer_interfacesSrvSetCoords_RequestSize(Ros2Mypalletizer_interfacesSrvSetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvSetCoords_RequestCapture(value),n=Ros2Mypalletizer_interfacesSrvSetCoords_RequestSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvSetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvSetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvSetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvSetCoords_RequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvSetCoords_RequestRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros2Mypalletizer_interfacesSrvSetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 mypalletizer_interfaces/srv/SetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0cedaf9087ba0ae3726b9dd0d304665a226fc6df2ee068f6ac2b1c12ce88cb5b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Mypalletizer_interfacesSrvSetCoords_ResponseSize(Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCapture(value),n=Ros2Mypalletizer_interfacesSrvSetCoords_ResponseSize(owned),w=new Writer(n);Ros2Mypalletizer_interfacesSrvSetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Mypalletizer_interfacesSrvSetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Mypalletizer_interfacesSrvSetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Mypalletizer_interfacesSrvSetCoords_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Mypalletizer_interfacesSrvSetCoords_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/msg/MycobotAngles codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesMsgMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_75965ba712c12f465c2424b540359dbc4c88e2e85aa772698ee040db7c32a945",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesMsgMycobotAnglesSize(Ros2Ultraarm_p1_interfacesMsgMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesMsgMycobotAnglesCapture(value),n=Ros2Ultraarm_p1_interfacesMsgMycobotAnglesSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesMsgMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesMsgMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesMsgMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesMsgMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesMsgMycobotAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesMsgMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/msg/MycobotCoords codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesMsgMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_c79774c9af7fb4c150cb18c2510df52bbc7bad3c05b604abf90e19087aa05874",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesMsgMycobotCoordsSize(Ros2Ultraarm_p1_interfacesMsgMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesMsgMycobotCoordsCapture(value),n=Ros2Ultraarm_p1_interfacesMsgMycobotCoordsSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesMsgMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesMsgMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesMsgMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesMsgMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesMsgMycobotCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesMsgMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/msg/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_1efb83ff22c5eb211fe92355cb5358cb991a373693d33180362e43a2ef56d22f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusSize(Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusCapture(value),n=Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/msg/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_cb53aab11e200533386b764a423f86bd7a24c65105236945c6b27ba7a9c3221c",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesSize(Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesCapture(value),n=Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["speed"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/msg/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_fca21ab6eed31f03f0f7c9c6010d893f709f1eebb1bf07c1911452da1c50bbee",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsSize(Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsCapture(value),n=Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["speed"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b2ff68f6919df813ddf49ad9337c2345e3c4e70242ad81f6440d5cd1cd612ff9",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestSize(Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestWrite(value,w){
}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestRead(r){const value={};
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cc8b4259c50ed82d6f3b3bbb9470ac089b127b70ace12457cb41fccb7ae67cbc",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseSize(Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0ed9a0f7124cb00e62b71aecaedb60ea8832a94aaf4dc8f1de943d91dbf74e01",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestSize(Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestWrite(value,w){
}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestRead(r){const value={};
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_47ef794ffd2a3a4125ce3c9cbbd6b0d85fbb42cbfc888dd8fec39020cd454565",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseSize(Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseRead(r){const value={};
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
value["rx"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetGripperValue_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_dad3d4af78250b2b8e44424ee1bb532b1e24bd9b867417fa428dec761fcf4be9",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestSize(Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestWrite(value,w){
}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestRead(r){const value={};
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GetGripperValue_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3b3830d2ae5b4327771923acab70739d05dd3f9c8e2b3b060f6a3136a9e6c647",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseSize(Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],1,true);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseWrite(value,w){
w.v.setInt8(w.at,value["gripper_angle"],true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GripperStatus_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cdaff7754e24b6910102060d2af379d4bcc9c3a647604ccdf2dc8b4a50d854b8",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":false};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestSize(Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestWrite(value,w){
w.v.setUint8(w.at,value["status"]?1:0,true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["status"]=decoded!==0;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/GripperStatus_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_55826c68885c7382e5a9e8a0ec35b312af3b2e974236d80518564c3fb8b9fa48",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseSize(Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/SetAngles_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4184d40dd5cef87f13e913d349e37b0661905c5c9f0d8c5f9cc965ea272be025",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestSize(Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_4"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["speed"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_1"];
out["joint_1"]=field;
}
{const field=value["joint_2"];
out["joint_2"]=field;
}
{const field=value["joint_3"];
out["joint_3"]=field;
}
{const field=value["joint_4"];
out["joint_4"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/SetAngles_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_de517cd68c164d4214aed8ad92f7bb9ee49f24c7e43d96c18e193095b0b23e1a",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseSize(Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/SetCoords_Request codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_df9914ced1ef5f92f6102595de772ed076be380360b8fd5732cf7f0d4c0ec3e3",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestSize(Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCapture(value),n=Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestRead(r){const value={};
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
value["rx"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["speed"]=decoded;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["z"];
out["z"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS2 ultraarm_p1_interfaces/srv/SetCoords_Response codec; maxima are not actual lengths. */
export const Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c66cfcc79da5bbc9a19b077761cfd3e89b3dae56ae604205de6316aa37935d7f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"flag":false};},
validateValue(value){Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseSize(Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCapture(value),n=Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseSize(owned),w=new Writer(n);Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseWrite(value,w){
w.v.setUint8(w.at,value["flag"]?1:0,true);w.at+=1;
}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["flag"]=decoded!==0;
}
return value;}
function Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["flag"];
out["flag"]=field;
}
return out;}
