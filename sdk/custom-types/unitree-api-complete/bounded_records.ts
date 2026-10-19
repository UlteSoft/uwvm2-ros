/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original unitree_api/msg/RequestIdentity; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgRequestIdentity {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field id, native scalar int64. */
id:i64=0;
/** Exact native field api_id, native scalar int64. */
api_id:i64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
{
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.id,8);
w.integer(<u64>this.api_id,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgRequestIdentity{const value=new Ros2Unitree_apiMsgRequestIdentity();
{
const decoded=<i64>r.integer(8);
value.id=decoded;
}
{
const decoded=<i64>r.integer(8);
value.api_id=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgRequestIdentityCodec implements VendorCodec<Ros2Unitree_apiMsgRequestIdentity>{
static readonly codecIdentity:string="UWVMROS1_e3daab89abcdaf6526511a189cfcc0c78aaebe8aefc53814ba77f6eb80cdc4e2";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Unitree_apiMsgRequestIdentity):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgRequestIdentity):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgRequestIdentity>{const out=new VendorDecoded<Ros2Unitree_apiMsgRequestIdentity>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgRequestIdentity.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgRequestIdentity{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgRequestIdentity():changetype<Ros2Unitree_apiMsgRequestIdentity>(out.value);}
}
/** Complete original unitree_api/msg/RequestLease; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgRequestLease {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field id, native scalar int64. */
id:i64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.id,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgRequestLease{const value=new Ros2Unitree_apiMsgRequestLease();
{
const decoded=<i64>r.integer(8);
value.id=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgRequestLeaseCodec implements VendorCodec<Ros2Unitree_apiMsgRequestLease>{
static readonly codecIdentity:string="UWVMROS1_3dcb1fe2998c880e2d9196967da172ccd9060ebc7f6d2feb8682dc3d104c2e6e";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Unitree_apiMsgRequestLease):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgRequestLease):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgRequestLease>{const out=new VendorDecoded<Ros2Unitree_apiMsgRequestLease>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgRequestLease.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgRequestLease{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgRequestLease():changetype<Ros2Unitree_apiMsgRequestLease>(out.value);}
}
/** Complete original unitree_api/msg/RequestPolicy; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgRequestPolicy {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field priority, native scalar int32. */
priority:i32=0;
/** Exact native field noreply, native scalar bool. */
noreply:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>5-n)return -90;n+=4;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.priority,4);
w.u8(this.noreply?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgRequestPolicy{const value=new Ros2Unitree_apiMsgRequestPolicy();
{
const decoded=<i32>r.integer(4);
value.priority=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.noreply=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgRequestPolicyCodec implements VendorCodec<Ros2Unitree_apiMsgRequestPolicy>{
static readonly codecIdentity:string="UWVMROS1_61f1a1eafec2f694d24cf798963dcb5a0c8f40e9791db5a502f0736cc8587bb8";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Unitree_apiMsgRequestPolicy):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgRequestPolicy):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgRequestPolicy>{const out=new VendorDecoded<Ros2Unitree_apiMsgRequestPolicy>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgRequestPolicy.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgRequestPolicy{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgRequestPolicy():changetype<Ros2Unitree_apiMsgRequestPolicy>(out.value);}
}
/** Complete original unitree_api/msg/RequestHeader; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgRequestHeader {
static readonly maxWireBytes:i32=29;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=29;
/** Exact native field identity, native scalar unitree_api/msg/RequestIdentity. */
identity:Ros2Unitree_apiMsgRequestIdentity=new Ros2Unitree_apiMsgRequestIdentity();
/** Exact native field lease, native scalar unitree_api/msg/RequestLease. */
lease:Ros2Unitree_apiMsgRequestLease=new Ros2Unitree_apiMsgRequestLease();
/** Exact native field policy, native scalar unitree_api/msg/RequestPolicy. */
policy:Ros2Unitree_apiMsgRequestPolicy=new Ros2Unitree_apiMsgRequestPolicy();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.identity.wireSize();if(child<0)return child;if(child>29-n)return -90;n+=child;
if(n>29)return -90;
}
{
const child=this.lease.wireSize();if(child<0)return child;if(child>29-n)return -90;n+=child;
if(n>29)return -90;
}
{
const child=this.policy.wireSize();if(child<0)return child;if(child>29-n)return -90;n+=child;
if(n>29)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.identity.writeControl(w);
this.lease.writeControl(w);
this.policy.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgRequestHeader{const value=new Ros2Unitree_apiMsgRequestHeader();
{
const decoded=Ros2Unitree_apiMsgRequestIdentity.readControl(r);
value.identity=decoded;
}
{
const decoded=Ros2Unitree_apiMsgRequestLease.readControl(r);
value.lease=decoded;
}
{
const decoded=Ros2Unitree_apiMsgRequestPolicy.readControl(r);
value.policy=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgRequestHeaderCodec implements VendorCodec<Ros2Unitree_apiMsgRequestHeader>{
static readonly codecIdentity:string="UWVMROS1_c8e8b83e2cdc978c5279483dbfac8d4185a3e05c87799b424643f0fcb02d0fd0";
static readonly maxWireBytes:i32=29;
encodeChecked(value:Ros2Unitree_apiMsgRequestHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgRequestHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgRequestHeader>{const out=new VendorDecoded<Ros2Unitree_apiMsgRequestHeader>();if(bytes.length>29){out.status=90;return out;}if(bytes.length<29){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgRequestHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgRequestHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgRequestHeader():changetype<Ros2Unitree_apiMsgRequestHeader>(out.value);}
}
/** Complete original unitree_api/msg/Request; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgRequest {
static readonly maxWireBytes:i32=6181;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6181;
/** Exact native field header, native scalar unitree_api/msg/RequestHeader. */
header:Ros2Unitree_apiMsgRequestHeader=new Ros2Unitree_apiMsgRequestHeader();
/** Exact native field parameter, native scalar string. */
parameter:string="";
/** Exact native field binary, native scalar uint8. */
binary:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>6181-n)return -90;n+=child;
if(n>6181)return -90;
}
{
if(!validUnicode(this.parameter)||this.parameter.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.parameter);if(size>4096)return -22;if(4+size>6181-n)return -90;n+=4+size;
if(n>6181)return -90;
}
{
if(this.binary.length>2048)return -22;
if(4>6181-n)return -90;n+=4;
for(let i=0;i<this.binary.length;++i){
if(1>6181-n)return -90;n+=1;
if(n>6181)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.parameter);
w.u32(<u32>this.binary.length);
for(let i=0;i<this.binary.length;++i){
w.integer(<u64>this.binary[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgRequest{const value=new Ros2Unitree_apiMsgRequest();
{
const decoded=Ros2Unitree_apiMsgRequestHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.parameter=decoded;
}
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.binary=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.binary.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_apiMsgRequestCodec implements VendorCodec<Ros2Unitree_apiMsgRequest>{
static readonly codecIdentity:string="UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2";
static readonly maxWireBytes:i32=6181;
encodeChecked(value:Ros2Unitree_apiMsgRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgRequest>{const out=new VendorDecoded<Ros2Unitree_apiMsgRequest>();if(bytes.length>6181){out.status=90;return out;}if(bytes.length<37){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgRequest():changetype<Ros2Unitree_apiMsgRequest>(out.value);}
}
/** Complete original unitree_api/msg/ResponseStatus; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgResponseStatus {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field code, native scalar int32. */
code:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.code,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgResponseStatus{const value=new Ros2Unitree_apiMsgResponseStatus();
{
const decoded=<i32>r.integer(4);
value.code=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgResponseStatusCodec implements VendorCodec<Ros2Unitree_apiMsgResponseStatus>{
static readonly codecIdentity:string="UWVMROS1_aaad17e09d5d6549455f69060d1bb03ec81cddfc0ede323a5f2365a6c1a7b7b1";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Unitree_apiMsgResponseStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgResponseStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgResponseStatus>{const out=new VendorDecoded<Ros2Unitree_apiMsgResponseStatus>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgResponseStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgResponseStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgResponseStatus():changetype<Ros2Unitree_apiMsgResponseStatus>(out.value);}
}
/** Complete original unitree_api/msg/ResponseHeader; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgResponseHeader {
static readonly maxWireBytes:i32=20;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20;
/** Exact native field identity, native scalar unitree_api/msg/RequestIdentity. */
identity:Ros2Unitree_apiMsgRequestIdentity=new Ros2Unitree_apiMsgRequestIdentity();
/** Exact native field status, native scalar unitree_api/msg/ResponseStatus. */
status:Ros2Unitree_apiMsgResponseStatus=new Ros2Unitree_apiMsgResponseStatus();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.identity.wireSize();if(child<0)return child;if(child>20-n)return -90;n+=child;
if(n>20)return -90;
}
{
const child=this.status.wireSize();if(child<0)return child;if(child>20-n)return -90;n+=child;
if(n>20)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.identity.writeControl(w);
this.status.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgResponseHeader{const value=new Ros2Unitree_apiMsgResponseHeader();
{
const decoded=Ros2Unitree_apiMsgRequestIdentity.readControl(r);
value.identity=decoded;
}
{
const decoded=Ros2Unitree_apiMsgResponseStatus.readControl(r);
value.status=decoded;
}
return value;}
}
export class Ros2Unitree_apiMsgResponseHeaderCodec implements VendorCodec<Ros2Unitree_apiMsgResponseHeader>{
static readonly codecIdentity:string="UWVMROS1_20ddef706f0be7d7e2e356ab2222b6e198bd36fd6d8a81706553d5a883de85e0";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros2Unitree_apiMsgResponseHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgResponseHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgResponseHeader>{const out=new VendorDecoded<Ros2Unitree_apiMsgResponseHeader>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgResponseHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgResponseHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgResponseHeader():changetype<Ros2Unitree_apiMsgResponseHeader>(out.value);}
}
/** Complete original unitree_api/msg/Response; spellings are published in generation proof. */
export class Ros2Unitree_apiMsgResponse {
static readonly maxWireBytes:i32=6172;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6172;
/** Exact native field header, native scalar unitree_api/msg/ResponseHeader. */
header:Ros2Unitree_apiMsgResponseHeader=new Ros2Unitree_apiMsgResponseHeader();
/** Exact native field data, native scalar string. */
data:string="";
/** Exact native field binary, native scalar int8. */
binary:Array<i8>=new Array<i8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>6172-n)return -90;n+=child;
if(n>6172)return -90;
}
{
if(!validUnicode(this.data)||this.data.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.data);if(size>4096)return -22;if(4+size>6172-n)return -90;n+=4+size;
if(n>6172)return -90;
}
{
if(this.binary.length>2048)return -22;
if(4>6172-n)return -90;n+=4;
for(let i=0;i<this.binary.length;++i){
if(1>6172-n)return -90;n+=1;
if(n>6172)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.data);
w.u32(<u32>this.binary.length);
for(let i=0;i<this.binary.length;++i){
w.integer(<u64>this.binary[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_apiMsgResponse{const value=new Ros2Unitree_apiMsgResponse();
{
const decoded=Ros2Unitree_apiMsgResponseHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.data=decoded;
}
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.binary=new Array<i8>();for(let i:u32=0;i<count;++i){
const decoded=<i8>r.integer(1);
value.binary.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_apiMsgResponseCodec implements VendorCodec<Ros2Unitree_apiMsgResponse>{
static readonly codecIdentity:string="UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749";
static readonly maxWireBytes:i32=6172;
encodeChecked(value:Ros2Unitree_apiMsgResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_apiMsgResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_apiMsgResponse>{const out=new VendorDecoded<Ros2Unitree_apiMsgResponse>();if(bytes.length>6172){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_apiMsgResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_apiMsgResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_apiMsgResponse():changetype<Ros2Unitree_apiMsgResponse>(out.value);}
}
