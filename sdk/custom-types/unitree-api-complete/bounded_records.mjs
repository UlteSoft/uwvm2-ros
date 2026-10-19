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
/** Complete ROS2 unitree_api/msg/RequestIdentity codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgRequestIdentityCodec=Object.freeze({codecIdentity:"UWVMROS1_e3daab89abcdaf6526511a189cfcc0c78aaebe8aefc53814ba77f6eb80cdc4e2",maxWireBytes:16,minWireBytes:16,cartesianMaxWireBytes:16n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"id":0n,"api_id":0n};},
validateValue(value){Ros2Unitree_apiMsgRequestIdentitySize(Ros2Unitree_apiMsgRequestIdentityCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgRequestIdentityCapture(value),n=Ros2Unitree_apiMsgRequestIdentitySize(owned),w=new Writer(n);Ros2Unitree_apiMsgRequestIdentityWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,16,90);if(owned.length<16)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgRequestIdentityRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgRequestIdentitySize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["id"],8,true);n=addSize(n,8);
integer(value["api_id"],8,true);n=addSize(n,8);
if(n>16)bad(90);return n;}
function Ros2Unitree_apiMsgRequestIdentityWrite(value,w){
w.v.setBigInt64(w.at,value["id"],true);w.at+=8;
w.v.setBigInt64(w.at,value["api_id"],true);w.at+=8;
}
function Ros2Unitree_apiMsgRequestIdentityRead(r){const value={};
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["id"]=decoded;
}
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["api_id"]=decoded;
}
return value;}
function Ros2Unitree_apiMsgRequestIdentityCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["id"];
out["id"]=field;
}
{const field=value["api_id"];
out["api_id"]=field;
}
return out;}
/** Complete ROS2 unitree_api/msg/RequestLease codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgRequestLeaseCodec=Object.freeze({codecIdentity:"UWVMROS1_3dcb1fe2998c880e2d9196967da172ccd9060ebc7f6d2feb8682dc3d104c2e6e",maxWireBytes:8,minWireBytes:8,cartesianMaxWireBytes:8n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"id":0n};},
validateValue(value){Ros2Unitree_apiMsgRequestLeaseSize(Ros2Unitree_apiMsgRequestLeaseCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgRequestLeaseCapture(value),n=Ros2Unitree_apiMsgRequestLeaseSize(owned),w=new Writer(n);Ros2Unitree_apiMsgRequestLeaseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,8,90);if(owned.length<8)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgRequestLeaseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgRequestLeaseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["id"],8,true);n=addSize(n,8);
if(n>8)bad(90);return n;}
function Ros2Unitree_apiMsgRequestLeaseWrite(value,w){
w.v.setBigInt64(w.at,value["id"],true);w.at+=8;
}
function Ros2Unitree_apiMsgRequestLeaseRead(r){const value={};
{
const decoded=r.v.getBigInt64(r.take(8),true);
value["id"]=decoded;
}
return value;}
function Ros2Unitree_apiMsgRequestLeaseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["id"];
out["id"]=field;
}
return out;}
/** Complete ROS2 unitree_api/msg/RequestPolicy codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgRequestPolicyCodec=Object.freeze({codecIdentity:"UWVMROS1_61f1a1eafec2f694d24cf798963dcb5a0c8f40e9791db5a502f0736cc8587bb8",maxWireBytes:5,minWireBytes:5,cartesianMaxWireBytes:5n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"priority":0,"noreply":false};},
validateValue(value){Ros2Unitree_apiMsgRequestPolicySize(Ros2Unitree_apiMsgRequestPolicyCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgRequestPolicyCapture(value),n=Ros2Unitree_apiMsgRequestPolicySize(owned),w=new Writer(n);Ros2Unitree_apiMsgRequestPolicyWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,5,90);if(owned.length<5)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgRequestPolicyRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgRequestPolicySize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["priority"],4,true);n=addSize(n,4);
if(typeof value["noreply"]!=="boolean")bad();n=addSize(n,1);
if(n>5)bad(90);return n;}
function Ros2Unitree_apiMsgRequestPolicyWrite(value,w){
w.v.setInt32(w.at,value["priority"],true);w.at+=4;
w.v.setUint8(w.at,value["noreply"]?1:0,true);w.at+=1;
}
function Ros2Unitree_apiMsgRequestPolicyRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["priority"]=decoded;
}
{
const decoded=r.v.getUint8(r.take(1),true);
if(decoded>1)bad();
value["noreply"]=decoded!==0;
}
return value;}
function Ros2Unitree_apiMsgRequestPolicyCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["priority"];
out["priority"]=field;
}
{const field=value["noreply"];
out["noreply"]=field;
}
return out;}
/** Complete ROS2 unitree_api/msg/RequestHeader codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgRequestHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_c8e8b83e2cdc978c5279483dbfac8d4185a3e05c87799b424643f0fcb02d0fd0",maxWireBytes:29,minWireBytes:29,cartesianMaxWireBytes:29n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"identity":Ros2Unitree_apiMsgRequestIdentityCodec.defaultValue(),"lease":Ros2Unitree_apiMsgRequestLeaseCodec.defaultValue(),"policy":Ros2Unitree_apiMsgRequestPolicyCodec.defaultValue()};},
validateValue(value){Ros2Unitree_apiMsgRequestHeaderSize(Ros2Unitree_apiMsgRequestHeaderCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgRequestHeaderCapture(value),n=Ros2Unitree_apiMsgRequestHeaderSize(owned),w=new Writer(n);Ros2Unitree_apiMsgRequestHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,29,90);if(owned.length<29)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgRequestHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgRequestHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Unitree_apiMsgRequestIdentitySize(value["identity"]));
n=addSize(n,Ros2Unitree_apiMsgRequestLeaseSize(value["lease"]));
n=addSize(n,Ros2Unitree_apiMsgRequestPolicySize(value["policy"]));
if(n>29)bad(90);return n;}
function Ros2Unitree_apiMsgRequestHeaderWrite(value,w){
Ros2Unitree_apiMsgRequestIdentityWrite(value["identity"],w);
Ros2Unitree_apiMsgRequestLeaseWrite(value["lease"],w);
Ros2Unitree_apiMsgRequestPolicyWrite(value["policy"],w);
}
function Ros2Unitree_apiMsgRequestHeaderRead(r){const value={};
{
const decoded=Ros2Unitree_apiMsgRequestIdentityRead(r);
value["identity"]=decoded;
}
{
const decoded=Ros2Unitree_apiMsgRequestLeaseRead(r);
value["lease"]=decoded;
}
{
const decoded=Ros2Unitree_apiMsgRequestPolicyRead(r);
value["policy"]=decoded;
}
return value;}
function Ros2Unitree_apiMsgRequestHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["identity"];
out["identity"]=Ros2Unitree_apiMsgRequestIdentityCapture(field);
}
{const field=value["lease"];
out["lease"]=Ros2Unitree_apiMsgRequestLeaseCapture(field);
}
{const field=value["policy"];
out["policy"]=Ros2Unitree_apiMsgRequestPolicyCapture(field);
}
return out;}
/** Complete ROS2 unitree_api/msg/Request codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgRequestCodec=Object.freeze({codecIdentity:"UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2",maxWireBytes:6181,minWireBytes:37,cartesianMaxWireBytes:6181n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Unitree_apiMsgRequestHeaderCodec.defaultValue(),"parameter":"","binary":[]};},
validateValue(value){Ros2Unitree_apiMsgRequestSize(Ros2Unitree_apiMsgRequestCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgRequestCapture(value),n=Ros2Unitree_apiMsgRequestSize(owned),w=new Writer(n);Ros2Unitree_apiMsgRequestWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6181,90);if(owned.length<37)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgRequestRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgRequestSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Unitree_apiMsgRequestHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["parameter"],4096).length);
if(!Array.isArray(value["binary"])||value["binary"].length>2048)bad();
n=addSize(n,4);
for(const element of value["binary"]){
integer(element,1,false);n=addSize(n,1);
}
if(n>6181)bad(90);return n;}
function Ros2Unitree_apiMsgRequestWrite(value,w){
Ros2Unitree_apiMsgRequestHeaderWrite(value["header"],w);
w.text(value["parameter"],4096);
w.v.setUint32(w.at,value["binary"].length,true);w.at+=4;
for(const element of value["binary"]){
w.v.setUint8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_apiMsgRequestRead(r){const value={};
{
const decoded=Ros2Unitree_apiMsgRequestHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(4096);
value["parameter"]=decoded;
}
{
{const count=r.u32();if(count>2048)bad();value["binary"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getUint8(r.take(1),true);
value["binary"].push(decoded);
}}
}
return value;}
function Ros2Unitree_apiMsgRequestCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Unitree_apiMsgRequestHeaderCapture(field);
}
{const field=value["parameter"];
out["parameter"]=field;
}
{const field=value["binary"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["binary"]=values;
}
return out;}
/** Complete ROS2 unitree_api/msg/ResponseStatus codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgResponseStatusCodec=Object.freeze({codecIdentity:"UWVMROS1_aaad17e09d5d6549455f69060d1bb03ec81cddfc0ede323a5f2365a6c1a7b7b1",maxWireBytes:4,minWireBytes:4,cartesianMaxWireBytes:4n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"code":0};},
validateValue(value){Ros2Unitree_apiMsgResponseStatusSize(Ros2Unitree_apiMsgResponseStatusCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgResponseStatusCapture(value),n=Ros2Unitree_apiMsgResponseStatusSize(owned),w=new Writer(n);Ros2Unitree_apiMsgResponseStatusWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,4,90);if(owned.length<4)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgResponseStatusRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgResponseStatusSize(value){if(value===null||typeof value!=="object")bad();let n=0;
integer(value["code"],4,true);n=addSize(n,4);
if(n>4)bad(90);return n;}
function Ros2Unitree_apiMsgResponseStatusWrite(value,w){
w.v.setInt32(w.at,value["code"],true);w.at+=4;
}
function Ros2Unitree_apiMsgResponseStatusRead(r){const value={};
{
const decoded=r.v.getInt32(r.take(4),true);
value["code"]=decoded;
}
return value;}
function Ros2Unitree_apiMsgResponseStatusCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["code"];
out["code"]=field;
}
return out;}
/** Complete ROS2 unitree_api/msg/ResponseHeader codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgResponseHeaderCodec=Object.freeze({codecIdentity:"UWVMROS1_20ddef706f0be7d7e2e356ab2222b6e198bd36fd6d8a81706553d5a883de85e0",maxWireBytes:20,minWireBytes:20,cartesianMaxWireBytes:20n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"identity":Ros2Unitree_apiMsgRequestIdentityCodec.defaultValue(),"status":Ros2Unitree_apiMsgResponseStatusCodec.defaultValue()};},
validateValue(value){Ros2Unitree_apiMsgResponseHeaderSize(Ros2Unitree_apiMsgResponseHeaderCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgResponseHeaderCapture(value),n=Ros2Unitree_apiMsgResponseHeaderSize(owned),w=new Writer(n);Ros2Unitree_apiMsgResponseHeaderWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,20,90);if(owned.length<20)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgResponseHeaderRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgResponseHeaderSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Unitree_apiMsgRequestIdentitySize(value["identity"]));
n=addSize(n,Ros2Unitree_apiMsgResponseStatusSize(value["status"]));
if(n>20)bad(90);return n;}
function Ros2Unitree_apiMsgResponseHeaderWrite(value,w){
Ros2Unitree_apiMsgRequestIdentityWrite(value["identity"],w);
Ros2Unitree_apiMsgResponseStatusWrite(value["status"],w);
}
function Ros2Unitree_apiMsgResponseHeaderRead(r){const value={};
{
const decoded=Ros2Unitree_apiMsgRequestIdentityRead(r);
value["identity"]=decoded;
}
{
const decoded=Ros2Unitree_apiMsgResponseStatusRead(r);
value["status"]=decoded;
}
return value;}
function Ros2Unitree_apiMsgResponseHeaderCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["identity"];
out["identity"]=Ros2Unitree_apiMsgRequestIdentityCapture(field);
}
{const field=value["status"];
out["status"]=Ros2Unitree_apiMsgResponseStatusCapture(field);
}
return out;}
/** Complete ROS2 unitree_api/msg/Response codec; maxima are not actual lengths. */
export const Ros2Unitree_apiMsgResponseCodec=Object.freeze({codecIdentity:"UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749",maxWireBytes:6172,minWireBytes:28,cartesianMaxWireBytes:6172n,aggregateWireBytes:0,constants:Object.freeze({}),
defaultValue(){return {"header":Ros2Unitree_apiMsgResponseHeaderCodec.defaultValue(),"data":"","binary":[]};},
validateValue(value){Ros2Unitree_apiMsgResponseSize(Ros2Unitree_apiMsgResponseCapture(value));return true;},
encode(value){const owned=Ros2Unitree_apiMsgResponseCapture(value),n=Ros2Unitree_apiMsgResponseSize(owned),w=new Writer(n);Ros2Unitree_apiMsgResponseWrite(owned,w);if(w.at!==n)bad();return w.b;},
decode(bytes){const owned=copyRaw(bytes,6172,90);if(owned.length<28)bad();const r=new Reader(owned),value=Ros2Unitree_apiMsgResponseRead(r);if(r.at!==owned.length)bad();return value;},
valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});
function Ros2Unitree_apiMsgResponseSize(value){if(value===null||typeof value!=="object")bad();let n=0;
n=addSize(n,Ros2Unitree_apiMsgResponseHeaderSize(value["header"]));
n=addSize(n,4+textBytes(value["data"],4096).length);
if(!Array.isArray(value["binary"])||value["binary"].length>2048)bad();
n=addSize(n,4);
for(const element of value["binary"]){
integer(element,1,true);n=addSize(n,1);
}
if(n>6172)bad(90);return n;}
function Ros2Unitree_apiMsgResponseWrite(value,w){
Ros2Unitree_apiMsgResponseHeaderWrite(value["header"],w);
w.text(value["data"],4096);
w.v.setUint32(w.at,value["binary"].length,true);w.at+=4;
for(const element of value["binary"]){
w.v.setInt8(w.at,element,true);w.at+=1;
}
}
function Ros2Unitree_apiMsgResponseRead(r){const value={};
{
const decoded=Ros2Unitree_apiMsgResponseHeaderRead(r);
value["header"]=decoded;
}
{
const decoded=r.text(4096);
value["data"]=decoded;
}
{
{const count=r.u32();if(count>2048)bad();value["binary"]=[];for(let i=0;i<count;++i){
const decoded=r.v.getInt8(r.take(1),true);
value["binary"].push(decoded);
}}
}
return value;}
function Ros2Unitree_apiMsgResponseCapture(value){if(value===null||typeof value!=="object")bad();const out=Object.create(null);
{const field=value["header"];
out["header"]=Ros2Unitree_apiMsgResponseHeaderCapture(field);
}
{const field=value["data"];
out["data"]=field;
}
{const field=value["binary"];
if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count>2048)bad();const values=new Array(count);for(let i=0;i<count;++i){const element=field[i];
values[i]=element;
}out["binary"]=values;
}
return out;}
