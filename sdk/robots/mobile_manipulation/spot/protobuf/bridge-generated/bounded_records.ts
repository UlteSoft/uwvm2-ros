/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original uwvm_spot_bridge/srv/Command_Request; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvCommand_Request {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field budget_ns, native scalar uint64. */
budget_ns:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.budget_ns,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvCommand_Request{const value=new Ros2Uwvm_spot_bridgeSrvCommand_Request();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u64>r.integer(8);
value.budget_ns=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvCommand_RequestCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvCommand_Request>{
static readonly codecIdentity:string="UWVMROS1_29996d4dacb4ea847c5d629cb634d4aa5e2451627d2469d6cdf301cc007968f8";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvCommand_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvCommand_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvCommand_Request>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvCommand_Request>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvCommand_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvCommand_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvCommand_Request():changetype<Ros2Uwvm_spot_bridgeSrvCommand_Request>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Command_Response; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvCommand_Response {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field error, native scalar uint32. */
error:u32=0;
/** Exact native field unknown, native scalar bool. */
unknown:bool=false;
/** Exact native field native_accepted, native scalar bool. */
native_accepted:bool=false;
/** Exact native field native_command_id, native scalar uint32. */
native_command_id:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>26-n)return -90;n+=8;
if(n>26)return -90;
}
{
if(8>26-n)return -90;n+=8;
if(n>26)return -90;
}
{
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.error,4);
w.u8(this.unknown?1:0);
w.u8(this.native_accepted?1:0);
w.integer(<u64>this.native_command_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvCommand_Response{const value=new Ros2Uwvm_spot_bridgeSrvCommand_Response();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.unknown=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.native_accepted=decoded;
}
{
const decoded=<u32>r.integer(4);
value.native_command_id=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvCommand_ResponseCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvCommand_Response>{
static readonly codecIdentity:string="UWVMROS1_e35556e7439c8d5ca79cb0f4ad23cf73c85a8a79d1382cde398dd85082dd1733";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvCommand_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvCommand_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvCommand_Response>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvCommand_Response>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvCommand_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvCommand_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvCommand_Response():changetype<Ros2Uwvm_spot_bridgeSrvCommand_Response>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Feedback_Request; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvFeedback_Request {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field budget_ns, native scalar uint64. */
budget_ns:u64=0;
/** Exact native field command_token, native scalar uint64. */
command_token:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.budget_ns,8);
w.integer(<u64>this.command_token,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvFeedback_Request{const value=new Ros2Uwvm_spot_bridgeSrvFeedback_Request();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u64>r.integer(8);
value.budget_ns=decoded;
}
{
const decoded=<u64>r.integer(8);
value.command_token=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvFeedback_RequestCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvFeedback_Request>{
static readonly codecIdentity:string="UWVMROS1_de64ce283f49f5c324912ec262f7d15b6fd828879c01039d455c765743c45ba3";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvFeedback_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvFeedback_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvFeedback_Request>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvFeedback_Request>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvFeedback_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvFeedback_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvFeedback_Request():changetype<Ros2Uwvm_spot_bridgeSrvFeedback_Request>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Feedback_Response; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvFeedback_Response {
static readonly maxWireBytes:i32=16429;
static readonly aggregateWireBytes:i32=32768;
static readonly cartesianWireBytes:u64=16429;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field error, native scalar uint32. */
error:u32=0;
/** Exact native field unknown, native scalar bool. */
unknown:bool=false;
/** Exact native field native_command_id, native scalar uint32. */
native_command_id:u32=0;
/** Exact native field has_full_body_feedback, native scalar bool. */
has_full_body_feedback:bool=false;
/** Exact native field full_body_status, native scalar uint32. */
full_body_status:u32=0;
/** Exact native field has_mobility_feedback, native scalar bool. */
has_mobility_feedback:bool=false;
/** Exact native field mobility_status, native scalar uint32. */
mobility_status:u32=0;
/** Exact native field feedback_kind, native scalar uint8. */
feedback_kind:u8=0;
/** Exact native field has_subcommand_status, native scalar bool. */
has_subcommand_status:bool=false;
/** Exact native field subcommand_status, native scalar uint32. */
subcommand_status:u32=0;
/** Exact native field native_proto, native scalar uint8. */
native_proto:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>16429-n)return -90;n+=8;
if(n>16429)return -90;
}
{
if(8>16429-n)return -90;n+=8;
if(n>16429)return -90;
}
{
if(4>16429-n)return -90;n+=4;
if(n>16429)return -90;
}
{
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
{
if(4>16429-n)return -90;n+=4;
if(n>16429)return -90;
}
{
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
{
if(4>16429-n)return -90;n+=4;
if(n>16429)return -90;
}
{
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
{
if(4>16429-n)return -90;n+=4;
if(n>16429)return -90;
}
{
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
{
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
{
if(4>16429-n)return -90;n+=4;
if(n>16429)return -90;
}
{
if(this.native_proto.length>16384)return -22;
if(4>16429-n)return -90;n+=4;
for(let i=0;i<this.native_proto.length;++i){
if(1>16429-n)return -90;n+=1;
if(n>16429)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.error,4);
w.u8(this.unknown?1:0);
w.integer(<u64>this.native_command_id,4);
w.u8(this.has_full_body_feedback?1:0);
w.integer(<u64>this.full_body_status,4);
w.u8(this.has_mobility_feedback?1:0);
w.integer(<u64>this.mobility_status,4);
w.integer(<u64>this.feedback_kind,1);
w.u8(this.has_subcommand_status?1:0);
w.integer(<u64>this.subcommand_status,4);
w.u32(<u32>this.native_proto.length);
for(let i=0;i<this.native_proto.length;++i){
w.integer(<u64>this.native_proto[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvFeedback_Response{const value=new Ros2Uwvm_spot_bridgeSrvFeedback_Response();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.unknown=decoded;
}
{
const decoded=<u32>r.integer(4);
value.native_command_id=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.has_full_body_feedback=decoded;
}
{
const decoded=<u32>r.integer(4);
value.full_body_status=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.has_mobility_feedback=decoded;
}
{
const decoded=<u32>r.integer(4);
value.mobility_status=decoded;
}
{
const decoded=<u8>r.integer(1);
value.feedback_kind=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.has_subcommand_status=decoded;
}
{
const decoded=<u32>r.integer(4);
value.subcommand_status=decoded;
}
{
const count=r.u32();if(!r.valid||count>16384){r.valid=false;return value;}
value.native_proto=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.native_proto.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvFeedback_ResponseCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvFeedback_Response>{
static readonly codecIdentity:string="UWVMROS1_dc801501c5f5c993eea419f4a94f5c7eb57a5cfe113d0351a3b1e3fbfd514901";
static readonly maxWireBytes:i32=16429;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvFeedback_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvFeedback_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvFeedback_Response>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvFeedback_Response>();if(bytes.length>16429){out.status=90;return out;}if(bytes.length<45){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvFeedback_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvFeedback_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvFeedback_Response():changetype<Ros2Uwvm_spot_bridgeSrvFeedback_Response>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Lease_Request; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvLease_Request {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field budget_ns, native scalar uint64. */
budget_ns:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.budget_ns,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvLease_Request{const value=new Ros2Uwvm_spot_bridgeSrvLease_Request();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u64>r.integer(8);
value.budget_ns=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvLease_RequestCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvLease_Request>{
static readonly codecIdentity:string="UWVMROS1_a9aacd78dc6ae9d18fb6bb0d185797c6463c882edadf251eb5c21e3aa3c538a1";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvLease_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvLease_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvLease_Request>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvLease_Request>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvLease_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvLease_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvLease_Request():changetype<Ros2Uwvm_spot_bridgeSrvLease_Request>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Lease_Response; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvLease_Response {
static readonly maxWireBytes:i32=22;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=22;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field error, native scalar uint32. */
error:u32=0;
/** Exact native field unknown, native scalar bool. */
unknown:bool=false;
/** Exact native field provider_holds_body_lease, native scalar bool. */
provider_holds_body_lease:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>22-n)return -90;n+=8;
if(n>22)return -90;
}
{
if(8>22-n)return -90;n+=8;
if(n>22)return -90;
}
{
if(4>22-n)return -90;n+=4;
if(n>22)return -90;
}
{
if(1>22-n)return -90;n+=1;
if(n>22)return -90;
}
{
if(1>22-n)return -90;n+=1;
if(n>22)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.error,4);
w.u8(this.unknown?1:0);
w.u8(this.provider_holds_body_lease?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvLease_Response{const value=new Ros2Uwvm_spot_bridgeSrvLease_Response();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.unknown=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.provider_holds_body_lease=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvLease_ResponseCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvLease_Response>{
static readonly codecIdentity:string="UWVMROS1_ce875eb701b607c01f8a19dc8fcff607829824ad99276f906c7fc8fc5567f6d3";
static readonly maxWireBytes:i32=22;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvLease_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvLease_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvLease_Response>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvLease_Response>();if(bytes.length>22){out.status=90;return out;}if(bytes.length<22){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvLease_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvLease_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvLease_Response():changetype<Ros2Uwvm_spot_bridgeSrvLease_Response>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/State_Request; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvState_Request {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field budget_ns, native scalar uint64. */
budget_ns:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.budget_ns,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvState_Request{const value=new Ros2Uwvm_spot_bridgeSrvState_Request();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u64>r.integer(8);
value.budget_ns=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvState_RequestCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvState_Request>{
static readonly codecIdentity:string="UWVMROS1_9becf278bc168ed6d5b15fe7f1ec35b83e274de8a5173cb2335948306bc7a47e";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvState_Request>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvState_Request>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvState_Request():changetype<Ros2Uwvm_spot_bridgeSrvState_Request>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/State_Response; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvState_Response {
static readonly maxWireBytes:i32=21433;
static readonly aggregateWireBytes:i32=32768;
static readonly cartesianWireBytes:u64=21433;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field error, native scalar uint32. */
error:u32=0;
/** Exact native field unknown, native scalar bool. */
unknown:bool=false;
/** Exact native field acquisition_sec, native scalar int64. */
acquisition_sec:i64=0;
/** Exact native field acquisition_nsec, native scalar uint32. */
acquisition_nsec:u32=0;
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field joint_position, native scalar float64. */
joint_position:Array<f64>=new Array<f64>();
/** Exact native field joint_velocity, native scalar float64. */
joint_velocity:Array<f64>=new Array<f64>();
/** Exact native field joint_load, native scalar float64. */
joint_load:Array<f64>=new Array<f64>();
/** Exact native field motor_power_state, native scalar uint32. */
motor_power_state:u32=0;
/** Exact native field native_proto, native scalar uint8. */
native_proto:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
{
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
{
if(4>21433-n)return -90;n+=4;
if(n>21433)return -90;
}
{
if(1>21433-n)return -90;n+=1;
if(n>21433)return -90;
}
{
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
{
if(4>21433-n)return -90;n+=4;
if(n>21433)return -90;
}
{
if(this.joint_names.length>32)return -22;
if(4>21433-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>128)return -22;if(4+size>21433-n)return -90;n+=4+size;
if(n>21433)return -90;
}
}
{
if(this.joint_position.length>32)return -22;
if(4>21433-n)return -90;n+=4;
for(let i=0;i<this.joint_position.length;++i){
if(!isFinite(this.joint_position[i]))return -22;
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
}
{
if(this.joint_velocity.length>32)return -22;
if(4>21433-n)return -90;n+=4;
for(let i=0;i<this.joint_velocity.length;++i){
if(!isFinite(this.joint_velocity[i]))return -22;
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
}
{
if(this.joint_load.length>32)return -22;
if(4>21433-n)return -90;n+=4;
for(let i=0;i<this.joint_load.length;++i){
if(!isFinite(this.joint_load[i]))return -22;
if(8>21433-n)return -90;n+=8;
if(n>21433)return -90;
}
}
{
if(4>21433-n)return -90;n+=4;
if(n>21433)return -90;
}
{
if(this.native_proto.length>16384)return -22;
if(4>21433-n)return -90;n+=4;
for(let i=0;i<this.native_proto.length;++i){
if(1>21433-n)return -90;n+=1;
if(n>21433)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.error,4);
w.u8(this.unknown?1:0);
w.integer(<u64>this.acquisition_sec,8);
w.integer(<u64>this.acquisition_nsec,4);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
w.u32(<u32>this.joint_position.length);
for(let i=0;i<this.joint_position.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_position[i]),8);
}
w.u32(<u32>this.joint_velocity.length);
for(let i=0;i<this.joint_velocity.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_velocity[i]),8);
}
w.u32(<u32>this.joint_load.length);
for(let i=0;i<this.joint_load.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_load[i]),8);
}
w.integer(<u64>this.motor_power_state,4);
w.u32(<u32>this.native_proto.length);
for(let i=0;i<this.native_proto.length;++i){
w.integer(<u64>this.native_proto[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvState_Response{const value=new Ros2Uwvm_spot_bridgeSrvState_Response();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.unknown=decoded;
}
{
const decoded=<i64>r.integer(8);
value.acquisition_sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.acquisition_nsec=decoded;
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.joint_position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_position.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.joint_velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_velocity.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.joint_load=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_load.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.motor_power_state=decoded;
}
{
const count=r.u32();if(!r.valid||count>16384){r.valid=false;return value;}
value.native_proto=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.native_proto.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvState_ResponseCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvState_Response>{
static readonly codecIdentity:string="UWVMROS1_8ffde1e9e2470975f03e038dbd51cf288c506671b4f3fe64b59223cb9711fb6c";
static readonly maxWireBytes:i32=21433;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvState_Response>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvState_Response>();if(bytes.length>21433){out.status=90;return out;}if(bytes.length<57){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvState_Response():changetype<Ros2Uwvm_spot_bridgeSrvState_Response>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Velocity_Request; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvVelocity_Request {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field budget_ns, native scalar uint64. */
budget_ns:u64=0;
/** Exact native field x_mps, native scalar float64. */
x_mps:f64=0;
/** Exact native field y_mps, native scalar float64. */
y_mps:f64=0;
/** Exact native field yaw_rps, native scalar float64. */
yaw_rps:f64=0;
/** Exact native field duration_ns, native scalar uint64. */
duration_ns:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(!isFinite(this.x_mps))return -22;
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(!isFinite(this.y_mps))return -22;
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(!isFinite(this.yaw_rps))return -22;
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
{
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.budget_ns,8);
w.integer(<u64>reinterpret<u64>(this.x_mps),8);
w.integer(<u64>reinterpret<u64>(this.y_mps),8);
w.integer(<u64>reinterpret<u64>(this.yaw_rps),8);
w.integer(<u64>this.duration_ns,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvVelocity_Request{const value=new Ros2Uwvm_spot_bridgeSrvVelocity_Request();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u64>r.integer(8);
value.budget_ns=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x_mps=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y_mps=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.yaw_rps=decoded;
}
{
const decoded=<u64>r.integer(8);
value.duration_ns=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvVelocity_RequestCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvVelocity_Request>{
static readonly codecIdentity:string="UWVMROS1_ffe3b1dec91a25cd84d221ba4ebeae576bd7d6b96f258579c0b3d280cad79f8d";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvVelocity_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvVelocity_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvVelocity_Request>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvVelocity_Request>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvVelocity_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvVelocity_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvVelocity_Request():changetype<Ros2Uwvm_spot_bridgeSrvVelocity_Request>(out.value);}
}
/** Complete original uwvm_spot_bridge/srv/Velocity_Response; spellings are published in generation proof. */
export class Ros2Uwvm_spot_bridgeSrvVelocity_Response {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field token, native scalar uint64. */
token:u64=0;
/** Exact native field nonce, native scalar uint64. */
nonce:u64=0;
/** Exact native field error, native scalar uint32. */
error:u32=0;
/** Exact native field unknown, native scalar bool. */
unknown:bool=false;
/** Exact native field native_accepted, native scalar bool. */
native_accepted:bool=false;
/** Exact native field native_command_id, native scalar uint32. */
native_command_id:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>26-n)return -90;n+=8;
if(n>26)return -90;
}
{
if(8>26-n)return -90;n+=8;
if(n>26)return -90;
}
{
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.token,8);
w.integer(<u64>this.nonce,8);
w.integer(<u64>this.error,4);
w.u8(this.unknown?1:0);
w.u8(this.native_accepted?1:0);
w.integer(<u64>this.native_command_id,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_spot_bridgeSrvVelocity_Response{const value=new Ros2Uwvm_spot_bridgeSrvVelocity_Response();
{
const decoded=<u64>r.integer(8);
value.token=decoded;
}
{
const decoded=<u64>r.integer(8);
value.nonce=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.unknown=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.native_accepted=decoded;
}
{
const decoded=<u32>r.integer(4);
value.native_command_id=decoded;
}
return value;}
}
export class Ros2Uwvm_spot_bridgeSrvVelocity_ResponseCodec implements VendorCodec<Ros2Uwvm_spot_bridgeSrvVelocity_Response>{
static readonly codecIdentity:string="UWVMROS1_031f37beeb5a03c4e65d4be8fd70bc680408d110a020e066bea593d86498401a";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Uwvm_spot_bridgeSrvVelocity_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_spot_bridgeSrvVelocity_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_spot_bridgeSrvVelocity_Response>{const out=new VendorDecoded<Ros2Uwvm_spot_bridgeSrvVelocity_Response>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_spot_bridgeSrvVelocity_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_spot_bridgeSrvVelocity_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_spot_bridgeSrvVelocity_Response():changetype<Ros2Uwvm_spot_bridgeSrvVelocity_Response>(out.value);}
}
