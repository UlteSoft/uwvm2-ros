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
/** Complete ROS1 abb_egm_msgs/EGMChannelState codec; maxima are not actual lengths. */
export const Ros1Abb_egm_msgsEGMChannelStateCodec=Object.freeze({codecIdentity:"UWVMROS1_452d2adde3b1475fe3ff09ea11ff2ec7105a58c32e20d764e0c2ff152cb432f2",maxWireBytes:81,minWireBytes:17,cartesianMaxWireBytes:81n,aggregateWireBytes:0,constants:Object.freeze({"EGM_UNDEFINED":1,"EGM_ERROR":2,"EGM_STOPPED":3,"EGM_RUNNING":4,"MOTORS_UNDEFINED":1,"MOTORS_ON":2,"MOTORS_OFF":3,"RAPID_UNDEFINED":1,"RAPID_STOPPED":2,"RAPID_RUNNING":3}),
defaultValue(){return {"name":"","active":false,"egm_convergence_met":false,"egm_client_state":0,"motor_state":0,"rapid_execution_state":0,"utilization_rate":0};},
validateValue(value){Ros1Abb_egm_msgsEGMChannelStateSize(Ros1Abb_egm_msgsEGMChannelStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_egm_msgsEGMChannelStateCapture(value),n=Ros1Abb_egm_msgsEGMChannelStateSize(owned),w=new Writer(n);Ros1Abb_egm_msgsEGMChannelStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,81,90);if(owned.length<17)bad();const r=new Reader(owned),value=Ros1Abb_egm_msgsEGMChannelStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_egm_msgsEGMChannelStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["active"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["egm_convergence_met"]!=="boolean")bad();n=addSize(n,1);
integer(value["egm_client_state"],1,false);n=addSize(n,1);
integer(value["motor_state"],1,false);n=addSize(n,1);
integer(value["rapid_execution_state"],1,false);n=addSize(n,1);
if(typeof value["utilization_rate"]!=="number"||!Number.isFinite(value["utilization_rate"]))bad();n=addSize(n,8);
if(n>81)bad(90);return n;}
function Ros1Abb_egm_msgsEGMChannelStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["active"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["egm_convergence_met"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["egm_client_state"],true);w.at+=1;
w.v.setUint8(w.at,value["motor_state"],true);w.at+=1;
w.v.setUint8(w.at,value["rapid_execution_state"],true);w.at+=1;
w.v.setFloat64(w.at,value["utilization_rate"],true);w.at+=8;
}
function Ros1Abb_egm_msgsEGMChannelStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["active"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["egm_convergence_met"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["egm_client_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["motor_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["rapid_execution_state"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["utilization_rate"]=decoded;
}
return value;}
function Ros1Abb_egm_msgsEGMChannelStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["active"];
out["active"]=field;
}
{const field=value["egm_convergence_met"];
out["egm_convergence_met"]=field;
}
{const field=value["egm_client_state"];
out["egm_client_state"]=field;
}
{const field=value["motor_state"];
out["motor_state"]=field;
}
{const field=value["rapid_execution_state"];
out["rapid_execution_state"]=field;
}
{const field=value["utilization_rate"];
out["utilization_rate"]=field;
}
return out;}
/** Complete ROS1 time codec; maxima are not actual lengths. */
export const Ros1TimeCodec=Object.freeze({codecIdentity:"UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nsec":0};},
validateValue(value){Ros1TimeSize(Ros1TimeCapture(value));return true;},
encode(value){const owned=Ros1TimeCapture(value),n=Ros1TimeSize(owned),w=new Writer(n);Ros1TimeWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1TimeRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1TimeSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,false);n=addSize(n,4);
integer(value["nsec"],4,false);n=addSize(n,4);
if(value["nsec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros1TimeWrite(value,w){
w.v.setUint32(w.at,value["sec"],true);w.at+=4;
w.v.setUint32(w.at,value["nsec"],true);w.at+=4;
}
function Ros1TimeRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getUint32(r.take(4),true);
value["nsec"]=decoded;
}
if(value["nsec"]>=1000000000)bad();
return value;}
function Ros1TimeCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nsec"];
out["nsec"]=field;
}
return out;}
/** Complete ROS1 std_msgs/Header codec; maxima are not actual lengths. */
export const Ros1Std_msgsHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_ca1b8ffce276bdc4f86dccae58ede9f860ffe8778c6d4ffe0fb70675777801db",maxWireBytes:80,minWireBytes:16,cartesianMaxWireBytes:80n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"seq":0,"stamp":Ros1TimeCodec.defaultValue(),"frame_id":""};},
validateValue(value){Ros1Std_msgsHeaderSize(Ros1Std_msgsHeaderCapture(value));return true;},
encode(value){const owned=Ros1Std_msgsHeaderCapture(value),n=Ros1Std_msgsHeaderSize(owned),w=new Writer(n);Ros1Std_msgsHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,80,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Std_msgsHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Std_msgsHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["seq"],4,false);n=addSize(n,4);
n=addSize(n,Ros1TimeSize(value["stamp"]));
n=addSize(n,4+textBytes(value["frame_id"],64).length);
if(n>80)bad(90);return n;}
function Ros1Std_msgsHeaderWrite(value,w){
w.v.setUint32(w.at,value["seq"],true);w.at+=4;
Ros1TimeWrite(value["stamp"],w);
w.text(value["frame_id"],64);
}
function Ros1Std_msgsHeaderRead(r){const value={};
{
const decoded=r.v.getUint32(r.take(4),true);
value["seq"]=decoded;
}
{
const decoded=Ros1TimeRead(r);
value["stamp"]=decoded;
}
{
const decoded=r.text(64);
value["frame_id"]=decoded;
}
return value;}
function Ros1Std_msgsHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["seq"];
out["seq"]=field;
}
{const field=value["stamp"];
out["stamp"]=Ros1TimeCapture(field);
}
{const field=value["frame_id"];
out["frame_id"]=field;
}
return out;}
/** Complete ROS1 abb_egm_msgs/EGMState codec; maxima are not actual lengths. */
export const Ros1Abb_egm_msgsEGMStateCodec=Object.freeze({codecIdentity:"UWVMROS1_f7fe1d3924dc4e497c58aab27da69cfa6ab1a11f64b970b42e2d040a1312c4d8",maxWireBytes:1380,minWireBytes:20,cartesianMaxWireBytes:1380n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"egm_channels":[]};},
validateValue(value){Ros1Abb_egm_msgsEGMStateSize(Ros1Abb_egm_msgsEGMStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_egm_msgsEGMStateCapture(value),n=Ros1Abb_egm_msgsEGMStateSize(owned),w=new Writer(n);Ros1Abb_egm_msgsEGMStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1380,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Abb_egm_msgsEGMStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_egm_msgsEGMStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["egm_channels"])||value["egm_channels"].length>16)bad();
n=addSize(n,4);
for(const element of value["egm_channels"]){
n=addSize(n,Ros1Abb_egm_msgsEGMChannelStateSize(element));
}
if(n>1380)bad(90);return n;}
function Ros1Abb_egm_msgsEGMStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["egm_channels"].length,true);w.at+=4;
for(const element of value["egm_channels"]){
Ros1Abb_egm_msgsEGMChannelStateWrite(element,w);
}
}
function Ros1Abb_egm_msgsEGMStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["egm_channels"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Abb_egm_msgsEGMChannelStateRead(r);
value["egm_channels"].push(decoded);
}}
}
return value;}
function Ros1Abb_egm_msgsEGMStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["egm_channels"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Abb_egm_msgsEGMChannelStateCapture(element);
}out["egm_channels"]=values;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/ConfData codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsConfDataCodec=Object.freeze({codecIdentity:"UWVMROS1_293eb01e8fa150a8a3c741626b76804255d0eb321a0484aac402201447392af2",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cf1":0,"cf4":0,"cf6":0,"cfx":0};},
validateValue(value){Ros1Abb_rapid_msgsConfDataSize(Ros1Abb_rapid_msgsConfDataCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsConfDataCapture(value),n=Ros1Abb_rapid_msgsConfDataSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsConfDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsConfDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsConfDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["cf1"]!=="number"||!Number.isFinite(value["cf1"])||!Number.isFinite(Math.fround(value["cf1"])))bad();n=addSize(n,4);
if(typeof value["cf4"]!=="number"||!Number.isFinite(value["cf4"])||!Number.isFinite(Math.fround(value["cf4"])))bad();n=addSize(n,4);
if(typeof value["cf6"]!=="number"||!Number.isFinite(value["cf6"])||!Number.isFinite(Math.fround(value["cf6"])))bad();n=addSize(n,4);
if(typeof value["cfx"]!=="number"||!Number.isFinite(value["cfx"])||!Number.isFinite(Math.fround(value["cfx"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Abb_rapid_msgsConfDataWrite(value,w){
w.v.setFloat32(w.at,value["cf1"],true);w.at+=4;
w.v.setFloat32(w.at,value["cf4"],true);w.at+=4;
w.v.setFloat32(w.at,value["cf6"],true);w.at+=4;
w.v.setFloat32(w.at,value["cfx"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsConfDataRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cf6"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cfx"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsConfDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cf1"];
out["cf1"]=field;
}
{const field=value["cf4"];
out["cf4"]=field;
}
{const field=value["cf6"];
out["cf6"]=field;
}
{const field=value["cfx"];
out["cfx"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/ExtJoint codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsExtJointCodec=Object.freeze({codecIdentity:"UWVMROS1_653a47c44ab85151fd9e6215901fb154962c32fe1082386733ec6205eaae613a",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"eax_a":0,"eax_b":0,"eax_c":0,"eax_d":0,"eax_e":0,"eax_f":0};},
validateValue(value){Ros1Abb_rapid_msgsExtJointSize(Ros1Abb_rapid_msgsExtJointCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsExtJointCapture(value),n=Ros1Abb_rapid_msgsExtJointSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsExtJointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsExtJointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsExtJointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["eax_a"]!=="number"||!Number.isFinite(value["eax_a"])||!Number.isFinite(Math.fround(value["eax_a"])))bad();n=addSize(n,4);
if(typeof value["eax_b"]!=="number"||!Number.isFinite(value["eax_b"])||!Number.isFinite(Math.fround(value["eax_b"])))bad();n=addSize(n,4);
if(typeof value["eax_c"]!=="number"||!Number.isFinite(value["eax_c"])||!Number.isFinite(Math.fround(value["eax_c"])))bad();n=addSize(n,4);
if(typeof value["eax_d"]!=="number"||!Number.isFinite(value["eax_d"])||!Number.isFinite(Math.fround(value["eax_d"])))bad();n=addSize(n,4);
if(typeof value["eax_e"]!=="number"||!Number.isFinite(value["eax_e"])||!Number.isFinite(Math.fround(value["eax_e"])))bad();n=addSize(n,4);
if(typeof value["eax_f"]!=="number"||!Number.isFinite(value["eax_f"])||!Number.isFinite(Math.fround(value["eax_f"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Abb_rapid_msgsExtJointWrite(value,w){
w.v.setFloat32(w.at,value["eax_a"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_b"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_c"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_d"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_e"],true);w.at+=4;
w.v.setFloat32(w.at,value["eax_f"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsExtJointRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_a"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_b"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_c"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_d"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_e"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["eax_f"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsExtJointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["eax_a"];
out["eax_a"]=field;
}
{const field=value["eax_b"];
out["eax_b"]=field;
}
{const field=value["eax_c"];
out["eax_c"]=field;
}
{const field=value["eax_d"];
out["eax_d"]=field;
}
{const field=value["eax_e"];
out["eax_e"]=field;
}
{const field=value["eax_f"];
out["eax_f"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/RobJoint codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsRobJointCodec=Object.freeze({codecIdentity:"UWVMROS1_5d45bc0efae8c4f5b26e4043fe537e163c4cc4c6458d1eac9be429c37aaf82f0",maxWireBytes:24,minWireBytes:24,cartesianMaxWireBytes:24n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"rax_1":0,"rax_2":0,"rax_3":0,"rax_4":0,"rax_5":0,"rax_6":0};},
validateValue(value){Ros1Abb_rapid_msgsRobJointSize(Ros1Abb_rapid_msgsRobJointCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsRobJointCapture(value),n=Ros1Abb_rapid_msgsRobJointSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsRobJointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,24,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsRobJointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsRobJointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["rax_1"]!=="number"||!Number.isFinite(value["rax_1"])||!Number.isFinite(Math.fround(value["rax_1"])))bad();n=addSize(n,4);
if(typeof value["rax_2"]!=="number"||!Number.isFinite(value["rax_2"])||!Number.isFinite(Math.fround(value["rax_2"])))bad();n=addSize(n,4);
if(typeof value["rax_3"]!=="number"||!Number.isFinite(value["rax_3"])||!Number.isFinite(Math.fround(value["rax_3"])))bad();n=addSize(n,4);
if(typeof value["rax_4"]!=="number"||!Number.isFinite(value["rax_4"])||!Number.isFinite(Math.fround(value["rax_4"])))bad();n=addSize(n,4);
if(typeof value["rax_5"]!=="number"||!Number.isFinite(value["rax_5"])||!Number.isFinite(Math.fround(value["rax_5"])))bad();n=addSize(n,4);
if(typeof value["rax_6"]!=="number"||!Number.isFinite(value["rax_6"])||!Number.isFinite(Math.fround(value["rax_6"])))bad();n=addSize(n,4);
if(n>24)bad(90);return n;}
function Ros1Abb_rapid_msgsRobJointWrite(value,w){
w.v.setFloat32(w.at,value["rax_1"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_2"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_3"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_4"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_5"],true);w.at+=4;
w.v.setFloat32(w.at,value["rax_6"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsRobJointRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_4"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_5"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["rax_6"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsRobJointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rax_1"];
out["rax_1"]=field;
}
{const field=value["rax_2"];
out["rax_2"]=field;
}
{const field=value["rax_3"];
out["rax_3"]=field;
}
{const field=value["rax_4"];
out["rax_4"]=field;
}
{const field=value["rax_5"];
out["rax_5"]=field;
}
{const field=value["rax_6"];
out["rax_6"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/JointTarget codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsJointTargetCodec=Object.freeze({codecIdentity:"UWVMROS1_4a1cc175468da9204c5f2f399b76712617daec21a0d1675a6e73c84ff170dc24",maxWireBytes:48,minWireBytes:48,cartesianMaxWireBytes:48n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robax":Ros1Abb_rapid_msgsRobJointCodec.defaultValue(),"extax":Ros1Abb_rapid_msgsExtJointCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_msgsJointTargetSize(Ros1Abb_rapid_msgsJointTargetCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsJointTargetCapture(value),n=Ros1Abb_rapid_msgsJointTargetSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsJointTargetWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,48,90);if(owned.length<48)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsJointTargetRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsJointTargetSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_rapid_msgsRobJointSize(value["robax"]));
n=addSize(n,Ros1Abb_rapid_msgsExtJointSize(value["extax"]));
if(n>48)bad(90);return n;}
function Ros1Abb_rapid_msgsJointTargetWrite(value,w){
Ros1Abb_rapid_msgsRobJointWrite(value["robax"],w);
Ros1Abb_rapid_msgsExtJointWrite(value["extax"],w);
}
function Ros1Abb_rapid_msgsJointTargetRead(r){const value={};
{
const decoded=Ros1Abb_rapid_msgsRobJointRead(r);
value["robax"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsExtJointRead(r);
value["extax"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsJointTargetCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robax"];
out["robax"]=Ros1Abb_rapid_msgsRobJointCapture(field);
}
{const field=value["extax"];
out["extax"]=Ros1Abb_rapid_msgsExtJointCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_msgs/Pos codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsPosCodec=Object.freeze({codecIdentity:"UWVMROS1_79edd4553fad8a8f833a3399e354c93538d4971bc3820cc14cf540e960356c01",maxWireBytes:12,minWireBytes:12,cartesianMaxWireBytes:12n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"x":0,"y":0,"z":0};},
validateValue(value){Ros1Abb_rapid_msgsPosSize(Ros1Abb_rapid_msgsPosCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsPosCapture(value),n=Ros1Abb_rapid_msgsPosSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsPosWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,12,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsPosRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsPosSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["x"]!=="number"||!Number.isFinite(value["x"])||!Number.isFinite(Math.fround(value["x"])))bad();n=addSize(n,4);
if(typeof value["y"]!=="number"||!Number.isFinite(value["y"])||!Number.isFinite(Math.fround(value["y"])))bad();n=addSize(n,4);
if(typeof value["z"]!=="number"||!Number.isFinite(value["z"])||!Number.isFinite(Math.fround(value["z"])))bad();n=addSize(n,4);
if(n>12)bad(90);return n;}
function Ros1Abb_rapid_msgsPosWrite(value,w){
w.v.setFloat32(w.at,value["x"],true);w.at+=4;
w.v.setFloat32(w.at,value["y"],true);w.at+=4;
w.v.setFloat32(w.at,value["z"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsPosRead(r){const value={};
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
function Ros1Abb_rapid_msgsPosCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
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
/** Complete ROS1 abb_rapid_msgs/Orient codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsOrientCodec=Object.freeze({codecIdentity:"UWVMROS1_5a9c29323a5e3bc0c4c0afa7d305738cd52b49ae339e72f6b34b35b165f3d270",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"q1":0,"q2":0,"q3":0,"q4":0};},
validateValue(value){Ros1Abb_rapid_msgsOrientSize(Ros1Abb_rapid_msgsOrientCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsOrientCapture(value),n=Ros1Abb_rapid_msgsOrientSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsOrientWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsOrientRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsOrientSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["q1"]!=="number"||!Number.isFinite(value["q1"])||!Number.isFinite(Math.fround(value["q1"])))bad();n=addSize(n,4);
if(typeof value["q2"]!=="number"||!Number.isFinite(value["q2"])||!Number.isFinite(Math.fround(value["q2"])))bad();n=addSize(n,4);
if(typeof value["q3"]!=="number"||!Number.isFinite(value["q3"])||!Number.isFinite(Math.fround(value["q3"])))bad();n=addSize(n,4);
if(typeof value["q4"]!=="number"||!Number.isFinite(value["q4"])||!Number.isFinite(Math.fround(value["q4"])))bad();n=addSize(n,4);
if(n>16)bad(90);return n;}
function Ros1Abb_rapid_msgsOrientWrite(value,w){
w.v.setFloat32(w.at,value["q1"],true);w.at+=4;
w.v.setFloat32(w.at,value["q2"],true);w.at+=4;
w.v.setFloat32(w.at,value["q3"],true);w.at+=4;
w.v.setFloat32(w.at,value["q4"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsOrientRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q1"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q2"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q3"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["q4"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsOrientCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["q1"];
out["q1"]=field;
}
{const field=value["q2"];
out["q2"]=field;
}
{const field=value["q3"];
out["q3"]=field;
}
{const field=value["q4"];
out["q4"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/LoadData codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsLoadDataCodec=Object.freeze({codecIdentity:"UWVMROS1_2cc985cc188a03e7c6de3d22310802cac1e0a748bae7f828d957436c1e03303d",maxWireBytes:44,minWireBytes:44,cartesianMaxWireBytes:44n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"mass":0,"cog":Ros1Abb_rapid_msgsPosCodec.defaultValue(),"aom":Ros1Abb_rapid_msgsOrientCodec.defaultValue(),"ix":0,"iy":0,"iz":0};},
validateValue(value){Ros1Abb_rapid_msgsLoadDataSize(Ros1Abb_rapid_msgsLoadDataCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsLoadDataCapture(value),n=Ros1Abb_rapid_msgsLoadDataSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsLoadDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,44,90);if(owned.length<44)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsLoadDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsLoadDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["mass"]!=="number"||!Number.isFinite(value["mass"])||!Number.isFinite(Math.fround(value["mass"])))bad();n=addSize(n,4);
n=addSize(n,Ros1Abb_rapid_msgsPosSize(value["cog"]));
n=addSize(n,Ros1Abb_rapid_msgsOrientSize(value["aom"]));
if(typeof value["ix"]!=="number"||!Number.isFinite(value["ix"])||!Number.isFinite(Math.fround(value["ix"])))bad();n=addSize(n,4);
if(typeof value["iy"]!=="number"||!Number.isFinite(value["iy"])||!Number.isFinite(Math.fround(value["iy"])))bad();n=addSize(n,4);
if(typeof value["iz"]!=="number"||!Number.isFinite(value["iz"])||!Number.isFinite(Math.fround(value["iz"])))bad();n=addSize(n,4);
if(n>44)bad(90);return n;}
function Ros1Abb_rapid_msgsLoadDataWrite(value,w){
w.v.setFloat32(w.at,value["mass"],true);w.at+=4;
Ros1Abb_rapid_msgsPosWrite(value["cog"],w);
Ros1Abb_rapid_msgsOrientWrite(value["aom"],w);
w.v.setFloat32(w.at,value["ix"],true);w.at+=4;
w.v.setFloat32(w.at,value["iy"],true);w.at+=4;
w.v.setFloat32(w.at,value["iz"],true);w.at+=4;
}
function Ros1Abb_rapid_msgsLoadDataRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["mass"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPosRead(r);
value["cog"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrientRead(r);
value["aom"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ix"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["iy"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["iz"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsLoadDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["mass"];
out["mass"]=field;
}
{const field=value["cog"];
out["cog"]=Ros1Abb_rapid_msgsPosCapture(field);
}
{const field=value["aom"];
out["aom"]=Ros1Abb_rapid_msgsOrientCapture(field);
}
{const field=value["ix"];
out["ix"]=field;
}
{const field=value["iy"];
out["iy"]=field;
}
{const field=value["iz"];
out["iz"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_msgs/Pose codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsPoseCodec=Object.freeze({codecIdentity:"UWVMROS1_3e0cfbb7e25376639d1ff6e555a86e69f8c9ed177c1ef14b301d90b206a19c99",maxWireBytes:28,minWireBytes:28,cartesianMaxWireBytes:28n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trans":Ros1Abb_rapid_msgsPosCodec.defaultValue(),"rot":Ros1Abb_rapid_msgsOrientCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_msgsPoseSize(Ros1Abb_rapid_msgsPoseCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsPoseCapture(value),n=Ros1Abb_rapid_msgsPoseSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsPoseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,28,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsPoseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsPoseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_rapid_msgsPosSize(value["trans"]));
n=addSize(n,Ros1Abb_rapid_msgsOrientSize(value["rot"]));
if(n>28)bad(90);return n;}
function Ros1Abb_rapid_msgsPoseWrite(value,w){
Ros1Abb_rapid_msgsPosWrite(value["trans"],w);
Ros1Abb_rapid_msgsOrientWrite(value["rot"],w);
}
function Ros1Abb_rapid_msgsPoseRead(r){const value={};
{
const decoded=Ros1Abb_rapid_msgsPosRead(r);
value["trans"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrientRead(r);
value["rot"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsPoseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trans"];
out["trans"]=Ros1Abb_rapid_msgsPosCapture(field);
}
{const field=value["rot"];
out["rot"]=Ros1Abb_rapid_msgsOrientCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_msgs/RobTarget codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsRobTargetCodec=Object.freeze({codecIdentity:"UWVMROS1_0abb3aee8b1303ccf9419f2774829d1231de3ac033e4b644d585f195972f09ce",maxWireBytes:68,minWireBytes:68,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trans":Ros1Abb_rapid_msgsPosCodec.defaultValue(),"rot":Ros1Abb_rapid_msgsOrientCodec.defaultValue(),"robconf":Ros1Abb_rapid_msgsConfDataCodec.defaultValue(),"extax":Ros1Abb_rapid_msgsExtJointCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_msgsRobTargetSize(Ros1Abb_rapid_msgsRobTargetCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsRobTargetCapture(value),n=Ros1Abb_rapid_msgsRobTargetSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsRobTargetWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<68)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsRobTargetRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsRobTargetSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_rapid_msgsPosSize(value["trans"]));
n=addSize(n,Ros1Abb_rapid_msgsOrientSize(value["rot"]));
n=addSize(n,Ros1Abb_rapid_msgsConfDataSize(value["robconf"]));
n=addSize(n,Ros1Abb_rapid_msgsExtJointSize(value["extax"]));
if(n>68)bad(90);return n;}
function Ros1Abb_rapid_msgsRobTargetWrite(value,w){
Ros1Abb_rapid_msgsPosWrite(value["trans"],w);
Ros1Abb_rapid_msgsOrientWrite(value["rot"],w);
Ros1Abb_rapid_msgsConfDataWrite(value["robconf"],w);
Ros1Abb_rapid_msgsExtJointWrite(value["extax"],w);
}
function Ros1Abb_rapid_msgsRobTargetRead(r){const value={};
{
const decoded=Ros1Abb_rapid_msgsPosRead(r);
value["trans"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrientRead(r);
value["rot"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsConfDataRead(r);
value["robconf"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsExtJointRead(r);
value["extax"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsRobTargetCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trans"];
out["trans"]=Ros1Abb_rapid_msgsPosCapture(field);
}
{const field=value["rot"];
out["rot"]=Ros1Abb_rapid_msgsOrientCapture(field);
}
{const field=value["robconf"];
out["robconf"]=Ros1Abb_rapid_msgsConfDataCapture(field);
}
{const field=value["extax"];
out["extax"]=Ros1Abb_rapid_msgsExtJointCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_msgs/ToolData codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsToolDataCodec=Object.freeze({codecIdentity:"UWVMROS1_e2feaee4bda891f594542384d70883184262777e359fb037a648b7526ef76cba",maxWireBytes:73,minWireBytes:73,cartesianMaxWireBytes:73n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robhold":false,"tframe":Ros1Abb_rapid_msgsPoseCodec.defaultValue(),"tload":Ros1Abb_rapid_msgsLoadDataCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_msgsToolDataSize(Ros1Abb_rapid_msgsToolDataCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsToolDataCapture(value),n=Ros1Abb_rapid_msgsToolDataSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsToolDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,73,90);if(owned.length<73)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsToolDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsToolDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["robhold"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["tframe"]));
n=addSize(n,Ros1Abb_rapid_msgsLoadDataSize(value["tload"]));
if(n>73)bad(90);return n;}
function Ros1Abb_rapid_msgsToolDataWrite(value,w){
w.v.setUint8(w.at,value["robhold"]?1:0,true);w.at+=1;
Ros1Abb_rapid_msgsPoseWrite(value["tframe"],w);
Ros1Abb_rapid_msgsLoadDataWrite(value["tload"],w);
}
function Ros1Abb_rapid_msgsToolDataRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["robhold"]=decoded!==0;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["tframe"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsLoadDataRead(r);
value["tload"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsToolDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robhold"];
out["robhold"]=field;
}
{const field=value["tframe"];
out["tframe"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
{const field=value["tload"];
out["tload"]=Ros1Abb_rapid_msgsLoadDataCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_msgs/WObjData codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_msgsWObjDataCodec=Object.freeze({codecIdentity:"UWVMROS1_6de4cf654a15ef3b63d9b0f3d0984438f21963c10ffd1b00238113b6fae8454f",maxWireBytes:574,minWireBytes:62,cartesianMaxWireBytes:574n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"robhold":false,"ufprog":false,"ufmec":"","uframe":Ros1Abb_rapid_msgsPoseCodec.defaultValue(),"oframe":Ros1Abb_rapid_msgsPoseCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_msgsWObjDataSize(Ros1Abb_rapid_msgsWObjDataCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_msgsWObjDataCapture(value),n=Ros1Abb_rapid_msgsWObjDataSize(owned),w=new Writer(n);Ros1Abb_rapid_msgsWObjDataWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,574,90);if(owned.length<62)bad();const r=new Reader(owned),value=Ros1Abb_rapid_msgsWObjDataRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_msgsWObjDataSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["robhold"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["ufprog"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,4+textBytes(value["ufmec"],512).length);
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["uframe"]));
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["oframe"]));
if(n>574)bad(90);return n;}
function Ros1Abb_rapid_msgsWObjDataWrite(value,w){
w.v.setUint8(w.at,value["robhold"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["ufprog"]?1:0,true);w.at+=1;
w.text(value["ufmec"],512);
Ros1Abb_rapid_msgsPoseWrite(value["uframe"],w);
Ros1Abb_rapid_msgsPoseWrite(value["oframe"],w);
}
function Ros1Abb_rapid_msgsWObjDataRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["robhold"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["ufprog"]=decoded!==0;
}
{
const decoded=r.text(512);
value["ufmec"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["uframe"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["oframe"]=decoded;
}
return value;}
function Ros1Abb_rapid_msgsWObjDataCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["robhold"];
out["robhold"]=field;
}
{const field=value["ufprog"];
out["ufprog"]=field;
}
{const field=value["ufmec"];
out["ufmec"]=field;
}
{const field=value["uframe"];
out["uframe"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
{const field=value["oframe"];
out["oframe"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMActivateSettings codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_c50be5682021e5d1a7a938e6fe3595cec7c0b0ae727070c0924da731c2c73d4d",maxWireBytes:716,minWireBytes:204,cartesianMaxWireBytes:716n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"tool":Ros1Abb_rapid_msgsToolDataCodec.defaultValue(),"wobj":Ros1Abb_rapid_msgsWObjDataCodec.defaultValue(),"correction_frame":Ros1Abb_rapid_msgsPoseCodec.defaultValue(),"sensor_frame":Ros1Abb_rapid_msgsPoseCodec.defaultValue(),"cond_min_max":0,"lp_filter":0,"sample_rate":0,"max_speed_deviation":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsSize(Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCapture(value),n=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,716,90);if(owned.length<204)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_rapid_msgsToolDataSize(value["tool"]));
n=addSize(n,Ros1Abb_rapid_msgsWObjDataSize(value["wobj"]));
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["correction_frame"]));
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["sensor_frame"]));
if(typeof value["cond_min_max"]!=="number"||!Number.isFinite(value["cond_min_max"])||!Number.isFinite(Math.fround(value["cond_min_max"])))bad();n=addSize(n,4);
if(typeof value["lp_filter"]!=="number"||!Number.isFinite(value["lp_filter"])||!Number.isFinite(Math.fround(value["lp_filter"])))bad();n=addSize(n,4);
integer(value["sample_rate"],1,false);n=addSize(n,1);
if(typeof value["max_speed_deviation"]!=="number"||!Number.isFinite(value["max_speed_deviation"])||!Number.isFinite(Math.fround(value["max_speed_deviation"])))bad();n=addSize(n,4);
if(n>716)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsWrite(value,w){
Ros1Abb_rapid_msgsToolDataWrite(value["tool"],w);
Ros1Abb_rapid_msgsWObjDataWrite(value["wobj"],w);
Ros1Abb_rapid_msgsPoseWrite(value["correction_frame"],w);
Ros1Abb_rapid_msgsPoseWrite(value["sensor_frame"],w);
w.v.setFloat32(w.at,value["cond_min_max"],true);w.at+=4;
w.v.setFloat32(w.at,value["lp_filter"],true);w.at+=4;
w.v.setUint8(w.at,value["sample_rate"],true);w.at+=1;
w.v.setFloat32(w.at,value["max_speed_deviation"],true);w.at+=4;
}
function Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsRead(r){const value={};
{
const decoded=Ros1Abb_rapid_msgsToolDataRead(r);
value["tool"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsWObjDataRead(r);
value["wobj"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["correction_frame"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["sensor_frame"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cond_min_max"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["lp_filter"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["sample_rate"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["max_speed_deviation"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["tool"];
out["tool"]=Ros1Abb_rapid_msgsToolDataCapture(field);
}
{const field=value["wobj"];
out["wobj"]=Ros1Abb_rapid_msgsWObjDataCapture(field);
}
{const field=value["correction_frame"];
out["correction_frame"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
{const field=value["sensor_frame"];
out["sensor_frame"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
{const field=value["cond_min_max"];
out["cond_min_max"]=field;
}
{const field=value["lp_filter"];
out["lp_filter"]=field;
}
{const field=value["sample_rate"];
out["sample_rate"]=field;
}
{const field=value["max_speed_deviation"];
out["max_speed_deviation"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMRunSettings codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_09a843da7aa89c60e6404f8d36b9a1a57f75e330329ac043449fd438b7029506",maxWireBytes:40,minWireBytes:40,cartesianMaxWireBytes:40n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"cond_time":0,"ramp_in_time":0,"offset":Ros1Abb_rapid_msgsPoseCodec.defaultValue(),"pos_corr_gain":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsSize(Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCapture(value),n=Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,40,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["cond_time"]!=="number"||!Number.isFinite(value["cond_time"])||!Number.isFinite(Math.fround(value["cond_time"])))bad();n=addSize(n,4);
if(typeof value["ramp_in_time"]!=="number"||!Number.isFinite(value["ramp_in_time"])||!Number.isFinite(Math.fround(value["ramp_in_time"])))bad();n=addSize(n,4);
n=addSize(n,Ros1Abb_rapid_msgsPoseSize(value["offset"]));
if(typeof value["pos_corr_gain"]!=="number"||!Number.isFinite(value["pos_corr_gain"])||!Number.isFinite(Math.fround(value["pos_corr_gain"])))bad();n=addSize(n,4);
if(n>40)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsWrite(value,w){
w.v.setFloat32(w.at,value["cond_time"],true);w.at+=4;
w.v.setFloat32(w.at,value["ramp_in_time"],true);w.at+=4;
Ros1Abb_rapid_msgsPoseWrite(value["offset"],w);
w.v.setFloat32(w.at,value["pos_corr_gain"],true);w.at+=4;
}
function Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["cond_time"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ramp_in_time"]=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPoseRead(r);
value["offset"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["pos_corr_gain"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["cond_time"];
out["cond_time"]=field;
}
{const field=value["ramp_in_time"];
out["ramp_in_time"]=field;
}
{const field=value["offset"];
out["offset"]=Ros1Abb_rapid_msgsPoseCapture(field);
}
{const field=value["pos_corr_gain"];
out["pos_corr_gain"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMSetupSettings codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_ece31a7631bb3bb248aece52d4d5f6e190a94dcc3052b2761b759bed24513779",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"use_filtering":false,"comm_timeout":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsSize(Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCapture(value),n=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["use_filtering"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["comm_timeout"]!=="number"||!Number.isFinite(value["comm_timeout"])||!Number.isFinite(Math.fround(value["comm_timeout"])))bad();n=addSize(n,4);
if(n>5)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsWrite(value,w){
w.v.setUint8(w.at,value["use_filtering"]?1:0,true);w.at+=1;
w.v.setFloat32(w.at,value["comm_timeout"],true);w.at+=4;
}
function Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_filtering"]=decoded!==0;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["comm_timeout"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["use_filtering"];
out["use_filtering"]=field;
}
{const field=value["comm_timeout"];
out["comm_timeout"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMStopSettings codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_64009cc452b9b2b12b3501d150e8c88c9cc07c796df06775195fa17c2a103a08",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"ramp_out_time":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsSize(Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCapture(value),n=Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["ramp_out_time"]!=="number"||!Number.isFinite(value["ramp_out_time"])||!Number.isFinite(Math.fround(value["ramp_out_time"])))bad();n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsWrite(value,w){
w.v.setFloat32(w.at,value["ramp_out_time"],true);w.at+=4;
}
function Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["ramp_out_time"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["ramp_out_time"];
out["ramp_out_time"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMSettings codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsEGMSettingsCodec=Object.freeze({codecIdentity:"UWVMROS1_ebc0ca24a54451f863964c74bf0257b5b25546d0d4b8a75ac75c265e1d62317b",maxWireBytes:767,minWireBytes:255,cartesianMaxWireBytes:767n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"allow_egm_motions":false,"use_presync":false,"setup_uc":Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCodec.defaultValue(),"activate":Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCodec.defaultValue(),"run":Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCodec.defaultValue(),"stop":Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsEGMSettingsSize(Ros1Abb_rapid_sm_addin_msgsEGMSettingsCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsEGMSettingsCapture(value),n=Ros1Abb_rapid_sm_addin_msgsEGMSettingsSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsEGMSettingsWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,767,90);if(owned.length<255)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsEGMSettingsRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsEGMSettingsSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["allow_egm_motions"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["use_presync"]!=="boolean")bad();n=addSize(n,1);
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsSize(value["setup_uc"]));
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsSize(value["activate"]));
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsSize(value["run"]));
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsSize(value["stop"]));
if(n>767)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsEGMSettingsWrite(value,w){
w.v.setUint8(w.at,value["allow_egm_motions"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["use_presync"]?1:0,true);w.at+=1;
Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsWrite(value["setup_uc"],w);
Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsWrite(value["activate"],w);
Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsWrite(value["run"],w);
Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsWrite(value["stop"],w);
}
function Ros1Abb_rapid_sm_addin_msgsEGMSettingsRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["allow_egm_motions"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["use_presync"]=decoded!==0;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsRead(r);
value["setup_uc"]=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsRead(r);
value["activate"]=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsRead(r);
value["run"]=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsRead(r);
value["stop"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsEGMSettingsCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["allow_egm_motions"];
out["allow_egm_motions"]=field;
}
{const field=value["use_presync"];
out["use_presync"]=field;
}
{const field=value["setup_uc"];
out["setup_uc"]=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCapture(field);
}
{const field=value["activate"];
out["activate"]=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCapture(field);
}
{const field=value["run"];
out["run"]=Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCapture(field);
}
{const field=value["stop"];
out["stop"]=Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsRequest codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_42cca857367e08720f989416d18d6a1f7d677f7e5bf49435b9735a9eb10a8f9c",maxWireBytes:68,minWireBytes:4,cartesianMaxWireBytes:68n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestSize(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCapture(value),n=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,68,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
if(n>68)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestWrite(value,w){
w.text(value["task"],64);
}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsResponse codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_094fd8b0b992c26d708bbd09952d36a3921ad5e5e66f45edb9d19af5ec3472bc",maxWireBytes:1285,minWireBytes:261,cartesianMaxWireBytes:1285n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"settings":Ros1Abb_rapid_sm_addin_msgsEGMSettingsCodec.defaultValue(),"result_code":0,"message":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseSize(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCapture(value),n=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1285,90);if(owned.length<261)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMSettingsSize(value["settings"]));
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1285)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseWrite(value,w){
Ros1Abb_rapid_sm_addin_msgsEGMSettingsWrite(value["settings"],w);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseRead(r){const value={};
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettingsRead(r);
value["settings"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["settings"];
out["settings"]=Ros1Abb_rapid_sm_addin_msgsEGMSettingsCapture(field);
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/StateMachineState codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsStateMachineStateCodec=Object.freeze({codecIdentity:"UWVMROS1_9ef8cc3a0438ad892acc1a2f485039ca49abce6fedf40cc5d9b2fe03dd9490bb",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({"SM_STATE_UNKNOWN":1,"SM_STATE_IDLE":2,"SM_STATE_INITIALIZE":3,"SM_STATE_RUN_RAPID_ROUTINE":4,"SM_STATE_RUN_EGM_ROUTINE":5,"EGM_ACTION_UNKNOWN":1,"EGM_ACTION_NONE":2,"EGM_ACTION_RUN_JOINT":3,"EGM_ACTION_RUN_POSE":4,"EGM_ACTION_STOP":5,"EGM_ACTION_START_STREAM":6,"EGM_ACTION_STOP_STREAM":7}),
defaultValue(){return {"rapid_task":"","sm_state":0,"egm_action":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsStateMachineStateSize(Ros1Abb_rapid_sm_addin_msgsStateMachineStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsStateMachineStateCapture(value),n=Ros1Abb_rapid_sm_addin_msgsStateMachineStateSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsStateMachineStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsStateMachineStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsStateMachineStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["rapid_task"],512).length);
integer(value["sm_state"],1,false);n=addSize(n,1);
integer(value["egm_action"],1,false);n=addSize(n,1);
if(n>518)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsStateMachineStateWrite(value,w){
w.text(value["rapid_task"],512);
w.v.setUint8(w.at,value["sm_state"],true);w.at+=1;
w.v.setUint8(w.at,value["egm_action"],true);w.at+=1;
}
function Ros1Abb_rapid_sm_addin_msgsStateMachineStateRead(r){const value={};
{
const decoded=r.text(512);
value["rapid_task"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["sm_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["egm_action"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsStateMachineStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["rapid_task"];
out["rapid_task"]=field;
}
{const field=value["sm_state"];
out["sm_state"]=field;
}
{const field=value["egm_action"];
out["egm_action"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/RuntimeState codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsRuntimeStateCodec=Object.freeze({codecIdentity:"UWVMROS1_50d7eab2eed3ce0b55160e85b44b4108d966290f6b8f126980ee452846f3660c",maxWireBytes:8372,minWireBytes:20,cartesianMaxWireBytes:8372n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"state_machines":[]};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsRuntimeStateSize(Ros1Abb_rapid_sm_addin_msgsRuntimeStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsRuntimeStateCapture(value),n=Ros1Abb_rapid_sm_addin_msgsRuntimeStateSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsRuntimeStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8372,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsRuntimeStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsRuntimeStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["state_machines"])||value["state_machines"].length>16)bad();
n=addSize(n,4);
for(const element of value["state_machines"]){
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsStateMachineStateSize(element));
}
if(n>8372)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsRuntimeStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["state_machines"].length,true);w.at+=4;
for(const element of value["state_machines"]){
Ros1Abb_rapid_sm_addin_msgsStateMachineStateWrite(element,w);
}
}
function Ros1Abb_rapid_sm_addin_msgsRuntimeStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>16)bad();value["state_machines"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Abb_rapid_sm_addin_msgsStateMachineStateRead(r);
value["state_machines"].push(decoded);
}}
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsRuntimeStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["state_machines"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Abb_rapid_sm_addin_msgsStateMachineStateCapture(element);
}out["state_machines"]=values;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsRequest codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_fe85c34f8dff2c0a547f89385d75771af13a03d997b880ab7be2eb84ea800155",maxWireBytes:835,minWireBytes:259,cartesianMaxWireBytes:835n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","settings":Ros1Abb_rapid_sm_addin_msgsEGMSettingsCodec.defaultValue()};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestSize(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,835,90);if(owned.length<259)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,Ros1Abb_rapid_sm_addin_msgsEGMSettingsSize(value["settings"]));
if(n>835)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestWrite(value,w){
w.text(value["task"],64);
Ros1Abb_rapid_sm_addin_msgsEGMSettingsWrite(value["settings"],w);
}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettingsRead(r);
value["settings"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["settings"];
out["settings"]=Ros1Abb_rapid_sm_addin_msgsEGMSettingsCapture(field);
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsResponse codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c0d4e594a51a6cdd245ad03b6e27bbdc298c5c9fbe26cc25fa3594deef9b2591",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseSize(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_b16ba3e98ee1d45864532b01f914746f041e49463b9330ef6056e427d199a5a3",maxWireBytes:584,minWireBytes:8,cartesianMaxWireBytes:584n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","routine":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestSize(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,584,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,4+textBytes(value["routine"],512).length);
if(n>584)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestWrite(value,w){
w.text(value["task"],64);
w.text(value["routine"],512);
}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.text(512);
value["routine"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["routine"];
out["routine"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_44de2385f12a8612fa75ca7c3f97b07f22d04e920bdeca0f0bd7fed5322c680e",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseSize(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetSGCommandRequest codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e39d7fee66c1efb9ea963216823f6e10bd632d78634d8c47f3283a1a1779f0c2",maxWireBytes:73,minWireBytes:9,cartesianMaxWireBytes:73n,aggregateWireBytes:0,constants:Object.freeze({"SG_COMMAND_UNKNOWN":1,"SG_COMMAND_NONE":2,"SG_COMMAND_INITIALIZE":3,"SG_COMMAND_CALIBRATE":4,"SG_COMMAND_MOVE_TO":5,"SG_COMMAND_GRIP_IN":6,"SG_COMMAND_GRIP_OUT":7,"SG_COMMAND_BLOW_ON_1":8,"SG_COMMAND_BLOW_ON_2":9,"SG_COMMAND_BLOW_OFF_1":10,"SG_COMMAND_BLOW_OFF_2":11,"SG_COMMAND_VACUUM_ON_1":12,"SG_COMMAND_VACUUM_ON_2":13,"SG_COMMAND_VACUUM_OFF_1":14,"SG_COMMAND_VACUUM_OFF_2":15}),
defaultValue(){return {"task":"","command":0,"target_position":0};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestSize(Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,73,90);if(owned.length<9)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
integer(value["command"],1,false);n=addSize(n,1);
if(typeof value["target_position"]!=="number"||!Number.isFinite(value["target_position"])||!Number.isFinite(Math.fround(value["target_position"])))bad();n=addSize(n,4);
if(n>73)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestWrite(value,w){
w.text(value["task"],64);
w.v.setUint8(w.at,value["command"],true);w.at+=1;
w.v.setFloat32(w.at,value["target_position"],true);w.at+=4;
}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["command"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["target_position"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["command"];
out["command"]=field;
}
{const field=value["target_position"];
out["target_position"]=field;
}
return out;}
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetSGCommandResponse codec; maxima are not actual lengths. */
export const Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_2f22b2098d86dd0764277247eb7f64dccecebf0cddce85654b01c1928eb9c192",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseSize(Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCapture(value),n=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseSize(owned),w=new Writer(n);Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetFileContentsRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetFileContentsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_75f5802b385cc1efb524fadea657fda1d5cc4298c5720222d33cd3dd39656f21",maxWireBytes:516,minWireBytes:4,cartesianMaxWireBytes:516n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":""};},
validateValue(value){Ros1Abb_robot_msgsGetFileContentsRequestSize(Ros1Abb_robot_msgsGetFileContentsRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetFileContentsRequestCapture(value),n=Ros1Abb_robot_msgsGetFileContentsRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetFileContentsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,516,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetFileContentsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetFileContentsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],512).length);
if(n>516)bad(90);return n;}
function Ros1Abb_robot_msgsGetFileContentsRequestWrite(value,w){
w.text(value["filename"],512);
}
function Ros1Abb_robot_msgsGetFileContentsRequestRead(r){const value={};
{
const decoded=r.text(512);
value["filename"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetFileContentsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetFileContentsResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetFileContentsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_693b53ed168f81a7c93ac8083977c4f8bd6f4a3999d079f5b47029699ba68fdb",maxWireBytes:4618,minWireBytes:10,cartesianMaxWireBytes:4618n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"contents":"","result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetFileContentsResponseSize(Ros1Abb_robot_msgsGetFileContentsResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetFileContentsResponseCapture(value),n=Ros1Abb_robot_msgsGetFileContentsResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetFileContentsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4618,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetFileContentsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetFileContentsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["contents"],4096).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>4618)bad(90);return n;}
function Ros1Abb_robot_msgsGetFileContentsResponseWrite(value,w){
w.text(value["contents"],4096);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetFileContentsResponseRead(r){const value={};
{
const decoded=r.text(4096);
value["contents"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetFileContentsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["contents"];
out["contents"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetIOSignalRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetIOSignalRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d98f7577756b476cac0dd325d45f333fcd2b735df7dce58fb660790b5a91f83b",maxWireBytes:516,minWireBytes:4,cartesianMaxWireBytes:516n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"signal":""};},
validateValue(value){Ros1Abb_robot_msgsGetIOSignalRequestSize(Ros1Abb_robot_msgsGetIOSignalRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetIOSignalRequestCapture(value),n=Ros1Abb_robot_msgsGetIOSignalRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetIOSignalRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,516,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetIOSignalRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetIOSignalRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["signal"],512).length);
if(n>516)bad(90);return n;}
function Ros1Abb_robot_msgsGetIOSignalRequestWrite(value,w){
w.text(value["signal"],512);
}
function Ros1Abb_robot_msgsGetIOSignalRequestRead(r){const value={};
{
const decoded=r.text(512);
value["signal"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetIOSignalRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["signal"];
out["signal"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetIOSignalResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetIOSignalResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_10a66146c4815b0d758ec81dde1c552695c97a8727ee995299018b23f129d79e",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetIOSignalResponseSize(Ros1Abb_robot_msgsGetIOSignalResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetIOSignalResponseCapture(value),n=Ros1Abb_robot_msgsGetIOSignalResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetIOSignalResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetIOSignalResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetIOSignalResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros1Abb_robot_msgsGetIOSignalResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetIOSignalResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetIOSignalResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/RAPIDSymbolPath codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsRAPIDSymbolPathCodec=Object.freeze({codecIdentity:"UWVMROS1_07308d9ce1e71a02f5cc3546cdbaaa0b24a06534a55aa3ee3354b498038faad2",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"task":"","module":"","symbol":""};},
validateValue(value){Ros1Abb_robot_msgsRAPIDSymbolPathSize(Ros1Abb_robot_msgsRAPIDSymbolPathCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(value),n=Ros1Abb_robot_msgsRAPIDSymbolPathSize(owned),w=new Writer(n);Ros1Abb_robot_msgsRAPIDSymbolPathWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsRAPIDSymbolPathSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["task"],64).length);
n=addSize(n,4+textBytes(value["module"],64).length);
n=addSize(n,4+textBytes(value["symbol"],64).length);
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value,w){
w.text(value["task"],64);
w.text(value["module"],64);
w.text(value["symbol"],64);
}
function Ros1Abb_robot_msgsRAPIDSymbolPathRead(r){const value={};
{
const decoded=r.text(64);
value["task"]=decoded;
}
{
const decoded=r.text(64);
value["module"]=decoded;
}
{
const decoded=r.text(64);
value["symbol"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsRAPIDSymbolPathCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["task"];
out["task"]=field;
}
{const field=value["module"];
out["module"]=field;
}
{const field=value["symbol"];
out["symbol"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDBoolRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDBoolRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_46e49c0d6796175d0c56460dd4c7d2557deaa8db4f1a3bec40b53636d3b44b96",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDBoolRequestSize(Ros1Abb_robot_msgsGetRAPIDBoolRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDBoolRequestCapture(value),n=Ros1Abb_robot_msgsGetRAPIDBoolRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDBoolRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDBoolRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDBoolRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDBoolRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
}
function Ros1Abb_robot_msgsGetRAPIDBoolRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDBoolRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDBoolResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDBoolResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_bfe21e95cb79f1d9bc1bc1dc2f61c744a2a7dc49e5a5ed07b6c8619344b97b3f",maxWireBytes:519,minWireBytes:7,cartesianMaxWireBytes:519n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":false,"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDBoolResponseSize(Ros1Abb_robot_msgsGetRAPIDBoolResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDBoolResponseCapture(value),n=Ros1Abb_robot_msgsGetRAPIDBoolResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDBoolResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,519,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDBoolResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDBoolResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>519)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDBoolResponseWrite(value,w){
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRAPIDBoolResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDBoolResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDDnumRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDDnumRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9058d8774de0da657344a00d75a7f75878da2b0453bf25708bc8ed7f45c90c9d",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDDnumRequestSize(Ros1Abb_robot_msgsGetRAPIDDnumRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDDnumRequestCapture(value),n=Ros1Abb_robot_msgsGetRAPIDDnumRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDDnumRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDDnumRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDDnumRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDDnumRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
}
function Ros1Abb_robot_msgsGetRAPIDDnumRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDDnumRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDDnumResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDDnumResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9c209b4b12d733e33b76cc33ae93b6d561ce1eb7f8864429a56bd073317a01b3",maxWireBytes:526,minWireBytes:14,cartesianMaxWireBytes:526n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0,"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDDnumResponseSize(Ros1Abb_robot_msgsGetRAPIDDnumResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDDnumResponseCapture(value),n=Ros1Abb_robot_msgsGetRAPIDDnumResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDDnumResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,526,90);if(owned.length<14)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDDnumResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDDnumResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"]))bad();n=addSize(n,8);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>526)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDDnumResponseWrite(value,w){
w.v.setFloat64(w.at,value["value"],true);w.at+=8;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRAPIDDnumResponseRead(r){const value={};
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDDnumResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDNumRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDNumRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a316395c8a19b7c0a9c3a555e625fffc7d647c7b567f38c53b1c3735b50ee910",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDNumRequestSize(Ros1Abb_robot_msgsGetRAPIDNumRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDNumRequestCapture(value),n=Ros1Abb_robot_msgsGetRAPIDNumRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDNumRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDNumRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDNumRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDNumRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
}
function Ros1Abb_robot_msgsGetRAPIDNumRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDNumRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDNumResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDNumResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1573cba9a9046a6b304a49e3a77266fa0d6f615d2ea9a24692c25616970fd9b0",maxWireBytes:522,minWireBytes:10,cartesianMaxWireBytes:522n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":0,"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDNumResponseSize(Ros1Abb_robot_msgsGetRAPIDNumResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDNumResponseCapture(value),n=Ros1Abb_robot_msgsGetRAPIDNumResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDNumResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,522,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDNumResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDNumResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>522)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDNumResponseWrite(value,w){
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRAPIDNumResponseRead(r){const value={};
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDNumResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDStringRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDStringRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_56514142e9e064aa43d09078b49ed8ee0366189727c35a9bc130b35007da4c9c",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDStringRequestSize(Ros1Abb_robot_msgsGetRAPIDStringRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDStringRequestCapture(value),n=Ros1Abb_robot_msgsGetRAPIDStringRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDStringRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDStringRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDStringRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDStringRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
}
function Ros1Abb_robot_msgsGetRAPIDStringRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDStringRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDStringResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDStringResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_d1188827af3c307a721f8128b6f14fa6020f3187afce8a4162ce6562cdd1c854",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDStringResponseSize(Ros1Abb_robot_msgsGetRAPIDStringResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDStringResponseCapture(value),n=Ros1Abb_robot_msgsGetRAPIDStringResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDStringResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDStringResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDStringResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDStringResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRAPIDStringResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDStringResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDSymbolRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDSymbolRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_31302ad912a8ff39b56a429bc9dae22cbfc2c2101b8cd05162d00d9206aecf5b",maxWireBytes:204,minWireBytes:12,cartesianMaxWireBytes:204n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue()};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDSymbolRequestSize(Ros1Abb_robot_msgsGetRAPIDSymbolRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDSymbolRequestCapture(value),n=Ros1Abb_robot_msgsGetRAPIDSymbolRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDSymbolRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,204,90);if(owned.length<12)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDSymbolRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDSymbolRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(n>204)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDSymbolRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
}
function Ros1Abb_robot_msgsGetRAPIDSymbolRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDSymbolRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRAPIDSymbolResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRAPIDSymbolResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_182dc465ea5fb9543791b6fb76520313cee1ed472ab0296c8ef7a9ed3d4d110f",maxWireBytes:1034,minWireBytes:10,cartesianMaxWireBytes:1034n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"value":"","result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRAPIDSymbolResponseSize(Ros1Abb_robot_msgsGetRAPIDSymbolResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRAPIDSymbolResponseCapture(value),n=Ros1Abb_robot_msgsGetRAPIDSymbolResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRAPIDSymbolResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1034,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRAPIDSymbolResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRAPIDSymbolResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["value"],512).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>1034)bad(90);return n;}
function Ros1Abb_robot_msgsGetRAPIDSymbolResponseWrite(value,w){
w.text(value["value"],512);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRAPIDSymbolResponseRead(r){const value={};
{
const decoded=r.text(512);
value["value"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRAPIDSymbolResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["value"];
out["value"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetRobotControllerDescriptionRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_bc5f87f4d0f33cbc640c8188fbb15c9b0f43b3067f2406700d6f5bd2475089b6",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestSize(Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCapture(value),n=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestWrite(value,w){
}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestRead(r){const value={};
return value;}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 abb_robot_msgs/GetRobotControllerDescriptionResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7db41283fbe12a010e526b87d77a5bb7faecdf85595ef2dc13f14bfed56b6e39",maxWireBytes:4618,minWireBytes:10,cartesianMaxWireBytes:4618n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"description":"","result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseSize(Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCapture(value),n=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4618,90);if(owned.length<10)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["description"],4096).length);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>4618)bad(90);return n;}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseWrite(value,w){
w.text(value["description"],4096);
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseRead(r){const value={};
{
const decoded=r.text(4096);
value["description"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["description"];
out["description"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/GetSpeedRatioRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetSpeedRatioRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_82230aebf6fa923e09bcd413ecb3b4803a6f1cfd3e8334e0ce0c496b73008373",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Abb_robot_msgsGetSpeedRatioRequestSize(Ros1Abb_robot_msgsGetSpeedRatioRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetSpeedRatioRequestCapture(value),n=Ros1Abb_robot_msgsGetSpeedRatioRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetSpeedRatioRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetSpeedRatioRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetSpeedRatioRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Abb_robot_msgsGetSpeedRatioRequestWrite(value,w){
}
function Ros1Abb_robot_msgsGetSpeedRatioRequestRead(r){const value={};
return value;}
function Ros1Abb_robot_msgsGetSpeedRatioRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 abb_robot_msgs/GetSpeedRatioResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsGetSpeedRatioResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_7784360ddafa1b8ad9626fde0afa2b2335f6a294507036c3aee953df99e91940",maxWireBytes:519,minWireBytes:7,cartesianMaxWireBytes:519n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_ratio":0,"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsGetSpeedRatioResponseSize(Ros1Abb_robot_msgsGetSpeedRatioResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsGetSpeedRatioResponseCapture(value),n=Ros1Abb_robot_msgsGetSpeedRatioResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsGetSpeedRatioResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,519,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsGetSpeedRatioResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsGetSpeedRatioResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["speed_ratio"],1,false);n=addSize(n,1);
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>519)bad(90);return n;}
function Ros1Abb_robot_msgsGetSpeedRatioResponseWrite(value,w){
w.v.setUint8(w.at,value["speed_ratio"],true);w.at+=1;
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsGetSpeedRatioResponseRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["speed_ratio"]=decoded;
}
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsGetSpeedRatioResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_ratio"];
out["speed_ratio"]=field;
}
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/MechanicalUnitState codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsMechanicalUnitStateCodec=Object.freeze({codecIdentity:"UWVMROS1_872cc6e8bdf32426bb9e24322c951452aab6ebcbc1f2998f4fb19c34abece8fa",maxWireBytes:69,minWireBytes:5,cartesianMaxWireBytes:69n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","activated":false};},
validateValue(value){Ros1Abb_robot_msgsMechanicalUnitStateSize(Ros1Abb_robot_msgsMechanicalUnitStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsMechanicalUnitStateCapture(value),n=Ros1Abb_robot_msgsMechanicalUnitStateSize(owned),w=new Writer(n);Ros1Abb_robot_msgsMechanicalUnitStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,69,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsMechanicalUnitStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsMechanicalUnitStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["activated"]!=="boolean")bad();n=addSize(n,1);
if(n>69)bad(90);return n;}
function Ros1Abb_robot_msgsMechanicalUnitStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["activated"]?1:0,true);w.at+=1;
}
function Ros1Abb_robot_msgsMechanicalUnitStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["activated"]=decoded!==0;
}
return value;}
function Ros1Abb_robot_msgsMechanicalUnitStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["activated"];
out["activated"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/RAPIDTaskState codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsRAPIDTaskStateCodec=Object.freeze({codecIdentity:"UWVMROS1_ab1245cc352efa91cdcc8de801392857e25a4101a141439554b761525b31b947",maxWireBytes:71,minWireBytes:7,cartesianMaxWireBytes:71n,aggregateWireBytes:0,constants:Object.freeze({"EXECUTION_STATE_UNKNOWN":1,"EXECUTION_STATE_READY":2,"EXECUTION_STATE_STOPPED":3,"EXECUTION_STATE_STARTED":4,"EXECUTION_STATE_UNINITIALIZED":5}),
defaultValue(){return {"name":"","activated":false,"execution_state":0,"motion_task":false};},
validateValue(value){Ros1Abb_robot_msgsRAPIDTaskStateSize(Ros1Abb_robot_msgsRAPIDTaskStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsRAPIDTaskStateCapture(value),n=Ros1Abb_robot_msgsRAPIDTaskStateSize(owned),w=new Writer(n);Ros1Abb_robot_msgsRAPIDTaskStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,71,90);if(owned.length<7)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsRAPIDTaskStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsRAPIDTaskStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["activated"]!=="boolean")bad();n=addSize(n,1);
integer(value["execution_state"],1,false);n=addSize(n,1);
if(typeof value["motion_task"]!=="boolean")bad();n=addSize(n,1);
if(n>71)bad(90);return n;}
function Ros1Abb_robot_msgsRAPIDTaskStateWrite(value,w){
w.text(value["name"],64);
w.v.setUint8(w.at,value["activated"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["execution_state"],true);w.at+=1;
w.v.setUint8(w.at,value["motion_task"]?1:0,true);w.at+=1;
}
function Ros1Abb_robot_msgsRAPIDTaskStateRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["activated"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
value["execution_state"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motion_task"]=decoded!==0;
}
return value;}
function Ros1Abb_robot_msgsRAPIDTaskStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["activated"];
out["activated"]=field;
}
{const field=value["execution_state"];
out["execution_state"]=field;
}
{const field=value["motion_task"];
out["motion_task"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/ServiceResponses codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsServiceResponsesCodec=Object.freeze({codecIdentity:"UWVMROS1_f0a3c4b160bf0ca52dbbb4e26d2c8a7d3c6a40ac70787ba59cf181945e4276d4",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESS":"","RC_SUCCESS":1,"FAILED":"Service failed (e.g. timeout or resource not found, see the DEBUG log for details)","RC_FAILED":2,"SERVER_IS_BUSY":"Server is busy","RC_SERVER_IS_BUSY":1001,"EMPTY_FILENAME":"Filename is empty","RC_EMPTY_FILENAME":2001,"EMPTY_SIGNAL_NAME":"Signal name is empty","RC_EMPTY_SIGNAL_NAME":2002,"EMPTY_RAPID_TASK_NAME":"RAPID task name is empty","RC_EMPTY_RAPID_TASK_NAME":2003,"EMPTY_RAPID_MODULE_NAME":"RAPID module name is empty","RC_EMPTY_RAPID_MODULE_NAME":2004,"EMPTY_RAPID_SYMBOL_NAME":"RAPID symbol name is empty","RC_EMPTY_RAPID_SYMBOL_NAME":2005,"NOT_IN_AUTO_MODE":"Robot controller is not in AUTO mode","RC_NOT_IN_AUTO_MODE":3001,"MOTORS_ARE_OFF":"Motors are off","RC_MOTORS_ARE_OFF":3002,"MOTORS_ARE_ON":"Motors are on","RC_MOTORS_ARE_ON":3003,"RAPID_NOT_STOPPED":"RAPID has not been stopped","RC_RAPID_NOT_STOPPED":3004,"RAPID_NOT_RUNNING":"RAPID is not running","RC_RAPID_NOT_RUNNING":3005,"SM_RUNTIME_STATES_MISSING":"No runtime states received for any StateMachine Add-In instance","RC_SM_RUNTIME_STATES_MISSING":4001,"SM_UNKNOWN_RAPID_TASK":"RAPID task not found among known StateMachine Add-In instances","RC_SM_UNKNOWN_RAPID_TASK":4002,"SM_UNINITIALIZED":"StateMachine Add-In instance has not been initialized","RC_SM_UNINITIALIZED":4003}),
defaultValue(){return {};},
validateValue(value){Ros1Abb_robot_msgsServiceResponsesSize(Ros1Abb_robot_msgsServiceResponsesCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsServiceResponsesCapture(value),n=Ros1Abb_robot_msgsServiceResponsesSize(owned),w=new Writer(n);Ros1Abb_robot_msgsServiceResponsesWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsServiceResponsesRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsServiceResponsesSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Abb_robot_msgsServiceResponsesWrite(value,w){
}
function Ros1Abb_robot_msgsServiceResponsesRead(r){const value={};
return value;}
function Ros1Abb_robot_msgsServiceResponsesCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 abb_robot_msgs/SetFileContentsRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetFileContentsRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_f764448d9754e9ebc5e5fab64f0c8ebb01c534ec7b95751ed7d58e8ce0bd3012",maxWireBytes:4616,minWireBytes:8,cartesianMaxWireBytes:4616n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"filename":"","contents":""};},
validateValue(value){Ros1Abb_robot_msgsSetFileContentsRequestSize(Ros1Abb_robot_msgsSetFileContentsRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetFileContentsRequestCapture(value),n=Ros1Abb_robot_msgsSetFileContentsRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetFileContentsRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4616,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetFileContentsRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetFileContentsRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["filename"],512).length);
n=addSize(n,4+textBytes(value["contents"],4096).length);
if(n>4616)bad(90);return n;}
function Ros1Abb_robot_msgsSetFileContentsRequestWrite(value,w){
w.text(value["filename"],512);
w.text(value["contents"],4096);
}
function Ros1Abb_robot_msgsSetFileContentsRequestRead(r){const value={};
{
const decoded=r.text(512);
value["filename"]=decoded;
}
{
const decoded=r.text(4096);
value["contents"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetFileContentsRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["filename"];
out["filename"]=field;
}
{const field=value["contents"];
out["contents"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetFileContentsResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetFileContentsResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_3de2a08b8e158e0612898880d6d9c98f1995828eb53bd5639b47f730c6cbefd0",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetFileContentsResponseSize(Ros1Abb_robot_msgsSetFileContentsResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetFileContentsResponseCapture(value),n=Ros1Abb_robot_msgsSetFileContentsResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetFileContentsResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetFileContentsResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetFileContentsResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetFileContentsResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetFileContentsResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetFileContentsResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetIOSignalRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetIOSignalRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_57a0197e2a5de7efd2fc91d945d699be24559baec6651179a1256f1d9d35bbc9",maxWireBytes:1032,minWireBytes:8,cartesianMaxWireBytes:1032n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"signal":"","value":""};},
validateValue(value){Ros1Abb_robot_msgsSetIOSignalRequestSize(Ros1Abb_robot_msgsSetIOSignalRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetIOSignalRequestCapture(value),n=Ros1Abb_robot_msgsSetIOSignalRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetIOSignalRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1032,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetIOSignalRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetIOSignalRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["signal"],512).length);
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>1032)bad(90);return n;}
function Ros1Abb_robot_msgsSetIOSignalRequestWrite(value,w){
w.text(value["signal"],512);
w.text(value["value"],512);
}
function Ros1Abb_robot_msgsSetIOSignalRequestRead(r){const value={};
{
const decoded=r.text(512);
value["signal"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetIOSignalRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["signal"];
out["signal"]=field;
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetIOSignalResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetIOSignalResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_becc0f844ce6a7fabd8a683175edd8033ba5c943c2de152991755ee112b460e5",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetIOSignalResponseSize(Ros1Abb_robot_msgsSetIOSignalResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetIOSignalResponseCapture(value),n=Ros1Abb_robot_msgsSetIOSignalResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetIOSignalResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetIOSignalResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetIOSignalResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetIOSignalResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetIOSignalResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetIOSignalResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDBoolRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDBoolRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_d8e5420678fe5feeffa20ad9ab8431678ec346f764c94871c4496c62e9dd8e8d",maxWireBytes:205,minWireBytes:13,cartesianMaxWireBytes:205n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue(),"value":false};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDBoolRequestSize(Ros1Abb_robot_msgsSetRAPIDBoolRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDBoolRequestCapture(value),n=Ros1Abb_robot_msgsSetRAPIDBoolRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDBoolRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,205,90);if(owned.length<13)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDBoolRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDBoolRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="boolean")bad();n=addSize(n,1);
if(n>205)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDBoolRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
w.v.setUint8(w.at,value["value"]?1:0,true);w.at+=1;
}
function Ros1Abb_robot_msgsSetRAPIDBoolRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["value"]=decoded!==0;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDBoolRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDBoolResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDBoolResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_da2ae1231cb0c1ec452fb2733d5c99dcae78ad0bfd10771241708931b271bf13",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDBoolResponseSize(Ros1Abb_robot_msgsSetRAPIDBoolResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDBoolResponseCapture(value),n=Ros1Abb_robot_msgsSetRAPIDBoolResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDBoolResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDBoolResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDBoolResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDBoolResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetRAPIDBoolResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDBoolResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDDnumRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDDnumRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0f17e3068f86bca6effb99f3bc850234b0163028f372340c450fce31657b9cf9",maxWireBytes:212,minWireBytes:20,cartesianMaxWireBytes:212n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue(),"value":0};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDDnumRequestSize(Ros1Abb_robot_msgsSetRAPIDDnumRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDDnumRequestCapture(value),n=Ros1Abb_robot_msgsSetRAPIDDnumRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDDnumRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,212,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDDnumRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDDnumRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"]))bad();n=addSize(n,8);
if(n>212)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDDnumRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
w.v.setFloat64(w.at,value["value"],true);w.at+=8;
}
function Ros1Abb_robot_msgsSetRAPIDDnumRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDDnumRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDDnumResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDDnumResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_1905aed9683e03494baa54627ddbaf4aed0b92357823c0c042f9b7c6a970c8ed",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDDnumResponseSize(Ros1Abb_robot_msgsSetRAPIDDnumResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDDnumResponseCapture(value),n=Ros1Abb_robot_msgsSetRAPIDDnumResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDDnumResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDDnumResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDDnumResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDDnumResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetRAPIDDnumResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDDnumResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDNumRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDNumRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_9982d02cb0b6ee5813f5fb38072d665226b7885aeb90517af881f4fc990f392f",maxWireBytes:208,minWireBytes:16,cartesianMaxWireBytes:208n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue(),"value":0};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDNumRequestSize(Ros1Abb_robot_msgsSetRAPIDNumRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDNumRequestCapture(value),n=Ros1Abb_robot_msgsSetRAPIDNumRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDNumRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,208,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDNumRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDNumRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
if(typeof value["value"]!=="number"||!Number.isFinite(value["value"])||!Number.isFinite(Math.fround(value["value"])))bad();n=addSize(n,4);
if(n>208)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDNumRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
w.v.setFloat32(w.at,value["value"],true);w.at+=4;
}
function Ros1Abb_robot_msgsSetRAPIDNumRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.v.getFloat32(r.take(4),true);
if(!Number.isFinite(decoded))bad();
value["value"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDNumRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDNumResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDNumResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_af1181242b4afdb6d387d39c24d0bc343e872fb9ab21c0ec1e451785786a4e64",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDNumResponseSize(Ros1Abb_robot_msgsSetRAPIDNumResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDNumResponseCapture(value),n=Ros1Abb_robot_msgsSetRAPIDNumResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDNumResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDNumResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDNumResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDNumResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetRAPIDNumResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDNumResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDStringRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDStringRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_e06839324f8abfa8780b683386f87bab1ec04f1e6463e63c0599f5c2f8a75f0c",maxWireBytes:720,minWireBytes:16,cartesianMaxWireBytes:720n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue(),"value":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDStringRequestSize(Ros1Abb_robot_msgsSetRAPIDStringRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDStringRequestCapture(value),n=Ros1Abb_robot_msgsSetRAPIDStringRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDStringRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,720,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDStringRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDStringRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>720)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDStringRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
w.text(value["value"],512);
}
function Ros1Abb_robot_msgsSetRAPIDStringRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDStringRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDStringResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDStringResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_e175bdff5a150722b8c50cf2e1c0f28a99289a0b148339efdee4b034ea3b96b4",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDStringResponseSize(Ros1Abb_robot_msgsSetRAPIDStringResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDStringResponseCapture(value),n=Ros1Abb_robot_msgsSetRAPIDStringResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDStringResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDStringResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDStringResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDStringResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetRAPIDStringResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDStringResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDSymbolRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDSymbolRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_0654a5a8e5ec42f4feab54b086a008e02a74d44df65d2fcf60f007736e3fec7d",maxWireBytes:720,minWireBytes:16,cartesianMaxWireBytes:720n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"path":Ros1Abb_robot_msgsRAPIDSymbolPathCodec.defaultValue(),"value":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDSymbolRequestSize(Ros1Abb_robot_msgsSetRAPIDSymbolRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDSymbolRequestCapture(value),n=Ros1Abb_robot_msgsSetRAPIDSymbolRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDSymbolRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,720,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDSymbolRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDSymbolRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Abb_robot_msgsRAPIDSymbolPathSize(value["path"]));
n=addSize(n,4+textBytes(value["value"],512).length);
if(n>720)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDSymbolRequestWrite(value,w){
Ros1Abb_robot_msgsRAPIDSymbolPathWrite(value["path"],w);
w.text(value["value"],512);
}
function Ros1Abb_robot_msgsSetRAPIDSymbolRequestRead(r){const value={};
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPathRead(r);
value["path"]=decoded;
}
{
const decoded=r.text(512);
value["value"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDSymbolRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["path"];
out["path"]=Ros1Abb_robot_msgsRAPIDSymbolPathCapture(field);
}
{const field=value["value"];
out["value"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetRAPIDSymbolResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetRAPIDSymbolResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_a4ca0a8821337d1f16ba1f2ad2b232aceaba419308ca03b27f43dac427a32846",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetRAPIDSymbolResponseSize(Ros1Abb_robot_msgsSetRAPIDSymbolResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetRAPIDSymbolResponseCapture(value),n=Ros1Abb_robot_msgsSetRAPIDSymbolResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetRAPIDSymbolResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetRAPIDSymbolResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetRAPIDSymbolResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetRAPIDSymbolResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetRAPIDSymbolResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetRAPIDSymbolResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetSpeedRatioRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetSpeedRatioRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_a6ac368d243d488901b03d5e4303600d5f0eaf62150801103e5f5f9c0863fab5",maxWireBytes:1,minWireBytes:1,cartesianMaxWireBytes:1n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"speed_ratio":0};},
validateValue(value){Ros1Abb_robot_msgsSetSpeedRatioRequestSize(Ros1Abb_robot_msgsSetSpeedRatioRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetSpeedRatioRequestCapture(value),n=Ros1Abb_robot_msgsSetSpeedRatioRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetSpeedRatioRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1,90);if(owned.length<1)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetSpeedRatioRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetSpeedRatioRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["speed_ratio"],1,false);n=addSize(n,1);
if(n>1)bad(90);return n;}
function Ros1Abb_robot_msgsSetSpeedRatioRequestWrite(value,w){
w.v.setUint8(w.at,value["speed_ratio"],true);w.at+=1;
}
function Ros1Abb_robot_msgsSetSpeedRatioRequestRead(r){const value={};
{
const decoded=r.v.getUint8(r.take(1),true);
value["speed_ratio"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetSpeedRatioRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["speed_ratio"];
out["speed_ratio"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SetSpeedRatioResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSetSpeedRatioResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_9775dc1029993d1dfdf5f3080e6ae4262177ba96cdc0fbe9851dc4fea16c70d8",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsSetSpeedRatioResponseSize(Ros1Abb_robot_msgsSetSpeedRatioResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSetSpeedRatioResponseCapture(value),n=Ros1Abb_robot_msgsSetSpeedRatioResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSetSpeedRatioResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSetSpeedRatioResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSetSpeedRatioResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsSetSpeedRatioResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsSetSpeedRatioResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsSetSpeedRatioResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 abb_robot_msgs/SystemState codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsSystemStateCodec=Object.freeze({codecIdentity:"UWVMROS1_b57aa1fdf098aa78c51a96bd2978e02f97cbae67148c0028557d2a3e1e8630f2",maxWireBytes:2331,minWireBytes:27,cartesianMaxWireBytes:2331n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"motors_on":false,"auto_mode":false,"rapid_running":false,"rapid_tasks":[],"mechanical_units":[]};},
validateValue(value){Ros1Abb_robot_msgsSystemStateSize(Ros1Abb_robot_msgsSystemStateCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsSystemStateCapture(value),n=Ros1Abb_robot_msgsSystemStateSize(owned),w=new Writer(n);Ros1Abb_robot_msgsSystemStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2331,90);if(owned.length<27)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsSystemStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsSystemStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(typeof value["motors_on"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["auto_mode"]!=="boolean")bad();n=addSize(n,1);
if(typeof value["rapid_running"]!=="boolean")bad();n=addSize(n,1);
if(!Array.isArray(value["rapid_tasks"])||value["rapid_tasks"].length>16)bad();
n=addSize(n,4);
for(const element of value["rapid_tasks"]){
n=addSize(n,Ros1Abb_robot_msgsRAPIDTaskStateSize(element));
}
if(!Array.isArray(value["mechanical_units"])||value["mechanical_units"].length>16)bad();
n=addSize(n,4);
for(const element of value["mechanical_units"]){
n=addSize(n,Ros1Abb_robot_msgsMechanicalUnitStateSize(element));
}
if(n>2331)bad(90);return n;}
function Ros1Abb_robot_msgsSystemStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint8(w.at,value["motors_on"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["auto_mode"]?1:0,true);w.at+=1;
w.v.setUint8(w.at,value["rapid_running"]?1:0,true);w.at+=1;
w.v.setUint32(w.at,value["rapid_tasks"].length,true);w.at+=4;
for(const element of value["rapid_tasks"]){
Ros1Abb_robot_msgsRAPIDTaskStateWrite(element,w);
}
w.v.setUint32(w.at,value["mechanical_units"].length,true);w.at+=4;
for(const element of value["mechanical_units"]){
Ros1Abb_robot_msgsMechanicalUnitStateWrite(element,w);
}
}
function Ros1Abb_robot_msgsSystemStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["motors_on"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["auto_mode"]=decoded!==0;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["rapid_running"]=decoded!==0;
}
{
{const count=r.u32();if(count>16)bad();value["rapid_tasks"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Abb_robot_msgsRAPIDTaskStateRead(r);
value["rapid_tasks"].push(decoded);
}}
}
{
{const count=r.u32();if(count>16)bad();value["mechanical_units"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Abb_robot_msgsMechanicalUnitStateRead(r);
value["mechanical_units"].push(decoded);
}}
}
return value;}
function Ros1Abb_robot_msgsSystemStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["motors_on"];
out["motors_on"]=field;
}
{const field=value["auto_mode"];
out["auto_mode"]=field;
}
{const field=value["rapid_running"];
out["rapid_running"]=field;
}
{const field=value["rapid_tasks"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Abb_robot_msgsRAPIDTaskStateCapture(element);
}out["rapid_tasks"]=values;
}
{const field=value["mechanical_units"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>16)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Abb_robot_msgsMechanicalUnitStateCapture(element);
}out["mechanical_units"]=values;
}
return out;}
/** Complete ROS1 abb_robot_msgs/TriggerWithResultCodeRequest codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsTriggerWithResultCodeRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_999f63504788e522f7d0108a895cf35cd101e9a50856e480ae9edabf791bcf94",maxWireBytes:0,minWireBytes:0,cartesianMaxWireBytes:0n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {};},
validateValue(value){Ros1Abb_robot_msgsTriggerWithResultCodeRequestSize(Ros1Abb_robot_msgsTriggerWithResultCodeRequestCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsTriggerWithResultCodeRequestCapture(value),n=Ros1Abb_robot_msgsTriggerWithResultCodeRequestSize(owned),w=new Writer(n);Ros1Abb_robot_msgsTriggerWithResultCodeRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,0,90);if(owned.length<0)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsTriggerWithResultCodeRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsTriggerWithResultCodeRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(n>0)bad(90);return n;}
function Ros1Abb_robot_msgsTriggerWithResultCodeRequestWrite(value,w){
}
function Ros1Abb_robot_msgsTriggerWithResultCodeRequestRead(r){const value={};
return value;}
function Ros1Abb_robot_msgsTriggerWithResultCodeRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
return out;}
/** Complete ROS1 abb_robot_msgs/TriggerWithResultCodeResponse codec; maxima are not actual lengths. */
export const Ros1Abb_robot_msgsTriggerWithResultCodeResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_4ef5c987592e849766b82b7fd99a65b7a1e17605f3e3341c99803181887967ff",maxWireBytes:518,minWireBytes:6,cartesianMaxWireBytes:518n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"result_code":0,"message":""};},
validateValue(value){Ros1Abb_robot_msgsTriggerWithResultCodeResponseSize(Ros1Abb_robot_msgsTriggerWithResultCodeResponseCapture(value));return true;},
encode(value){const owned=Ros1Abb_robot_msgsTriggerWithResultCodeResponseCapture(value),n=Ros1Abb_robot_msgsTriggerWithResultCodeResponseSize(owned),w=new Writer(n);Ros1Abb_robot_msgsTriggerWithResultCodeResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,518,90);if(owned.length<6)bad();const r=new Reader(owned),value=Ros1Abb_robot_msgsTriggerWithResultCodeResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Abb_robot_msgsTriggerWithResultCodeResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["result_code"],2,false);n=addSize(n,2);
n=addSize(n,4+textBytes(value["message"],512).length);
if(n>518)bad(90);return n;}
function Ros1Abb_robot_msgsTriggerWithResultCodeResponseWrite(value,w){
w.v.setUint16(w.at,value["result_code"],true);w.at+=2;
w.text(value["message"],512);
}
function Ros1Abb_robot_msgsTriggerWithResultCodeResponseRead(r){const value={};
{
const decoded=r.v.getUint16(r.take(2),true);
value["result_code"]=decoded;
}
{
const decoded=r.text(512);
value["message"]=decoded;
}
return value;}
function Ros1Abb_robot_msgsTriggerWithResultCodeResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["result_code"];
out["result_code"]=field;
}
{const field=value["message"];
out["message"]=field;
}
return out;}
/** Complete ROS1 duration codec; maxima are not actual lengths. */
export const Ros1DurationCodec=Object.freeze({codecIdentity:"UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"sec":0,"nsec":0};},
validateValue(value){Ros1DurationSize(Ros1DurationCapture(value));return true;},
encode(value){const owned=Ros1DurationCapture(value),n=Ros1DurationSize(owned),w=new Writer(n);Ros1DurationWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1DurationRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1DurationSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["sec"],4,true);n=addSize(n,4);
integer(value["nsec"],4,true);n=addSize(n,4);
if(value["nsec"]<0||value["nsec"]>=1000000000)bad();
if(n>8)bad(90);return n;}
function Ros1DurationWrite(value,w){
w.v.setInt32(w.at,value["sec"],true);w.at+=4;
w.v.setInt32(w.at,value["nsec"],true);w.at+=4;
}
function Ros1DurationRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["sec"]=decoded;
}
{
const decoded=r.v.getInt32(r.take(4),true);
value["nsec"]=decoded;
}
if(value["nsec"]<0||value["nsec"]>=1000000000)bad();
return value;}
function Ros1DurationCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["sec"];
out["sec"]=field;
}
{const field=value["nsec"];
out["nsec"]=field;
}
return out;}
/** Complete ROS1 trajectory_msgs/JointTrajectoryPoint codec; maxima are not actual lengths. */
export const Ros1Trajectory_msgsJointTrajectoryPointCodec=Object.freeze({codecIdentity:"UWVMROS1_9773572fff8616235c4d7e23fccbe01f28dcfa62c7c80323a65c8c305feba3c5",maxWireBytes:408,minWireBytes:24,cartesianMaxWireBytes:408n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"positions":[],"velocities":[],"accelerations":[],"effort":[],"time_from_start":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Trajectory_msgsJointTrajectoryPointSize(Ros1Trajectory_msgsJointTrajectoryPointCapture(value));return true;},
encode(value){const owned=Ros1Trajectory_msgsJointTrajectoryPointCapture(value),n=Ros1Trajectory_msgsJointTrajectoryPointSize(owned),w=new Writer(n);Ros1Trajectory_msgsJointTrajectoryPointWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,408,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Trajectory_msgsJointTrajectoryPointRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Trajectory_msgsJointTrajectoryPointSize(value){if(value===null||typeof value!=="object")bad();let n=0;
if(!Array.isArray(value["positions"])||value["positions"].length>12)bad();
n=addSize(n,4);
for(const element of value["positions"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocities"])||value["velocities"].length>12)bad();
n=addSize(n,4);
for(const element of value["velocities"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["accelerations"])||value["accelerations"].length>12)bad();
n=addSize(n,4);
for(const element of value["accelerations"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>12)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
n=addSize(n,Ros1DurationSize(value["time_from_start"]));
if(n>408)bad(90);return n;}
function Ros1Trajectory_msgsJointTrajectoryPointWrite(value,w){
w.v.setUint32(w.at,value["positions"].length,true);w.at+=4;
for(const element of value["positions"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["velocities"].length,true);w.at+=4;
for(const element of value["velocities"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["accelerations"].length,true);w.at+=4;
for(const element of value["accelerations"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["effort"].length,true);w.at+=4;
for(const element of value["effort"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
Ros1DurationWrite(value["time_from_start"],w);
}
function Ros1Trajectory_msgsJointTrajectoryPointRead(r){const value={};
{
{const count=r.u32();if(count>12)bad();value["positions"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["positions"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["velocities"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocities"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["accelerations"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["accelerations"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["effort"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["effort"].push(decoded);
}}
}
{
const decoded=Ros1DurationRead(r);
value["time_from_start"]=decoded;
}
return value;}
function Ros1Trajectory_msgsJointTrajectoryPointCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["positions"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["positions"]=values;
}
{const field=value["velocities"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocities"]=values;
}
{const field=value["accelerations"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["accelerations"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
{const field=value["time_from_start"];
out["time_from_start"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 trajectory_msgs/JointTrajectory codec; maxima are not actual lengths. */
export const Ros1Trajectory_msgsJointTrajectoryCodec=Object.freeze({codecIdentity:"UWVMROS1_079e6c432f0a858d6bd47c87aaf353a8b0676706809e0f984705b1a688de282b",maxWireBytes:4168,minWireBytes:24,cartesianMaxWireBytes:4168n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"joint_names":[],"points":[]};},
validateValue(value){Ros1Trajectory_msgsJointTrajectorySize(Ros1Trajectory_msgsJointTrajectoryCapture(value));return true;},
encode(value){const owned=Ros1Trajectory_msgsJointTrajectoryCapture(value),n=Ros1Trajectory_msgsJointTrajectorySize(owned),w=new Writer(n);Ros1Trajectory_msgsJointTrajectoryWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4168,90);if(owned.length<24)bad();const r=new Reader(owned),value=Ros1Trajectory_msgsJointTrajectoryRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Trajectory_msgsJointTrajectorySize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>12)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["points"])||value["points"].length>8)bad();
n=addSize(n,4);
for(const element of value["points"]){
n=addSize(n,Ros1Trajectory_msgsJointTrajectoryPointSize(element));
}
if(n>4168)bad(90);return n;}
function Ros1Trajectory_msgsJointTrajectoryWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["points"].length,true);w.at+=4;
for(const element of value["points"]){
Ros1Trajectory_msgsJointTrajectoryPointWrite(element,w);
}
}
function Ros1Trajectory_msgsJointTrajectoryRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
{const count=r.u32();if(count>8)bad();value["points"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Trajectory_msgsJointTrajectoryPointRead(r);
value["points"].push(decoded);
}}
}
return value;}
function Ros1Trajectory_msgsJointTrajectoryCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["points"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>8)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Trajectory_msgsJointTrajectoryPointCapture(element);
}out["points"]=values;
}
return out;}
/** Complete ROS1 control_msgs/JointTolerance codec; maxima are not actual lengths. */
export const Ros1Control_msgsJointToleranceCodec=Object.freeze({codecIdentity:"UWVMROS1_c9f71f98726d4a6621dcc8db96a394a24941a79d9041c8ad7a07724ee64d3c88",maxWireBytes:92,minWireBytes:28,cartesianMaxWireBytes:92n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"name":"","position":0,"velocity":0,"acceleration":0};},
validateValue(value){Ros1Control_msgsJointToleranceSize(Ros1Control_msgsJointToleranceCapture(value));return true;},
encode(value){const owned=Ros1Control_msgsJointToleranceCapture(value),n=Ros1Control_msgsJointToleranceSize(owned),w=new Writer(n);Ros1Control_msgsJointToleranceWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,92,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros1Control_msgsJointToleranceRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Control_msgsJointToleranceSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,4+textBytes(value["name"],64).length);
if(typeof value["position"]!=="number"||!Number.isFinite(value["position"]))bad();n=addSize(n,8);
if(typeof value["velocity"]!=="number"||!Number.isFinite(value["velocity"]))bad();n=addSize(n,8);
if(typeof value["acceleration"]!=="number"||!Number.isFinite(value["acceleration"]))bad();n=addSize(n,8);
if(n>92)bad(90);return n;}
function Ros1Control_msgsJointToleranceWrite(value,w){
w.text(value["name"],64);
w.v.setFloat64(w.at,value["position"],true);w.at+=8;
w.v.setFloat64(w.at,value["velocity"],true);w.at+=8;
w.v.setFloat64(w.at,value["acceleration"],true);w.at+=8;
}
function Ros1Control_msgsJointToleranceRead(r){const value={};
{
const decoded=r.text(64);
value["name"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"]=decoded;
}
{
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["acceleration"]=decoded;
}
return value;}
function Ros1Control_msgsJointToleranceCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["name"];
out["name"]=field;
}
{const field=value["position"];
out["position"]=field;
}
{const field=value["velocity"];
out["velocity"]=field;
}
{const field=value["acceleration"];
out["acceleration"]=field;
}
return out;}
/** Complete ROS1 control_msgs/FollowJointTrajectoryGoal codec; maxima are not actual lengths. */
export const Ros1Control_msgsFollowJointTrajectoryGoalCodec=Object.freeze({codecIdentity:"UWVMROS1_e9fda6f9e732450f2cf1af046ceb67815e6e1a22105eda2fee38f8f470252d40",maxWireBytes:6392,minWireBytes:40,cartesianMaxWireBytes:6392n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"trajectory":Ros1Trajectory_msgsJointTrajectoryCodec.defaultValue(),"path_tolerance":[],"goal_tolerance":[],"goal_time_tolerance":Ros1DurationCodec.defaultValue()};},
validateValue(value){Ros1Control_msgsFollowJointTrajectoryGoalSize(Ros1Control_msgsFollowJointTrajectoryGoalCapture(value));return true;},
encode(value){const owned=Ros1Control_msgsFollowJointTrajectoryGoalCapture(value),n=Ros1Control_msgsFollowJointTrajectoryGoalSize(owned),w=new Writer(n);Ros1Control_msgsFollowJointTrajectoryGoalWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6392,90);if(owned.length<40)bad();const r=new Reader(owned),value=Ros1Control_msgsFollowJointTrajectoryGoalRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Control_msgsFollowJointTrajectoryGoalSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Trajectory_msgsJointTrajectorySize(value["trajectory"]));
if(!Array.isArray(value["path_tolerance"])||value["path_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["path_tolerance"]){
n=addSize(n,Ros1Control_msgsJointToleranceSize(element));
}
if(!Array.isArray(value["goal_tolerance"])||value["goal_tolerance"].length>12)bad();
n=addSize(n,4);
for(const element of value["goal_tolerance"]){
n=addSize(n,Ros1Control_msgsJointToleranceSize(element));
}
n=addSize(n,Ros1DurationSize(value["goal_time_tolerance"]));
if(n>6392)bad(90);return n;}
function Ros1Control_msgsFollowJointTrajectoryGoalWrite(value,w){
Ros1Trajectory_msgsJointTrajectoryWrite(value["trajectory"],w);
w.v.setUint32(w.at,value["path_tolerance"].length,true);w.at+=4;
for(const element of value["path_tolerance"]){
Ros1Control_msgsJointToleranceWrite(element,w);
}
w.v.setUint32(w.at,value["goal_tolerance"].length,true);w.at+=4;
for(const element of value["goal_tolerance"]){
Ros1Control_msgsJointToleranceWrite(element,w);
}
Ros1DurationWrite(value["goal_time_tolerance"],w);
}
function Ros1Control_msgsFollowJointTrajectoryGoalRead(r){const value={};
{
const decoded=Ros1Trajectory_msgsJointTrajectoryRead(r);
value["trajectory"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["path_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Control_msgsJointToleranceRead(r);
value["path_tolerance"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["goal_tolerance"]=[];for(let i=0;i<count;++i){
const decoded=Ros1Control_msgsJointToleranceRead(r);
value["goal_tolerance"].push(decoded);
}}
}
{
const decoded=Ros1DurationRead(r);
value["goal_time_tolerance"]=decoded;
}
return value;}
function Ros1Control_msgsFollowJointTrajectoryGoalCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["trajectory"];
out["trajectory"]=Ros1Trajectory_msgsJointTrajectoryCapture(field);
}
{const field=value["path_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Control_msgsJointToleranceCapture(element);
}out["path_tolerance"]=values;
}
{const field=value["goal_tolerance"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=Ros1Control_msgsJointToleranceCapture(element);
}out["goal_tolerance"]=values;
}
{const field=value["goal_time_tolerance"];
out["goal_time_tolerance"]=Ros1DurationCapture(field);
}
return out;}
/** Complete ROS1 control_msgs/FollowJointTrajectoryResult codec; maxima are not actual lengths. */
export const Ros1Control_msgsFollowJointTrajectoryResultCodec=Object.freeze({codecIdentity:"UWVMROS1_71da832be83c08804668aa63576768bbe831f21f5f270cdc8d851b30d2fc4281",maxWireBytes:4104,minWireBytes:8,cartesianMaxWireBytes:4104n,aggregateWireBytes:0,constants:Object.freeze({"SUCCESSFUL":0,"INVALID_GOAL":-1,"INVALID_JOINTS":-2,"OLD_HEADER_TIMESTAMP":-3,"PATH_TOLERANCE_VIOLATED":-4,"GOAL_TOLERANCE_VIOLATED":-5}),
defaultValue(){return {"error_code":0,"error_string":""};},
validateValue(value){Ros1Control_msgsFollowJointTrajectoryResultSize(Ros1Control_msgsFollowJointTrajectoryResultCapture(value));return true;},
encode(value){const owned=Ros1Control_msgsFollowJointTrajectoryResultCapture(value),n=Ros1Control_msgsFollowJointTrajectoryResultSize(owned),w=new Writer(n);Ros1Control_msgsFollowJointTrajectoryResultWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4104,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros1Control_msgsFollowJointTrajectoryResultRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Control_msgsFollowJointTrajectoryResultSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["error_code"],4,true);n=addSize(n,4);
n=addSize(n,4+textBytes(value["error_string"],4096).length);
if(n>4104)bad(90);return n;}
function Ros1Control_msgsFollowJointTrajectoryResultWrite(value,w){
w.v.setInt32(w.at,value["error_code"],true);w.at+=4;
w.text(value["error_string"],4096);
}
function Ros1Control_msgsFollowJointTrajectoryResultRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["error_code"]=decoded;
}
{
const decoded=r.text(4096);
value["error_string"]=decoded;
}
return value;}
function Ros1Control_msgsFollowJointTrajectoryResultCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["error_code"];
out["error_code"]=field;
}
{const field=value["error_string"];
out["error_string"]=field;
}
return out;}
/** Complete ROS1 control_msgs/FollowJointTrajectoryFeedback codec; maxima are not actual lengths. */
export const Ros1Control_msgsFollowJointTrajectoryFeedbackCodec=Object.freeze({codecIdentity:"UWVMROS1_a1ad5a1f8a00b60f97d06209737993bd395d83d1a008705d65193b63933b62a8",maxWireBytes:2124,minWireBytes:92,cartesianMaxWireBytes:2124n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"joint_names":[],"desired":Ros1Trajectory_msgsJointTrajectoryPointCodec.defaultValue(),"actual":Ros1Trajectory_msgsJointTrajectoryPointCodec.defaultValue(),"error":Ros1Trajectory_msgsJointTrajectoryPointCodec.defaultValue()};},
validateValue(value){Ros1Control_msgsFollowJointTrajectoryFeedbackSize(Ros1Control_msgsFollowJointTrajectoryFeedbackCapture(value));return true;},
encode(value){const owned=Ros1Control_msgsFollowJointTrajectoryFeedbackCapture(value),n=Ros1Control_msgsFollowJointTrajectoryFeedbackSize(owned),w=new Writer(n);Ros1Control_msgsFollowJointTrajectoryFeedbackWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,2124,90);if(owned.length<92)bad();const r=new Reader(owned),value=Ros1Control_msgsFollowJointTrajectoryFeedbackRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Control_msgsFollowJointTrajectoryFeedbackSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["joint_names"])||value["joint_names"].length>12)bad();
n=addSize(n,4);
for(const element of value["joint_names"]){
n=addSize(n,4+textBytes(element,64).length);
}
n=addSize(n,Ros1Trajectory_msgsJointTrajectoryPointSize(value["desired"]));
n=addSize(n,Ros1Trajectory_msgsJointTrajectoryPointSize(value["actual"]));
n=addSize(n,Ros1Trajectory_msgsJointTrajectoryPointSize(value["error"]));
if(n>2124)bad(90);return n;}
function Ros1Control_msgsFollowJointTrajectoryFeedbackWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["joint_names"].length,true);w.at+=4;
for(const element of value["joint_names"]){
w.text(element,64);
}
Ros1Trajectory_msgsJointTrajectoryPointWrite(value["desired"],w);
Ros1Trajectory_msgsJointTrajectoryPointWrite(value["actual"],w);
Ros1Trajectory_msgsJointTrajectoryPointWrite(value["error"],w);
}
function Ros1Control_msgsFollowJointTrajectoryFeedbackRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["joint_names"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["joint_names"].push(decoded);
}}
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPointRead(r);
value["desired"]=decoded;
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPointRead(r);
value["actual"]=decoded;
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPointRead(r);
value["error"]=decoded;
}
return value;}
function Ros1Control_msgsFollowJointTrajectoryFeedbackCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["joint_names"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["joint_names"]=values;
}
{const field=value["desired"];
out["desired"]=Ros1Trajectory_msgsJointTrajectoryPointCapture(field);
}
{const field=value["actual"];
out["actual"]=Ros1Trajectory_msgsJointTrajectoryPointCapture(field);
}
{const field=value["error"];
out["error"]=Ros1Trajectory_msgsJointTrajectoryPointCapture(field);
}
return out;}
/** Complete ROS1 sensor_msgs/JointState codec; maxima are not actual lengths. */
export const Ros1Sensor_msgsJointStateCodec=Object.freeze({codecIdentity:"UWVMROS1_8d1bb9c4903dd4b070a5ab157b3590815722d6b574d610c9b9fb5ead8be7a057",maxWireBytes:1200,minWireBytes:32,cartesianMaxWireBytes:1200n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros1Std_msgsHeaderCodec.defaultValue(),"name":[],"position":[],"velocity":[],"effort":[]};},
validateValue(value){Ros1Sensor_msgsJointStateSize(Ros1Sensor_msgsJointStateCapture(value));return true;},
encode(value){const owned=Ros1Sensor_msgsJointStateCapture(value),n=Ros1Sensor_msgsJointStateSize(owned),w=new Writer(n);Ros1Sensor_msgsJointStateWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,1200,90);if(owned.length<32)bad();const r=new Reader(owned),value=Ros1Sensor_msgsJointStateRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros1Sensor_msgsJointStateSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros1Std_msgsHeaderSize(value["header"]));
if(!Array.isArray(value["name"])||value["name"].length>12)bad();
n=addSize(n,4);
for(const element of value["name"]){
n=addSize(n,4+textBytes(element,64).length);
}
if(!Array.isArray(value["position"])||value["position"].length>12)bad();
n=addSize(n,4);
for(const element of value["position"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["velocity"])||value["velocity"].length>12)bad();
n=addSize(n,4);
for(const element of value["velocity"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if(!Array.isArray(value["effort"])||value["effort"].length>12)bad();
n=addSize(n,4);
for(const element of value["effort"]){
if(typeof element!=="number"||!Number.isFinite(element))bad();n=addSize(n,8);
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
if(n>1200)bad(90);return n;}
function Ros1Sensor_msgsJointStateWrite(value,w){
Ros1Std_msgsHeaderWrite(value["header"],w);
w.v.setUint32(w.at,value["name"].length,true);w.at+=4;
for(const element of value["name"]){
w.text(element,64);
}
w.v.setUint32(w.at,value["position"].length,true);w.at+=4;
for(const element of value["position"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["velocity"].length,true);w.at+=4;
for(const element of value["velocity"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
w.v.setUint32(w.at,value["effort"].length,true);w.at+=4;
for(const element of value["effort"]){
w.v.setFloat64(w.at,element,true);w.at+=8;
}
}
function Ros1Sensor_msgsJointStateRead(r){const value={};
{
const decoded=Ros1Std_msgsHeaderRead(r);
value["header"]=decoded;
}
{
{const count=r.u32();if(count>12)bad();value["name"]=[];for(let i=0;i<count;++i){
const decoded=r.text(64);
value["name"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["position"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["position"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["velocity"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["velocity"].push(decoded);
}}
}
{
{const count=r.u32();if(count>12)bad();value["effort"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getFloat64(r.take(8),true);
if(!Number.isFinite(decoded))bad();
value["effort"].push(decoded);
}}
}
if((value["name"].length!=0&&value["position"].length!=0&&value["name"].length!=value["position"].length)||(value["name"].length!=0&&value["velocity"].length!=0&&value["name"].length!=value["velocity"].length)||(value["name"].length!=0&&value["effort"].length!=0&&value["name"].length!=value["effort"].length)||(value["position"].length!=0&&value["velocity"].length!=0&&value["position"].length!=value["velocity"].length)||(value["position"].length!=0&&value["effort"].length!=0&&value["position"].length!=value["effort"].length)||(value["velocity"].length!=0&&value["effort"].length!=0&&value["velocity"].length!=value["effort"].length))bad();
return value;}
function Ros1Sensor_msgsJointStateCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros1Std_msgsHeaderCapture(field);
}
{const field=value["name"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["name"]=values;
}
{const field=value["position"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["position"]=values;
}
{const field=value["velocity"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["velocity"]=values;
}
{const field=value["effort"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>12)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["effort"]=values;
}
return out;}
