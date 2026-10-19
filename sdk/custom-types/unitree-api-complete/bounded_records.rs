//! Generated bounded-v2 canonical values; no I/O, threads, clocks or authority.
//! Enable the existing SDK alloc feature. Containers/UTF-8 are validated before
//! output allocation; decoder lengths are checked before bounded allocations.
//! Empty interface components are zero-sized semantic values, not native byte copies.
extern crate alloc;
use alloc::{vec::Vec,string::String};
use roswasm::{Error,codes};
use roswasm::typed::Canonical;
fn invalid()->Error {Error::new(codes::INVALID)}
fn add_size(n:usize,m:usize)->Result<usize,Error>{n.checked_add(m).filter(|v|*v<=32768).ok_or(Error::new(codes::TOO_LARGE))}
fn take<'a>(bytes:&'a [u8],at:&mut usize,n:usize)->Result<&'a [u8],Error>{if *at>bytes.len()||n>bytes.len()-*at{return Err(invalid());}let start=*at;*at+=n;Ok(&bytes[start..*at])}
fn read_count(bytes:&[u8],at:&mut usize)->Result<usize,Error>{Ok(u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?) as usize)}
fn put(out:&mut [u8],at:&mut usize,bytes:&[u8]){out[*at..*at+bytes.len()].copy_from_slice(bytes);*at+=bytes.len();}
/// Complete bounded value for ROS2 unitree_api/msg/RequestIdentity.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgRequestIdentity {
  /// Declared id; canonical bounds are part of CODEC_IDENTITY.
  pub id:i64,
  /// Declared api_id; canonical bounds are part of CODEC_IDENTITY.
  pub api_id:i64,
}
impl Default for Ros2Unitree_apiMsgRequestIdentity {fn default()->Self {Self {
id:Default::default(),
api_id:Default::default(),
}}}
impl Ros2Unitree_apiMsgRequestIdentity {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e3daab89abcdaf6526511a189cfcc0c78aaebe8aefc53814ba77f6eb80cdc4e2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.id.to_le_bytes());
put(out,at,&self.api_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.id=decoded;
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.api_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgRequestIdentity {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/RequestLease.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgRequestLease {
  /// Declared id; canonical bounds are part of CODEC_IDENTITY.
  pub id:i64,
}
impl Default for Ros2Unitree_apiMsgRequestLease {fn default()->Self {Self {
id:Default::default(),
}}}
impl Ros2Unitree_apiMsgRequestLease {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3dcb1fe2998c880e2d9196967da172ccd9060ebc7f6d2feb8682dc3d104c2e6e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.id=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgRequestLease {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/RequestPolicy.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgRequestPolicy {
  /// Declared priority; canonical bounds are part of CODEC_IDENTITY.
  pub priority:i32,
  /// Declared noreply; canonical bounds are part of CODEC_IDENTITY.
  pub noreply:bool,
}
impl Default for Ros2Unitree_apiMsgRequestPolicy {fn default()->Self {Self {
priority:Default::default(),
noreply:Default::default(),
}}}
impl Ros2Unitree_apiMsgRequestPolicy {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_61f1a1eafec2f694d24cf798963dcb5a0c8f40e9791db5a502f0736cc8587bb8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.priority.to_le_bytes());
put(out,at,&[u8::from(self.noreply)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.priority=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.noreply=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgRequestPolicy {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/RequestHeader.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgRequestHeader {
  /// Declared identity; canonical bounds are part of CODEC_IDENTITY.
  pub identity:Ros2Unitree_apiMsgRequestIdentity,
  /// Declared lease; canonical bounds are part of CODEC_IDENTITY.
  pub lease:Ros2Unitree_apiMsgRequestLease,
  /// Declared policy; canonical bounds are part of CODEC_IDENTITY.
  pub policy:Ros2Unitree_apiMsgRequestPolicy,
}
impl Default for Ros2Unitree_apiMsgRequestHeader {fn default()->Self {Self {
identity:Default::default(),
lease:Default::default(),
policy:Default::default(),
}}}
impl Ros2Unitree_apiMsgRequestHeader {
  pub const MAX_WIRE_BYTES:usize=29;
  pub const CARTESIAN_WIRE_BYTES:u64=29;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c8e8b83e2cdc978c5279483dbfac8d4185a3e05c87799b424643f0fcb02d0fd0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.identity.wire_size()?)?;
n=add_size(n,self.lease.wire_size()?)?;
n=add_size(n,self.policy.wire_size()?)?;
if n>29{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.identity.write_fields(out,at);
self.lease.write_fields(out,at);
self.policy.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Unitree_apiMsgRequestIdentity::read_fields(bytes,at)?;
value.identity=decoded;
let decoded=Ros2Unitree_apiMsgRequestLease::read_fields(bytes,at)?;
value.lease=decoded;
let decoded=Ros2Unitree_apiMsgRequestPolicy::read_fields(bytes,at)?;
value.policy=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgRequestHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>29{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<29{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgRequest {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Unitree_apiMsgRequestHeader,
  /// Declared parameter; canonical bounds are part of CODEC_IDENTITY.
  pub parameter:String,
  /// Declared binary; canonical bounds are part of CODEC_IDENTITY.
  pub binary:Vec<u8>,
}
impl Default for Ros2Unitree_apiMsgRequest {fn default()->Self {Self {
header:Default::default(),
parameter:Default::default(),
binary:Default::default(),
}}}
impl Ros2Unitree_apiMsgRequest {
  pub const MAX_WIRE_BYTES:usize=6181;
  pub const CARTESIAN_WIRE_BYTES:u64=6181;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.parameter.len()>4096||self.parameter.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.parameter.len())?;
if self.binary.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.binary {
n=add_size(n,1)?;
}
if n>6181{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.parameter.len() as u32).to_le_bytes());put(out,at,self.parameter.as_bytes());
put(out,at,&(self.binary.len() as u32).to_le_bytes());
for element in &self.binary {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Unitree_apiMsgRequestHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.parameter=decoded;
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.binary=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.binary.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6181{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<37{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/ResponseStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgResponseStatus {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:i32,
}
impl Default for Ros2Unitree_apiMsgResponseStatus {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Unitree_apiMsgResponseStatus {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aaad17e09d5d6549455f69060d1bb03ec81cddfc0ede323a5f2365a6c1a7b7b1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.code.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgResponseStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/ResponseHeader.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgResponseHeader {
  /// Declared identity; canonical bounds are part of CODEC_IDENTITY.
  pub identity:Ros2Unitree_apiMsgRequestIdentity,
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:Ros2Unitree_apiMsgResponseStatus,
}
impl Default for Ros2Unitree_apiMsgResponseHeader {fn default()->Self {Self {
identity:Default::default(),
status:Default::default(),
}}}
impl Ros2Unitree_apiMsgResponseHeader {
  pub const MAX_WIRE_BYTES:usize=20;
  pub const CARTESIAN_WIRE_BYTES:u64=20;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_20ddef706f0be7d7e2e356ab2222b6e198bd36fd6d8a81706553d5a883de85e0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.identity.wire_size()?)?;
n=add_size(n,self.status.wire_size()?)?;
if n>20{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.identity.write_fields(out,at);
self.status.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Unitree_apiMsgRequestIdentity::read_fields(bytes,at)?;
value.identity=decoded;
let decoded=Ros2Unitree_apiMsgResponseStatus::read_fields(bytes,at)?;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgResponseHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_api/msg/Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_apiMsgResponse {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Unitree_apiMsgResponseHeader,
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:String,
  /// Declared binary; canonical bounds are part of CODEC_IDENTITY.
  pub binary:Vec<i8>,
}
impl Default for Ros2Unitree_apiMsgResponse {fn default()->Self {Self {
header:Default::default(),
data:Default::default(),
binary:Default::default(),
}}}
impl Ros2Unitree_apiMsgResponse {
  pub const MAX_WIRE_BYTES:usize=6172;
  pub const CARTESIAN_WIRE_BYTES:u64=6172;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.data.len()>4096||self.data.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.data.len())?;
if self.binary.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.binary {
n=add_size(n,1)?;
}
if n>6172{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.data.len() as u32).to_le_bytes());put(out,at,self.data.as_bytes());
put(out,at,&(self.binary.len() as u32).to_le_bytes());
for element in &self.binary {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Unitree_apiMsgResponseHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.data=decoded;
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.binary=Vec::with_capacity(count);
for i in 0..count {
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.binary.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_apiMsgResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6172{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
