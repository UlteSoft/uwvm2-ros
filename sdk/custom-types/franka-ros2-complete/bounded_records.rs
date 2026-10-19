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
/// Complete bounded value for ROS2 builtin_interfaces/msg/Time.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Builtin_interfacesMsgTime {
  /// Declared sec; canonical bounds are part of CODEC_IDENTITY.
  pub sec:i32,
  /// Declared nanosec; canonical bounds are part of CODEC_IDENTITY.
  pub nanosec:u32,
}
impl Default for Ros2Builtin_interfacesMsgTime {fn default()->Self {Self {
sec:Default::default(),
nanosec:Default::default(),
}}}
impl Ros2Builtin_interfacesMsgTime {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.nanosec>=1000000000{return Err(invalid());}
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.sec.to_le_bytes());
put(out,at,&self.nanosec.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sec=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.nanosec=decoded;
if value.nanosec>=1000000000{return Err(invalid());}
Ok(value)}
}
impl Canonical for Ros2Builtin_interfacesMsgTime {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/ErrorRecovery_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionErrorRecovery_Goal {
}
impl Default for Ros2Franka_msgsActionErrorRecovery_Goal {fn default()->Self {Self {
}}}
impl Ros2Franka_msgsActionErrorRecovery_Goal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2caee3e8d5c04f12239a71f6e9a9259b818b9f1ad75103442bfc9d2343a3a070";
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
impl Canonical for Ros2Franka_msgsActionErrorRecovery_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/ErrorRecovery_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionErrorRecovery_Result {
}
impl Default for Ros2Franka_msgsActionErrorRecovery_Result {fn default()->Self {Self {
}}}
impl Ros2Franka_msgsActionErrorRecovery_Result {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9112b16c42034737b96214f23249d499ab6a9f0aee529cfec34c67cdc76809e9";
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
impl Canonical for Ros2Franka_msgsActionErrorRecovery_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/ErrorRecovery_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionErrorRecovery_Feedback {
}
impl Default for Ros2Franka_msgsActionErrorRecovery_Feedback {fn default()->Self {Self {
}}}
impl Ros2Franka_msgsActionErrorRecovery_Feedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cc817bcbe5790860e83c71e50a8c3610cf3965e34036b3099712cdcdb426127f";
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
impl Canonical for Ros2Franka_msgsActionErrorRecovery_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/GraspEpsilon.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgGraspEpsilon {
  /// Declared inner; canonical bounds are part of CODEC_IDENTITY.
  pub inner:f64,
  /// Declared outer; canonical bounds are part of CODEC_IDENTITY.
  pub outer:f64,
}
impl Default for Ros2Franka_msgsMsgGraspEpsilon {fn default()->Self {Self {
inner:f64::from_bits(0x3f747ae147ae147b),
outer:f64::from_bits(0x3f747ae147ae147b),
}}}
impl Ros2Franka_msgsMsgGraspEpsilon {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_21787ae5929dc83b9f644f4914e22da9ddd3e4f7ad3d8c6cda6bbcdfeba20c38";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.inner.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.outer.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.inner.to_le_bytes());
put(out,at,&self.outer.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.inner=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.outer=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgGraspEpsilon {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Grasp_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionGrasp_Goal {
  /// Declared width; canonical bounds are part of CODEC_IDENTITY.
  pub width:f64,
  /// Declared epsilon; canonical bounds are part of CODEC_IDENTITY.
  pub epsilon:Ros2Franka_msgsMsgGraspEpsilon,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f64,
  /// Declared force; canonical bounds are part of CODEC_IDENTITY.
  pub force:f64,
}
impl Default for Ros2Franka_msgsActionGrasp_Goal {fn default()->Self {Self {
width:Default::default(),
epsilon:Default::default(),
speed:Default::default(),
force:Default::default(),
}}}
impl Ros2Franka_msgsActionGrasp_Goal {
  pub const MAX_WIRE_BYTES:usize=40;
  pub const CARTESIAN_WIRE_BYTES:u64=40;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b7ef701e9fd93b5da9894499234d3647ecc4cea1936f637105c172f4579b6e1f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.width.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,self.epsilon.wire_size()?)?;
if !self.speed.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.force.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>40{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.width.to_le_bytes());
self.epsilon.write_fields(out,at);
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.force.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.width=decoded;
let decoded=Ros2Franka_msgsMsgGraspEpsilon::read_fields(bytes,at)?;
value.epsilon=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.force=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionGrasp_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>40{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Grasp_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionGrasp_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsActionGrasp_Result {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsActionGrasp_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9a4c3150c0c2d5eb8ff3dad403473a9546ee37cc3086a55a628a1a85b905b733";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionGrasp_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Grasp_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionGrasp_Feedback {
  /// Declared current_width; canonical bounds are part of CODEC_IDENTITY.
  pub current_width:f64,
}
impl Default for Ros2Franka_msgsActionGrasp_Feedback {fn default()->Self {Self {
current_width:Default::default(),
}}}
impl Ros2Franka_msgsActionGrasp_Feedback {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2e7697286c002e2d891e9c3167aa0dd1664acfc66c649fff6e8f9101c42703fe";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current_width.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current_width.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current_width=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionGrasp_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Homing_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionHoming_Goal {
}
impl Default for Ros2Franka_msgsActionHoming_Goal {fn default()->Self {Self {
}}}
impl Ros2Franka_msgsActionHoming_Goal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8008426ce3fa8c8d62133704f8738201b8154b324ba07e7b6f605cd3132a885d";
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
impl Canonical for Ros2Franka_msgsActionHoming_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Homing_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionHoming_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsActionHoming_Result {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsActionHoming_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_74a9677213ffa9ba3a26b3053956350b8f75d5d1769152e5e3d4d26370bcd77c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionHoming_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Homing_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionHoming_Feedback {
  /// Declared current_width; canonical bounds are part of CODEC_IDENTITY.
  pub current_width:f64,
}
impl Default for Ros2Franka_msgsActionHoming_Feedback {fn default()->Self {Self {
current_width:Default::default(),
}}}
impl Ros2Franka_msgsActionHoming_Feedback {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2260e092d369a0d2e4bf97ca2dfa78c02b6836b8167e71d63e76b5c0c40a6b46";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current_width.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current_width.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current_width=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionHoming_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Move_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionMove_Goal {
  /// Declared width; canonical bounds are part of CODEC_IDENTITY.
  pub width:f64,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f64,
}
impl Default for Ros2Franka_msgsActionMove_Goal {fn default()->Self {Self {
width:Default::default(),
speed:Default::default(),
}}}
impl Ros2Franka_msgsActionMove_Goal {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9f9e308ebd034344ee73b22a07d005944975c037943279504b2414bdd9ad206f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.width.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.speed.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.width.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.width=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionMove_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Move_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionMove_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsActionMove_Result {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsActionMove_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_66aa505c035357274c68be773e1b502c7d77f195f3dca848e818811ce95dd266";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionMove_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/Move_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionMove_Feedback {
  /// Declared current_width; canonical bounds are part of CODEC_IDENTITY.
  pub current_width:f64,
}
impl Default for Ros2Franka_msgsActionMove_Feedback {fn default()->Self {Self {
current_width:Default::default(),
}}}
impl Ros2Franka_msgsActionMove_Feedback {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_18d0e658f109492380c135e787adbdcc026b051e9a547fcea70453bbd9750596";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current_width.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current_width.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current_width=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionMove_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/PTPMotion_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionPTPMotion_Goal {
  /// Declared goal_joint_configuration; canonical bounds are part of CODEC_IDENTITY.
  pub goal_joint_configuration:Vec<f64>,
  /// Declared maximum_joint_velocities; canonical bounds are part of CODEC_IDENTITY.
  pub maximum_joint_velocities:Vec<f64>,
  /// Declared goal_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_tolerance:f64,
}
impl Default for Ros2Franka_msgsActionPTPMotion_Goal {fn default()->Self {Self {
goal_joint_configuration:Default::default(),
maximum_joint_velocities:Default::default(),
goal_tolerance:Default::default(),
}}}
impl Ros2Franka_msgsActionPTPMotion_Goal {
  pub const MAX_WIRE_BYTES:usize=128;
  pub const CARTESIAN_WIRE_BYTES:u64=128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7e1b02257a7be37e7bccd0870ebced1e1560e7250eaeaa4547836e721f2ea2cc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.goal_joint_configuration.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.goal_joint_configuration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.maximum_joint_velocities.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.maximum_joint_velocities {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.goal_tolerance.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.goal_joint_configuration.len() as u32).to_le_bytes());
for element in &self.goal_joint_configuration {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.maximum_joint_velocities.len() as u32).to_le_bytes());
for element in &self.maximum_joint_velocities {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.goal_tolerance.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.goal_joint_configuration=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.goal_joint_configuration.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.maximum_joint_velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.maximum_joint_velocities.push(decoded);
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.goal_tolerance=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionPTPMotion_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/TargetStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgTargetStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:u8,
}
impl Default for Ros2Franka_msgsMsgTargetStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Franka_msgsMsgTargetStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_df49eedc13cd8be2055361d8640c519bfa018ed2a421aa2b59b35c8828eac585";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.status.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgTargetStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/PTPMotion_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionPTPMotion_Result {
  /// Declared target_status; canonical bounds are part of CODEC_IDENTITY.
  pub target_status:Ros2Franka_msgsMsgTargetStatus,
  /// Declared error_message; canonical bounds are part of CODEC_IDENTITY.
  pub error_message:String,
}
impl Default for Ros2Franka_msgsActionPTPMotion_Result {fn default()->Self {Self {
target_status:Default::default(),
error_message:Default::default(),
}}}
impl Ros2Franka_msgsActionPTPMotion_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b61fabf13a3d8d910a35a5145e87b4dd80180b0678ef5580784f461e850fa4c1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.target_status.wire_size()?)?;
if self.error_message.len()>256||self.error_message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_message.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.target_status.write_fields(out,at);
put(out,at,&(self.error_message.len() as u32).to_le_bytes());put(out,at,self.error_message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Franka_msgsMsgTargetStatus::read_fields(bytes,at)?;
value.target_status=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionPTPMotion_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/action/PTPMotion_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsActionPTPMotion_Feedback {
  /// Declared target_status; canonical bounds are part of CODEC_IDENTITY.
  pub target_status:Ros2Franka_msgsMsgTargetStatus,
}
impl Default for Ros2Franka_msgsActionPTPMotion_Feedback {fn default()->Self {Self {
target_status:Default::default(),
}}}
impl Ros2Franka_msgsActionPTPMotion_Feedback {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c4dcfeb668595ebc341d76b5cbc606c1ae4c38b28a1b329f8436e61f506a0300";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.target_status.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.target_status.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Franka_msgsMsgTargetStatus::read_fields(bytes,at)?;
value.target_status=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsActionPTPMotion_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Vector3.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgVector3 {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
}
impl Default for Ros2Geometry_msgsMsgVector3 {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros2Geometry_msgsMsgVector3 {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgVector3 {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/CollisionIndicators.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgCollisionIndicators {
  /// Declared is_cartesian_linear_collision; canonical bounds are part of CODEC_IDENTITY.
  pub is_cartesian_linear_collision:Ros2Geometry_msgsMsgVector3,
  /// Declared is_cartesian_angular_collision; canonical bounds are part of CODEC_IDENTITY.
  pub is_cartesian_angular_collision:Ros2Geometry_msgsMsgVector3,
  /// Declared is_cartesian_linear_contact; canonical bounds are part of CODEC_IDENTITY.
  pub is_cartesian_linear_contact:Ros2Geometry_msgsMsgVector3,
  /// Declared is_cartesian_angular_contact; canonical bounds are part of CODEC_IDENTITY.
  pub is_cartesian_angular_contact:Ros2Geometry_msgsMsgVector3,
  /// Declared is_joint_collision; canonical bounds are part of CODEC_IDENTITY.
  pub is_joint_collision:[f64;7],
  /// Declared is_joint_contact; canonical bounds are part of CODEC_IDENTITY.
  pub is_joint_contact:[f64;7],
}
impl Default for Ros2Franka_msgsMsgCollisionIndicators {fn default()->Self {Self {
is_cartesian_linear_collision:Default::default(),
is_cartesian_angular_collision:Default::default(),
is_cartesian_linear_contact:Default::default(),
is_cartesian_angular_contact:Default::default(),
is_joint_collision:core::array::from_fn(|_|Default::default()),
is_joint_contact:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsMsgCollisionIndicators {
  pub const MAX_WIRE_BYTES:usize=208;
  pub const CARTESIAN_WIRE_BYTES:u64=208;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2bdfb3ece01b47b07fae84a998b23c56fc9482a0518969e43304b318d4ee0ce1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.is_cartesian_linear_collision.wire_size()?)?;
n=add_size(n,self.is_cartesian_angular_collision.wire_size()?)?;
n=add_size(n,self.is_cartesian_linear_contact.wire_size()?)?;
n=add_size(n,self.is_cartesian_angular_contact.wire_size()?)?;
for element in &self.is_joint_collision {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.is_joint_contact {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>208{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.is_cartesian_linear_collision.write_fields(out,at);
self.is_cartesian_angular_collision.write_fields(out,at);
self.is_cartesian_linear_contact.write_fields(out,at);
self.is_cartesian_angular_contact.write_fields(out,at);
for element in &self.is_joint_collision {
put(out,at,&element.to_le_bytes());
}
for element in &self.is_joint_contact {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.is_cartesian_linear_collision=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.is_cartesian_angular_collision=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.is_cartesian_linear_contact=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.is_cartesian_angular_contact=decoded;
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.is_joint_collision[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.is_joint_contact[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgCollisionIndicators {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>208{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<208{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/Elbow.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgElbow {
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:[f64;2],
  /// Declared desired_position; canonical bounds are part of CODEC_IDENTITY.
  pub desired_position:[f64;2],
  /// Declared commanded_position; canonical bounds are part of CODEC_IDENTITY.
  pub commanded_position:[f64;2],
  /// Declared commanded_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub commanded_velocity:[f64;2],
  /// Declared commanded_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub commanded_acceleration:[f64;2],
}
impl Default for Ros2Franka_msgsMsgElbow {fn default()->Self {Self {
position:core::array::from_fn(|_|Default::default()),
desired_position:core::array::from_fn(|_|Default::default()),
commanded_position:core::array::from_fn(|_|Default::default()),
commanded_velocity:core::array::from_fn(|_|Default::default()),
commanded_acceleration:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsMsgElbow {
  pub const MAX_WIRE_BYTES:usize=80;
  pub const CARTESIAN_WIRE_BYTES:u64=80;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_686cf2da29007046f9855ffeecac304c159245ec3da153990ab83be4ffce53b6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.desired_position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.commanded_position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.commanded_velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.commanded_acceleration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>80{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
for element in &self.desired_position {
put(out,at,&element.to_le_bytes());
}
for element in &self.commanded_position {
put(out,at,&element.to_le_bytes());
}
for element in &self.commanded_velocity {
put(out,at,&element.to_le_bytes());
}
for element in &self.commanded_acceleration {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.desired_position[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.commanded_position[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.commanded_velocity[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.commanded_acceleration[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgElbow {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>80{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<80{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/Errors.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgErrors {
  /// Declared joint_position_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub joint_position_limits_violation:bool,
  /// Declared cartesian_position_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_position_limits_violation:bool,
  /// Declared self_collision_avoidance_violation; canonical bounds are part of CODEC_IDENTITY.
  pub self_collision_avoidance_violation:bool,
  /// Declared joint_velocity_violation; canonical bounds are part of CODEC_IDENTITY.
  pub joint_velocity_violation:bool,
  /// Declared cartesian_velocity_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_velocity_violation:bool,
  /// Declared force_control_safety_violation; canonical bounds are part of CODEC_IDENTITY.
  pub force_control_safety_violation:bool,
  /// Declared joint_reflex; canonical bounds are part of CODEC_IDENTITY.
  pub joint_reflex:bool,
  /// Declared cartesian_reflex; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_reflex:bool,
  /// Declared max_goal_pose_deviation_violation; canonical bounds are part of CODEC_IDENTITY.
  pub max_goal_pose_deviation_violation:bool,
  /// Declared max_path_pose_deviation_violation; canonical bounds are part of CODEC_IDENTITY.
  pub max_path_pose_deviation_violation:bool,
  /// Declared cartesian_velocity_profile_safety_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_velocity_profile_safety_violation:bool,
  /// Declared joint_position_motion_generator_start_pose_invalid; canonical bounds are part of CODEC_IDENTITY.
  pub joint_position_motion_generator_start_pose_invalid:bool,
  /// Declared joint_motion_generator_position_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub joint_motion_generator_position_limits_violation:bool,
  /// Declared joint_motion_generator_velocity_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub joint_motion_generator_velocity_limits_violation:bool,
  /// Declared joint_motion_generator_velocity_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub joint_motion_generator_velocity_discontinuity:bool,
  /// Declared joint_motion_generator_acceleration_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub joint_motion_generator_acceleration_discontinuity:bool,
  /// Declared cartesian_position_motion_generator_start_pose_invalid; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_position_motion_generator_start_pose_invalid:bool,
  /// Declared cartesian_motion_generator_elbow_limit_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_elbow_limit_violation:bool,
  /// Declared cartesian_motion_generator_velocity_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_velocity_limits_violation:bool,
  /// Declared cartesian_motion_generator_velocity_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_velocity_discontinuity:bool,
  /// Declared cartesian_motion_generator_acceleration_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_acceleration_discontinuity:bool,
  /// Declared cartesian_motion_generator_elbow_sign_inconsistent; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_elbow_sign_inconsistent:bool,
  /// Declared cartesian_motion_generator_start_elbow_invalid; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_start_elbow_invalid:bool,
  /// Declared cartesian_motion_generator_joint_position_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_joint_position_limits_violation:bool,
  /// Declared cartesian_motion_generator_joint_velocity_limits_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_joint_velocity_limits_violation:bool,
  /// Declared cartesian_motion_generator_joint_velocity_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_joint_velocity_discontinuity:bool,
  /// Declared cartesian_motion_generator_joint_acceleration_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_motion_generator_joint_acceleration_discontinuity:bool,
  /// Declared cartesian_position_motion_generator_invalid_frame; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_position_motion_generator_invalid_frame:bool,
  /// Declared force_controller_desired_force_tolerance_violation; canonical bounds are part of CODEC_IDENTITY.
  pub force_controller_desired_force_tolerance_violation:bool,
  /// Declared controller_torque_discontinuity; canonical bounds are part of CODEC_IDENTITY.
  pub controller_torque_discontinuity:bool,
  /// Declared start_elbow_sign_inconsistent; canonical bounds are part of CODEC_IDENTITY.
  pub start_elbow_sign_inconsistent:bool,
  /// Declared communication_constraints_violation; canonical bounds are part of CODEC_IDENTITY.
  pub communication_constraints_violation:bool,
  /// Declared power_limit_violation; canonical bounds are part of CODEC_IDENTITY.
  pub power_limit_violation:bool,
  /// Declared joint_p2p_insufficient_torque_for_planning; canonical bounds are part of CODEC_IDENTITY.
  pub joint_p2p_insufficient_torque_for_planning:bool,
  /// Declared tau_j_range_violation; canonical bounds are part of CODEC_IDENTITY.
  pub tau_j_range_violation:bool,
  /// Declared instability_detected; canonical bounds are part of CODEC_IDENTITY.
  pub instability_detected:bool,
}
impl Default for Ros2Franka_msgsMsgErrors {fn default()->Self {Self {
joint_position_limits_violation:Default::default(),
cartesian_position_limits_violation:Default::default(),
self_collision_avoidance_violation:Default::default(),
joint_velocity_violation:Default::default(),
cartesian_velocity_violation:Default::default(),
force_control_safety_violation:Default::default(),
joint_reflex:Default::default(),
cartesian_reflex:Default::default(),
max_goal_pose_deviation_violation:Default::default(),
max_path_pose_deviation_violation:Default::default(),
cartesian_velocity_profile_safety_violation:Default::default(),
joint_position_motion_generator_start_pose_invalid:Default::default(),
joint_motion_generator_position_limits_violation:Default::default(),
joint_motion_generator_velocity_limits_violation:Default::default(),
joint_motion_generator_velocity_discontinuity:Default::default(),
joint_motion_generator_acceleration_discontinuity:Default::default(),
cartesian_position_motion_generator_start_pose_invalid:Default::default(),
cartesian_motion_generator_elbow_limit_violation:Default::default(),
cartesian_motion_generator_velocity_limits_violation:Default::default(),
cartesian_motion_generator_velocity_discontinuity:Default::default(),
cartesian_motion_generator_acceleration_discontinuity:Default::default(),
cartesian_motion_generator_elbow_sign_inconsistent:Default::default(),
cartesian_motion_generator_start_elbow_invalid:Default::default(),
cartesian_motion_generator_joint_position_limits_violation:Default::default(),
cartesian_motion_generator_joint_velocity_limits_violation:Default::default(),
cartesian_motion_generator_joint_velocity_discontinuity:Default::default(),
cartesian_motion_generator_joint_acceleration_discontinuity:Default::default(),
cartesian_position_motion_generator_invalid_frame:Default::default(),
force_controller_desired_force_tolerance_violation:Default::default(),
controller_torque_discontinuity:Default::default(),
start_elbow_sign_inconsistent:Default::default(),
communication_constraints_violation:Default::default(),
power_limit_violation:Default::default(),
joint_p2p_insufficient_torque_for_planning:Default::default(),
tau_j_range_violation:Default::default(),
instability_detected:Default::default(),
}}}
impl Ros2Franka_msgsMsgErrors {
  pub const MAX_WIRE_BYTES:usize=36;
  pub const CARTESIAN_WIRE_BYTES:u64=36;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c7927af1abe7710bb686574ad6f2459f8425af1d6cdb3189160bb732b5a6bc8c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>36{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.joint_position_limits_violation)]);
put(out,at,&[u8::from(self.cartesian_position_limits_violation)]);
put(out,at,&[u8::from(self.self_collision_avoidance_violation)]);
put(out,at,&[u8::from(self.joint_velocity_violation)]);
put(out,at,&[u8::from(self.cartesian_velocity_violation)]);
put(out,at,&[u8::from(self.force_control_safety_violation)]);
put(out,at,&[u8::from(self.joint_reflex)]);
put(out,at,&[u8::from(self.cartesian_reflex)]);
put(out,at,&[u8::from(self.max_goal_pose_deviation_violation)]);
put(out,at,&[u8::from(self.max_path_pose_deviation_violation)]);
put(out,at,&[u8::from(self.cartesian_velocity_profile_safety_violation)]);
put(out,at,&[u8::from(self.joint_position_motion_generator_start_pose_invalid)]);
put(out,at,&[u8::from(self.joint_motion_generator_position_limits_violation)]);
put(out,at,&[u8::from(self.joint_motion_generator_velocity_limits_violation)]);
put(out,at,&[u8::from(self.joint_motion_generator_velocity_discontinuity)]);
put(out,at,&[u8::from(self.joint_motion_generator_acceleration_discontinuity)]);
put(out,at,&[u8::from(self.cartesian_position_motion_generator_start_pose_invalid)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_elbow_limit_violation)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_velocity_limits_violation)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_velocity_discontinuity)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_acceleration_discontinuity)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_elbow_sign_inconsistent)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_start_elbow_invalid)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_joint_position_limits_violation)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_joint_velocity_limits_violation)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_joint_velocity_discontinuity)]);
put(out,at,&[u8::from(self.cartesian_motion_generator_joint_acceleration_discontinuity)]);
put(out,at,&[u8::from(self.cartesian_position_motion_generator_invalid_frame)]);
put(out,at,&[u8::from(self.force_controller_desired_force_tolerance_violation)]);
put(out,at,&[u8::from(self.controller_torque_discontinuity)]);
put(out,at,&[u8::from(self.start_elbow_sign_inconsistent)]);
put(out,at,&[u8::from(self.communication_constraints_violation)]);
put(out,at,&[u8::from(self.power_limit_violation)]);
put(out,at,&[u8::from(self.joint_p2p_insufficient_torque_for_planning)]);
put(out,at,&[u8::from(self.tau_j_range_violation)]);
put(out,at,&[u8::from(self.instability_detected)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_position_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_position_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.self_collision_avoidance_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_velocity_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_velocity_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.force_control_safety_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_reflex=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_reflex=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.max_goal_pose_deviation_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.max_path_pose_deviation_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_velocity_profile_safety_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_position_motion_generator_start_pose_invalid=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_motion_generator_position_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_motion_generator_velocity_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_motion_generator_velocity_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_motion_generator_acceleration_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_position_motion_generator_start_pose_invalid=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_elbow_limit_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_velocity_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_velocity_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_acceleration_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_elbow_sign_inconsistent=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_start_elbow_invalid=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_joint_position_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_joint_velocity_limits_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_joint_velocity_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_motion_generator_joint_acceleration_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_position_motion_generator_invalid_frame=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.force_controller_desired_force_tolerance_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.controller_torque_discontinuity=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.start_elbow_sign_inconsistent=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.communication_constraints_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.power_limit_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_p2p_insufficient_torque_for_planning=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.tau_j_range_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.instability_detected=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgErrors {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>36{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 std_msgs/msg/Header.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Std_msgsMsgHeader {
  /// Declared stamp; canonical bounds are part of CODEC_IDENTITY.
  pub stamp:Ros2Builtin_interfacesMsgTime,
  /// Declared frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub frame_id:String,
}
impl Default for Ros2Std_msgsMsgHeader {fn default()->Self {Self {
stamp:Default::default(),
frame_id:Default::default(),
}}}
impl Ros2Std_msgsMsgHeader {
  pub const MAX_WIRE_BYTES:usize=140;
  pub const CARTESIAN_WIRE_BYTES:u64=140;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.stamp.wire_size()?)?;
if self.frame_id.len()>128||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if n>140{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.stamp.write_fields(out,at);
put(out,at,&(self.frame_id.len() as u32).to_le_bytes());put(out,at,self.frame_id.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Builtin_interfacesMsgTime::read_fields(bytes,at)?;
value.stamp=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_msgsMsgHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>140{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 sensor_msgs/msg/JointState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Sensor_msgsMsgJointState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:Vec<String>,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Vec<f64>,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:Vec<f64>,
  /// Declared effort; canonical bounds are part of CODEC_IDENTITY.
  pub effort:Vec<f64>,
}
impl Default for Ros2Sensor_msgsMsgJointState {fn default()->Self {Self {
header:Default::default(),
name:Default::default(),
position:Default::default(),
velocity:Default::default(),
effort:Default::default(),
}}}
impl Ros2Sensor_msgsMsgJointState {
  pub const MAX_WIRE_BYTES:usize=1248;
  pub const CARTESIAN_WIRE_BYTES:u64=1248;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_912ba4261e2dfbec0e4a543ea8e5d308ef1d26877b4a7d71162a981ac64c40a3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.name.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.name {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.position.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocity.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>7{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if (self.name.len()!=0&&self.position.len()!=0&&self.name.len()!=self.position.len())||(self.name.len()!=0&&self.velocity.len()!=0&&self.name.len()!=self.velocity.len())||(self.name.len()!=0&&self.effort.len()!=0&&self.name.len()!=self.effort.len())||(self.position.len()!=0&&self.velocity.len()!=0&&self.position.len()!=self.velocity.len())||(self.position.len()!=0&&self.effort.len()!=0&&self.position.len()!=self.effort.len())||(self.velocity.len()!=0&&self.effort.len()!=0&&self.velocity.len()!=self.effort.len()){return Err(invalid());}
if n>1248{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.name=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.position=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.velocity=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>7{return Err(invalid());}
value.effort=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.effort.push(decoded);
}}
if (value.name.len()!=0&&value.position.len()!=0&&value.name.len()!=value.position.len())||(value.name.len()!=0&&value.velocity.len()!=0&&value.name.len()!=value.velocity.len())||(value.name.len()!=0&&value.effort.len()!=0&&value.name.len()!=value.effort.len())||(value.position.len()!=0&&value.velocity.len()!=0&&value.position.len()!=value.velocity.len())||(value.position.len()!=0&&value.effort.len()!=0&&value.position.len()!=value.effort.len())||(value.velocity.len()!=0&&value.effort.len()!=0&&value.velocity.len()!=value.effort.len()){return Err(invalid());}
Ok(value)}
}
impl Canonical for Ros2Sensor_msgsMsgJointState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1248{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Wrench.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgWrench {
  /// Declared force; canonical bounds are part of CODEC_IDENTITY.
  pub force:Ros2Geometry_msgsMsgVector3,
  /// Declared torque; canonical bounds are part of CODEC_IDENTITY.
  pub torque:Ros2Geometry_msgsMsgVector3,
}
impl Default for Ros2Geometry_msgsMsgWrench {fn default()->Self {Self {
force:Default::default(),
torque:Default::default(),
}}}
impl Ros2Geometry_msgsMsgWrench {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fdfdf2474c0129b0ce1f85972cc87fd80478ac7d2429d9de1ec58951a82a11b6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.force.wire_size()?)?;
n=add_size(n,self.torque.wire_size()?)?;
if n>48{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.force.write_fields(out,at);
self.torque.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.force=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.torque=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgWrench {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/WrenchStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgWrenchStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared wrench; canonical bounds are part of CODEC_IDENTITY.
  pub wrench:Ros2Geometry_msgsMsgWrench,
}
impl Default for Ros2Geometry_msgsMsgWrenchStamped {fn default()->Self {Self {
header:Default::default(),
wrench:Default::default(),
}}}
impl Ros2Geometry_msgsMsgWrenchStamped {
  pub const MAX_WIRE_BYTES:usize=188;
  pub const CARTESIAN_WIRE_BYTES:u64=188;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e23600874a626ba9d52f043e14ed04beb3e316d47c3bca8bd87cfeed3f5d9d66";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.wrench.wire_size()?)?;
if n>188{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.wrench.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgWrench::read_fields(bytes,at)?;
value.wrench=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgWrenchStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>188{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<60{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Inertia.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgInertia {
  /// Declared m; canonical bounds are part of CODEC_IDENTITY.
  pub m:f64,
  /// Declared com; canonical bounds are part of CODEC_IDENTITY.
  pub com:Ros2Geometry_msgsMsgVector3,
  /// Declared ixx; canonical bounds are part of CODEC_IDENTITY.
  pub ixx:f64,
  /// Declared ixy; canonical bounds are part of CODEC_IDENTITY.
  pub ixy:f64,
  /// Declared ixz; canonical bounds are part of CODEC_IDENTITY.
  pub ixz:f64,
  /// Declared iyy; canonical bounds are part of CODEC_IDENTITY.
  pub iyy:f64,
  /// Declared iyz; canonical bounds are part of CODEC_IDENTITY.
  pub iyz:f64,
  /// Declared izz; canonical bounds are part of CODEC_IDENTITY.
  pub izz:f64,
}
impl Default for Ros2Geometry_msgsMsgInertia {fn default()->Self {Self {
m:Default::default(),
com:Default::default(),
ixx:Default::default(),
ixy:Default::default(),
ixz:Default::default(),
iyy:Default::default(),
iyz:Default::default(),
izz:Default::default(),
}}}
impl Ros2Geometry_msgsMsgInertia {
  pub const MAX_WIRE_BYTES:usize=80;
  pub const CARTESIAN_WIRE_BYTES:u64=80;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4a1b7a6a2516724a9a12e5eb166a85cd73d33fc6075c8d517ff2a279fd29e12a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.m.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,self.com.wire_size()?)?;
if !self.ixx.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.ixy.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.ixz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.iyy.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.iyz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.izz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>80{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.m.to_le_bytes());
self.com.write_fields(out,at);
put(out,at,&self.ixx.to_le_bytes());
put(out,at,&self.ixy.to_le_bytes());
put(out,at,&self.ixz.to_le_bytes());
put(out,at,&self.iyy.to_le_bytes());
put(out,at,&self.iyz.to_le_bytes());
put(out,at,&self.izz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.m=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.com=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixx=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixy=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixz=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iyy=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iyz=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.izz=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgInertia {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>80{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<80{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/InertiaStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgInertiaStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared inertia; canonical bounds are part of CODEC_IDENTITY.
  pub inertia:Ros2Geometry_msgsMsgInertia,
}
impl Default for Ros2Geometry_msgsMsgInertiaStamped {fn default()->Self {Self {
header:Default::default(),
inertia:Default::default(),
}}}
impl Ros2Geometry_msgsMsgInertiaStamped {
  pub const MAX_WIRE_BYTES:usize=220;
  pub const CARTESIAN_WIRE_BYTES:u64=220;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_19b0b442c5858ff1eee20e4658ecc3a2b76fdd64c4a21a1642fae5ab38198576";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.inertia.wire_size()?)?;
if n>220{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.inertia.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgInertia::read_fields(bytes,at)?;
value.inertia=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgInertiaStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>220{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<92{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Point.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgPoint {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
}
impl Default for Ros2Geometry_msgsMsgPoint {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros2Geometry_msgsMsgPoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_80f43d054abe333673c940c6d0fd88e8e26cea4a1cff53ff789cccaa38240616";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Quaternion.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgQuaternion {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f64,
}
impl Default for Ros2Geometry_msgsMsgQuaternion {fn default()->Self {Self {
x:f64::from_bits(0x0000000000000000),
y:f64::from_bits(0x0000000000000000),
z:f64::from_bits(0x0000000000000000),
w:f64::from_bits(0x3ff0000000000000),
}}}
impl Ros2Geometry_msgsMsgQuaternion {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgQuaternion {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Pose.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgPose {
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Ros2Geometry_msgsMsgPoint,
  /// Declared orientation; canonical bounds are part of CODEC_IDENTITY.
  pub orientation:Ros2Geometry_msgsMsgQuaternion,
}
impl Default for Ros2Geometry_msgsMsgPose {fn default()->Self {Self {
position:Default::default(),
orientation:Default::default(),
}}}
impl Ros2Geometry_msgsMsgPose {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9468a10b8b2728fe71a370b4348ffe5e81b884ff957533f8700aa16e70b12777";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.position.wire_size()?)?;
n=add_size(n,self.orientation.wire_size()?)?;
if n>56{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.position.write_fields(out,at);
self.orientation.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgPoint::read_fields(bytes,at)?;
value.position=decoded;
let decoded=Ros2Geometry_msgsMsgQuaternion::read_fields(bytes,at)?;
value.orientation=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgPose {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/PoseStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgPoseStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros2Geometry_msgsMsgPose,
}
impl Default for Ros2Geometry_msgsMsgPoseStamped {fn default()->Self {Self {
header:Default::default(),
pose:Default::default(),
}}}
impl Ros2Geometry_msgsMsgPoseStamped {
  pub const MAX_WIRE_BYTES:usize=196;
  pub const CARTESIAN_WIRE_BYTES:u64=196;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71abdc844f18f02f45a354967a5f1c644fb20dc14cbc61c64aa41b927e0712a2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.pose.wire_size()?)?;
if n>196{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.pose.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgPose::read_fields(bytes,at)?;
value.pose=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgPoseStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>196{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<68{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Twist.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgTwist {
  /// Declared linear; canonical bounds are part of CODEC_IDENTITY.
  pub linear:Ros2Geometry_msgsMsgVector3,
  /// Declared angular; canonical bounds are part of CODEC_IDENTITY.
  pub angular:Ros2Geometry_msgsMsgVector3,
}
impl Default for Ros2Geometry_msgsMsgTwist {fn default()->Self {Self {
linear:Default::default(),
angular:Default::default(),
}}}
impl Ros2Geometry_msgsMsgTwist {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.linear.wire_size()?)?;
n=add_size(n,self.angular.wire_size()?)?;
if n>48{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.linear.write_fields(out,at);
self.angular.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.linear=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.angular=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTwist {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/TwistStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgTwistStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros2Geometry_msgsMsgTwist,
}
impl Default for Ros2Geometry_msgsMsgTwistStamped {fn default()->Self {Self {
header:Default::default(),
twist:Default::default(),
}}}
impl Ros2Geometry_msgsMsgTwistStamped {
  pub const MAX_WIRE_BYTES:usize=188;
  pub const CARTESIAN_WIRE_BYTES:u64=188;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7448b885388ff3454de1a35b8c5c895ee58b791267b435df1e4cf7c1eee0f573";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>188{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.twist.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.twist=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTwistStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>188{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<60{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Accel.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgAccel {
  /// Declared linear; canonical bounds are part of CODEC_IDENTITY.
  pub linear:Ros2Geometry_msgsMsgVector3,
  /// Declared angular; canonical bounds are part of CODEC_IDENTITY.
  pub angular:Ros2Geometry_msgsMsgVector3,
}
impl Default for Ros2Geometry_msgsMsgAccel {fn default()->Self {Self {
linear:Default::default(),
angular:Default::default(),
}}}
impl Ros2Geometry_msgsMsgAccel {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_87264cb87970315248345c43cc629bbc0c9f9b2a8c6c4bc089da81c7af552fde";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.linear.wire_size()?)?;
n=add_size(n,self.angular.wire_size()?)?;
if n>48{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.linear.write_fields(out,at);
self.angular.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.linear=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.angular=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgAccel {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/AccelStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgAccelStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared accel; canonical bounds are part of CODEC_IDENTITY.
  pub accel:Ros2Geometry_msgsMsgAccel,
}
impl Default for Ros2Geometry_msgsMsgAccelStamped {fn default()->Self {Self {
header:Default::default(),
accel:Default::default(),
}}}
impl Ros2Geometry_msgsMsgAccelStamped {
  pub const MAX_WIRE_BYTES:usize=188;
  pub const CARTESIAN_WIRE_BYTES:u64=188;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c04d3ae310bd82f7c4c9f30c2ff185ab54989aa3adcc759c7d3eb19853819899";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.accel.wire_size()?)?;
if n>188{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.accel.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgAccel::read_fields(bytes,at)?;
value.accel=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgAccelStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>188{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<60{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/Vector3Stamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgVector3Stamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared vector; canonical bounds are part of CODEC_IDENTITY.
  pub vector:Ros2Geometry_msgsMsgVector3,
}
impl Default for Ros2Geometry_msgsMsgVector3Stamped {fn default()->Self {Self {
header:Default::default(),
vector:Default::default(),
}}}
impl Ros2Geometry_msgsMsgVector3Stamped {
  pub const MAX_WIRE_BYTES:usize=164;
  pub const CARTESIAN_WIRE_BYTES:u64=164;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c6b67646d3fc6d29c195637ea353dafa7085b2bdb55a15a8b65009200b5ed31b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.vector.wire_size()?)?;
if n>164{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.vector.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.vector=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgVector3Stamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>164{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/msg/FrankaRobotState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsMsgFrankaRobotState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared collision_indicators; canonical bounds are part of CODEC_IDENTITY.
  pub collision_indicators:Ros2Franka_msgsMsgCollisionIndicators,
  /// Declared measured_joint_state; canonical bounds are part of CODEC_IDENTITY.
  pub measured_joint_state:Ros2Sensor_msgsMsgJointState,
  /// Declared desired_joint_state; canonical bounds are part of CODEC_IDENTITY.
  pub desired_joint_state:Ros2Sensor_msgsMsgJointState,
  /// Declared measured_joint_motor_state; canonical bounds are part of CODEC_IDENTITY.
  pub measured_joint_motor_state:Ros2Sensor_msgsMsgJointState,
  /// Declared ddq_d; canonical bounds are part of CODEC_IDENTITY.
  pub ddq_d:[f64;7],
  /// Declared dtau_j; canonical bounds are part of CODEC_IDENTITY.
  pub dtau_j:[f64;7],
  /// Declared tau_ext_hat_filtered; canonical bounds are part of CODEC_IDENTITY.
  pub tau_ext_hat_filtered:Ros2Sensor_msgsMsgJointState,
  /// Declared elbow; canonical bounds are part of CODEC_IDENTITY.
  pub elbow:Ros2Franka_msgsMsgElbow,
  /// Declared k_f_ext_hat_k; canonical bounds are part of CODEC_IDENTITY.
  pub k_f_ext_hat_k:Ros2Geometry_msgsMsgWrenchStamped,
  /// Declared o_f_ext_hat_k; canonical bounds are part of CODEC_IDENTITY.
  pub o_f_ext_hat_k:Ros2Geometry_msgsMsgWrenchStamped,
  /// Declared inertia_ee; canonical bounds are part of CODEC_IDENTITY.
  pub inertia_ee:Ros2Geometry_msgsMsgInertiaStamped,
  /// Declared inertia_load; canonical bounds are part of CODEC_IDENTITY.
  pub inertia_load:Ros2Geometry_msgsMsgInertiaStamped,
  /// Declared inertia_total; canonical bounds are part of CODEC_IDENTITY.
  pub inertia_total:Ros2Geometry_msgsMsgInertiaStamped,
  /// Declared o_t_ee; canonical bounds are part of CODEC_IDENTITY.
  pub o_t_ee:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared o_t_ee_d; canonical bounds are part of CODEC_IDENTITY.
  pub o_t_ee_d:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared o_t_ee_c; canonical bounds are part of CODEC_IDENTITY.
  pub o_t_ee_c:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared f_t_ee; canonical bounds are part of CODEC_IDENTITY.
  pub f_t_ee:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared ee_t_k; canonical bounds are part of CODEC_IDENTITY.
  pub ee_t_k:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared o_dp_ee_d; canonical bounds are part of CODEC_IDENTITY.
  pub o_dp_ee_d:Ros2Geometry_msgsMsgTwistStamped,
  /// Declared o_dp_ee_c; canonical bounds are part of CODEC_IDENTITY.
  pub o_dp_ee_c:Ros2Geometry_msgsMsgTwistStamped,
  /// Declared o_ddp_ee_c; canonical bounds are part of CODEC_IDENTITY.
  pub o_ddp_ee_c:Ros2Geometry_msgsMsgAccelStamped,
  /// Declared time; canonical bounds are part of CODEC_IDENTITY.
  pub time:f64,
  /// Declared control_command_success_rate; canonical bounds are part of CODEC_IDENTITY.
  pub control_command_success_rate:f64,
  /// Declared robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub robot_mode:u8,
  /// Declared current_errors; canonical bounds are part of CODEC_IDENTITY.
  pub current_errors:Ros2Franka_msgsMsgErrors,
  /// Declared last_motion_errors; canonical bounds are part of CODEC_IDENTITY.
  pub last_motion_errors:Ros2Franka_msgsMsgErrors,
  /// Declared accelerometer_top; canonical bounds are part of CODEC_IDENTITY.
  pub accelerometer_top:[Ros2Geometry_msgsMsgVector3Stamped;6],
  /// Declared accelerometer_bottom; canonical bounds are part of CODEC_IDENTITY.
  pub accelerometer_bottom:[Ros2Geometry_msgsMsgVector3Stamped;6],
}
impl Default for Ros2Franka_msgsMsgFrankaRobotState {fn default()->Self {Self {
header:Default::default(),
collision_indicators:Default::default(),
measured_joint_state:Default::default(),
desired_joint_state:Default::default(),
measured_joint_motor_state:Default::default(),
ddq_d:core::array::from_fn(|_|Default::default()),
dtau_j:core::array::from_fn(|_|Default::default()),
tau_ext_hat_filtered:Default::default(),
elbow:Default::default(),
k_f_ext_hat_k:Default::default(),
o_f_ext_hat_k:Default::default(),
inertia_ee:Default::default(),
inertia_load:Default::default(),
inertia_total:Default::default(),
o_t_ee:Default::default(),
o_t_ee_d:Default::default(),
o_t_ee_c:Default::default(),
f_t_ee:Default::default(),
ee_t_k:Default::default(),
o_dp_ee_d:Default::default(),
o_dp_ee_c:Default::default(),
o_ddp_ee_c:Default::default(),
time:Default::default(),
control_command_success_rate:Default::default(),
robot_mode:Default::default(),
current_errors:Default::default(),
last_motion_errors:Default::default(),
accelerometer_top:core::array::from_fn(|_|Default::default()),
accelerometer_bottom:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsMsgFrankaRobotState {
  pub const MAX_WIRE_BYTES:usize=10169;
  pub const CARTESIAN_WIRE_BYTES:u64=10169;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0fdc5c99a098f4c891956bb6cd81505995d5dda41a740f88ee456f48f6e07282";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.collision_indicators.wire_size()?)?;
n=add_size(n,self.measured_joint_state.wire_size()?)?;
n=add_size(n,self.desired_joint_state.wire_size()?)?;
n=add_size(n,self.measured_joint_motor_state.wire_size()?)?;
for element in &self.ddq_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.dtau_j {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.tau_ext_hat_filtered.wire_size()?)?;
n=add_size(n,self.elbow.wire_size()?)?;
n=add_size(n,self.k_f_ext_hat_k.wire_size()?)?;
n=add_size(n,self.o_f_ext_hat_k.wire_size()?)?;
n=add_size(n,self.inertia_ee.wire_size()?)?;
n=add_size(n,self.inertia_load.wire_size()?)?;
n=add_size(n,self.inertia_total.wire_size()?)?;
n=add_size(n,self.o_t_ee.wire_size()?)?;
n=add_size(n,self.o_t_ee_d.wire_size()?)?;
n=add_size(n,self.o_t_ee_c.wire_size()?)?;
n=add_size(n,self.f_t_ee.wire_size()?)?;
n=add_size(n,self.ee_t_k.wire_size()?)?;
n=add_size(n,self.o_dp_ee_d.wire_size()?)?;
n=add_size(n,self.o_dp_ee_c.wire_size()?)?;
n=add_size(n,self.o_ddp_ee_c.wire_size()?)?;
if !self.time.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.control_command_success_rate.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,self.current_errors.wire_size()?)?;
n=add_size(n,self.last_motion_errors.wire_size()?)?;
for element in &self.accelerometer_top {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.accelerometer_bottom {
n=add_size(n,element.wire_size()?)?;
}
if n>10169{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.collision_indicators.write_fields(out,at);
self.measured_joint_state.write_fields(out,at);
self.desired_joint_state.write_fields(out,at);
self.measured_joint_motor_state.write_fields(out,at);
for element in &self.ddq_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.dtau_j {
put(out,at,&element.to_le_bytes());
}
self.tau_ext_hat_filtered.write_fields(out,at);
self.elbow.write_fields(out,at);
self.k_f_ext_hat_k.write_fields(out,at);
self.o_f_ext_hat_k.write_fields(out,at);
self.inertia_ee.write_fields(out,at);
self.inertia_load.write_fields(out,at);
self.inertia_total.write_fields(out,at);
self.o_t_ee.write_fields(out,at);
self.o_t_ee_d.write_fields(out,at);
self.o_t_ee_c.write_fields(out,at);
self.f_t_ee.write_fields(out,at);
self.ee_t_k.write_fields(out,at);
self.o_dp_ee_d.write_fields(out,at);
self.o_dp_ee_c.write_fields(out,at);
self.o_ddp_ee_c.write_fields(out,at);
put(out,at,&self.time.to_le_bytes());
put(out,at,&self.control_command_success_rate.to_le_bytes());
put(out,at,&self.robot_mode.to_le_bytes());
self.current_errors.write_fields(out,at);
self.last_motion_errors.write_fields(out,at);
for element in &self.accelerometer_top {
element.write_fields(out,at);
}
for element in &self.accelerometer_bottom {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Franka_msgsMsgCollisionIndicators::read_fields(bytes,at)?;
value.collision_indicators=decoded;
let decoded=Ros2Sensor_msgsMsgJointState::read_fields(bytes,at)?;
value.measured_joint_state=decoded;
let decoded=Ros2Sensor_msgsMsgJointState::read_fields(bytes,at)?;
value.desired_joint_state=decoded;
let decoded=Ros2Sensor_msgsMsgJointState::read_fields(bytes,at)?;
value.measured_joint_motor_state=decoded;
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ddq_d[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dtau_j[i]=decoded;
}}
let decoded=Ros2Sensor_msgsMsgJointState::read_fields(bytes,at)?;
value.tau_ext_hat_filtered=decoded;
let decoded=Ros2Franka_msgsMsgElbow::read_fields(bytes,at)?;
value.elbow=decoded;
let decoded=Ros2Geometry_msgsMsgWrenchStamped::read_fields(bytes,at)?;
value.k_f_ext_hat_k=decoded;
let decoded=Ros2Geometry_msgsMsgWrenchStamped::read_fields(bytes,at)?;
value.o_f_ext_hat_k=decoded;
let decoded=Ros2Geometry_msgsMsgInertiaStamped::read_fields(bytes,at)?;
value.inertia_ee=decoded;
let decoded=Ros2Geometry_msgsMsgInertiaStamped::read_fields(bytes,at)?;
value.inertia_load=decoded;
let decoded=Ros2Geometry_msgsMsgInertiaStamped::read_fields(bytes,at)?;
value.inertia_total=decoded;
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.o_t_ee=decoded;
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.o_t_ee_d=decoded;
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.o_t_ee_c=decoded;
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.f_t_ee=decoded;
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.ee_t_k=decoded;
let decoded=Ros2Geometry_msgsMsgTwistStamped::read_fields(bytes,at)?;
value.o_dp_ee_d=decoded;
let decoded=Ros2Geometry_msgsMsgTwistStamped::read_fields(bytes,at)?;
value.o_dp_ee_c=decoded;
let decoded=Ros2Geometry_msgsMsgAccelStamped::read_fields(bytes,at)?;
value.o_ddp_ee_c=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.time=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.control_command_success_rate=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.robot_mode=decoded;
let decoded=Ros2Franka_msgsMsgErrors::read_fields(bytes,at)?;
value.current_errors=decoded;
let decoded=Ros2Franka_msgsMsgErrors::read_fields(bytes,at)?;
value.last_motion_errors=decoded;
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgVector3Stamped::read_fields(bytes,at)?;
value.accelerometer_top[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgVector3Stamped::read_fields(bytes,at)?;
value.accelerometer_bottom[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsMsgFrankaRobotState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>10169{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1961{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetCartesianStiffness_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetCartesianStiffness_Request {
  /// Declared cartesian_stiffness; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_stiffness:[f64;6],
}
impl Default for Ros2Franka_msgsSrvSetCartesianStiffness_Request {fn default()->Self {Self {
cartesian_stiffness:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetCartesianStiffness_Request {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ae78b9887a5419225cfe0727bac83d8c53f789acd53bd47f024485b02aa98260";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.cartesian_stiffness {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>48{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.cartesian_stiffness {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cartesian_stiffness[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetCartesianStiffness_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetCartesianStiffness_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetCartesianStiffness_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetCartesianStiffness_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetCartesianStiffness_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3194f7469bbf228e653f5a93336fd5de4a1b2d7c1fe9542f3fadc8ad4fd51afa";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetCartesianStiffness_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request {
  /// Declared lower_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_torque_thresholds_nominal:[f64;7],
  /// Declared upper_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_torque_thresholds_nominal:[f64;7],
  /// Declared lower_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_force_thresholds_nominal:[f64;6],
  /// Declared upper_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_force_thresholds_nominal:[f64;6],
}
impl Default for Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request {fn default()->Self {Self {
lower_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request {
  pub const MAX_WIRE_BYTES:usize=208;
  pub const CARTESIAN_WIRE_BYTES:u64=208;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_829642a9ea418a53f7252f7489bd7f5ed976bf3689443012daf216db895cf99c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.lower_torque_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_torque_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.lower_force_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_force_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>208{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.lower_torque_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_torque_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.lower_force_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_force_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_torque_thresholds_nominal[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_torque_thresholds_nominal[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_force_thresholds_nominal[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_force_thresholds_nominal[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>208{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<208{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7d5640c34446e4262a8c00f0b242b68c0095688f0208fd692f21d855003eb47a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetFullCollisionBehavior_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetFullCollisionBehavior_Request {
  /// Declared lower_torque_thresholds_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub lower_torque_thresholds_acceleration:[f64;7],
  /// Declared upper_torque_thresholds_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub upper_torque_thresholds_acceleration:[f64;7],
  /// Declared lower_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_torque_thresholds_nominal:[f64;7],
  /// Declared upper_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_torque_thresholds_nominal:[f64;7],
  /// Declared lower_force_thresholds_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub lower_force_thresholds_acceleration:[f64;6],
  /// Declared upper_force_thresholds_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub upper_force_thresholds_acceleration:[f64;6],
  /// Declared lower_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_force_thresholds_nominal:[f64;6],
  /// Declared upper_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_force_thresholds_nominal:[f64;6],
}
impl Default for Ros2Franka_msgsSrvSetFullCollisionBehavior_Request {fn default()->Self {Self {
lower_torque_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
lower_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetFullCollisionBehavior_Request {
  pub const MAX_WIRE_BYTES:usize=416;
  pub const CARTESIAN_WIRE_BYTES:u64=416;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d6898555d931aee3a5b4f5f97830cb9982f2e378c59c40bcaa0201840d2f2824";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.lower_torque_thresholds_acceleration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_torque_thresholds_acceleration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.lower_torque_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_torque_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.lower_force_thresholds_acceleration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_force_thresholds_acceleration {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.lower_force_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.upper_force_thresholds_nominal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>416{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.lower_torque_thresholds_acceleration {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_torque_thresholds_acceleration {
put(out,at,&element.to_le_bytes());
}
for element in &self.lower_torque_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_torque_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.lower_force_thresholds_acceleration {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_force_thresholds_acceleration {
put(out,at,&element.to_le_bytes());
}
for element in &self.lower_force_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
for element in &self.upper_force_thresholds_nominal {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_torque_thresholds_acceleration[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_torque_thresholds_acceleration[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_torque_thresholds_nominal[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_torque_thresholds_nominal[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_force_thresholds_acceleration[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_force_thresholds_acceleration[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_force_thresholds_nominal[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_force_thresholds_nominal[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetFullCollisionBehavior_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>416{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<416{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetFullCollisionBehavior_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetFullCollisionBehavior_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetFullCollisionBehavior_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetFullCollisionBehavior_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_76430e384e9ad8b3f3c937e3004e8dc15a51d28abecd108c698e60d39c54f300";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetFullCollisionBehavior_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetJointStiffness_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetJointStiffness_Request {
  /// Declared joint_stiffness; canonical bounds are part of CODEC_IDENTITY.
  pub joint_stiffness:[f64;7],
}
impl Default for Ros2Franka_msgsSrvSetJointStiffness_Request {fn default()->Self {Self {
joint_stiffness:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetJointStiffness_Request {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_65b5d6599d1b97a0356f599102c22e0ae8c33b85f14c5a49b6c097eb227d334c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.joint_stiffness {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>56{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.joint_stiffness {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_stiffness[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetJointStiffness_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetJointStiffness_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetJointStiffness_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetJointStiffness_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetJointStiffness_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1a857ab809079e0eec664250a9521ddc1cdf66e61aa2e0f22eae8442f5231c4d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetJointStiffness_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetLoad_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetLoad_Request {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f64,
  /// Declared center_of_mass; canonical bounds are part of CODEC_IDENTITY.
  pub center_of_mass:[f64;3],
  /// Declared load_inertia; canonical bounds are part of CODEC_IDENTITY.
  pub load_inertia:[f64;9],
}
impl Default for Ros2Franka_msgsSrvSetLoad_Request {fn default()->Self {Self {
mass:Default::default(),
center_of_mass:core::array::from_fn(|_|Default::default()),
load_inertia:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetLoad_Request {
  pub const MAX_WIRE_BYTES:usize=104;
  pub const CARTESIAN_WIRE_BYTES:u64=104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a616a844864b7bbf4ee18ac481ddd8406bec47354b4519a0e17f8a305635b7f8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.center_of_mass {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.load_inertia {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
for element in &self.center_of_mass {
put(out,at,&element.to_le_bytes());
}
for element in &self.load_inertia {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.center_of_mass[i]=decoded;
}}
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.load_inertia[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetLoad_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<104{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetLoad_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetLoad_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetLoad_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetLoad_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_991ff575719ef9442063a13c45a9660c672f5607e6e40cf6ab5c7a3173fa00bf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetLoad_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetStiffnessFrame_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetStiffnessFrame_Request {
  /// Declared transformation; canonical bounds are part of CODEC_IDENTITY.
  pub transformation:[f64;16],
}
impl Default for Ros2Franka_msgsSrvSetStiffnessFrame_Request {fn default()->Self {Self {
transformation:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetStiffnessFrame_Request {
  pub const MAX_WIRE_BYTES:usize=128;
  pub const CARTESIAN_WIRE_BYTES:u64=128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f8ee91daba1642286b63178c18bf2a005597e94951f11a185241193132c9f402";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.transformation {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.transformation {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.transformation[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetStiffnessFrame_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<128{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetStiffnessFrame_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetStiffnessFrame_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetStiffnessFrame_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetStiffnessFrame_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e32cb83b1f0f3cda363566525c0b1fe367190f2e66f68282b330f4517f6ea744";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetStiffnessFrame_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetTCPFrame_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetTCPFrame_Request {
  /// Declared transformation; canonical bounds are part of CODEC_IDENTITY.
  pub transformation:[f64;16],
}
impl Default for Ros2Franka_msgsSrvSetTCPFrame_Request {fn default()->Self {Self {
transformation:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Franka_msgsSrvSetTCPFrame_Request {
  pub const MAX_WIRE_BYTES:usize=128;
  pub const CARTESIAN_WIRE_BYTES:u64=128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2ef445bfa494a6bc50ee3337695562255c06890216c30ba17df95e691871441f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.transformation {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.transformation {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.transformation[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetTCPFrame_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<128{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_msgs/srv/SetTCPFrame_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_msgsSrvSetTCPFrame_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_msgsSrvSetTCPFrame_Response {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_msgsSrvSetTCPFrame_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_87e4f5556c292289fe17898a40f1f5dd6161e7cc87ca570c86e9f69080d1df38";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_msgsSrvSetTCPFrame_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/action/MoveAbsolute_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsActionMoveAbsolute_Goal {
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:f64,
  /// Declared acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub acceleration:f64,
  /// Declared deceleration; canonical bounds are part of CODEC_IDENTITY.
  pub deceleration:f64,
}
impl Default for Ros2Franka_spine_msgsActionMoveAbsolute_Goal {fn default()->Self {Self {
position:Default::default(),
velocity:Default::default(),
acceleration:Default::default(),
deceleration:Default::default(),
}}}
impl Ros2Franka_spine_msgsActionMoveAbsolute_Goal {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a0c2f9037fd6730641b19b6da6ce1c6ca03bbd517fdd791c159effc93290cf13";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.acceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.deceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.position.to_le_bytes());
put(out,at,&self.velocity.to_le_bytes());
put(out,at,&self.acceleration.to_le_bytes());
put(out,at,&self.deceleration.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.acceleration=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.deceleration=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsActionMoveAbsolute_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/action/MoveAbsolute_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsActionMoveAbsolute_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared stop_by; canonical bounds are part of CODEC_IDENTITY.
  pub stop_by:String,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros2Franka_spine_msgsActionMoveAbsolute_Result {fn default()->Self {Self {
success:Default::default(),
stop_by:Default::default(),
error:Default::default(),
}}}
impl Ros2Franka_spine_msgsActionMoveAbsolute_Result {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86b5c823e6e6604595462a76cd259daef420b2dff169f6dbe00f742f079155d5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.stop_by.len()>256||self.stop_by.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.stop_by.len())?;
if self.error.len()>256||self.error.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.stop_by.len() as u32).to_le_bytes());put(out,at,self.stop_by.as_bytes());
put(out,at,&(self.error.len() as u32).to_le_bytes());put(out,at,self.error.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.stop_by=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsActionMoveAbsolute_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/action/MoveAbsolute_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsActionMoveAbsolute_Feedback {
  /// Declared current_position; canonical bounds are part of CODEC_IDENTITY.
  pub current_position:f64,
}
impl Default for Ros2Franka_spine_msgsActionMoveAbsolute_Feedback {fn default()->Self {Self {
current_position:Default::default(),
}}}
impl Ros2Franka_spine_msgsActionMoveAbsolute_Feedback {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cf2f760313ab8c6cc01dd79271619926320021746451f376e56d18f63ffad929";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current_position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current_position.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current_position=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsActionMoveAbsolute_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/msg/UserLimits.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsMsgUserLimits {
  /// Declared lower_limit; canonical bounds are part of CODEC_IDENTITY.
  pub lower_limit:f64,
  /// Declared upper_limit; canonical bounds are part of CODEC_IDENTITY.
  pub upper_limit:f64,
}
impl Default for Ros2Franka_spine_msgsMsgUserLimits {fn default()->Self {Self {
lower_limit:Default::default(),
upper_limit:Default::default(),
}}}
impl Ros2Franka_spine_msgsMsgUserLimits {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_20256e37f0ae710b5cb9ab9438b5ff8144ce2ab6fc606c5335d0f5de5384e368";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.lower_limit.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.upper_limit.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.lower_limit.to_le_bytes());
put(out,at,&self.upper_limit.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lower_limit=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.upper_limit=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsMsgUserLimits {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/msg/SpineParameters.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsMsgSpineParameters {
  /// Declared user_limits; canonical bounds are part of CODEC_IDENTITY.
  pub user_limits:Ros2Franka_spine_msgsMsgUserLimits,
}
impl Default for Ros2Franka_spine_msgsMsgSpineParameters {fn default()->Self {Self {
user_limits:Default::default(),
}}}
impl Ros2Franka_spine_msgsMsgSpineParameters {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d4c45cfccf278c99a189e92d438d0b11b2b693012cb0f76b65b6822eb816223";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.user_limits.wire_size()?)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.user_limits.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Franka_spine_msgsMsgUserLimits::read_fields(bytes,at)?;
value.user_limits=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsMsgSpineParameters {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/msg/SpinePosition.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsMsgSpinePosition {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared valid; canonical bounds are part of CODEC_IDENTITY.
  pub valid:bool,
}
impl Default for Ros2Franka_spine_msgsMsgSpinePosition {fn default()->Self {Self {
header:Default::default(),
position:Default::default(),
valid:Default::default(),
}}}
impl Ros2Franka_spine_msgsMsgSpinePosition {
  pub const MAX_WIRE_BYTES:usize=149;
  pub const CARTESIAN_WIRE_BYTES:u64=149;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71be15c40f39074dfdbe79a3587847ae512c5a3c8df23cc9415d5bfe59d05775";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
if n>149{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.position.to_le_bytes());
put(out,at,&[u8::from(self.valid)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.valid=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsMsgSpinePosition {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>149{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<21{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/msg/SpineState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsMsgSpineState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:String,
  /// Declared error_description; canonical bounds are part of CODEC_IDENTITY.
  pub error_description:String,
  /// Declared valid; canonical bounds are part of CODEC_IDENTITY.
  pub valid:bool,
}
impl Default for Ros2Franka_spine_msgsMsgSpineState {fn default()->Self {Self {
header:Default::default(),
state:Default::default(),
error_code:Default::default(),
error_description:Default::default(),
valid:Default::default(),
}}}
impl Ros2Franka_spine_msgsMsgSpineState {
  pub const MAX_WIRE_BYTES:usize=921;
  pub const CARTESIAN_WIRE_BYTES:u64=921;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ad0b5dc5d79956da854582765c147d3d33edab569c4829e69cc7990edfb89441";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
if self.error_code.len()>256||self.error_code.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_code.len())?;
if self.error_description.len()>256||self.error_description.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_description.len())?;
n=add_size(n,1)?;
if n>921{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&(self.error_code.len() as u32).to_le_bytes());put(out,at,self.error_code.as_bytes());
put(out,at,&(self.error_description.len() as u32).to_le_bytes());put(out,at,self.error_description.as_bytes());
put(out,at,&[u8::from(self.valid)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_code=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_description=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.valid=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsMsgSpineState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>921{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/FaultReset_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvFaultReset_Request {
}
impl Default for Ros2Franka_spine_msgsSrvFaultReset_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvFaultReset_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0f836229e8e8a1713b9e9a8d7f0f43677ad93380e1f7350ff1597d25067b7f33";
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
impl Canonical for Ros2Franka_spine_msgsSrvFaultReset_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/FaultReset_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvFaultReset_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Franka_spine_msgsSrvFaultReset_Response {fn default()->Self {Self {
state:Default::default(),
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvFaultReset_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ebeded0dbe4a4197a52de3fa43dc57fa372bd4e6ef5e2ded2558ccbf691410c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvFaultReset_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetParameters_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetParameters_Request {
}
impl Default for Ros2Franka_spine_msgsSrvGetParameters_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvGetParameters_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_80ef5d2ef6ff2df9d14f29417d1928429315c0b7be9ec59513b9b3943a970b14";
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
impl Canonical for Ros2Franka_spine_msgsSrvGetParameters_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetParameters_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetParameters_Response {
  /// Declared parameters; canonical bounds are part of CODEC_IDENTITY.
  pub parameters:Ros2Franka_spine_msgsMsgSpineParameters,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Franka_spine_msgsSrvGetParameters_Response {fn default()->Self {Self {
parameters:Default::default(),
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvGetParameters_Response {
  pub const MAX_WIRE_BYTES:usize=277;
  pub const CARTESIAN_WIRE_BYTES:u64=277;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8e648792534d7f4996179f31d353d2594cf63031edef4451fd20a3838dc896ed";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.parameters.wire_size()?)?;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>277{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.parameters.write_fields(out,at);
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Franka_spine_msgsMsgSpineParameters::read_fields(bytes,at)?;
value.parameters=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvGetParameters_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>277{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<21{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetPosition_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetPosition_Request {
}
impl Default for Ros2Franka_spine_msgsSrvGetPosition_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvGetPosition_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_66b05d1f08331161cafed301b3ab95a60dbb16aeb1a930ee780db163cb37c6c2";
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
impl Canonical for Ros2Franka_spine_msgsSrvGetPosition_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetPosition_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetPosition_Response {
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Franka_spine_msgsSrvGetPosition_Response {fn default()->Self {Self {
position:Default::default(),
success:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvGetPosition_Response {
  pub const MAX_WIRE_BYTES:usize=9;
  pub const CARTESIAN_WIRE_BYTES:u64=9;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_18d9af57cc9f82c3c03d01e850da254e9431820b8291f03dafd2e8f74e849ab8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
if n>9{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.position.to_le_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvGetPosition_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetSpineState_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetSpineState_Request {
}
impl Default for Ros2Franka_spine_msgsSrvGetSpineState_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvGetSpineState_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c4c456174d7b6647d09d986d37c041b942418941a5e01c447486727e919b8a67";
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
impl Canonical for Ros2Franka_spine_msgsSrvGetSpineState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/GetSpineState_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvGetSpineState_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:String,
  /// Declared error_description; canonical bounds are part of CODEC_IDENTITY.
  pub error_description:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Franka_spine_msgsSrvGetSpineState_Response {fn default()->Self {Self {
state:Default::default(),
error_code:Default::default(),
error_description:Default::default(),
success:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvGetSpineState_Response {
  pub const MAX_WIRE_BYTES:usize=781;
  pub const CARTESIAN_WIRE_BYTES:u64=781;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_297d70e2c5c24dcea4bdf8c5f9e5c8c32256e8f35f1900d8afd914ab2acfdcd1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
if self.error_code.len()>256||self.error_code.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_code.len())?;
if self.error_description.len()>256||self.error_description.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_description.len())?;
n=add_size(n,1)?;
if n>781{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&(self.error_code.len() as u32).to_le_bytes());put(out,at,self.error_code.as_bytes());
put(out,at,&(self.error_description.len() as u32).to_le_bytes());put(out,at,self.error_description.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_code=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_description=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvGetSpineState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>781{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/Halt_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvHalt_Request {
}
impl Default for Ros2Franka_spine_msgsSrvHalt_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvHalt_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_50af7625abf7ec20a30ec49b01e8d32de2ee0d4710820c8de659df211be8c111";
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
impl Canonical for Ros2Franka_spine_msgsSrvHalt_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/Halt_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvHalt_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Franka_spine_msgsSrvHalt_Response {fn default()->Self {Self {
state:Default::default(),
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvHalt_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_150e1d90b485a6ddf79f771379a0e1d3f99a77f88ab1cf1f569d614a63ac3d77";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvHalt_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/SwitchOff_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvSwitchOff_Request {
}
impl Default for Ros2Franka_spine_msgsSrvSwitchOff_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvSwitchOff_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_097f00adfdc8d1a49615792158a36052917c41385683b39de6686f591964fc16";
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
impl Canonical for Ros2Franka_spine_msgsSrvSwitchOff_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/SwitchOff_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvSwitchOff_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Franka_spine_msgsSrvSwitchOff_Response {fn default()->Self {Self {
state:Default::default(),
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvSwitchOff_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d3ab8516e62428b2caf71673f398ccc7199b7b25aac4c37b42c15a79878e54ad";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvSwitchOff_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/SwitchOn_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvSwitchOn_Request {
}
impl Default for Ros2Franka_spine_msgsSrvSwitchOn_Request {fn default()->Self {Self {
}}}
impl Ros2Franka_spine_msgsSrvSwitchOn_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_840d12dbe4d4a8558e72c2facbccfc547de35536f749573b6de60ce9a9238a56";
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
impl Canonical for Ros2Franka_spine_msgsSrvSwitchOn_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 franka_spine_msgs/srv/SwitchOn_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Franka_spine_msgsSrvSwitchOn_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Franka_spine_msgsSrvSwitchOn_Response {fn default()->Self {Self {
state:Default::default(),
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Franka_spine_msgsSrvSwitchOn_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8c58d63385a5fc6f160585504b3c9c5ca5375c6187f7a494c3db696c5cc7a37d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Franka_spine_msgsSrvSwitchOn_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Franka_msgsMsgTargetStatus {pub const IDLE:u8=0;
pub const EXECUTING:u8=1;
pub const TARGET_REACHED:u8=2;
pub const ABORTED:u8=3;}

impl Ros2Franka_msgsMsgFrankaRobotState {pub const ROBOT_MODE_OTHER:u8=0;
pub const ROBOT_MODE_IDLE:u8=1;
pub const ROBOT_MODE_MOVE:u8=2;
pub const ROBOT_MODE_GUIDING:u8=3;
pub const ROBOT_MODE_REFLEX:u8=4;
pub const ROBOT_MODE_USER_STOPPED:u8=5;
pub const ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY:u8=6;}
