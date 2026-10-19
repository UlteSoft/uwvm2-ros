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
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Command_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvCommand_Request {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared budget_ns; canonical bounds are part of CODEC_IDENTITY.
  pub budget_ns:u64,
}
impl Default for Ros2Uwvm_spot_bridgeSrvCommand_Request {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
budget_ns:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvCommand_Request {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_29996d4dacb4ea847c5d629cb634d4aa5e2451627d2469d6cdf301cc007968f8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.budget_ns.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.budget_ns=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvCommand_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Command_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvCommand_Response {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:u32,
  /// Declared unknown; canonical bounds are part of CODEC_IDENTITY.
  pub unknown:bool,
  /// Declared native_accepted; canonical bounds are part of CODEC_IDENTITY.
  pub native_accepted:bool,
  /// Declared native_command_id; canonical bounds are part of CODEC_IDENTITY.
  pub native_command_id:u32,
}
impl Default for Ros2Uwvm_spot_bridgeSrvCommand_Response {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
error:Default::default(),
unknown:Default::default(),
native_accepted:Default::default(),
native_command_id:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvCommand_Response {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e35556e7439c8d5ca79cb0f4ad23cf73c85a8a79d1382cde398dd85082dd1733";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.error.to_le_bytes());
put(out,at,&[u8::from(self.unknown)]);
put(out,at,&[u8::from(self.native_accepted)]);
put(out,at,&self.native_command_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.unknown=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.native_accepted=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.native_command_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvCommand_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Feedback_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvFeedback_Request {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared budget_ns; canonical bounds are part of CODEC_IDENTITY.
  pub budget_ns:u64,
  /// Declared command_token; canonical bounds are part of CODEC_IDENTITY.
  pub command_token:u64,
}
impl Default for Ros2Uwvm_spot_bridgeSrvFeedback_Request {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
budget_ns:Default::default(),
command_token:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvFeedback_Request {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_de64ce283f49f5c324912ec262f7d15b6fd828879c01039d455c765743c45ba3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.budget_ns.to_le_bytes());
put(out,at,&self.command_token.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.budget_ns=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.command_token=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvFeedback_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Feedback_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvFeedback_Response {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:u32,
  /// Declared unknown; canonical bounds are part of CODEC_IDENTITY.
  pub unknown:bool,
  /// Declared native_command_id; canonical bounds are part of CODEC_IDENTITY.
  pub native_command_id:u32,
  /// Declared has_full_body_feedback; canonical bounds are part of CODEC_IDENTITY.
  pub has_full_body_feedback:bool,
  /// Declared full_body_status; canonical bounds are part of CODEC_IDENTITY.
  pub full_body_status:u32,
  /// Declared has_mobility_feedback; canonical bounds are part of CODEC_IDENTITY.
  pub has_mobility_feedback:bool,
  /// Declared mobility_status; canonical bounds are part of CODEC_IDENTITY.
  pub mobility_status:u32,
  /// Declared feedback_kind; canonical bounds are part of CODEC_IDENTITY.
  pub feedback_kind:u8,
  /// Declared has_subcommand_status; canonical bounds are part of CODEC_IDENTITY.
  pub has_subcommand_status:bool,
  /// Declared subcommand_status; canonical bounds are part of CODEC_IDENTITY.
  pub subcommand_status:u32,
  /// Declared native_proto; canonical bounds are part of CODEC_IDENTITY.
  pub native_proto:Vec<u8>,
}
impl Default for Ros2Uwvm_spot_bridgeSrvFeedback_Response {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
error:Default::default(),
unknown:Default::default(),
native_command_id:Default::default(),
has_full_body_feedback:Default::default(),
full_body_status:Default::default(),
has_mobility_feedback:Default::default(),
mobility_status:Default::default(),
feedback_kind:Default::default(),
has_subcommand_status:Default::default(),
subcommand_status:Default::default(),
native_proto:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvFeedback_Response {
  pub const MAX_WIRE_BYTES:usize=16429;
  pub const CARTESIAN_WIRE_BYTES:u64=16429;
  pub const AGGREGATE_WIRE_BYTES:usize=32768;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dc801501c5f5c993eea419f4a94f5c7eb57a5cfe113d0351a3b1e3fbfd514901";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
if self.native_proto.len()>16384{return Err(invalid());}n=add_size(n,4)?;
for element in &self.native_proto {
n=add_size(n,1)?;
}
if n>16429{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.error.to_le_bytes());
put(out,at,&[u8::from(self.unknown)]);
put(out,at,&self.native_command_id.to_le_bytes());
put(out,at,&[u8::from(self.has_full_body_feedback)]);
put(out,at,&self.full_body_status.to_le_bytes());
put(out,at,&[u8::from(self.has_mobility_feedback)]);
put(out,at,&self.mobility_status.to_le_bytes());
put(out,at,&self.feedback_kind.to_le_bytes());
put(out,at,&[u8::from(self.has_subcommand_status)]);
put(out,at,&self.subcommand_status.to_le_bytes());
put(out,at,&(self.native_proto.len() as u32).to_le_bytes());
for element in &self.native_proto {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.unknown=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.native_command_id=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.has_full_body_feedback=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.full_body_status=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.has_mobility_feedback=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.mobility_status=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.feedback_kind=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.has_subcommand_status=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.subcommand_status=decoded;
{let count=read_count(bytes,at)?;if count>16384{return Err(invalid());}
value.native_proto=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.native_proto.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvFeedback_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16429{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<45{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Lease_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvLease_Request {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared budget_ns; canonical bounds are part of CODEC_IDENTITY.
  pub budget_ns:u64,
}
impl Default for Ros2Uwvm_spot_bridgeSrvLease_Request {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
budget_ns:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvLease_Request {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a9aacd78dc6ae9d18fb6bb0d185797c6463c882edadf251eb5c21e3aa3c538a1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.budget_ns.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.budget_ns=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvLease_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Lease_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvLease_Response {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:u32,
  /// Declared unknown; canonical bounds are part of CODEC_IDENTITY.
  pub unknown:bool,
  /// Declared provider_holds_body_lease; canonical bounds are part of CODEC_IDENTITY.
  pub provider_holds_body_lease:bool,
}
impl Default for Ros2Uwvm_spot_bridgeSrvLease_Response {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
error:Default::default(),
unknown:Default::default(),
provider_holds_body_lease:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvLease_Response {
  pub const MAX_WIRE_BYTES:usize=22;
  pub const CARTESIAN_WIRE_BYTES:u64=22;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ce875eb701b607c01f8a19dc8fcff607829824ad99276f906c7fc8fc5567f6d3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>22{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.error.to_le_bytes());
put(out,at,&[u8::from(self.unknown)]);
put(out,at,&[u8::from(self.provider_holds_body_lease)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.unknown=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.provider_holds_body_lease=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvLease_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>22{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<22{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/State_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvState_Request {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared budget_ns; canonical bounds are part of CODEC_IDENTITY.
  pub budget_ns:u64,
}
impl Default for Ros2Uwvm_spot_bridgeSrvState_Request {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
budget_ns:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvState_Request {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9becf278bc168ed6d5b15fe7f1ec35b83e274de8a5173cb2335948306bc7a47e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.budget_ns.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.budget_ns=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/State_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvState_Response {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:u32,
  /// Declared unknown; canonical bounds are part of CODEC_IDENTITY.
  pub unknown:bool,
  /// Declared acquisition_sec; canonical bounds are part of CODEC_IDENTITY.
  pub acquisition_sec:i64,
  /// Declared acquisition_nsec; canonical bounds are part of CODEC_IDENTITY.
  pub acquisition_nsec:u32,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared joint_position; canonical bounds are part of CODEC_IDENTITY.
  pub joint_position:Vec<f64>,
  /// Declared joint_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub joint_velocity:Vec<f64>,
  /// Declared joint_load; canonical bounds are part of CODEC_IDENTITY.
  pub joint_load:Vec<f64>,
  /// Declared motor_power_state; canonical bounds are part of CODEC_IDENTITY.
  pub motor_power_state:u32,
  /// Declared native_proto; canonical bounds are part of CODEC_IDENTITY.
  pub native_proto:Vec<u8>,
}
impl Default for Ros2Uwvm_spot_bridgeSrvState_Response {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
error:Default::default(),
unknown:Default::default(),
acquisition_sec:Default::default(),
acquisition_nsec:Default::default(),
joint_names:Default::default(),
joint_position:Default::default(),
joint_velocity:Default::default(),
joint_load:Default::default(),
motor_power_state:Default::default(),
native_proto:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvState_Response {
  pub const MAX_WIRE_BYTES:usize=21433;
  pub const CARTESIAN_WIRE_BYTES:u64=21433;
  pub const AGGREGATE_WIRE_BYTES:usize=32768;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8ffde1e9e2470975f03e038dbd51cf288c506671b4f3fe64b59223cb9711fb6c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
if self.joint_names.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.joint_position.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.joint_velocity.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.joint_load.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_load {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,4)?;
if self.native_proto.len()>16384{return Err(invalid());}n=add_size(n,4)?;
for element in &self.native_proto {
n=add_size(n,1)?;
}
if n>21433{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.error.to_le_bytes());
put(out,at,&[u8::from(self.unknown)]);
put(out,at,&self.acquisition_sec.to_le_bytes());
put(out,at,&self.acquisition_nsec.to_le_bytes());
put(out,at,&(self.joint_names.len() as u32).to_le_bytes());
for element in &self.joint_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.joint_position.len() as u32).to_le_bytes());
for element in &self.joint_position {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.joint_velocity.len() as u32).to_le_bytes());
for element in &self.joint_velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.joint_load.len() as u32).to_le_bytes());
for element in &self.joint_load {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.motor_power_state.to_le_bytes());
put(out,at,&(self.native_proto.len() as u32).to_le_bytes());
for element in &self.native_proto {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.unknown=decoded;
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.acquisition_sec=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.acquisition_nsec=decoded;
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.joint_position=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_position.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.joint_velocity=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_velocity.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.joint_load=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_load.push(decoded);
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.motor_power_state=decoded;
{let count=read_count(bytes,at)?;if count>16384{return Err(invalid());}
value.native_proto=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.native_proto.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>21433{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<57{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Velocity_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvVelocity_Request {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared budget_ns; canonical bounds are part of CODEC_IDENTITY.
  pub budget_ns:u64,
  /// Declared x_mps; canonical bounds are part of CODEC_IDENTITY.
  pub x_mps:f64,
  /// Declared y_mps; canonical bounds are part of CODEC_IDENTITY.
  pub y_mps:f64,
  /// Declared yaw_rps; canonical bounds are part of CODEC_IDENTITY.
  pub yaw_rps:f64,
  /// Declared duration_ns; canonical bounds are part of CODEC_IDENTITY.
  pub duration_ns:u64,
}
impl Default for Ros2Uwvm_spot_bridgeSrvVelocity_Request {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
budget_ns:Default::default(),
x_mps:Default::default(),
y_mps:Default::default(),
yaw_rps:Default::default(),
duration_ns:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvVelocity_Request {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ffe3b1dec91a25cd84d221ba4ebeae576bd7d6b96f258579c0b3d280cad79f8d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if !self.x_mps.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.y_mps.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.yaw_rps.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,8)?;
if n>56{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.budget_ns.to_le_bytes());
put(out,at,&self.x_mps.to_le_bytes());
put(out,at,&self.y_mps.to_le_bytes());
put(out,at,&self.yaw_rps.to_le_bytes());
put(out,at,&self.duration_ns.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.budget_ns=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x_mps=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y_mps=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yaw_rps=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.duration_ns=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvVelocity_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_spot_bridge/srv/Velocity_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_spot_bridgeSrvVelocity_Response {
  /// Declared token; canonical bounds are part of CODEC_IDENTITY.
  pub token:u64,
  /// Declared nonce; canonical bounds are part of CODEC_IDENTITY.
  pub nonce:u64,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:u32,
  /// Declared unknown; canonical bounds are part of CODEC_IDENTITY.
  pub unknown:bool,
  /// Declared native_accepted; canonical bounds are part of CODEC_IDENTITY.
  pub native_accepted:bool,
  /// Declared native_command_id; canonical bounds are part of CODEC_IDENTITY.
  pub native_command_id:u32,
}
impl Default for Ros2Uwvm_spot_bridgeSrvVelocity_Response {fn default()->Self {Self {
token:Default::default(),
nonce:Default::default(),
error:Default::default(),
unknown:Default::default(),
native_accepted:Default::default(),
native_command_id:Default::default(),
}}}
impl Ros2Uwvm_spot_bridgeSrvVelocity_Response {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_031f37beeb5a03c4e65d4be8fd70bc680408d110a020e066bea593d86498401a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.token.to_le_bytes());
put(out,at,&self.nonce.to_le_bytes());
put(out,at,&self.error.to_le_bytes());
put(out,at,&[u8::from(self.unknown)]);
put(out,at,&[u8::from(self.native_accepted)]);
put(out,at,&self.native_command_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.token=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.nonce=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.unknown=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.native_accepted=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.native_command_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_spot_bridgeSrvVelocity_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
