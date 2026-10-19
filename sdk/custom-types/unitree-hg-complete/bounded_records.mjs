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
/** Complete ROS2 unitree_hg/msg/BmsCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgBmsCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_a267c92d22aa04a15f7d68c87b7638b15a368ac973f49a42021dc91f991b13de",maxWireBytes:41,minWireBytes:41,cartesianMaxWireBytes:41n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cmd":0,"reserve":Array.from({length:40},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgBmsCmdSize(Ros2Unitree_hgMsgBmsCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgBmsCmdCapture(value),n=Ros2Unitree_hgMsgBmsCmdSize(owned),w=new Writer(n);Ros2Unitree_hgMsgBmsCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,41,90);if(owned.length<41)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgBmsCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgBmsCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["cmd"],1,false);n=addSize(n,1);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==40)bad();
for(const element of value["reserve"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>41)bad(90);return n;}
function Ros2Unitree_hgMsgBmsCmdWrite(value,w){
w.v.setUint8(w.at,value["cmd"],true);w.at+=1;
for(const element of value["reserve"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_hgMsgBmsCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["cmd"]=decoded;
}
{
{const count=40;if(count>40)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgBmsCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cmd"];
out["cmd"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_hg/msg/BmsState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgBmsStateCodec=Object.freeze({codecIdentity:"UWVMROS1_b7ac163c4ab4786d37a9ec027767258adcfc06637ed8b4ef67e30c517397761d",maxWireBytes:161,minWireBytes:161,cartesianMaxWireBytes:161n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version_high":0,"version_low":0,"fn":0,"cell_vol":Array.from({length:40},()=>(0)),"bmsvoltage":Array.from({length:3},()=>(0)),"current":0,"soc":0,"soh":0,"temperature":Array.from({length:12},()=>(0)),"cycle":0,"manufacturer_date":0,"bmsstate":Array.from({length:5},()=>(0)),"reserve":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgBmsStateSize(Ros2Unitree_hgMsgBmsStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgBmsStateCapture(value),n=Ros2Unitree_hgMsgBmsStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgBmsStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,161,90);if(owned.length<161)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgBmsStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgBmsStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["version_high"],1,false);n=addSize(n,1);
integer(value["version_low"],1,false);n=addSize(n,1);
integer(value["fn"],1,false);n=addSize(n,1);
if(!Array.isArray(value["cell_vol"])||value["cell_vol"].length!==40)bad();
for(const element of value["cell_vol"]){
integer(element,2,false);n=addSize(n,2);
}
if(!Array.isArray(value["bmsvoltage"])||value["bmsvoltage"].length!==3)bad();
for(const element of value["bmsvoltage"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["current"],4,true);n=addSize(n,4);
integer(value["soc"],1,false);n=addSize(n,1);
integer(value["soh"],1,false);n=addSize(n,1);
if(!Array.isArray(value["temperature"])||value["temperature"].length!==12)bad();
for(const element of value["temperature"]){
integer(element,2,true);n=addSize(n,2);
}
integer(value["cycle"],2,false);n=addSize(n,2);
integer(value["manufacturer_date"],2,false);n=addSize(n,2);
if(!Array.isArray(value["bmsstate"])||value["bmsstate"].length!==5)bad();
for(const element of value["bmsstate"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["reserve"])||value["reserve"].length!==3)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>161)bad(90);return n;}
function Ros2Unitree_hgMsgBmsStateWrite(value,w){
w.v.setUint8(w.at,value["version_high"],true);w.at+=1;
w.v.setUint8(w.at,value["version_low"],true);w.at+=1;
w.v.setUint8(w.at,value["fn"],true);w.at+=1;
for(const element of value["cell_vol"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
for(const element of value["bmsvoltage"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setInt32(w.at,value["current"],true);w.at+=4;
w.v.setUint8(w.at,value["soc"],true);w.at+=1;
w.v.setUint8(w.at,value["soh"],true);w.at+=1;
for(const element of value["temperature"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
w.v.setUint16(w.at,value["cycle"],true);w.at+=2;
w.v.setUint16(w.at,value["manufacturer_date"],true);w.at+=2;
for(const element of value["bmsstate"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_hgMsgBmsStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["version_high"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["version_low"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["fn"]=decoded;
}
{
{const count=40;if(count>40)bad();value["cell_vol"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["cell_vol"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["bmsvoltage"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["bmsvoltage"].push(decoded);
}}
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["current"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["soc"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["soh"]=decoded;
}
{
{const count=12;if(count>12)bad();value["temperature"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["temperature"].push(decoded);
}}
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["cycle"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["manufacturer_date"]=decoded;
}
{
{const count=5;if(count>5)bad();value["bmsstate"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["bmsstate"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgBmsStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version_high"];
out["version_high"]=field;
}
{const field=value["version_low"];
out["version_low"]=field;
}
{const field=value["fn"];
out["fn"]=field;
}
{const field=value["cell_vol"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cell_vol"]=values;
}
{const field=value["bmsvoltage"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["bmsvoltage"]=values;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["soc"];
out["soc"]=field;
}
{const field=value["soh"];
out["soh"]=field;
}
{const field=value["temperature"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["temperature"]=values;
}
{const field=value["cycle"];
out["cycle"]=field;
}
{const field=value["manufacturer_date"];
out["manufacturer_date"]=field;
}
{const field=value["bmsstate"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==5)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["bmsstate"]=values;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_hg/msg/MotorCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgMotorCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_f3e472df5d061a1515ac185f87b01f702ef2f2bbeff39e42c6c7088d1da26cbb",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"tau":0,"kp":0,"kd":0,"reserve":0};},
validateValue(value){Ros2Unitree_hgMsgMotorCmdSize(Ros2Unitree_hgMsgMotorCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgMotorCmdCapture(value),n=Ros2Unitree_hgMsgMotorCmdSize(owned),w=new Writer(n);Ros2Unitree_hgMsgMotorCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgMotorCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgMotorCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["tau"]!=="number"||!Number.isFinite(value["tau"])||!Number.isFinite(Math.fround(value["tau"])))bad();n=addSize(n,4);
if(typeof value["kp"]!=="number"||!Number.isFinite(value["kp"])||!Number.isFinite(Math.fround(value["kp"])))bad();n=addSize(n,4);
if(typeof value["kd"]!=="number"||!Number.isFinite(value["kd"])||!Number.isFinite(Math.fround(value["kd"])))bad();n=addSize(n,4);
integer(value["reserve"],4,false);n=addSize(n,4);
if(n>25)bad(90);return n;}
function Ros2Unitree_hgMsgMotorCmdWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau"],true);w.at+=4;
w.v.setFloat32(w.at,value["kp"],true);w.at+=4;
w.v.setFloat32(w.at,value["kd"],true);w.at+=4;
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
}
function Ros2Unitree_hgMsgMotorCmdRead(r){const value={};
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
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
return value;}
function Ros2Unitree_hgMsgMotorCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["reserve"];
out["reserve"]=field;
}
return out;}
/** Complete ROS2 unitree_hg/msg/HandCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgHandCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_2ebc0e32e831a48e21f0b15f39cb477d999bf3a415e7863713ce37416020a835",maxWireBytes:895,minWireBytes:20,cartesianMaxWireBytes:895n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"motor_cmd":[],"reserve":Array.from({length:4},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgHandCmdSize(Ros2Unitree_hgMsgHandCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgHandCmdCapture(value),n=Ros2Unitree_hgMsgHandCmdSize(owned),w=new Writer(n);Ros2Unitree_hgMsgHandCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,895,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgHandCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgHandCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["motor_cmd"])||value["motor_cmd"].length>35)bad();
n=addSize(n,4);
for(const element of value["motor_cmd"]){
n=addSize(n,Ros2Unitree_hgMsgMotorCmdSize(element));
}
if(!Array.isArray(value["reserve"])||value["reserve"].length!==4)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>895)bad(90);return n;}
function Ros2Unitree_hgMsgHandCmdWrite(value,w){
w.v.setUint32(w.at,value["motor_cmd"].length,true);w.at+=4;
for(const element of value["motor_cmd"]){
Ros2Unitree_hgMsgMotorCmdWrite(element,w);
}
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_hgMsgHandCmdRead(r){const value={};
{
{const count=r.u32();if(count>35)bad();value["motor_cmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorCmdRead(r);
value["motor_cmd"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgHandCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["motor_cmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>35)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_hgMsgMotorCmdCapture(element);
}out["motor_cmd"]=values;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_hg/msg/MotorState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_65497908fe0136a5c90f11b4a2cda2110507bcc8805f9347ac82be3b32fe5934",maxWireBytes:53,minWireBytes:53,cartesianMaxWireBytes:53n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"ddq":0,"tau_est":0,"temperature":Array.from({length:2},()=>(0)),"vol":0,"sensor":Array.from({length:2},()=>(0)),"motorstate":0,"reserve":Array.from({length:4},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgMotorStateSize(Ros2Unitree_hgMsgMotorStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgMotorStateCapture(value),n=Ros2Unitree_hgMsgMotorStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,53,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["ddq"]!=="number"||!Number.isFinite(value["ddq"])||!Number.isFinite(Math.fround(value["ddq"])))bad();n=addSize(n,4);
if(typeof value["tau_est"]!=="number"||!Number.isFinite(value["tau_est"])||!Number.isFinite(Math.fround(value["tau_est"])))bad();n=addSize(n,4);
if(!Array.isArray(value["temperature"])||value["temperature"].length!==2)bad();
for(const element of value["temperature"]){
integer(element,2,true);n=addSize(n,2);
}
if(typeof value["vol"]!=="number"||!Number.isFinite(value["vol"])||!Number.isFinite(Math.fround(value["vol"])))bad();n=addSize(n,4);
if(!Array.isArray(value["sensor"])||value["sensor"].length!==2)bad();
for(const element of value["sensor"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["motorstate"],4,false);n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==4)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>53)bad(90);return n;}
function Ros2Unitree_hgMsgMotorStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau_est"],true);w.at+=4;
for(const element of value["temperature"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
w.v.setFloat32(w.at,value["vol"],true);w.at+=4;
for(const element of value["sensor"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["motorstate"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_hgMsgMotorStateRead(r){const value={};
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
{const count=2;if(count>2)bad();value["temperature"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["temperature"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["vol"]=decoded;
}
{
{const count=2;if(count>2)bad();value["sensor"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["sensor"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["motorstate"]=decoded;
}
{
{const count=4;if(count>4)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["temperature"]=values;
}
{const field=value["vol"];
out["vol"]=field;
}
{const field=value["sensor"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["sensor"]=values;
}
{const field=value["motorstate"];
out["motorstate"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_hg/msg/PressSensorState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgPressSensorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_4abc296d4acd21132c0c1d185e3fafd9f0db8836a5c16ed040ebcc6127380fe5",maxWireBytes:104,minWireBytes:104,cartesianMaxWireBytes:104n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"pressure":Array.from({length:12},()=>(0)),"temperature":Array.from({length:12},()=>(0)),"lost":0,"reserve":0};},
validateValue(value){Ros2Unitree_hgMsgPressSensorStateSize(Ros2Unitree_hgMsgPressSensorStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgPressSensorStateCapture(value),n=Ros2Unitree_hgMsgPressSensorStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgPressSensorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,104,90);if(owned.length<104)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgPressSensorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgPressSensorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["pressure"])||value["pressure"].length!==12)bad();
for(const element of value["pressure"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["temperature"])||value["temperature"].length!==12)bad();
for(const element of value["temperature"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
integer(value["lost"],4,false);n=addSize(n,4);
integer(value["reserve"],4,false);n=addSize(n,4);
if(n>104)bad(90);return n;}
function Ros2Unitree_hgMsgPressSensorStateWrite(value,w){
for(const element of value["pressure"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["temperature"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["lost"],true);w.at+=4;
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
}
function Ros2Unitree_hgMsgPressSensorStateRead(r){const value={};
{
{const count=12;if(count>12)bad();value["pressure"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pressure"].push(decoded);
}}
}
{
{const count=12;if(count>12)bad();value["temperature"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["temperature"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["lost"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
return value;}
function Ros2Unitree_hgMsgPressSensorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["pressure"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["pressure"]=values;
}
{const field=value["temperature"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["temperature"]=values;
}
{const field=value["lost"];
out["lost"]=field;
}
{const field=value["reserve"];
out["reserve"]=field;
}
return out;}
/** Complete ROS2 unitree_hg/msg/IMUState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgIMUStateCodec=Object.freeze({codecIdentity:"UWVMROS1_d83832e8c27273785113c8388f7f22ea1251d6198817255f13a7e542afb26420",maxWireBytes:54,minWireBytes:54,cartesianMaxWireBytes:54n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"quaternion":Array.from({length:4},()=>(0)),"gyroscope":Array.from({length:3},()=>(0)),"accelerometer":Array.from({length:3},()=>(0)),"rpy":Array.from({length:3},()=>(0)),"temperature":0};},
validateValue(value){Ros2Unitree_hgMsgIMUStateSize(Ros2Unitree_hgMsgIMUStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgIMUStateCapture(value),n=Ros2Unitree_hgMsgIMUStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgIMUStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,54,90);if(owned.length<54)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgIMUStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgIMUStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["quaternion"])||value["quaternion"].length!==4)bad();
for(const element of value["quaternion"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["gyroscope"])||value["gyroscope"].length!==3)bad();
for(const element of value["gyroscope"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["accelerometer"])||value["accelerometer"].length!==3)bad();
for(const element of value["accelerometer"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["rpy"])||value["rpy"].length!==3)bad();
for(const element of value["rpy"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
integer(value["temperature"],2,true);n=addSize(n,2);
if(n>54)bad(90);return n;}
function Ros2Unitree_hgMsgIMUStateWrite(value,w){
for(const element of value["quaternion"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["gyroscope"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["accelerometer"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["rpy"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setInt16(w.at,value["temperature"],true);w.at+=2;
}
function Ros2Unitree_hgMsgIMUStateRead(r){const value={};
{
{const count=4;if(count>4)bad();value["quaternion"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["quaternion"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["gyroscope"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["gyroscope"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["accelerometer"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["accelerometer"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["rpy"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rpy"].push(decoded);
}}
}
{
const decoded=r.v.getInt16(r.take(2),true);
value["temperature"]=decoded;
}
return value;}
function Ros2Unitree_hgMsgIMUStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["quaternion"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["quaternion"]=values;
}
{const field=value["gyroscope"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["gyroscope"]=values;
}
{const field=value["accelerometer"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["accelerometer"]=values;
}
{const field=value["rpy"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["rpy"]=values;
}
{const field=value["temperature"];
out["temperature"]=field;
}
return out;}
/** Complete ROS2 unitree_hg/msg/HandState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgHandStateCodec=Object.freeze({codecIdentity:"UWVMROS1_5721fdfc155f15ca331dbd398f07cb909196b07e93cabdb6dfbb67b10d4abc4a",maxWireBytes:2365,minWireBytes:94,cartesianMaxWireBytes:2365n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"motor_state":[],"press_sensor_state":[],"imu_state":Ros2Unitree_hgMsgIMUStateCodec.defaultValue(),"power_v":0,"power_a":0,"system_v":0,"device_v":0,"error":Array.from({length:2},()=>(0)),"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgHandStateSize(Ros2Unitree_hgMsgHandStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgHandStateCapture(value),n=Ros2Unitree_hgMsgHandStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgHandStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2365,90);if(owned.length<94)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgHandStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgHandStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["motor_state"])||value["motor_state"].length>35)bad();
n=addSize(n,4);
for(const element of value["motor_state"]){
n=addSize(n,Ros2Unitree_hgMsgMotorStateSize(element));
}
if(!Array.isArray(value["press_sensor_state"])||value["press_sensor_state"].length>4)bad();
n=addSize(n,4);
for(const element of value["press_sensor_state"]){
n=addSize(n,Ros2Unitree_hgMsgPressSensorStateSize(element));
}
n=addSize(n,Ros2Unitree_hgMsgIMUStateSize(value["imu_state"]));
if(typeof value["power_v"]!=="number"||!Number.isFinite(value["power_v"])||!Number.isFinite(Math.fround(value["power_v"])))bad();n=addSize(n,4);
if(typeof value["power_a"]!=="number"||!Number.isFinite(value["power_a"])||!Number.isFinite(Math.fround(value["power_a"])))bad();n=addSize(n,4);
if(typeof value["system_v"]!=="number"||!Number.isFinite(value["system_v"])||!Number.isFinite(Math.fround(value["system_v"])))bad();n=addSize(n,4);
if(typeof value["device_v"]!=="number"||!Number.isFinite(value["device_v"])||!Number.isFinite(Math.fround(value["device_v"])))bad();n=addSize(n,4);
if(!Array.isArray(value["error"])||value["error"].length!==2)bad();
for(const element of value["error"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>2365)bad(90);return n;}
function Ros2Unitree_hgMsgHandStateWrite(value,w){
w.v.setUint32(w.at,value["motor_state"].length,true);w.at+=4;
for(const element of value["motor_state"]){
Ros2Unitree_hgMsgMotorStateWrite(element,w);
}
w.v.setUint32(w.at,value["press_sensor_state"].length,true);w.at+=4;
for(const element of value["press_sensor_state"]){
Ros2Unitree_hgMsgPressSensorStateWrite(element,w);
}
Ros2Unitree_hgMsgIMUStateWrite(value["imu_state"],w);
w.v.setFloat32(w.at,value["power_v"],true);w.at+=4;
w.v.setFloat32(w.at,value["power_a"],true);w.at+=4;
w.v.setFloat32(w.at,value["system_v"],true);w.at+=4;
w.v.setFloat32(w.at,value["device_v"],true);w.at+=4;
for(const element of value["error"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_hgMsgHandStateRead(r){const value={};
{
{const count=r.u32();if(count>35)bad();value["motor_state"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorStateRead(r);
value["motor_state"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["press_sensor_state"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgPressSensorStateRead(r);
value["press_sensor_state"].push(decoded);
}}
}
{
const decoded=Ros2Unitree_hgMsgIMUStateRead(r);
value["imu_state"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["power_v"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["power_a"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["system_v"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["device_v"]=decoded;
}
{
{const count=2;if(count>2)bad();value["error"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["error"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgHandStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["motor_state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>35)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_hgMsgMotorStateCapture(element);
}out["motor_state"]=values;
}
{const field=value["press_sensor_state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_hgMsgPressSensorStateCapture(element);
}out["press_sensor_state"]=values;
}
{const field=value["imu_state"];
out["imu_state"]=Ros2Unitree_hgMsgIMUStateCapture(field);
}
{const field=value["power_v"];
out["power_v"]=field;
}
{const field=value["power_a"];
out["power_a"]=field;
}
{const field=value["system_v"];
out["system_v"]=field;
}
{const field=value["device_v"];
out["device_v"]=field;
}
{const field=value["error"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["error"]=values;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_hg/msg/LowCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgLowCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_8303ac6e2e3273251662d31daf966a11ced5b5ac10b93aa6253eebc56f4ec461",maxWireBytes:897,minWireBytes:897,cartesianMaxWireBytes:897n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode_pr":0,"mode_machine":0,"motor_cmd":Array.from({length:35},()=>(Ros2Unitree_hgMsgMotorCmdCodec.defaultValue())),"reserve":Array.from({length:4},()=>(0)),"crc":0};},
validateValue(value){Ros2Unitree_hgMsgLowCmdSize(Ros2Unitree_hgMsgLowCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgLowCmdCapture(value),n=Ros2Unitree_hgMsgLowCmdSize(owned),w=new Writer(n);Ros2Unitree_hgMsgLowCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,897,90);if(owned.length<897)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgLowCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgLowCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode_pr"],1,false);n=addSize(n,1);
integer(value["mode_machine"],1,false);n=addSize(n,1);
if(!Array.isArray(value["motor_cmd"])||value["motor_cmd"].length!==35)bad();
for(const element of value["motor_cmd"]){
n=addSize(n,Ros2Unitree_hgMsgMotorCmdSize(element));
}
if(!Array.isArray(value["reserve"])||value["reserve"].length!==4)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["crc"],4,false);n=addSize(n,4);
if(n>897)bad(90);return n;}
function Ros2Unitree_hgMsgLowCmdWrite(value,w){
w.v.setUint8(w.at,value["mode_pr"],true);w.at+=1;
w.v.setUint8(w.at,value["mode_machine"],true);w.at+=1;
for(const element of value["motor_cmd"]){
Ros2Unitree_hgMsgMotorCmdWrite(element,w);
}
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros2Unitree_hgMsgLowCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode_pr"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode_machine"]=decoded;
}
{
{const count=35;if(count>35)bad();value["motor_cmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorCmdRead(r);
value["motor_cmd"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
return value;}
function Ros2Unitree_hgMsgLowCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode_pr"];
out["mode_pr"]=field;
}
{const field=value["mode_machine"];
out["mode_machine"]=field;
}
{const field=value["motor_cmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==35)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_hgMsgMotorCmdCapture(element);
}out["motor_cmd"]=values;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS2 unitree_hg/msg/LowState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgLowStateCodec=Object.freeze({codecIdentity:"UWVMROS1_7eed2692672f536a476643f1d3a07affc8e5b3272663e3d282c199fff1c2fa1d",maxWireBytes:1983,minWireBytes:1983,cartesianMaxWireBytes:1983n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version":Array.from({length:2},()=>(0)),"mode_pr":0,"mode_machine":0,"tick":0,"imu_state":Ros2Unitree_hgMsgIMUStateCodec.defaultValue(),"motor_state":Array.from({length:35},()=>(Ros2Unitree_hgMsgMotorStateCodec.defaultValue())),"wireless_remote":Array.from({length:40},()=>(0)),"reserve":Array.from({length:4},()=>(0)),"crc":0};},
validateValue(value){Ros2Unitree_hgMsgLowStateSize(Ros2Unitree_hgMsgLowStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgLowStateCapture(value),n=Ros2Unitree_hgMsgLowStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgLowStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1983,90);if(owned.length<1983)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgLowStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgLowStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["mode_pr"],1,false);n=addSize(n,1);
integer(value["mode_machine"],1,false);n=addSize(n,1);
integer(value["tick"],4,false);n=addSize(n,4);
n=addSize(n,Ros2Unitree_hgMsgIMUStateSize(value["imu_state"]));
if(!Array.isArray(value["motor_state"])||value["motor_state"].length!==35)bad();
for(const element of value["motor_state"]){
n=addSize(n,Ros2Unitree_hgMsgMotorStateSize(element));
}
if(!Array.isArray(value["wireless_remote"])||value["wireless_remote"].length!==40)bad();
for(const element of value["wireless_remote"]){
integer(element,1,false);n=addSize(n,1);
}
if(!Array.isArray(value["reserve"])||value["reserve"].length!==4)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["crc"],4,false);n=addSize(n,4);
if(n>1983)bad(90);return n;}
function Ros2Unitree_hgMsgLowStateWrite(value,w){
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint8(w.at,value["mode_pr"],true);w.at+=1;
w.v.setUint8(w.at,value["mode_machine"],true);w.at+=1;
w.v.setUint32(w.at,value["tick"],true);w.at+=4;
Ros2Unitree_hgMsgIMUStateWrite(value["imu_state"],w);
for(const element of value["motor_state"]){
Ros2Unitree_hgMsgMotorStateWrite(element,w);
}
for(const element of value["wireless_remote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros2Unitree_hgMsgLowStateRead(r){const value={};
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode_pr"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode_machine"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["tick"]=decoded;
}
{
const decoded=Ros2Unitree_hgMsgIMUStateRead(r);
value["imu_state"]=decoded;
}
{
{const count=35;if(count>35)bad();value["motor_state"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorStateRead(r);
value["motor_state"].push(decoded);
}}
}
{
{const count=40;if(count>40)bad();value["wireless_remote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wireless_remote"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
return value;}
function Ros2Unitree_hgMsgLowStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["mode_pr"];
out["mode_pr"]=field;
}
{const field=value["mode_machine"];
out["mode_machine"]=field;
}
{const field=value["tick"];
out["tick"]=field;
}
{const field=value["imu_state"];
out["imu_state"]=Ros2Unitree_hgMsgIMUStateCapture(field);
}
{const field=value["motor_state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==35)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_hgMsgMotorStateCapture(element);
}out["motor_state"]=values;
}
{const field=value["wireless_remote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wireless_remote"]=values;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS2 unitree_hg/msg/MainBoardState codec; maxima are not actual lengths. */
export const Ros2Unitree_hgMsgMainBoardStateCodec=Object.freeze({codecIdentity:"UWVMROS1_2a27a979aadc7bef33a757e169680faa632936c2634ee787f0c9f25cb08535a6",maxWireBytes:72,minWireBytes:72,cartesianMaxWireBytes:72n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"fan_state":Array.from({length:6},()=>(0)),"temperature":Array.from({length:6},()=>(0)),"value":Array.from({length:6},()=>(0)),"state":Array.from({length:6},()=>(0))};},
validateValue(value){Ros2Unitree_hgMsgMainBoardStateSize(Ros2Unitree_hgMsgMainBoardStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_hgMsgMainBoardStateCapture(value),n=Ros2Unitree_hgMsgMainBoardStateSize(owned),w=new Writer(n);Ros2Unitree_hgMsgMainBoardStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,72,90);if(owned.length<72)bad();const r=new Reader(owned),value=Ros2Unitree_hgMsgMainBoardStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_hgMsgMainBoardStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["fan_state"])||value["fan_state"].length!==6)bad();
for(const element of value["fan_state"]){
integer(element,2,false);n=addSize(n,2);
}
if(!Array.isArray(value["temperature"])||value["temperature"].length!==6)bad();
for(const element of value["temperature"]){
integer(element,2,true);n=addSize(n,2);
}
if(!Array.isArray(value["value"])||value["value"].length!==6)bad();
for(const element of value["value"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["state"])||value["state"].length!==6)bad();
for(const element of value["state"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>72)bad(90);return n;}
function Ros2Unitree_hgMsgMainBoardStateWrite(value,w){
for(const element of value["fan_state"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
for(const element of value["temperature"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
for(const element of value["value"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["state"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_hgMsgMainBoardStateRead(r){const value={};
{
{const count=6;if(count>6)bad();value["fan_state"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["fan_state"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["temperature"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["temperature"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["value"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"].push(decoded);
}}
}
{
{const count=6;if(count>6)bad();value["state"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["state"].push(decoded);
}}
}
return value;}
function Ros2Unitree_hgMsgMainBoardStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["fan_state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["fan_state"]=values;
}
{const field=value["temperature"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["temperature"]=values;
}
{const field=value["value"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["value"]=values;
}
{const field=value["state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==6)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["state"]=values;
}
return out;}
