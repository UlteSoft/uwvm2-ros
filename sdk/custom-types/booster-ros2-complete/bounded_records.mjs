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
/** Complete ROS2 booster_interface/msg/BoosterApiReqMsg codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgBoosterApiReqMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40",maxWireBytes:4108,minWireBytes:12,cartesianMaxWireBytes:4108n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"api_id":0n,"body":""};},
validateValue(value){Ros2Booster_interfaceMsgBoosterApiReqMsgSize(Ros2Booster_interfaceMsgBoosterApiReqMsgCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgBoosterApiReqMsgCapture(value),n=Ros2Booster_interfaceMsgBoosterApiReqMsgSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgBoosterApiReqMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4108,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgBoosterApiReqMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgBoosterApiReqMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["api_id"],8,true);n=addSize(n,8);
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>4108)bad(90);return n;}
function Ros2Booster_interfaceMsgBoosterApiReqMsgWrite(value,w){
w.v.setBigInt64(w.at,value["api_id"],true);w.at+=8;
w.text(value["body"],4096);
}
function Ros2Booster_interfaceMsgBoosterApiReqMsgRead(r){const value={};
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["api_id"]=decoded;
}
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgBoosterApiReqMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["api_id"];
out["api_id"]=field;
}
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/BoosterApiRespMsg codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgBoosterApiRespMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f",maxWireBytes:4108,minWireBytes:12,cartesianMaxWireBytes:4108n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"status":0n,"body":""};},
validateValue(value){Ros2Booster_interfaceMsgBoosterApiRespMsgSize(Ros2Booster_interfaceMsgBoosterApiRespMsgCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgBoosterApiRespMsgCapture(value),n=Ros2Booster_interfaceMsgBoosterApiRespMsgSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgBoosterApiRespMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4108,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgBoosterApiRespMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgBoosterApiRespMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["status"],8,true);n=addSize(n,8);
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>4108)bad(90);return n;}
function Ros2Booster_interfaceMsgBoosterApiRespMsgWrite(value,w){
w.v.setBigInt64(w.at,value["status"],true);w.at+=8;
w.text(value["body"],4096);
}
function Ros2Booster_interfaceMsgBoosterApiRespMsgRead(r){const value={};
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["status"]=decoded;
}
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgBoosterApiRespMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["status"];
out["status"]=field;
}
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/ButtonEventMsg codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgButtonEventMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({"PRESS_DOWN":0,"PRESS_UP":1,"SINGLE_CLICK":0,"DOUBLE_CLICK":1,"TRIPLE_CLICK":0,"LONG_PRESS_START":1,"LONG_PRESS_HOLD":0,"LONG_PRESS_END":1}),
defaultValue(){return {"event":0,"button":0};},
validateValue(value){Ros2Booster_interfaceMsgButtonEventMsgSize(Ros2Booster_interfaceMsgButtonEventMsgCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgButtonEventMsgCapture(value),n=Ros2Booster_interfaceMsgButtonEventMsgSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgButtonEventMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgButtonEventMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgButtonEventMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["event"],1,true);n=addSize(n,1);
integer(value["button"],4,true);n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros2Booster_interfaceMsgButtonEventMsgWrite(value,w){
w.v.setInt8(w.at,value["event"],true);w.at+=1;
w.v.setInt32(w.at,value["button"],true);w.at+=4;
}
function Ros2Booster_interfaceMsgButtonEventMsgRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["event"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["button"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgButtonEventMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=field;
}
{const field=value["button"];
out["button"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/FallDownState codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgFallDownStateCodec=Object.freeze({codecIdentity:"UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({"IS_READY":0,"IS_FALLING":1,"HAS_FALLEN":2,"IS_GETTING_UP":3}),
defaultValue(){return {"fall_down_state":0,"is_recovery_available":false};},
validateValue(value){Ros2Booster_interfaceMsgFallDownStateSize(Ros2Booster_interfaceMsgFallDownStateCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgFallDownStateCapture(value),n=Ros2Booster_interfaceMsgFallDownStateSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgFallDownStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgFallDownStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgFallDownStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["fall_down_state"],4,false);n=addSize(n,4);
if(typeof value["is_recovery_available"]!=="boolean")bad();n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Booster_interfaceMsgFallDownStateWrite(value,w){
w.v.setUint32(w.at,value["fall_down_state"],true);w.at+=4;
w.v.setUint8(w.at,value["is_recovery_available"]?1:0,true);w.at+=1;
}
function Ros2Booster_interfaceMsgFallDownStateRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["fall_down_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["is_recovery_available"]=decoded!==0;
}
return value;}
function Ros2Booster_interfaceMsgFallDownStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["fall_down_state"];
out["fall_down_state"]=field;
}
{const field=value["is_recovery_available"];
out["is_recovery_available"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/HandParam codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgHandParamCodec=Object.freeze({codecIdentity:"UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"angle":0,"force":0,"speed":0,"seq":0};},
validateValue(value){Ros2Booster_interfaceMsgHandParamSize(Ros2Booster_interfaceMsgHandParamCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgHandParamCapture(value),n=Ros2Booster_interfaceMsgHandParamSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgHandParamWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgHandParamRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgHandParamSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["angle"],4,true);n=addSize(n,4);
integer(value["force"],4,true);n=addSize(n,4);
integer(value["speed"],4,true);n=addSize(n,4);
integer(value["seq"],4,true);n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros2Booster_interfaceMsgHandParamWrite(value,w){
w.v.setInt32(w.at,value["angle"],true);w.at+=4;
w.v.setInt32(w.at,value["force"],true);w.at+=4;
w.v.setInt32(w.at,value["speed"],true);w.at+=4;
w.v.setInt32(w.at,value["seq"],true);w.at+=4;
}
function Ros2Booster_interfaceMsgHandParamRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["angle"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["force"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["speed"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["seq"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgHandParamCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["angle"];
out["angle"]=field;
}
{const field=value["force"];
out["force"]=field;
}
{const field=value["speed"];
out["speed"]=field;
}
{const field=value["seq"];
out["seq"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/HandCommand codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgHandCommandCodec=Object.freeze({codecIdentity:"UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0",maxWireBytes:528,minWireBytes:16,cartesianMaxWireBytes:528n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"hand_param":[],"force_mode":0,"hand_index":0,"hand_type":0};},
validateValue(value){Ros2Booster_interfaceMsgHandCommandSize(Ros2Booster_interfaceMsgHandCommandCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgHandCommandCapture(value),n=Ros2Booster_interfaceMsgHandCommandSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgHandCommandWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,528,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgHandCommandRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgHandCommandSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["hand_param"])||value["hand_param"].length>32)bad();
n=addSize(n,4);
for(const element of value["hand_param"]){
n=addSize(n,Ros2Booster_interfaceMsgHandParamSize(element));
}
integer(value["force_mode"],4,true);n=addSize(n,4);
integer(value["hand_index"],4,true);n=addSize(n,4);
integer(value["hand_type"],4,true);n=addSize(n,4);
if(n>528)bad(90);return n;}
function Ros2Booster_interfaceMsgHandCommandWrite(value,w){
w.v.setUint32(w.at,value["hand_param"].length,true);w.at+=4;
for(const element of value["hand_param"]){
Ros2Booster_interfaceMsgHandParamWrite(element,w);
}
w.v.setInt32(w.at,value["force_mode"],true);w.at+=4;
w.v.setInt32(w.at,value["hand_index"],true);w.at+=4;
w.v.setInt32(w.at,value["hand_type"],true);w.at+=4;
}
function Ros2Booster_interfaceMsgHandCommandRead(r){const value={};
{
{const count=r.u32();if(count>32)bad();value["hand_param"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgHandParamRead(r);
value["hand_param"].push(decoded);
}}
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["force_mode"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["hand_index"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["hand_type"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgHandCommandCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["hand_param"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>32)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Booster_interfaceMsgHandParamCapture(element);
}out["hand_param"]=values;
}
{const field=value["force_mode"];
out["force_mode"]=field;
}
{const field=value["hand_index"];
out["hand_index"]=field;
}
{const field=value["hand_type"];
out["hand_type"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/HandDdsMsg codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgHandDdsMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397",maxWireBytes:1060,minWireBytes:4,cartesianMaxWireBytes:1060n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"hands_vec":[]};},
validateValue(value){Ros2Booster_interfaceMsgHandDdsMsgSize(Ros2Booster_interfaceMsgHandDdsMsgCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgHandDdsMsgCapture(value),n=Ros2Booster_interfaceMsgHandDdsMsgSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgHandDdsMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1060,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgHandDdsMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgHandDdsMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["hands_vec"])||value["hands_vec"].length>2)bad();
n=addSize(n,4);
for(const element of value["hands_vec"]){
n=addSize(n,Ros2Booster_interfaceMsgHandCommandSize(element));
}
if(n>1060)bad(90);return n;}
function Ros2Booster_interfaceMsgHandDdsMsgWrite(value,w){
w.v.setUint32(w.at,value["hands_vec"].length,true);w.at+=4;
for(const element of value["hands_vec"]){
Ros2Booster_interfaceMsgHandCommandWrite(element,w);
}
}
function Ros2Booster_interfaceMsgHandDdsMsgRead(r){const value={};
{
{const count=r.u32();if(count>2)bad();value["hands_vec"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgHandCommandRead(r);
value["hands_vec"].push(decoded);
}}
}
return value;}
function Ros2Booster_interfaceMsgHandDdsMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["hands_vec"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Booster_interfaceMsgHandCommandCapture(element);
}out["hands_vec"]=values;
}
return out;}
/** Complete ROS2 booster_interface/msg/ImuState codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgImuStateCodec=Object.freeze({codecIdentity:"UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092",maxWireBytes:36,minWireBytes:36,cartesianMaxWireBytes:36n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"rpy":Array.from({length:3},()=>(0)),"gyro":Array.from({length:3},()=>(0)),"acc":Array.from({length:3},()=>(0))};},
validateValue(value){Ros2Booster_interfaceMsgImuStateSize(Ros2Booster_interfaceMsgImuStateCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgImuStateCapture(value),n=Ros2Booster_interfaceMsgImuStateSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgImuStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,36,90);if(owned.length<36)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgImuStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgImuStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
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
function Ros2Booster_interfaceMsgImuStateWrite(value,w){
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
function Ros2Booster_interfaceMsgImuStateRead(r){const value={};
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
function Ros2Booster_interfaceMsgImuStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 booster_interface/msg/MotorCmd codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgMotorCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392",maxWireBytes:25,minWireBytes:25,cartesianMaxWireBytes:25n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"tau":0,"kp":0,"kd":0,"weight":0};},
validateValue(value){Ros2Booster_interfaceMsgMotorCmdSize(Ros2Booster_interfaceMsgMotorCmdCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgMotorCmdCapture(value),n=Ros2Booster_interfaceMsgMotorCmdSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgMotorCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,25,90);if(owned.length<25)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgMotorCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgMotorCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["tau"]!=="number"||!Number.isFinite(value["tau"])||!Number.isFinite(Math.fround(value["tau"])))bad();n=addSize(n,4);
if(typeof value["kp"]!=="number"||!Number.isFinite(value["kp"])||!Number.isFinite(Math.fround(value["kp"])))bad();n=addSize(n,4);
if(typeof value["kd"]!=="number"||!Number.isFinite(value["kd"])||!Number.isFinite(Math.fround(value["kd"])))bad();n=addSize(n,4);
if(typeof value["weight"]!=="number"||!Number.isFinite(value["weight"])||!Number.isFinite(Math.fround(value["weight"])))bad();n=addSize(n,4);
if(n>25)bad(90);return n;}
function Ros2Booster_interfaceMsgMotorCmdWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau"],true);w.at+=4;
w.v.setFloat32(w.at,value["kp"],true);w.at+=4;
w.v.setFloat32(w.at,value["kd"],true);w.at+=4;
w.v.setFloat32(w.at,value["weight"],true);w.at+=4;
}
function Ros2Booster_interfaceMsgMotorCmdRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
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
function Ros2Booster_interfaceMsgMotorCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 booster_interface/msg/LowCmd codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgLowCmdCodec=Object.freeze({codecIdentity:"UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870",maxWireBytes:730,minWireBytes:5,cartesianMaxWireBytes:730n,aggregateWireBytes:0,constants:Object.freeze({"CMD_TYPE_PARALLEL":0,"CMD_TYPE_SERIAL":1}),
defaultValue(){return {"cmd_type":0,"motor_cmd":[]};},
validateValue(value){Ros2Booster_interfaceMsgLowCmdSize(Ros2Booster_interfaceMsgLowCmdCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgLowCmdCapture(value),n=Ros2Booster_interfaceMsgLowCmdSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgLowCmdWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,730,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgLowCmdRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgLowCmdSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["cmd_type"],1,true);n=addSize(n,1);
if(!Array.isArray(value["motor_cmd"])||value["motor_cmd"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_cmd"]){
n=addSize(n,Ros2Booster_interfaceMsgMotorCmdSize(element));
}
if(n>730)bad(90);return n;}
function Ros2Booster_interfaceMsgLowCmdWrite(value,w){
w.v.setInt8(w.at,value["cmd_type"],true);w.at+=1;
w.v.setUint32(w.at,value["motor_cmd"].length,true);w.at+=4;
for(const element of value["motor_cmd"]){
Ros2Booster_interfaceMsgMotorCmdWrite(element,w);
}
}
function Ros2Booster_interfaceMsgLowCmdRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
value["cmd_type"]=decoded;
}
{
{const count=r.u32();if(count>29)bad();value["motor_cmd"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorCmdRead(r);
value["motor_cmd"].push(decoded);
}}
}
return value;}
function Ros2Booster_interfaceMsgLowCmdCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cmd_type"];
out["cmd_type"]=field;
}
{const field=value["motor_cmd"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Booster_interfaceMsgMotorCmdCapture(element);
}out["motor_cmd"]=values;
}
return out;}
/** Complete ROS2 booster_interface/msg/MotorState codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgMotorStateCodec=Object.freeze({codecIdentity:"UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b",maxWireBytes:30,minWireBytes:30,cartesianMaxWireBytes:30n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mode":0,"q":0,"dq":0,"ddq":0,"tau_est":0,"temperature":0,"lost":0,"reserve":Array.from({length:2},()=>(0))};},
validateValue(value){Ros2Booster_interfaceMsgMotorStateSize(Ros2Booster_interfaceMsgMotorStateCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgMotorStateCapture(value),n=Ros2Booster_interfaceMsgMotorStateSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgMotorStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,30,90);if(owned.length<30)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgMotorStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgMotorStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["mode"],1,true);n=addSize(n,1);
if(typeof value["q"]!=="number"||!Number.isFinite(value["q"])||!Number.isFinite(Math.fround(value["q"])))bad();n=addSize(n,4);
if(typeof value["dq"]!=="number"||!Number.isFinite(value["dq"])||!Number.isFinite(Math.fround(value["dq"])))bad();n=addSize(n,4);
if(typeof value["ddq"]!=="number"||!Number.isFinite(value["ddq"])||!Number.isFinite(Math.fround(value["ddq"])))bad();n=addSize(n,4);
if(typeof value["tau_est"]!=="number"||!Number.isFinite(value["tau_est"])||!Number.isFinite(Math.fround(value["tau_est"])))bad();n=addSize(n,4);
integer(value["temperature"],1,true);n=addSize(n,1);
integer(value["lost"],4,false);n=addSize(n,4);
if(!Array.isArray(value["reserve"])||value["reserve"].length!==2)bad();
for(const element of value["reserve"]){
integer(element,4,false);n=addSize(n,4);
}
if(n>30)bad(90);return n;}
function Ros2Booster_interfaceMsgMotorStateWrite(value,w){
w.v.setInt8(w.at,value["mode"],true);w.at+=1;
w.v.setFloat32(w.at,value["q"],true);w.at+=4;
w.v.setFloat32(w.at,value["dq"],true);w.at+=4;
w.v.setFloat32(w.at,value["ddq"],true);w.at+=4;
w.v.setFloat32(w.at,value["tau_est"],true);w.at+=4;
w.v.setInt8(w.at,value["temperature"],true);w.at+=1;
w.v.setUint32(w.at,value["lost"],true);w.at+=4;
for(const element of value["reserve"]){
w.v.setUint32(w.at,element,true);w.at+=4;
}
}
function Ros2Booster_interfaceMsgMotorStateRead(r){const value={};
{
const decoded=r.v.getInt8(r.take(1),true);
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
function Ros2Booster_interfaceMsgMotorStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS2 booster_interface/msg/LowState codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgLowStateCodec=Object.freeze({codecIdentity:"UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f",maxWireBytes:1784,minWireBytes:44,cartesianMaxWireBytes:1784n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"imu_state":Ros2Booster_interfaceMsgImuStateCodec.defaultValue(),"motor_state_parallel":[],"motor_state_serial":[]};},
validateValue(value){Ros2Booster_interfaceMsgLowStateSize(Ros2Booster_interfaceMsgLowStateCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgLowStateCapture(value),n=Ros2Booster_interfaceMsgLowStateSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgLowStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1784,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgLowStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgLowStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Booster_interfaceMsgImuStateSize(value["imu_state"]));
if(!Array.isArray(value["motor_state_parallel"])||value["motor_state_parallel"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_state_parallel"]){
n=addSize(n,Ros2Booster_interfaceMsgMotorStateSize(element));
}
if(!Array.isArray(value["motor_state_serial"])||value["motor_state_serial"].length>29)bad();
n=addSize(n,4);
for(const element of value["motor_state_serial"]){
n=addSize(n,Ros2Booster_interfaceMsgMotorStateSize(element));
}
if(n>1784)bad(90);return n;}
function Ros2Booster_interfaceMsgLowStateWrite(value,w){
Ros2Booster_interfaceMsgImuStateWrite(value["imu_state"],w);
w.v.setUint32(w.at,value["motor_state_parallel"].length,true);w.at+=4;
for(const element of value["motor_state_parallel"]){
Ros2Booster_interfaceMsgMotorStateWrite(element,w);
}
w.v.setUint32(w.at,value["motor_state_serial"].length,true);w.at+=4;
for(const element of value["motor_state_serial"]){
Ros2Booster_interfaceMsgMotorStateWrite(element,w);
}
}
function Ros2Booster_interfaceMsgLowStateRead(r){const value={};
{
const decoded=Ros2Booster_interfaceMsgImuStateRead(r);
value["imu_state"]=decoded;
}
{
{const count=r.u32();if(count>29)bad();value["motor_state_parallel"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorStateRead(r);
value["motor_state_parallel"].push(decoded);
}}
}
{
{const count=r.u32();if(count>29)bad();value["motor_state_serial"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorStateRead(r);
value["motor_state_serial"].push(decoded);
}}
}
return value;}
function Ros2Booster_interfaceMsgLowStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["imu_state"];
out["imu_state"]=Ros2Booster_interfaceMsgImuStateCapture(field);
}
{const field=value["motor_state_parallel"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Booster_interfaceMsgMotorStateCapture(element);
}out["motor_state_parallel"]=values;
}
{const field=value["motor_state_serial"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>29)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros2Booster_interfaceMsgMotorStateCapture(element);
}out["motor_state_serial"]=values;
}
return out;}
/** Complete ROS2 booster_interface/msg/Odometer codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgOdometerCodec=Object.freeze({codecIdentity:"UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"theta":0};},
validateValue(value){Ros2Booster_interfaceMsgOdometerSize(Ros2Booster_interfaceMsgOdometerCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgOdometerCapture(value),n=Ros2Booster_interfaceMsgOdometerSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgOdometerWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgOdometerRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgOdometerSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["theta"]!=="number"||!Number.isFinite(value["theta"])||!Number.isFinite(Math.fround(value["theta"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros2Booster_interfaceMsgOdometerWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["theta"],true);w.at+=4;
}
function Ros2Booster_interfaceMsgOdometerRead(r){const value={};
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
value["theta"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgOdometerCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["theta"];
out["theta"]=field;
}
return out;}
/** Complete ROS2 booster_interface/msg/RawBytesMsg codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgRawBytesMsgCodec=Object.freeze({codecIdentity:"UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c",maxWireBytes:2052,minWireBytes:4,cartesianMaxWireBytes:2052n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":[]};},
validateValue(value){Ros2Booster_interfaceMsgRawBytesMsgSize(Ros2Booster_interfaceMsgRawBytesMsgCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgRawBytesMsgCapture(value),n=Ros2Booster_interfaceMsgRawBytesMsgSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgRawBytesMsgWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2052,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgRawBytesMsgRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgRawBytesMsgSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["msg"])||value["msg"].length>2048)bad();
n=addSize(n,4);
for(const element of value["msg"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>2052)bad(90);return n;}
function Ros2Booster_interfaceMsgRawBytesMsgWrite(value,w){
w.v.setUint32(w.at,value["msg"].length,true);w.at+=4;
for(const element of value["msg"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Booster_interfaceMsgRawBytesMsgRead(r){const value={};
{
{const count=r.u32();if(count>2048)bad();value["msg"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["msg"].push(decoded);
}}
}
return value;}
function Ros2Booster_interfaceMsgRawBytesMsgCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["msg"]=values;
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
/** Complete ROS2 booster_interface/msg/RawBytesStamped codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgRawBytesStampedCodec=Object.freeze({codecIdentity:"UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e",maxWireBytes:2192,minWireBytes:16,cartesianMaxWireBytes:2192n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Std_msgsMsgHeaderCodec.defaultValue(),"msg":[]};},
validateValue(value){Ros2Booster_interfaceMsgRawBytesStampedSize(Ros2Booster_interfaceMsgRawBytesStampedCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgRawBytesStampedCapture(value),n=Ros2Booster_interfaceMsgRawBytesStampedSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgRawBytesStampedWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2192,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgRawBytesStampedRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgRawBytesStampedSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Std_msgsMsgHeaderSize(value["header"]));
if(!Array.isArray(value["msg"])||value["msg"].length>2048)bad();
n=addSize(n,4);
for(const element of value["msg"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>2192)bad(90);return n;}
function Ros2Booster_interfaceMsgRawBytesStampedWrite(value,w){
Ros2Std_msgsMsgHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["msg"].length,true);w.at+=4;
for(const element of value["msg"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Booster_interfaceMsgRawBytesStampedRead(r){const value={};
{
const decoded=Ros2Std_msgsMsgHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>2048)bad();value["msg"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["msg"].push(decoded);
}}
}
return value;}
function Ros2Booster_interfaceMsgRawBytesStampedCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Std_msgsMsgHeaderCapture(field);
}
{const field=value["msg"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["msg"]=values;
}
return out;}
/** Complete ROS2 booster_interface/msg/RemoteControllerState codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceMsgRemoteControllerStateCodec=Object.freeze({codecIdentity:"UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7",maxWireBytes:42,minWireBytes:42,cartesianMaxWireBytes:42n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"event":0,"lx":0,"ly":0,"rx":0,"ry":0,"a":false,"b":false,"x":false,"y":false,"lb":false,"rb":false,"lt":false,"rt":false,"ls":false,"rs":false,"back":false,"start":false,"hat_c":false,"hat_u":false,"hat_d":false,"hat_l":false,"hat_r":false,"hat_lu":false,"hat_ld":false,"hat_ru":false,"hat_rd":false,"hat_pos":0};},
validateValue(value){Ros2Booster_interfaceMsgRemoteControllerStateSize(Ros2Booster_interfaceMsgRemoteControllerStateCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceMsgRemoteControllerStateCapture(value),n=Ros2Booster_interfaceMsgRemoteControllerStateSize(owned),w=new Writer(n);Ros2Booster_interfaceMsgRemoteControllerStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,42,90);if(owned.length<42)bad();const r=new Reader(owned),value=Ros2Booster_interfaceMsgRemoteControllerStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceMsgRemoteControllerStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["event"],4,false);n=addSize(n,4);
if(typeof value["lx"]!=="number"||!Number.isFinite(value["lx"])||!Number.isFinite(Math.fround(value["lx"])))bad();n=addSize(n,4);
if(typeof value["ly"]!=="number"||!Number.isFinite(value["ly"])||!Number.isFinite(Math.fround(value["ly"])))bad();n=addSize(n,4);
if(typeof value["rx"]!=="number"||!Number.isFinite(value["rx"])||!Number.isFinite(Math.fround(value["rx"])))bad();n=addSize(n,4);
if(typeof value["ry"]!=="number"||!Number.isFinite(value["ry"])||!Number.isFinite(Math.fround(value["ry"])))bad();n=addSize(n,4);
if(typeof value["a"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["b"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["x"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["y"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["lb"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rb"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["lt"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rt"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["ls"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rs"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["back"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["start"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_c"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_u"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_d"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_l"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_r"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_lu"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_ld"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_ru"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["hat_rd"]!=="boolean")bad();n=addSize(n,1);
integer(value["hat_pos"],1,false);n=addSize(n,1);
if(n>42)bad(90);return n;}
function Ros2Booster_interfaceMsgRemoteControllerStateWrite(value,w){
w.v.setUint32(w.at,value["event"],true);w.at+=4;
w.v.setFloat32(w.at,value["lx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ly"],true);w.at+=4;
w.v.setFloat32(w.at,value["rx"],true);w.at+=4;
w.v.setFloat32(w.at,value["ry"],true);w.at+=4;
w.v.setUint8(w.at,value["a"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["b"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["x"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["y"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["lb"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rb"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["lt"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rt"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["ls"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rs"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["back"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["start"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_c"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_u"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_d"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_l"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_r"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_lu"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_ld"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_ru"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_rd"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["hat_pos"],true);w.at+=1;
}
function Ros2Booster_interfaceMsgRemoteControllerStateRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["event"]=decoded;
}
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
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["a"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["b"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["x"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["y"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["lb"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rb"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["lt"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rt"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ls"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rs"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["back"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["start"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_c"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_u"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_d"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_l"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_r"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_lu"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_ld"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_ru"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["hat_rd"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["hat_pos"]=decoded;
}
return value;}
function Ros2Booster_interfaceMsgRemoteControllerStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["event"];
out["event"]=field;
}
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
{const field=value["a"];
out["a"]=field;
}
{const field=value["b"];
out["b"]=field;
}
{const field=value["x"];
out["x"]=field;
}
{const field=value["y"];
out["y"]=field;
}
{const field=value["lb"];
out["lb"]=field;
}
{const field=value["rb"];
out["rb"]=field;
}
{const field=value["lt"];
out["lt"]=field;
}
{const field=value["rt"];
out["rt"]=field;
}
{const field=value["ls"];
out["ls"]=field;
}
{const field=value["rs"];
out["rs"]=field;
}
{const field=value["back"];
out["back"]=field;
}
{const field=value["start"];
out["start"]=field;
}
{const field=value["hat_c"];
out["hat_c"]=field;
}
{const field=value["hat_u"];
out["hat_u"]=field;
}
{const field=value["hat_d"];
out["hat_d"]=field;
}
{const field=value["hat_l"];
out["hat_l"]=field;
}
{const field=value["hat_r"];
out["hat_r"]=field;
}
{const field=value["hat_lu"];
out["hat_lu"]=field;
}
{const field=value["hat_ld"];
out["hat_ld"]=field;
}
{const field=value["hat_ru"];
out["hat_ru"]=field;
}
{const field=value["hat_rd"];
out["hat_rd"]=field;
}
{const field=value["hat_pos"];
out["hat_pos"]=field;
}
return out;}
/** Complete ROS2 booster_interface/srv/AgentService_Request codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceSrvAgentService_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8",maxWireBytes:4100,minWireBytes:4,cartesianMaxWireBytes:4100n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"body":""};},
validateValue(value){Ros2Booster_interfaceSrvAgentService_RequestSize(Ros2Booster_interfaceSrvAgentService_RequestCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceSrvAgentService_RequestCapture(value),n=Ros2Booster_interfaceSrvAgentService_RequestSize(owned),w=new Writer(n);Ros2Booster_interfaceSrvAgentService_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4100,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Booster_interfaceSrvAgentService_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceSrvAgentService_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>4100)bad(90);return n;}
function Ros2Booster_interfaceSrvAgentService_RequestWrite(value,w){
w.text(value["body"],4096);
}
function Ros2Booster_interfaceSrvAgentService_RequestRead(r){const value={};
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Booster_interfaceSrvAgentService_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 booster_interface/srv/AgentService_Response codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceSrvAgentService_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520",maxWireBytes:4100,minWireBytes:4,cartesianMaxWireBytes:4100n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"body":""};},
validateValue(value){Ros2Booster_interfaceSrvAgentService_ResponseSize(Ros2Booster_interfaceSrvAgentService_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceSrvAgentService_ResponseCapture(value),n=Ros2Booster_interfaceSrvAgentService_ResponseSize(owned),w=new Writer(n);Ros2Booster_interfaceSrvAgentService_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4100,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Booster_interfaceSrvAgentService_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceSrvAgentService_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["body"],4096).length);
if(n>4100)bad(90);return n;}
function Ros2Booster_interfaceSrvAgentService_ResponseWrite(value,w){
w.text(value["body"],4096);
}
function Ros2Booster_interfaceSrvAgentService_ResponseRead(r){const value={};
{
const decoded=r.text(4096);
value["body"]=decoded;
}
return value;}
function Ros2Booster_interfaceSrvAgentService_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["body"];
out["body"]=field;
}
return out;}
/** Complete ROS2 booster_interface/srv/RpcService_Request codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceSrvRpcService_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150",maxWireBytes:4108,minWireBytes:12,cartesianMaxWireBytes:4108n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":Ros2Booster_interfaceMsgBoosterApiReqMsgCodec.defaultValue()};},
validateValue(value){Ros2Booster_interfaceSrvRpcService_RequestSize(Ros2Booster_interfaceSrvRpcService_RequestCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceSrvRpcService_RequestCapture(value),n=Ros2Booster_interfaceSrvRpcService_RequestSize(owned),w=new Writer(n);Ros2Booster_interfaceSrvRpcService_RequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4108,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Booster_interfaceSrvRpcService_RequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceSrvRpcService_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Booster_interfaceMsgBoosterApiReqMsgSize(value["msg"]));
if(n>4108)bad(90);return n;}
function Ros2Booster_interfaceSrvRpcService_RequestWrite(value,w){
Ros2Booster_interfaceMsgBoosterApiReqMsgWrite(value["msg"],w);
}
function Ros2Booster_interfaceSrvRpcService_RequestRead(r){const value={};
{
const decoded=Ros2Booster_interfaceMsgBoosterApiReqMsgRead(r);
value["msg"]=decoded;
}
return value;}
function Ros2Booster_interfaceSrvRpcService_RequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
out["msg"]=Ros2Booster_interfaceMsgBoosterApiReqMsgCapture(field);
}
return out;}
/** Complete ROS2 booster_interface/srv/RpcService_Response codec; maxima are not actual lengths. */
export const Ros2Booster_interfaceSrvRpcService_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322",maxWireBytes:4108,minWireBytes:12,cartesianMaxWireBytes:4108n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"msg":Ros2Booster_interfaceMsgBoosterApiRespMsgCodec.defaultValue()};},
validateValue(value){Ros2Booster_interfaceSrvRpcService_ResponseSize(Ros2Booster_interfaceSrvRpcService_ResponseCapture(value));return true;},
encode(value){const owned=Ros2Booster_interfaceSrvRpcService_ResponseCapture(value),n=Ros2Booster_interfaceSrvRpcService_ResponseSize(owned),w=new Writer(n);Ros2Booster_interfaceSrvRpcService_ResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4108,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros2Booster_interfaceSrvRpcService_ResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Booster_interfaceSrvRpcService_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Booster_interfaceMsgBoosterApiRespMsgSize(value["msg"]));
if(n>4108)bad(90);return n;}
function Ros2Booster_interfaceSrvRpcService_ResponseWrite(value,w){
Ros2Booster_interfaceMsgBoosterApiRespMsgWrite(value["msg"],w);
}
function Ros2Booster_interfaceSrvRpcService_ResponseRead(r){const value={};
{
const decoded=Ros2Booster_interfaceMsgBoosterApiRespMsgRead(r);
value["msg"]=decoded;
}
return value;}
function Ros2Booster_interfaceSrvRpcService_ResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["msg"];
out["msg"]=Ros2Booster_interfaceMsgBoosterApiRespMsgCapture(field);
}
return out;}
