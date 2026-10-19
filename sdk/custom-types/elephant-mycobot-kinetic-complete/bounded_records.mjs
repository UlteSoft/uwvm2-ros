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
/** Complete ROS1 mycobot_communication/MycobotAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_communicationMycobotAnglesSize(Ros1Mycobot_communicationMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotAnglesCapture(value),n=Ros1Mycobot_communicationMycobotAnglesSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_communicationMycobotAnglesRead(r){const value={};
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
function Ros1Mycobot_communicationMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/MycobotCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_communicationMycobotCoordsSize(Ros1Mycobot_communicationMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotCoordsCapture(value),n=Ros1Mycobot_communicationMycobotCoordsSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_communicationMycobotCoordsRead(r){const value={};
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
function Ros1Mycobot_communicationMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_communicationMycobotGripperStatusSize(Ros1Mycobot_communicationMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotGripperStatusCapture(value),n=Ros1Mycobot_communicationMycobotGripperStatusSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/MycobotPumpStatus codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mycobot_communicationMycobotPumpStatusSize(Ros1Mycobot_communicationMycobotPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotPumpStatusCapture(value),n=Ros1Mycobot_communicationMycobotPumpStatusSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mycobot_communicationMycobotPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotPumpStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin2"]=decoded;
}
return value;}
function Ros1Mycobot_communicationMycobotPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
{const field=value["Pin1"];
out["Pin1"]=field;
}
{const field=value["Pin2"];
out["Pin2"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetAnglesSize(Ros1Mycobot_communicationMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetAnglesCapture(value),n=Ros1Mycobot_communicationMycobotSetAnglesSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotSetAnglesRead(r){const value={};
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
function Ros1Mycobot_communicationMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetCoordsSize(Ros1Mycobot_communicationMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetCoordsCapture(value),n=Ros1Mycobot_communicationMycobotSetCoordsSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotSetCoordsRead(r){const value={};
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
function Ros1Mycobot_communicationMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_communicationGetAnglesRequestSize(Ros1Mycobot_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGetAnglesRequestCapture(value),n=Ros1Mycobot_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mycobot_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mycobot_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_communicationGetAnglesResponseSize(Ros1Mycobot_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGetAnglesResponseCapture(value),n=Ros1Mycobot_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Mycobot_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_communicationGetCoordsRequestSize(Ros1Mycobot_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGetCoordsRequestCapture(value),n=Ros1Mycobot_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mycobot_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mycobot_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_communicationGetCoordsResponseSize(Ros1Mycobot_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGetCoordsResponseCapture(value),n=Ros1Mycobot_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mycobot_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_communicationGripperStatusRequestSize(Ros1Mycobot_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGripperStatusRequestCapture(value),n=Ros1Mycobot_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationGripperStatusResponseSize(Ros1Mycobot_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationGripperStatusResponseCapture(value),n=Ros1Mycobot_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mycobot_communicationPumpStatusRequestSize(Ros1Mycobot_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationPumpStatusRequestCapture(value),n=Ros1Mycobot_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mycobot_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mycobot_communicationPumpStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin2"]=decoded;
}
return value;}
function Ros1Mycobot_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
{const field=value["Pin1"];
out["Pin1"]=field;
}
{const field=value["Pin2"];
out["Pin2"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationPumpStatusResponseSize(Ros1Mycobot_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationPumpStatusResponseCapture(value),n=Ros1Mycobot_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_communicationSetAnglesRequestSize(Ros1Mycobot_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetAnglesRequestCapture(value),n=Ros1Mycobot_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Mycobot_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetAnglesResponseSize(Ros1Mycobot_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetAnglesResponseCapture(value),n=Ros1Mycobot_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mycobot_communicationSetCoordsRequestSize(Ros1Mycobot_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetCoordsRequestCapture(value),n=Ros1Mycobot_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mycobot_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mycobot_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Mycobot_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetCoordsResponseSize(Ros1Mycobot_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetCoordsResponseCapture(value),n=Ros1Mycobot_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/MypalAngles codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros1Mypalletizer_communicationMypalAnglesSize(Ros1Mypalletizer_communicationMypalAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalAnglesCapture(value),n=Ros1Mypalletizer_communicationMypalAnglesSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros1Mypalletizer_communicationMypalAnglesRead(r){const value={};
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
function Ros1Mypalletizer_communicationMypalAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/MypalCoords codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros1Mypalletizer_communicationMypalCoordsSize(Ros1Mypalletizer_communicationMypalCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalCoordsCapture(value),n=Ros1Mypalletizer_communicationMypalCoordsSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros1Mypalletizer_communicationMypalCoordsRead(r){const value={};
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
function Ros1Mypalletizer_communicationMypalCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/MypalGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mypalletizer_communicationMypalGripperStatusSize(Ros1Mypalletizer_communicationMypalGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalGripperStatusCapture(value),n=Ros1Mypalletizer_communicationMypalGripperStatusSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationMypalGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationMypalGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/MypalPumpStatus codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mypalletizer_communicationMypalPumpStatusSize(Ros1Mypalletizer_communicationMypalPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalPumpStatusCapture(value),n=Ros1Mypalletizer_communicationMypalPumpStatusSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationMypalPumpStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin2"]=decoded;
}
return value;}
function Ros1Mypalletizer_communicationMypalPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
{const field=value["Pin1"];
out["Pin1"]=field;
}
{const field=value["Pin2"];
out["Pin2"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/MypalSetAngles codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros1Mypalletizer_communicationMypalSetAnglesSize(Ros1Mypalletizer_communicationMypalSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalSetAnglesCapture(value),n=Ros1Mypalletizer_communicationMypalSetAnglesSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationMypalSetAnglesRead(r){const value={};
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
function Ros1Mypalletizer_communicationMypalSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/MypalSetCoords codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationMypalSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448",maxWireBytes:18,minWireBytes:18,cartesianMaxWireBytes:18n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0,"model":0};},
validateValue(value){Ros1Mypalletizer_communicationMypalSetCoordsSize(Ros1Mypalletizer_communicationMypalSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationMypalSetCoordsCapture(value),n=Ros1Mypalletizer_communicationMypalSetCoordsSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationMypalSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,18,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationMypalSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationMypalSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>18)bad(90);return n;}
function Ros1Mypalletizer_communicationMypalSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationMypalSetCoordsRead(r){const value={};
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
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros1Mypalletizer_communicationMypalSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mypalletizer_communicationGetAnglesRequestSize(Ros1Mypalletizer_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGetAnglesRequestCapture(value),n=Ros1Mypalletizer_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mypalletizer_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mypalletizer_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mypalletizer_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mypalletizer_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros1Mypalletizer_communicationGetAnglesResponseSize(Ros1Mypalletizer_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGetAnglesResponseCapture(value),n=Ros1Mypalletizer_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Mypalletizer_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros1Mypalletizer_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Mypalletizer_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mypalletizer_communicationGetCoordsRequestSize(Ros1Mypalletizer_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGetCoordsRequestCapture(value),n=Ros1Mypalletizer_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mypalletizer_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mypalletizer_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mypalletizer_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mypalletizer_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros1Mypalletizer_communicationGetCoordsResponseSize(Ros1Mypalletizer_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGetCoordsResponseCapture(value),n=Ros1Mypalletizer_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Mypalletizer_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros1Mypalletizer_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mypalletizer_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mypalletizer_communicationGripperStatusRequestSize(Ros1Mypalletizer_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGripperStatusRequestCapture(value),n=Ros1Mypalletizer_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mypalletizer_communicationGripperStatusResponseSize(Ros1Mypalletizer_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationGripperStatusResponseCapture(value),n=Ros1Mypalletizer_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mypalletizer_communicationPumpStatusRequestSize(Ros1Mypalletizer_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationPumpStatusRequestCapture(value),n=Ros1Mypalletizer_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mypalletizer_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationPumpStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["Pin2"]=decoded;
}
return value;}
function Ros1Mypalletizer_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
{const field=value["Pin1"];
out["Pin1"]=field;
}
{const field=value["Pin2"];
out["Pin2"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mypalletizer_communicationPumpStatusResponseSize(Ros1Mypalletizer_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationPumpStatusResponseCapture(value),n=Ros1Mypalletizer_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d",maxWireBytes:17,minWireBytes:17,cartesianMaxWireBytes:17n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros1Mypalletizer_communicationSetAnglesRequestSize(Ros1Mypalletizer_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationSetAnglesRequestCapture(value),n=Ros1Mypalletizer_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,17,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>17)bad(90);return n;}
function Ros1Mypalletizer_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Mypalletizer_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mypalletizer_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mypalletizer_communicationSetAnglesResponseSize(Ros1Mypalletizer_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationSetAnglesResponseCapture(value),n=Ros1Mypalletizer_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1",maxWireBytes:18,minWireBytes:18,cartesianMaxWireBytes:18n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0,"model":0};},
validateValue(value){Ros1Mypalletizer_communicationSetCoordsRequestSize(Ros1Mypalletizer_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationSetCoordsRequestCapture(value),n=Ros1Mypalletizer_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,18,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>18)bad(90);return n;}
function Ros1Mypalletizer_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mypalletizer_communicationSetCoordsRequestRead(r){const value={};
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
{
const decoded=r.v.getInt8(r.take(1),true);
value["model"]=decoded;
}
return value;}
function Ros1Mypalletizer_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["model"];
out["model"]=field;
}
return out;}
/** Complete ROS1 mypalletizer_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mypalletizer_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mypalletizer_communicationSetCoordsResponseSize(Ros1Mypalletizer_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mypalletizer_communicationSetCoordsResponseCapture(value),n=Ros1Mypalletizer_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mypalletizer_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mypalletizer_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mypalletizer_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mypalletizer_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mypalletizer_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mypalletizer_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
