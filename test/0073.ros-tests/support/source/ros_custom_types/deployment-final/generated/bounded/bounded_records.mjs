/** Generated bounded-v2 canonical codecs. No ROS, filesystem or network imports.
 * Records are ordinary owned values. Decode throws before exposing malformed bytes;
 * encode validates the complete value before allocating its one wire output.
 * 64-bit integers remain BigInt. Strings are strict UTF-8 with no embedded NUL.
 * Pass these codec objects to existing SDK typed Node factories explicitly. */
const utf8Encoder=new TextEncoder(),utf8Decoder=new TextDecoder("utf-8",{fatal:true,ignoreBOM:true});
function bad(){throw new TypeError("invalid bounded canonical value");}
function textBytes(value,bound){if(typeof value!=="string"||value.length>bound||value.includes("\0"))bad();for(let i=0;i<value.length;++i){const c=value.charCodeAt(i);if(c>=0xd800&&c<=0xdbff){if(++i===value.length)bad();const d=value.charCodeAt(i);if(d<0xdc00||d>0xdfff)bad();}else if(c>=0xdc00&&c<=0xdfff)bad();}const b=utf8Encoder.encode(value);if(b.length>bound)bad();return b;}
function integer(value,width,signed){if(width===8){if(typeof value!=="bigint"||value<(signed?-(1n<<63n):0n)||value>(signed?(1n<<63n)-1n:(1n<<64n)-1n))bad();}else if(!Number.isInteger(value)||value<(signed?-(2**(width*8-1)):0)||value>(signed?2**(width*8-1)-1:2**(width*8)-1))bad();}
class Reader{constructor(bytes){if(!(bytes instanceof Uint8Array)||bytes.length>32768)bad();this.b=bytes;this.v=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);this.at=0;}take(n){if(!Number.isInteger(n)||n<0||n>this.b.length-this.at)bad();const p=this.at;this.at+=n;return p;}u32(){return this.v.getUint32(this.take(4),true);}text(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n),s=utf8Decoder.decode(this.b.subarray(p,p+n));if(s.includes("\0"))bad();return s;}}
class Writer{constructor(size){if(size>32768)bad();this.b=new Uint8Array(size);this.v=new DataView(this.b.buffer);this.at=0;}text(value,bound){const b=textBytes(value,bound);this.v.setUint32(this.at,b.length,true);this.at+=4;this.b.set(b,this.at);this.at+=b.length;}}
/** Complete ROS1 uwvm_ros_bounded_v2/Point codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2PointCodec=Object.freeze({codecIdentity:"UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28",maxWireBytes:5,minWireBytes:5,
defaultValue(){return {"value":0,"enabled":false};},
validateValue(value){Ros1Uwvm_ros_bounded_v2PointSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2PointSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2PointWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<5||bytes.length>5)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2PointRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2PointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],4,true);n+=4;
if(typeof value["enabled"]!=="boolean")bad();n+=1;
if(n>5)bad();return n;}
function Ros1Uwvm_ros_bounded_v2PointWrite(value,w){
w.v.setInt32(w.at,value["value"],true);w.at+=4;
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros1Uwvm_ros_bounded_v2PointRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["value"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/Envelope codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2EnvelopeCodec=Object.freeze({codecIdentity:"UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4",maxWireBytes:120,minWireBytes:20,
defaultValue(){return {"label":"","samples":[],"points":[],"labels":Array.from({length:2},()=>(""))};},
validateValue(value){Ros1Uwvm_ros_bounded_v2EnvelopeSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2EnvelopeSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2EnvelopeWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<20||bytes.length>120)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2EnvelopeRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2EnvelopeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=4+textBytes(value["label"],32).length;
if(!Array.isArray(value["samples"])||value["samples"].length>8)bad();
n+=4;
for(const element of value["samples"]){
integer(element,2,false);n+=2;
}
if(!Array.isArray(value["points"])||value["points"].length>4)bad();
n+=4;
for(const element of value["points"]){
n+=Ros1Uwvm_ros_bounded_v2PointSize(element);
}
if(!Array.isArray(value["labels"])||value["labels"].length!==2)bad();
for(const element of value["labels"]){
n+=4+textBytes(element,16).length;
}
if(n>120)bad();return n;}
function Ros1Uwvm_ros_bounded_v2EnvelopeWrite(value,w){
w.text(value["label"],32);
w.v.setUint32(w.at,value["samples"].length,true);w.at+=4;
for(const element of value["samples"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Uwvm_ros_bounded_v2PointWrite(element,w);
}
for(const element of value["labels"]){
w.text(element,16);
}
}
function Ros1Uwvm_ros_bounded_v2EnvelopeRead(r){const value={};
{
const decoded=r.text(32);
value["label"]=decoded;
}
{
{const count=r.u32();if(count>8)bad();value["samples"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["samples"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Uwvm_ros_bounded_v2PointRead(r);
value["points"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["labels"]=[];for(let i=0;i<count;++i){
const decoded=r.text(16);
value["labels"].push(decoded);
}}
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoRequest codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2EchoRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420",maxWireBytes:124,minWireBytes:24,
defaultValue(){return {"sample":Ros1Uwvm_ros_bounded_v2EnvelopeCodec.defaultValue(),"limit":0};},
validateValue(value){Ros1Uwvm_ros_bounded_v2EchoRequestSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2EchoRequestSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2EchoRequestWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<24||bytes.length>124)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2EchoRequestRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2EchoRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=Ros1Uwvm_ros_bounded_v2EnvelopeSize(value["sample"]);
integer(value["limit"],4,false);n+=4;
if(n>124)bad();return n;}
function Ros1Uwvm_ros_bounded_v2EchoRequestWrite(value,w){
Ros1Uwvm_ros_bounded_v2EnvelopeWrite(value["sample"],w);
w.v.setUint32(w.at,value["limit"],true);w.at+=4;
}
function Ros1Uwvm_ros_bounded_v2EchoRequestRead(r){const value={};
{
const decoded=Ros1Uwvm_ros_bounded_v2EnvelopeRead(r);
value["sample"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["limit"]=decoded;
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoResponse codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2EchoResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b",maxWireBytes:121,minWireBytes:21,
defaultValue(){return {"accepted":false,"echoed":Ros1Uwvm_ros_bounded_v2EnvelopeCodec.defaultValue()};},
validateValue(value){Ros1Uwvm_ros_bounded_v2EchoResponseSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2EchoResponseSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2EchoResponseWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<21||bytes.length>121)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2EchoResponseRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2EchoResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["accepted"]!=="boolean")bad();n+=1;
n+=Ros1Uwvm_ros_bounded_v2EnvelopeSize(value["echoed"]);
if(n>121)bad();return n;}
function Ros1Uwvm_ros_bounded_v2EchoResponseWrite(value,w){
w.v.setUint8(w.at,value["accepted"]?1:0,true);w.at+=1;
Ros1Uwvm_ros_bounded_v2EnvelopeWrite(value["echoed"],w);
}
function Ros1Uwvm_ros_bounded_v2EchoResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["accepted"]=decoded!==0;
}
{
const decoded=Ros1Uwvm_ros_bounded_v2EnvelopeRead(r);
value["echoed"]=decoded;
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyRequest codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2EmptyRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40",maxWireBytes:0,minWireBytes:0,
defaultValue(){return {};},
validateValue(value){Ros1Uwvm_ros_bounded_v2EmptyRequestSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2EmptyRequestSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2EmptyRequestWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<0||bytes.length>0)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2EmptyRequestRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2EmptyRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad();return n;}
function Ros1Uwvm_ros_bounded_v2EmptyRequestWrite(value,w){
}
function Ros1Uwvm_ros_bounded_v2EmptyRequestRead(r){const value={};
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyResponse codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2EmptyResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817",maxWireBytes:0,minWireBytes:0,
defaultValue(){return {};},
validateValue(value){Ros1Uwvm_ros_bounded_v2EmptyResponseSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2EmptyResponseSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2EmptyResponseWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<0||bytes.length>0)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2EmptyResponseRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2EmptyResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad();return n;}
function Ros1Uwvm_ros_bounded_v2EmptyResponseWrite(value,w){
}
function Ros1Uwvm_ros_bounded_v2EmptyResponseRead(r){const value={};
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestGoal codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2DigestGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc",maxWireBytes:120,minWireBytes:20,
defaultValue(){return {"sample":Ros1Uwvm_ros_bounded_v2EnvelopeCodec.defaultValue()};},
validateValue(value){Ros1Uwvm_ros_bounded_v2DigestGoalSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2DigestGoalSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2DigestGoalWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<20||bytes.length>120)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2DigestGoalRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2DigestGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=Ros1Uwvm_ros_bounded_v2EnvelopeSize(value["sample"]);
if(n>120)bad();return n;}
function Ros1Uwvm_ros_bounded_v2DigestGoalWrite(value,w){
Ros1Uwvm_ros_bounded_v2EnvelopeWrite(value["sample"],w);
}
function Ros1Uwvm_ros_bounded_v2DigestGoalRead(r){const value={};
{
const decoded=Ros1Uwvm_ros_bounded_v2EnvelopeRead(r);
value["sample"]=decoded;
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestResult codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2DigestResultCodec=Object.freeze({codecIdentity:"UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd",maxWireBytes:41,minWireBytes:9,
defaultValue(){return {"accepted":false,"sum":0,"note":""};},
validateValue(value){Ros1Uwvm_ros_bounded_v2DigestResultSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2DigestResultSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2DigestResultWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<9||bytes.length>41)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2DigestResultRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2DigestResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["accepted"]!=="boolean")bad();n+=1;
integer(value["sum"],4,false);n+=4;
n+=4+textBytes(value["note"],32).length;
if(n>41)bad();return n;}
function Ros1Uwvm_ros_bounded_v2DigestResultWrite(value,w){
w.v.setUint8(w.at,value["accepted"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["sum"],true);w.at+=4;
w.text(value["note"],32);
}
function Ros1Uwvm_ros_bounded_v2DigestResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["accepted"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["sum"]=decoded;
}
{
const decoded=r.text(32);
value["note"]=decoded;
}
return value;}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestFeedback codec; maxima are not actual lengths. */
export const Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619",maxWireBytes:24,minWireBytes:8,
defaultValue(){return {"processed":0,"phase":""};},
validateValue(value){Ros1Uwvm_ros_bounded_v2DigestFeedbackSize(value);return true;},
encode(value){const n=Ros1Uwvm_ros_bounded_v2DigestFeedbackSize(value),w=new Writer(n);Ros1Uwvm_ros_bounded_v2DigestFeedbackWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<8||bytes.length>24)bad();const r=new Reader(bytes),value=Ros1Uwvm_ros_bounded_v2DigestFeedbackRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Uwvm_ros_bounded_v2DigestFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["processed"],4,false);n+=4;
n+=4+textBytes(value["phase"],16).length;
if(n>24)bad();return n;}
function Ros1Uwvm_ros_bounded_v2DigestFeedbackWrite(value,w){
w.v.setUint32(w.at,value["processed"],true);w.at+=4;
w.text(value["phase"],16);
}
function Ros1Uwvm_ros_bounded_v2DigestFeedbackRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["processed"]=decoded;
}
{
const decoded=r.text(16);
value["phase"]=decoded;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Point codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2MsgPointCodec=Object.freeze({codecIdentity:"UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333",maxWireBytes:5,minWireBytes:5,
defaultValue(){return {"value":0,"enabled":false};},
validateValue(value){Ros2Uwvm_ros_bounded_v2MsgPointSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2MsgPointSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2MsgPointWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<5||bytes.length>5)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2MsgPointRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2MsgPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["value"],4,true);n+=4;
if(typeof value["enabled"]!=="boolean")bad();n+=1;
if(n>5)bad();return n;}
function Ros2Uwvm_ros_bounded_v2MsgPointWrite(value,w){
w.v.setInt32(w.at,value["value"],true);w.at+=4;
w.v.setUint8(w.at,value["enabled"]?1:0,true);w.at+=1;
}
function Ros2Uwvm_ros_bounded_v2MsgPointRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["value"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["enabled"]=decoded!==0;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Envelope codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec=Object.freeze({codecIdentity:"UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36",maxWireBytes:120,minWireBytes:20,
defaultValue(){return {"label":"","samples":[],"points":[],"labels":Array.from({length:2},()=>(""))};},
validateValue(value){Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2MsgEnvelopeWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<20||bytes.length>120)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2MsgEnvelopeRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=4+textBytes(value["label"],32).length;
if(!Array.isArray(value["samples"])||value["samples"].length>8)bad();
n+=4;
for(const element of value["samples"]){
integer(element,2,false);n+=2;
}
if(!Array.isArray(value["points"])||value["points"].length>4)bad();
n+=4;
for(const element of value["points"]){
n+=Ros2Uwvm_ros_bounded_v2MsgPointSize(element);
}
if(!Array.isArray(value["labels"])||value["labels"].length!==2)bad();
for(const element of value["labels"]){
n+=4+textBytes(element,16).length;
}
if(n>120)bad();return n;}
function Ros2Uwvm_ros_bounded_v2MsgEnvelopeWrite(value,w){
w.text(value["label"],32);
w.v.setUint32(w.at,value["samples"].length,true);w.at+=4;
for(const element of value["samples"]){
w.v.setUint16(w.at,element,true);w.at+=2;
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros2Uwvm_ros_bounded_v2MsgPointWrite(element,w);
}
for(const element of value["labels"]){
w.text(element,16);
}
}
function Ros2Uwvm_ros_bounded_v2MsgEnvelopeRead(r){const value={};
{
const decoded=r.text(32);
value["label"]=decoded;
}
{
{const count=r.u32();if(count>8)bad();value["samples"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint16(r.take(2),true);
value["samples"].push(decoded);
}}
}
{
{const count=r.u32();if(count>4)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros2Uwvm_ros_bounded_v2MsgPointRead(r);
value["points"].push(decoded);
}}
}
{
{const count=2;if(count>2)bad();value["labels"]=[];for(let i=0;i<count;++i){
const decoded=r.text(16);
value["labels"].push(decoded);
}}
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2SrvEcho_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3",maxWireBytes:124,minWireBytes:24,
defaultValue(){return {"sample":Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec.defaultValue(),"limit":0};},
validateValue(value){Ros2Uwvm_ros_bounded_v2SrvEcho_RequestSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2SrvEcho_RequestSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2SrvEcho_RequestWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<24||bytes.length>124)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2SrvEcho_RequestRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2SrvEcho_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value["sample"]);
integer(value["limit"],4,false);n+=4;
if(n>124)bad();return n;}
function Ros2Uwvm_ros_bounded_v2SrvEcho_RequestWrite(value,w){
Ros2Uwvm_ros_bounded_v2MsgEnvelopeWrite(value["sample"],w);
w.v.setUint32(w.at,value["limit"],true);w.at+=4;
}
function Ros2Uwvm_ros_bounded_v2SrvEcho_RequestRead(r){const value={};
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelopeRead(r);
value["sample"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["limit"]=decoded;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5",maxWireBytes:121,minWireBytes:21,
defaultValue(){return {"accepted":false,"echoed":Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec.defaultValue()};},
validateValue(value){Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<21||bytes.length>121)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["accepted"]!=="boolean")bad();n+=1;
n+=Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value["echoed"]);
if(n>121)bad();return n;}
function Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseWrite(value,w){
w.v.setUint8(w.at,value["accepted"]?1:0,true);w.at+=1;
Ros2Uwvm_ros_bounded_v2MsgEnvelopeWrite(value["echoed"],w);
}
function Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["accepted"]=decoded!==0;
}
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelopeRead(r);
value["echoed"]=decoded;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Request codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestCodec=Object.freeze({codecIdentity:"UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4",maxWireBytes:0,minWireBytes:0,
defaultValue(){return {};},
validateValue(value){Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<0||bytes.length>0)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad();return n;}
function Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestWrite(value,w){
}
function Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestRead(r){const value={};
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Response codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b",maxWireBytes:0,minWireBytes:0,
defaultValue(){return {};},
validateValue(value){Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<0||bytes.length>0)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad();return n;}
function Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseWrite(value,w){
}
function Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseRead(r){const value={};
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Goal codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2ActionDigest_GoalCodec=Object.freeze({codecIdentity:"UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c",maxWireBytes:120,minWireBytes:20,
defaultValue(){return {"sample":Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec.defaultValue()};},
validateValue(value){Ros2Uwvm_ros_bounded_v2ActionDigest_GoalSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2ActionDigest_GoalSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2ActionDigest_GoalWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<20||bytes.length>120)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2ActionDigest_GoalRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2ActionDigest_GoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n+=Ros2Uwvm_ros_bounded_v2MsgEnvelopeSize(value["sample"]);
if(n>120)bad();return n;}
function Ros2Uwvm_ros_bounded_v2ActionDigest_GoalWrite(value,w){
Ros2Uwvm_ros_bounded_v2MsgEnvelopeWrite(value["sample"],w);
}
function Ros2Uwvm_ros_bounded_v2ActionDigest_GoalRead(r){const value={};
{
const decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelopeRead(r);
value["sample"]=decoded;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Result codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2ActionDigest_ResultCodec=Object.freeze({codecIdentity:"UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da",maxWireBytes:41,minWireBytes:9,
defaultValue(){return {"accepted":false,"sum":0,"note":""};},
validateValue(value){Ros2Uwvm_ros_bounded_v2ActionDigest_ResultSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2ActionDigest_ResultSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2ActionDigest_ResultWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<9||bytes.length>41)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2ActionDigest_ResultRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2ActionDigest_ResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["accepted"]!=="boolean")bad();n+=1;
integer(value["sum"],4,false);n+=4;
n+=4+textBytes(value["note"],32).length;
if(n>41)bad();return n;}
function Ros2Uwvm_ros_bounded_v2ActionDigest_ResultWrite(value,w){
w.v.setUint8(w.at,value["accepted"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["sum"],true);w.at+=4;
w.text(value["note"],32);
}
function Ros2Uwvm_ros_bounded_v2ActionDigest_ResultRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["accepted"]=decoded!==0;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["sum"]=decoded;
}
{
const decoded=r.text(32);
value["note"]=decoded;
}
return value;}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Feedback codec; maxima are not actual lengths. */
export const Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1",maxWireBytes:24,minWireBytes:8,
defaultValue(){return {"processed":0,"phase":""};},
validateValue(value){Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackSize(value);return true;},
encode(value){const n=Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackSize(value),w=new Writer(n);Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackWrite(value,w);if(w.at!==n)bad();return w.b;},
decode(bytes){if(bytes.length<8||bytes.length>24)bad();const r=new Reader(bytes),value=Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackRead(r);if(r.at!==bytes.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["processed"],4,false);n+=4;
n+=4+textBytes(value["phase"],16).length;
if(n>24)bad();return n;}
function Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackWrite(value,w){
w.v.setUint32(w.at,value["processed"],true);w.at+=4;
w.text(value["phase"],16);
}
function Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["processed"]=decoded;
}
{
const decoded=r.text(16);
value["phase"]=decoded;
}
return value;}
