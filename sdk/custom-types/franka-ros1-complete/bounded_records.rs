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
/// Complete bounded value for ROS1 franka_example_controllers/JointTorqueComparison.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_example_controllersJointTorqueComparison {
  /// Declared tau_error; canonical bounds are part of CODEC_IDENTITY.
  pub tau_error:[f64;7],
  /// Declared tau_commanded; canonical bounds are part of CODEC_IDENTITY.
  pub tau_commanded:[f64;7],
  /// Declared tau_measured; canonical bounds are part of CODEC_IDENTITY.
  pub tau_measured:[f64;7],
  /// Declared root_mean_square_error; canonical bounds are part of CODEC_IDENTITY.
  pub root_mean_square_error:f64,
}
impl Default for Ros1Franka_example_controllersJointTorqueComparison {fn default()->Self {Self {
tau_error:core::array::from_fn(|_|Default::default()),
tau_commanded:core::array::from_fn(|_|Default::default()),
tau_measured:core::array::from_fn(|_|Default::default()),
root_mean_square_error:Default::default(),
}}}
impl Ros1Franka_example_controllersJointTorqueComparison {
  pub const MAX_WIRE_BYTES:usize=176;
  pub const CARTESIAN_WIRE_BYTES:u64=176;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dde6eaf404002642e64af59eb5367948b00bc07c0b2fd8a57e359e9df3084254";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.tau_error {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.tau_commanded {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.tau_measured {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.root_mean_square_error.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>176{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.tau_error {
put(out,at,&element.to_le_bytes());
}
for element in &self.tau_commanded {
put(out,at,&element.to_le_bytes());
}
for element in &self.tau_measured {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.root_mean_square_error.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_error[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_commanded[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_measured[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.root_mean_square_error=decoded;
Ok(value)}
}
impl Canonical for Ros1Franka_example_controllersJointTorqueComparison {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>176{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<176{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/GraspEpsilon.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperGraspEpsilon {
  /// Declared inner; canonical bounds are part of CODEC_IDENTITY.
  pub inner:f64,
  /// Declared outer; canonical bounds are part of CODEC_IDENTITY.
  pub outer:f64,
}
impl Default for Ros1Franka_gripperGraspEpsilon {fn default()->Self {Self {
inner:Default::default(),
outer:Default::default(),
}}}
impl Ros1Franka_gripperGraspEpsilon {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_538ac801e9874841f232909d2d9a16893a8d29c8380cd623c00c408690f978f9";
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
impl Canonical for Ros1Franka_gripperGraspEpsilon {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/GraspGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperGraspGoal {
  /// Declared width; canonical bounds are part of CODEC_IDENTITY.
  pub width:f64,
  /// Declared epsilon; canonical bounds are part of CODEC_IDENTITY.
  pub epsilon:Ros1Franka_gripperGraspEpsilon,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f64,
  /// Declared force; canonical bounds are part of CODEC_IDENTITY.
  pub force:f64,
}
impl Default for Ros1Franka_gripperGraspGoal {fn default()->Self {Self {
width:Default::default(),
epsilon:Default::default(),
speed:Default::default(),
force:Default::default(),
}}}
impl Ros1Franka_gripperGraspGoal {
  pub const MAX_WIRE_BYTES:usize=40;
  pub const CARTESIAN_WIRE_BYTES:u64=40;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d3dd0d3f47e49e06a95d907a6219335b35234749518ab645e102257595ed7bcf";
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
let decoded=Ros1Franka_gripperGraspEpsilon::read_fields(bytes,at)?;
value.epsilon=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.force=decoded;
Ok(value)}
}
impl Canonical for Ros1Franka_gripperGraspGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>40{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/GraspResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperGraspResult {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_gripperGraspResult {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_gripperGraspResult {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9ce1e4f95f16fd6c1603f0b30a9fcc33e4f8436cdb96710ea91eb20b38a1e02e";
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
impl Canonical for Ros1Franka_gripperGraspResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/GraspFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperGraspFeedback {
}
impl Default for Ros1Franka_gripperGraspFeedback {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperGraspFeedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_24ca23625df0bd9fb984e65c8ecd7a3af162a3db00c15951225df5e18d3a4d0d";
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
impl Canonical for Ros1Franka_gripperGraspFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/HomingGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperHomingGoal {
}
impl Default for Ros1Franka_gripperHomingGoal {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperHomingGoal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b4f861fa9ff44afda4217cf613720e90353b0d43c1fc3425d5937157353910f";
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
impl Canonical for Ros1Franka_gripperHomingGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/HomingResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperHomingResult {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_gripperHomingResult {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_gripperHomingResult {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ddf202b4e7dbf100d304b4eb32aa0ce8dca6ffadcf720820f2d7932af056477";
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
impl Canonical for Ros1Franka_gripperHomingResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/HomingFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperHomingFeedback {
}
impl Default for Ros1Franka_gripperHomingFeedback {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperHomingFeedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4a1118d8ab544b2bc99ff878356aae49b4d3ecb4cb5ba7e2f93f7c0c7a29512e";
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
impl Canonical for Ros1Franka_gripperHomingFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/MoveGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperMoveGoal {
  /// Declared width; canonical bounds are part of CODEC_IDENTITY.
  pub width:f64,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f64,
}
impl Default for Ros1Franka_gripperMoveGoal {fn default()->Self {Self {
width:Default::default(),
speed:Default::default(),
}}}
impl Ros1Franka_gripperMoveGoal {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c83296f2540854e78cc159691c229f5d50267b3fdb31789e325a55fcd7d6830a";
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
impl Canonical for Ros1Franka_gripperMoveGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/MoveResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperMoveResult {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_gripperMoveResult {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_gripperMoveResult {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_527adf0720dabcc91866a0858cddfbfae1ed4203d4eb34a3965ebe085d0ac3b3";
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
impl Canonical for Ros1Franka_gripperMoveResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/MoveFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperMoveFeedback {
}
impl Default for Ros1Franka_gripperMoveFeedback {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperMoveFeedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8e02ca1d66ae38a5d71ca5f708aa2c57bb3df573d37b692e24986dba384c5f80";
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
impl Canonical for Ros1Franka_gripperMoveFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/StopGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperStopGoal {
}
impl Default for Ros1Franka_gripperStopGoal {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperStopGoal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ecb09ccc03cb2140a457a11c174a614392ce1f9263e46c5b82427a99f7ff0245";
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
impl Canonical for Ros1Franka_gripperStopGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/StopResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperStopResult {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_gripperStopResult {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_gripperStopResult {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_343e16d9b839b61ed8f8c1ce93005cb3a21511a220095200eb075dfb62a40c61";
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
impl Canonical for Ros1Franka_gripperStopResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_gripper/StopFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_gripperStopFeedback {
}
impl Default for Ros1Franka_gripperStopFeedback {fn default()->Self {Self {
}}}
impl Ros1Franka_gripperStopFeedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_62b637081e95f59df491f71b0ca84394a7e1f36137ca389a8f6d0d3a0f19038d";
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
impl Canonical for Ros1Franka_gripperStopFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/ErrorRecoveryGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsErrorRecoveryGoal {
}
impl Default for Ros1Franka_msgsErrorRecoveryGoal {fn default()->Self {Self {
}}}
impl Ros1Franka_msgsErrorRecoveryGoal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b4f11555bc4919bf59aa548bd21046bdc7b1e2ed5cce6dc46298eb7486ef7b64";
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
impl Canonical for Ros1Franka_msgsErrorRecoveryGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/ErrorRecoveryResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsErrorRecoveryResult {
}
impl Default for Ros1Franka_msgsErrorRecoveryResult {fn default()->Self {Self {
}}}
impl Ros1Franka_msgsErrorRecoveryResult {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6f06297f59f06dc46e3f983b72101ae3c25cd054bca44cea1f6dd11a9aaa7794";
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
impl Canonical for Ros1Franka_msgsErrorRecoveryResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/ErrorRecoveryFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsErrorRecoveryFeedback {
}
impl Default for Ros1Franka_msgsErrorRecoveryFeedback {fn default()->Self {Self {
}}}
impl Ros1Franka_msgsErrorRecoveryFeedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3faee6d8afdc0524598cec0fe30fd51bd723e8e44849f035368723e59be87a21";
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
impl Canonical for Ros1Franka_msgsErrorRecoveryFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/Errors.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsErrors {
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
  /// Declared joint_move_in_wrong_direction; canonical bounds are part of CODEC_IDENTITY.
  pub joint_move_in_wrong_direction:bool,
  /// Declared cartesian_spline_motion_generator_violation; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_spline_motion_generator_violation:bool,
  /// Declared joint_via_motion_generator_planning_joint_limit_violation; canonical bounds are part of CODEC_IDENTITY.
  pub joint_via_motion_generator_planning_joint_limit_violation:bool,
  /// Declared base_acceleration_initialization_timeout; canonical bounds are part of CODEC_IDENTITY.
  pub base_acceleration_initialization_timeout:bool,
  /// Declared base_acceleration_invalid_reading; canonical bounds are part of CODEC_IDENTITY.
  pub base_acceleration_invalid_reading:bool,
}
impl Default for Ros1Franka_msgsErrors {fn default()->Self {Self {
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
joint_move_in_wrong_direction:Default::default(),
cartesian_spline_motion_generator_violation:Default::default(),
joint_via_motion_generator_planning_joint_limit_violation:Default::default(),
base_acceleration_initialization_timeout:Default::default(),
base_acceleration_invalid_reading:Default::default(),
}}}
impl Ros1Franka_msgsErrors {
  pub const MAX_WIRE_BYTES:usize=41;
  pub const CARTESIAN_WIRE_BYTES:u64=41;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_80e3d1157d53bf80d4db3621f9f22b35a1ef665382974599ff016709c6a629ca";
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
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>41{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
put(out,at,&[u8::from(self.joint_move_in_wrong_direction)]);
put(out,at,&[u8::from(self.cartesian_spline_motion_generator_violation)]);
put(out,at,&[u8::from(self.joint_via_motion_generator_planning_joint_limit_violation)]);
put(out,at,&[u8::from(self.base_acceleration_initialization_timeout)]);
put(out,at,&[u8::from(self.base_acceleration_invalid_reading)]);
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
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_move_in_wrong_direction=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cartesian_spline_motion_generator_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.joint_via_motion_generator_planning_joint_limit_violation=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.base_acceleration_initialization_timeout=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.base_acceleration_invalid_reading=decoded;
Ok(value)}
}
impl Canonical for Ros1Franka_msgsErrors {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>41{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<41{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=144;
  pub const CARTESIAN_WIRE_BYTES:u64=144;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,self.stamp.wire_size()?)?;
if self.frame_id.len()>128||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if n>144{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
Ok(value)}
}
impl Canonical for Ros1Std_msgsHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>144{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/FrankaState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsFrankaState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared cartesian_collision; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_collision:[f64;6],
  /// Declared cartesian_contact; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_contact:[f64;6],
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:[f64;7],
  /// Declared q_d; canonical bounds are part of CODEC_IDENTITY.
  pub q_d:[f64;7],
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:[f64;7],
  /// Declared dq_d; canonical bounds are part of CODEC_IDENTITY.
  pub dq_d:[f64;7],
  /// Declared ddq_d; canonical bounds are part of CODEC_IDENTITY.
  pub ddq_d:[f64;7],
  /// Declared theta; canonical bounds are part of CODEC_IDENTITY.
  pub theta:[f64;7],
  /// Declared dtheta; canonical bounds are part of CODEC_IDENTITY.
  pub dtheta:[f64;7],
  /// Declared tau_J; canonical bounds are part of CODEC_IDENTITY.
  pub tau_J:[f64;7],
  /// Declared dtau_J; canonical bounds are part of CODEC_IDENTITY.
  pub dtau_J:[f64;7],
  /// Declared tau_J_d; canonical bounds are part of CODEC_IDENTITY.
  pub tau_J_d:[f64;7],
  /// Declared K_F_ext_hat_K; canonical bounds are part of CODEC_IDENTITY.
  pub K_F_ext_hat_K:[f64;6],
  /// Declared elbow; canonical bounds are part of CODEC_IDENTITY.
  pub elbow:[f64;2],
  /// Declared elbow_d; canonical bounds are part of CODEC_IDENTITY.
  pub elbow_d:[f64;2],
  /// Declared elbow_c; canonical bounds are part of CODEC_IDENTITY.
  pub elbow_c:[f64;2],
  /// Declared delbow_c; canonical bounds are part of CODEC_IDENTITY.
  pub delbow_c:[f64;2],
  /// Declared ddelbow_c; canonical bounds are part of CODEC_IDENTITY.
  pub ddelbow_c:[f64;2],
  /// Declared joint_collision; canonical bounds are part of CODEC_IDENTITY.
  pub joint_collision:[f64;7],
  /// Declared joint_contact; canonical bounds are part of CODEC_IDENTITY.
  pub joint_contact:[f64;7],
  /// Declared O_F_ext_hat_K; canonical bounds are part of CODEC_IDENTITY.
  pub O_F_ext_hat_K:[f64;6],
  /// Declared O_dP_EE_d; canonical bounds are part of CODEC_IDENTITY.
  pub O_dP_EE_d:[f64;6],
  /// Declared O_ddP_O; canonical bounds are part of CODEC_IDENTITY.
  pub O_ddP_O:[f64;3],
  /// Declared O_dP_EE_c; canonical bounds are part of CODEC_IDENTITY.
  pub O_dP_EE_c:[f64;6],
  /// Declared O_ddP_EE_c; canonical bounds are part of CODEC_IDENTITY.
  pub O_ddP_EE_c:[f64;6],
  /// Declared tau_ext_hat_filtered; canonical bounds are part of CODEC_IDENTITY.
  pub tau_ext_hat_filtered:[f64;7],
  /// Declared m_ee; canonical bounds are part of CODEC_IDENTITY.
  pub m_ee:f64,
  /// Declared F_x_Cee; canonical bounds are part of CODEC_IDENTITY.
  pub F_x_Cee:[f64;3],
  /// Declared I_ee; canonical bounds are part of CODEC_IDENTITY.
  pub I_ee:[f64;9],
  /// Declared m_load; canonical bounds are part of CODEC_IDENTITY.
  pub m_load:f64,
  /// Declared F_x_Cload; canonical bounds are part of CODEC_IDENTITY.
  pub F_x_Cload:[f64;3],
  /// Declared I_load; canonical bounds are part of CODEC_IDENTITY.
  pub I_load:[f64;9],
  /// Declared m_total; canonical bounds are part of CODEC_IDENTITY.
  pub m_total:f64,
  /// Declared F_x_Ctotal; canonical bounds are part of CODEC_IDENTITY.
  pub F_x_Ctotal:[f64;3],
  /// Declared I_total; canonical bounds are part of CODEC_IDENTITY.
  pub I_total:[f64;9],
  /// Declared O_T_EE; canonical bounds are part of CODEC_IDENTITY.
  pub O_T_EE:[f64;16],
  /// Declared O_T_EE_d; canonical bounds are part of CODEC_IDENTITY.
  pub O_T_EE_d:[f64;16],
  /// Declared O_T_EE_c; canonical bounds are part of CODEC_IDENTITY.
  pub O_T_EE_c:[f64;16],
  /// Declared F_T_EE; canonical bounds are part of CODEC_IDENTITY.
  pub F_T_EE:[f64;16],
  /// Declared F_T_NE; canonical bounds are part of CODEC_IDENTITY.
  pub F_T_NE:[f64;16],
  /// Declared NE_T_EE; canonical bounds are part of CODEC_IDENTITY.
  pub NE_T_EE:[f64;16],
  /// Declared EE_T_K; canonical bounds are part of CODEC_IDENTITY.
  pub EE_T_K:[f64;16],
  /// Declared time; canonical bounds are part of CODEC_IDENTITY.
  pub time:f64,
  /// Declared control_command_success_rate; canonical bounds are part of CODEC_IDENTITY.
  pub control_command_success_rate:f64,
  /// Declared robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub robot_mode:u8,
  /// Declared current_errors; canonical bounds are part of CODEC_IDENTITY.
  pub current_errors:Ros1Franka_msgsErrors,
  /// Declared last_motion_errors; canonical bounds are part of CODEC_IDENTITY.
  pub last_motion_errors:Ros1Franka_msgsErrors,
}
impl Default for Ros1Franka_msgsFrankaState {fn default()->Self {Self {
header:Default::default(),
cartesian_collision:core::array::from_fn(|_|Default::default()),
cartesian_contact:core::array::from_fn(|_|Default::default()),
q:core::array::from_fn(|_|Default::default()),
q_d:core::array::from_fn(|_|Default::default()),
dq:core::array::from_fn(|_|Default::default()),
dq_d:core::array::from_fn(|_|Default::default()),
ddq_d:core::array::from_fn(|_|Default::default()),
theta:core::array::from_fn(|_|Default::default()),
dtheta:core::array::from_fn(|_|Default::default()),
tau_J:core::array::from_fn(|_|Default::default()),
dtau_J:core::array::from_fn(|_|Default::default()),
tau_J_d:core::array::from_fn(|_|Default::default()),
K_F_ext_hat_K:core::array::from_fn(|_|Default::default()),
elbow:core::array::from_fn(|_|Default::default()),
elbow_d:core::array::from_fn(|_|Default::default()),
elbow_c:core::array::from_fn(|_|Default::default()),
delbow_c:core::array::from_fn(|_|Default::default()),
ddelbow_c:core::array::from_fn(|_|Default::default()),
joint_collision:core::array::from_fn(|_|Default::default()),
joint_contact:core::array::from_fn(|_|Default::default()),
O_F_ext_hat_K:core::array::from_fn(|_|Default::default()),
O_dP_EE_d:core::array::from_fn(|_|Default::default()),
O_ddP_O:core::array::from_fn(|_|Default::default()),
O_dP_EE_c:core::array::from_fn(|_|Default::default()),
O_ddP_EE_c:core::array::from_fn(|_|Default::default()),
tau_ext_hat_filtered:core::array::from_fn(|_|Default::default()),
m_ee:Default::default(),
F_x_Cee:core::array::from_fn(|_|Default::default()),
I_ee:core::array::from_fn(|_|Default::default()),
m_load:Default::default(),
F_x_Cload:core::array::from_fn(|_|Default::default()),
I_load:core::array::from_fn(|_|Default::default()),
m_total:Default::default(),
F_x_Ctotal:core::array::from_fn(|_|Default::default()),
I_total:core::array::from_fn(|_|Default::default()),
O_T_EE:core::array::from_fn(|_|Default::default()),
O_T_EE_d:core::array::from_fn(|_|Default::default()),
O_T_EE_c:core::array::from_fn(|_|Default::default()),
F_T_EE:core::array::from_fn(|_|Default::default()),
F_T_NE:core::array::from_fn(|_|Default::default()),
NE_T_EE:core::array::from_fn(|_|Default::default()),
EE_T_K:core::array::from_fn(|_|Default::default()),
time:Default::default(),
control_command_success_rate:Default::default(),
robot_mode:Default::default(),
current_errors:Default::default(),
last_motion_errors:Default::default(),
}}}
impl Ros1Franka_msgsFrankaState {
  pub const MAX_WIRE_BYTES:usize=2619;
  pub const CARTESIAN_WIRE_BYTES:u64=2619;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_79f22f36ce2c11f519ee29e65a248ec1b67240e27712b2667e9b803a2562ee71";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
for element in &self.cartesian_collision {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.cartesian_contact {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.q {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.q_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.dq {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.dq_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.ddq_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.theta {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.dtheta {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.tau_J {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.dtau_J {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.tau_J_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.K_F_ext_hat_K {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.elbow {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.elbow_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.elbow_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.delbow_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.ddelbow_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.joint_collision {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.joint_contact {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_F_ext_hat_K {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_dP_EE_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_ddP_O {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_dP_EE_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_ddP_EE_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.tau_ext_hat_filtered {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.m_ee.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.F_x_Cee {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.I_ee {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.m_load.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.F_x_Cload {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.I_load {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.m_total.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.F_x_Ctotal {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.I_total {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_T_EE {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_T_EE_d {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.O_T_EE_c {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.F_T_EE {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.F_T_NE {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.NE_T_EE {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.EE_T_K {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.time.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.control_command_success_rate.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,self.current_errors.wire_size()?)?;
n=add_size(n,self.last_motion_errors.wire_size()?)?;
if n>2619{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
for element in &self.cartesian_collision {
put(out,at,&element.to_le_bytes());
}
for element in &self.cartesian_contact {
put(out,at,&element.to_le_bytes());
}
for element in &self.q {
put(out,at,&element.to_le_bytes());
}
for element in &self.q_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.dq {
put(out,at,&element.to_le_bytes());
}
for element in &self.dq_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.ddq_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.theta {
put(out,at,&element.to_le_bytes());
}
for element in &self.dtheta {
put(out,at,&element.to_le_bytes());
}
for element in &self.tau_J {
put(out,at,&element.to_le_bytes());
}
for element in &self.dtau_J {
put(out,at,&element.to_le_bytes());
}
for element in &self.tau_J_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.K_F_ext_hat_K {
put(out,at,&element.to_le_bytes());
}
for element in &self.elbow {
put(out,at,&element.to_le_bytes());
}
for element in &self.elbow_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.elbow_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.delbow_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.ddelbow_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.joint_collision {
put(out,at,&element.to_le_bytes());
}
for element in &self.joint_contact {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_F_ext_hat_K {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_dP_EE_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_ddP_O {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_dP_EE_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_ddP_EE_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.tau_ext_hat_filtered {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.m_ee.to_le_bytes());
for element in &self.F_x_Cee {
put(out,at,&element.to_le_bytes());
}
for element in &self.I_ee {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.m_load.to_le_bytes());
for element in &self.F_x_Cload {
put(out,at,&element.to_le_bytes());
}
for element in &self.I_load {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.m_total.to_le_bytes());
for element in &self.F_x_Ctotal {
put(out,at,&element.to_le_bytes());
}
for element in &self.I_total {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_T_EE {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_T_EE_d {
put(out,at,&element.to_le_bytes());
}
for element in &self.O_T_EE_c {
put(out,at,&element.to_le_bytes());
}
for element in &self.F_T_EE {
put(out,at,&element.to_le_bytes());
}
for element in &self.F_T_NE {
put(out,at,&element.to_le_bytes());
}
for element in &self.NE_T_EE {
put(out,at,&element.to_le_bytes());
}
for element in &self.EE_T_K {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.time.to_le_bytes());
put(out,at,&self.control_command_success_rate.to_le_bytes());
put(out,at,&self.robot_mode.to_le_bytes());
self.current_errors.write_fields(out,at);
self.last_motion_errors.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cartesian_collision[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cartesian_contact[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q_d[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dq[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dq_d[i]=decoded;
}}
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
value.theta[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dtheta[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_J[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dtau_J[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_J_d[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.K_F_ext_hat_K[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.elbow[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.elbow_d[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.elbow_c[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.delbow_c[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ddelbow_c[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_collision[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_contact[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_F_ext_hat_K[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_dP_EE_d[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_ddP_O[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_dP_EE_c[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_ddP_EE_c[i]=decoded;
}}
{let count=7usize;if count>7{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_ext_hat_filtered[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.m_ee=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.F_x_Cee[i]=decoded;
}}
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.I_ee[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.m_load=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.F_x_Cload[i]=decoded;
}}
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.I_load[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.m_total=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.F_x_Ctotal[i]=decoded;
}}
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.I_total[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_T_EE[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_T_EE_d[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.O_T_EE_c[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.F_T_EE[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.F_T_NE[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.NE_T_EE[i]=decoded;
}}
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.EE_T_K[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.time=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.control_command_success_rate=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.robot_mode=decoded;
let decoded=Ros1Franka_msgsErrors::read_fields(bytes,at)?;
value.current_errors=decoded;
let decoded=Ros1Franka_msgsErrors::read_fields(bytes,at)?;
value.last_motion_errors=decoded;
Ok(value)}
}
impl Canonical for Ros1Franka_msgsFrankaState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2619{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2491{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetCartesianImpedanceRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetCartesianImpedanceRequest {
  /// Declared cartesian_stiffness; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian_stiffness:[f64;6],
}
impl Default for Ros1Franka_msgsSetCartesianImpedanceRequest {fn default()->Self {Self {
cartesian_stiffness:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetCartesianImpedanceRequest {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ff604aae9ab1ef36b5af107966e64ee2115ee17618a4ece6f15997c889cacbdf";
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
impl Canonical for Ros1Franka_msgsSetCartesianImpedanceRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetCartesianImpedanceResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetCartesianImpedanceResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetCartesianImpedanceResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetCartesianImpedanceResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b091d2fe98c84528460ba3d84524c38355e725c161456edf0034e62c6f63ab5b";
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
impl Canonical for Ros1Franka_msgsSetCartesianImpedanceResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetEEFrameRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetEEFrameRequest {
  /// Declared NE_T_EE; canonical bounds are part of CODEC_IDENTITY.
  pub NE_T_EE:[f64;16],
}
impl Default for Ros1Franka_msgsSetEEFrameRequest {fn default()->Self {Self {
NE_T_EE:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetEEFrameRequest {
  pub const MAX_WIRE_BYTES:usize=128;
  pub const CARTESIAN_WIRE_BYTES:u64=128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1bc740d3998a14dd6c0604d5024ebda2b4916a8497950841b14a8073c6c1ecfc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.NE_T_EE {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.NE_T_EE {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.NE_T_EE[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Franka_msgsSetEEFrameRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<128{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetEEFrameResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetEEFrameResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetEEFrameResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetEEFrameResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b40c9595d57d6ed52ea4eca78e21f2c3d81f5a0afaad2420511211931f7d1c67";
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
impl Canonical for Ros1Franka_msgsSetEEFrameResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetForceTorqueCollisionBehaviorRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {
  /// Declared lower_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_torque_thresholds_nominal:[f64;7],
  /// Declared upper_torque_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_torque_thresholds_nominal:[f64;7],
  /// Declared lower_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub lower_force_thresholds_nominal:[f64;6],
  /// Declared upper_force_thresholds_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub upper_force_thresholds_nominal:[f64;6],
}
impl Default for Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {fn default()->Self {Self {
lower_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {
  pub const MAX_WIRE_BYTES:usize=208;
  pub const CARTESIAN_WIRE_BYTES:u64=208;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2513f0ea74bf6a871a85ce00c0f822891f2c0bed890b87e35d25a637aa62c4b5";
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
impl Canonical for Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>208{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<208{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetForceTorqueCollisionBehaviorResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_72418d73d504ae385d7ddce7e3cef859a104346dbfea7fc9da82e0f7e33ca2f5";
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
impl Canonical for Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetFullCollisionBehaviorRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetFullCollisionBehaviorRequest {
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
impl Default for Ros1Franka_msgsSetFullCollisionBehaviorRequest {fn default()->Self {Self {
lower_torque_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
lower_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_torque_thresholds_nominal:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_acceleration:core::array::from_fn(|_|Default::default()),
lower_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
upper_force_thresholds_nominal:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetFullCollisionBehaviorRequest {
  pub const MAX_WIRE_BYTES:usize=416;
  pub const CARTESIAN_WIRE_BYTES:u64=416;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_47a86620ec5abdd6cc820b095d8f5f4214c1d9af00ac1ff0740137bbdda7217c";
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
impl Canonical for Ros1Franka_msgsSetFullCollisionBehaviorRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>416{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<416{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetFullCollisionBehaviorResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetFullCollisionBehaviorResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetFullCollisionBehaviorResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetFullCollisionBehaviorResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_26f46e286c42d51a8ef45db7c8b3bf315fb02faf7b2be5d3741563d43169a0e2";
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
impl Canonical for Ros1Franka_msgsSetFullCollisionBehaviorResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetJointImpedanceRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetJointImpedanceRequest {
  /// Declared joint_stiffness; canonical bounds are part of CODEC_IDENTITY.
  pub joint_stiffness:[f64;7],
}
impl Default for Ros1Franka_msgsSetJointImpedanceRequest {fn default()->Self {Self {
joint_stiffness:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetJointImpedanceRequest {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_29b5419f698b39452d38fbc58a0e3c572b8d285b70c8acf7d326c440eb417c53";
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
impl Canonical for Ros1Franka_msgsSetJointImpedanceRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetJointImpedanceResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetJointImpedanceResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetJointImpedanceResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetJointImpedanceResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ae0cd7c4ad89de09ccffd6c4999435fa6965509e99a9adb98b7f3ebb3ea4c55";
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
impl Canonical for Ros1Franka_msgsSetJointImpedanceResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetKFrameRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetKFrameRequest {
  /// Declared EE_T_K; canonical bounds are part of CODEC_IDENTITY.
  pub EE_T_K:[f64;16],
}
impl Default for Ros1Franka_msgsSetKFrameRequest {fn default()->Self {Self {
EE_T_K:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetKFrameRequest {
  pub const MAX_WIRE_BYTES:usize=128;
  pub const CARTESIAN_WIRE_BYTES:u64=128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_36afcdc6402259fb5a71ae1b27be0bb8623a64dd93caffa376adea7862c86446";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.EE_T_K {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.EE_T_K {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=16usize;if count>16{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.EE_T_K[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Franka_msgsSetKFrameRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<128{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetKFrameResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetKFrameResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetKFrameResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetKFrameResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4ea77d3e827a9b9eaf3c3f9805f78c38bfe91789555d0469cffd4ff4266cd443";
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
impl Canonical for Ros1Franka_msgsSetKFrameResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetLoadRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetLoadRequest {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f64,
  /// Declared F_x_center_load; canonical bounds are part of CODEC_IDENTITY.
  pub F_x_center_load:[f64;3],
  /// Declared load_inertia; canonical bounds are part of CODEC_IDENTITY.
  pub load_inertia:[f64;9],
}
impl Default for Ros1Franka_msgsSetLoadRequest {fn default()->Self {Self {
mass:Default::default(),
F_x_center_load:core::array::from_fn(|_|Default::default()),
load_inertia:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Franka_msgsSetLoadRequest {
  pub const MAX_WIRE_BYTES:usize=104;
  pub const CARTESIAN_WIRE_BYTES:u64=104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d7c08dd9105cb7b83cb251e80c7d2dd1e9f7bb2a43f614bff6d00d808cccbcb5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.F_x_center_load {
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
for element in &self.F_x_center_load {
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
value.F_x_center_load[i]=decoded;
}}
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.load_inertia[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Franka_msgsSetLoadRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<104{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 franka_msgs/SetLoadResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Franka_msgsSetLoadResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:String,
}
impl Default for Ros1Franka_msgsSetLoadResponse {fn default()->Self {Self {
success:Default::default(),
error:Default::default(),
}}}
impl Ros1Franka_msgsSetLoadResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f18952407bf9e2813deeaa3734a781e90b74cb79892919c1218812ad71255cf3";
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
impl Canonical for Ros1Franka_msgsSetLoadResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros1Franka_msgsFrankaState {pub const ROBOT_MODE_OTHER:u8=0;
pub const ROBOT_MODE_IDLE:u8=1;
pub const ROBOT_MODE_MOVE:u8=2;
pub const ROBOT_MODE_GUIDING:u8=3;
pub const ROBOT_MODE_REFLEX:u8=4;
pub const ROBOT_MODE_USER_STOPPED:u8=5;
pub const ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY:u8=6;}
