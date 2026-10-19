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
/** Complete ROS2 unitree_go/msg/AudioData codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgAudioDataCodec=Object.freeze({codecIdentity:"UWVMROS1_aa14877a076c7fc6b662efa965cc8a75f42b3f84990a078c7c3c5def01d2c9fe",maxWireBytes:2060,minWireBytes:12,cartesianMaxWireBytes:2060n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"time_frame":0n,"data":[]};},
validateValue(value){Ros2Unitree_goMsgAudioDataSize(Ros2Unitree_goMsgAudioDataCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgAudioDataCapture(value),n=Ros2Unitree_goMsgAudioDataSize(owned),w=new Writer(n);Ros2Unitree_goMsgAudioDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2060,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgAudioDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgAudioDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["time_frame"],8,false);n=addSize(n,8);
if(!Array.isArray(value["data"])||value["data"].length>2048)bad();
n=addSize(n,4);
for(const element of value["data"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>2060)bad(90);return n;}
function Ros2Unitree_goMsgAudioDataWrite(value,w){
w.v.setBigUint64(w.at,value["time_frame"],true);w.at+=8;
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_goMsgAudioDataRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["time_frame"]=decoded;
}
{
{const count=r.u32();if(count>2048)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["data"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgAudioDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["time_frame"];
out["time_frame"]=field;
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/BmsCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgBmsCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_d212c1ab0c92d64801a3b10c3a4d79517ea24a830db3d85ca2bb18e1e0ea5339",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"off":0,"reserve":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgBmsCmdSize(Ros2Unitree_goMsgBmsCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgBmsCmdCapture(value),n=Ros2Unitree_goMsgBmsCmdSize(owned),w=new Writer(n);Ros2Unitree_goMsgBmsCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgBmsCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgBmsCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["off"],1,false);n=addSize(n,1);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==3)bad();
for(const element of value["reserve"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>4)bad(90);return n;}
function Ros2Unitree_goMsgBmsCmdWrite(value,w){
w.v.setUint8(w.at,value["off"],true);w.at+=1;
for(const element of value["reserve"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_goMsgBmsCmdRead(r){const value={};
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
function Ros2Unitree_goMsgBmsCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["off"];
out["off"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/BmsState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgBmsStateCodec=Object.freeze({codecIdentity:"UWVMROS1_7e4d5e3c0a2eddddf39c68b73bff5f7721e1623b0a9322dce23d3e551a07f5b0",maxWireBytes:44,minWireBytes:44,cartesianMaxWireBytes:44n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version_high":0,"version_low":0,"status":0,"soc":0,"current":0,"cycle":0,"bq_ntc":Array.from({length:2},()=>(0)),"mcu_ntc":Array.from({length:2},()=>(0)),"cell_vol":Array.from({length:15},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgBmsStateSize(Ros2Unitree_goMsgBmsStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgBmsStateCapture(value),n=Ros2Unitree_goMsgBmsStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgBmsStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,44,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgBmsStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgBmsStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["version_high"],1,false);n=addSize(n,1);
integer(value["version_low"],1,false);n=addSize(n,1);
integer(value["status"],1,false);n=addSize(n,1);
integer(value["soc"],1,false);n=addSize(n,1);
integer(value["current"],4,true);n=addSize(n,4);
integer(value["cycle"],2,false);n=addSize(n,2);
if(!Array.isArray(value["bq_ntc"])||value["bq_ntc"].length!==2)bad();
for(const element of value["bq_ntc"]){
integer(element,1,true);n=addSize(n,1);
}
if(!Array.isArray(value["mcu_ntc"])||value["mcu_ntc"].length!==2)bad();
for(const element of value["mcu_ntc"]){
integer(element,1,true);n=addSize(n,1);
}
if(!Array.isArray(value["cell_vol"])||value["cell_vol"].length!==15)bad();
for(const element of value["cell_vol"]){
integer(element,2,false);n=addSize(n,2);
}
if(n>44)bad(90);return n;}
function Ros2Unitree_goMsgBmsStateWrite(value,w){
w.v.setUint8(w.at,value["version_high"],true);w.at+=1;
w.v.setUint8(w.at,value["version_low"],true);w.at+=1;
w.v.setUint8(w.at,value["status"],true);w.at+=1;
w.v.setUint8(w.at,value["soc"],true);w.at+=1;
w.v.setInt32(w.at,value["current"],true);w.at+=4;
w.v.setUint16(w.at,value["cycle"],true);w.at+=2;
for(const element of value["bq_ntc"]){
w.v.setInt8(w.at,element,true);w.at+=1;
}
for(const element of value["mcu_ntc"]){
w.v.setInt8(w.at,element,true);w.at+=1;
}
for(const element of value["cell_vol"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
}
function Ros2Unitree_goMsgBmsStateRead(r){const value={};
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
value["status"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["soc"]=decoded;
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
{const count=2;if(count>2)bad();value["bq_ntc"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt8(r.take(1),true);
value["bq_ntc"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["mcu_ntc"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt8(r.take(1),true);
value["mcu_ntc"].push(decoded);
}}
}
{
{const count=15;if(count>15)bad();value["cell_vol"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["cell_vol"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgBmsStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version_high"];
out["version_high"]=field;
}
{const field=value["version_low"];
out["version_low"]=field;
}
{const field=value["status"];
out["status"]=field;
}
{const field=value["soc"];
out["soc"]=field;
}
{const field=value["current"];
out["current"]=field;
}
{const field=value["cycle"];
out["cycle"]=field;
}
{const field=value["bq_ntc"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["bq_ntc"]=values;
}
{const field=value["mcu_ntc"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["mcu_ntc"]=values;
}
{const field=value["cell_vol"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==15)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["cell_vol"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/Error codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgErrorCodec=Object.freeze({codecIdentity:"UWVMROS1_c3e572e08288b6ed0b32575e645fe546f92b5091d62e57db01ec00d27f5e4afd",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"source":0,"state":0};},
validateValue(value){Ros2Unitree_goMsgErrorSize(Ros2Unitree_goMsgErrorCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgErrorCapture(value),n=Ros2Unitree_goMsgErrorSize(owned),w=new Writer(n);Ros2Unitree_goMsgErrorWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgErrorRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgErrorSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["source"],4,false);n=addSize(n,4);
integer(value["state"],4,false);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Unitree_goMsgErrorWrite(value,w){
w.v.setUint32(w.at,value["source"],true);w.at+=4;
w.v.setUint32(w.at,value["state"],true);w.at+=4;
}
function Ros2Unitree_goMsgErrorRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["source"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["state"]=decoded;
}
return value;}
function Ros2Unitree_goMsgErrorCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["source"];
out["source"]=field;
}
{const field=value["state"];
out["state"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/Go2FrontVideoData codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgGo2FrontVideoDataCodec=Object.freeze({codecIdentity:"UWVMROS1_322b6bd4e563436838fe6a05174d173c0d95eb48108b889dca4f61ce6cc71c60",maxWireBytes:3092,minWireBytes:20,cartesianMaxWireBytes:3092n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"time_frame":0n,"video720p":[],"video360p":[],"video180p":[]};},
validateValue(value){Ros2Unitree_goMsgGo2FrontVideoDataSize(Ros2Unitree_goMsgGo2FrontVideoDataCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgGo2FrontVideoDataCapture(value),n=Ros2Unitree_goMsgGo2FrontVideoDataSize(owned),w=new Writer(n);Ros2Unitree_goMsgGo2FrontVideoDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,3092,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgGo2FrontVideoDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgGo2FrontVideoDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["time_frame"],8,false);n=addSize(n,8);
if(!Array.isArray(value["video720p"])||value["video720p"].length>1024)bad();
n=addSize(n,4);
for(const element of value["video720p"]){
integer(element,1,false);n=addSize(n,1);
}
if(!Array.isArray(value["video360p"])||value["video360p"].length>1024)bad();
n=addSize(n,4);
for(const element of value["video360p"]){
integer(element,1,false);n=addSize(n,1);
}
if(!Array.isArray(value["video180p"])||value["video180p"].length>1024)bad();
n=addSize(n,4);
for(const element of value["video180p"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>3092)bad(90);return n;}
function Ros2Unitree_goMsgGo2FrontVideoDataWrite(value,w){
w.v.setBigUint64(w.at,value["time_frame"],true);w.at+=8;
w.v.setUint32(w.at,value["video720p"].length,true);w.at+=4;
for(const element of value["video720p"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["video360p"].length,true);w.at+=4;
for(const element of value["video360p"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint32(w.at,value["video180p"].length,true);w.at+=4;
for(const element of value["video180p"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_goMsgGo2FrontVideoDataRead(r){const value={};
{
const decoded=r.v.getBigUint64(r.take(8),true);
value["time_frame"]=decoded;
}
{
{const count=r.u32();if(count>1024)bad();value["video720p"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["video720p"].push(decoded);
}}
}
{
{const count=r.u32();if(count>1024)bad();value["video360p"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["video360p"].push(decoded);
}}
}
{
{const count=r.u32();if(count>1024)bad();value["video180p"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["video180p"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgGo2FrontVideoDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["time_frame"];
out["time_frame"]=field;
}
{const field=value["video720p"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["video720p"]=values;
}
{const field=value["video360p"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["video360p"]=values;
}
{const field=value["video180p"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["video180p"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/HeightMap codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgHeightMapCodec=Object.freeze({codecIdentity:"UWVMROS1_f4aed8c4e1ba382c418ca83a10ae617355f1169757128be8b7cca46611bb0012",maxWireBytes:8356,minWireBytes:36,cartesianMaxWireBytes:8356n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":0,"frame_id":"","resolution":0,"width":0,"height":0,"origin":Array.from({length:2},()=>(0)),"data":[]};},
validateValue(value){Ros2Unitree_goMsgHeightMapSize(Ros2Unitree_goMsgHeightMapCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgHeightMapCapture(value),n=Ros2Unitree_goMsgHeightMapSize(owned),w=new Writer(n);Ros2Unitree_goMsgHeightMapWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8356,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgHeightMapRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgHeightMapSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["stamp"]!=="number"||!Number.isFinite(value["stamp"]))bad();n=addSize(n,8);
n=addSize(n,4+textBytes(value["frame_id"],128).length);
if(typeof value["resolution"]!=="number"||!Number.isFinite(value["resolution"])||!Number.isFinite(Math.fround(value["resolution"])))bad();n=addSize(n,4);
integer(value["width"],4,false);n=addSize(n,4);
integer(value["height"],4,false);n=addSize(n,4);
if(!Array.isArray(value["origin"])||value["origin"].length!==2)bad();
for(const element of value["origin"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["data"])||value["data"].length>2048)bad();
n=addSize(n,4);
for(const element of value["data"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>8356)bad(90);return n;}
function Ros2Unitree_goMsgHeightMapWrite(value,w){
w.v.setFloat64(w.at,value["stamp"],true);w.at+=8;
w.text(value["frame_id"],128);
w.v.setFloat32(w.at,value["resolution"],true);w.at+=4;
w.v.setUint32(w.at,value["width"],true);w.at+=4;
w.v.setUint32(w.at,value["height"],true);w.at+=4;
for(const element of value["origin"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_goMsgHeightMapRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["stamp"]=decoded;
}
{
const decoded=r.text(128);
value["frame_id"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["resolution"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["width"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["height"]=decoded;
}
{
{const count=2;if(count>2)bad();value["origin"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["origin"].push(decoded);
}}
}
{
{const count=r.u32();if(count>2048)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["data"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgHeightMapCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["stamp"];
out["stamp"]=field;
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
{const field=value["resolution"];
out["resolution"]=field;
}
{const field=value["width"];
out["width"]=field;
}
{const field=value["height"];
out["height"]=field;
}
{const field=value["origin"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["origin"]=values;
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/IMUState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgIMUStateCodec=Object.freeze({codecIdentity:"UWVMROS1_e5326e97c3bfaa06962ad1989d1aa13fa0e60af15d39d9ef6da6ac0a37261bbc",maxWireBytes:53,minWireBytes:53,cartesianMaxWireBytes:53n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"quaternion":Array.from({length:4},()=>(0)),"gyroscope":Array.from({length:3},()=>(0)),"accelerometer":Array.from({length:3},()=>(0)),"rpy":Array.from({length:3},()=>(0)),"temperature":0};},
validateValue(value){Ros2Unitree_goMsgIMUStateSize(Ros2Unitree_goMsgIMUStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgIMUStateCapture(value),n=Ros2Unitree_goMsgIMUStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgIMUStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,53,90);if(owned.length<53)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgIMUStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgIMUStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros2Unitree_goMsgIMUStateWrite(value,w){
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
function Ros2Unitree_goMsgIMUStateRead(r){const value={};
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
function Ros2Unitree_goMsgIMUStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 unitree_go/msg/InterfaceConfig codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgInterfaceConfigCodec=Object.freeze({codecIdentity:"UWVMROS1_9b96ed6672f6a3e039edb28dec8384cd195116dda929d5ba7be85ef7e7668110",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"value":0,"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgInterfaceConfigSize(Ros2Unitree_goMsgInterfaceConfigCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgInterfaceConfigCapture(value),n=Ros2Unitree_goMsgInterfaceConfigSize(owned),w=new Writer(n);Ros2Unitree_goMsgInterfaceConfigWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgInterfaceConfigRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgInterfaceConfigSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["value"],1,false);n=addSize(n,1);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>4)bad(90);return n;}
function Ros2Unitree_goMsgInterfaceConfigWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["value"],true);w.at+=1;
for(const element of value["reserve"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_goMsgInterfaceConfigRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["value"]=decoded;
}
{
{const count=2;if(count>2)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgInterfaceConfigCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["value"];
out["value"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/LidarState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgLidarStateCodec=Object.freeze({codecIdentity:"UWVMROS1_edc9253eb66e466ae5a893b830b0bc5c9102ef18d2bbed7f7d9ead67bcb1a8e6",maxWireBytes:465,minWireBytes:81,cartesianMaxWireBytes:465n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":0,"firmware_version":"","software_version":"","sdk_version":"","sys_rotation_speed":0,"com_rotation_speed":0,"error_state":0,"cloud_frequency":0,"cloud_packet_loss_rate":0,"cloud_size":0,"cloud_scan_num":0,"imu_frequency":0,"imu_packet_loss_rate":0,"imu_rpy":Array.from({length:3},()=>(0)),"serial_recv_stamp":0,"serial_buffer_size":0,"serial_buffer_read":0};},
validateValue(value){Ros2Unitree_goMsgLidarStateSize(Ros2Unitree_goMsgLidarStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgLidarStateCapture(value),n=Ros2Unitree_goMsgLidarStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgLidarStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,465,90);if(owned.length<81)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgLidarStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgLidarStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["stamp"]!=="number"||!Number.isFinite(value["stamp"]))bad();n=addSize(n,8);
n=addSize(n,4+textBytes(value["firmware_version"],128).length);
n=addSize(n,4+textBytes(value["software_version"],128).length);
n=addSize(n,4+textBytes(value["sdk_version"],128).length);
if(typeof value["sys_rotation_speed"]!=="number"||!Number.isFinite(value["sys_rotation_speed"])||!Number.isFinite(Math.fround(value["sys_rotation_speed"])))bad();n=addSize(n,4);
if(typeof value["com_rotation_speed"]!=="number"||!Number.isFinite(value["com_rotation_speed"])||!Number.isFinite(Math.fround(value["com_rotation_speed"])))bad();n=addSize(n,4);
integer(value["error_state"],1,false);n=addSize(n,1);
if(typeof value["cloud_frequency"]!=="number"||!Number.isFinite(value["cloud_frequency"])||!Number.isFinite(Math.fround(value["cloud_frequency"])))bad();n=addSize(n,4);
if(typeof value["cloud_packet_loss_rate"]!=="number"||!Number.isFinite(value["cloud_packet_loss_rate"])||!Number.isFinite(Math.fround(value["cloud_packet_loss_rate"])))bad();n=addSize(n,4);
integer(value["cloud_size"],4,false);n=addSize(n,4);
integer(value["cloud_scan_num"],4,false);n=addSize(n,4);
if(typeof value["imu_frequency"]!=="number"||!Number.isFinite(value["imu_frequency"])||!Number.isFinite(Math.fround(value["imu_frequency"])))bad();n=addSize(n,4);
if(typeof value["imu_packet_loss_rate"]!=="number"||!Number.isFinite(value["imu_packet_loss_rate"])||!Number.isFinite(Math.fround(value["imu_packet_loss_rate"])))bad();n=addSize(n,4);
if(!Array.isArray(value["imu_rpy"])||value["imu_rpy"].length!==3)bad();
for(const element of value["imu_rpy"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["serial_recv_stamp"]!=="number"||!Number.isFinite(value["serial_recv_stamp"]))bad();n=addSize(n,8);
integer(value["serial_buffer_size"],4,false);n=addSize(n,4);
integer(value["serial_buffer_read"],4,false);n=addSize(n,4);
if(n>465)bad(90);return n;}
function Ros2Unitree_goMsgLidarStateWrite(value,w){
w.v.setFloat64(w.at,value["stamp"],true);w.at+=8;
w.text(value["firmware_version"],128);
w.text(value["software_version"],128);
w.text(value["sdk_version"],128);
w.v.setFloat32(w.at,value["sys_rotation_speed"],true);w.at+=4;
w.v.setFloat32(w.at,value["com_rotation_speed"],true);w.at+=4;
w.v.setUint8(w.at,value["error_state"],true);w.at+=1;
w.v.setFloat32(w.at,value["cloud_frequency"],true);w.at+=4;
w.v.setFloat32(w.at,value["cloud_packet_loss_rate"],true);w.at+=4;
w.v.setUint32(w.at,value["cloud_size"],true);w.at+=4;
w.v.setUint32(w.at,value["cloud_scan_num"],true);w.at+=4;
w.v.setFloat32(w.at,value["imu_frequency"],true);w.at+=4;
w.v.setFloat32(w.at,value["imu_packet_loss_rate"],true);w.at+=4;
for(const element of value["imu_rpy"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat64(w.at,value["serial_recv_stamp"],true);w.at+=8;
w.v.setUint32(w.at,value["serial_buffer_size"],true);w.at+=4;
w.v.setUint32(w.at,value["serial_buffer_read"],true);w.at+=4;
}
function Ros2Unitree_goMsgLidarStateRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["stamp"]=decoded;
}
{
const decoded=r.text(128);
value["firmware_version"]=decoded;
}
{
const decoded=r.text(128);
value["software_version"]=decoded;
}
{
const decoded=r.text(128);
value["sdk_version"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["sys_rotation_speed"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["com_rotation_speed"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["error_state"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cloud_frequency"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cloud_packet_loss_rate"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["cloud_size"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["cloud_scan_num"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["imu_frequency"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["imu_packet_loss_rate"]=decoded;
}
{
{const count=3;if(count>3)bad();value["imu_rpy"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["imu_rpy"].push(decoded);
}}
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["serial_recv_stamp"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["serial_buffer_size"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["serial_buffer_read"]=decoded;
}
return value;}
function Ros2Unitree_goMsgLidarStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["stamp"];
out["stamp"]=field;
}
{const field=value["firmware_version"];
out["firmware_version"]=field;
}
{const field=value["software_version"];
out["software_version"]=field;
}
{const field=value["sdk_version"];
out["sdk_version"]=field;
}
{const field=value["sys_rotation_speed"];
out["sys_rotation_speed"]=field;
}
{const field=value["com_rotation_speed"];
out["com_rotation_speed"]=field;
}
{const field=value["error_state"];
out["error_state"]=field;
}
{const field=value["cloud_frequency"];
out["cloud_frequency"]=field;
}
{const field=value["cloud_packet_loss_rate"];
out["cloud_packet_loss_rate"]=field;
}
{const field=value["cloud_size"];
out["cloud_size"]=field;
}
{const field=value["cloud_scan_num"];
out["cloud_scan_num"]=field;
}
{const field=value["imu_frequency"];
out["imu_frequency"]=field;
}
{const field=value["imu_packet_loss_rate"];
out["imu_packet_loss_rate"]=field;
}
{const field=value["imu_rpy"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["imu_rpy"]=values;
}
{const field=value["serial_recv_stamp"];
out["serial_recv_stamp"]=field;
}
{const field=value["serial_buffer_size"];
out["serial_buffer_size"]=field;
}
{const field=value["serial_buffer_read"];
out["serial_buffer_read"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/MotorCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgMotorCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_e5b88c02a6d339ae4ae2706678a3d82f89904f024b6068f4fd174e1e045b0989",maxWireBytes:33,minWireBytes:33,cartesianMaxWireBytes:33n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"tau":0,"kp":0,"kd":0,"reserve":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgMotorCmdSize(Ros2Unitree_goMsgMotorCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgMotorCmdCapture(value),n=Ros2Unitree_goMsgMotorCmdSize(owned),w=new Writer(n);Ros2Unitree_goMsgMotorCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,33,90);if(owned.length<33)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgMotorCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgMotorCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["tau"]!=="number"||!Number.isFinite(value["tau"])||!Number.isFinite(Math.fround(value["tau"])))bad();n=addSize(n,4);
if(typeof value["kp"]!=="number"||!Number.isFinite(value["kp"])||!Number.isFinite(Math.fround(value["kp"])))bad();n=addSize(n,4);
if(typeof value["kd"]!=="number"||!Number.isFinite(value["kd"])||!Number.isFinite(Math.fround(value["kd"])))bad();n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==3)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>33)bad(90);return n;}
function Ros2Unitree_goMsgMotorCmdWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau"],true);w.at+=4;
w.v.setFloat32(w.at,value["kp"],true);w.at+=4;
w.v.setFloat32(w.at,value["kd"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_goMsgMotorCmdRead(r){const value={};
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
{const count=3;if(count>3)bad();value["reserve"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["reserve"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgMotorCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/LowCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgLowCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_efd061d06d38ecc686edb95d1a891c26860b16da1e35747fc97eeaf4a5d93d63",maxWireBytes:749,minWireBytes:749,cartesianMaxWireBytes:749n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"level_flag":0,"frame_reserve":0,"sn":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandwidth":0,"motor_cmd":Array.from({length:20},()=>(Ros2Unitree_goMsgMotorCmdCodec.defaultValue())),"bms_cmd":Ros2Unitree_goMsgBmsCmdCodec.defaultValue(),"wireless_remote":Array.from({length:40},()=>(0)),"led":Array.from({length:12},()=>(0)),"fan":Array.from({length:2},()=>(0)),"gpio":0,"reserve":0,"crc":0};},
validateValue(value){Ros2Unitree_goMsgLowCmdSize(Ros2Unitree_goMsgLowCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgLowCmdCapture(value),n=Ros2Unitree_goMsgLowCmdSize(owned),w=new Writer(n);Ros2Unitree_goMsgLowCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,749,90);if(owned.length<749)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgLowCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgLowCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["level_flag"],1,false);n=addSize(n,1);
integer(value["frame_reserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["sn"])||value["sn"].length!==2)bad();
for(const element of value["sn"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandwidth"],2,false);n=addSize(n,2);
if(!Array.isArray(value["motor_cmd"])||value["motor_cmd"].length!==20)bad();
for(const element of value["motor_cmd"]){
n=addSize(n,Ros2Unitree_goMsgMotorCmdSize(element));
}
n=addSize(n,Ros2Unitree_goMsgBmsCmdSize(value["bms_cmd"]));
if(!Array.isArray(value["wireless_remote"])||value["wireless_remote"].length!==40)bad();
for(const element of value["wireless_remote"]){
integer(element,1,false);n=addSize(n,1);
}
if(!Array.isArray(value["led"])||value["led"].length!==12)bad();
for(const element of value["led"]){
integer(element,1,false);n=addSize(n,1);
}
if(!Array.isArray(value["fan"])||value["fan"].length!==2)bad();
for(const element of value["fan"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["gpio"],1,false);n=addSize(n,1);
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(n>749)bad(90);return n;}
function Ros2Unitree_goMsgLowCmdWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["level_flag"],true);w.at+=1;
w.v.setUint8(w.at,value["frame_reserve"],true);w.at+=1;
for(const element of value["sn"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandwidth"],true);w.at+=2;
for(const element of value["motor_cmd"]){
Ros2Unitree_goMsgMotorCmdWrite(element,w);
}
Ros2Unitree_goMsgBmsCmdWrite(value["bms_cmd"],w);
for(const element of value["wireless_remote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
for(const element of value["led"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
for(const element of value["fan"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["gpio"],true);w.at+=1;
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros2Unitree_goMsgLowCmdRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["level_flag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frame_reserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["sn"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["sn"].push(decoded);
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
value["bandwidth"]=decoded;
}
{
{const count=20;if(count>20)bad();value["motor_cmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorCmdRead(r);
value["motor_cmd"].push(decoded);
}}
}
{
const decoded=Ros2Unitree_goMsgBmsCmdRead(r);
value["bms_cmd"]=decoded;
}
{
{const count=40;if(count>40)bad();value["wireless_remote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wireless_remote"].push(decoded);
}}
}
{
{const count=12;if(count>12)bad();value["led"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["led"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["fan"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["fan"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gpio"]=decoded;
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
function Ros2Unitree_goMsgLowCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["level_flag"];
out["level_flag"]=field;
}
{const field=value["frame_reserve"];
out["frame_reserve"]=field;
}
{const field=value["sn"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["sn"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandwidth"];
out["bandwidth"]=field;
}
{const field=value["motor_cmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_goMsgMotorCmdCapture(element);
}out["motor_cmd"]=values;
}
{const field=value["bms_cmd"];
out["bms_cmd"]=Ros2Unitree_goMsgBmsCmdCapture(field);
}
{const field=value["wireless_remote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wireless_remote"]=values;
}
{const field=value["led"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["led"]=values;
}
{const field=value["fan"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["fan"]=values;
}
{const field=value["gpio"];
out["gpio"]=field;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/MotorState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_b0b5e638b9fd9eec6d0019422d417f80c60e61d47f4750fe9adac8c5084a2a62",maxWireBytes:42,minWireBytes:42,cartesianMaxWireBytes:42n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"ddq":0,"tau_est":0,"q_raw":0,"dq_raw":0,"ddq_raw":0,"temperature":0,"lost":0,"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgMotorStateSize(Ros2Unitree_goMsgMotorStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgMotorStateCapture(value),n=Ros2Unitree_goMsgMotorStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,42,90);if(owned.length<42)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["ddq"]!=="number"||!Number.isFinite(value["ddq"])||!Number.isFinite(Math.fround(value["ddq"])))bad();n=addSize(n,4);
if(typeof value["tau_est"]!=="number"||!Number.isFinite(value["tau_est"])||!Number.isFinite(Math.fround(value["tau_est"])))bad();n=addSize(n,4);
if(typeof value["q_raw"]!=="number"||!Number.isFinite(value["q_raw"])||!Number.isFinite(Math.fround(value["q_raw"])))bad();n=addSize(n,4);
if(typeof value["dq_raw"]!=="number"||!Number.isFinite(value["dq_raw"])||!Number.isFinite(Math.fround(value["dq_raw"])))bad();n=addSize(n,4);
if(typeof value["ddq_raw"]!=="number"||!Number.isFinite(value["ddq_raw"])||!Number.isFinite(Math.fround(value["ddq_raw"])))bad();n=addSize(n,4);
integer(value["temperature"],1,true);n=addSize(n,1);
integer(value["lost"],4,false);n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>42)bad(90);return n;}
function Ros2Unitree_goMsgMotorStateWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau_est"],true);w.at+=4;
w.v.setFloat32(w.at,value["q_raw"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq_raw"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq_raw"],true);w.at+=4;
w.v.setInt8(w.at,value["temperature"],true);w.at+=1;
w.v.setUint32(w.at,value["lost"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_goMsgMotorStateRead(r){const value={};
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
function Ros2Unitree_goMsgMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
{const field=value["lost"];
out["lost"]=field;
}
{const field=value["reserve"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["reserve"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/LowState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgLowStateCodec=Object.freeze({codecIdentity:"UWVMROS1_56c01d52b26d7b2ef1fa7475fc8a353777ed6952a6f1ceee01da81f1edad3cab",maxWireBytes:1050,minWireBytes:1050,cartesianMaxWireBytes:1050n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"head":Array.from({length:2},()=>(0)),"level_flag":0,"frame_reserve":0,"sn":Array.from({length:2},()=>(0)),"version":Array.from({length:2},()=>(0)),"bandwidth":0,"imu_state":Ros2Unitree_goMsgIMUStateCodec.defaultValue(),"motor_state":Array.from({length:20},()=>(Ros2Unitree_goMsgMotorStateCodec.defaultValue())),"bms_state":Ros2Unitree_goMsgBmsStateCodec.defaultValue(),"foot_force":Array.from({length:4},()=>(0)),"foot_force_est":Array.from({length:4},()=>(0)),"tick":0,"wireless_remote":Array.from({length:40},()=>(0)),"bit_flag":0,"adc_reel":0,"temperature_ntc1":0,"temperature_ntc2":0,"power_v":0,"power_a":0,"fan_frequency":Array.from({length:4},()=>(0)),"reserve":0,"crc":0};},
validateValue(value){Ros2Unitree_goMsgLowStateSize(Ros2Unitree_goMsgLowStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgLowStateCapture(value),n=Ros2Unitree_goMsgLowStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgLowStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1050,90);if(owned.length<1050)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgLowStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgLowStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["head"])||value["head"].length!==2)bad();
for(const element of value["head"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["level_flag"],1,false);n=addSize(n,1);
integer(value["frame_reserve"],1,false);n=addSize(n,1);
if(!Array.isArray(value["sn"])||value["sn"].length!==2)bad();
for(const element of value["sn"]){
integer(element,4,false);n=addSize(n,4);
}
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,4,false);n=addSize(n,4);
}
integer(value["bandwidth"],2,false);n=addSize(n,2);
n=addSize(n,Ros2Unitree_goMsgIMUStateSize(value["imu_state"]));
if(!Array.isArray(value["motor_state"])||value["motor_state"].length!==20)bad();
for(const element of value["motor_state"]){
n=addSize(n,Ros2Unitree_goMsgMotorStateSize(element));
}
n=addSize(n,Ros2Unitree_goMsgBmsStateSize(value["bms_state"]));
if(!Array.isArray(value["foot_force"])||value["foot_force"].length!==4)bad();
for(const element of value["foot_force"]){
integer(element,2,true);n=addSize(n,2);
}
if(!Array.isArray(value["foot_force_est"])||value["foot_force_est"].length!==4)bad();
for(const element of value["foot_force_est"]){
integer(element,2,true);n=addSize(n,2);
}
integer(value["tick"],4,false);n=addSize(n,4);
if(!Array.isArray(value["wireless_remote"])||value["wireless_remote"].length!==40)bad();
for(const element of value["wireless_remote"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["bit_flag"],1,false);n=addSize(n,1);
if(typeof value["adc_reel"]!=="number"||!Number.isFinite(value["adc_reel"])||!Number.isFinite(Math.fround(value["adc_reel"])))bad();n=addSize(n,4);
integer(value["temperature_ntc1"],1,true);n=addSize(n,1);
integer(value["temperature_ntc2"],1,true);n=addSize(n,1);
if(typeof value["power_v"]!=="number"||!Number.isFinite(value["power_v"])||!Number.isFinite(Math.fround(value["power_v"])))bad();n=addSize(n,4);
if(typeof value["power_a"]!=="number"||!Number.isFinite(value["power_a"])||!Number.isFinite(Math.fround(value["power_a"])))bad();n=addSize(n,4);
if(!Array.isArray(value["fan_frequency"])||value["fan_frequency"].length!==4)bad();
for(const element of value["fan_frequency"]){
integer(element,2,false);n=addSize(n,2);
}
integer(value["reserve"],4,false);n=addSize(n,4);
integer(value["crc"],4,false);n=addSize(n,4);
if(n>1050)bad(90);return n;}
function Ros2Unitree_goMsgLowStateWrite(value,w){
for(const element of value["head"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["level_flag"],true);w.at+=1;
w.v.setUint8(w.at,value["frame_reserve"],true);w.at+=1;
for(const element of value["sn"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
for(const element of value["version"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
w.v.setUint16(w.at,value["bandwidth"],true);w.at+=2;
Ros2Unitree_goMsgIMUStateWrite(value["imu_state"],w);
for(const element of value["motor_state"]){
Ros2Unitree_goMsgMotorStateWrite(element,w);
}
Ros2Unitree_goMsgBmsStateWrite(value["bms_state"],w);
for(const element of value["foot_force"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
for(const element of value["foot_force_est"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
w.v.setUint32(w.at,value["tick"],true);w.at+=4;
for(const element of value["wireless_remote"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["bit_flag"],true);w.at+=1;
w.v.setFloat32(w.at,value["adc_reel"],true);w.at+=4;
w.v.setInt8(w.at,value["temperature_ntc1"],true);w.at+=1;
w.v.setInt8(w.at,value["temperature_ntc2"],true);w.at+=1;
w.v.setFloat32(w.at,value["power_v"],true);w.at+=4;
w.v.setFloat32(w.at,value["power_a"],true);w.at+=4;
for(const element of value["fan_frequency"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
w.v.setUint32(w.at,value["reserve"],true);w.at+=4;
w.v.setUint32(w.at,value["crc"],true);w.at+=4;
}
function Ros2Unitree_goMsgLowStateRead(r){const value={};
{
{const count=2;if(count>2)bad();value["head"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["head"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["level_flag"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["frame_reserve"]=decoded;
}
{
{const count=2;if(count>2)bad();value["sn"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint32(r.take(4),true);
value["sn"].push(decoded);
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
value["bandwidth"]=decoded;
}
{
const decoded=Ros2Unitree_goMsgIMUStateRead(r);
value["imu_state"]=decoded;
}
{
{const count=20;if(count>20)bad();value["motor_state"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorStateRead(r);
value["motor_state"].push(decoded);
}}
}
{
const decoded=Ros2Unitree_goMsgBmsStateRead(r);
value["bms_state"]=decoded;
}
{
{const count=4;if(count>4)bad();value["foot_force"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["foot_force"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["foot_force_est"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["foot_force_est"].push(decoded);
}}
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["tick"]=decoded;
}
{
{const count=40;if(count>40)bad();value["wireless_remote"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["wireless_remote"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["bit_flag"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["adc_reel"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["temperature_ntc1"]=decoded;
}
{
const decoded=r.v.getInt8(r.take(1),true);
value["temperature_ntc2"]=decoded;
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
{const count=4;if(count>4)bad();value["fan_frequency"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["fan_frequency"].push(decoded);
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
function Ros2Unitree_goMsgLowStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["head"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["head"]=values;
}
{const field=value["level_flag"];
out["level_flag"]=field;
}
{const field=value["frame_reserve"];
out["frame_reserve"]=field;
}
{const field=value["sn"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["sn"]=values;
}
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["bandwidth"];
out["bandwidth"]=field;
}
{const field=value["imu_state"];
out["imu_state"]=Ros2Unitree_goMsgIMUStateCapture(field);
}
{const field=value["motor_state"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_goMsgMotorStateCapture(element);
}out["motor_state"]=values;
}
{const field=value["bms_state"];
out["bms_state"]=Ros2Unitree_goMsgBmsStateCapture(field);
}
{const field=value["foot_force"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["foot_force"]=values;
}
{const field=value["foot_force_est"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["foot_force_est"]=values;
}
{const field=value["tick"];
out["tick"]=field;
}
{const field=value["wireless_remote"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==40)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["wireless_remote"]=values;
}
{const field=value["bit_flag"];
out["bit_flag"]=field;
}
{const field=value["adc_reel"];
out["adc_reel"]=field;
}
{const field=value["temperature_ntc1"];
out["temperature_ntc1"]=field;
}
{const field=value["temperature_ntc2"];
out["temperature_ntc2"]=field;
}
{const field=value["power_v"];
out["power_v"]=field;
}
{const field=value["power_a"];
out["power_a"]=field;
}
{const field=value["fan_frequency"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["fan_frequency"]=values;
}
{const field=value["reserve"];
out["reserve"]=field;
}
{const field=value["crc"];
out["crc"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/MotorCmds codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgMotorCmdsCodec=Object.freeze({codecIdentity:"UWVMROS1_59f6b09147eb868cad93205c27cdd339e55f5928c8eef32da11ec5675c61a671",maxWireBytes:664,minWireBytes:4,cartesianMaxWireBytes:664n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cmds":[]};},
validateValue(value){Ros2Unitree_goMsgMotorCmdsSize(Ros2Unitree_goMsgMotorCmdsCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgMotorCmdsCapture(value),n=Ros2Unitree_goMsgMotorCmdsSize(owned),w=new Writer(n);Ros2Unitree_goMsgMotorCmdsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,664,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgMotorCmdsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgMotorCmdsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["cmds"])||value["cmds"].length>20)bad();
n=addSize(n,4);
for(const element of value["cmds"]){
n=addSize(n,Ros2Unitree_goMsgMotorCmdSize(element));
}
if(n>664)bad(90);return n;}
function Ros2Unitree_goMsgMotorCmdsWrite(value,w){
w.v.setUint32(w.at,value["cmds"].length,true);w.at+=4;
for(const element of value["cmds"]){
Ros2Unitree_goMsgMotorCmdWrite(element,w);
}
}
function Ros2Unitree_goMsgMotorCmdsRead(r){const value={};
{
{const count=r.u32();if(count>20)bad();value["cmds"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorCmdRead(r);
value["cmds"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgMotorCmdsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cmds"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_goMsgMotorCmdCapture(element);
}out["cmds"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/MotorStates codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgMotorStatesCodec=Object.freeze({codecIdentity:"UWVMROS1_0843528158199493384c36a65b0fb1e15244d21160c33aac82f9a87718c08178",maxWireBytes:844,minWireBytes:4,cartesianMaxWireBytes:844n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"states":[]};},
validateValue(value){Ros2Unitree_goMsgMotorStatesSize(Ros2Unitree_goMsgMotorStatesCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgMotorStatesCapture(value),n=Ros2Unitree_goMsgMotorStatesSize(owned),w=new Writer(n);Ros2Unitree_goMsgMotorStatesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,844,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgMotorStatesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgMotorStatesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["states"])||value["states"].length>20)bad();
n=addSize(n,4);
for(const element of value["states"]){
n=addSize(n,Ros2Unitree_goMsgMotorStateSize(element));
}
if(n>844)bad(90);return n;}
function Ros2Unitree_goMsgMotorStatesWrite(value,w){
w.v.setUint32(w.at,value["states"].length,true);w.at+=4;
for(const element of value["states"]){
Ros2Unitree_goMsgMotorStateWrite(element,w);
}
}
function Ros2Unitree_goMsgMotorStatesRead(r){const value={};
{
{const count=r.u32();if(count>20)bad();value["states"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorStateRead(r);
value["states"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgMotorStatesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["states"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>20)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_goMsgMotorStateCapture(element);
}out["states"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/PathPoint codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgPathPointCodec=Object.freeze({codecIdentity:"UWVMROS1_35eb1cc629dcecbace17b342029c287310c30d4e6d988740bf2c212a1251cee4",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"t_from_start":0,"x":0,"y":0,"yaw":0,"vx":0,"vy":0,"vyaw":0};},
validateValue(value){Ros2Unitree_goMsgPathPointSize(Ros2Unitree_goMsgPathPointCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgPathPointCapture(value),n=Ros2Unitree_goMsgPathPointSize(owned),w=new Writer(n);Ros2Unitree_goMsgPathPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgPathPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgPathPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["t_from_start"]!=="number"||!Number.isFinite(value["t_from_start"])||!Number.isFinite(Math.fround(value["t_from_start"])))bad();n=addSize(n,4);
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["yaw"]!=="number"||!Number.isFinite(value["yaw"])||!Number.isFinite(Math.fround(value["yaw"])))bad();n=addSize(n,4);
if(typeof value["vx"]!=="number"||!Number.isFinite(value["vx"])||!Number.isFinite(Math.fround(value["vx"])))bad();n=addSize(n,4);
if(typeof value["vy"]!=="number"||!Number.isFinite(value["vy"])||!Number.isFinite(Math.fround(value["vy"])))bad();n=addSize(n,4);
if(typeof value["vyaw"]!=="number"||!Number.isFinite(value["vyaw"])||!Number.isFinite(Math.fround(value["vyaw"])))bad();n=addSize(n,4);
if(n>28)bad(90);return n;}
function Ros2Unitree_goMsgPathPointWrite(value,w){
w.v.setFloat32(w.at,value["t_from_start"],true);w.at+=4;
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["yaw"],true);w.at+=4;
w.v.setFloat32(w.at,value["vx"],true);w.at+=4;
w.v.setFloat32(w.at,value["vy"],true);w.at+=4;
w.v.setFloat32(w.at,value["vyaw"],true);w.at+=4;
}
function Ros2Unitree_goMsgPathPointRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["t_from_start"]=decoded;
}
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
value["yaw"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["vx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["vy"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["vyaw"]=decoded;
}
return value;}
function Ros2Unitree_goMsgPathPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["t_from_start"];
out["t_from_start"]=field;
}
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["yaw"];
out["yaw"]=field;
}
{const field=value["vx"];
out["vx"]=field;
}
{const field=value["vy"];
out["vy"]=field;
}
{const field=value["vyaw"];
out["vyaw"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/Req codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgReqCodec=Object.freeze({codecIdentity:"UWVMROS1_43e0126993336d81a3f6a6706979c19f3a193f6a1ef504cadcd864be8bfaf84e",maxWireBytes:4232,minWireBytes:8,cartesianMaxWireBytes:4232n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","body":""};},
validateValue(value){Ros2Unitree_goMsgReqSize(Ros2Unitree_goMsgReqCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgReqCapture(value),n=Ros2Unitree_goMsgReqSize(owned),w=new Writer(n);Ros2Unitree_goMsgReqWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4232,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgReqRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgReqSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],128).length);
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>4232)bad(90);return n;}
function Ros2Unitree_goMsgReqWrite(value,w){
w.text(value["uuid"],128);
w.text(value["body"],4096);
}
function Ros2Unitree_goMsgReqRead(r){const value={};
{
const decoded=r.text(128);
value["uuid"]=decoded;
}
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Unitree_goMsgReqCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/Res codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgResCodec=Object.freeze({codecIdentity:"UWVMROS1_53c1d85c0cb17cfdd7d3799d6e92602a70700e8c55da4045f501a6a12ff55bfb",maxWireBytes:5260,minWireBytes:12,cartesianMaxWireBytes:5260n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"uuid":"","data":[],"body":""};},
validateValue(value){Ros2Unitree_goMsgResSize(Ros2Unitree_goMsgResCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgResCapture(value),n=Ros2Unitree_goMsgResSize(owned),w=new Writer(n);Ros2Unitree_goMsgResWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5260,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgResRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgResSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["uuid"],128).length);
if(!Array.isArray(value["data"])||value["data"].length>1024)bad();
n=addSize(n,4);
for(const element of value["data"]){
integer(element,1,false);n=addSize(n,1);
}
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>5260)bad(90);return n;}
function Ros2Unitree_goMsgResWrite(value,w){
w.text(value["uuid"],128);
w.v.setUint32(w.at,value["data"].length,true);w.at+=4;
for(const element of value["data"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.text(value["body"],4096);
}
function Ros2Unitree_goMsgResRead(r){const value={};
{
const decoded=r.text(128);
value["uuid"]=decoded;
}
{
{const count=r.u32();if(count>1024)bad();value["data"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["data"].push(decoded);
}}
}
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Unitree_goMsgResCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["uuid"];
out["uuid"]=field;
}
{const field=value["data"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>1024)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["data"]=values;
}
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/SportModeCmd codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgSportModeCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_cf90a065007b9b8cedde3f0b58aa79da3b39a1456dc1b5297f4fe799d92ff22c",maxWireBytes:887,minWireBytes:887,cartesianMaxWireBytes:887n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"gait_type":0,"speed_level":0,"foot_raise_height":0,"body_height":0,"position":Array.from({length:2},()=>(0)),"euler":Array.from({length:3},()=>(0)),"velocity":Array.from({length:2},()=>(0)),"yaw_speed":0,"bms_cmd":Ros2Unitree_goMsgBmsCmdCodec.defaultValue(),"path_point":Array.from({length:30},()=>(Ros2Unitree_goMsgPathPointCodec.defaultValue()))};},
validateValue(value){Ros2Unitree_goMsgSportModeCmdSize(Ros2Unitree_goMsgSportModeCmdCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgSportModeCmdCapture(value),n=Ros2Unitree_goMsgSportModeCmdSize(owned),w=new Writer(n);Ros2Unitree_goMsgSportModeCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,887,90);if(owned.length<887)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgSportModeCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgSportModeCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,false);n=addSize(n,1);
integer(value["gait_type"],1,false);n=addSize(n,1);
integer(value["speed_level"],1,false);n=addSize(n,1);
if(typeof value["foot_raise_height"]!=="number"||!Number.isFinite(value["foot_raise_height"])||!Number.isFinite(Math.fround(value["foot_raise_height"])))bad();n=addSize(n,4);
if(typeof value["body_height"]!=="number"||!Number.isFinite(value["body_height"])||!Number.isFinite(Math.fround(value["body_height"])))bad();n=addSize(n,4);
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
if(typeof value["yaw_speed"]!=="number"||!Number.isFinite(value["yaw_speed"])||!Number.isFinite(Math.fround(value["yaw_speed"])))bad();n=addSize(n,4);
n=addSize(n,Ros2Unitree_goMsgBmsCmdSize(value["bms_cmd"]));
if(!Array.isArray(value["path_point"])||value["path_point"].length!==30)bad();
for(const element of value["path_point"]){
n=addSize(n,Ros2Unitree_goMsgPathPointSize(element));
}
if(n>887)bad(90);return n;}
function Ros2Unitree_goMsgSportModeCmdWrite(value,w){
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setUint8(w.at,value["gait_type"],true);w.at+=1;
w.v.setUint8(w.at,value["speed_level"],true);w.at+=1;
w.v.setFloat32(w.at,value["foot_raise_height"],true);w.at+=4;
w.v.setFloat32(w.at,value["body_height"],true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["euler"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["velocity"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["yaw_speed"],true);w.at+=4;
Ros2Unitree_goMsgBmsCmdWrite(value["bms_cmd"],w);
for(const element of value["path_point"]){
Ros2Unitree_goMsgPathPointWrite(element,w);
}
}
function Ros2Unitree_goMsgSportModeCmdRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["mode"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["gait_type"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["speed_level"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["foot_raise_height"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["body_height"]=decoded;
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
value["yaw_speed"]=decoded;
}
{
const decoded=Ros2Unitree_goMsgBmsCmdRead(r);
value["bms_cmd"]=decoded;
}
{
{const count=30;if(count>30)bad();value["path_point"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Unitree_goMsgPathPointRead(r);
value["path_point"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgSportModeCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["gait_type"];
out["gait_type"]=field;
}
{const field=value["speed_level"];
out["speed_level"]=field;
}
{const field=value["foot_raise_height"];
out["foot_raise_height"]=field;
}
{const field=value["body_height"];
out["body_height"]=field;
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
{const field=value["yaw_speed"];
out["yaw_speed"]=field;
}
{const field=value["bms_cmd"];
out["bms_cmd"]=Ros2Unitree_goMsgBmsCmdCapture(field);
}
{const field=value["path_point"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==30)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Unitree_goMsgPathPointCapture(element);
}out["path_point"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/TimeSpec codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgTimeSpecCodec=Object.freeze({codecIdentity:"UWVMROS1_9ad3c6ed5a9ff0bba22593b50e1d777ae2f63480a01a744d777ee6b7dad0ecc4",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nanosec":0};},
validateValue(value){Ros2Unitree_goMsgTimeSpecSize(Ros2Unitree_goMsgTimeSpecCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgTimeSpecCapture(value),n=Ros2Unitree_goMsgTimeSpecSize(owned),w=new Writer(n);Ros2Unitree_goMsgTimeSpecWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgTimeSpecRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgTimeSpecSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nanosec"],4,false);n=addSize(n,4);
if(n>8)bad(90);return n;}
function Ros2Unitree_goMsgTimeSpecWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nanosec"],true);w.at+=4;
}
function Ros2Unitree_goMsgTimeSpecRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["nanosec"]=decoded;
}
return value;}
function Ros2Unitree_goMsgTimeSpecCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nanosec"];
out["nanosec"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/SportModeState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgSportModeStateCodec=Object.freeze({codecIdentity:"UWVMROS1_2935b9d838087267840b379582862a15c2e1a72fe8ad2bcbf8f19af2f8054760",maxWireBytes:227,minWireBytes:227,cartesianMaxWireBytes:227n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"stamp":Ros2Unitree_goMsgTimeSpecCodec.defaultValue(),"error_code":0,"imu_state":Ros2Unitree_goMsgIMUStateCodec.defaultValue(),"mode":0,"progress":0,"gait_type":0,"foot_raise_height":0,"position":Array.from({length:3},()=>(0)),"body_height":0,"velocity":Array.from({length:3},()=>(0)),"yaw_speed":0,"range_obstacle":Array.from({length:4},()=>(0)),"foot_force":Array.from({length:4},()=>(0)),"foot_position_body":Array.from({length:12},()=>(0)),"foot_speed_body":Array.from({length:12},()=>(0))};},
validateValue(value){Ros2Unitree_goMsgSportModeStateSize(Ros2Unitree_goMsgSportModeStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgSportModeStateCapture(value),n=Ros2Unitree_goMsgSportModeStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgSportModeStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,227,90);if(owned.length<227)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgSportModeStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgSportModeStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Unitree_goMsgTimeSpecSize(value["stamp"]));
integer(value["error_code"],4,false);n=addSize(n,4);
n=addSize(n,Ros2Unitree_goMsgIMUStateSize(value["imu_state"]));
integer(value["mode"],1,false);n=addSize(n,1);
if(typeof value["progress"]!=="number"||!Number.isFinite(value["progress"])||!Number.isFinite(Math.fround(value["progress"])))bad();n=addSize(n,4);
integer(value["gait_type"],1,false);n=addSize(n,1);
if(typeof value["foot_raise_height"]!=="number"||!Number.isFinite(value["foot_raise_height"])||!Number.isFinite(Math.fround(value["foot_raise_height"])))bad();n=addSize(n,4);
if(!Array.isArray(value["position"])||value["position"].length!==3)bad();
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["body_height"]!=="number"||!Number.isFinite(value["body_height"])||!Number.isFinite(Math.fround(value["body_height"])))bad();n=addSize(n,4);
if(!Array.isArray(value["velocity"])||value["velocity"].length!==3)bad();
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(typeof value["yaw_speed"]!=="number"||!Number.isFinite(value["yaw_speed"])||!Number.isFinite(Math.fround(value["yaw_speed"])))bad();n=addSize(n,4);
if(!Array.isArray(value["range_obstacle"])||value["range_obstacle"].length!==4)bad();
for(const element of value["range_obstacle"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["foot_force"])||value["foot_force"].length!==4)bad();
for(const element of value["foot_force"]){
integer(element,2,true);n=addSize(n,2);
}
if(!Array.isArray(value["foot_position_body"])||value["foot_position_body"].length!==12)bad();
for(const element of value["foot_position_body"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(!Array.isArray(value["foot_speed_body"])||value["foot_speed_body"].length!==12)bad();
for(const element of value["foot_speed_body"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
if(n>227)bad(90);return n;}
function Ros2Unitree_goMsgSportModeStateWrite(value,w){
Ros2Unitree_goMsgTimeSpecWrite(value["stamp"],w);
w.v.setUint32(w.at,value["error_code"],true);w.at+=4;
Ros2Unitree_goMsgIMUStateWrite(value["imu_state"],w);
w.v.setUint8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["progress"],true);w.at+=4;
w.v.setUint8(w.at,value["gait_type"],true);w.at+=1;
w.v.setFloat32(w.at,value["foot_raise_height"],true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["body_height"],true);w.at+=4;
for(const element of value["velocity"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setFloat32(w.at,value["yaw_speed"],true);w.at+=4;
for(const element of value["range_obstacle"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["foot_force"]){
w.v.setInt16(w.at,element,true);w.at+=2;
}
for(const element of value["foot_position_body"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
for(const element of value["foot_speed_body"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
}
function Ros2Unitree_goMsgSportModeStateRead(r){const value={};
{
const decoded=Ros2Unitree_goMsgTimeSpecRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=Ros2Unitree_goMsgIMUStateRead(r);
value["imu_state"]=decoded;
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
value["gait_type"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["foot_raise_height"]=decoded;
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
value["body_height"]=decoded;
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
value["yaw_speed"]=decoded;
}
{
{const count=4;if(count>4)bad();value["range_obstacle"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["range_obstacle"].push(decoded);
}}
}
{
{const count=4;if(count>4)bad();value["foot_force"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt16(r.take(2),true);
value["foot_force"].push(decoded);
}}
}
{
{const count=12;if(count>12)bad();value["foot_position_body"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["foot_position_body"].push(decoded);
}}
}
{
{const count=12;if(count>12)bad();value["foot_speed_body"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["foot_speed_body"].push(decoded);
}}
}
return value;}
function Ros2Unitree_goMsgSportModeStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["stamp"];
out["stamp"]=Ros2Unitree_goMsgTimeSpecCapture(field);
}
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["imu_state"];
out["imu_state"]=Ros2Unitree_goMsgIMUStateCapture(field);
}
{const field=value["mode"];
out["mode"]=field;
}
{const field=value["progress"];
out["progress"]=field;
}
{const field=value["gait_type"];
out["gait_type"]=field;
}
{const field=value["foot_raise_height"];
out["foot_raise_height"]=field;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["body_height"];
out["body_height"]=field;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==3)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["yaw_speed"];
out["yaw_speed"]=field;
}
{const field=value["range_obstacle"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["range_obstacle"]=values;
}
{const field=value["foot_force"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==4)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["foot_force"]=values;
}
{const field=value["foot_position_body"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["foot_position_body"]=values;
}
{const field=value["foot_speed_body"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["foot_speed_body"]=values;
}
return out;}
/** Complete ROS2 unitree_go/msg/UwbState codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgUwbStateCodec=Object.freeze({codecIdentity:"UWVMROS1_017fbafd61a006aeaf6cb80456ac89f429d30a562da2e0d394c6cfb406e61a34",maxWireBytes:55,minWireBytes:55,cartesianMaxWireBytes:55n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"version":Array.from({length:2},()=>(0)),"channel":0,"joy_mode":0,"orientation_est":0,"pitch_est":0,"distance_est":0,"yaw_est":0,"tag_roll":0,"tag_pitch":0,"tag_yaw":0,"base_roll":0,"base_pitch":0,"base_yaw":0,"joystick":Array.from({length:2},()=>(0)),"error_state":0,"buttons":0,"enabled_from_app":0};},
validateValue(value){Ros2Unitree_goMsgUwbStateSize(Ros2Unitree_goMsgUwbStateCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgUwbStateCapture(value),n=Ros2Unitree_goMsgUwbStateSize(owned),w=new Writer(n);Ros2Unitree_goMsgUwbStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,55,90);if(owned.length<55)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgUwbStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgUwbStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["version"])||value["version"].length!==2)bad();
for(const element of value["version"]){
integer(element,1,false);n=addSize(n,1);
}
integer(value["channel"],1,false);n=addSize(n,1);
integer(value["joy_mode"],1,false);n=addSize(n,1);
if(typeof value["orientation_est"]!=="number"||!Number.isFinite(value["orientation_est"])||!Number.isFinite(Math.fround(value["orientation_est"])))bad();n=addSize(n,4);
if(typeof value["pitch_est"]!=="number"||!Number.isFinite(value["pitch_est"])||!Number.isFinite(Math.fround(value["pitch_est"])))bad();n=addSize(n,4);
if(typeof value["distance_est"]!=="number"||!Number.isFinite(value["distance_est"])||!Number.isFinite(Math.fround(value["distance_est"])))bad();n=addSize(n,4);
if(typeof value["yaw_est"]!=="number"||!Number.isFinite(value["yaw_est"])||!Number.isFinite(Math.fround(value["yaw_est"])))bad();n=addSize(n,4);
if(typeof value["tag_roll"]!=="number"||!Number.isFinite(value["tag_roll"])||!Number.isFinite(Math.fround(value["tag_roll"])))bad();n=addSize(n,4);
if(typeof value["tag_pitch"]!=="number"||!Number.isFinite(value["tag_pitch"])||!Number.isFinite(Math.fround(value["tag_pitch"])))bad();n=addSize(n,4);
if(typeof value["tag_yaw"]!=="number"||!Number.isFinite(value["tag_yaw"])||!Number.isFinite(Math.fround(value["tag_yaw"])))bad();n=addSize(n,4);
if(typeof value["base_roll"]!=="number"||!Number.isFinite(value["base_roll"])||!Number.isFinite(Math.fround(value["base_roll"])))bad();n=addSize(n,4);
if(typeof value["base_pitch"]!=="number"||!Number.isFinite(value["base_pitch"])||!Number.isFinite(Math.fround(value["base_pitch"])))bad();n=addSize(n,4);
if(typeof value["base_yaw"]!=="number"||!Number.isFinite(value["base_yaw"])||!Number.isFinite(Math.fround(value["base_yaw"])))bad();n=addSize(n,4);
if(!Array.isArray(value["joystick"])||value["joystick"].length!==2)bad();
for(const element of value["joystick"]){
if(typeof element!=="number"||!Number.isFinite(element)||!Number.isFinite(Math.fround(element)))bad();n=addSize(n,4);
}
integer(value["error_state"],1,false);n=addSize(n,1);
integer(value["buttons"],1,false);n=addSize(n,1);
integer(value["enabled_from_app"],1,false);n=addSize(n,1);
if(n>55)bad(90);return n;}
function Ros2Unitree_goMsgUwbStateWrite(value,w){
for(const element of value["version"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
w.v.setUint8(w.at,value["channel"],true);w.at+=1;
w.v.setUint8(w.at,value["joy_mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["orientation_est"],true);w.at+=4;
w.v.setFloat32(w.at,value["pitch_est"],true);w.at+=4;
w.v.setFloat32(w.at,value["distance_est"],true);w.at+=4;
w.v.setFloat32(w.at,value["yaw_est"],true);w.at+=4;
w.v.setFloat32(w.at,value["tag_roll"],true);w.at+=4;
w.v.setFloat32(w.at,value["tag_pitch"],true);w.at+=4;
w.v.setFloat32(w.at,value["tag_yaw"],true);w.at+=4;
w.v.setFloat32(w.at,value["base_roll"],true);w.at+=4;
w.v.setFloat32(w.at,value["base_pitch"],true);w.at+=4;
w.v.setFloat32(w.at,value["base_yaw"],true);w.at+=4;
for(const element of value["joystick"]){
w.v.setFloat32(w.at,element,true);w.at+=4;
}
w.v.setUint8(w.at,value["error_state"],true);w.at+=1;
w.v.setUint8(w.at,value["buttons"],true);w.at+=1;
w.v.setUint8(w.at,value["enabled_from_app"],true);w.at+=1;
}
function Ros2Unitree_goMsgUwbStateRead(r){const value={};
{
{const count=2;if(count>2)bad();value["version"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["version"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["channel"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["joy_mode"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["orientation_est"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pitch_est"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["distance_est"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["yaw_est"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tag_roll"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tag_pitch"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["tag_yaw"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["base_roll"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["base_pitch"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["base_yaw"]=decoded;
}
{
{const count=2;if(count>2)bad();value["joystick"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["joystick"].push(decoded);
}}
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["error_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["buttons"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["enabled_from_app"]=decoded;
}
return value;}
function Ros2Unitree_goMsgUwbStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["version"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["version"]=values;
}
{const field=value["channel"];
out["channel"]=field;
}
{const field=value["joy_mode"];
out["joy_mode"]=field;
}
{const field=value["orientation_est"];
out["orientation_est"]=field;
}
{const field=value["pitch_est"];
out["pitch_est"]=field;
}
{const field=value["distance_est"];
out["distance_est"]=field;
}
{const field=value["yaw_est"];
out["yaw_est"]=field;
}
{const field=value["tag_roll"];
out["tag_roll"]=field;
}
{const field=value["tag_pitch"];
out["tag_pitch"]=field;
}
{const field=value["tag_yaw"];
out["tag_yaw"]=field;
}
{const field=value["base_roll"];
out["base_roll"]=field;
}
{const field=value["base_pitch"];
out["base_pitch"]=field;
}
{const field=value["base_yaw"];
out["base_yaw"]=field;
}
{const field=value["joystick"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count!==2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joystick"]=values;
}
{const field=value["error_state"];
out["error_state"]=field;
}
{const field=value["buttons"];
out["buttons"]=field;
}
{const field=value["enabled_from_app"];
out["enabled_from_app"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/UwbSwitch codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgUwbSwitchCodec=Object.freeze({codecIdentity:"UWVMROS1_4546618797829b3911d1533d8d6bacc9da969d40d7752424de34bbd3de4bede5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"enabled":0};},
validateValue(value){Ros2Unitree_goMsgUwbSwitchSize(Ros2Unitree_goMsgUwbSwitchCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgUwbSwitchCapture(value),n=Ros2Unitree_goMsgUwbSwitchSize(owned),w=new Writer(n);Ros2Unitree_goMsgUwbSwitchWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgUwbSwitchRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgUwbSwitchSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["enabled"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros2Unitree_goMsgUwbSwitchWrite(value,w){
w.v.setUint8(w.at,value["enabled"],true);w.at+=1;
}
function Ros2Unitree_goMsgUwbSwitchRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["enabled"]=decoded;
}
return value;}
function Ros2Unitree_goMsgUwbSwitchCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["enabled"];
out["enabled"]=field;
}
return out;}
/** Complete ROS2 unitree_go/msg/WirelessController codec; maxima are not actual lengths. */
export const Ros2Unitree_goMsgWirelessControllerCodec=Object.freeze({codecIdentity:"UWVMROS1_536e69ce5459227cb68b3c6d484a96bfa40e1400aab820f8ad0d0d62ff6ba438",maxWireBytes:18,minWireBytes:18,cartesianMaxWireBytes:18n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"lx":0,"ly":0,"rx":0,"ry":0,"keys":0};},
validateValue(value){Ros2Unitree_goMsgWirelessControllerSize(Ros2Unitree_goMsgWirelessControllerCapture(value));return true;},
encode(value){const owned=Ros2Unitree_goMsgWirelessControllerCapture(value),n=Ros2Unitree_goMsgWirelessControllerSize(owned),w=new Writer(n);Ros2Unitree_goMsgWirelessControllerWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,18,90);if(owned.length<18)bad();const r=new Reader(owned),value=Ros2Unitree_goMsgWirelessControllerRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_goMsgWirelessControllerSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["lx"]!=="number"||!Number.isFinite(value["lx"])||!Number.isFinite(Math.fround(value["lx"])))bad();n=addSize(n,4);
if(typeof value["ly"]!=="number"||!Number.isFinite(value["ly"])||!Number.isFinite(Math.fround(value["ly"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
integer(value["keys"],2,false);n=addSize(n,2);
if(n>18)bad(90);return n;}
function Ros2Unitree_goMsgWirelessControllerWrite(value,w){
w.v.setFloat32(w.at,value["lx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ly"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setUint16(w.at,value["keys"],true);w.at+=2;
}
function Ros2Unitree_goMsgWirelessControllerRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["lx"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ly"]=decoded;
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
const decoded=r.v.getUint16(r.take(2),true);
value["keys"]=decoded;
}
return value;}
function Ros2Unitree_goMsgWirelessControllerCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["lx"];
out["lx"]=field;
}
{const field=value["ly"];
out["ly"]=field;
}
{const field=value["rx"];
out["rx"]=field;
}
{const field=value["ry"];
out["ry"]=field;
}
{const field=value["keys"];
out["keys"]=field;
}
return out;}
