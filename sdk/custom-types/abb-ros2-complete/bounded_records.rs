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
/// Complete bounded value for ROS2 abb_egm_msgs/msg/EGMChannelState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_egm_msgsMsgEGMChannelState {
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
impl Default for Ros2Abb_egm_msgsMsgEGMChannelState {fn default()->Self {Self {
name:Default::default(),
active:Default::default(),
egm_convergence_met:Default::default(),
egm_client_state:Default::default(),
motor_state:Default::default(),
rapid_execution_state:Default::default(),
utilization_rate:Default::default(),
}}}
impl Ros2Abb_egm_msgsMsgEGMChannelState {
  pub const MAX_WIRE_BYTES:usize=81;
  pub const CARTESIAN_WIRE_BYTES:u64=81;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fbd288b93963fccc92aa114570c0a4f6a1fdc5b473cb8e252922720f5d88b519";
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
impl Canonical for Ros2Abb_egm_msgsMsgEGMChannelState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>81{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
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
  pub const MAX_WIRE_BYTES:usize=76;
  pub const CARTESIAN_WIRE_BYTES:u64=76;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.stamp.wire_size()?)?;
if self.frame_id.len()>64||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if n>76{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.stamp.write_fields(out,at);
put(out,at,&(self.frame_id.len() as u32).to_le_bytes());put(out,at,self.frame_id.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Builtin_interfacesMsgTime::read_fields(bytes,at)?;
value.stamp=decoded;
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_msgsMsgHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>76{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_egm_msgs/msg/EGMState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_egm_msgsMsgEGMState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared egm_channels; canonical bounds are part of CODEC_IDENTITY.
  pub egm_channels:Vec<Ros2Abb_egm_msgsMsgEGMChannelState>,
}
impl Default for Ros2Abb_egm_msgsMsgEGMState {fn default()->Self {Self {
header:Default::default(),
egm_channels:Default::default(),
}}}
impl Ros2Abb_egm_msgsMsgEGMState {
  pub const MAX_WIRE_BYTES:usize=1376;
  pub const CARTESIAN_WIRE_BYTES:u64=1376;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5b10efb13c3be7290c7e391d55679a174969887edd1b8baca682edac311a8e5d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.egm_channels.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.egm_channels {
n=add_size(n,element.wire_size()?)?;
}
if n>1376{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.egm_channels=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Abb_egm_msgsMsgEGMChannelState::read_fields(bytes,at)?;
value.egm_channels.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Abb_egm_msgsMsgEGMState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1376{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/ConfData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgConfData {
  /// Declared cf1; canonical bounds are part of CODEC_IDENTITY.
  pub cf1:f32,
  /// Declared cf4; canonical bounds are part of CODEC_IDENTITY.
  pub cf4:f32,
  /// Declared cf6; canonical bounds are part of CODEC_IDENTITY.
  pub cf6:f32,
  /// Declared cfx; canonical bounds are part of CODEC_IDENTITY.
  pub cfx:f32,
}
impl Default for Ros2Abb_rapid_msgsMsgConfData {fn default()->Self {Self {
cf1:Default::default(),
cf4:Default::default(),
cf6:Default::default(),
cfx:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgConfData {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0c22cc29db62f5001ef20ea7b131de63099e31ac5f150d3e9f0bb62120d60571";
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
impl Canonical for Ros2Abb_rapid_msgsMsgConfData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/ExtJoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgExtJoint {
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
impl Default for Ros2Abb_rapid_msgsMsgExtJoint {fn default()->Self {Self {
eax_a:Default::default(),
eax_b:Default::default(),
eax_c:Default::default(),
eax_d:Default::default(),
eax_e:Default::default(),
eax_f:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgExtJoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8b799c6bec9e4eab0900a633a93375a7792e49fde0bf302f6752115a001a4ae2";
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
impl Canonical for Ros2Abb_rapid_msgsMsgExtJoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/RobJoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgRobJoint {
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
impl Default for Ros2Abb_rapid_msgsMsgRobJoint {fn default()->Self {Self {
rax_1:Default::default(),
rax_2:Default::default(),
rax_3:Default::default(),
rax_4:Default::default(),
rax_5:Default::default(),
rax_6:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgRobJoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6d842aebc0a3b222a727063a41ef7e96f2b8c14ffbba5c8d6e140ba2890e69cd";
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
impl Canonical for Ros2Abb_rapid_msgsMsgRobJoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/JointTarget.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgJointTarget {
  /// Declared robax; canonical bounds are part of CODEC_IDENTITY.
  pub robax:Ros2Abb_rapid_msgsMsgRobJoint,
  /// Declared extax; canonical bounds are part of CODEC_IDENTITY.
  pub extax:Ros2Abb_rapid_msgsMsgExtJoint,
}
impl Default for Ros2Abb_rapid_msgsMsgJointTarget {fn default()->Self {Self {
robax:Default::default(),
extax:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgJointTarget {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5949cf0121391646c1f04b54d8f7deaae0497f4920f7817739fc25bfddc4e540";
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
let decoded=Ros2Abb_rapid_msgsMsgRobJoint::read_fields(bytes,at)?;
value.robax=decoded;
let decoded=Ros2Abb_rapid_msgsMsgExtJoint::read_fields(bytes,at)?;
value.extax=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_msgsMsgJointTarget {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/Pos.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgPos {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
}
impl Default for Ros2Abb_rapid_msgsMsgPos {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgPos {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_31d490d6ead5a1a78295dc4e934f0b89e74e4732da2b57d28d297477bf4cbdd2";
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
impl Canonical for Ros2Abb_rapid_msgsMsgPos {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/Orient.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgOrient {
  /// Declared q1; canonical bounds are part of CODEC_IDENTITY.
  pub q1:f32,
  /// Declared q2; canonical bounds are part of CODEC_IDENTITY.
  pub q2:f32,
  /// Declared q3; canonical bounds are part of CODEC_IDENTITY.
  pub q3:f32,
  /// Declared q4; canonical bounds are part of CODEC_IDENTITY.
  pub q4:f32,
}
impl Default for Ros2Abb_rapid_msgsMsgOrient {fn default()->Self {Self {
q1:Default::default(),
q2:Default::default(),
q3:Default::default(),
q4:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgOrient {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_821d887b88298422f9466aaf3926db580f878e964055fbd72226b7f76f2f536e";
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
impl Canonical for Ros2Abb_rapid_msgsMsgOrient {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/LoadData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgLoadData {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cog; canonical bounds are part of CODEC_IDENTITY.
  pub cog:Ros2Abb_rapid_msgsMsgPos,
  /// Declared aom; canonical bounds are part of CODEC_IDENTITY.
  pub aom:Ros2Abb_rapid_msgsMsgOrient,
  /// Declared ix; canonical bounds are part of CODEC_IDENTITY.
  pub ix:f32,
  /// Declared iy; canonical bounds are part of CODEC_IDENTITY.
  pub iy:f32,
  /// Declared iz; canonical bounds are part of CODEC_IDENTITY.
  pub iz:f32,
}
impl Default for Ros2Abb_rapid_msgsMsgLoadData {fn default()->Self {Self {
mass:Default::default(),
cog:Default::default(),
aom:Default::default(),
ix:Default::default(),
iy:Default::default(),
iz:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgLoadData {
  pub const MAX_WIRE_BYTES:usize=44;
  pub const CARTESIAN_WIRE_BYTES:u64=44;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f73e480a0e6746a4df8f4d62bc123d68d01d3dc48f0bc5f0eac51b859a0750f9";
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
let decoded=Ros2Abb_rapid_msgsMsgPos::read_fields(bytes,at)?;
value.cog=decoded;
let decoded=Ros2Abb_rapid_msgsMsgOrient::read_fields(bytes,at)?;
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
impl Canonical for Ros2Abb_rapid_msgsMsgLoadData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>44{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/Pose.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgPose {
  /// Declared trans; canonical bounds are part of CODEC_IDENTITY.
  pub trans:Ros2Abb_rapid_msgsMsgPos,
  /// Declared rot; canonical bounds are part of CODEC_IDENTITY.
  pub rot:Ros2Abb_rapid_msgsMsgOrient,
}
impl Default for Ros2Abb_rapid_msgsMsgPose {fn default()->Self {Self {
trans:Default::default(),
rot:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgPose {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8f1535fe21326706fd0b16314857162d36bca91ce8e8f6a8bec3864dafb9e2f2";
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
let decoded=Ros2Abb_rapid_msgsMsgPos::read_fields(bytes,at)?;
value.trans=decoded;
let decoded=Ros2Abb_rapid_msgsMsgOrient::read_fields(bytes,at)?;
value.rot=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_msgsMsgPose {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/RobTarget.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgRobTarget {
  /// Declared trans; canonical bounds are part of CODEC_IDENTITY.
  pub trans:Ros2Abb_rapid_msgsMsgPos,
  /// Declared rot; canonical bounds are part of CODEC_IDENTITY.
  pub rot:Ros2Abb_rapid_msgsMsgOrient,
  /// Declared robconf; canonical bounds are part of CODEC_IDENTITY.
  pub robconf:Ros2Abb_rapid_msgsMsgConfData,
  /// Declared extax; canonical bounds are part of CODEC_IDENTITY.
  pub extax:Ros2Abb_rapid_msgsMsgExtJoint,
}
impl Default for Ros2Abb_rapid_msgsMsgRobTarget {fn default()->Self {Self {
trans:Default::default(),
rot:Default::default(),
robconf:Default::default(),
extax:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgRobTarget {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8decb56e8faf0a6a954af974adfea5c500aea8392e6a778ae7394174323489d8";
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
let decoded=Ros2Abb_rapid_msgsMsgPos::read_fields(bytes,at)?;
value.trans=decoded;
let decoded=Ros2Abb_rapid_msgsMsgOrient::read_fields(bytes,at)?;
value.rot=decoded;
let decoded=Ros2Abb_rapid_msgsMsgConfData::read_fields(bytes,at)?;
value.robconf=decoded;
let decoded=Ros2Abb_rapid_msgsMsgExtJoint::read_fields(bytes,at)?;
value.extax=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_msgsMsgRobTarget {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<68{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/ToolData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgToolData {
  /// Declared robhold; canonical bounds are part of CODEC_IDENTITY.
  pub robhold:bool,
  /// Declared tframe; canonical bounds are part of CODEC_IDENTITY.
  pub tframe:Ros2Abb_rapid_msgsMsgPose,
  /// Declared tload; canonical bounds are part of CODEC_IDENTITY.
  pub tload:Ros2Abb_rapid_msgsMsgLoadData,
}
impl Default for Ros2Abb_rapid_msgsMsgToolData {fn default()->Self {Self {
robhold:Default::default(),
tframe:Default::default(),
tload:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgToolData {
  pub const MAX_WIRE_BYTES:usize=73;
  pub const CARTESIAN_WIRE_BYTES:u64=73;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_948fa0bbaaf95f0d88ca03cad687f16102261f54b0da889ae8bca82042fc5237";
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
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
value.tframe=decoded;
let decoded=Ros2Abb_rapid_msgsMsgLoadData::read_fields(bytes,at)?;
value.tload=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_msgsMsgToolData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>73{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<73{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_msgs/msg/WObjData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_msgsMsgWObjData {
  /// Declared robhold; canonical bounds are part of CODEC_IDENTITY.
  pub robhold:bool,
  /// Declared ufprog; canonical bounds are part of CODEC_IDENTITY.
  pub ufprog:bool,
  /// Declared ufmec; canonical bounds are part of CODEC_IDENTITY.
  pub ufmec:String,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:Ros2Abb_rapid_msgsMsgPose,
  /// Declared oframe; canonical bounds are part of CODEC_IDENTITY.
  pub oframe:Ros2Abb_rapid_msgsMsgPose,
}
impl Default for Ros2Abb_rapid_msgsMsgWObjData {fn default()->Self {Self {
robhold:Default::default(),
ufprog:Default::default(),
ufmec:Default::default(),
uframe:Default::default(),
oframe:Default::default(),
}}}
impl Ros2Abb_rapid_msgsMsgWObjData {
  pub const MAX_WIRE_BYTES:usize=574;
  pub const CARTESIAN_WIRE_BYTES:u64=574;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b36f0448512fab6028431d19b39a8ae31b7b81b2615ffc8311139f63b5d8e7cd";
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
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
value.uframe=decoded;
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
value.oframe=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_msgsMsgWObjData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>574{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<62{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/EGMActivateSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings {
  /// Declared tool; canonical bounds are part of CODEC_IDENTITY.
  pub tool:Ros2Abb_rapid_msgsMsgToolData,
  /// Declared wobj; canonical bounds are part of CODEC_IDENTITY.
  pub wobj:Ros2Abb_rapid_msgsMsgWObjData,
  /// Declared correction_frame; canonical bounds are part of CODEC_IDENTITY.
  pub correction_frame:Ros2Abb_rapid_msgsMsgPose,
  /// Declared sensor_frame; canonical bounds are part of CODEC_IDENTITY.
  pub sensor_frame:Ros2Abb_rapid_msgsMsgPose,
  /// Declared cond_min_max; canonical bounds are part of CODEC_IDENTITY.
  pub cond_min_max:f32,
  /// Declared lp_filter; canonical bounds are part of CODEC_IDENTITY.
  pub lp_filter:f32,
  /// Declared sample_rate; canonical bounds are part of CODEC_IDENTITY.
  pub sample_rate:u8,
  /// Declared max_speed_deviation; canonical bounds are part of CODEC_IDENTITY.
  pub max_speed_deviation:f32,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings {fn default()->Self {Self {
tool:Default::default(),
wobj:Default::default(),
correction_frame:Default::default(),
sensor_frame:Default::default(),
cond_min_max:Default::default(),
lp_filter:Default::default(),
sample_rate:Default::default(),
max_speed_deviation:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings {
  pub const MAX_WIRE_BYTES:usize=716;
  pub const CARTESIAN_WIRE_BYTES:u64=716;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fce73aea80d9b8a299b8e2ab742602871146b3ebbd624a2e807e25c1ac107cfa";
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
let decoded=Ros2Abb_rapid_msgsMsgToolData::read_fields(bytes,at)?;
value.tool=decoded;
let decoded=Ros2Abb_rapid_msgsMsgWObjData::read_fields(bytes,at)?;
value.wobj=decoded;
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
value.correction_frame=decoded;
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>716{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<204{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/EGMRunSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings {
  /// Declared cond_time; canonical bounds are part of CODEC_IDENTITY.
  pub cond_time:f32,
  /// Declared ramp_in_time; canonical bounds are part of CODEC_IDENTITY.
  pub ramp_in_time:f32,
  /// Declared offset; canonical bounds are part of CODEC_IDENTITY.
  pub offset:Ros2Abb_rapid_msgsMsgPose,
  /// Declared pos_corr_gain; canonical bounds are part of CODEC_IDENTITY.
  pub pos_corr_gain:f32,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings {fn default()->Self {Self {
cond_time:Default::default(),
ramp_in_time:Default::default(),
offset:Default::default(),
pos_corr_gain:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings {
  pub const MAX_WIRE_BYTES:usize=40;
  pub const CARTESIAN_WIRE_BYTES:u64=40;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_92a2a654eb2eda89077933f955f04ed87884827b8875cf563570eed5d211b0e2";
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
let decoded=Ros2Abb_rapid_msgsMsgPose::read_fields(bytes,at)?;
value.offset=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pos_corr_gain=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>40{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/EGMSetupSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings {
  /// Declared use_filtering; canonical bounds are part of CODEC_IDENTITY.
  pub use_filtering:bool,
  /// Declared comm_timeout; canonical bounds are part of CODEC_IDENTITY.
  pub comm_timeout:f32,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings {fn default()->Self {Self {
use_filtering:Default::default(),
comm_timeout:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2565bed5b83b132c69f37d7294c69181800ad2974ddc54f60868da57e28e782c";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/EGMStopSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings {
  /// Declared ramp_out_time; canonical bounds are part of CODEC_IDENTITY.
  pub ramp_out_time:f32,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings {fn default()->Self {Self {
ramp_out_time:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0f3f2f6034567a7d49dd140ff9ff620e8567548139597b9b823e63a947673cee";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/EGMSettings.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings {
  /// Declared allow_egm_motions; canonical bounds are part of CODEC_IDENTITY.
  pub allow_egm_motions:bool,
  /// Declared use_presync; canonical bounds are part of CODEC_IDENTITY.
  pub use_presync:bool,
  /// Declared setup_uc; canonical bounds are part of CODEC_IDENTITY.
  pub setup_uc:Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings,
  /// Declared activate; canonical bounds are part of CODEC_IDENTITY.
  pub activate:Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings,
  /// Declared run; canonical bounds are part of CODEC_IDENTITY.
  pub run:Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings,
  /// Declared stop; canonical bounds are part of CODEC_IDENTITY.
  pub stop:Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings {fn default()->Self {Self {
allow_egm_motions:Default::default(),
use_presync:Default::default(),
setup_uc:Default::default(),
activate:Default::default(),
run:Default::default(),
stop:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings {
  pub const MAX_WIRE_BYTES:usize=767;
  pub const CARTESIAN_WIRE_BYTES:u64=767;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_622d3a975eecb11f7729aecfadc161cada271208d6fe7599f455750b90144ef3";
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
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings::read_fields(bytes,at)?;
value.setup_uc=decoded;
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings::read_fields(bytes,at)?;
value.activate=decoded;
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings::read_fields(bytes,at)?;
value.run=decoded;
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings::read_fields(bytes,at)?;
value.stop=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>767{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<255{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/StateMachineState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {
  /// Declared rapid_task; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_task:String,
  /// Declared sm_state; canonical bounds are part of CODEC_IDENTITY.
  pub sm_state:u8,
  /// Declared egm_action; canonical bounds are part of CODEC_IDENTITY.
  pub egm_action:u8,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {fn default()->Self {Self {
rapid_task:Default::default(),
sm_state:Default::default(),
egm_action:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ff2a09e4b6e8ef26cbdc3ecdffdceb33d6b2c82f0d51ed2c38ce9648c6f47711";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/msg/RuntimeState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared state_machines; canonical bounds are part of CODEC_IDENTITY.
  pub state_machines:Vec<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState {fn default()->Self {Self {
header:Default::default(),
state_machines:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState {
  pub const MAX_WIRE_BYTES:usize=8368;
  pub const CARTESIAN_WIRE_BYTES:u64=8368;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3c0f157544fa82c4e206a4fb42f8ba7f67213d19533f79b77c66dec6fd0460d9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.state_machines.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.state_machines {
n=add_size(n,element.wire_size()?)?;
}
if n>8368{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.state_machines=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState::read_fields(bytes,at)?;
value.state_machines.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8368{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {fn default()->Self {Self {
task:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_363be2459fcc9c475c6305cbcfebf7689e891eb27645b7667960aace23d56e8e";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {
  /// Declared settings; canonical bounds are part of CODEC_IDENTITY.
  pub settings:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {fn default()->Self {Self {
settings:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {
  pub const MAX_WIRE_BYTES:usize=1285;
  pub const CARTESIAN_WIRE_BYTES:u64=1285;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d8f340e1c6d78fc0a782c3279efb34ae5e11df175a82f9dd5ee82e603d6b2ffb";
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
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings::read_fields(bytes,at)?;
value.settings=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1285{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<261{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared settings; canonical bounds are part of CODEC_IDENTITY.
  pub settings:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {fn default()->Self {Self {
task:Default::default(),
settings:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {
  pub const MAX_WIRE_BYTES:usize=835;
  pub const CARTESIAN_WIRE_BYTES:u64=835;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c2f3dd535e9475526bf18bdffa0e9a839049d315c9aebb150f8561ee3361a2bd";
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
let decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings::read_fields(bytes,at)?;
value.settings=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>835{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<259{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6a1adeebe31dfe101a6d57019e26ee2f1144b9a6ce30aee08d4c6fe56ea6838d";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared routine; canonical bounds are part of CODEC_IDENTITY.
  pub routine:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {fn default()->Self {Self {
task:Default::default(),
routine:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {
  pub const MAX_WIRE_BYTES:usize=584;
  pub const CARTESIAN_WIRE_BYTES:u64=584;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4561b7e5e296a1a329d5f5684bcbde1ef41324554a34e9a79a85824286dd477c";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>584{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_980d2a16536289f446161acdf2fc57d459baa2290d74f1dd5aaf8129126cbba1";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared command; canonical bounds are part of CODEC_IDENTITY.
  pub command:u8,
  /// Declared target_position; canonical bounds are part of CODEC_IDENTITY.
  pub target_position:f32,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {fn default()->Self {Self {
task:Default::default(),
command:Default::default(),
target_position:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {
  pub const MAX_WIRE_BYTES:usize=73;
  pub const CARTESIAN_WIRE_BYTES:u64=73;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d6983aa85c2c52f8ec06cbd723d5929cfc381db2e0bfffb4c27078497d86c90";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>73{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_87cdc41c81d49930dce7193d4b7af98add266a6e9a7cf5e69457d01ded388cf8";
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
impl Canonical for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/msg/MechanicalUnitState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsMsgMechanicalUnitState {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared activated; canonical bounds are part of CODEC_IDENTITY.
  pub activated:bool,
}
impl Default for Ros2Abb_robot_msgsMsgMechanicalUnitState {fn default()->Self {Self {
name:Default::default(),
activated:Default::default(),
}}}
impl Ros2Abb_robot_msgsMsgMechanicalUnitState {
  pub const MAX_WIRE_BYTES:usize=69;
  pub const CARTESIAN_WIRE_BYTES:u64=69;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1dc7a7dcbaa19e78cd6419f8844287a7fbd9d7a839ab1a8ad97ef0be34f3a3a8";
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
impl Canonical for Ros2Abb_robot_msgsMsgMechanicalUnitState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>69{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/msg/RAPIDSymbolPath.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsMsgRAPIDSymbolPath {
  /// Declared task; canonical bounds are part of CODEC_IDENTITY.
  pub task:String,
  /// Declared module; canonical bounds are part of CODEC_IDENTITY.
  pub module:String,
  /// Declared symbol; canonical bounds are part of CODEC_IDENTITY.
  pub symbol:String,
}
impl Default for Ros2Abb_robot_msgsMsgRAPIDSymbolPath {fn default()->Self {Self {
task:Default::default(),
module:Default::default(),
symbol:Default::default(),
}}}
impl Ros2Abb_robot_msgsMsgRAPIDSymbolPath {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_249523197567fbf638f30487a7f5b8d8ff5ea746fcd4a65a35f2dbb18a3adf0e";
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
impl Canonical for Ros2Abb_robot_msgsMsgRAPIDSymbolPath {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/msg/RAPIDTaskState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsMsgRAPIDTaskState {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared activated; canonical bounds are part of CODEC_IDENTITY.
  pub activated:bool,
  /// Declared execution_state; canonical bounds are part of CODEC_IDENTITY.
  pub execution_state:u8,
  /// Declared motion_task; canonical bounds are part of CODEC_IDENTITY.
  pub motion_task:bool,
}
impl Default for Ros2Abb_robot_msgsMsgRAPIDTaskState {fn default()->Self {Self {
name:Default::default(),
activated:Default::default(),
execution_state:Default::default(),
motion_task:Default::default(),
}}}
impl Ros2Abb_robot_msgsMsgRAPIDTaskState {
  pub const MAX_WIRE_BYTES:usize=71;
  pub const CARTESIAN_WIRE_BYTES:u64=71;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ff698e375b0ff33026cfd248c1ac734f939763aba25a6c4f7cac628d445422fd";
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
impl Canonical for Ros2Abb_robot_msgsMsgRAPIDTaskState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>71{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/msg/ServiceResponses.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsMsgServiceResponses {
}
impl Default for Ros2Abb_robot_msgsMsgServiceResponses {fn default()->Self {Self {
}}}
impl Ros2Abb_robot_msgsMsgServiceResponses {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4e01b972b67454e851801f28aee09c0774b67907d2241cc67cae7d045c18f646";
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
impl Canonical for Ros2Abb_robot_msgsMsgServiceResponses {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/msg/SystemState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsMsgSystemState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared motors_on; canonical bounds are part of CODEC_IDENTITY.
  pub motors_on:bool,
  /// Declared auto_mode; canonical bounds are part of CODEC_IDENTITY.
  pub auto_mode:bool,
  /// Declared rapid_running; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_running:bool,
  /// Declared rapid_tasks; canonical bounds are part of CODEC_IDENTITY.
  pub rapid_tasks:Vec<Ros2Abb_robot_msgsMsgRAPIDTaskState>,
  /// Declared mechanical_units; canonical bounds are part of CODEC_IDENTITY.
  pub mechanical_units:Vec<Ros2Abb_robot_msgsMsgMechanicalUnitState>,
}
impl Default for Ros2Abb_robot_msgsMsgSystemState {fn default()->Self {Self {
header:Default::default(),
motors_on:Default::default(),
auto_mode:Default::default(),
rapid_running:Default::default(),
rapid_tasks:Default::default(),
mechanical_units:Default::default(),
}}}
impl Ros2Abb_robot_msgsMsgSystemState {
  pub const MAX_WIRE_BYTES:usize=2327;
  pub const CARTESIAN_WIRE_BYTES:u64=2327;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c1cdbf0e578fddc5bf70cc2cc181d786f3e8189afc6a67962de6eb273973a38f";
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
if n>2327{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDTaskState::read_fields(bytes,at)?;
value.rapid_tasks.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.mechanical_units=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Abb_robot_msgsMsgMechanicalUnitState::read_fields(bytes,at)?;
value.mechanical_units.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsMsgSystemState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2327{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<23{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetFileContents_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetFileContents_Request {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetFileContents_Request {fn default()->Self {Self {
filename:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetFileContents_Request {
  pub const MAX_WIRE_BYTES:usize=516;
  pub const CARTESIAN_WIRE_BYTES:u64=516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bdcf73a965bd05356e48019b74d633176fdeda62452bc7185d6b6ff30576016b";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetFileContents_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetFileContents_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetFileContents_Response {
  /// Declared contents; canonical bounds are part of CODEC_IDENTITY.
  pub contents:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetFileContents_Response {fn default()->Self {Self {
contents:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetFileContents_Response {
  pub const MAX_WIRE_BYTES:usize=4618;
  pub const CARTESIAN_WIRE_BYTES:u64=4618;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f05617145a40043e87cbc251f833274d2a962bd7cdaabc5f9a8e4591bd56e107";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetFileContents_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4618{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetIOSignal_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetIOSignal_Request {
  /// Declared signal; canonical bounds are part of CODEC_IDENTITY.
  pub signal:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetIOSignal_Request {fn default()->Self {Self {
signal:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetIOSignal_Request {
  pub const MAX_WIRE_BYTES:usize=516;
  pub const CARTESIAN_WIRE_BYTES:u64=516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2406d53c2fce44e72ad7e6f1c9e75ea19c4df694ff9f00917229ec758209b1f5";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetIOSignal_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetIOSignal_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetIOSignal_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetIOSignal_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetIOSignal_Response {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1f668ef78c353f2ec8defdac619d349cda731576f2429cf07107b9b56ba9030e";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetIOSignal_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDBool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c381080d235c78216a8e64275819ff3fb97e1b1de96e7da758c4889630b8e0ff";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDBool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {
  pub const MAX_WIRE_BYTES:usize=519;
  pub const CARTESIAN_WIRE_BYTES:u64=519;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b853e212489fb815866fa29b165eea474a977483158b6203699c4a1293aa6acd";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>519{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71057c2fe44bc96505ec6e05d998fcb6323b281fff40808ce990207c48626979";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f64,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {
  pub const MAX_WIRE_BYTES:usize=526;
  pub const CARTESIAN_WIRE_BYTES:u64=526;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_af0420c126368a7e45b728c7fef76062b5ef2f0adea7535ad38e4771244edd56";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>526{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<14{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDNum_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_03d5da34af8d90ed8a20f8ca38fa75d9e777496828978f0f348e3c1c8f67d0a5";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDNum_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {
  pub const MAX_WIRE_BYTES:usize=522;
  pub const CARTESIAN_WIRE_BYTES:u64=522;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a0da03014dd001e58efc4a5b274cd5c62016e7fa257af1120a8142a79635220d";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>522{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDString_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDString_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDString_Request {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDString_Request {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c7bc9c788301e619a165c18a1cdb0aca80df8def5be31da5dbf34c0038e73596";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDString_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDString_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDString_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDString_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDString_Response {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cf7bfdb2af5b91eb58b19b909f7fa3357dc1dad02716f8fa4465b53f3f4a1458";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDString_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {fn default()->Self {Self {
path:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {
  pub const MAX_WIRE_BYTES:usize=204;
  pub const CARTESIAN_WIRE_BYTES:u64=204;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e216a54cdae70f8147489928ff05a4b172ba4fc47e9ed5df4828b070354ac469";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>204{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {fn default()->Self {Self {
value:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {
  pub const MAX_WIRE_BYTES:usize=1034;
  pub const CARTESIAN_WIRE_BYTES:u64=1034;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b1f6de7ad0640f5645678d327804dc7e732bdad38805e642b13ed7294c21d602";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1034{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {
}
impl Default for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {fn default()->Self {Self {
}}}
impl Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_caf70e11b386a933f62d06a433ca1cedaa3c8ecc75b7e8de8c1d5ae00a29f03e";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {
  /// Declared description; canonical bounds are part of CODEC_IDENTITY.
  pub description:String,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {fn default()->Self {Self {
description:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {
  pub const MAX_WIRE_BYTES:usize=4618;
  pub const CARTESIAN_WIRE_BYTES:u64=4618;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1af2591f035379597f376ed1e2343f73a89f9fef5c29269aa420243a82e6c368";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4618{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetSpeedRatio_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {
}
impl Default for Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {fn default()->Self {Self {
}}}
impl Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_54ee2c897e2bf2f39c96cc42246ffe5997c8df280280433e8748b3bcf24c299c";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/GetSpeedRatio_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {
  /// Declared speed_ratio; canonical bounds are part of CODEC_IDENTITY.
  pub speed_ratio:u8,
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {fn default()->Self {Self {
speed_ratio:Default::default(),
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {
  pub const MAX_WIRE_BYTES:usize=519;
  pub const CARTESIAN_WIRE_BYTES:u64=519;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_baa271a258c7ca274e0145fea1abb7cbc43e98c82b8e40d67c4743237a19910b";
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
impl Canonical for Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>519{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetFileContents_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetFileContents_Request {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
  /// Declared contents; canonical bounds are part of CODEC_IDENTITY.
  pub contents:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetFileContents_Request {fn default()->Self {Self {
filename:Default::default(),
contents:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetFileContents_Request {
  pub const MAX_WIRE_BYTES:usize=4616;
  pub const CARTESIAN_WIRE_BYTES:u64=4616;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3bfa8ad1846fc31acae47a875d3555a78a64b777923eb9b62e87a836f041b26b";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetFileContents_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4616{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetFileContents_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetFileContents_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetFileContents_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetFileContents_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1586344401d43055460b49f2487567638f66838cfe5cac99a47e0a39c0b06ede";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetFileContents_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetIOSignal_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetIOSignal_Request {
  /// Declared signal; canonical bounds are part of CODEC_IDENTITY.
  pub signal:String,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetIOSignal_Request {fn default()->Self {Self {
signal:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetIOSignal_Request {
  pub const MAX_WIRE_BYTES:usize=1032;
  pub const CARTESIAN_WIRE_BYTES:u64=1032;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2d206d0020179263e83e745a428616ed53d809b0c65a79ad495cb882ae9e49b7";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetIOSignal_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1032{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetIOSignal_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetIOSignal_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetIOSignal_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetIOSignal_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1fa28cc3160d51f71d0740cbe145663ec360d6fa488c2a6bf14ad6ab92004a75";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetIOSignal_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDBool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {
  pub const MAX_WIRE_BYTES:usize=205;
  pub const CARTESIAN_WIRE_BYTES:u64=205;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4e707e429acc860d032d216bcd2c1eb1a0f731fad7e7c0bef7beed810c6e4a52";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>205{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDBool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_80d2b31c93771cfa8a19ff82a3d9d63951bf9d28c039d2725017327fa3b0114a";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f64,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {
  pub const MAX_WIRE_BYTES:usize=212;
  pub const CARTESIAN_WIRE_BYTES:u64=212;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cece086311a908f12c0b0ab3988ca829e891749c74687c717d2a52bfb8ceaf76";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>212{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5f77fe9850f109d873a2784b9690c8aa0632b9e91d158ed4d14daf1aaf90ea6";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDNum_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {
  pub const MAX_WIRE_BYTES:usize=208;
  pub const CARTESIAN_WIRE_BYTES:u64=208;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a5bee9a64be69dc4246f68837a44362f52795ae3c6420a2df47685606283ea02";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>208{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDNum_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_744f3c5fac58ff098c96cedcea94fe6fa87c88ceb89674449f50fbdcd1ca30bb";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDString_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDString_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDString_Request {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDString_Request {
  pub const MAX_WIRE_BYTES:usize=720;
  pub const CARTESIAN_WIRE_BYTES:u64=720;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f21c45f3f8cd4c0b50781bdd487fdc6ec0c0b35adbc7e36e9ca5b7cc47c3d461";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDString_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>720{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDString_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDString_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDString_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDString_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1496ef4f4ffe48fe74b9e25b925c19f10f0e171a9d33fa2e56c15aa5cfbe4774";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDString_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {
  /// Declared path; canonical bounds are part of CODEC_IDENTITY.
  pub path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {fn default()->Self {Self {
path:Default::default(),
value:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {
  pub const MAX_WIRE_BYTES:usize=720;
  pub const CARTESIAN_WIRE_BYTES:u64=720;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_928d427801b79775595fed6e3779f9ba9e7e8e27f2de66cf79610cb51bb72bcf";
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
let decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath::read_fields(bytes,at)?;
value.path=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>720{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e156616a51bf4691a1c3853d62a8b0c43bb17d1a330cd4e7a58eceb5fe442382";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetSpeedRatio_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {
  /// Declared speed_ratio; canonical bounds are part of CODEC_IDENTITY.
  pub speed_ratio:u8,
}
impl Default for Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {fn default()->Self {Self {
speed_ratio:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_da30970377ba1bc151b544560773f02e17f4eaed56eb4c8d791de72bdef04355";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/SetSpeedRatio_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_25e6d7edaa0503a4d45d893e886db4215bfd0af8711307c141bc63552609e3a5";
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
impl Canonical for Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {
}
impl Default for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {fn default()->Self {Self {
}}}
impl Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fd1d49c3b4e489464c6662f306210137f71d15192f64cd8a8abf93acd5fbff8d";
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
impl Canonical for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u16,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e38422b2a008f07e108505ac3ce65c24189a1d6657e88a6289e17d7d4ae38d51";
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
impl Canonical for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 builtin_interfaces/msg/Duration.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Builtin_interfacesMsgDuration {
  /// Declared sec; canonical bounds are part of CODEC_IDENTITY.
  pub sec:i32,
  /// Declared nanosec; canonical bounds are part of CODEC_IDENTITY.
  pub nanosec:u32,
}
impl Default for Ros2Builtin_interfacesMsgDuration {fn default()->Self {Self {
sec:Default::default(),
nanosec:Default::default(),
}}}
impl Ros2Builtin_interfacesMsgDuration {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
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
impl Canonical for Ros2Builtin_interfacesMsgDuration {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 trajectory_msgs/msg/JointTrajectoryPoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Trajectory_msgsMsgJointTrajectoryPoint {
  /// Declared positions; canonical bounds are part of CODEC_IDENTITY.
  pub positions:Vec<f64>,
  /// Declared velocities; canonical bounds are part of CODEC_IDENTITY.
  pub velocities:Vec<f64>,
  /// Declared accelerations; canonical bounds are part of CODEC_IDENTITY.
  pub accelerations:Vec<f64>,
  /// Declared effort; canonical bounds are part of CODEC_IDENTITY.
  pub effort:Vec<f64>,
  /// Declared time_from_start; canonical bounds are part of CODEC_IDENTITY.
  pub time_from_start:Ros2Builtin_interfacesMsgDuration,
}
impl Default for Ros2Trajectory_msgsMsgJointTrajectoryPoint {fn default()->Self {Self {
positions:Default::default(),
velocities:Default::default(),
accelerations:Default::default(),
effort:Default::default(),
time_from_start:Default::default(),
}}}
impl Ros2Trajectory_msgsMsgJointTrajectoryPoint {
  pub const MAX_WIRE_BYTES:usize=408;
  pub const CARTESIAN_WIRE_BYTES:u64=408;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5";
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
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.time_from_start=decoded;
Ok(value)}
}
impl Canonical for Ros2Trajectory_msgsMsgJointTrajectoryPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>408{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 trajectory_msgs/msg/JointTrajectory.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Trajectory_msgsMsgJointTrajectory {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared points; canonical bounds are part of CODEC_IDENTITY.
  pub points:Vec<Ros2Trajectory_msgsMsgJointTrajectoryPoint>,
}
impl Default for Ros2Trajectory_msgsMsgJointTrajectory {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
points:Default::default(),
}}}
impl Ros2Trajectory_msgsMsgJointTrajectory {
  pub const MAX_WIRE_BYTES:usize=4164;
  pub const CARTESIAN_WIRE_BYTES:u64=4164;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0";
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
if n>4164{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
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
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.points.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Trajectory_msgsMsgJointTrajectory {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4164{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 geometry_msgs/msg/Transform.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgTransform {
  /// Declared translation; canonical bounds are part of CODEC_IDENTITY.
  pub translation:Ros2Geometry_msgsMsgVector3,
  /// Declared rotation; canonical bounds are part of CODEC_IDENTITY.
  pub rotation:Ros2Geometry_msgsMsgQuaternion,
}
impl Default for Ros2Geometry_msgsMsgTransform {fn default()->Self {Self {
translation:Default::default(),
rotation:Default::default(),
}}}
impl Ros2Geometry_msgsMsgTransform {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.translation.wire_size()?)?;
n=add_size(n,self.rotation.wire_size()?)?;
if n>56{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.translation.write_fields(out,at);
self.rotation.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.translation=decoded;
let decoded=Ros2Geometry_msgsMsgQuaternion::read_fields(bytes,at)?;
value.rotation=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTransform {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 trajectory_msgs/msg/MultiDOFJointTrajectoryPoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {
  /// Declared transforms; canonical bounds are part of CODEC_IDENTITY.
  pub transforms:Vec<Ros2Geometry_msgsMsgTransform>,
  /// Declared velocities; canonical bounds are part of CODEC_IDENTITY.
  pub velocities:Vec<Ros2Geometry_msgsMsgTwist>,
  /// Declared accelerations; canonical bounds are part of CODEC_IDENTITY.
  pub accelerations:Vec<Ros2Geometry_msgsMsgTwist>,
  /// Declared time_from_start; canonical bounds are part of CODEC_IDENTITY.
  pub time_from_start:Ros2Builtin_interfacesMsgDuration,
}
impl Default for Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {fn default()->Self {Self {
transforms:Default::default(),
velocities:Default::default(),
accelerations:Default::default(),
time_from_start:Default::default(),
}}}
impl Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {
  pub const MAX_WIRE_BYTES:usize=324;
  pub const CARTESIAN_WIRE_BYTES:u64=324;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.transforms.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.transforms {
n=add_size(n,element.wire_size()?)?;
}
if self.velocities.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocities {
n=add_size(n,element.wire_size()?)?;
}
if self.accelerations.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accelerations {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.time_from_start.wire_size()?)?;
if n>324{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.transforms.len() as u32).to_le_bytes());
for element in &self.transforms {
element.write_fields(out,at);
}
put(out,at,&(self.velocities.len() as u32).to_le_bytes());
for element in &self.velocities {
element.write_fields(out,at);
}
put(out,at,&(self.accelerations.len() as u32).to_le_bytes());
for element in &self.accelerations {
element.write_fields(out,at);
}
self.time_from_start.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.transforms=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTransform::read_fields(bytes,at)?;
value.transforms.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.velocities.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.accelerations=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.accelerations.push(decoded);
}}
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.time_from_start=decoded;
Ok(value)}
}
impl Canonical for Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>324{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 trajectory_msgs/msg/MultiDOFJointTrajectory.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared points; canonical bounds are part of CODEC_IDENTITY.
  pub points:Vec<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>,
}
impl Default for Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
points:Default::default(),
}}}
impl Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {
  pub const MAX_WIRE_BYTES:usize=1516;
  pub const CARTESIAN_WIRE_BYTES:u64=1516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.points.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.points {
n=add_size(n,element.wire_size()?)?;
}
if n>1516{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.points=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint::read_fields(bytes,at)?;
value.points.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 control_msgs/msg/JointTolerance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Control_msgsMsgJointTolerance {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:f64,
  /// Declared acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub acceleration:f64,
}
impl Default for Ros2Control_msgsMsgJointTolerance {fn default()->Self {Self {
name:Default::default(),
position:Default::default(),
velocity:Default::default(),
acceleration:Default::default(),
}}}
impl Ros2Control_msgsMsgJointTolerance {
  pub const MAX_WIRE_BYTES:usize=92;
  pub const CARTESIAN_WIRE_BYTES:u64=92;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2";
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
impl Canonical for Ros2Control_msgsMsgJointTolerance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>92{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 control_msgs/msg/JointComponentTolerance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Control_msgsMsgJointComponentTolerance {
  /// Declared joint_name; canonical bounds are part of CODEC_IDENTITY.
  pub joint_name:String,
  /// Declared component; canonical bounds are part of CODEC_IDENTITY.
  pub component:u16,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:f64,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:f64,
  /// Declared acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub acceleration:f64,
}
impl Default for Ros2Control_msgsMsgJointComponentTolerance {fn default()->Self {Self {
joint_name:Default::default(),
component:Default::default(),
position:Default::default(),
velocity:Default::default(),
acceleration:Default::default(),
}}}
impl Ros2Control_msgsMsgJointComponentTolerance {
  pub const MAX_WIRE_BYTES:usize=542;
  pub const CARTESIAN_WIRE_BYTES:u64=542;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.joint_name.len()>512||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
n=add_size(n,2)?;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.acceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>542{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.joint_name.len() as u32).to_le_bytes());put(out,at,self.joint_name.as_bytes());
put(out,at,&self.component.to_le_bytes());
put(out,at,&self.position.to_le_bytes());
put(out,at,&self.velocity.to_le_bytes());
put(out,at,&self.acceleration.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_name=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.component=decoded;
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
impl Canonical for Ros2Control_msgsMsgJointComponentTolerance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>542{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 control_msgs/action/FollowJointTrajectory_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Control_msgsActionFollowJointTrajectory_Goal {
  /// Declared trajectory; canonical bounds are part of CODEC_IDENTITY.
  pub trajectory:Ros2Trajectory_msgsMsgJointTrajectory,
  /// Declared multi_dof_trajectory; canonical bounds are part of CODEC_IDENTITY.
  pub multi_dof_trajectory:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory,
  /// Declared path_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub path_tolerance:Vec<Ros2Control_msgsMsgJointTolerance>,
  /// Declared component_path_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub component_path_tolerance:Vec<Ros2Control_msgsMsgJointComponentTolerance>,
  /// Declared goal_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_tolerance:Vec<Ros2Control_msgsMsgJointTolerance>,
  /// Declared component_goal_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub component_goal_tolerance:Vec<Ros2Control_msgsMsgJointComponentTolerance>,
  /// Declared goal_time_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_time_tolerance:Ros2Builtin_interfacesMsgDuration,
}
impl Default for Ros2Control_msgsActionFollowJointTrajectory_Goal {fn default()->Self {Self {
trajectory:Default::default(),
multi_dof_trajectory:Default::default(),
path_tolerance:Default::default(),
component_path_tolerance:Default::default(),
goal_tolerance:Default::default(),
component_goal_tolerance:Default::default(),
goal_time_tolerance:Default::default(),
}}}
impl Ros2Control_msgsActionFollowJointTrajectory_Goal {
  pub const MAX_WIRE_BYTES:usize=20920;
  pub const CARTESIAN_WIRE_BYTES:u64=20920;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trajectory.wire_size()?)?;
n=add_size(n,self.multi_dof_trajectory.wire_size()?)?;
if self.path_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.component_path_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.component_path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.goal_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.component_goal_tolerance.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.component_goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.goal_time_tolerance.wire_size()?)?;
if n>20920{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trajectory.write_fields(out,at);
self.multi_dof_trajectory.write_fields(out,at);
put(out,at,&(self.path_tolerance.len() as u32).to_le_bytes());
for element in &self.path_tolerance {
element.write_fields(out,at);
}
put(out,at,&(self.component_path_tolerance.len() as u32).to_le_bytes());
for element in &self.component_path_tolerance {
element.write_fields(out,at);
}
put(out,at,&(self.goal_tolerance.len() as u32).to_le_bytes());
for element in &self.goal_tolerance {
element.write_fields(out,at);
}
put(out,at,&(self.component_goal_tolerance.len() as u32).to_le_bytes());
for element in &self.component_goal_tolerance {
element.write_fields(out,at);
}
self.goal_time_tolerance.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Trajectory_msgsMsgJointTrajectory::read_fields(bytes,at)?;
value.trajectory=decoded;
let decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectory::read_fields(bytes,at)?;
value.multi_dof_trajectory=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.component_path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointComponentTolerance::read_fields(bytes,at)?;
value.component_path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.goal_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.goal_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.component_goal_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointComponentTolerance::read_fields(bytes,at)?;
value.component_goal_tolerance.push(decoded);
}}
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.goal_time_tolerance=decoded;
Ok(value)}
}
impl Canonical for Ros2Control_msgsActionFollowJointTrajectory_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20920{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<64{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 control_msgs/action/FollowJointTrajectory_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Control_msgsActionFollowJointTrajectory_Result {
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:i32,
  /// Declared error_string; canonical bounds are part of CODEC_IDENTITY.
  pub error_string:String,
}
impl Default for Ros2Control_msgsActionFollowJointTrajectory_Result {fn default()->Self {Self {
error_code:Default::default(),
error_string:Default::default(),
}}}
impl Ros2Control_msgsActionFollowJointTrajectory_Result {
  pub const MAX_WIRE_BYTES:usize=4104;
  pub const CARTESIAN_WIRE_BYTES:u64=4104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70";
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
impl Canonical for Ros2Control_msgsActionFollowJointTrajectory_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 control_msgs/action/FollowJointTrajectory_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Control_msgsActionFollowJointTrajectory_Feedback {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared desired; canonical bounds are part of CODEC_IDENTITY.
  pub desired:Ros2Trajectory_msgsMsgJointTrajectoryPoint,
  /// Declared actual; canonical bounds are part of CODEC_IDENTITY.
  pub actual:Ros2Trajectory_msgsMsgJointTrajectoryPoint,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:Ros2Trajectory_msgsMsgJointTrajectoryPoint,
  /// Declared multi_dof_joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub multi_dof_joint_names:Vec<String>,
  /// Declared multi_dof_desired; canonical bounds are part of CODEC_IDENTITY.
  pub multi_dof_desired:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint,
  /// Declared multi_dof_actual; canonical bounds are part of CODEC_IDENTITY.
  pub multi_dof_actual:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint,
  /// Declared multi_dof_error; canonical bounds are part of CODEC_IDENTITY.
  pub multi_dof_error:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint,
}
impl Default for Ros2Control_msgsActionFollowJointTrajectory_Feedback {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
desired:Default::default(),
actual:Default::default(),
error:Default::default(),
multi_dof_joint_names:Default::default(),
multi_dof_desired:Default::default(),
multi_dof_actual:Default::default(),
multi_dof_error:Default::default(),
}}}
impl Ros2Control_msgsActionFollowJointTrajectory_Feedback {
  pub const MAX_WIRE_BYTES:usize=4128;
  pub const CARTESIAN_WIRE_BYTES:u64=4128;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb";
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
if self.multi_dof_joint_names.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.multi_dof_joint_names {
if element.len()>512||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.multi_dof_desired.wire_size()?)?;
n=add_size(n,self.multi_dof_actual.wire_size()?)?;
n=add_size(n,self.multi_dof_error.wire_size()?)?;
if n>4128{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
put(out,at,&(self.multi_dof_joint_names.len() as u32).to_le_bytes());
for element in &self.multi_dof_joint_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
self.multi_dof_desired.write_fields(out,at);
self.multi_dof_actual.write_fields(out,at);
self.multi_dof_error.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.desired=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.actual=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.error=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.multi_dof_joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.multi_dof_joint_names.push(decoded);
}}
let decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint::read_fields(bytes,at)?;
value.multi_dof_desired=decoded;
let decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint::read_fields(bytes,at)?;
value.multi_dof_actual=decoded;
let decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint::read_fields(bytes,at)?;
value.multi_dof_error=decoded;
Ok(value)}
}
impl Canonical for Ros2Control_msgsActionFollowJointTrajectory_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4128{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<152{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=1196;
  pub const CARTESIAN_WIRE_BYTES:u64=1196;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884";
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
if n>1196{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
impl Canonical for Ros2Sensor_msgsMsgJointState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1196{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 std_srvs/srv/Trigger_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Std_srvsSrvTrigger_Request {
}
impl Default for Ros2Std_srvsSrvTrigger_Request {fn default()->Self {Self {
}}}
impl Ros2Std_srvsSrvTrigger_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77";
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
impl Canonical for Ros2Std_srvsSrvTrigger_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 std_srvs/srv/Trigger_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Std_srvsSrvTrigger_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Std_srvsSrvTrigger_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Std_srvsSrvTrigger_Response {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_srvsSrvTrigger_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Abb_egm_msgsMsgEGMChannelState {pub const EGM_UNDEFINED:u8=1;
pub const EGM_ERROR:u8=2;
pub const EGM_STOPPED:u8=3;
pub const EGM_RUNNING:u8=4;
pub const MOTORS_UNDEFINED:u8=1;
pub const MOTORS_ON:u8=2;
pub const MOTORS_OFF:u8=3;
pub const RAPID_UNDEFINED:u8=1;
pub const RAPID_STOPPED:u8=2;
pub const RAPID_RUNNING:u8=3;}

impl Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {pub const SM_STATE_UNKNOWN:u8=1;
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

impl Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {pub const SG_COMMAND_UNKNOWN:u8=1;
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

impl Ros2Abb_robot_msgsMsgRAPIDTaskState {pub const EXECUTION_STATE_UNKNOWN:u8=1;
pub const EXECUTION_STATE_READY:u8=2;
pub const EXECUTION_STATE_STOPPED:u8=3;
pub const EXECUTION_STATE_STARTED:u8=4;
pub const EXECUTION_STATE_UNINITIALIZED:u8=5;}

impl Ros2Abb_robot_msgsMsgServiceResponses {pub const SUCCESS:&'static str="";
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

impl Ros2Control_msgsMsgJointComponentTolerance {pub const X_AXIS:u16=1;
pub const Y_AXIS:u16=2;
pub const Z_AXIS:u16=3;
pub const TRANSLATION:u16=4;
pub const ROTATION:u16=5;}

impl Ros2Control_msgsActionFollowJointTrajectory_Result {pub const SUCCESSFUL:i32=0;
pub const INVALID_GOAL:i32=-1;
pub const INVALID_JOINTS:i32=-2;
pub const OLD_HEADER_TIMESTAMP:i32=-3;
pub const PATH_TOLERANCE_VIOLATED:i32=-4;
pub const GOAL_TOLERANCE_VIOLATED:i32=-5;}
