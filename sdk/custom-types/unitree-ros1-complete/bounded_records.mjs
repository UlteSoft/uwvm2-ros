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
/** Complete ROS1 unitree_legged_msgs/BmsCmd codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsBmsCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_75732753b156fff8db3ba7e39541444bed2b71e139389a9b5bfbc49a4fa14928",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"off":0,"reserve":Array.from({length:3},()=>(0))};},
validateValue(value){Ros1Unitree_legged_msgsBmsCmdSize(Ros1Unitree_legged_msgsBmsCmdCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsBmsCmdCapture(value),n=Ros1Unitree_legged_msgsBmsCmdSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsBmsCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsBmsCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsBmsCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["off"],1,false);n=addSize(n,1);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==3)bad();
for(const element of value["reserve"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>4)bad(90);return n;}
function Ros1Unitree_legged_msgsBmsCmdWrite(value,w){
w.v.setUint8(w.at,value["off"],true);w.at+=1;
for(const element of value["reserve"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros1Unitree_legged_msgsBmsCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["off"]=decoded;
}
{
{const count=3;if(count>3)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros1Unitree_legged_msgsBmsCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["off"];
out["off"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/BmsState codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsBmsStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ef0f93501f54d0ff760039d86bd1ba1739cb0c888b3a873611679484ccec094a",maxWireBytes:34,minWireBytes:34,cartesianMaxWireBytes:34n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version_h":0,"version_l":0,"bms_status":0,"SOC":0,"current":0,"cycle":0,"BQ_NTC":Array.from({length:2},()=>(0)),"MCU_NTC":Array.from({length:2},()=>(0)),"cell_vol":Array.from({length:10},()=>(0))};},
validateValue(value){Ros1Unitree_legged_msgsBmsStateSize(Ros1Unitree_legged_msgsBmsStateCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsBmsStateCapture(value),n=Ros1Unitree_legged_msgsBmsStateSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsBmsStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,34,90);if(owned.length<34)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsBmsStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsBmsStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["version_h"],1,false);n=addSize(n,1);
integer(value["version_l"],1,false);n=addSize(n,1);
integer(value["bms_status"],1,false);n=addSize(n,1);
integer(value["SOC"],1,false);n=addSize(n,1);
integer(value["current"],4,true);n=addSize(n,4);
integer(value["cycle"],2,false);n=addSize(n,2);
if(!Array.isArray(value["BQ_NTC"])||value["BQ_NTC"].length!==2)bad();
for(const element of value["BQ_NTC"]){
integer(element,1,true);n=addSize(n,1);
}
if(!Array.isArray(value["MCU_NTC"])||value["MCU_NTC"].length!==2)bad();
for(const element of value["MCU_NTC"]){
integer(element,1,true);n=addSize(n,1);
}
if(!Array.isArray(value["cell_vol"])||value["cell_vol"].length!==10)bad();
for(const element of value["cell_vol"]){
integer(element,2,false);n=addSize(n,2);
}
if(n>34)bad(90);return n;}
function Ros1Unitree_legged_msgsBmsStateWrite(value,w){
w.v.setUint8(w.at,value["version_h"],true);w.at+=1;
w.v.setUint8(w.at,value["version_l"],true);w.at+=1;
w.v.setUint8(w.at,value["bms_status"],true);w.at+=1;
w.v.setUint8(w.at,value["SOC"],true);w.at+=1;
w.v.setInt32(w.at,value["current"],true);w.at+=4;
w.v.setUint16(w.at,value["cycle"],true);w.at+=2;
for(const element of value["BQ_NTC"]){
w.v.setInt8(w.at,element,true);w.at+=1;
}
for(const element of value["MCU_NTC"]){
w.v.setInt8(w.at,element,true);w.at+=1;
}
for(const element of value["cell_vol"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
}
function Ros1Unitree_legged_msgsBmsStateRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["version_h"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["version_l"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["bms_status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["SOC"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["current"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["cycle"]=decoded;
}
{
{const count=2;if(count>2)bad();value["BQ_NTC"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt8(r.take(1),true);
value["BQ_NTC"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["MCU_NTC"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt8(r.take(1),true);
value["MCU_NTC"].push(decoded);
}}
}
{
{const count=10;if(count>10)bad();value["cell_vol"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["cell_vol"].push(decoded);
}}
}
return value;}
function Ros1Unitree_legged_msgsBmsStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version_h"];
out["version_h"]=field;
}
{const field=value["version_l"];
out["version_l"]=field;
}
{const field=value["bms_status"];
out["bms_status"]=field;
}
{const field=value["SOC"];
out["SOC"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["cycle"];
out["cycle"]=field;
}
{const field=value["BQ_NTC"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["BQ_NTC"]=values;
}
{const field=value["MCU_NTC"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["MCU_NTC"]=values;
}
{const field=value["cell_vol"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==10)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cell_vol"]=values;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/Cartesian codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsCartesianCodec=Object.freeze({codecIdentity:"UWVMROS1_43c7c62ba2a689751a0a15e326c5c9690a9b1f166b6726a0f803b98d2b3fa722",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros1Unitree_legged_msgsCartesianSize(Ros1Unitree_legged_msgsCartesianCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsCartesianCapture(value),n=Ros1Unitree_legged_msgsCartesianSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsCartesianWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsCartesianRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsCartesianSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros1Unitree_legged_msgsCartesianWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
}
function Ros1Unitree_legged_msgsCartesianRead(r){const value={};
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
return value;}
function Ros1Unitree_legged_msgsCartesianCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 unitree_legged_msgs/LED codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsLEDCodec=Object.freeze({codecIdentity:"UWVMROS1_b4efac59eaf4d3f09c069e4833dd82b5422980f6a9f27db84950243a47164ea5",maxWireBytes:3,minWireBytes:3,cartesianMaxWireBytes:3n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"r":0,"g":0,"b":0};},
validateValue(value){Ros1Unitree_legged_msgsLEDSize(Ros1Unitree_legged_msgsLEDCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsLEDCapture(value),n=Ros1Unitree_legged_msgsLEDSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsLEDWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3,90);if(owned.length<3)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsLEDRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsLEDSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["r"],1,false);n=addSize(n,1);
integer(value["g"],1,false);n=addSize(n,1);
integer(value["b"],1,false);n=addSize(n,1);
if(n>3)bad(90);return n;}
function Ros1Unitree_legged_msgsLEDWrite(value,w){
w.v.setUint8(w.at,value["r"],true);w.at+=1;
w.v.setUint8(w.at,value["g"],true);w.at+=1;
w.v.setUint8(w.at,value["b"],true);w.at+=1;
}
function Ros1Unitree_legged_msgsLEDRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["r"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["g"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["b"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsLEDCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["r"];
out["r"]=field;
}
{const field=value["g"];
out["g"]=field;
}
{const field=value["b"];
out["b"]=field;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/HighCmd codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsHighCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_ec6ec8937b2629a4441adc4a21eed83fca940b18d46b970d96e05c957ac90b38",maxWireBytes:129,minWireBytes:129,cartesianMaxWireBytes:129n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"levelFlag":0,"frameReserve":0,"SN":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandWidth":0,"mode":0,"gaitType":0,"speedLevel":0,"footRaiseHeight":0,"bodyHeight":0,"position":Array.from({length:2},()=>(0)),"euler":Array.from({length:3},()=>(0)),"velocity":Array.from({length:2},()=>(0)),"yawSpeed":0,"bms":Ros1Unitree_legged_msgsBmsCmdCodec.defaultValue(),"led":Array.from({length:4},()=>(Ros1Unitree_legged_msgsLEDCodec.defaultValue())),"wirelessRemote":Array.from({length:40},()=>(0)),"reserve":0,"crc":0};},
validateValue(value){Ros1Unitree_legged_msgsHighCmdSize(Ros1Unitree_legged_msgsHighCmdCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsHighCmdCapture(value),n=Ros1Unitree_legged_msgsHighCmdSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsHighCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,129,90);if(owned.length<129)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsHighCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsHighCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["levelFlag"],1,false);n=addSize(n,1);
integer(value["frameReserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["SN"])||value["SN"].length!==2)bad();
for(const element of value["SN"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandWidth"],2,false);n=addSize(n,2);
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["gaitType"],1,false);n=addSize(n,1);
integer(value["speedLevel"],1,false);n=addSize(n,1);
if(typeof value["footRaiseHeight"]!=="number"||!Number.isFinite(value["footRaiseHeight"])||!Number.isFinite(Math.fround(value["footRaiseHeight"])))bad();n=addSize(n,4);
if(typeof value["bodyHeight"]!=="number"||!Number.isFinite(value["bodyHeight"])||!Number.isFinite(Math.fround(value["bodyHeight"])))bad();n=addSize(n,4);
if(!Array.isArray(value["position"])||value["position"].length!==2)bad();
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["euler"])||value["euler"].length!==3)bad();
for(const element of value["euler"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["velocity"])||value["velocity"].length!==2)bad();
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["yawSpeed"]!=="number"||!Number.isFinite(value["yawSpeed"])||!Number.isFinite(Math.fround(value["yawSpeed"])))bad();n=addSize(n,4);
n=addSize(n,Ros1Unitree_legged_msgsBmsCmdSize(value["bms"]));
if(!Array.isArray(value["led"])||value["led"].length!==4)bad();
for(const element of value["led"]){
n=addSize(n,Ros1Unitree_legged_msgsLEDSize(element));
}
if(!Array.isArray(value["wirelessRemote"])||value["wirelessRemote"].length!==40)bad();
for(const element of value["wirelessRemote"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(n>129)bad(90);return n;}
function Ros1Unitree_legged_msgsHighCmdWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["levelFlag"],true);w.at+=1;
w.v.setUint8(w.at,value["frameReserve"],true);w.at+=1;
for(const element of value["SN"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandWidth"],true);w.at+=2;
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["gaitType"],true);w.at+=1;
w.v.setUint8(w.at,value["speedLevel"],true);w.at+=1;
w.v.setFloat32(w.at,value["footRaiseHeight"],true);w.at+=4;
w.v.setFloat32(w.at,value["bodyHeight"],true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["euler"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["velocity"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["yawSpeed"],true);w.at+=4;
Ros1Unitree_legged_msgsBmsCmdWrite(value["bms"],w);
for(const element of value["led"]){
Ros1Unitree_legged_msgsLEDWrite(element,w);
}
for(const element of value["wirelessRemote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros1Unitree_legged_msgsHighCmdRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["levelFlag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frameReserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["SN"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["SN"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["bandWidth"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gaitType"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["speedLevel"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["footRaiseHeight"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["bodyHeight"]=decoded;
}
{
{const count=2;if(count>2)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=3;if(count>3)bad();value["euler"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["euler"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["yawSpeed"]=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsBmsCmdRead(r);
value["bms"]=decoded;
}
{
{const count=4;if(count>4)bad();value["led"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsLEDRead(r);
value["led"].push(decoded);
}}
}
{
{const count=40;if(count>40)bad();value["wirelessRemote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wirelessRemote"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsHighCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["levelFlag"];
out["levelFlag"]=field;
}
{const field=value["frameReserve"];
out["frameReserve"]=field;
}
{const field=value["SN"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["SN"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandWidth"];
out["bandWidth"]=field;
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["gaitType"];
out["gaitType"]=field;
}
{const field=value["speedLevel"];
out["speedLevel"]=field;
}
{const field=value["footRaiseHeight"];
out["footRaiseHeight"]=field;
}
{const field=value["bodyHeight"];
out["bodyHeight"]=field;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["euler"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["euler"]=values;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["yawSpeed"];
out["yawSpeed"]=field;
}
{const field=value["bms"];
out["bms"]=Ros1Unitree_legged_msgsBmsCmdCapture(field);
}
{const field=value["led"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsLEDCapture(element);
}out["led"]=values;
}
{const field=value["wirelessRemote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wirelessRemote"]=values;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/IMU codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsIMUCodec=Object.freeze({codecIdentity:"UWVMROS1_29d9ec99e676fe8747665da588b78c8ab77a3197e281ba892ab389d2d81fa276",maxWireBytes:53,minWireBytes:53,cartesianMaxWireBytes:53n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"quaternion":Array.from({length:4},()=>(0)),"gyroscope":Array.from({length:3},()=>(0)),"accelerometer":Array.from({length:3},()=>(0)),"rpy":Array.from({length:3},()=>(0)),"temperature":0};},
validateValue(value){Ros1Unitree_legged_msgsIMUSize(Ros1Unitree_legged_msgsIMUCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsIMUCapture(value),n=Ros1Unitree_legged_msgsIMUSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsIMUWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,53,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsIMURead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsIMUSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
integer(value["temperature"],1,true);n=addSize(n,1);
if(n>53)bad(90);return n;}
function Ros1Unitree_legged_msgsIMUWrite(value,w){
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
w.v.setInt8(w.at,value["temperature"],true);w.at+=1;
}
function Ros1Unitree_legged_msgsIMURead(r){const value={};
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
const decoded=r.v.getInt8(r.take(1),true);
value["temperature"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsIMUCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 unitree_legged_msgs/MotorState codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_be540cb22db5f08b6b9a8082777c76244e80f3148e9cfd63f510e4b2acdc0abf",maxWireBytes:38,minWireBytes:38,cartesianMaxWireBytes:38n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"ddq":0,"tauEst":0,"q_raw":0,"dq_raw":0,"ddq_raw":0,"temperature":0,"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros1Unitree_legged_msgsMotorStateSize(Ros1Unitree_legged_msgsMotorStateCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsMotorStateCapture(value),n=Ros1Unitree_legged_msgsMotorStateSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,38,90);if(owned.length<38)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["ddq"]!=="number"||!Number.isFinite(value["ddq"])||!Number.isFinite(Math.fround(value["ddq"])))bad();n=addSize(n,4);
if(typeof value["tauEst"]!=="number"||!Number.isFinite(value["tauEst"])||!Number.isFinite(Math.fround(value["tauEst"])))bad();n=addSize(n,4);
if(typeof value["q_raw"]!=="number"||!Number.isFinite(value["q_raw"])||!Number.isFinite(Math.fround(value["q_raw"])))bad();n=addSize(n,4);
if(typeof value["dq_raw"]!=="number"||!Number.isFinite(value["dq_raw"])||!Number.isFinite(Math.fround(value["dq_raw"])))bad();n=addSize(n,4);
if(typeof value["ddq_raw"]!=="number"||!Number.isFinite(value["ddq_raw"])||!Number.isFinite(Math.fround(value["ddq_raw"])))bad();n=addSize(n,4);
integer(value["temperature"],1,true);n=addSize(n,1);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>38)bad(90);return n;}
function Ros1Unitree_legged_msgsMotorStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tauEst"],true);w.at+=4;
w.v.setFloat32(w.at,value["q_raw"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq_raw"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq_raw"],true);w.at+=4;
w.v.setInt8(w.at,value["temperature"],true);w.at+=1;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros1Unitree_legged_msgsMotorStateRead(r){const value={};
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
value["tauEst"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q_raw"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["dq_raw"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ddq_raw"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["temperature"]=decoded;
}
{
{const count=2;if(count>2)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros1Unitree_legged_msgsMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["tauEst"];
out["tauEst"]=field;
}
{const field=value["q_raw"];
out["q_raw"]=field;
}
{const field=value["dq_raw"];
out["dq_raw"]=field;
}
{const field=value["ddq_raw"];
out["ddq_raw"]=field;
}
{const field=value["temperature"];
out["temperature"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/HighState codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsHighStateCodec=Object.freeze({codecIdentity:"UWVMROS1_82ee0efaab21b5cb89e99f5c0f5daec583b48dd5feb2dd6a7e09ba9d9110b53d",maxWireBytes:1087,minWireBytes:1087,cartesianMaxWireBytes:1087n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"levelFlag":0,"frameReserve":0,"SN":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandWidth":0,"imu":Ros1Unitree_legged_msgsIMUCodec.defaultValue(),"motorState":Array.from({length:20},()=>(Ros1Unitree_legged_msgsMotorStateCodec.defaultValue())),"bms":Ros1Unitree_legged_msgsBmsStateCodec.defaultValue(),"footForce":Array.from({length:4},()=>(0)),"footForceEst":Array.from({length:4},()=>(0)),"mode":0,"progress":0,"gaitType":0,"footRaiseHeight":0,"position":Array.from({length:3},()=>(0)),"bodyHeight":0,"velocity":Array.from({length:3},()=>(0)),"yawSpeed":0,"rangeObstacle":Array.from({length:4},()=>(0)),"footPosition2Body":Array.from({length:4},()=>(Ros1Unitree_legged_msgsCartesianCodec.defaultValue())),"footSpeed2Body":Array.from({length:4},()=>(Ros1Unitree_legged_msgsCartesianCodec.defaultValue())),"wirelessRemote":Array.from({length:40},()=>(0)),"reserve":0,"crc":0};},
validateValue(value){Ros1Unitree_legged_msgsHighStateSize(Ros1Unitree_legged_msgsHighStateCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsHighStateCapture(value),n=Ros1Unitree_legged_msgsHighStateSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsHighStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1087,90);if(owned.length<1087)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsHighStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsHighStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["levelFlag"],1,false);n=addSize(n,1);
integer(value["frameReserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["SN"])||value["SN"].length!==2)bad();
for(const element of value["SN"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandWidth"],2,false);n=addSize(n,2);
n=addSize(n,Ros1Unitree_legged_msgsIMUSize(value["imu"]));
if(!Array.isArray(value["motorState"])||value["motorState"].length!==20)bad();
for(const element of value["motorState"]){
n=addSize(n,Ros1Unitree_legged_msgsMotorStateSize(element));
}
n=addSize(n,Ros1Unitree_legged_msgsBmsStateSize(value["bms"]));
if(!Array.isArray(value["footForce"])||value["footForce"].length!==4)bad();
for(const element of value["footForce"]){
integer(element,2,true);n=addSize(n,2);
}
if(!Array.isArray(value["footForceEst"])||value["footForceEst"].length!==4)bad();
for(const element of value["footForceEst"]){
integer(element,2,true);n=addSize(n,2);
}
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["progress"]!=="number"||!Number.isFinite(value["progress"])||!Number.isFinite(Math.fround(value["progress"])))bad();n=addSize(n,4);
integer(value["gaitType"],1,false);n=addSize(n,1);
if(typeof value["footRaiseHeight"]!=="number"||!Number.isFinite(value["footRaiseHeight"])||!Number.isFinite(Math.fround(value["footRaiseHeight"])))bad();n=addSize(n,4);
if(!Array.isArray(value["position"])||value["position"].length!==3)bad();
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["bodyHeight"]!=="number"||!Number.isFinite(value["bodyHeight"])||!Number.isFinite(Math.fround(value["bodyHeight"])))bad();n=addSize(n,4);
if(!Array.isArray(value["velocity"])||value["velocity"].length!==3)bad();
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["yawSpeed"]!=="number"||!Number.isFinite(value["yawSpeed"])||!Number.isFinite(Math.fround(value["yawSpeed"])))bad();n=addSize(n,4);
if(!Array.isArray(value["rangeObstacle"])||value["rangeObstacle"].length!==4)bad();
for(const element of value["rangeObstacle"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["footPosition2Body"])||value["footPosition2Body"].length!==4)bad();
for(const element of value["footPosition2Body"]){
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(element));
}
if(!Array.isArray(value["footSpeed2Body"])||value["footSpeed2Body"].length!==4)bad();
for(const element of value["footSpeed2Body"]){
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(element));
}
if(!Array.isArray(value["wirelessRemote"])||value["wirelessRemote"].length!==40)bad();
for(const element of value["wirelessRemote"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(n>1087)bad(90);return n;}
function Ros1Unitree_legged_msgsHighStateWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["levelFlag"],true);w.at+=1;
w.v.setUint8(w.at,value["frameReserve"],true);w.at+=1;
for(const element of value["SN"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandWidth"],true);w.at+=2;
Ros1Unitree_legged_msgsIMUWrite(value["imu"],w);
for(const element of value["motorState"]){
Ros1Unitree_legged_msgsMotorStateWrite(element,w);
}
Ros1Unitree_legged_msgsBmsStateWrite(value["bms"],w);
for(const element of value["footForce"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
for(const element of value["footForceEst"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["progress"],true);w.at+=4;
w.v.setUint8(w.at,value["gaitType"],true);w.at+=1;
w.v.setFloat32(w.at,value["footRaiseHeight"],true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["bodyHeight"],true);w.at+=4;
for(const element of value["velocity"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["yawSpeed"],true);w.at+=4;
for(const element of value["rangeObstacle"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["footPosition2Body"]){
Ros1Unitree_legged_msgsCartesianWrite(element,w);
}
for(const element of value["footSpeed2Body"]){
Ros1Unitree_legged_msgsCartesianWrite(element,w);
}
for(const element of value["wirelessRemote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros1Unitree_legged_msgsHighStateRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["levelFlag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frameReserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["SN"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["SN"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["bandWidth"]=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsIMURead(r);
value["imu"]=decoded;
}
{
{const count=20;if(count>20)bad();value["motorState"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorStateRead(r);
value["motorState"].push(decoded);
}}
}
{
const decoded=Ros1Unitree_legged_msgsBmsStateRead(r);
value["bms"]=decoded;
}
{
{const count=4;if(count>4)bad();value["footForce"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["footForce"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["footForceEst"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["footForceEst"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["progress"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gaitType"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["footRaiseHeight"]=decoded;
}
{
{const count=3;if(count>3)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["bodyHeight"]=decoded;
}
{
{const count=3;if(count>3)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["yawSpeed"]=decoded;
}
{
{const count=4;if(count>4)bad();value["rangeObstacle"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rangeObstacle"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["footPosition2Body"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["footPosition2Body"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["footSpeed2Body"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["footSpeed2Body"].push(decoded);
}}
}
{
{const count=40;if(count>40)bad();value["wirelessRemote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wirelessRemote"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsHighStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["levelFlag"];
out["levelFlag"]=field;
}
{const field=value["frameReserve"];
out["frameReserve"]=field;
}
{const field=value["SN"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["SN"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandWidth"];
out["bandWidth"]=field;
}
{const field=value["imu"];
out["imu"]=Ros1Unitree_legged_msgsIMUCapture(field);
}
{const field=value["motorState"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsMotorStateCapture(element);
}out["motorState"]=values;
}
{const field=value["bms"];
out["bms"]=Ros1Unitree_legged_msgsBmsStateCapture(field);
}
{const field=value["footForce"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["footForce"]=values;
}
{const field=value["footForceEst"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["footForceEst"]=values;
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["progress"];
out["progress"]=field;
}
{const field=value["gaitType"];
out["gaitType"]=field;
}
{const field=value["footRaiseHeight"];
out["footRaiseHeight"]=field;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["bodyHeight"];
out["bodyHeight"]=field;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["yawSpeed"];
out["yawSpeed"]=field;
}
{const field=value["rangeObstacle"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["rangeObstacle"]=values;
}
{const field=value["footPosition2Body"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsCartesianCapture(element);
}out["footPosition2Body"]=values;
}
{const field=value["footSpeed2Body"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsCartesianCapture(element);
}out["footSpeed2Body"]=values;
}
{const field=value["wirelessRemote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wirelessRemote"]=values;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/MotorCmd codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsMotorCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_632b25f209e72857182d92752aa72e3ab47a45e895cc352ee6dc05f1c1168083",maxWireBytes:33,minWireBytes:33,cartesianMaxWireBytes:33n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"tau":0,"Kp":0,"Kd":0,"reserve":Array.from({length:3},()=>(0))};},
validateValue(value){Ros1Unitree_legged_msgsMotorCmdSize(Ros1Unitree_legged_msgsMotorCmdCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsMotorCmdCapture(value),n=Ros1Unitree_legged_msgsMotorCmdSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsMotorCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,33,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsMotorCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsMotorCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["tau"]!=="number"||!Number.isFinite(value["tau"])||!Number.isFinite(Math.fround(value["tau"])))bad();n=addSize(n,4);
if(typeof value["Kp"]!=="number"||!Number.isFinite(value["Kp"])||!Number.isFinite(Math.fround(value["Kp"])))bad();n=addSize(n,4);
if(typeof value["Kd"]!=="number"||!Number.isFinite(value["Kd"])||!Number.isFinite(Math.fround(value["Kd"])))bad();n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==3)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>33)bad(90);return n;}
function Ros1Unitree_legged_msgsMotorCmdWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau"],true);w.at+=4;
w.v.setFloat32(w.at,value["Kp"],true);w.at+=4;
w.v.setFloat32(w.at,value["Kd"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros1Unitree_legged_msgsMotorCmdRead(r){const value={};
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
value["Kp"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["Kd"]=decoded;
}
{
{const count=3;if(count>3)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros1Unitree_legged_msgsMotorCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["Kp"];
out["Kp"]=field;
}
{const field=value["Kd"];
out["Kd"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/LowCmd codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsLowCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_11d3ac7c497f01fbf6624c39ec71fa613c462cd62acb20c1f263cdde672e9181",maxWireBytes:734,minWireBytes:734,cartesianMaxWireBytes:734n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"levelFlag":0,"frameReserve":0,"SN":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandWidth":0,"motorCmd":Array.from({length:20},()=>(Ros1Unitree_legged_msgsMotorCmdCodec.defaultValue())),"bms":Ros1Unitree_legged_msgsBmsCmdCodec.defaultValue(),"wirelessRemote":Array.from({length:40},()=>(0)),"reserve":0,"crc":0};},
validateValue(value){Ros1Unitree_legged_msgsLowCmdSize(Ros1Unitree_legged_msgsLowCmdCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsLowCmdCapture(value),n=Ros1Unitree_legged_msgsLowCmdSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsLowCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,734,90);if(owned.length<734)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsLowCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsLowCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["levelFlag"],1,false);n=addSize(n,1);
integer(value["frameReserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["SN"])||value["SN"].length!==2)bad();
for(const element of value["SN"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandWidth"],2,false);n=addSize(n,2);
if(!Array.isArray(value["motorCmd"])||value["motorCmd"].length!==20)bad();
for(const element of value["motorCmd"]){
n=addSize(n,Ros1Unitree_legged_msgsMotorCmdSize(element));
}
n=addSize(n,Ros1Unitree_legged_msgsBmsCmdSize(value["bms"]));
if(!Array.isArray(value["wirelessRemote"])||value["wirelessRemote"].length!==40)bad();
for(const element of value["wirelessRemote"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(n>734)bad(90);return n;}
function Ros1Unitree_legged_msgsLowCmdWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["levelFlag"],true);w.at+=1;
w.v.setUint8(w.at,value["frameReserve"],true);w.at+=1;
for(const element of value["SN"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandWidth"],true);w.at+=2;
for(const element of value["motorCmd"]){
Ros1Unitree_legged_msgsMotorCmdWrite(element,w);
}
Ros1Unitree_legged_msgsBmsCmdWrite(value["bms"],w);
for(const element of value["wirelessRemote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros1Unitree_legged_msgsLowCmdRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["levelFlag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frameReserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["SN"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["SN"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["bandWidth"]=decoded;
}
{
{const count=20;if(count>20)bad();value["motorCmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorCmdRead(r);
value["motorCmd"].push(decoded);
}}
}
{
const decoded=Ros1Unitree_legged_msgsBmsCmdRead(r);
value["bms"]=decoded;
}
{
{const count=40;if(count>40)bad();value["wirelessRemote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wirelessRemote"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsLowCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["levelFlag"];
out["levelFlag"]=field;
}
{const field=value["frameReserve"];
out["frameReserve"]=field;
}
{const field=value["SN"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["SN"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandWidth"];
out["bandWidth"]=field;
}
{const field=value["motorCmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsMotorCmdCapture(element);
}out["motorCmd"]=values;
}
{const field=value["bms"];
out["bms"]=Ros1Unitree_legged_msgsBmsCmdCapture(field);
}
{const field=value["wirelessRemote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wirelessRemote"]=values;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS1 unitree_legged_msgs/LowState codec; maxima are not actual lengths. */
export const Ros1Unitree_legged_msgsLowStateCodec=Object.freeze({codecIdentity:"UWVMROS1_37425ed49c0eebd762b6084ee413c7706fc43defc524414d3f50cdd2d25552c0",maxWireBytes:1069,minWireBytes:1069,cartesianMaxWireBytes:1069n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"levelFlag":0,"frameReserve":0,"SN":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandWidth":0,"imu":Ros1Unitree_legged_msgsIMUCodec.defaultValue(),"motorState":Array.from({length:20},()=>(Ros1Unitree_legged_msgsMotorStateCodec.defaultValue())),"bms":Ros1Unitree_legged_msgsBmsStateCodec.defaultValue(),"footForce":Array.from({length:4},()=>(0)),"footForceEst":Array.from({length:4},()=>(0)),"tick":0,"wirelessRemote":Array.from({length:40},()=>(0)),"reserve":0,"crc":0,"eeForceRaw":Array.from({length:4},()=>(Ros1Unitree_legged_msgsCartesianCodec.defaultValue())),"eeForce":Array.from({length:4},()=>(Ros1Unitree_legged_msgsCartesianCodec.defaultValue())),"position":Ros1Unitree_legged_msgsCartesianCodec.defaultValue(),"velocity":Ros1Unitree_legged_msgsCartesianCodec.defaultValue(),"velocity_w":Ros1Unitree_legged_msgsCartesianCodec.defaultValue()};},
validateValue(value){Ros1Unitree_legged_msgsLowStateSize(Ros1Unitree_legged_msgsLowStateCapture(value));return true;},
encode(value){const owned=Ros1Unitree_legged_msgsLowStateCapture(value),n=Ros1Unitree_legged_msgsLowStateSize(owned),w=new Writer(n);Ros1Unitree_legged_msgsLowStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1069,90);if(owned.length<1069)bad();const r=new Reader(owned),value=Ros1Unitree_legged_msgsLowStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Unitree_legged_msgsLowStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["levelFlag"],1,false);n=addSize(n,1);
integer(value["frameReserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["SN"])||value["SN"].length!==2)bad();
for(const element of value["SN"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandWidth"],2,false);n=addSize(n,2);
n=addSize(n,Ros1Unitree_legged_msgsIMUSize(value["imu"]));
if(!Array.isArray(value["motorState"])||value["motorState"].length!==20)bad();
for(const element of value["motorState"]){
n=addSize(n,Ros1Unitree_legged_msgsMotorStateSize(element));
}
n=addSize(n,Ros1Unitree_legged_msgsBmsStateSize(value["bms"]));
if(!Array.isArray(value["footForce"])||value["footForce"].length!==4)bad();
for(const element of value["footForce"]){
integer(element,2,true);n=addSize(n,2);
}
if(!Array.isArray(value["footForceEst"])||value["footForceEst"].length!==4)bad();
for(const element of value["footForceEst"]){
integer(element,2,true);n=addSize(n,2);
}
integer(value["tick"],4,false);n=addSize(n,4);
if(!Array.isArray(value["wirelessRemote"])||value["wirelessRemote"].length!==40)bad();
for(const element of value["wirelessRemote"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(!Array.isArray(value["eeForceRaw"])||value["eeForceRaw"].length!==4)bad();
for(const element of value["eeForceRaw"]){
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(element));
}
if(!Array.isArray(value["eeForce"])||value["eeForce"].length!==4)bad();
for(const element of value["eeForce"]){
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(element));
}
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(value["position"]));
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(value["velocity"]));
n=addSize(n,Ros1Unitree_legged_msgsCartesianSize(value["velocity_w"]));
if(n>1069)bad(90);return n;}
function Ros1Unitree_legged_msgsLowStateWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["levelFlag"],true);w.at+=1;
w.v.setUint8(w.at,value["frameReserve"],true);w.at+=1;
for(const element of value["SN"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandWidth"],true);w.at+=2;
Ros1Unitree_legged_msgsIMUWrite(value["imu"],w);
for(const element of value["motorState"]){
Ros1Unitree_legged_msgsMotorStateWrite(element,w);
}
Ros1Unitree_legged_msgsBmsStateWrite(value["bms"],w);
for(const element of value["footForce"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
for(const element of value["footForceEst"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
w.v.setUint32(w.at,value["tick"],true);w.at+=4;
for(const element of value["wirelessRemote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
for(const element of value["eeForceRaw"]){
Ros1Unitree_legged_msgsCartesianWrite(element,w);
}
for(const element of value["eeForce"]){
Ros1Unitree_legged_msgsCartesianWrite(element,w);
}
Ros1Unitree_legged_msgsCartesianWrite(value["position"],w);
Ros1Unitree_legged_msgsCartesianWrite(value["velocity"],w);
Ros1Unitree_legged_msgsCartesianWrite(value["velocity_w"],w);
}
function Ros1Unitree_legged_msgsLowStateRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["levelFlag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frameReserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["SN"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["SN"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["bandWidth"]=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsIMURead(r);
value["imu"]=decoded;
}
{
{const count=20;if(count>20)bad();value["motorState"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorStateRead(r);
value["motorState"].push(decoded);
}}
}
{
const decoded=Ros1Unitree_legged_msgsBmsStateRead(r);
value["bms"]=decoded;
}
{
{const count=4;if(count>4)bad();value["footForce"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["footForce"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["footForceEst"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["footForceEst"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["tick"]=decoded;
}
{
{const count=40;if(count>40)bad();value["wirelessRemote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wirelessRemote"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["crc"]=decoded;
}
{
{const count=4;if(count>4)bad();value["eeForceRaw"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["eeForceRaw"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["eeForce"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["eeForce"].push(decoded);
}}
}
{
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["position"]=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["velocity"]=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsCartesianRead(r);
value["velocity_w"]=decoded;
}
return value;}
function Ros1Unitree_legged_msgsLowStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["levelFlag"];
out["levelFlag"]=field;
}
{const field=value["frameReserve"];
out["frameReserve"]=field;
}
{const field=value["SN"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["SN"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandWidth"];
out["bandWidth"]=field;
}
{const field=value["imu"];
out["imu"]=Ros1Unitree_legged_msgsIMUCapture(field);
}
{const field=value["motorState"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsMotorStateCapture(element);
}out["motorState"]=values;
}
{const field=value["bms"];
out["bms"]=Ros1Unitree_legged_msgsBmsStateCapture(field);
}
{const field=value["footForce"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["footForce"]=values;
}
{const field=value["footForceEst"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["footForceEst"]=values;
}
{const field=value["tick"];
out["tick"]=field;
}
{const field=value["wirelessRemote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wirelessRemote"]=values;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
{const field=value["eeForceRaw"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsCartesianCapture(element);
}out["eeForceRaw"]=values;
}
{const field=value["eeForce"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Unitree_legged_msgsCartesianCapture(element);
}out["eeForce"]=values;
}
{const field=value["position"];
out["position"]=Ros1Unitree_legged_msgsCartesianCapture(field);
}
{const field=value["velocity"];
out["velocity"]=Ros1Unitree_legged_msgsCartesianCapture(field);
}
{const field=value["velocity_w"];
out["velocity_w"]=Ros1Unitree_legged_msgsCartesianCapture(field);
}
return out;}
