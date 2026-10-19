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
/** Complete ROS1 mybuddy_communication/MybuddyAngles codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddyAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_f3c510869207e6d72e868ae35ef95d49a1eea5164c7ef1056cb3d335029563e9",maxWireBytes:52,minWireBytes:52,cartesianMaxWireBytes:52n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_0":0,"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"joint_8":0,"joint_9":0,"joint_10":0,"joint_11":0,"joint_12":0};},
validateValue(value){Ros1Mybuddy_communicationMybuddyAnglesSize(Ros1Mybuddy_communicationMybuddyAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddyAnglesCapture(value),n=Ros1Mybuddy_communicationMybuddyAnglesSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddyAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,52,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddyAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddyAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_0"]!=="number"||!Number.isFinite(value["joint_0"])||!Number.isFinite(Math.fround(value["joint_0"])))bad();n=addSize(n,4);
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(typeof value["joint_8"]!=="number"||!Number.isFinite(value["joint_8"])||!Number.isFinite(Math.fround(value["joint_8"])))bad();n=addSize(n,4);
if(typeof value["joint_9"]!=="number"||!Number.isFinite(value["joint_9"])||!Number.isFinite(Math.fround(value["joint_9"])))bad();n=addSize(n,4);
if(typeof value["joint_10"]!=="number"||!Number.isFinite(value["joint_10"])||!Number.isFinite(Math.fround(value["joint_10"])))bad();n=addSize(n,4);
if(typeof value["joint_11"]!=="number"||!Number.isFinite(value["joint_11"])||!Number.isFinite(Math.fround(value["joint_11"])))bad();n=addSize(n,4);
if(typeof value["joint_12"]!=="number"||!Number.isFinite(value["joint_12"])||!Number.isFinite(Math.fround(value["joint_12"])))bad();n=addSize(n,4);
if(n>52)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddyAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_0"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_8"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_9"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_10"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_11"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_12"],true);w.at+=4;
}
function Ros1Mybuddy_communicationMybuddyAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_0"]=decoded;
}
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_9"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_10"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_11"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_12"]=decoded;
}
return value;}
function Ros1Mybuddy_communicationMybuddyAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_0"];
out["joint_0"]=field;
}
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["joint_8"];
out["joint_8"]=field;
}
{const field=value["joint_9"];
out["joint_9"]=field;
}
{const field=value["joint_10"];
out["joint_10"]=field;
}
{const field=value["joint_11"];
out["joint_11"]=field;
}
{const field=value["joint_12"];
out["joint_12"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/MybuddyCoords codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddyCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_a1df376733f2673e25fed814a038e00f556b82c2b8bce3755644904e50c95384",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mybuddy_communicationMybuddyCoordsSize(Ros1Mybuddy_communicationMybuddyCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddyCoordsCapture(value),n=Ros1Mybuddy_communicationMybuddyCoordsSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddyCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddyCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddyCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddyCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mybuddy_communicationMybuddyCoordsRead(r){const value={};
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
function Ros1Mybuddy_communicationMybuddyCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/MybuddyGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddyGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_86fdaf7d3397b26155c891e15d2ebdd903d5899a0895128e720276eb35a63854",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mybuddy_communicationMybuddyGripperStatusSize(Ros1Mybuddy_communicationMybuddyGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddyGripperStatusCapture(value),n=Ros1Mybuddy_communicationMybuddyGripperStatusSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddyGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddyGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddyGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddyGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationMybuddyGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationMybuddyGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/MybuddyPumpStatus codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddyPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_33b6419700f9baf41b584bb464d0234426a04e1bd86d198fe3d66c06845cd80a",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mybuddy_communicationMybuddyPumpStatusSize(Ros1Mybuddy_communicationMybuddyPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddyPumpStatusCapture(value),n=Ros1Mybuddy_communicationMybuddyPumpStatusSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddyPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddyPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddyPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddyPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mybuddy_communicationMybuddyPumpStatusRead(r){const value={};
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
function Ros1Mybuddy_communicationMybuddyPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/MybuddySetAngles codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddySetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_8f41fddfb715bfaa95c90974cf257181a9ae3e2f36969f6fdfdf3ab03b7d4486",maxWireBytes:53,minWireBytes:53,cartesianMaxWireBytes:53n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_0":0,"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"joint_8":0,"joint_9":0,"joint_10":0,"joint_11":0,"joint_12":0,"speed":0};},
validateValue(value){Ros1Mybuddy_communicationMybuddySetAnglesSize(Ros1Mybuddy_communicationMybuddySetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddySetAnglesCapture(value),n=Ros1Mybuddy_communicationMybuddySetAnglesSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddySetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,53,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddySetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddySetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_0"]!=="number"||!Number.isFinite(value["joint_0"])||!Number.isFinite(Math.fround(value["joint_0"])))bad();n=addSize(n,4);
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(typeof value["joint_8"]!=="number"||!Number.isFinite(value["joint_8"])||!Number.isFinite(Math.fround(value["joint_8"])))bad();n=addSize(n,4);
if(typeof value["joint_9"]!=="number"||!Number.isFinite(value["joint_9"])||!Number.isFinite(Math.fround(value["joint_9"])))bad();n=addSize(n,4);
if(typeof value["joint_10"]!=="number"||!Number.isFinite(value["joint_10"])||!Number.isFinite(Math.fround(value["joint_10"])))bad();n=addSize(n,4);
if(typeof value["joint_11"]!=="number"||!Number.isFinite(value["joint_11"])||!Number.isFinite(Math.fround(value["joint_11"])))bad();n=addSize(n,4);
if(typeof value["joint_12"]!=="number"||!Number.isFinite(value["joint_12"])||!Number.isFinite(Math.fround(value["joint_12"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>53)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddySetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_0"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_8"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_9"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_10"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_11"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_12"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mybuddy_communicationMybuddySetAnglesRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_0"]=decoded;
}
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_9"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_10"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_11"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_12"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros1Mybuddy_communicationMybuddySetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_0"];
out["joint_0"]=field;
}
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["joint_8"];
out["joint_8"]=field;
}
{const field=value["joint_9"];
out["joint_9"]=field;
}
{const field=value["joint_10"];
out["joint_10"]=field;
}
{const field=value["joint_11"];
out["joint_11"]=field;
}
{const field=value["joint_12"];
out["joint_12"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/MybuddySetCoords codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationMybuddySetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_bf9f2ee073928aafabbd532644eb57eec70d4b864a3097e2ab1a37840abd3669",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mybuddy_communicationMybuddySetCoordsSize(Ros1Mybuddy_communicationMybuddySetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationMybuddySetCoordsCapture(value),n=Ros1Mybuddy_communicationMybuddySetCoordsSize(owned),w=new Writer(n);Ros1Mybuddy_communicationMybuddySetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationMybuddySetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationMybuddySetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mybuddy_communicationMybuddySetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mybuddy_communicationMybuddySetCoordsRead(r){const value={};
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
function Ros1Mybuddy_communicationMybuddySetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7ba90c12eb91dbcc296c0d61069d0dc12c3ac576c970cd2fe9a0fed29f058e38",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mybuddy_communicationGetAnglesRequestSize(Ros1Mybuddy_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGetAnglesRequestCapture(value),n=Ros1Mybuddy_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mybuddy_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mybuddy_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mybuddy_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mybuddy_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_98085b01a21ae26a74a286d790aab310530fff2a88d7a3c35e071df09abd709d",maxWireBytes:52,minWireBytes:52,cartesianMaxWireBytes:52n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_0":0,"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"joint_8":0,"joint_9":0,"joint_10":0,"joint_11":0,"joint_12":0};},
validateValue(value){Ros1Mybuddy_communicationGetAnglesResponseSize(Ros1Mybuddy_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGetAnglesResponseCapture(value),n=Ros1Mybuddy_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,52,90);if(owned.length<52)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_0"]!=="number"||!Number.isFinite(value["joint_0"])||!Number.isFinite(Math.fround(value["joint_0"])))bad();n=addSize(n,4);
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(typeof value["joint_8"]!=="number"||!Number.isFinite(value["joint_8"])||!Number.isFinite(Math.fround(value["joint_8"])))bad();n=addSize(n,4);
if(typeof value["joint_9"]!=="number"||!Number.isFinite(value["joint_9"])||!Number.isFinite(Math.fround(value["joint_9"])))bad();n=addSize(n,4);
if(typeof value["joint_10"]!=="number"||!Number.isFinite(value["joint_10"])||!Number.isFinite(Math.fround(value["joint_10"])))bad();n=addSize(n,4);
if(typeof value["joint_11"]!=="number"||!Number.isFinite(value["joint_11"])||!Number.isFinite(Math.fround(value["joint_11"])))bad();n=addSize(n,4);
if(typeof value["joint_12"]!=="number"||!Number.isFinite(value["joint_12"])||!Number.isFinite(Math.fround(value["joint_12"])))bad();n=addSize(n,4);
if(n>52)bad(90);return n;}
function Ros1Mybuddy_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_0"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_8"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_9"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_10"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_11"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_12"],true);w.at+=4;
}
function Ros1Mybuddy_communicationGetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_0"]=decoded;
}
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_9"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_10"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_11"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_12"]=decoded;
}
return value;}
function Ros1Mybuddy_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_0"];
out["joint_0"]=field;
}
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["joint_8"];
out["joint_8"]=field;
}
{const field=value["joint_9"];
out["joint_9"]=field;
}
{const field=value["joint_10"];
out["joint_10"]=field;
}
{const field=value["joint_11"];
out["joint_11"]=field;
}
{const field=value["joint_12"];
out["joint_12"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e5b4bd6f4d17f79bf3477c2577e73355b871523291aace267fea201f5a2b43ef",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mybuddy_communicationGetCoordsRequestSize(Ros1Mybuddy_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGetCoordsRequestCapture(value),n=Ros1Mybuddy_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mybuddy_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mybuddy_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mybuddy_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mybuddy_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_604a9a58b11d6e51faee74226c4820db7668b2054b2e48949cf9deb5958f5873",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mybuddy_communicationGetCoordsResponseSize(Ros1Mybuddy_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGetCoordsResponseCapture(value),n=Ros1Mybuddy_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mybuddy_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mybuddy_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mybuddy_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7816f3fa4c8b7068eeb16e74940d2e26401df47b7bc6881d123f94b554523173",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mybuddy_communicationGripperStatusRequestSize(Ros1Mybuddy_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGripperStatusRequestCapture(value),n=Ros1Mybuddy_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b329fd687ef106ed366d80c5651fc536ad266d2b16dee8f985f1bad32738330b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mybuddy_communicationGripperStatusResponseSize(Ros1Mybuddy_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationGripperStatusResponseCapture(value),n=Ros1Mybuddy_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_4261e7e11f0c6065de868444a0be45d363465b0913246b06b4a2c413714d77e9",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mybuddy_communicationPumpStatusRequestSize(Ros1Mybuddy_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationPumpStatusRequestCapture(value),n=Ros1Mybuddy_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mybuddy_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mybuddy_communicationPumpStatusRequestRead(r){const value={};
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
function Ros1Mybuddy_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_975f537d88c880f8daa3459f6762dc6ec7d7eee1f580c0e79f3cb581b77cabd4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mybuddy_communicationPumpStatusResponseSize(Ros1Mybuddy_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationPumpStatusResponseCapture(value),n=Ros1Mybuddy_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_47d19578189892ca10e073d8d250839f1e2b39ebbb3525ed9b03a15af18eb240",maxWireBytes:53,minWireBytes:53,cartesianMaxWireBytes:53n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_0":0,"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"joint_8":0,"joint_9":0,"joint_10":0,"joint_11":0,"joint_12":0,"speed":0};},
validateValue(value){Ros1Mybuddy_communicationSetAnglesRequestSize(Ros1Mybuddy_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationSetAnglesRequestCapture(value),n=Ros1Mybuddy_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,53,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_0"]!=="number"||!Number.isFinite(value["joint_0"])||!Number.isFinite(Math.fround(value["joint_0"])))bad();n=addSize(n,4);
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(typeof value["joint_8"]!=="number"||!Number.isFinite(value["joint_8"])||!Number.isFinite(Math.fround(value["joint_8"])))bad();n=addSize(n,4);
if(typeof value["joint_9"]!=="number"||!Number.isFinite(value["joint_9"])||!Number.isFinite(Math.fround(value["joint_9"])))bad();n=addSize(n,4);
if(typeof value["joint_10"]!=="number"||!Number.isFinite(value["joint_10"])||!Number.isFinite(Math.fround(value["joint_10"])))bad();n=addSize(n,4);
if(typeof value["joint_11"]!=="number"||!Number.isFinite(value["joint_11"])||!Number.isFinite(Math.fround(value["joint_11"])))bad();n=addSize(n,4);
if(typeof value["joint_12"]!=="number"||!Number.isFinite(value["joint_12"])||!Number.isFinite(Math.fround(value["joint_12"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>53)bad(90);return n;}
function Ros1Mybuddy_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_0"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_8"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_9"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_10"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_11"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_12"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mybuddy_communicationSetAnglesRequestRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_0"]=decoded;
}
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_8"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_9"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_10"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_11"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_12"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros1Mybuddy_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["joint_0"];
out["joint_0"]=field;
}
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["joint_8"];
out["joint_8"]=field;
}
{const field=value["joint_9"];
out["joint_9"]=field;
}
{const field=value["joint_10"];
out["joint_10"]=field;
}
{const field=value["joint_11"];
out["joint_11"]=field;
}
{const field=value["joint_12"];
out["joint_12"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7371fa369708af552fd68522d7effdd00bf82bf7e2fe21469ceb210f51582f81",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mybuddy_communicationSetAnglesResponseSize(Ros1Mybuddy_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationSetAnglesResponseCapture(value),n=Ros1Mybuddy_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mybuddy_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_81f7315bebd612a9406a2805b393415b8a1956e49f3d1b8efb21f9e703b0bfe0",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mybuddy_communicationSetCoordsRequestSize(Ros1Mybuddy_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationSetCoordsRequestCapture(value),n=Ros1Mybuddy_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mybuddy_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mybuddy_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mybuddy_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Mybuddy_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mybuddy_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mybuddy_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e5c41ea9420e43a5406ddee23cee961347bfad4f83baf9ebac93d16cdb5d0b21",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mybuddy_communicationSetCoordsResponseSize(Ros1Mybuddy_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mybuddy_communicationSetCoordsResponseCapture(value),n=Ros1Mybuddy_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mybuddy_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mybuddy_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mybuddy_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mybuddy_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mybuddy_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mybuddy_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/MycobotAngles codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_4afa3dc66ce6ffe02170c76da688514a04aa7b47008aeb8f79c44f9ff867d189",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mecharm_communicationMycobotAnglesSize(Ros1Mecharm_communicationMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotAnglesCapture(value),n=Ros1Mecharm_communicationMycobotAnglesSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mecharm_communicationMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mecharm_communicationMycobotAnglesRead(r){const value={};
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
function Ros1Mecharm_communicationMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/MycobotCoords codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_a6a82e22cd0a2f24657c0a253ed00bf31c0a266e6ebaab071cfd369780818d35",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mecharm_communicationMycobotCoordsSize(Ros1Mecharm_communicationMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotCoordsCapture(value),n=Ros1Mecharm_communicationMycobotCoordsSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mecharm_communicationMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mecharm_communicationMycobotCoordsRead(r){const value={};
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
function Ros1Mecharm_communicationMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_07e04dad52b423c371dc5432b68a2f4272cf55328d6f0a10f091dc389d57a4bf",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mecharm_communicationMycobotGripperStatusSize(Ros1Mecharm_communicationMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotGripperStatusCapture(value),n=Ros1Mecharm_communicationMycobotGripperStatusSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/MycobotPumpStatus codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_f559fbf7d5d2c872b3660132eb8c17c79ca3c2d27e975f8fb8fde1d86072a6fa",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mecharm_communicationMycobotPumpStatusSize(Ros1Mecharm_communicationMycobotPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotPumpStatusCapture(value),n=Ros1Mecharm_communicationMycobotPumpStatusSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mecharm_communicationMycobotPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mecharm_communicationMycobotPumpStatusRead(r){const value={};
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
function Ros1Mecharm_communicationMycobotPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_bce09b709e806417595c05e0e795d689b8c53008f9916633d934f935f9389de9",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mecharm_communicationMycobotSetAnglesSize(Ros1Mecharm_communicationMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotSetAnglesCapture(value),n=Ros1Mecharm_communicationMycobotSetAnglesSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mecharm_communicationMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mecharm_communicationMycobotSetAnglesRead(r){const value={};
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
function Ros1Mecharm_communicationMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_68ffa6950e0934b845b2b45140612997c8276a3ac7ed4303e43cd70417491913",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mecharm_communicationMycobotSetCoordsSize(Ros1Mecharm_communicationMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationMycobotSetCoordsCapture(value),n=Ros1Mecharm_communicationMycobotSetCoordsSize(owned),w=new Writer(n);Ros1Mecharm_communicationMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mecharm_communicationMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mecharm_communicationMycobotSetCoordsRead(r){const value={};
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
function Ros1Mecharm_communicationMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_6a8f60e71495c9944e1e97c308d5c992674bd46060b6f6abf2df3f9748c13123",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mecharm_communicationGetAnglesRequestSize(Ros1Mecharm_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGetAnglesRequestCapture(value),n=Ros1Mecharm_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mecharm_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mecharm_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mecharm_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mecharm_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e792a206903dc49fab725e3b3507af3cf4712f10740e9dcceeb90dedfe6ea6e8",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mecharm_communicationGetAnglesResponseSize(Ros1Mecharm_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGetAnglesResponseCapture(value),n=Ros1Mecharm_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mecharm_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mecharm_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Mecharm_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_55d4d1b8965ea12f9b15d4bd5a22c43793d04af452fefb84bd98bc3d7d8da8bf",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mecharm_communicationGetCoordsRequestSize(Ros1Mecharm_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGetCoordsRequestCapture(value),n=Ros1Mecharm_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mecharm_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mecharm_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mecharm_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mecharm_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2230e517685ea625507334067ccceb8424ecf782fcd4163c551d09048e900ca8",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mecharm_communicationGetCoordsResponseSize(Ros1Mecharm_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGetCoordsResponseCapture(value),n=Ros1Mecharm_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mecharm_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mecharm_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mecharm_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_98284fbac3bdb61857b9175b1f82adc7265d0ed418088123d79bf75cb04c9a2c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mecharm_communicationGripperStatusRequestSize(Ros1Mecharm_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGripperStatusRequestCapture(value),n=Ros1Mecharm_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_29cfb0267fbe68d5dafcd3c9d16a8d4dab6b21477921a48ba35135bc0c22bb32",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mecharm_communicationGripperStatusResponseSize(Ros1Mecharm_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationGripperStatusResponseCapture(value),n=Ros1Mecharm_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_aa138db91e46b41659dfc5901225aaaae1c1ae0461c35d3f7a90d0cf86c9920d",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mecharm_communicationPumpStatusRequestSize(Ros1Mecharm_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationPumpStatusRequestCapture(value),n=Ros1Mecharm_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mecharm_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mecharm_communicationPumpStatusRequestRead(r){const value={};
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
function Ros1Mecharm_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_876caa88c833bfa180b8747110683c789aa238c0cda162a8e262c66fddefdd8f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mecharm_communicationPumpStatusResponseSize(Ros1Mecharm_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationPumpStatusResponseCapture(value),n=Ros1Mecharm_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_7ad54edf4730457f2b2124c925b87a99bdbeddd8bfcc1e0d806f7b9dad31507b",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mecharm_communicationSetAnglesRequestSize(Ros1Mecharm_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationSetAnglesRequestCapture(value),n=Ros1Mecharm_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mecharm_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mecharm_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Mecharm_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_940a294ef91d04afeb65b88bbdd91df3a7db0a07435ced3993db3a9d0f574c6e",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mecharm_communicationSetAnglesResponseSize(Ros1Mecharm_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationSetAnglesResponseCapture(value),n=Ros1Mecharm_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mecharm_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_68deaf663a1fe2d3d7434935f10161eb444e7ea2f79e4b746fee9bd0383fc929",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mecharm_communicationSetCoordsRequestSize(Ros1Mecharm_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationSetCoordsRequestCapture(value),n=Ros1Mecharm_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mecharm_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mecharm_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mecharm_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Mecharm_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mecharm_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mecharm_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c66a6e14c4c7eba1b52d7ef57802cb6122a120f692bcc1552c51078301b1b409",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mecharm_communicationSetCoordsResponseSize(Ros1Mecharm_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mecharm_communicationSetCoordsResponseCapture(value),n=Ros1Mecharm_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mecharm_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mecharm_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mecharm_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mecharm_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mecharm_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mecharm_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/MyArmAngles codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_eff11a03e974b4af030c2dc92d0111cf95e0fb3ce004235bada3fe358b63fd4c",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0};},
validateValue(value){Ros1Myarm_communicationMyArmAnglesSize(Ros1Myarm_communicationMyArmAnglesCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmAnglesCapture(value),n=Ros1Myarm_communicationMyArmAnglesSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(n>28)bad(90);return n;}
function Ros1Myarm_communicationMyArmAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
}
function Ros1Myarm_communicationMyArmAnglesRead(r){const value={};
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
return value;}
function Ros1Myarm_communicationMyArmAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/MyArmCoords codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_f0df1605988924f0263805fb2b27ab7ef9e2296691ed15e03a337606362c983c",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Myarm_communicationMyArmCoordsSize(Ros1Myarm_communicationMyArmCoordsCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmCoordsCapture(value),n=Ros1Myarm_communicationMyArmCoordsSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Myarm_communicationMyArmCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Myarm_communicationMyArmCoordsRead(r){const value={};
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
function Ros1Myarm_communicationMyArmCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/MyArmGripperStatus codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_7875df5b50417da1cef60d79c872aff0b40a08f9bbfe80c103729d3c4c892c6f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Myarm_communicationMyArmGripperStatusSize(Ros1Myarm_communicationMyArmGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmGripperStatusCapture(value),n=Ros1Myarm_communicationMyArmGripperStatusSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationMyArmGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationMyArmGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationMyArmGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/MyArmPumpStatus codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_58765b94f1ecb3bb131f6480ea6830631cde9c96eeef94fd9b22598ccc9ea928",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Myarm_communicationMyArmPumpStatusSize(Ros1Myarm_communicationMyArmPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmPumpStatusCapture(value),n=Ros1Myarm_communicationMyArmPumpStatusSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Myarm_communicationMyArmPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Myarm_communicationMyArmPumpStatusRead(r){const value={};
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
function Ros1Myarm_communicationMyArmPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/MyArmSetAngles codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_6acbfd19e3864b66e47b9da5c7c0e8890e7336e8d6c9a5657c4f5eaa5285fe42",maxWireBytes:29,minWireBytes:29,cartesianMaxWireBytes:29n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"speed":0};},
validateValue(value){Ros1Myarm_communicationMyArmSetAnglesSize(Ros1Myarm_communicationMyArmSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmSetAnglesCapture(value),n=Ros1Myarm_communicationMyArmSetAnglesSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,29,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>29)bad(90);return n;}
function Ros1Myarm_communicationMyArmSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Myarm_communicationMyArmSetAnglesRead(r){const value={};
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros1Myarm_communicationMyArmSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/MyArmSetCoords codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationMyArmSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_ce10c29be4cfd832d5076fead55817a0ac7ccde002d5bbbd7a2befd95e9752fd",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Myarm_communicationMyArmSetCoordsSize(Ros1Myarm_communicationMyArmSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationMyArmSetCoordsCapture(value),n=Ros1Myarm_communicationMyArmSetCoordsSize(owned),w=new Writer(n);Ros1Myarm_communicationMyArmSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Myarm_communicationMyArmSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationMyArmSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Myarm_communicationMyArmSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Myarm_communicationMyArmSetCoordsRead(r){const value={};
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
function Ros1Myarm_communicationMyArmSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_18ea682d1c475321b67435e9f207a14627113ad237a38c759cfe4e1c92fded37",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Myarm_communicationGetAnglesRequestSize(Ros1Myarm_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGetAnglesRequestCapture(value),n=Ros1Myarm_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Myarm_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Myarm_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Myarm_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 myarm_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0c68b293384d21d06e23d4aba10b4a4c2eff4e1a0eaa81903e026fe28da410c7",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0};},
validateValue(value){Ros1Myarm_communicationGetAnglesResponseSize(Ros1Myarm_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGetAnglesResponseCapture(value),n=Ros1Myarm_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
if(n>28)bad(90);return n;}
function Ros1Myarm_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
}
function Ros1Myarm_communicationGetAnglesResponseRead(r){const value={};
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
return value;}
function Ros1Myarm_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_04d9d79ad983cf4631cf65a48cae60d982cd0f234f0f4ed1afa0cc630bd63d95",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Myarm_communicationGetCoordsRequestSize(Ros1Myarm_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGetCoordsRequestCapture(value),n=Ros1Myarm_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Myarm_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Myarm_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Myarm_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 myarm_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_752564704c08fb2076df8eea4f6f5b89621041372472f0f99b6627fdfda831b7",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Myarm_communicationGetCoordsResponseSize(Ros1Myarm_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGetCoordsResponseCapture(value),n=Ros1Myarm_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Myarm_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Myarm_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Myarm_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_adec2e0fbf48862c2245aea4c31c59ea12abdec26b4727b14005b65006c58976",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Myarm_communicationGripperStatusRequestSize(Ros1Myarm_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGripperStatusRequestCapture(value),n=Ros1Myarm_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d703a0ec6f562f1deb444313e36e6a728b4e521d1196d90a63725753d5fe9659",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Myarm_communicationGripperStatusResponseSize(Ros1Myarm_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationGripperStatusResponseCapture(value),n=Ros1Myarm_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_eac2f5c006a0fa86df638c52e9cfb7d2c17818ec10e32ad5277af216c6c944f4",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Myarm_communicationPumpStatusRequestSize(Ros1Myarm_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationPumpStatusRequestCapture(value),n=Ros1Myarm_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Myarm_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Myarm_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Myarm_communicationPumpStatusRequestRead(r){const value={};
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
function Ros1Myarm_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_28533ec27c01d85c9a4644959ea8e5c3f2480f5203a9545e96e4242158f08044",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Myarm_communicationPumpStatusResponseSize(Ros1Myarm_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationPumpStatusResponseCapture(value),n=Ros1Myarm_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c15c81907fbace209686a98578019d893d86cecf4782427ac2c3858da8cd3e0a",maxWireBytes:29,minWireBytes:29,cartesianMaxWireBytes:29n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"joint_7":0,"speed":0};},
validateValue(value){Ros1Myarm_communicationSetAnglesRequestSize(Ros1Myarm_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationSetAnglesRequestCapture(value),n=Ros1Myarm_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,29,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros1Myarm_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(typeof value["joint_7"]!=="number"||!Number.isFinite(value["joint_7"])||!Number.isFinite(Math.fround(value["joint_7"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>29)bad(90);return n;}
function Ros1Myarm_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_7"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Myarm_communicationSetAnglesRequestRead(r){const value={};
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
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joint_7"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["speed"]=decoded;
}
return value;}
function Ros1Myarm_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["joint_7"];
out["joint_7"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_dbdbcb001f0478a4bba5ae9d7320d8dbfa4e36a5c1c130215ff5ab864c73af0f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Myarm_communicationSetAnglesResponseSize(Ros1Myarm_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationSetAnglesResponseCapture(value),n=Ros1Myarm_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 myarm_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_86cfc6edb6b86f90a21818e824aea50b697e314687472e1e96e9ce5399ef66c6",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Myarm_communicationSetCoordsRequestSize(Ros1Myarm_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationSetCoordsRequestCapture(value),n=Ros1Myarm_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Myarm_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Myarm_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Myarm_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Myarm_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Myarm_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 myarm_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Myarm_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_52a54371663141d4384dae8969b729baf097a0aa6105f263e78593743e44272d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Myarm_communicationSetCoordsResponseSize(Ros1Myarm_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Myarm_communicationSetCoordsResponseCapture(value),n=Ros1Myarm_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Myarm_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Myarm_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Myarm_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Myarm_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Myarm_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Myarm_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/MycobotAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_67ec61da0c2bf7ee461c21abbfcab14d74b84cb6c9a12430e2a306abdc65690a",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_320_communicationMycobotAnglesSize(Ros1Mycobot_320_communicationMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotAnglesCapture(value),n=Ros1Mycobot_320_communicationMycobotAnglesSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_320_communicationMycobotAnglesRead(r){const value={};
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
function Ros1Mycobot_320_communicationMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/MycobotCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_1f2ada8331783764a52ed35c81c3ce9a2fdd260e2dffa2990751ce5156c3ad03",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_320_communicationMycobotCoordsSize(Ros1Mycobot_320_communicationMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotCoordsCapture(value),n=Ros1Mycobot_320_communicationMycobotCoordsSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_320_communicationMycobotCoordsRead(r){const value={};
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
function Ros1Mycobot_320_communicationMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_efb4ccdb3ce35d67174700f3d3dd7332d0f7a834c5d76b6153e389e031907984",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_320_communicationMycobotGripperStatusSize(Ros1Mycobot_320_communicationMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotGripperStatusCapture(value),n=Ros1Mycobot_320_communicationMycobotGripperStatusSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/MycobotPumpStatus codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotPumpStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_bb1d48067f44b7022820a6175afe05d5ba8caed9f750874a4e703389cb0d83f4",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mycobot_320_communicationMycobotPumpStatusSize(Ros1Mycobot_320_communicationMycobotPumpStatusCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotPumpStatusCapture(value),n=Ros1Mycobot_320_communicationMycobotPumpStatusSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotPumpStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotPumpStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotPumpStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotPumpStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationMycobotPumpStatusRead(r){const value={};
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
function Ros1Mycobot_320_communicationMycobotPumpStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_bf04b1bd24ab2f56c1835d1abe44b73fe023407c3ebdd3d35341242116a5d259",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_320_communicationMycobotSetAnglesSize(Ros1Mycobot_320_communicationMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotSetAnglesCapture(value),n=Ros1Mycobot_320_communicationMycobotSetAnglesSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationMycobotSetAnglesRead(r){const value={};
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
function Ros1Mycobot_320_communicationMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_761235ef96bcc1c687ad8dd3f6d39982d1b0a7a24eaab3bb54feb9780b9e0092",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mycobot_320_communicationMycobotSetCoordsSize(Ros1Mycobot_320_communicationMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationMycobotSetCoordsCapture(value),n=Ros1Mycobot_320_communicationMycobotSetCoordsSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mycobot_320_communicationMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationMycobotSetCoordsRead(r){const value={};
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
function Ros1Mycobot_320_communicationMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fe13e75d9def5e6f7f831297c771cd4d319e1e9862b5026382fed23747b721b8",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_320_communicationGetAnglesRequestSize(Ros1Mycobot_320_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGetAnglesRequestCapture(value),n=Ros1Mycobot_320_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_320_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mycobot_320_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mycobot_320_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_320_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c0cb03f21add6e566a30129ed7ce3174fa86786859c472a848c6768678adc80a",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_320_communicationGetAnglesResponseSize(Ros1Mycobot_320_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGetAnglesResponseCapture(value),n=Ros1Mycobot_320_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_320_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_320_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Mycobot_320_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_82b2ae7ba4bf2aff8cd6c88ae344b376604624612abdd633c930695c1d1650d6",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_320_communicationGetCoordsRequestSize(Ros1Mycobot_320_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGetCoordsRequestCapture(value),n=Ros1Mycobot_320_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_320_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mycobot_320_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mycobot_320_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_320_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_821bac75a42a37fa06f51e57c09839f3557487dc8e883310601cb2e00bb6b05d",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_320_communicationGetCoordsResponseSize(Ros1Mycobot_320_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGetCoordsResponseCapture(value),n=Ros1Mycobot_320_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_320_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_320_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mycobot_320_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_68d072c047f4f026459309282a40c78c5db51571126efb85b7ca9e9f121b5eb5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_320_communicationGripperStatusRequestSize(Ros1Mycobot_320_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGripperStatusRequestCapture(value),n=Ros1Mycobot_320_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9c1387ec1a93d6674ac6dd735b2811957db57359e9e461dcedc70ae6fccc5544",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_320_communicationGripperStatusResponseSize(Ros1Mycobot_320_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationGripperStatusResponseCapture(value),n=Ros1Mycobot_320_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/PumpStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationPumpStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_23a6e68b36341b30ab13026cbddfd01b16d352105bb5faecd7c1ec6ad654ab60",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false,"Pin1":0,"Pin2":0};},
validateValue(value){Ros1Mycobot_320_communicationPumpStatusRequestSize(Ros1Mycobot_320_communicationPumpStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationPumpStatusRequestCapture(value),n=Ros1Mycobot_320_communicationPumpStatusRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationPumpStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationPumpStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationPumpStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
integer(value["Pin1"],1,true);n=addSize(n,1);
integer(value["Pin2"],1,true);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Mycobot_320_communicationPumpStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
w.v.setInt8(w.at,value["Pin1"],true);w.at+=1;
w.v.setInt8(w.at,value["Pin2"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationPumpStatusRequestRead(r){const value={};
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
function Ros1Mycobot_320_communicationPumpStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/PumpStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationPumpStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_037f53ebd0750c76a798ff17507759fcf71aa8a55496e5b151b8d977dff2c1c8",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_320_communicationPumpStatusResponseSize(Ros1Mycobot_320_communicationPumpStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationPumpStatusResponseCapture(value),n=Ros1Mycobot_320_communicationPumpStatusResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationPumpStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationPumpStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationPumpStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationPumpStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationPumpStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationPumpStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_644a7c29bae881d5f29708b552dcd8fd4102289aa396614590bca33dbda9f7da",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_320_communicationSetAnglesRequestSize(Ros1Mycobot_320_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationSetAnglesRequestCapture(value),n=Ros1Mycobot_320_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_320_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Mycobot_320_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_57578e8427fc6a3a89aa595259e03c3498153482afd716dd44c0d09ee9f59f20",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_320_communicationSetAnglesResponseSize(Ros1Mycobot_320_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationSetAnglesResponseCapture(value),n=Ros1Mycobot_320_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_320_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f398444b45a64155c410a44da67043cee13993d75beabd6bc104b23ccd1d6656",maxWireBytes:26,minWireBytes:26,cartesianMaxWireBytes:26n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0,"model":0};},
validateValue(value){Ros1Mycobot_320_communicationSetCoordsRequestSize(Ros1Mycobot_320_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationSetCoordsRequestCapture(value),n=Ros1Mycobot_320_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,26,90);if(owned.length<26)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
integer(value["model"],1,true);n=addSize(n,1);
if(n>26)bad(90);return n;}
function Ros1Mycobot_320_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
w.v.setInt8(w.at,value["model"],true);w.at+=1;
}
function Ros1Mycobot_320_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Mycobot_320_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_320_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_320_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3fef1bfe9036381f8ad45bc1490e96b4917df5689ccf3a84d66e5ba5145e56c4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_320_communicationSetCoordsResponseSize(Ros1Mycobot_320_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_320_communicationSetCoordsResponseCapture(value),n=Ros1Mycobot_320_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_320_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_320_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_320_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_320_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_320_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_320_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
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
/** Complete ROS1 mycobot_communication/MycobotGetGripperValue codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotGetGripperValueCodec=Object.freeze({codecIdentity:"UWVMROS1_8080d3e4a1844f789af2ef6752e8b46654a9c595c7dcbb3495f49e6c62419ae9",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros1Mycobot_communicationMycobotGetGripperValueSize(Ros1Mycobot_communicationMycobotGetGripperValueCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotGetGripperValueCapture(value),n=Ros1Mycobot_communicationMycobotGetGripperValueSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotGetGripperValueWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotGetGripperValueRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotGetGripperValueSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Mycobot_communicationMycobotGetGripperValueWrite(value,w){
w.v.setInt32(w.at,value["gripper_angle"],true);w.at+=4;
}
function Ros1Mycobot_communicationMycobotGetGripperValueRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros1Mycobot_communicationMycobotGetGripperValueCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
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
/** Complete ROS1 mycobot_communication/MycobotSetEndType codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetEndTypeCodec=Object.freeze({codecIdentity:"UWVMROS1_bb540db63ace33482ad35b74951f0423705297f9a70a4c0114cd604f6018f316",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetEndTypeSize(Ros1Mycobot_communicationMycobotSetEndTypeCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetEndTypeCapture(value),n=Ros1Mycobot_communicationMycobotSetEndTypeSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetEndTypeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetEndTypeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetEndTypeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetEndTypeWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotSetEndTypeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationMycobotSetEndTypeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/MycobotSetFreshMode codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetFreshModeCodec=Object.freeze({codecIdentity:"UWVMROS1_8e09301673b7d625945b4cd01e6bdd2ed1b7e6cfa1bd88beda7f409322a62c8f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetFreshModeSize(Ros1Mycobot_communicationMycobotSetFreshModeCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetFreshModeCapture(value),n=Ros1Mycobot_communicationMycobotSetFreshModeSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetFreshModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetFreshModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetFreshModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetFreshModeWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotSetFreshModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationMycobotSetFreshModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/MycobotSetToolReference codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetToolReferenceCodec=Object.freeze({codecIdentity:"UWVMROS1_1fb3b9a6679217b95db0a430f5a1f0bece575615dfc80599277edf90190ea11c",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetToolReferenceSize(Ros1Mycobot_communicationMycobotSetToolReferenceCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetToolReferenceCapture(value),n=Ros1Mycobot_communicationMycobotSetToolReferenceSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetToolReferenceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetToolReferenceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetToolReferenceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetToolReferenceWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_communicationMycobotSetToolReferenceRead(r){const value={};
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
function Ros1Mycobot_communicationMycobotSetToolReferenceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/MycobotSetVisionMode codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationMycobotSetVisionModeCodec=Object.freeze({codecIdentity:"UWVMROS1_c930276fdaaef4870fc6058b156ccc574cc04b4ac96f0bc2f6031aba8016efe9",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationMycobotSetVisionModeSize(Ros1Mycobot_communicationMycobotSetVisionModeCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationMycobotSetVisionModeCapture(value),n=Ros1Mycobot_communicationMycobotSetVisionModeSize(owned),w=new Writer(n);Ros1Mycobot_communicationMycobotSetVisionModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationMycobotSetVisionModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationMycobotSetVisionModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationMycobotSetVisionModeWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationMycobotSetVisionModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationMycobotSetVisionModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
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
/** Complete ROS1 mycobot_communication/SetEndTypeRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetEndTypeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_59248e0ca199fb3bab9b866351648a6d1a58ed2240556122cfecb99331635472",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationSetEndTypeRequestSize(Ros1Mycobot_communicationSetEndTypeRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetEndTypeRequestCapture(value),n=Ros1Mycobot_communicationSetEndTypeRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetEndTypeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetEndTypeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetEndTypeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetEndTypeRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationSetEndTypeRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationSetEndTypeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetEndTypeResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetEndTypeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bf91211b167627be2325f38c0b2981bffa3984848681d52ed3c31d87586f9768",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetEndTypeResponseSize(Ros1Mycobot_communicationSetEndTypeResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetEndTypeResponseCapture(value),n=Ros1Mycobot_communicationSetEndTypeResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetEndTypeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetEndTypeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetEndTypeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetEndTypeResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetEndTypeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetEndTypeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetFreshModeRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetFreshModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b2ed41361e0adfc856bea480c2703f183b397b4f8b53bc561c95be0ec94d9350",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationSetFreshModeRequestSize(Ros1Mycobot_communicationSetFreshModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetFreshModeRequestCapture(value),n=Ros1Mycobot_communicationSetFreshModeRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetFreshModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetFreshModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetFreshModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetFreshModeRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationSetFreshModeRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationSetFreshModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetFreshModeResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetFreshModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e075cb6f6112e26ec799de1a72299af83a6bb2361cb90b6aae88434c33023e6f",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetFreshModeResponseSize(Ros1Mycobot_communicationSetFreshModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetFreshModeResponseCapture(value),n=Ros1Mycobot_communicationSetFreshModeResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetFreshModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetFreshModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetFreshModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetFreshModeResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetFreshModeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetFreshModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetToolReferenceRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetToolReferenceRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_ef6dc2cc6e16fae40d20fa7d55d0d988204f4ca62fb99b755a2693aa628f7a79",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_communicationSetToolReferenceRequestSize(Ros1Mycobot_communicationSetToolReferenceRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetToolReferenceRequestCapture(value),n=Ros1Mycobot_communicationSetToolReferenceRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetToolReferenceRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetToolReferenceRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetToolReferenceRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_communicationSetToolReferenceRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_communicationSetToolReferenceRequestRead(r){const value={};
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
function Ros1Mycobot_communicationSetToolReferenceRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_communication/SetToolReferenceResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetToolReferenceResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a14c04cc61a7b5233212072bd203a643b90c551dec9235e57d7eddf1c5e0b146",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetToolReferenceResponseSize(Ros1Mycobot_communicationSetToolReferenceResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetToolReferenceResponseCapture(value),n=Ros1Mycobot_communicationSetToolReferenceResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetToolReferenceResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetToolReferenceResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetToolReferenceResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetToolReferenceResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetToolReferenceResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetToolReferenceResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetVisionModeRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetVisionModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_2715ecd55f7669963c076c781ceeef4ae54dac5ab71c378737ade7279c115c8b",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_communicationSetVisionModeRequestSize(Ros1Mycobot_communicationSetVisionModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetVisionModeRequestCapture(value),n=Ros1Mycobot_communicationSetVisionModeRequestSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetVisionModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetVisionModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetVisionModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetVisionModeRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_communicationSetVisionModeRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_communicationSetVisionModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_communication/SetVisionModeResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_communicationSetVisionModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2caae9357e25a170c1d26630e666c2818a9a0f305e4608ad07031819f7d89cdb",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_communicationSetVisionModeResponseSize(Ros1Mycobot_communicationSetVisionModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_communicationSetVisionModeResponseCapture(value),n=Ros1Mycobot_communicationSetVisionModeResponseSize(owned),w=new Writer(n);Ros1Mycobot_communicationSetVisionModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_communicationSetVisionModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_communicationSetVisionModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_communicationSetVisionModeResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_communicationSetVisionModeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_communicationSetVisionModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/MycobotAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_cebcee90c89b4e73d096135b0c70c8fb644f6aa32c5db066f0bf90982e25c72d",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotAnglesSize(Ros1Mycobot_pro450_communicationMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotAnglesCapture(value),n=Ros1Mycobot_pro450_communicationMycobotAnglesSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationMycobotAnglesRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/MycobotCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_4a604335875e926bd7593712c93d8802048266e4ea926ba5447fa9b396ea32e6",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotCoordsSize(Ros1Mycobot_pro450_communicationMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotCoordsCapture(value),n=Ros1Mycobot_pro450_communicationMycobotCoordsSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationMycobotCoordsRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/MycobotGetGripperValue codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotGetGripperValueCodec=Object.freeze({codecIdentity:"UWVMROS1_88ad1a27a0982359460b5043bdd91a2c661ca0dc19009473ae7e0220c5215772",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotGetGripperValueSize(Ros1Mycobot_pro450_communicationMycobotGetGripperValueCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotGetGripperValueCapture(value),n=Ros1Mycobot_pro450_communicationMycobotGetGripperValueSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotGetGripperValueWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotGetGripperValueRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotGetGripperValueSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotGetGripperValueWrite(value,w){
w.v.setInt32(w.at,value["gripper_angle"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationMycobotGetGripperValueRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros1Mycobot_pro450_communicationMycobotGetGripperValueCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_7c9ed441c8d5bac4dbf566f7a5052d99f0a01a8dcd281059fd7c329f4b232843",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotGripperStatusSize(Ros1Mycobot_pro450_communicationMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotGripperStatusCapture(value),n=Ros1Mycobot_pro450_communicationMycobotGripperStatusSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_896b2be337b734707291fc763629a14362649079634aaa782630607986be6154",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotSetAnglesSize(Ros1Mycobot_pro450_communicationMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotSetAnglesCapture(value),n=Ros1Mycobot_pro450_communicationMycobotSetAnglesSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationMycobotSetAnglesRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_27409949ade9bb2415cd1af1a278139049bc58098e7de7a5c8adc83d1ad851f6",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotSetCoordsSize(Ros1Mycobot_pro450_communicationMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotSetCoordsCapture(value),n=Ros1Mycobot_pro450_communicationMycobotSetCoordsSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationMycobotSetCoordsRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/MycobotSetFreshMode codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationMycobotSetFreshModeCodec=Object.freeze({codecIdentity:"UWVMROS1_9e06b1944e7fbab814bbaaa85f167d3804b5d11a1968f18b10e973222d0880ec",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_pro450_communicationMycobotSetFreshModeSize(Ros1Mycobot_pro450_communicationMycobotSetFreshModeCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationMycobotSetFreshModeCapture(value),n=Ros1Mycobot_pro450_communicationMycobotSetFreshModeSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationMycobotSetFreshModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationMycobotSetFreshModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationMycobotSetFreshModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationMycobotSetFreshModeWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationMycobotSetFreshModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_pro450_communicationMycobotSetFreshModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_07223209461cde1ca99ba125067ca257ceaa9bd1aeceed1ffeb6a3769c152278",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_pro450_communicationGetAnglesRequestSize(Ros1Mycobot_pro450_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetAnglesRequestCapture(value),n=Ros1Mycobot_pro450_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Mycobot_pro450_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Mycobot_pro450_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_pro450_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_000239e0594bda363723370cdde86974aa367ebf3bc93582779edbb2bf578ffb",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0};},
validateValue(value){Ros1Mycobot_pro450_communicationGetAnglesResponseSize(Ros1Mycobot_pro450_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetAnglesResponseCapture(value),n=Ros1Mycobot_pro450_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_db7906239cfb05812965b398a427fe4e23bf4f0f43fd927c82b0b2370132a649",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_pro450_communicationGetCoordsRequestSize(Ros1Mycobot_pro450_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetCoordsRequestCapture(value),n=Ros1Mycobot_pro450_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Mycobot_pro450_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Mycobot_pro450_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_pro450_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_46aee6d68fc728529b5662b86d4f6aa283ff4709a0fdeafa7604347a0ed50d72",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0};},
validateValue(value){Ros1Mycobot_pro450_communicationGetCoordsResponseSize(Ros1Mycobot_pro450_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetCoordsResponseCapture(value),n=Ros1Mycobot_pro450_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/GetGripperValueRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetGripperValueRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_63c7779d43e5fef7c0ec249a46840bfc1844f55efcd29933f6b7a6a9dbdadec6",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Mycobot_pro450_communicationGetGripperValueRequestSize(Ros1Mycobot_pro450_communicationGetGripperValueRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetGripperValueRequestCapture(value),n=Ros1Mycobot_pro450_communicationGetGripperValueRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetGripperValueRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetGripperValueRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetGripperValueRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetGripperValueRequestWrite(value,w){
}
function Ros1Mycobot_pro450_communicationGetGripperValueRequestRead(r){const value={};
return value;}
function Ros1Mycobot_pro450_communicationGetGripperValueRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 mycobot_pro450_communication/GetGripperValueResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGetGripperValueResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7c15e7468746ac577b70084cee685a3d71265a9f296caf5ae0b21755a2523b9e",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros1Mycobot_pro450_communicationGetGripperValueResponseSize(Ros1Mycobot_pro450_communicationGetGripperValueResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGetGripperValueResponseCapture(value),n=Ros1Mycobot_pro450_communicationGetGripperValueResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGetGripperValueResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGetGripperValueResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGetGripperValueResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGetGripperValueResponseWrite(value,w){
w.v.setInt32(w.at,value["gripper_angle"],true);w.at+=4;
}
function Ros1Mycobot_pro450_communicationGetGripperValueResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros1Mycobot_pro450_communicationGetGripperValueResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_10b38d264413567fdf84efc6b7f8effe15e69956db282f9b66dae8b32eddb363",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Mycobot_pro450_communicationGripperStatusRequestSize(Ros1Mycobot_pro450_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGripperStatusRequestCapture(value),n=Ros1Mycobot_pro450_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_0d8dc6af5bd1643e43105c6a91f9dfef97b23ea5b7ac2d55e3511959f3d75f56",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_pro450_communicationGripperStatusResponseSize(Ros1Mycobot_pro450_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationGripperStatusResponseCapture(value),n=Ros1Mycobot_pro450_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_8868bfafb22b925be720901f303faeec6396f2aab40d9b3e4d94518abd5e7afd",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"joint_5":0,"joint_6":0,"speed":0};},
validateValue(value){Ros1Mycobot_pro450_communicationSetAnglesRequestSize(Ros1Mycobot_pro450_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetAnglesRequestCapture(value),n=Ros1Mycobot_pro450_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(typeof value["joint_5"]!=="number"||!Number.isFinite(value["joint_5"])||!Number.isFinite(Math.fround(value["joint_5"])))bad();n=addSize(n,4);
if(typeof value["joint_6"]!=="number"||!Number.isFinite(value["joint_6"])||!Number.isFinite(Math.fround(value["joint_6"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_6"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2228dd37fed5398cd2a53dd7653d6749fd5b70960c8eaa60fcfb09de4220474c",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_pro450_communicationSetAnglesResponseSize(Ros1Mycobot_pro450_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetAnglesResponseCapture(value),n=Ros1Mycobot_pro450_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_972fe4365e24ef46a54d7f655873ba4351aa6bdfd9caaec83f158655905d536d",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"ry":0,"rz":0,"speed":0};},
validateValue(value){Ros1Mycobot_pro450_communicationSetCoordsRequestSize(Ros1Mycobot_pro450_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetCoordsRequestCapture(value),n=Ros1Mycobot_pro450_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["rz"]!=="number"||!Number.isFinite(value["rz"])||!Number.isFinite(Math.fround(value["rz"])))bad();n=addSize(n,4);
integer(value["speed"],1,true);n=addSize(n,1);
if(n>25)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setFloat32(w.at,value["rz"],true);w.at+=4;
w.v.setInt8(w.at,value["speed"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Mycobot_pro450_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 mycobot_pro450_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9c62a41f3a4198ab13a993036e78164f2cf09bb61b43a23d54f0674dec58435d",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_pro450_communicationSetCoordsResponseSize(Ros1Mycobot_pro450_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetCoordsResponseCapture(value),n=Ros1Mycobot_pro450_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/SetFreshModeRequest codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetFreshModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d744f6c3f5e9ddf6db911e423d5aa4904bbf32ff156121aa4ad7fe5ee8b28628",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Mycobot_pro450_communicationSetFreshModeRequestSize(Ros1Mycobot_pro450_communicationSetFreshModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetFreshModeRequestCapture(value),n=Ros1Mycobot_pro450_communicationSetFreshModeRequestSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetFreshModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetFreshModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetFreshModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetFreshModeRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetFreshModeRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Mycobot_pro450_communicationSetFreshModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 mycobot_pro450_communication/SetFreshModeResponse codec; maxima are not actual lengths. */
export const Ros1Mycobot_pro450_communicationSetFreshModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_97d367a4d675b8762303e66c1d1aaab32277ac2c5fd92103eb60747214d9d431",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Mycobot_pro450_communicationSetFreshModeResponseSize(Ros1Mycobot_pro450_communicationSetFreshModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Mycobot_pro450_communicationSetFreshModeResponseCapture(value),n=Ros1Mycobot_pro450_communicationSetFreshModeResponseSize(owned),w=new Writer(n);Ros1Mycobot_pro450_communicationSetFreshModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Mycobot_pro450_communicationSetFreshModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Mycobot_pro450_communicationSetFreshModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Mycobot_pro450_communicationSetFreshModeResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Mycobot_pro450_communicationSetFreshModeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Mycobot_pro450_communicationSetFreshModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/MycobotAngles codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_6bd94559b201105277e0638cff9820be0cf03efb492bc958a61a78222e1afe67",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotAnglesSize(Ros1Ultraarm_communicationMycobotAnglesCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotAnglesCapture(value),n=Ros1Ultraarm_communicationMycobotAnglesSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros1Ultraarm_communicationMycobotAnglesRead(r){const value={};
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
function Ros1Ultraarm_communicationMycobotAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/MycobotCoords codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_f4228da3bdb19f7a63e4fbdddb7bc271debed1178663a855476c2b2425e36134",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotCoordsSize(Ros1Ultraarm_communicationMycobotCoordsCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotCoordsCapture(value),n=Ros1Ultraarm_communicationMycobotCoordsSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros1Ultraarm_communicationMycobotCoordsRead(r){const value={};
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
function Ros1Ultraarm_communicationMycobotCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/MycobotGetGripperValue codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotGetGripperValueCodec=Object.freeze({codecIdentity:"UWVMROS1_c5c4c82dfab7600d1ee48fc899c1e0906a80b59e03fee89ef75de5e8bbd1a672",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotGetGripperValueSize(Ros1Ultraarm_communicationMycobotGetGripperValueCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotGetGripperValueCapture(value),n=Ros1Ultraarm_communicationMycobotGetGripperValueSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotGetGripperValueWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotGetGripperValueRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotGetGripperValueSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotGetGripperValueWrite(value,w){
w.v.setInt32(w.at,value["gripper_angle"],true);w.at+=4;
}
function Ros1Ultraarm_communicationMycobotGetGripperValueRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros1Ultraarm_communicationMycobotGetGripperValueCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/MycobotGripperStatus codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotGripperStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_c3ca37a8201c045099baec080410ebfc399793a7953d36c0bce7d50e786611ce",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Ultraarm_communicationMycobotGripperStatusSize(Ros1Ultraarm_communicationMycobotGripperStatusCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotGripperStatusCapture(value),n=Ros1Ultraarm_communicationMycobotGripperStatusSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotGripperStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotGripperStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotGripperStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotGripperStatusWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationMycobotGripperStatusRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationMycobotGripperStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/MycobotSetAngles codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotSetAnglesCodec=Object.freeze({codecIdentity:"UWVMROS1_cb0fbf2cc911d7b907fbe1c09f7dbd4643439430b05976ce508bd7dbf71f9baa",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotSetAnglesSize(Ros1Ultraarm_communicationMycobotSetAnglesCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotSetAnglesCapture(value),n=Ros1Ultraarm_communicationMycobotSetAnglesSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotSetAnglesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotSetAnglesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotSetAnglesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotSetAnglesWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros1Ultraarm_communicationMycobotSetAnglesRead(r){const value={};
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
function Ros1Ultraarm_communicationMycobotSetAnglesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/MycobotSetCoords codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotSetCoordsCodec=Object.freeze({codecIdentity:"UWVMROS1_7eb6643462e1eec9cf943d9251c8b5fb47a6c1c63a4381bacd540e22c3584a68",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotSetCoordsSize(Ros1Ultraarm_communicationMycobotSetCoordsCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotSetCoordsCapture(value),n=Ros1Ultraarm_communicationMycobotSetCoordsSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotSetCoordsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotSetCoordsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotSetCoordsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotSetCoordsWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros1Ultraarm_communicationMycobotSetCoordsRead(r){const value={};
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
function Ros1Ultraarm_communicationMycobotSetCoordsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/MycobotSetFreshMode codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationMycobotSetFreshModeCodec=Object.freeze({codecIdentity:"UWVMROS1_0934edaedfc4606d4301b38218c72e6fc323fcea3bb39143bd507ec3b410e638",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Ultraarm_communicationMycobotSetFreshModeSize(Ros1Ultraarm_communicationMycobotSetFreshModeCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationMycobotSetFreshModeCapture(value),n=Ros1Ultraarm_communicationMycobotSetFreshModeSize(owned),w=new Writer(n);Ros1Ultraarm_communicationMycobotSetFreshModeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationMycobotSetFreshModeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationMycobotSetFreshModeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationMycobotSetFreshModeWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Ultraarm_communicationMycobotSetFreshModeRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Ultraarm_communicationMycobotSetFreshModeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/GetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fd6922aabe6ec7f82731d19efb0963753d86a8f412de96f8bc67ea2615a7c860",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ultraarm_communicationGetAnglesRequestSize(Ros1Ultraarm_communicationGetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetAnglesRequestCapture(value),n=Ros1Ultraarm_communicationGetAnglesRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ultraarm_communicationGetAnglesRequestWrite(value,w){
}
function Ros1Ultraarm_communicationGetAnglesRequestRead(r){const value={};
return value;}
function Ros1Ultraarm_communicationGetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ultraarm_communication/GetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d6b80e48675b3a8f262cc463bca84353d52291cd450a656b14f5d81be851a8cf",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0};},
validateValue(value){Ros1Ultraarm_communicationGetAnglesResponseSize(Ros1Ultraarm_communicationGetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetAnglesResponseCapture(value),n=Ros1Ultraarm_communicationGetAnglesResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Ultraarm_communicationGetAnglesResponseWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
}
function Ros1Ultraarm_communicationGetAnglesResponseRead(r){const value={};
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
function Ros1Ultraarm_communicationGetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/GetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_40a6535b429baf0bbe0bc642ade7a8048e693633388ccd061bf12b6d795bbbd1",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ultraarm_communicationGetCoordsRequestSize(Ros1Ultraarm_communicationGetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetCoordsRequestCapture(value),n=Ros1Ultraarm_communicationGetCoordsRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ultraarm_communicationGetCoordsRequestWrite(value,w){
}
function Ros1Ultraarm_communicationGetCoordsRequestRead(r){const value={};
return value;}
function Ros1Ultraarm_communicationGetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ultraarm_communication/GetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4ce97f0eb2846366887ab0ae90d2b2ca8f4571cddb5e0c7095ca7098b2e127a1",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0};},
validateValue(value){Ros1Ultraarm_communicationGetCoordsResponseSize(Ros1Ultraarm_communicationGetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetCoordsResponseCapture(value),n=Ros1Ultraarm_communicationGetCoordsResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Ultraarm_communicationGetCoordsResponseWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
}
function Ros1Ultraarm_communicationGetCoordsResponseRead(r){const value={};
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
function Ros1Ultraarm_communicationGetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/GetGripperValueRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetGripperValueRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f4daff0f9c6ba692e9b20e29cc67066afb3931d146ba995848dcdc83db2b52b1",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Ultraarm_communicationGetGripperValueRequestSize(Ros1Ultraarm_communicationGetGripperValueRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetGripperValueRequestCapture(value),n=Ros1Ultraarm_communicationGetGripperValueRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetGripperValueRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetGripperValueRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetGripperValueRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Ultraarm_communicationGetGripperValueRequestWrite(value,w){
}
function Ros1Ultraarm_communicationGetGripperValueRequestRead(r){const value={};
return value;}
function Ros1Ultraarm_communicationGetGripperValueRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 ultraarm_communication/GetGripperValueResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGetGripperValueResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_ff7452fb5fb1f5a55c8db0a2aa6d18df1a62a85d32a659506dd31a1ce1721c90",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"gripper_angle":0};},
validateValue(value){Ros1Ultraarm_communicationGetGripperValueResponseSize(Ros1Ultraarm_communicationGetGripperValueResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGetGripperValueResponseCapture(value),n=Ros1Ultraarm_communicationGetGripperValueResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGetGripperValueResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGetGripperValueResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGetGripperValueResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["gripper_angle"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Ultraarm_communicationGetGripperValueResponseWrite(value,w){
w.v.setInt32(w.at,value["gripper_angle"],true);w.at+=4;
}
function Ros1Ultraarm_communicationGetGripperValueResponseRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["gripper_angle"]=decoded;
}
return value;}
function Ros1Ultraarm_communicationGetGripperValueResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["gripper_angle"];
out["gripper_angle"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/GripperStatusRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGripperStatusRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_c435d39bef04bd7022e048c0cf364d100bf7a92855f04c816d2a22f0e672d586",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":false};},
validateValue(value){Ros1Ultraarm_communicationGripperStatusRequestSize(Ros1Ultraarm_communicationGripperStatusRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGripperStatusRequestCapture(value),n=Ros1Ultraarm_communicationGripperStatusRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGripperStatusRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGripperStatusRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGripperStatusRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Status"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationGripperStatusRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationGripperStatusRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Status"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationGripperStatusRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/GripperStatusResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationGripperStatusResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_fa0c99ff37acea63c5d507174f9725a9341ffaa007e722e21a8e0b93097e56d4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Ultraarm_communicationGripperStatusResponseSize(Ros1Ultraarm_communicationGripperStatusResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationGripperStatusResponseCapture(value),n=Ros1Ultraarm_communicationGripperStatusResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationGripperStatusResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationGripperStatusResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationGripperStatusResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationGripperStatusResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationGripperStatusResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationGripperStatusResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/SetAnglesRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetAnglesRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_5a1aab8379585f404a15dbfd8832dd6434d969f41681127befca7458181fc65a",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"joint_1":0,"joint_2":0,"joint_3":0,"joint_4":0,"speed":0};},
validateValue(value){Ros1Ultraarm_communicationSetAnglesRequestSize(Ros1Ultraarm_communicationSetAnglesRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetAnglesRequestCapture(value),n=Ros1Ultraarm_communicationSetAnglesRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetAnglesRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetAnglesRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetAnglesRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["joint_1"]!=="number"||!Number.isFinite(value["joint_1"])||!Number.isFinite(Math.fround(value["joint_1"])))bad();n=addSize(n,4);
if(typeof value["joint_2"]!=="number"||!Number.isFinite(value["joint_2"])||!Number.isFinite(Math.fround(value["joint_2"])))bad();n=addSize(n,4);
if(typeof value["joint_3"]!=="number"||!Number.isFinite(value["joint_3"])||!Number.isFinite(Math.fround(value["joint_3"])))bad();n=addSize(n,4);
if(typeof value["joint_4"]!=="number"||!Number.isFinite(value["joint_4"])||!Number.isFinite(Math.fround(value["joint_4"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros1Ultraarm_communicationSetAnglesRequestWrite(value,w){
w.v.setFloat32(w.at,value["joint_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["joint_4"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros1Ultraarm_communicationSetAnglesRequestRead(r){const value={};
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
function Ros1Ultraarm_communicationSetAnglesRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/SetAnglesResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetAnglesResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_f1ecaff9a60a47ef978b0faf2b9b2b9166ae308b3df718b0536536004dc8a3b4",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Ultraarm_communicationSetAnglesResponseSize(Ros1Ultraarm_communicationSetAnglesResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetAnglesResponseCapture(value),n=Ros1Ultraarm_communicationSetAnglesResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetAnglesResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetAnglesResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetAnglesResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationSetAnglesResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationSetAnglesResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationSetAnglesResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/SetCoordsRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetCoordsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_54d83be688da3c885a7166e4d2c1c636a1144ba863a0a388907511896d5626e5",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0,"rx":0,"speed":0};},
validateValue(value){Ros1Ultraarm_communicationSetCoordsRequestSize(Ros1Ultraarm_communicationSetCoordsRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetCoordsRequestCapture(value),n=Ros1Ultraarm_communicationSetCoordsRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetCoordsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetCoordsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetCoordsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
if(n>20)bad(90);return n;}
function Ros1Ultraarm_communicationSetCoordsRequestWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
}
function Ros1Ultraarm_communicationSetCoordsRequestRead(r){const value={};
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
function Ros1Ultraarm_communicationSetCoordsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 ultraarm_communication/SetCoordsResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetCoordsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_02aa6c82612c5ffd43e890bb173a59245e00cd91dbc38303dcbdc0fb30683431",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Ultraarm_communicationSetCoordsResponseSize(Ros1Ultraarm_communicationSetCoordsResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetCoordsResponseCapture(value),n=Ros1Ultraarm_communicationSetCoordsResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetCoordsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetCoordsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetCoordsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationSetCoordsResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationSetCoordsResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationSetCoordsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/SetFreshModeRequest codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetFreshModeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_142c43e6dcd72843cdd002e5b6815524b2b6a5a02372e5b966dc630947a90649",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Status":0};},
validateValue(value){Ros1Ultraarm_communicationSetFreshModeRequestSize(Ros1Ultraarm_communicationSetFreshModeRequestCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetFreshModeRequestCapture(value),n=Ros1Ultraarm_communicationSetFreshModeRequestSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetFreshModeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetFreshModeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetFreshModeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["Status"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationSetFreshModeRequestWrite(value,w){
w.v.setUint8(w.at,value["Status"],true);w.at+=1;
}
function Ros1Ultraarm_communicationSetFreshModeRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["Status"]=decoded;
}
return value;}
function Ros1Ultraarm_communicationSetFreshModeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Status"];
out["Status"]=field;
}
return out;}
/** Complete ROS1 ultraarm_communication/SetFreshModeResponse codec; maxima are not actual lengths. */
export const Ros1Ultraarm_communicationSetFreshModeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_764d9c9dfff1f18a5501c5f040a6bc66ae6f93bfb7b932a33c2ece2a70d38104",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"Flag":false};},
validateValue(value){Ros1Ultraarm_communicationSetFreshModeResponseSize(Ros1Ultraarm_communicationSetFreshModeResponseCapture(value));return true;},
encode(value){const owned=Ros1Ultraarm_communicationSetFreshModeResponseCapture(value),n=Ros1Ultraarm_communicationSetFreshModeResponseSize(owned),w=new Writer(n);Ros1Ultraarm_communicationSetFreshModeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Ultraarm_communicationSetFreshModeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Ultraarm_communicationSetFreshModeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["Flag"]!=="boolean")bad();n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Ultraarm_communicationSetFreshModeResponseWrite(value,w){
w.v.setUint8(w.at,value["Flag"]?1:0,true);w.at+=1;
}
function Ros1Ultraarm_communicationSetFreshModeResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["Flag"]=decoded!==0;
}
return value;}
function Ros1Ultraarm_communicationSetFreshModeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["Flag"];
out["Flag"]=field;
}
return out;}
