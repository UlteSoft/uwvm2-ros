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
/// Complete bounded value for ROS1 abb_egm_msgs/EGMChannelState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_egm_msgsEGMChannelState {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared active; canonical bounds are part of CODEC_IDENTITY.
  pub active:bool,
  /// Declared egm_convergence_met; canonical bounds are part of CODEC_IDENTITY.
  pub egm_convergence_met:bool,
  /// Declared egm_client_state; canonical bounds are part of CODEC_IDENTITY.
  pub egm_client_state:u8,
  /// Declared motor_state; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state:u8,
  /// Declared rapid_execution_state; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_execution_state:u8,
  /// Declared utilization_rate; canonical bounds are part of CODEC_IDENTITY.
  pub utilization_rate:f64,
}
impl Default for Ros1Abb_egm_msgsEGMChannelState {fn default()->Self {Self {
name:Default::default(),
active:Default::default(),
egm_convergence_met:Default::default(),
egm_client_state:Default::default(),
motor_state:Default::default(),
rapid_execution_state:Default::default(),
utilization_rate:Default::default(),
}}}
impl Ros1Abb_egm_msgsEGMChannelState {
  pub const MAX_WIRE_BYTES:usize=81;
  pub const CARTESIAN_WIRE_BYTES:u64=81;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_452d2adde3b1475fe3ff09ea11ff2ec7105a58c32e20d764e0c2ff152cb432f2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>64||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.utilization_rate.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>81{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&[u8::from(self.active)]);
put(out,at,&[u8::from(self.egm_convergence_met)]);
put(out,at,&self.egm_client_state.to_le_bytes());
put(out,at,&self.motor_state.to_le_bytes());
put(out,at,&self.rapid_execution_state.to_le_bytes());
put(out,at,&self.utilization_rate.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.active=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.egm_convergence_met=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.egm_client_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.motor_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.rapid_execution_state=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.utilization_rate=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_egm_msgsEGMChannelState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>81{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 time.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Time {
  /// Declared sec; canonical bounds are part of CODEC_IDENTITY.
  pub sec:u32,
  /// Declared nsec; canonical bounds are part of CODEC_IDENTITY.
  pub nsec:u32,
}
impl Default for Ros1Time {fn default()->Self {Self {
sec:Default::default(),
nsec:Default::default(),
}}}
impl Ros1Time {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.nsec>=1000000000{return Err(invalid());}
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.sec.to_le_bytes());
put(out,at,&self.nsec.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sec=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.nsec=decoded;
if value.nsec>=1000000000{return Err(invalid());}
Ok(value)}
}
impl Canonical for Ros1Time {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 std_msgs/Header.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Std_msgsHeader {
  /// Declared seq; canonical bounds are part of CODEC_IDENTITY.
  pub seq:u32,
  /// Declared stamp; canonical bounds are part of CODEC_IDENTITY.
  pub stamp:Ros1Time,
  /// Declared frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub frame_id:String,
}
impl Default for Ros1Std_msgsHeader {fn default()->Self {Self {
seq:Default::default(),
stamp:Default::default(),
frame_id:Default::default(),
}}}
impl Ros1Std_msgsHeader {
  pub const MAX_WIRE_BYTES:usize=80;
  pub const CARTESIAN_WIRE_BYTES:u64=80;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ca1b8ffce276bdc4f86dccae58ede9f860ffe8778c6d4ffe0fb70675777801db";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,self.stamp.wire_size()?)?;
if self.frame_id.len()>64||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if n>80{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.seq.to_le_bytes());
self.stamp.write_fields(out,at);
put(out,at,&(self.frame_id.len() as u32).to_le_bytes());put(out,at,self.frame_id.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.seq=decoded;
let decoded=Ros1Time::read_fields(bytes,at)?;
value.stamp=decoded;
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
Ok(value)}
}
impl Canonical for Ros1Std_msgsHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>80{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_egm_msgs/EGMState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_egm_msgsEGMState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared egm_channels; canonical bounds are part of CODEC_IDENTITY.
  pub egm_channels:Vec<Ros1Abb_egm_msgsEGMChannelState>,
}
impl Default for Ros1Abb_egm_msgsEGMState {fn default()->Self {Self {
header:Default::default(),
egm_channels:Default::default(),
}}}
impl Ros1Abb_egm_msgsEGMState {
  pub const MAX_WIRE_BYTES:usize=1380;
  pub const CARTESIAN_WIRE_BYTES:u64=1380;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f7fe1d3924dc4e497c58aab27da69cfa6ab1a11f64b970b42e2d040a1312c4d8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.egm_channels.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.egm_channels {
n=add_size(n,element.wire_size()?)?;
}
if n>1380{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.egm_channels.len() as u32).to_le_bytes());
for element in &self.egm_channels {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.egm_channels=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Abb_egm_msgsEGMChannelState::read_fields(bytes,at)?;
value.egm_channels.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros1Abb_egm_msgsEGMState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1380{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/ConfData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsConfData {
  /// Declared cf1; canonical bounds are part of CODEC_IDENTITY.
  pub cf1:f32,
  /// Declared cf4; canonical bounds are part of CODEC_IDENTITY.
  pub cf4:f32,
  /// Declared cf6; canonical bounds are part of CODEC_IDENTITY.
  pub cf6:f32,
  /// Declared cfx; canonical bounds are part of CODEC_IDENTITY.
  pub cfx:f32,
}
impl Default for Ros1Abb_rapid_msgsConfData {fn default()->Self {Self {
cf1:Default::default(),
cf4:Default::default(),
cf6:Default::default(),
cfx:Default::default(),
}}}
impl Ros1Abb_rapid_msgsConfData {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_293eb01e8fa150a8a3c741626b76804255d0eb321a0484aac402201447392af2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.cf1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cf4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cf6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cfx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.cf1.to_le_bytes());
put(out,at,&self.cf4.to_le_bytes());
put(out,at,&self.cf6.to_le_bytes());
put(out,at,&self.cfx.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cf1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cf4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cf6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cfx=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsConfData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/ExtJoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsExtJoint {
  /// Declared eax_a; canonical bounds are part of CODEC_IDENTITY.
  pub eax_a:f32,
  /// Declared eax_b; canonical bounds are part of CODEC_IDENTITY.
  pub eax_b:f32,
  /// Declared eax_c; canonical bounds are part of CODEC_IDENTITY.
  pub eax_c:f32,
  /// Declared eax_d; canonical bounds are part of CODEC_IDENTITY.
  pub eax_d:f32,
  /// Declared eax_e; canonical bounds are part of CODEC_IDENTITY.
  pub eax_e:f32,
  /// Declared eax_f; canonical bounds are part of CODEC_IDENTITY.
  pub eax_f:f32,
}
impl Default for Ros1Abb_rapid_msgsExtJoint {fn default()->Self {Self {
eax_a:Default::default(),
eax_b:Default::default(),
eax_c:Default::default(),
eax_d:Default::default(),
eax_e:Default::default(),
eax_f:Default::default(),
}}}
impl Ros1Abb_rapid_msgsExtJoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_653a47c44ab85151fd9e6215901fb154962c32fe1082386733ec6205eaae613a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.eax_a.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.eax_b.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.eax_c.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.eax_d.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.eax_e.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.eax_f.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.eax_a.to_le_bytes());
put(out,at,&self.eax_b.to_le_bytes());
put(out,at,&self.eax_c.to_le_bytes());
put(out,at,&self.eax_d.to_le_bytes());
put(out,at,&self.eax_e.to_le_bytes());
put(out,at,&self.eax_f.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_a=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_b=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_c=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_d=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_e=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.eax_f=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsExtJoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/RobJoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsRobJoint {
  /// Declared rax_1; canonical bounds are part of CODEC_IDENTITY.
  pub rax_1:f32,
  /// Declared rax_2; canonical bounds are part of CODEC_IDENTITY.
  pub rax_2:f32,
  /// Declared rax_3; canonical bounds are part of CODEC_IDENTITY.
  pub rax_3:f32,
  /// Declared rax_4; canonical bounds are part of CODEC_IDENTITY.
  pub rax_4:f32,
  /// Declared rax_5; canonical bounds are part of CODEC_IDENTITY.
  pub rax_5:f32,
  /// Declared rax_6; canonical bounds are part of CODEC_IDENTITY.
  pub rax_6:f32,
}
impl Default for Ros1Abb_rapid_msgsRobJoint {fn default()->Self {Self {
rax_1:Default::default(),
rax_2:Default::default(),
rax_3:Default::default(),
rax_4:Default::default(),
rax_5:Default::default(),
rax_6:Default::default(),
}}}
impl Ros1Abb_rapid_msgsRobJoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d45bc0efae8c4f5b26e4043fe537e163c4cc4c6458d1eac9be429c37aaf82f0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.rax_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rax_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rax_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rax_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rax_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rax_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.rax_1.to_le_bytes());
put(out,at,&self.rax_2.to_le_bytes());
put(out,at,&self.rax_3.to_le_bytes());
put(out,at,&self.rax_4.to_le_bytes());
put(out,at,&self.rax_5.to_le_bytes());
put(out,at,&self.rax_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rax_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsRobJoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/JointTarget.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsJointTarget {
  /// Declared robax; canonical bounds are part of CODEC_IDENTITY.
  pub robax:Ros1Abb_rapid_msgsRobJoint,
  /// Declared extax; canonical bounds are part of CODEC_IDENTITY.
  pub extax:Ros1Abb_rapid_msgsExtJoint,
}
impl Default for Ros1Abb_rapid_msgsJointTarget {fn default()->Self {Self {
robax:Default::default(),
extax:Default::default(),
}}}
impl Ros1Abb_rapid_msgsJointTarget {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4a1cc175468da9204c5f2f399b76712617daec21a0d1675a6e73c84ff170dc24";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.robax.wire_size()?)?;
n=add_size(n,self.extax.wire_size()?)?;
if n>48{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.robax.write_fields(out,at);
self.extax.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_rapid_msgsRobJoint::read_fields(bytes,at)?;
value.robax=decoded;
let decoded=Ros1Abb_rapid_msgsExtJoint::read_fields(bytes,at)?;
value.extax=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsJointTarget {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/Pos.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsPos {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
}
impl Default for Ros1Abb_rapid_msgsPos {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Abb_rapid_msgsPos {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_79edd4553fad8a8f833a3399e354c93538d4971bc3820cc14cf540e960356c01";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsPos {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/Orient.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsOrient {
  /// Declared q1; canonical bounds are part of CODEC_IDENTITY.
  pub q1:f32,
  /// Declared q2; canonical bounds are part of CODEC_IDENTITY.
  pub q2:f32,
  /// Declared q3; canonical bounds are part of CODEC_IDENTITY.
  pub q3:f32,
  /// Declared q4; canonical bounds are part of CODEC_IDENTITY.
  pub q4:f32,
}
impl Default for Ros1Abb_rapid_msgsOrient {fn default()->Self {Self {
q1:Default::default(),
q2:Default::default(),
q3:Default::default(),
q4:Default::default(),
}}}
impl Ros1Abb_rapid_msgsOrient {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5a9c29323a5e3bc0c4c0afa7d305738cd52b49ae339e72f6b34b35b165f3d270";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.q1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.q2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.q3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.q4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.q1.to_le_bytes());
put(out,at,&self.q2.to_le_bytes());
put(out,at,&self.q3.to_le_bytes());
put(out,at,&self.q4.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q4=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsOrient {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/LoadData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsLoadData {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cog; canonical bounds are part of CODEC_IDENTITY.
  pub cog:Ros1Abb_rapid_msgsPos,
  /// Declared aom; canonical bounds are part of CODEC_IDENTITY.
  pub aom:Ros1Abb_rapid_msgsOrient,
  /// Declared ix; canonical bounds are part of CODEC_IDENTITY.
  pub ix:f32,
  /// Declared iy; canonical bounds are part of CODEC_IDENTITY.
  pub iy:f32,
  /// Declared iz; canonical bounds are part of CODEC_IDENTITY.
  pub iz:f32,
}
impl Default for Ros1Abb_rapid_msgsLoadData {fn default()->Self {Self {
mass:Default::default(),
cog:Default::default(),
aom:Default::default(),
ix:Default::default(),
iy:Default::default(),
iz:Default::default(),
}}}
impl Ros1Abb_rapid_msgsLoadData {
  pub const MAX_WIRE_BYTES:usize=44;
  pub const CARTESIAN_WIRE_BYTES:u64=44;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2cc985cc188a03e7c6de3d22310802cac1e0a748bae7f828d957436c1e03303d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.cog.wire_size()?)?;
n=add_size(n,self.aom.wire_size()?)?;
if !self.ix.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.iy.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.iz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>44{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
self.cog.write_fields(out,at);
self.aom.write_fields(out,at);
put(out,at,&self.ix.to_le_bytes());
put(out,at,&self.iy.to_le_bytes());
put(out,at,&self.iz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=Ros1Abb_rapid_msgsPos::read_fields(bytes,at)?;
value.cog=decoded;
let decoded=Ros1Abb_rapid_msgsOrient::read_fields(bytes,at)?;
value.aom=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ix=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iy=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iz=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsLoadData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>44{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/Pose.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsPose {
  /// Declared trans; canonical bounds are part of CODEC_IDENTITY.
  pub trans:Ros1Abb_rapid_msgsPos,
  /// Declared rot; canonical bounds are part of CODEC_IDENTITY.
  pub rot:Ros1Abb_rapid_msgsOrient,
}
impl Default for Ros1Abb_rapid_msgsPose {fn default()->Self {Self {
trans:Default::default(),
rot:Default::default(),
}}}
impl Ros1Abb_rapid_msgsPose {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3e0cfbb7e25376639d1ff6e555a86e69f8c9ed177c1ef14b301d90b206a19c99";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trans.wire_size()?)?;
n=add_size(n,self.rot.wire_size()?)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trans.write_fields(out,at);
self.rot.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_rapid_msgsPos::read_fields(bytes,at)?;
value.trans=decoded;
let decoded=Ros1Abb_rapid_msgsOrient::read_fields(bytes,at)?;
value.rot=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsPose {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/RobTarget.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsRobTarget {
  /// Declared trans; canonical bounds are part of CODEC_IDENTITY.
  pub trans:Ros1Abb_rapid_msgsPos,
  /// Declared rot; canonical bounds are part of CODEC_IDENTITY.
  pub rot:Ros1Abb_rapid_msgsOrient,
  /// Declared robconf; canonical bounds are part of CODEC_IDENTITY.
  pub robconf:Ros1Abb_rapid_msgsConfData,
  /// Declared extax; canonical bounds are part of CODEC_IDENTITY.
  pub extax:Ros1Abb_rapid_msgsExtJoint,
}
impl Default for Ros1Abb_rapid_msgsRobTarget {fn default()->Self {Self {
trans:Default::default(),
rot:Default::default(),
robconf:Default::default(),
extax:Default::default(),
}}}
impl Ros1Abb_rapid_msgsRobTarget {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0abb3aee8b1303ccf9419f2774829d1231de3ac033e4b644d585f195972f09ce";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trans.wire_size()?)?;
n=add_size(n,self.rot.wire_size()?)?;
n=add_size(n,self.robconf.wire_size()?)?;
n=add_size(n,self.extax.wire_size()?)?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trans.write_fields(out,at);
self.rot.write_fields(out,at);
self.robconf.write_fields(out,at);
self.extax.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_rapid_msgsPos::read_fields(bytes,at)?;
value.trans=decoded;
let decoded=Ros1Abb_rapid_msgsOrient::read_fields(bytes,at)?;
value.rot=decoded;
let decoded=Ros1Abb_rapid_msgsConfData::read_fields(bytes,at)?;
value.robconf=decoded;
let decoded=Ros1Abb_rapid_msgsExtJoint::read_fields(bytes,at)?;
value.extax=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsRobTarget {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<68{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/ToolData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsToolData {
  /// Declared robhold; canonical bounds are part of CODEC_IDENTITY.
  pub robhold:bool,
  /// Declared tframe; canonical bounds are part of CODEC_IDENTITY.
  pub tframe:Ros1Abb_rapid_msgsPose,
  /// Declared tload; canonical bounds are part of CODEC_IDENTITY.
  pub tload:Ros1Abb_rapid_msgsLoadData,
}
impl Default for Ros1Abb_rapid_msgsToolData {fn default()->Self {Self {
robhold:Default::default(),
tframe:Default::default(),
tload:Default::default(),
}}}
impl Ros1Abb_rapid_msgsToolData {
  pub const MAX_WIRE_BYTES:usize=73;
  pub const CARTESIAN_WIRE_BYTES:u64=73;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e2feaee4bda891f594542384d70883184262777e359fb037a648b7526ef76cba";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,self.tframe.wire_size()?)?;
n=add_size(n,self.tload.wire_size()?)?;
if n>73{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.robhold)]);
self.tframe.write_fields(out,at);
self.tload.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.robhold=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.tframe=decoded;
let decoded=Ros1Abb_rapid_msgsLoadData::read_fields(bytes,at)?;
value.tload=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsToolData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>73{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<73{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_msgs/WObjData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_msgsWObjData {
  /// Declared robhold; canonical bounds are part of CODEC_IDENTITY.
  pub robhold:bool,
  /// Declared ufprog; canonical bounds are part of CODEC_IDENTITY.
  pub ufprog:bool,
  /// Declared ufmec; canonical bounds are part of CODEC_IDENTITY.
  pub ufmec:String,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:Ros1Abb_rapid_msgsPose,
  /// Declared oframe; canonical bounds are part of CODEC_IDENTITY.
  pub oframe:Ros1Abb_rapid_msgsPose,
}
impl Default for Ros1Abb_rapid_msgsWObjData {fn default()->Self {Self {
robhold:Default::default(),
ufprog:Default::default(),
ufmec:Default::default(),
uframe:Default::default(),
oframe:Default::default(),
}}}
impl Ros1Abb_rapid_msgsWObjData {
  pub const MAX_WIRE_BYTES:usize=574;
  pub const CARTESIAN_WIRE_BYTES:u64=574;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6de4cf654a15ef3b63d9b0f3d0984438f21963c10ffd1b00238113b6fae8454f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if self.ufmec.len()>512||self.ufmec.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.ufmec.len())?;
n=add_size(n,self.uframe.wire_size()?)?;
n=add_size(n,self.oframe.wire_size()?)?;
if n>574{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.robhold)]);
put(out,at,&[u8::from(self.ufprog)]);
put(out,at,&(self.ufmec.len() as u32).to_le_bytes());put(out,at,self.ufmec.as_bytes());
self.uframe.write_fields(out,at);
self.oframe.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.robhold=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.ufprog=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.ufmec=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.uframe=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.oframe=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_msgsWObjData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>574{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<62{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/EGMActivateSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings {
  /// Declared tool; canonical bounds are part of CODEC_IDENTITY.
  pub tool:Ros1Abb_rapid_msgsToolData,
  /// Declared wobj; canonical bounds are part of CODEC_IDENTITY.
  pub wobj:Ros1Abb_rapid_msgsWObjData,
  /// Declared correction_frame; canonical bounds are part of CODEC_IDENTITY.
  pub correction_frame:Ros1Abb_rapid_msgsPose,
  /// Declared sensor_frame; canonical bounds are part of CODEC_IDENTITY.
  pub sensor_frame:Ros1Abb_rapid_msgsPose,
  /// Declared cond_min_max; canonical bounds are part of CODEC_IDENTITY.
  pub cond_min_max:f32,
  /// Declared lp_filter; canonical bounds are part of CODEC_IDENTITY.
  pub lp_filter:f32,
  /// Declared sample_rate; canonical bounds are part of CODEC_IDENTITY.
  pub sample_rate:u8,
  /// Declared max_speed_deviation; canonical bounds are part of CODEC_IDENTITY.
  pub max_speed_deviation:f32,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings {fn default()->Self {Self {
tool:Default::default(),
wobj:Default::default(),
correction_frame:Default::default(),
sensor_frame:Default::default(),
cond_min_max:Default::default(),
lp_filter:Default::default(),
sample_rate:Default::default(),
max_speed_deviation:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings {
  pub const MAX_WIRE_BYTES:usize=716;
  pub const CARTESIAN_WIRE_BYTES:u64=716;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c50be5682021e5d1a7a938e6fe3595cec7c0b0ae727070c0924da731c2c73d4d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.tool.wire_size()?)?;
n=add_size(n,self.wobj.wire_size()?)?;
n=add_size(n,self.correction_frame.wire_size()?)?;
n=add_size(n,self.sensor_frame.wire_size()?)?;
if !self.cond_min_max.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.lp_filter.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.max_speed_deviation.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>716{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.tool.write_fields(out,at);
self.wobj.write_fields(out,at);
self.correction_frame.write_fields(out,at);
self.sensor_frame.write_fields(out,at);
put(out,at,&self.cond_min_max.to_le_bytes());
put(out,at,&self.lp_filter.to_le_bytes());
put(out,at,&self.sample_rate.to_le_bytes());
put(out,at,&self.max_speed_deviation.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_rapid_msgsToolData::read_fields(bytes,at)?;
value.tool=decoded;
let decoded=Ros1Abb_rapid_msgsWObjData::read_fields(bytes,at)?;
value.wobj=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.correction_frame=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.sensor_frame=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cond_min_max=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lp_filter=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.sample_rate=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.max_speed_deviation=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>716{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<204{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/EGMRunSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsEGMRunSettings {
  /// Declared cond_time; canonical bounds are part of CODEC_IDENTITY.
  pub cond_time:f32,
  /// Declared ramp_in_time; canonical bounds are part of CODEC_IDENTITY.
  pub ramp_in_time:f32,
  /// Declared offset; canonical bounds are part of CODEC_IDENTITY.
  pub offset:Ros1Abb_rapid_msgsPose,
  /// Declared pos_corr_gain; canonical bounds are part of CODEC_IDENTITY.
  pub pos_corr_gain:f32,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsEGMRunSettings {fn default()->Self {Self {
cond_time:Default::default(),
ramp_in_time:Default::default(),
offset:Default::default(),
pos_corr_gain:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsEGMRunSettings {
  pub const MAX_WIRE_BYTES:usize=40;
  pub const CARTESIAN_WIRE_BYTES:u64=40;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_09a843da7aa89c60e6404f8d36b9a1a57f75e330329ac043449fd438b7029506";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.cond_time.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ramp_in_time.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.offset.wire_size()?)?;
if !self.pos_corr_gain.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>40{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.cond_time.to_le_bytes());
put(out,at,&self.ramp_in_time.to_le_bytes());
self.offset.write_fields(out,at);
put(out,at,&self.pos_corr_gain.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cond_time=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ramp_in_time=decoded;
let decoded=Ros1Abb_rapid_msgsPose::read_fields(bytes,at)?;
value.offset=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pos_corr_gain=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsEGMRunSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>40{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/EGMSetupSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings {
  /// Declared use_filtering; canonical bounds are part of CODEC_IDENTITY.
  pub use_filtering:bool,
  /// Declared comm_timeout; canonical bounds are part of CODEC_IDENTITY.
  pub comm_timeout:f32,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings {fn default()->Self {Self {
use_filtering:Default::default(),
comm_timeout:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ece31a7631bb3bb248aece52d4d5f6e190a94dcc3052b2761b759bed24513779";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if !self.comm_timeout.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.use_filtering)]);
put(out,at,&self.comm_timeout.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_filtering=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.comm_timeout=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/EGMStopSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsEGMStopSettings {
  /// Declared ramp_out_time; canonical bounds are part of CODEC_IDENTITY.
  pub ramp_out_time:f32,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsEGMStopSettings {fn default()->Self {Self {
ramp_out_time:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsEGMStopSettings {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_64009cc452b9b2b12b3501d150e8c88c9cc07c796df06775195fa17c2a103a08";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.ramp_out_time.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.ramp_out_time.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ramp_out_time=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsEGMStopSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/EGMSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsEGMSettings {
  /// Declared allow_egm_motions; canonical bounds are part of CODEC_IDENTITY.
  pub allow_egm_motions:bool,
  /// Declared use_presync; canonical bounds are part of CODEC_IDENTITY.
  pub use_presync:bool,
  /// Declared setup_uc; canonical bounds are part of CODEC_IDENTITY.
  pub setup_uc:Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings,
  /// Declared activate; canonical bounds are part of CODEC_IDENTITY.
  pub activate:Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings,
  /// Declared run; canonical bounds are part of CODEC_IDENTITY.
  pub run:Ros1Abb_rapid_sm_addin_msgsEGMRunSettings,
  /// Declared stop; canonical bounds are part of CODEC_IDENTITY.
  pub stop:Ros1Abb_rapid_sm_addin_msgsEGMStopSettings,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsEGMSettings {fn default()->Self {Self {
allow_egm_motions:Default::default(),
use_presync:Default::default(),
setup_uc:Default::default(),
activate:Default::default(),
run:Default::default(),
stop:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsEGMSettings {
  pub const MAX_WIRE_BYTES:usize=767;
  pub const CARTESIAN_WIRE_BYTES:u64=767;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ebc0ca24a54451f863964c74bf0257b5b25546d0d4b8a75ac75c265e1d62317b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,self.setup_uc.wire_size()?)?;
n=add_size(n,self.activate.wire_size()?)?;
n=add_size(n,self.run.wire_size()?)?;
n=add_size(n,self.stop.wire_size()?)?;
if n>767{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.allow_egm_motions)]);
put(out,at,&[u8::from(self.use_presync)]);
self.setup_uc.write_fields(out,at);
self.activate.write_fields(out,at);
self.run.write_fields(out,at);
self.stop.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.allow_egm_motions=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_presync=decoded;
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings::read_fields(bytes,at)?;
value.setup_uc=decoded;
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings::read_fields(bytes,at)?;
value.activate=decoded;
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMRunSettings::read_fields(bytes,at)?;
value.run=decoded;
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMStopSettings::read_fields(bytes,at)?;
value.stop=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsEGMSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>767{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<255{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest {fn default()->Self {Self {
task:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_42cca857367e08720f989416d18d6a1f7d677f7e5bf49435b9735a9eb10a8f9c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.task.len()>64||self.task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.task.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.task.len() as u32).to_le_bytes());put(out,at,self.task.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.task=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse {
  /// Declared settings; canonical bounds are part of CODEC_IDENTITY.
  pub settings:Ros1Abb_rapid_sm_addin_msgsEGMSettings,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse {fn default()->Self {Self {
settings:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse {
  pub const MAX_WIRE_BYTES:usize=1285;
  pub const CARTESIAN_WIRE_BYTES:u64=1285;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_094fd8b0b992c26d708bbd09952d36a3921ad5e5e66f45edb9d19af5ec3472bc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.settings.wire_size()?)?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>1285{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.settings.write_fields(out,at);
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettings::read_fields(bytes,at)?;
value.settings=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1285{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<261{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/StateMachineState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsStateMachineState {
  /// Declared rapid_task; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_task:String,
  /// Declared sm_state; canonical bounds are part of CODEC_IDENTITY.
  pub sm_state:u8,
  /// Declared egm_action; canonical bounds are part of CODEC_IDENTITY.
  pub egm_action:u8,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsStateMachineState {fn default()->Self {Self {
rapid_task:Default::default(),
sm_state:Default::default(),
egm_action:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsStateMachineState {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9ef8cc3a0438ad892acc1a2f485039ca49abce6fedf40cc5d9b2fe03dd9490bb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.rapid_task.len()>512||self.rapid_task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.rapid_task.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.rapid_task.len() as u32).to_le_bytes());put(out,at,self.rapid_task.as_bytes());
put(out,at,&self.sm_state.to_le_bytes());
put(out,at,&self.egm_action.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.rapid_task=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.sm_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.egm_action=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsStateMachineState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/RuntimeState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsRuntimeState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared state_machines; canonical bounds are part of CODEC_IDENTITY.
  pub state_machines:Vec<Ros1Abb_rapid_sm_addin_msgsStateMachineState>,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsRuntimeState {fn default()->Self {Self {
header:Default::default(),
state_machines:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsRuntimeState {
  pub const MAX_WIRE_BYTES:usize=8372;
  pub const CARTESIAN_WIRE_BYTES:u64=8372;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_50d7eab2eed3ce0b55160e85b44b4108d966290f6b8f126980ee452846f3660c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.state_machines.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.state_machines {
n=add_size(n,element.wire_size()?)?;
}
if n>8372{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.state_machines.len() as u32).to_le_bytes());
for element in &self.state_machines {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.state_machines=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Abb_rapid_sm_addin_msgsStateMachineState::read_fields(bytes,at)?;
value.state_machines.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsRuntimeState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8372{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared settings; canonical bounds are part of CODEC_IDENTITY.
  pub settings:Ros1Abb_rapid_sm_addin_msgsEGMSettings,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest {fn default()->Self {Self {
task:Default::default(),
settings:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest {
  pub const MAX_WIRE_BYTES:usize=835;
  pub const CARTESIAN_WIRE_BYTES:u64=835;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fe85c34f8dff2c0a547f89385d75771af13a03d997b880ab7be2eb84ea800155";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.task.len()>64||self.task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.task.len())?;
n=add_size(n,self.settings.wire_size()?)?;
if n>835{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.task.len() as u32).to_le_bytes());put(out,at,self.task.as_bytes());
self.settings.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.task=decoded;
let decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettings::read_fields(bytes,at)?;
value.settings=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>835{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<259{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c0d4e594a51a6cdd245ad03b6e27bbdc298c5c9fbe26cc25fa3594deef9b2591";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared routine; canonical bounds are part of CODEC_IDENTITY.
  pub routine:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest {fn default()->Self {Self {
task:Default::default(),
routine:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest {
  pub const MAX_WIRE_BYTES:usize=584;
  pub const CARTESIAN_WIRE_BYTES:u64=584;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b16ba3e98ee1d45864532b01f914746f041e49463b9330ef6056e427d199a5a3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.task.len()>64||self.task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.task.len())?;
if self.routine.len()>512||self.routine.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.routine.len())?;
if n>584{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.task.len() as u32).to_le_bytes());put(out,at,self.task.as_bytes());
put(out,at,&(self.routine.len() as u32).to_le_bytes());put(out,at,self.routine.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.task=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.routine=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>584{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_44de2385f12a8612fa75ca7c3f97b07f22d04e920bdeca0f0bd7fed5322c680e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetSGCommandRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared command; canonical bounds are part of CODEC_IDENTITY.
  pub command:u8,
  /// Declared target_position; canonical bounds are part of CODEC_IDENTITY.
  pub target_position:f32,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {fn default()->Self {Self {
task:Default::default(),
command:Default::default(),
target_position:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {
  pub const MAX_WIRE_BYTES:usize=73;
  pub const CARTESIAN_WIRE_BYTES:u64=73;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e39d7fee66c1efb9ea963216823f6e10bd632d78634d8c47f3283a1a1779f0c2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.task.len()>64||self.task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.task.len())?;
n=add_size(n,1)?;
if !self.target_position.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>73{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.task.len() as u32).to_le_bytes());put(out,at,self.task.as_bytes());
put(out,at,&self.command.to_le_bytes());
put(out,at,&self.target_position.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.task=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.command=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.target_position=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>73{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_rapid_sm_addin_msgs/SetSGCommandResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2f22b2098d86dd0764277247eb7f64dccecebf0cddce85654b01c1928eb9c192";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetFileContentsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetFileContentsRequest {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
}
impl Default for Ros1Abb_robot_msgsGetFileContentsRequest {fn default()->Self {Self {
filename:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetFileContentsRequest {
  pub const MAX_WIRE_BYTES:usize=516;
  pub const CARTESIAN_WIRE_BYTES:u64=516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_75f5802b385cc1efb524fadea657fda1d5cc4298c5720222d33cd3dd39656f21";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.filename.len()>512||self.filename.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.filename.len())?;
if n>516{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.filename.len() as u32).to_le_bytes());put(out,at,self.filename.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.filename=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetFileContentsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetFileContentsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetFileContentsResponse {
  /// Declared contents; canonical bounds are part of CODEC_IDENTITY.
  pub contents:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetFileContentsResponse {fn default()->Self {Self {
contents:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetFileContentsResponse {
  pub const MAX_WIRE_BYTES:usize=4618;
  pub const CARTESIAN_WIRE_BYTES:u64=4618;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_693b53ed168f81a7c93ac8083977c4f8bd6f4a3999d079f5b47029699ba68fdb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.contents.len()>4096||self.contents.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.contents.len())?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>4618{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.contents.len() as u32).to_le_bytes());put(out,at,self.contents.as_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.contents=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetFileContentsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4618{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetIOSignalRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetIOSignalRequest {
  /// Declared signal; canonical bounds are part of CODEC_IDENTITY.
  pub signal:String,
}
impl Default for Ros1Abb_robot_msgsGetIOSignalRequest {fn default()->Self {Self {
signal:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetIOSignalRequest {
  pub const MAX_WIRE_BYTES:usize=516;
  pub const CARTESIAN_WIRE_BYTES:u64=516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d98f7577756b476cac0dd325d45f333fcd2b735df7dce58fb660790b5a91f83b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.signal.len()>512||self.signal.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.signal.len())?;
if n>516{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.signal.len() as u32).to_le_bytes());put(out,at,self.signal.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.signal=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetIOSignalRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetIOSignalResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetIOSignalResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetIOSignalResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetIOSignalResponse {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_10a66146c4815b0d758ec81dde1c552695c97a8727ee995299018b23f129d79e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>1034{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetIOSignalResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/RAPIDSymbolPath.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsRAPIDSymbolPath {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared module; canonical bounds are part of CODEC_IDENTITY.
  pub module:String,
  /// Declared symbol; canonical bounds are part of CODEC_IDENTITY.
  pub symbol:String,
}
impl Default for Ros1Abb_robot_msgsRAPIDSymbolPath {fn default()->Self {Self {
task:Default::default(),
module:Default::default(),
symbol:Default::default(),
}}}
impl Ros1Abb_robot_msgsRAPIDSymbolPath {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_07308d9ce1e71a02f5cc3546cdbaaa0b24a06534a55aa3ee3354b498038faad2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.task.len()>64||self.task.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.task.len())?;
if self.module.len()>64||self.module.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.module.len())?;
if self.symbol.len()>64||self.symbol.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.symbol.len())?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.task.len() as u32).to_le_bytes());put(out,at,self.task.as_bytes());
put(out,at,&(self.module.len() as u32).to_le_bytes());put(out,at,self.module.as_bytes());
put(out,at,&(self.symbol.len() as u32).to_le_bytes());put(out,at,self.symbol.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.task=decoded;
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.module=decoded;
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.symbol=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsRAPIDSymbolPath {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDBoolRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDBoolRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDBoolRequest {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDBoolRequest {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_46e49c0d6796175d0c56460dd4c7d2557deaa8db4f1a3bec40b53636d3b44b96";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDBoolRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDBoolResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDBoolResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDBoolResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDBoolResponse {
  pub const MAX_WIRE_BYTES:usize=519;
  pub const CARTESIAN_WIRE_BYTES:u64=519;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bfe21e95cb79f1d9bc1bc1dc2f61c744a2a7dc49e5a5ed07b6c8619344b97b3f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>519{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.value)]);
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDBoolResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>519{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDDnumRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDDnumRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDDnumRequest {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDDnumRequest {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9058d8774de0da657344a00d75a7f75878da2b0453bf25708bc8ed7f45c90c9d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDDnumRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDDnumResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDDnumResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f64,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDDnumResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDDnumResponse {
  pub const MAX_WIRE_BYTES:usize=526;
  pub const CARTESIAN_WIRE_BYTES:u64=526;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9c209b4b12d733e33b76cc33ae93b6d561ce1eb7f8864429a56bd073317a01b3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>526{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDDnumResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>526{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<14{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDNumRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDNumRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDNumRequest {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDNumRequest {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a316395c8a19b7c0a9c3a555e625fffc7d647c7b567f38c53b1c3735b50ee910";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDNumRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDNumResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDNumResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDNumResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDNumResponse {
  pub const MAX_WIRE_BYTES:usize=522;
  pub const CARTESIAN_WIRE_BYTES:u64=522;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1573cba9a9046a6b304a49e3a77266fa0d6f615d2ea9a24692c25616970fd9b0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>522{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDNumResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>522{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDStringRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDStringRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDStringRequest {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDStringRequest {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_56514142e9e064aa43d09078b49ed8ee0366189727c35a9bc130b35007da4c9c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDStringRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDStringResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDStringResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDStringResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDStringResponse {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d1188827af3c307a721f8128b6f14fa6020f3187afce8a4162ce6562cdd1c854";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>1034{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDStringResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDSymbolRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDSymbolRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDSymbolRequest {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDSymbolRequest {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_31302ad912a8ff39b56a429bc9dae22cbfc2c2101b8cd05162d00d9206aecf5b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if n>204{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDSymbolRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRAPIDSymbolResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRAPIDSymbolResponse {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRAPIDSymbolResponse {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRAPIDSymbolResponse {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_182dc465ea5fb9543791b6fb76520313cee1ed472ab0296c8ef7a9ed3d4d110f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>1034{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRAPIDSymbolResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRobotControllerDescriptionRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest {
}
impl Default for Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest {fn default()->Self {Self {
}}}
impl Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bc5f87f4d0f33cbc640c8188fbb15c9b0f43b3067f2406700d6f5bd2475089b6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetRobotControllerDescriptionResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse {
  /// Declared description; canonical bounds are part of CODEC_IDENTITY.
  pub description:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse {fn default()->Self {Self {
description:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse {
  pub const MAX_WIRE_BYTES:usize=4618;
  pub const CARTESIAN_WIRE_BYTES:u64=4618;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7db41283fbe12a010e526b87d77a5bb7faecdf85595ef2dc13f14bfed56b6e39";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.description.len()>4096||self.description.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.description.len())?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>4618{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.description.len() as u32).to_le_bytes());put(out,at,self.description.as_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.description=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4618{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetSpeedRatioRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetSpeedRatioRequest {
}
impl Default for Ros1Abb_robot_msgsGetSpeedRatioRequest {fn default()->Self {Self {
}}}
impl Ros1Abb_robot_msgsGetSpeedRatioRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_82230aebf6fa923e09bcd413ecb3b4803a6f1cfd3e8334e0ce0c496b73008373";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetSpeedRatioRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/GetSpeedRatioResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsGetSpeedRatioResponse {
  /// Declared speed_ratio; canonical bounds are part of CODEC_IDENTITY.
  pub speed_ratio:u8,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsGetSpeedRatioResponse {fn default()->Self {Self {
speed_ratio:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsGetSpeedRatioResponse {
  pub const MAX_WIRE_BYTES:usize=519;
  pub const CARTESIAN_WIRE_BYTES:u64=519;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7784360ddafa1b8ad9626fde0afa2b2335f6a294507036c3aee953df99e91940";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>519{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.speed_ratio.to_le_bytes());
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed_ratio=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsGetSpeedRatioResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>519{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/MechanicalUnitState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsMechanicalUnitState {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared activated; canonical bounds are part of CODEC_IDENTITY.
  pub activated:bool,
}
impl Default for Ros1Abb_robot_msgsMechanicalUnitState {fn default()->Self {Self {
name:Default::default(),
activated:Default::default(),
}}}
impl Ros1Abb_robot_msgsMechanicalUnitState {
  pub const MAX_WIRE_BYTES:usize=69;
  pub const CARTESIAN_WIRE_BYTES:u64=69;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_872cc6e8bdf32426bb9e24322c951452aab6ebcbc1f2998f4fb19c34abece8fa";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>64||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
n=add_size(n,1)?;
if n>69{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&[u8::from(self.activated)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.activated=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsMechanicalUnitState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>69{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/RAPIDTaskState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsRAPIDTaskState {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared activated; canonical bounds are part of CODEC_IDENTITY.
  pub activated:bool,
  /// Declared execution_state; canonical bounds are part of CODEC_IDENTITY.
  pub execution_state:u8,
  /// Declared motion_task; canonical bounds are part of CODEC_IDENTITY.
  pub motion_task:bool,
}
impl Default for Ros1Abb_robot_msgsRAPIDTaskState {fn default()->Self {Self {
name:Default::default(),
activated:Default::default(),
execution_state:Default::default(),
motion_task:Default::default(),
}}}
impl Ros1Abb_robot_msgsRAPIDTaskState {
  pub const MAX_WIRE_BYTES:usize=71;
  pub const CARTESIAN_WIRE_BYTES:u64=71;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ab1245cc352efa91cdcc8de801392857e25a4101a141439554b761525b31b947";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>64||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>71{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&[u8::from(self.activated)]);
put(out,at,&self.execution_state.to_le_bytes());
put(out,at,&[u8::from(self.motion_task)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.activated=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.execution_state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.motion_task=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsRAPIDTaskState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>71{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/ServiceResponses.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsServiceResponses {
}
impl Default for Ros1Abb_robot_msgsServiceResponses {fn default()->Self {Self {
}}}
impl Ros1Abb_robot_msgsServiceResponses {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f0a3c4b160bf0ca52dbbb4e26d2c8a7d3c6a40ac70787ba59cf181945e4276d4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsServiceResponses {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetFileContentsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetFileContentsRequest {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
  /// Declared contents; canonical bounds are part of CODEC_IDENTITY.
  pub contents:String,
}
impl Default for Ros1Abb_robot_msgsSetFileContentsRequest {fn default()->Self {Self {
filename:Default::default(),
contents:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetFileContentsRequest {
  pub const MAX_WIRE_BYTES:usize=4616;
  pub const CARTESIAN_WIRE_BYTES:u64=4616;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f764448d9754e9ebc5e5fab64f0c8ebb01c534ec7b95751ed7d58e8ce0bd3012";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.filename.len()>512||self.filename.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.filename.len())?;
if self.contents.len()>4096||self.contents.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.contents.len())?;
if n>4616{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.filename.len() as u32).to_le_bytes());put(out,at,self.filename.as_bytes());
put(out,at,&(self.contents.len() as u32).to_le_bytes());put(out,at,self.contents.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.filename=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.contents=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetFileContentsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4616{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetFileContentsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetFileContentsResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetFileContentsResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetFileContentsResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3de2a08b8e158e0612898880d6d9c98f1995828eb53bd5639b47f730c6cbefd0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetFileContentsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetIOSignalRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetIOSignalRequest {
  /// Declared signal; canonical bounds are part of CODEC_IDENTITY.
  pub signal:String,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros1Abb_robot_msgsSetIOSignalRequest {fn default()->Self {Self {
signal:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetIOSignalRequest {
  pub const MAX_WIRE_BYTES:usize=1032;
  pub const CARTESIAN_WIRE_BYTES:u64=1032;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_57a0197e2a5de7efd2fc91d945d699be24559baec6651179a1256f1d9d35bbc9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.signal.len()>512||self.signal.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.signal.len())?;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
if n>1032{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.signal.len() as u32).to_le_bytes());put(out,at,self.signal.as_bytes());
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.signal=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetIOSignalRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1032{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetIOSignalResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetIOSignalResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetIOSignalResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetIOSignalResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_becc0f844ce6a7fabd8a683175edd8033ba5c943c2de152991755ee112b460e5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetIOSignalResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDBoolRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDBoolRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDBoolRequest {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDBoolRequest {
  pub const MAX_WIRE_BYTES:usize=205;
  pub const CARTESIAN_WIRE_BYTES:u64=205;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d8e5420678fe5feeffa20ad9ab8431678ec346f764c94871c4496c62e9dd8e8d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
n=add_size(n,1)?;
if n>205{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
put(out,at,&[u8::from(self.value)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDBoolRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>205{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDBoolResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDBoolResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDBoolResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDBoolResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_da2ae1231cb0c1ec452fb2733d5c99dcae78ad0bfd10771241708931b271bf13";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDBoolResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDDnumRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDDnumRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f64,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDDnumRequest {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDDnumRequest {
  pub const MAX_WIRE_BYTES:usize=212;
  pub const CARTESIAN_WIRE_BYTES:u64=212;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0f17e3068f86bca6effb99f3bc850234b0163028f372340c450fce31657b9cf9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>212{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDDnumRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>212{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDDnumResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDDnumResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDDnumResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDDnumResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1905aed9683e03494baa54627ddbaf4aed0b92357823c0c042f9b7c6a970c8ed";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDDnumResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDNumRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDNumRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDNumRequest {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDNumRequest {
  pub const MAX_WIRE_BYTES:usize=208;
  pub const CARTESIAN_WIRE_BYTES:u64=208;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9982d02cb0b6ee5813f5fb38072d665226b7885aeb90517af881f4fc990f392f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>208{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDNumRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>208{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDNumResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDNumResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDNumResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDNumResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_af1181242b4afdb6d387d39c24d0bc343e872fb9ab21c0ec1e451785786a4e64";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDNumResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDStringRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDStringRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDStringRequest {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDStringRequest {
  pub const MAX_WIRE_BYTES:usize=720;
  pub const CARTESIAN_WIRE_BYTES:u64=720;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e06839324f8abfa8780b683386f87bab1ec04f1e6463e63c0599f5c2f8a75f0c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
if n>720{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDStringRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>720{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDStringResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDStringResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDStringResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDStringResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e175bdff5a150722b8c50cf2e1c0f28a99289a0b148339efdee4b034ea3b96b4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDStringResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDSymbolRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDSymbolRequest {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros1Abb_robot_msgsRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDSymbolRequest {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDSymbolRequest {
  pub const MAX_WIRE_BYTES:usize=720;
  pub const CARTESIAN_WIRE_BYTES:u64=720;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0654a5a8e5ec42f4feab54b086a008e02a74d44df65d2fcf60f007736e3fec7d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.path.wire_size()?)?;
if self.value.len()>512||self.value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.value.len())?;
if n>720{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.path.write_fields(out,at);
put(out,at,&(self.value.len() as u32).to_le_bytes());put(out,at,self.value.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Abb_robot_msgsRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDSymbolRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>720{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetRAPIDSymbolResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetRAPIDSymbolResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetRAPIDSymbolResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetRAPIDSymbolResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a4ca0a8821337d1f16ba1f2ad2b232aceaba419308ca03b27f43dac427a32846";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetRAPIDSymbolResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetSpeedRatioRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetSpeedRatioRequest {
  /// Declared speed_ratio; canonical bounds are part of CODEC_IDENTITY.
  pub speed_ratio:u8,
}
impl Default for Ros1Abb_robot_msgsSetSpeedRatioRequest {fn default()->Self {Self {
speed_ratio:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetSpeedRatioRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a6ac368d243d488901b03d5e4303600d5f0eaf62150801103e5f5f9c0863fab5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.speed_ratio.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed_ratio=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetSpeedRatioRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SetSpeedRatioResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSetSpeedRatioResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsSetSpeedRatioResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsSetSpeedRatioResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9775dc1029993d1dfdf5f3080e6ae4262177ba96cdc0fbe9851dc4fea16c70d8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSetSpeedRatioResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/SystemState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsSystemState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared motors_on; canonical bounds are part of CODEC_IDENTITY.
  pub motors_on:bool,
  /// Declared auto_mode; canonical bounds are part of CODEC_IDENTITY.
  pub auto_mode:bool,
  /// Declared rapid_running; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_running:bool,
  /// Declared rapid_tasks; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_tasks:Vec<Ros1Abb_robot_msgsRAPIDTaskState>,
  /// Declared mechanical_units; canonical bounds are part of CODEC_IDENTITY.
  pub mechanical_units:Vec<Ros1Abb_robot_msgsMechanicalUnitState>,
}
impl Default for Ros1Abb_robot_msgsSystemState {fn default()->Self {Self {
header:Default::default(),
motors_on:Default::default(),
auto_mode:Default::default(),
rapid_running:Default::default(),
rapid_tasks:Default::default(),
mechanical_units:Default::default(),
}}}
impl Ros1Abb_robot_msgsSystemState {
  pub const MAX_WIRE_BYTES:usize=2331;
  pub const CARTESIAN_WIRE_BYTES:u64=2331;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b57aa1fdf098aa78c51a96bd2978e02f97cbae67148c0028557d2a3e1e8630f2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if self.rapid_tasks.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.rapid_tasks {
n=add_size(n,element.wire_size()?)?;
}
if self.mechanical_units.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.mechanical_units {
n=add_size(n,element.wire_size()?)?;
}
if n>2331{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&[u8::from(self.motors_on)]);
put(out,at,&[u8::from(self.auto_mode)]);
put(out,at,&[u8::from(self.rapid_running)]);
put(out,at,&(self.rapid_tasks.len() as u32).to_le_bytes());
for element in &self.rapid_tasks {
element.write_fields(out,at);
}
put(out,at,&(self.mechanical_units.len() as u32).to_le_bytes());
for element in &self.mechanical_units {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.motors_on=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.auto_mode=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rapid_running=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.rapid_tasks=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Abb_robot_msgsRAPIDTaskState::read_fields(bytes,at)?;
value.rapid_tasks.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.mechanical_units=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Abb_robot_msgsMechanicalUnitState::read_fields(bytes,at)?;
value.mechanical_units.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsSystemState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2331{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<27{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/TriggerWithResultCodeRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsTriggerWithResultCodeRequest {
}
impl Default for Ros1Abb_robot_msgsTriggerWithResultCodeRequest {fn default()->Self {Self {
}}}
impl Ros1Abb_robot_msgsTriggerWithResultCodeRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_999f63504788e522f7d0108a895cf35cd101e9a50856e480ae9edabf791bcf94";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsTriggerWithResultCodeRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 abb_robot_msgs/TriggerWithResultCodeResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Abb_robot_msgsTriggerWithResultCodeResponse {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Abb_robot_msgsTriggerWithResultCodeResponse {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros1Abb_robot_msgsTriggerWithResultCodeResponse {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4ef5c987592e849766b82b7fd99a65b7a1e17605f3e3341c99803181887967ff";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros1Abb_robot_msgsTriggerWithResultCodeResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 duration.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Duration {
  /// Declared sec; canonical bounds are part of CODEC_IDENTITY.
  pub sec:i32,
  /// Declared nsec; canonical bounds are part of CODEC_IDENTITY.
  pub nsec:i32,
}
impl Default for Ros1Duration {fn default()->Self {Self {
sec:Default::default(),
nsec:Default::default(),
}}}
impl Ros1Duration {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.nsec<0||self.nsec>=1000000000{return Err(invalid());}
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.sec.to_le_bytes());
put(out,at,&self.nsec.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sec=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.nsec=decoded;
if value.nsec<0||value.nsec>=1000000000{return Err(invalid());}
Ok(value)}
}
impl Canonical for Ros1Duration {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 trajectory_msgs/JointTrajectoryPoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Trajectory_msgsJointTrajectoryPoint {
  /// Declared positions; canonical bounds are part of CODEC_IDENTITY.
  pub positions:Vec<f64>,
  /// Declared velocities; canonical bounds are part of CODEC_IDENTITY.
  pub velocities:Vec<f64>,
  /// Declared accelerations; canonical bounds are part of CODEC_IDENTITY.
  pub accelerations:Vec<f64>,
  /// Declared effort; canonical bounds are part of CODEC_IDENTITY.
  pub effort:Vec<f64>,
  /// Declared time_from_start; canonical bounds are part of CODEC_IDENTITY.
  pub time_from_start:Ros1Duration,
}
impl Default for Ros1Trajectory_msgsJointTrajectoryPoint {fn default()->Self {Self {
positions:Default::default(),
velocities:Default::default(),
accelerations:Default::default(),
effort:Default::default(),
time_from_start:Default::default(),
}}}
impl Ros1Trajectory_msgsJointTrajectoryPoint {
  pub const MAX_WIRE_BYTES:usize=408;
  pub const CARTESIAN_WIRE_BYTES:u64=408;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9773572fff8616235c4d7e23fccbe01f28dcfa62c7c80323a65c8c305feba3c5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.positions.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.positions {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocities.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocities {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.accelerations.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accelerations {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.time_from_start.wire_size()?)?;
if n>408{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.positions.len() as u32).to_le_bytes());
for element in &self.positions {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.velocities.len() as u32).to_le_bytes());
for element in &self.velocities {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.accelerations.len() as u32).to_le_bytes());
for element in &self.accelerations {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.effort.len() as u32).to_le_bytes());
for element in &self.effort {
put(out,at,&element.to_le_bytes());
}
self.time_from_start.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.positions=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.positions.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocities.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.accelerations=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.accelerations.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.effort=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.effort.push(decoded);
}}
let decoded=Ros1Duration::read_fields(bytes,at)?;
value.time_from_start=decoded;
Ok(value)}
}
impl Canonical for Ros1Trajectory_msgsJointTrajectoryPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>408{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 trajectory_msgs/JointTrajectory.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Trajectory_msgsJointTrajectory {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared points; canonical bounds are part of CODEC_IDENTITY.
  pub points:Vec<Ros1Trajectory_msgsJointTrajectoryPoint>,
}
impl Default for Ros1Trajectory_msgsJointTrajectory {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
points:Default::default(),
}}}
impl Ros1Trajectory_msgsJointTrajectory {
  pub const MAX_WIRE_BYTES:usize=4168;
  pub const CARTESIAN_WIRE_BYTES:u64=4168;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_079e6c432f0a858d6bd47c87aaf353a8b0676706809e0f984705b1a688de282b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.points.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.points {
n=add_size(n,element.wire_size()?)?;
}
if n>4168{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.joint_names.len() as u32).to_le_bytes());
for element in &self.joint_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.points.len() as u32).to_le_bytes());
for element in &self.points {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.points=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Trajectory_msgsJointTrajectoryPoint::read_fields(bytes,at)?;
value.points.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros1Trajectory_msgsJointTrajectory {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4168{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 control_msgs/JointTolerance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Control_msgsJointTolerance {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:f64,
  /// Declared acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub acceleration:f64,
}
impl Default for Ros1Control_msgsJointTolerance {fn default()->Self {Self {
name:Default::default(),
position:Default::default(),
velocity:Default::default(),
acceleration:Default::default(),
}}}
impl Ros1Control_msgsJointTolerance {
  pub const MAX_WIRE_BYTES:usize=92;
  pub const CARTESIAN_WIRE_BYTES:u64=92;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c9f71f98726d4a6621dcc8db96a394a24941a79d9041c8ad7a07724ee64d3c88";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>64||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.acceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>92{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&self.position.to_le_bytes());
put(out,at,&self.velocity.to_le_bytes());
put(out,at,&self.acceleration.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.acceleration=decoded;
Ok(value)}
}
impl Canonical for Ros1Control_msgsJointTolerance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>92{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 control_msgs/FollowJointTrajectoryGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Control_msgsFollowJointTrajectoryGoal {
  /// Declared trajectory; canonical bounds are part of CODEC_IDENTITY.
  pub trajectory:Ros1Trajectory_msgsJointTrajectory,
  /// Declared path_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub path_tolerance:Vec<Ros1Control_msgsJointTolerance>,
  /// Declared goal_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_tolerance:Vec<Ros1Control_msgsJointTolerance>,
  /// Declared goal_time_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_time_tolerance:Ros1Duration,
}
impl Default for Ros1Control_msgsFollowJointTrajectoryGoal {fn default()->Self {Self {
trajectory:Default::default(),
path_tolerance:Default::default(),
goal_tolerance:Default::default(),
goal_time_tolerance:Default::default(),
}}}
impl Ros1Control_msgsFollowJointTrajectoryGoal {
  pub const MAX_WIRE_BYTES:usize=6392;
  pub const CARTESIAN_WIRE_BYTES:u64=6392;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e9fda6f9e732450f2cf1af046ceb67815e6e1a22105eda2fee38f8f470252d40";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trajectory.wire_size()?)?;
if self.path_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.goal_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.goal_time_tolerance.wire_size()?)?;
if n>6392{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trajectory.write_fields(out,at);
put(out,at,&(self.path_tolerance.len() as u32).to_le_bytes());
for element in &self.path_tolerance {
element.write_fields(out,at);
}
put(out,at,&(self.goal_tolerance.len() as u32).to_le_bytes());
for element in &self.goal_tolerance {
element.write_fields(out,at);
}
self.goal_time_tolerance.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Trajectory_msgsJointTrajectory::read_fields(bytes,at)?;
value.trajectory=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Control_msgsJointTolerance::read_fields(bytes,at)?;
value.path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.goal_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Control_msgsJointTolerance::read_fields(bytes,at)?;
value.goal_tolerance.push(decoded);
}}
let decoded=Ros1Duration::read_fields(bytes,at)?;
value.goal_time_tolerance=decoded;
Ok(value)}
}
impl Canonical for Ros1Control_msgsFollowJointTrajectoryGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6392{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 control_msgs/FollowJointTrajectoryResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Control_msgsFollowJointTrajectoryResult {
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:i32,
  /// Declared error_string; canonical bounds are part of CODEC_IDENTITY.
  pub error_string:String,
}
impl Default for Ros1Control_msgsFollowJointTrajectoryResult {fn default()->Self {Self {
error_code:Default::default(),
error_string:Default::default(),
}}}
impl Ros1Control_msgsFollowJointTrajectoryResult {
  pub const MAX_WIRE_BYTES:usize=4104;
  pub const CARTESIAN_WIRE_BYTES:u64=4104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71da832be83c08804668aa63576768bbe831f21f5f270cdc8d851b30d2fc4281";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.error_string.len()>4096||self.error_string.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_string.len())?;
if n>4104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.error_code.to_le_bytes());
put(out,at,&(self.error_string.len() as u32).to_le_bytes());put(out,at,self.error_string.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_code=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_string=decoded;
Ok(value)}
}
impl Canonical for Ros1Control_msgsFollowJointTrajectoryResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 control_msgs/FollowJointTrajectoryFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Control_msgsFollowJointTrajectoryFeedback {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared desired; canonical bounds are part of CODEC_IDENTITY.
  pub desired:Ros1Trajectory_msgsJointTrajectoryPoint,
  /// Declared actual; canonical bounds are part of CODEC_IDENTITY.
  pub actual:Ros1Trajectory_msgsJointTrajectoryPoint,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:Ros1Trajectory_msgsJointTrajectoryPoint,
}
impl Default for Ros1Control_msgsFollowJointTrajectoryFeedback {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
desired:Default::default(),
actual:Default::default(),
error:Default::default(),
}}}
impl Ros1Control_msgsFollowJointTrajectoryFeedback {
  pub const MAX_WIRE_BYTES:usize=2124;
  pub const CARTESIAN_WIRE_BYTES:u64=2124;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a1ad5a1f8a00b60f97d06209737993bd395d83d1a008705d65193b63933b62a8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.desired.wire_size()?)?;
n=add_size(n,self.actual.wire_size()?)?;
n=add_size(n,self.error.wire_size()?)?;
if n>2124{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.joint_names.len() as u32).to_le_bytes());
for element in &self.joint_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
self.desired.write_fields(out,at);
self.actual.write_fields(out,at);
self.error.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
let decoded=Ros1Trajectory_msgsJointTrajectoryPoint::read_fields(bytes,at)?;
value.desired=decoded;
let decoded=Ros1Trajectory_msgsJointTrajectoryPoint::read_fields(bytes,at)?;
value.actual=decoded;
let decoded=Ros1Trajectory_msgsJointTrajectoryPoint::read_fields(bytes,at)?;
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros1Control_msgsFollowJointTrajectoryFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2124{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<92{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/JointState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsJointState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:Vec<String>,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Vec<f64>,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:Vec<f64>,
  /// Declared effort; canonical bounds are part of CODEC_IDENTITY.
  pub effort:Vec<f64>,
}
impl Default for Ros1Sensor_msgsJointState {fn default()->Self {Self {
header:Default::default(),
name:Default::default(),
position:Default::default(),
velocity:Default::default(),
effort:Default::default(),
}}}
impl Ros1Sensor_msgsJointState {
  pub const MAX_WIRE_BYTES:usize=1200;
  pub const CARTESIAN_WIRE_BYTES:u64=1200;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8d1bb9c4903dd4b070a5ab157b3590815722d6b574d610c9b9fb5ead8be7a057";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.name.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.name {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.position.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocity.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if (self.name.len()!=0&&self.position.len()!=0&&self.name.len()!=self.position.len())||(self.name.len()!=0&&self.velocity.len()!=0&&self.name.len()!=self.velocity.len())||(self.name.len()!=0&&self.effort.len()!=0&&self.name.len()!=self.effort.len())||(self.position.len()!=0&&self.velocity.len()!=0&&self.position.len()!=self.velocity.len())||(self.position.len()!=0&&self.effort.len()!=0&&self.position.len()!=self.effort.len())||(self.velocity.len()!=0&&self.effort.len()!=0&&self.velocity.len()!=self.effort.len()){return Err(invalid());}
if n>1200{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.name.len() as u32).to_le_bytes());
for element in &self.name {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.position.len() as u32).to_le_bytes());
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.velocity.len() as u32).to_le_bytes());
for element in &self.velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.effort.len() as u32).to_le_bytes());
for element in &self.effort {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.name=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.position=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.velocity=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.effort=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.effort.push(decoded);
}}
if (value.name.len()!=0&&value.position.len()!=0&&value.name.len()!=value.position.len())||(value.name.len()!=0&&value.velocity.len()!=0&&value.name.len()!=value.velocity.len())||(value.name.len()!=0&&value.effort.len()!=0&&value.name.len()!=value.effort.len())||(value.position.len()!=0&&value.velocity.len()!=0&&value.position.len()!=value.velocity.len())||(value.position.len()!=0&&value.effort.len()!=0&&value.position.len()!=value.effort.len())||(value.velocity.len()!=0&&value.effort.len()!=0&&value.velocity.len()!=value.effort.len()){return Err(invalid());}
Ok(value)}
}
impl Canonical for Ros1Sensor_msgsJointState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1200{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros1Abb_egm_msgsEGMChannelState {pub const EGM_UNDEFINED:u8=1;
pub const EGM_ERROR:u8=2;
pub const EGM_STOPPED:u8=3;
pub const EGM_RUNNING:u8=4;
pub const MOTORS_UNDEFINED:u8=1;
pub const MOTORS_ON:u8=2;
pub const MOTORS_OFF:u8=3;
pub const RAPID_UNDEFINED:u8=1;
pub const RAPID_STOPPED:u8=2;
pub const RAPID_RUNNING:u8=3;}

impl Ros1Abb_rapid_sm_addin_msgsStateMachineState {pub const SM_STATE_UNKNOWN:u8=1;
pub const SM_STATE_IDLE:u8=2;
pub const SM_STATE_INITIALIZE:u8=3;
pub const SM_STATE_RUN_RAPID_ROUTINE:u8=4;
pub const SM_STATE_RUN_EGM_ROUTINE:u8=5;
pub const EGM_ACTION_UNKNOWN:u8=1;
pub const EGM_ACTION_NONE:u8=2;
pub const EGM_ACTION_RUN_JOINT:u8=3;
pub const EGM_ACTION_RUN_POSE:u8=4;
pub const EGM_ACTION_STOP:u8=5;
pub const EGM_ACTION_START_STREAM:u8=6;
pub const EGM_ACTION_STOP_STREAM:u8=7;}

impl Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {pub const SG_COMMAND_UNKNOWN:u8=1;
pub const SG_COMMAND_NONE:u8=2;
pub const SG_COMMAND_INITIALIZE:u8=3;
pub const SG_COMMAND_CALIBRATE:u8=4;
pub const SG_COMMAND_MOVE_TO:u8=5;
pub const SG_COMMAND_GRIP_IN:u8=6;
pub const SG_COMMAND_GRIP_OUT:u8=7;
pub const SG_COMMAND_BLOW_ON_1:u8=8;
pub const SG_COMMAND_BLOW_ON_2:u8=9;
pub const SG_COMMAND_BLOW_OFF_1:u8=10;
pub const SG_COMMAND_BLOW_OFF_2:u8=11;
pub const SG_COMMAND_VACUUM_ON_1:u8=12;
pub const SG_COMMAND_VACUUM_ON_2:u8=13;
pub const SG_COMMAND_VACUUM_OFF_1:u8=14;
pub const SG_COMMAND_VACUUM_OFF_2:u8=15;}

impl Ros1Abb_robot_msgsRAPIDTaskState {pub const EXECUTION_STATE_UNKNOWN:u8=1;
pub const EXECUTION_STATE_READY:u8=2;
pub const EXECUTION_STATE_STOPPED:u8=3;
pub const EXECUTION_STATE_STARTED:u8=4;
pub const EXECUTION_STATE_UNINITIALIZED:u8=5;}

impl Ros1Abb_robot_msgsServiceResponses {pub const SUCCESS:&'static str="";
pub const RC_SUCCESS:u16=1;
pub const FAILED:&'static str="Service failed (e.g. timeout or resource not found, see the DEBUG log for details)";
pub const RC_FAILED:u16=2;
pub const SERVER_IS_BUSY:&'static str="Server is busy";
pub const RC_SERVER_IS_BUSY:u16=1001;
pub const EMPTY_FILENAME:&'static str="Filename is empty";
pub const RC_EMPTY_FILENAME:u16=2001;
pub const EMPTY_SIGNAL_NAME:&'static str="Signal name is empty";
pub const RC_EMPTY_SIGNAL_NAME:u16=2002;
pub const EMPTY_RAPID_TASK_NAME:&'static str="RAPID task name is empty";
pub const RC_EMPTY_RAPID_TASK_NAME:u16=2003;
pub const EMPTY_RAPID_MODULE_NAME:&'static str="RAPID module name is empty";
pub const RC_EMPTY_RAPID_MODULE_NAME:u16=2004;
pub const EMPTY_RAPID_SYMBOL_NAME:&'static str="RAPID symbol name is empty";
pub const RC_EMPTY_RAPID_SYMBOL_NAME:u16=2005;
pub const NOT_IN_AUTO_MODE:&'static str="Robot controller is not in AUTO mode";
pub const RC_NOT_IN_AUTO_MODE:u16=3001;
pub const MOTORS_ARE_OFF:&'static str="Motors are off";
pub const RC_MOTORS_ARE_OFF:u16=3002;
pub const MOTORS_ARE_ON:&'static str="Motors are on";
pub const RC_MOTORS_ARE_ON:u16=3003;
pub const RAPID_NOT_STOPPED:&'static str="RAPID has not been stopped";
pub const RC_RAPID_NOT_STOPPED:u16=3004;
pub const RAPID_NOT_RUNNING:&'static str="RAPID is not running";
pub const RC_RAPID_NOT_RUNNING:u16=3005;
pub const SM_RUNTIME_STATES_MISSING:&'static str="No runtime states received for any StateMachine Add-In instance";
pub const RC_SM_RUNTIME_STATES_MISSING:u16=4001;
pub const SM_UNKNOWN_RAPID_TASK:&'static str="RAPID task not found among known StateMachine Add-In instances";
pub const RC_SM_UNKNOWN_RAPID_TASK:u16=4002;
pub const SM_UNINITIALIZED:&'static str="StateMachine Add-In instance has not been initialized";
pub const RC_SM_UNINITIALIZED:u16=4003;}

impl Ros1Control_msgsFollowJointTrajectoryResult {pub const SUCCESSFUL:i32=0;
pub const INVALID_GOAL:i32=-1;
pub const INVALID_JOINTS:i32=-2;
pub const OLD_HEADER_TIMESTAMP:i32=-3;
pub const PATH_TOLERANCE_VIOLATED:i32=-4;
pub const GOAL_TOLERANCE_VIOLATED:i32=-5;}
