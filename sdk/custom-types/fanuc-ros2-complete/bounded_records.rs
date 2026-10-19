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
/// Complete bounded value for ROS2 fanuc_msgs/msg/IOType.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgIOType {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
}
impl Default for Ros2Fanuc_msgsMsgIOType {fn default()->Self {Self {
r#type:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgIOType {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1815aa6b1bcab1efe21de0e31570ca650553b5c70f0793301f82c0ca7934c586";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgIOType {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/AnalogIO.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgAnalogIO {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsMsgAnalogIO {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgAnalogIO {
  pub const MAX_WIRE_BYTES:usize=74;
  pub const CARTESIAN_WIRE_BYTES:u64=74;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d62097527ba99bf6e803283350161cd1415173a22eb3cb20331a232366076d00";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>74{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgAnalogIO {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>74{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/AnalogIOCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgAnalogIOCmd {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgAnalogIO>,
}
impl Default for Ros2Fanuc_msgsMsgAnalogIOCmd {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgAnalogIOCmd {
  pub const MAX_WIRE_BYTES:usize=2372;
  pub const CARTESIAN_WIRE_BYTES:u64=2372;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_62f38a088ed23f8daf7163f26514ae1aabd7b1b64845ccdbc281cb7839d857ba";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>2372{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgAnalogIO::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgAnalogIOCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2372{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/AnalogIOState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgAnalogIOState {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgAnalogIO>,
}
impl Default for Ros2Fanuc_msgsMsgAnalogIOState {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgAnalogIOState {
  pub const MAX_WIRE_BYTES:usize=2372;
  pub const CARTESIAN_WIRE_BYTES:u64=2372;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_820b41fecbf6db8e2b2e67d60cd1167cf474b22ca6c489f4d2d64dd33eaac9a4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>2372{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgAnalogIO::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgAnalogIOState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2372{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/BoolIO.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgBoolIO {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
}
impl Default for Ros2Fanuc_msgsMsgBoolIO {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgBoolIO {
  pub const MAX_WIRE_BYTES:usize=71;
  pub const CARTESIAN_WIRE_BYTES:u64=71;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9209ae1b8f5407a31beface0a3286f58bd823aabdc560c2a4bf4a9b1ca125aa7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
if n>71{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
put(out,at,&[u8::from(self.value)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgBoolIO {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>71{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/CollaborativeSpeedScaling.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgCollaborativeSpeedScaling {
  /// Declared collaborative_speed_scaling; canonical bounds are part of CODEC_IDENTITY.
  pub collaborative_speed_scaling:f64,
}
impl Default for Ros2Fanuc_msgsMsgCollaborativeSpeedScaling {fn default()->Self {Self {
collaborative_speed_scaling:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgCollaborativeSpeedScaling {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_693a3b581527d2eaf0f697f31e87ca2da428d1f51b821d42d6b06af387a2fb45";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.collaborative_speed_scaling.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.collaborative_speed_scaling.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.collaborative_speed_scaling=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgCollaborativeSpeedScaling {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/ConnectionStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgConnectionStatus {
  /// Declared is_connected; canonical bounds are part of CODEC_IDENTITY.
  pub is_connected:bool,
}
impl Default for Ros2Fanuc_msgsMsgConnectionStatus {fn default()->Self {Self {
is_connected:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgConnectionStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8ce24fb87538922b1f3c0ae856eb223d716143064eb46a6cfce5fec01b45b078";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.is_connected)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_connected=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgConnectionStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/ForceSensor.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgForceSensor {
  /// Declared force_x; canonical bounds are part of CODEC_IDENTITY.
  pub force_x:f32,
  /// Declared force_y; canonical bounds are part of CODEC_IDENTITY.
  pub force_y:f32,
  /// Declared force_z; canonical bounds are part of CODEC_IDENTITY.
  pub force_z:f32,
  /// Declared moment_x; canonical bounds are part of CODEC_IDENTITY.
  pub moment_x:f32,
  /// Declared moment_y; canonical bounds are part of CODEC_IDENTITY.
  pub moment_y:f32,
  /// Declared moment_z; canonical bounds are part of CODEC_IDENTITY.
  pub moment_z:f32,
  /// Declared fs_type; canonical bounds are part of CODEC_IDENTITY.
  pub fs_type:u32,
}
impl Default for Ros2Fanuc_msgsMsgForceSensor {fn default()->Self {Self {
force_x:Default::default(),
force_y:Default::default(),
force_z:Default::default(),
moment_x:Default::default(),
moment_y:Default::default(),
moment_z:Default::default(),
fs_type:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgForceSensor {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_06c99b43d7347646179850f3ba79192da7ce71296667d93d88659efbc1004032";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.force_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.force_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.force_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.moment_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.moment_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.moment_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.force_x.to_le_bytes());
put(out,at,&self.force_y.to_le_bytes());
put(out,at,&self.force_z.to_le_bytes());
put(out,at,&self.moment_x.to_le_bytes());
put(out,at,&self.moment_y.to_le_bytes());
put(out,at,&self.moment_z.to_le_bytes());
put(out,at,&self.fs_type.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.force_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.force_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.force_z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.moment_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.moment_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.moment_z=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.fs_type=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgForceSensor {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/Frame.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgFrame {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f32,
  /// Declared p; canonical bounds are part of CODEC_IDENTITY.
  pub p:f32,
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:f32,
}
impl Default for Ros2Fanuc_msgsMsgFrame {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
p:Default::default(),
r:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgFrame {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_609d1b7e49315f7f0a0fe86fa997673acd0d009e57b563690f36624b59b9effb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.p.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.r.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
put(out,at,&self.p.to_le_bytes());
put(out,at,&self.r.to_le_bytes());
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
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.p=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.r=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgFrame {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/IOCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgIOCmd {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgBoolIO>,
}
impl Default for Ros2Fanuc_msgsMsgIOCmd {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgIOCmd {
  pub const MAX_WIRE_BYTES:usize=2276;
  pub const CARTESIAN_WIRE_BYTES:u64=2276;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_462552bbaa3c9ed4caaf3108960d514e4ab61fcf2d6a84f0465b485bec1de45d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>2276{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgBoolIO::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgIOCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2276{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/IOState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgIOState {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgBoolIO>,
}
impl Default for Ros2Fanuc_msgsMsgIOState {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgIOState {
  pub const MAX_WIRE_BYTES:usize=2276;
  pub const CARTESIAN_WIRE_BYTES:u64=2276;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_332533445014c03d626a2c4dbdffd9421748aa6b73f49d407441fd6b9f2b24d8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>2276{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgBoolIO::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgIOState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2276{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/NumReg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgNumReg {
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsMsgNumReg {fn default()->Self {Self {
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgNumReg {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0fb9344c5768ea52ab072939f368f1e73dcc0ff83e74b4cb6d35fca47c1d29cc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.index.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgNumReg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/NumRegCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgNumRegCmd {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgNumReg>,
}
impl Default for Ros2Fanuc_msgsMsgNumRegCmd {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgNumRegCmd {
  pub const MAX_WIRE_BYTES:usize=196;
  pub const CARTESIAN_WIRE_BYTES:u64=196;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9c76500073889377525a0802c46bd96589a2639dd9534cf7ad2b9956cd95b5b6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>196{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgNumReg::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgNumRegCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>196{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/NumRegState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgNumRegState {
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<Ros2Fanuc_msgsMsgNumReg>,
}
impl Default for Ros2Fanuc_msgsMsgNumRegState {fn default()->Self {Self {
values:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgNumRegState {
  pub const MAX_WIRE_BYTES:usize=196;
  pub const CARTESIAN_WIRE_BYTES:u64=196;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9c8ff4917a6dd033e6a6776ae8525e93c2924d776f05fd54e7e8d2251fc48b3a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.values.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
n=add_size(n,element.wire_size()?)?;
}
if n>196{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Fanuc_msgsMsgNumReg::read_fields(bytes,at)?;
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgNumRegState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>196{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/RMIStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgRMIStatus {
  /// Declared program_status; canonical bounds are part of CODEC_IDENTITY.
  pub program_status:u32,
  /// Declared next_sequence; canonical bounds are part of CODEC_IDENTITY.
  pub next_sequence:u32,
  /// Declared last_sequence; canonical bounds are part of CODEC_IDENTITY.
  pub last_sequence:u32,
  /// Declared last_sequence_error; canonical bounds are part of CODEC_IDENTITY.
  pub last_sequence_error:i32,
  /// Declared last_sequence_instruction; canonical bounds are part of CODEC_IDENTITY.
  pub last_sequence_instruction:String,
  /// Declared last_sequence_error_message; canonical bounds are part of CODEC_IDENTITY.
  pub last_sequence_error_message:String,
}
impl Default for Ros2Fanuc_msgsMsgRMIStatus {fn default()->Self {Self {
program_status:Default::default(),
next_sequence:Default::default(),
last_sequence:Default::default(),
last_sequence_error:Default::default(),
last_sequence_instruction:Default::default(),
last_sequence_error_message:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgRMIStatus {
  pub const MAX_WIRE_BYTES:usize=1048;
  pub const CARTESIAN_WIRE_BYTES:u64=1048;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dc2e7eaabf44807142581905838f8951653682a6c6d61ebcf56494b16a10d5e8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.last_sequence_instruction.len()>512||self.last_sequence_instruction.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.last_sequence_instruction.len())?;
if self.last_sequence_error_message.len()>512||self.last_sequence_error_message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.last_sequence_error_message.len())?;
if n>1048{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.program_status.to_le_bytes());
put(out,at,&self.next_sequence.to_le_bytes());
put(out,at,&self.last_sequence.to_le_bytes());
put(out,at,&self.last_sequence_error.to_le_bytes());
put(out,at,&(self.last_sequence_instruction.len() as u32).to_le_bytes());put(out,at,self.last_sequence_instruction.as_bytes());
put(out,at,&(self.last_sequence_error_message.len() as u32).to_le_bytes());put(out,at,self.last_sequence_error_message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.program_status=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.next_sequence=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.last_sequence=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.last_sequence_error=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.last_sequence_instruction=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.last_sequence_error_message=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgRMIStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1048{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/RobotStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgRobotStatus {
  /// Declared in_error; canonical bounds are part of CODEC_IDENTITY.
  pub in_error:bool,
  /// Declared tp_enabled; canonical bounds are part of CODEC_IDENTITY.
  pub tp_enabled:bool,
  /// Declared e_stopped; canonical bounds are part of CODEC_IDENTITY.
  pub e_stopped:bool,
  /// Declared motion_possible; canonical bounds are part of CODEC_IDENTITY.
  pub motion_possible:bool,
  /// Declared contact_stop_mode; canonical bounds are part of CODEC_IDENTITY.
  pub contact_stop_mode:u8,
}
impl Default for Ros2Fanuc_msgsMsgRobotStatus {fn default()->Self {Self {
in_error:Default::default(),
tp_enabled:Default::default(),
e_stopped:Default::default(),
motion_possible:Default::default(),
contact_stop_mode:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgRobotStatus {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f31ee3631dc6f4c90eb5ded6209ccebf0f4193ddfe5f5032bdd2bcd71f8cc31";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.in_error)]);
put(out,at,&[u8::from(self.tp_enabled)]);
put(out,at,&[u8::from(self.e_stopped)]);
put(out,at,&[u8::from(self.motion_possible)]);
put(out,at,&self.contact_stop_mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.in_error=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.tp_enabled=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.e_stopped=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.motion_possible=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.contact_stop_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgRobotStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/msg/RobotStatusExt.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsMsgRobotStatusExt {
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:String,
  /// Declared in_motion; canonical bounds are part of CODEC_IDENTITY.
  pub in_motion:bool,
  /// Declared drives_powered; canonical bounds are part of CODEC_IDENTITY.
  pub drives_powered:bool,
  /// Declared gen_override; canonical bounds are part of CODEC_IDENTITY.
  pub gen_override:i32,
  /// Declared speed_clamp_limit; canonical bounds are part of CODEC_IDENTITY.
  pub speed_clamp_limit:f32,
  /// Declared control_mode; canonical bounds are part of CODEC_IDENTITY.
  pub control_mode:String,
}
impl Default for Ros2Fanuc_msgsMsgRobotStatusExt {fn default()->Self {Self {
error_code:Default::default(),
in_motion:Default::default(),
drives_powered:Default::default(),
gen_override:Default::default(),
speed_clamp_limit:Default::default(),
control_mode:Default::default(),
}}}
impl Ros2Fanuc_msgsMsgRobotStatusExt {
  pub const MAX_WIRE_BYTES:usize=1042;
  pub const CARTESIAN_WIRE_BYTES:u64=1042;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8ba47a11bfbe02026fe79b32e1ed02e5f609499ea55bb57c9f09af9fba389648";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.error_code.len()>512||self.error_code.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_code.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
if !self.speed_clamp_limit.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if self.control_mode.len()>512||self.control_mode.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.control_mode.len())?;
if n>1042{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.error_code.len() as u32).to_le_bytes());put(out,at,self.error_code.as_bytes());
put(out,at,&[u8::from(self.in_motion)]);
put(out,at,&[u8::from(self.drives_powered)]);
put(out,at,&self.gen_override.to_le_bytes());
put(out,at,&self.speed_clamp_limit.to_le_bytes());
put(out,at,&(self.control_mode.len() as u32).to_le_bytes());put(out,at,self.control_mode.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_code=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.in_motion=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.drives_powered=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.gen_override=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed_clamp_limit=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.control_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsMsgRobotStatusExt {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1042{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<18{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/CfgForceSensor_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvCfgForceSensor_Request {
  /// Declared do_reset; canonical bounds are part of CODEC_IDENTITY.
  pub do_reset:u32,
  /// Declared fs_type; canonical bounds are part of CODEC_IDENTITY.
  pub fs_type:u32,
}
impl Default for Ros2Fanuc_msgsSrvCfgForceSensor_Request {fn default()->Self {Self {
do_reset:Default::default(),
fs_type:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvCfgForceSensor_Request {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_01973ed21546cb747ad2ce2a990a8ec4e70efb66e1c8fc8856cf1082870984ff";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.do_reset.to_le_bytes());
put(out,at,&self.fs_type.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.do_reset=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.fs_type=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvCfgForceSensor_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/CfgForceSensor_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvCfgForceSensor_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvCfgForceSensor_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvCfgForceSensor_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_41799ce44c937b1524889391db61e2b4d35795055cb4bbacfe5cc8bbd5b9a37b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvCfgForceSensor_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetAnalogIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetAnalogIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetAnalogIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetAnalogIO_Request {
  pub const MAX_WIRE_BYTES:usize=70;
  pub const CARTESIAN_WIRE_BYTES:u64=70;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cd567f97de2de581975c3d4f280eec545dea37c941d316fc3708bbc4d3af94d9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
if n>70{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetAnalogIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>70{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetAnalogIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetAnalogIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsSrvGetAnalogIO_Response {fn default()->Self {Self {
result:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetAnalogIO_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3b53d3ce95d95454a782778c160fcc102ead869c5681c9c67224c373fef1b217";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetAnalogIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetBoolIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetBoolIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetBoolIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetBoolIO_Request {
  pub const MAX_WIRE_BYTES:usize=70;
  pub const CARTESIAN_WIRE_BYTES:u64=70;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ce8c4b1efd2828e92656e897baf041e8965367a236079231101807c898d4b50c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
if n>70{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetBoolIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>70{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetBoolIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetBoolIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
}
impl Default for Ros2Fanuc_msgsSrvGetBoolIO_Response {fn default()->Self {Self {
result:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetBoolIO_Response {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7dbdcdde8d76c4ef422ea158551dd912273220c25a08f28fc8d19fdf8bc8a01c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&[u8::from(self.value)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetBoolIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetCartesianPosition_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetCartesianPosition_Request {
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetCartesianPosition_Request {fn default()->Self {Self {
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetCartesianPosition_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a694a17bde88131efcadac4f134464a7777beab4b09e17624ae7b173ab62c3e4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetCartesianPosition_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetCartesianPosition_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetCartesianPosition_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared timetag; canonical bounds are part of CODEC_IDENTITY.
  pub timetag:i32,
  /// Declared utool; canonical bounds are part of CODEC_IDENTITY.
  pub utool:u8,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:u8,
  /// Declared front; canonical bounds are part of CODEC_IDENTITY.
  pub front:u8,
  /// Declared up; canonical bounds are part of CODEC_IDENTITY.
  pub up:u8,
  /// Declared left; canonical bounds are part of CODEC_IDENTITY.
  pub left:u8,
  /// Declared flip; canonical bounds are part of CODEC_IDENTITY.
  pub flip:u8,
  /// Declared turn4; canonical bounds are part of CODEC_IDENTITY.
  pub turn4:i8,
  /// Declared turn5; canonical bounds are part of CODEC_IDENTITY.
  pub turn5:i8,
  /// Declared turn6; canonical bounds are part of CODEC_IDENTITY.
  pub turn6:i8,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f32,
  /// Declared p; canonical bounds are part of CODEC_IDENTITY.
  pub p:f32,
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:f32,
  /// Declared ext1; canonical bounds are part of CODEC_IDENTITY.
  pub ext1:f32,
  /// Declared ext2; canonical bounds are part of CODEC_IDENTITY.
  pub ext2:f32,
  /// Declared ext3; canonical bounds are part of CODEC_IDENTITY.
  pub ext3:f32,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetCartesianPosition_Response {fn default()->Self {Self {
result:Default::default(),
timetag:Default::default(),
utool:Default::default(),
uframe:Default::default(),
front:Default::default(),
up:Default::default(),
left:Default::default(),
flip:Default::default(),
turn4:Default::default(),
turn5:Default::default(),
turn6:Default::default(),
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
p:Default::default(),
r:Default::default(),
ext1:Default::default(),
ext2:Default::default(),
ext3:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetCartesianPosition_Response {
  pub const MAX_WIRE_BYTES:usize=54;
  pub const CARTESIAN_WIRE_BYTES:u64=54;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0516f0145717b06550674e02f0b9c47f9b312f4f361c791f56257fbb79e67329";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.p.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.r.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>54{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.timetag.to_le_bytes());
put(out,at,&self.utool.to_le_bytes());
put(out,at,&self.uframe.to_le_bytes());
put(out,at,&self.front.to_le_bytes());
put(out,at,&self.up.to_le_bytes());
put(out,at,&self.left.to_le_bytes());
put(out,at,&self.flip.to_le_bytes());
put(out,at,&self.turn4.to_le_bytes());
put(out,at,&self.turn5.to_le_bytes());
put(out,at,&self.turn6.to_le_bytes());
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
put(out,at,&self.p.to_le_bytes());
put(out,at,&self.r.to_le_bytes());
put(out,at,&self.ext1.to_le_bytes());
put(out,at,&self.ext2.to_le_bytes());
put(out,at,&self.ext3.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.timetag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.up=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.left=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.flip=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn5=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.p=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.r=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext3=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetCartesianPosition_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>54{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<54{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetGroupIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetGroupIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetGroupIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetGroupIO_Request {
  pub const MAX_WIRE_BYTES:usize=70;
  pub const CARTESIAN_WIRE_BYTES:u64=70;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dbd2e8e791e8debc745843827ed296b51bf84159f4c4fe5d912c231d621de216";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
if n>70{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetGroupIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>70{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetGroupIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetGroupIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetGroupIO_Response {fn default()->Self {Self {
result:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetGroupIO_Response {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9e131b91f36badc90ddae54a9855095241ab86c51b419b684d62879fae0bda7f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,2)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetGroupIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetNumReg_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetNumReg_Request {
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetNumReg_Request {fn default()->Self {Self {
index:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetNumReg_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_727b5ff4ab9c8976280df12e80d3aee03b31288087adb94ee82161574fc8dcb5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.index.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetNumReg_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetNumReg_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetNumReg_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsSrvGetNumReg_Response {fn default()->Self {Self {
result:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetNumReg_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_78ae7ff82b3b2cd7d512fee8335f14a0759a71425fcf3d1b150dbe5e2a0179b3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetNumReg_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadComp_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadComp_Request {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadComp_Request {fn default()->Self {Self {
payload_schedule_id:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPayloadComp_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d48b88619f4a68ceeed02336e9ef078c27905707a21d977448fbbfbd560f398d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadComp_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadComp_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadComp_Response {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cg_x; canonical bounds are part of CODEC_IDENTITY.
  pub cg_x:f32,
  /// Declared cg_y; canonical bounds are part of CODEC_IDENTITY.
  pub cg_y:f32,
  /// Declared cg_z; canonical bounds are part of CODEC_IDENTITY.
  pub cg_z:f32,
  /// Declared in_x; canonical bounds are part of CODEC_IDENTITY.
  pub in_x:f32,
  /// Declared in_y; canonical bounds are part of CODEC_IDENTITY.
  pub in_y:f32,
  /// Declared in_z; canonical bounds are part of CODEC_IDENTITY.
  pub in_z:f32,
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadComp_Response {fn default()->Self {Self {
mass:Default::default(),
cg_x:Default::default(),
cg_y:Default::default(),
cg_z:Default::default(),
in_x:Default::default(),
in_y:Default::default(),
in_z:Default::default(),
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPayloadComp_Response {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_012b216c949f99755a36678b6be28d032f698b472ce4fa9ba975bc6d453ae4e4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
put(out,at,&self.cg_x.to_le_bytes());
put(out,at,&self.cg_y.to_le_bytes());
put(out,at,&self.cg_z.to_le_bytes());
put(out,at,&self.in_x.to_le_bytes());
put(out,at,&self.in_y.to_le_bytes());
put(out,at,&self.in_z.to_le_bytes());
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_z=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadComp_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadID_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadID_Request {
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadID_Request {fn default()->Self {Self {
}}}
impl Ros2Fanuc_msgsSrvGetPayloadID_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6c943659905c8adc7aa4b73832584994c83bfe009d18dc30a11d99b9fc0d6eef";
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
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadID_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadID_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadID_Response {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadID_Response {fn default()->Self {Self {
payload_schedule_id:Default::default(),
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPayloadID_Response {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5551f4eab590c87fd7679d593c9c446903bdaa02aa1ed2512b146768122a950c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
n=add_size(n,4)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadID_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadValue_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadValue_Request {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadValue_Request {fn default()->Self {Self {
payload_schedule_id:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPayloadValue_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d82667bd644873ae4fa5de0289ed94791eb2a7af49c1b8f86ed7f91cf875407a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadValue_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPayloadValue_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPayloadValue_Response {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cg_x; canonical bounds are part of CODEC_IDENTITY.
  pub cg_x:f32,
  /// Declared cg_y; canonical bounds are part of CODEC_IDENTITY.
  pub cg_y:f32,
  /// Declared cg_z; canonical bounds are part of CODEC_IDENTITY.
  pub cg_z:f32,
  /// Declared in_x; canonical bounds are part of CODEC_IDENTITY.
  pub in_x:f32,
  /// Declared in_y; canonical bounds are part of CODEC_IDENTITY.
  pub in_y:f32,
  /// Declared in_z; canonical bounds are part of CODEC_IDENTITY.
  pub in_z:f32,
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvGetPayloadValue_Response {fn default()->Self {Self {
mass:Default::default(),
cg_x:Default::default(),
cg_y:Default::default(),
cg_z:Default::default(),
in_x:Default::default(),
in_y:Default::default(),
in_z:Default::default(),
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPayloadValue_Response {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c3361ceec2ccd5b4d3ef566cbd885f8cddba75b4f1dc1712144147827689f06d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
put(out,at,&self.cg_x.to_le_bytes());
put(out,at,&self.cg_y.to_le_bytes());
put(out,at,&self.cg_z.to_le_bytes());
put(out,at,&self.in_x.to_le_bytes());
put(out,at,&self.in_y.to_le_bytes());
put(out,at,&self.in_z.to_le_bytes());
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_z=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPayloadValue_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPosReg_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPosReg_Request {
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
}
impl Default for Ros2Fanuc_msgsSrvGetPosReg_Request {fn default()->Self {Self {
index:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPosReg_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f7f1391c5ea385bff615d95c5e38f01a809828b142112829dc436916b063505a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.index.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPosReg_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetPosReg_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetPosReg_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared representation; canonical bounds are part of CODEC_IDENTITY.
  pub representation:String,
  /// Declared utool; canonical bounds are part of CODEC_IDENTITY.
  pub utool:u8,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:u8,
  /// Declared front; canonical bounds are part of CODEC_IDENTITY.
  pub front:u8,
  /// Declared up; canonical bounds are part of CODEC_IDENTITY.
  pub up:u8,
  /// Declared left; canonical bounds are part of CODEC_IDENTITY.
  pub left:u8,
  /// Declared flip; canonical bounds are part of CODEC_IDENTITY.
  pub flip:u8,
  /// Declared turn4; canonical bounds are part of CODEC_IDENTITY.
  pub turn4:i8,
  /// Declared turn5; canonical bounds are part of CODEC_IDENTITY.
  pub turn5:i8,
  /// Declared turn6; canonical bounds are part of CODEC_IDENTITY.
  pub turn6:i8,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f32,
  /// Declared p; canonical bounds are part of CODEC_IDENTITY.
  pub p:f32,
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:f32,
  /// Declared ext1; canonical bounds are part of CODEC_IDENTITY.
  pub ext1:f32,
  /// Declared ext2; canonical bounds are part of CODEC_IDENTITY.
  pub ext2:f32,
  /// Declared ext3; canonical bounds are part of CODEC_IDENTITY.
  pub ext3:f32,
  /// Declared j1; canonical bounds are part of CODEC_IDENTITY.
  pub j1:f32,
  /// Declared j2; canonical bounds are part of CODEC_IDENTITY.
  pub j2:f32,
  /// Declared j3; canonical bounds are part of CODEC_IDENTITY.
  pub j3:f32,
  /// Declared j4; canonical bounds are part of CODEC_IDENTITY.
  pub j4:f32,
  /// Declared j5; canonical bounds are part of CODEC_IDENTITY.
  pub j5:f32,
  /// Declared j6; canonical bounds are part of CODEC_IDENTITY.
  pub j6:f32,
  /// Declared j7; canonical bounds are part of CODEC_IDENTITY.
  pub j7:f32,
  /// Declared j8; canonical bounds are part of CODEC_IDENTITY.
  pub j8:f32,
  /// Declared j9; canonical bounds are part of CODEC_IDENTITY.
  pub j9:f32,
}
impl Default for Ros2Fanuc_msgsSrvGetPosReg_Response {fn default()->Self {Self {
result:Default::default(),
representation:Default::default(),
utool:Default::default(),
uframe:Default::default(),
front:Default::default(),
up:Default::default(),
left:Default::default(),
flip:Default::default(),
turn4:Default::default(),
turn5:Default::default(),
turn6:Default::default(),
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
p:Default::default(),
r:Default::default(),
ext1:Default::default(),
ext2:Default::default(),
ext3:Default::default(),
j1:Default::default(),
j2:Default::default(),
j3:Default::default(),
j4:Default::default(),
j5:Default::default(),
j6:Default::default(),
j7:Default::default(),
j8:Default::default(),
j9:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetPosReg_Response {
  pub const MAX_WIRE_BYTES:usize=601;
  pub const CARTESIAN_WIRE_BYTES:u64=601;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e540225b0d532c460905582ad99ee175dd771e971c7e75f5dfb9325d39ed6b27";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.representation.len()>512||self.representation.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.representation.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.p.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.r.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>601{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&(self.representation.len() as u32).to_le_bytes());put(out,at,self.representation.as_bytes());
put(out,at,&self.utool.to_le_bytes());
put(out,at,&self.uframe.to_le_bytes());
put(out,at,&self.front.to_le_bytes());
put(out,at,&self.up.to_le_bytes());
put(out,at,&self.left.to_le_bytes());
put(out,at,&self.flip.to_le_bytes());
put(out,at,&self.turn4.to_le_bytes());
put(out,at,&self.turn5.to_le_bytes());
put(out,at,&self.turn6.to_le_bytes());
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
put(out,at,&self.p.to_le_bytes());
put(out,at,&self.r.to_le_bytes());
put(out,at,&self.ext1.to_le_bytes());
put(out,at,&self.ext2.to_le_bytes());
put(out,at,&self.ext3.to_le_bytes());
put(out,at,&self.j1.to_le_bytes());
put(out,at,&self.j2.to_le_bytes());
put(out,at,&self.j3.to_le_bytes());
put(out,at,&self.j4.to_le_bytes());
put(out,at,&self.j5.to_le_bytes());
put(out,at,&self.j6.to_le_bytes());
put(out,at,&self.j7.to_le_bytes());
put(out,at,&self.j8.to_le_bytes());
put(out,at,&self.j9.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.representation=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.up=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.left=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.flip=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn5=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.p=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.r=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j9=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetPosReg_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>601{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<89{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetTCPSpeed_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetTCPSpeed_Request {
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetTCPSpeed_Request {fn default()->Self {Self {
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetTCPSpeed_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6bdfd44465b75bbd4e232f802dbd3c0bfa28e90a93f9a28b3c15fb150bce7ac7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetTCPSpeed_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetTCPSpeed_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetTCPSpeed_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared timetag; canonical bounds are part of CODEC_IDENTITY.
  pub timetag:i32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f32,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetTCPSpeed_Response {fn default()->Self {Self {
result:Default::default(),
timetag:Default::default(),
speed:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetTCPSpeed_Response {
  pub const MAX_WIRE_BYTES:usize=13;
  pub const CARTESIAN_WIRE_BYTES:u64=13;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3ec07d99342f275506452f32076f9e1ea01bf32f43662311708a9b44ca951147";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if !self.speed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>13{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.timetag.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.timetag=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetTCPSpeed_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>13{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUFrameData_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUFrameData_Request {
  /// Declared uframe_number; canonical bounds are part of CODEC_IDENTITY.
  pub uframe_number:u8,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUFrameData_Request {fn default()->Self {Self {
uframe_number:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUFrameData_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c1ef7077fd7f61ed8ff46931c8e39c5950658cfe1d36cffac10083ab304307be";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.uframe_number.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUFrameData_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUFrameData_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUFrameData_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared uframe_number; canonical bounds are part of CODEC_IDENTITY.
  pub uframe_number:u8,
  /// Declared frame; canonical bounds are part of CODEC_IDENTITY.
  pub frame:Ros2Fanuc_msgsMsgFrame,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUFrameData_Response {fn default()->Self {Self {
result:Default::default(),
uframe_number:Default::default(),
frame:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUFrameData_Response {
  pub const MAX_WIRE_BYTES:usize=30;
  pub const CARTESIAN_WIRE_BYTES:u64=30;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f956e1447bd4a15f547fdf5d6775836018d50153270fe8863c4b30d83422fd2e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,self.frame.wire_size()?)?;
n=add_size(n,1)?;
if n>30{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.uframe_number.to_le_bytes());
self.frame.write_fields(out,at);
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe_number=decoded;
let decoded=Ros2Fanuc_msgsMsgFrame::read_fields(bytes,at)?;
value.frame=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUFrameData_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>30{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUFrameUTool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUFrameUTool_Request {
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUFrameUTool_Request {fn default()->Self {Self {
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUFrameUTool_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3d57da6f8003b33b2a079d4f284eb0f182d7e088074112b7f4ee25d3113bd443";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUFrameUTool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUFrameUTool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUFrameUTool_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared uframe_number; canonical bounds are part of CODEC_IDENTITY.
  pub uframe_number:u8,
  /// Declared utool_number; canonical bounds are part of CODEC_IDENTITY.
  pub utool_number:u8,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUFrameUTool_Response {fn default()->Self {Self {
result:Default::default(),
uframe_number:Default::default(),
utool_number:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUFrameUTool_Response {
  pub const MAX_WIRE_BYTES:usize=7;
  pub const CARTESIAN_WIRE_BYTES:u64=7;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f9cb64b89f17b6811e4073b9f94d0fd6844d21ec4b846f78bfd13cb14a35a6b3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>7{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.uframe_number.to_le_bytes());
put(out,at,&self.utool_number.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUFrameUTool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>7{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUToolData_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUToolData_Request {
  /// Declared utool_number; canonical bounds are part of CODEC_IDENTITY.
  pub utool_number:u8,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUToolData_Request {fn default()->Self {Self {
utool_number:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUToolData_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_10c3799dcb1b09dbcb6d1b794a00ecfaf69a8f457d95eeb92855f79dba7be1bc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.utool_number.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUToolData_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/GetUToolData_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvGetUToolData_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared utool_number; canonical bounds are part of CODEC_IDENTITY.
  pub utool_number:u8,
  /// Declared frame; canonical bounds are part of CODEC_IDENTITY.
  pub frame:Ros2Fanuc_msgsMsgFrame,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvGetUToolData_Response {fn default()->Self {Self {
result:Default::default(),
utool_number:Default::default(),
frame:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvGetUToolData_Response {
  pub const MAX_WIRE_BYTES:usize=30;
  pub const CARTESIAN_WIRE_BYTES:u64=30;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f3a2b94f337cec9c58156b803675f38885350cc3fc3171234c0d361afc800d81";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,self.frame.wire_size()?)?;
n=add_size(n,1)?;
if n>30{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.utool_number.to_le_bytes());
self.frame.write_fields(out,at);
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool_number=decoded;
let decoded=Ros2Fanuc_msgsMsgFrame::read_fields(bytes,at)?;
value.frame=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvGetUToolData_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>30{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/ReadError_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvReadError_Request {
  /// Declared count; canonical bounds are part of CODEC_IDENTITY.
  pub count:u8,
}
impl Default for Ros2Fanuc_msgsSrvReadError_Request {fn default()->Self {Self {
count:1,
}}}
impl Ros2Fanuc_msgsSrvReadError_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b67ed8e2ecccefb6bdead80249f5916e6ea192d822183292db43351839fb6f9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.count.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.count=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvReadError_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/ReadError_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvReadError_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared count; canonical bounds are part of CODEC_IDENTITY.
  pub count:u8,
  /// Declared error_data; canonical bounds are part of CODEC_IDENTITY.
  pub error_data:[String;5],
}
impl Default for Ros2Fanuc_msgsSrvReadError_Response {fn default()->Self {Self {
result:Default::default(),
count:Default::default(),
error_data:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Fanuc_msgsSrvReadError_Response {
  pub const MAX_WIRE_BYTES:usize=2585;
  pub const CARTESIAN_WIRE_BYTES:u64=2585;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6e28db10c7afb06cb1256f6d74a43e26c5efa6c13fec8826ae0e623a5eb954b6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
for element in &self.error_data {
if element.len()>512||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if n>2585{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.count.to_le_bytes());
for element in &self.error_data {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.count=decoded;
{let count=5usize;if count>5{return Err(invalid());}
for i in 0..count {
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_data[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvReadError_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2585{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/Reset_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvReset_Request {
}
impl Default for Ros2Fanuc_msgsSrvReset_Request {fn default()->Self {Self {
}}}
impl Ros2Fanuc_msgsSrvReset_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b73f661d569f6af8dad1cd6625f4c0c55ef2c14c79b3e9af0f17fdc02736abc4";
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
impl Canonical for Ros2Fanuc_msgsSrvReset_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/Reset_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvReset_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvReset_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvReset_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5e9b92e5a112b4934ce0893afaa09156fdb47c26b0aa6ae9a27d61920d308ab8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvReset_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetAnalogIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetAnalogIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsSrvSetAnalogIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetAnalogIO_Request {
  pub const MAX_WIRE_BYTES:usize=74;
  pub const CARTESIAN_WIRE_BYTES:u64=74;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a6bb4acfae23db79464faff849f359657537026d7c79f8a6c60259caec5e87f4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>74{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetAnalogIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>74{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetAnalogIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetAnalogIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetAnalogIO_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetAnalogIO_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aeac383e2c4a292f1237494bff5b261cd0197542e00456b4ee0edf9b7fe8a39b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetAnalogIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetBoolIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetBoolIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:bool,
}
impl Default for Ros2Fanuc_msgsSrvSetBoolIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetBoolIO_Request {
  pub const MAX_WIRE_BYTES:usize=71;
  pub const CARTESIAN_WIRE_BYTES:u64=71;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0e8ace58c769670a2519e733d020a3ca902bff99982514d5cd9ecda391c1a4cf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
if n>71{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
put(out,at,&[u8::from(self.value)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetBoolIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>71{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<7{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetBoolIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetBoolIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetBoolIO_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetBoolIO_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c9c68ca577dd6e4d1743385d9c69c8f1c6e0780ae247877d823ab97a68e675a0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetBoolIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetGenOverride_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetGenOverride_Request {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:i8,
}
impl Default for Ros2Fanuc_msgsSrvSetGenOverride_Request {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetGenOverride_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7d321e33aaf6f58013c33ffc012c1ddbd27aefff3f21d7456030a1d590362039";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetGenOverride_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetGenOverride_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetGenOverride_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetGenOverride_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetGenOverride_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ffcb25cad7dbf51c216753dac2c117a191fb081a5a562e63d34d447e61f0a59";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetGenOverride_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetGroupIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetGroupIO_Request {
  /// Declared io_type; canonical bounds are part of CODEC_IDENTITY.
  pub io_type:Ros2Fanuc_msgsMsgIOType,
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Fanuc_msgsSrvSetGroupIO_Request {fn default()->Self {Self {
io_type:Default::default(),
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetGroupIO_Request {
  pub const MAX_WIRE_BYTES:usize=72;
  pub const CARTESIAN_WIRE_BYTES:u64=72;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_42a76aec283e26e3a2c99bb27a23778e34effb6c3328754793f783038a2b84bd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.io_type.wire_size()?)?;
n=add_size(n,2)?;
n=add_size(n,2)?;
if n>72{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.io_type.write_fields(out,at);
put(out,at,&self.index.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Fanuc_msgsMsgIOType::read_fields(bytes,at)?;
value.io_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetGroupIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>72{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetGroupIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetGroupIO_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetGroupIO_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetGroupIO_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aa6d2c99e8817273f8f2ab179df61d60efd8ee2cee0e1ec76c647956e22bff17";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetGroupIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetNumReg_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetNumReg_Request {
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:f32,
}
impl Default for Ros2Fanuc_msgsSrvSetNumReg_Request {fn default()->Self {Self {
index:Default::default(),
value:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetNumReg_Request {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7e362b671b8770db33f3876308c41a930863a0805771fa05fcbb170782207e36";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if !self.value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.index.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetNumReg_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetNumReg_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetNumReg_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetNumReg_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetNumReg_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7c7850ad8bb5c861d9b436311a8cf985e2e029d362877be80c5a6cb19021e482";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetNumReg_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadComp_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadComp_Request {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cg_x; canonical bounds are part of CODEC_IDENTITY.
  pub cg_x:f32,
  /// Declared cg_y; canonical bounds are part of CODEC_IDENTITY.
  pub cg_y:f32,
  /// Declared cg_z; canonical bounds are part of CODEC_IDENTITY.
  pub cg_z:f32,
  /// Declared in_x; canonical bounds are part of CODEC_IDENTITY.
  pub in_x:f32,
  /// Declared in_y; canonical bounds are part of CODEC_IDENTITY.
  pub in_y:f32,
  /// Declared in_z; canonical bounds are part of CODEC_IDENTITY.
  pub in_z:f32,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadComp_Request {fn default()->Self {Self {
payload_schedule_id:Default::default(),
mass:Default::default(),
cg_x:Default::default(),
cg_y:Default::default(),
cg_z:Default::default(),
in_x:Default::default(),
in_y:Default::default(),
in_z:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadComp_Request {
  pub const MAX_WIRE_BYTES:usize=30;
  pub const CARTESIAN_WIRE_BYTES:u64=30;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_92c495d71657249e15c172e850aa91869929a25817437567778df1b01d9037a8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>30{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
put(out,at,&self.mass.to_le_bytes());
put(out,at,&self.cg_x.to_le_bytes());
put(out,at,&self.cg_y.to_le_bytes());
put(out,at,&self.cg_z.to_le_bytes());
put(out,at,&self.in_x.to_le_bytes());
put(out,at,&self.in_y.to_le_bytes());
put(out,at,&self.in_z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_z=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadComp_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>30{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadComp_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadComp_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadComp_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadComp_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_37a9876e2a9c514fa8c4337ff1351f778d350318bdc6d8aa5d569b4039a2f76b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadComp_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadID_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadID_Request {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadID_Request {fn default()->Self {Self {
payload_schedule_id:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadID_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aa15b59db17d2afb7ef9e74676a9b744763954ae063efafb91a3ab3c9852a1c0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadID_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadID_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadID_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadID_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadID_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7b704b36da10dc71b4cfaad92ea02ed22e4966077dc0940b03b8b8e49c5769c7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadID_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadValue_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadValue_Request {
  /// Declared payload_schedule_id; canonical bounds are part of CODEC_IDENTITY.
  pub payload_schedule_id:u16,
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared cg_x; canonical bounds are part of CODEC_IDENTITY.
  pub cg_x:f32,
  /// Declared cg_y; canonical bounds are part of CODEC_IDENTITY.
  pub cg_y:f32,
  /// Declared cg_z; canonical bounds are part of CODEC_IDENTITY.
  pub cg_z:f32,
  /// Declared use_in; canonical bounds are part of CODEC_IDENTITY.
  pub use_in:bool,
  /// Declared in_x; canonical bounds are part of CODEC_IDENTITY.
  pub in_x:f32,
  /// Declared in_y; canonical bounds are part of CODEC_IDENTITY.
  pub in_y:f32,
  /// Declared in_z; canonical bounds are part of CODEC_IDENTITY.
  pub in_z:f32,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadValue_Request {fn default()->Self {Self {
payload_schedule_id:Default::default(),
mass:Default::default(),
cg_x:Default::default(),
cg_y:Default::default(),
cg_z:Default::default(),
use_in:Default::default(),
in_x:Default::default(),
in_y:Default::default(),
in_z:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadValue_Request {
  pub const MAX_WIRE_BYTES:usize=31;
  pub const CARTESIAN_WIRE_BYTES:u64=31;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d56ce8f2f8df9c3450d6a5dc77e9e77861471805aa6b9c6f8815c2e7c324974";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cg_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.in_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.in_z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>31{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.payload_schedule_id.to_le_bytes());
put(out,at,&self.mass.to_le_bytes());
put(out,at,&self.cg_x.to_le_bytes());
put(out,at,&self.cg_y.to_le_bytes());
put(out,at,&self.cg_z.to_le_bytes());
put(out,at,&[u8::from(self.use_in)]);
put(out,at,&self.in_x.to_le_bytes());
put(out,at,&self.in_y.to_le_bytes());
put(out,at,&self.in_z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.payload_schedule_id=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cg_z=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_in=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.in_z=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadValue_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>31{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<31{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPayloadValue_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPayloadValue_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetPayloadValue_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPayloadValue_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_09f30071b49ac57353429e8a2862ee80d3dcab9237e8192d008c12a3628cc6ab";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPayloadValue_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPosReg_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPosReg_Request {
  /// Declared index; canonical bounds are part of CODEC_IDENTITY.
  pub index:u16,
  /// Declared representation; canonical bounds are part of CODEC_IDENTITY.
  pub representation:String,
  /// Declared utool; canonical bounds are part of CODEC_IDENTITY.
  pub utool:u8,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:u8,
  /// Declared front; canonical bounds are part of CODEC_IDENTITY.
  pub front:u8,
  /// Declared up; canonical bounds are part of CODEC_IDENTITY.
  pub up:u8,
  /// Declared left; canonical bounds are part of CODEC_IDENTITY.
  pub left:u8,
  /// Declared flip; canonical bounds are part of CODEC_IDENTITY.
  pub flip:u8,
  /// Declared turn4; canonical bounds are part of CODEC_IDENTITY.
  pub turn4:i8,
  /// Declared turn5; canonical bounds are part of CODEC_IDENTITY.
  pub turn5:i8,
  /// Declared turn6; canonical bounds are part of CODEC_IDENTITY.
  pub turn6:i8,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f32,
  /// Declared p; canonical bounds are part of CODEC_IDENTITY.
  pub p:f32,
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:f32,
  /// Declared ext1; canonical bounds are part of CODEC_IDENTITY.
  pub ext1:f32,
  /// Declared ext2; canonical bounds are part of CODEC_IDENTITY.
  pub ext2:f32,
  /// Declared ext3; canonical bounds are part of CODEC_IDENTITY.
  pub ext3:f32,
  /// Declared j1; canonical bounds are part of CODEC_IDENTITY.
  pub j1:f32,
  /// Declared j2; canonical bounds are part of CODEC_IDENTITY.
  pub j2:f32,
  /// Declared j3; canonical bounds are part of CODEC_IDENTITY.
  pub j3:f32,
  /// Declared j4; canonical bounds are part of CODEC_IDENTITY.
  pub j4:f32,
  /// Declared j5; canonical bounds are part of CODEC_IDENTITY.
  pub j5:f32,
  /// Declared j6; canonical bounds are part of CODEC_IDENTITY.
  pub j6:f32,
  /// Declared j7; canonical bounds are part of CODEC_IDENTITY.
  pub j7:f32,
  /// Declared j8; canonical bounds are part of CODEC_IDENTITY.
  pub j8:f32,
  /// Declared j9; canonical bounds are part of CODEC_IDENTITY.
  pub j9:f32,
}
impl Default for Ros2Fanuc_msgsSrvSetPosReg_Request {fn default()->Self {Self {
index:Default::default(),
representation:Default::default(),
utool:Default::default(),
uframe:Default::default(),
front:Default::default(),
up:Default::default(),
left:Default::default(),
flip:Default::default(),
turn4:Default::default(),
turn5:Default::default(),
turn6:Default::default(),
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
p:Default::default(),
r:Default::default(),
ext1:Default::default(),
ext2:Default::default(),
ext3:Default::default(),
j1:Default::default(),
j2:Default::default(),
j3:Default::default(),
j4:Default::default(),
j5:Default::default(),
j6:Default::default(),
j7:Default::default(),
j8:Default::default(),
j9:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPosReg_Request {
  pub const MAX_WIRE_BYTES:usize=599;
  pub const CARTESIAN_WIRE_BYTES:u64=599;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f9af742e55a044a1a363a45cbe4a0a8ce4ca62fcaf43759041cef89149d96d30";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.representation.len()>512||self.representation.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.representation.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.p.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.r.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.j9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>599{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.index.to_le_bytes());
put(out,at,&(self.representation.len() as u32).to_le_bytes());put(out,at,self.representation.as_bytes());
put(out,at,&self.utool.to_le_bytes());
put(out,at,&self.uframe.to_le_bytes());
put(out,at,&self.front.to_le_bytes());
put(out,at,&self.up.to_le_bytes());
put(out,at,&self.left.to_le_bytes());
put(out,at,&self.flip.to_le_bytes());
put(out,at,&self.turn4.to_le_bytes());
put(out,at,&self.turn5.to_le_bytes());
put(out,at,&self.turn6.to_le_bytes());
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
put(out,at,&self.p.to_le_bytes());
put(out,at,&self.r.to_le_bytes());
put(out,at,&self.ext1.to_le_bytes());
put(out,at,&self.ext2.to_le_bytes());
put(out,at,&self.ext3.to_le_bytes());
put(out,at,&self.j1.to_le_bytes());
put(out,at,&self.j2.to_le_bytes());
put(out,at,&self.j3.to_le_bytes());
put(out,at,&self.j4.to_le_bytes());
put(out,at,&self.j5.to_le_bytes());
put(out,at,&self.j6.to_le_bytes());
put(out,at,&self.j7.to_le_bytes());
put(out,at,&self.j8.to_le_bytes());
put(out,at,&self.j9.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.index=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.representation=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.up=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.left=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.flip=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn5=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.p=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.r=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.j9=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPosReg_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>599{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<87{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetPosReg_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetPosReg_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSetPosReg_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetPosReg_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fb6f889d30c921df31f55cdc760346eb0d655816e71b52d4aac7a1e56d4e9cb2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetPosReg_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUFrameData_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUFrameData_Request {
  /// Declared uframe_number; canonical bounds are part of CODEC_IDENTITY.
  pub uframe_number:u8,
  /// Declared frame; canonical bounds are part of CODEC_IDENTITY.
  pub frame:Ros2Fanuc_msgsMsgFrame,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUFrameData_Request {fn default()->Self {Self {
uframe_number:Default::default(),
frame:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUFrameData_Request {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a067808d8fe5ca1418e18ac492313ad9211cc8f3bf402ad0ac51a2850a2bc169";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,self.frame.wire_size()?)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.uframe_number.to_le_bytes());
self.frame.write_fields(out,at);
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe_number=decoded;
let decoded=Ros2Fanuc_msgsMsgFrame::read_fields(bytes,at)?;
value.frame=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUFrameData_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUFrameData_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUFrameData_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUFrameData_Response {fn default()->Self {Self {
result:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUFrameData_Response {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f436e4fe5bd06b048f4f53d2df4933ac8eac5a784f31b42ccf13f5eaf1696eee";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUFrameData_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUFrameUTool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUFrameUTool_Request {
  /// Declared uframe_number; canonical bounds are part of CODEC_IDENTITY.
  pub uframe_number:u8,
  /// Declared utool_number; canonical bounds are part of CODEC_IDENTITY.
  pub utool_number:u8,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUFrameUTool_Request {fn default()->Self {Self {
uframe_number:Default::default(),
utool_number:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUFrameUTool_Request {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0b86b0a37e4dc23e0ff651df867f5958e7440bf4712b47779d69ca699af4c09f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.uframe_number.to_le_bytes());
put(out,at,&self.utool_number.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool_number=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUFrameUTool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUFrameUTool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUFrameUTool_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUFrameUTool_Response {fn default()->Self {Self {
result:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUFrameUTool_Response {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b77837445c9663f29f23465cc7178a183d245486f74bcff732fc2f4025b82a67";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUFrameUTool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUToolData_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUToolData_Request {
  /// Declared utool_number; canonical bounds are part of CODEC_IDENTITY.
  pub utool_number:u8,
  /// Declared frame; canonical bounds are part of CODEC_IDENTITY.
  pub frame:Ros2Fanuc_msgsMsgFrame,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUToolData_Request {fn default()->Self {Self {
utool_number:Default::default(),
frame:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUToolData_Request {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7d5b911133645744c8def79bc16c8b7c4ee4e87a057ea33741031d6a94e620ab";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,self.frame.wire_size()?)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.utool_number.to_le_bytes());
self.frame.write_fields(out,at);
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool_number=decoded;
let decoded=Ros2Fanuc_msgsMsgFrame::read_fields(bytes,at)?;
value.frame=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUToolData_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SetUToolData_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSetUToolData_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared group; canonical bounds are part of CODEC_IDENTITY.
  pub group:u8,
}
impl Default for Ros2Fanuc_msgsSrvSetUToolData_Response {fn default()->Self {Self {
result:Default::default(),
group:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSetUToolData_Response {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b33d265f275dfcb5fe20432d5e38b55574c85260135bded477c2d5fa2c85c7cb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.group.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSetUToolData_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SwitchControlState_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSwitchControlState_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:u32,
}
impl Default for Ros2Fanuc_msgsSrvSwitchControlState_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSwitchControlState_Request {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b485b7071fe2737c7dc8f0360a15b7be30f997430142d8260454fd423172cc21";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.status.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSwitchControlState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 fanuc_msgs/srv/SwitchControlState_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Fanuc_msgsSrvSwitchControlState_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Fanuc_msgsSrvSwitchControlState_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Fanuc_msgsSrvSwitchControlState_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8a3d7a3c1037174f4118843e34222ef8811eff3cc885a0c688d98071e1a9496c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Fanuc_msgsSrvSwitchControlState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/CallParam.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgCallParam {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
  /// Declared int_value; canonical bounds are part of CODEC_IDENTITY.
  pub int_value:i32,
  /// Declared float_value; canonical bounds are part of CODEC_IDENTITY.
  pub float_value:f32,
  /// Declared string_value; canonical bounds are part of CODEC_IDENTITY.
  pub string_value:String,
}
impl Default for Ros2Rmi_msgsMsgCallParam {fn default()->Self {Self {
r#type:Default::default(),
int_value:Default::default(),
float_value:Default::default(),
string_value:Default::default(),
}}}
impl Ros2Rmi_msgsMsgCallParam {
  pub const MAX_WIRE_BYTES:usize=592;
  pub const CARTESIAN_WIRE_BYTES:u64=592;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ed625d80db6c4314b8373f2d205344d6ab8d71be46182dab5a497d36564a2255";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
n=add_size(n,4)?;
if !self.float_value.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if self.string_value.len()>512||self.string_value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.string_value.len())?;
if n>592{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
put(out,at,&self.int_value.to_le_bytes());
put(out,at,&self.float_value.to_le_bytes());
put(out,at,&(self.string_value.len() as u32).to_le_bytes());put(out,at,self.string_value.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.int_value=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.float_value=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.string_value=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgCallParam {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>592{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/Cartesian.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgCartesian {
  /// Declared utool; canonical bounds are part of CODEC_IDENTITY.
  pub utool:i8,
  /// Declared uframe; canonical bounds are part of CODEC_IDENTITY.
  pub uframe:i8,
  /// Declared front; canonical bounds are part of CODEC_IDENTITY.
  pub front:u8,
  /// Declared up; canonical bounds are part of CODEC_IDENTITY.
  pub up:u8,
  /// Declared left; canonical bounds are part of CODEC_IDENTITY.
  pub left:u8,
  /// Declared flip; canonical bounds are part of CODEC_IDENTITY.
  pub flip:u8,
  /// Declared turn4; canonical bounds are part of CODEC_IDENTITY.
  pub turn4:i8,
  /// Declared turn5; canonical bounds are part of CODEC_IDENTITY.
  pub turn5:i8,
  /// Declared turn6; canonical bounds are part of CODEC_IDENTITY.
  pub turn6:i8,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f32,
  /// Declared p; canonical bounds are part of CODEC_IDENTITY.
  pub p:f32,
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:f32,
  /// Declared ext1; canonical bounds are part of CODEC_IDENTITY.
  pub ext1:f32,
  /// Declared ext2; canonical bounds are part of CODEC_IDENTITY.
  pub ext2:f32,
  /// Declared ext3; canonical bounds are part of CODEC_IDENTITY.
  pub ext3:f32,
}
impl Default for Ros2Rmi_msgsMsgCartesian {fn default()->Self {Self {
utool:1,
uframe:0,
front:1,
up:1,
left:0,
flip:0,
turn4:0,
turn5:0,
turn6:0,
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
p:Default::default(),
r:Default::default(),
ext1:Default::default(),
ext2:Default::default(),
ext3:Default::default(),
}}}
impl Ros2Rmi_msgsMsgCartesian {
  pub const MAX_WIRE_BYTES:usize=45;
  pub const CARTESIAN_WIRE_BYTES:u64=45;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2a0e1fff119007499a63d55c9722f666be1436a9212d9e43b556af347e8e4b40";
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
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.w.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.p.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.r.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ext3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>45{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.utool.to_le_bytes());
put(out,at,&self.uframe.to_le_bytes());
put(out,at,&self.front.to_le_bytes());
put(out,at,&self.up.to_le_bytes());
put(out,at,&self.left.to_le_bytes());
put(out,at,&self.flip.to_le_bytes());
put(out,at,&self.turn4.to_le_bytes());
put(out,at,&self.turn5.to_le_bytes());
put(out,at,&self.turn6.to_le_bytes());
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.w.to_le_bytes());
put(out,at,&self.p.to_le_bytes());
put(out,at,&self.r.to_le_bytes());
put(out,at,&self.ext1.to_le_bytes());
put(out,at,&self.ext2.to_le_bytes());
put(out,at,&self.ext3.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.utool=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.uframe=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.up=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.left=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.flip=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn5=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.turn6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.w=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.p=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.r=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ext3=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgCartesian {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>45{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<45{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/Command.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgCommand {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
}
impl Default for Ros2Rmi_msgsMsgCommand {fn default()->Self {Self {
r#type:Default::default(),
}}}
impl Ros2Rmi_msgsMsgCommand {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d2741856b8cdf57aa716566edfc705957611557b7da5b6099b173903bdd9960c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgCommand {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/CommandOptions.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgCommandOptions {
  /// Declared use_group_mask; canonical bounds are part of CODEC_IDENTITY.
  pub use_group_mask:bool,
  /// Declared group_mask; canonical bounds are part of CODEC_IDENTITY.
  pub group_mask:u8,
  /// Declared use_rtsa; canonical bounds are part of CODEC_IDENTITY.
  pub use_rtsa:bool,
  /// Declared rtsa; canonical bounds are part of CODEC_IDENTITY.
  pub rtsa:String,
  /// Declared use_pltzmode; canonical bounds are part of CODEC_IDENTITY.
  pub use_pltzmode:bool,
  /// Declared pltzmode; canonical bounds are part of CODEC_IDENTITY.
  pub pltzmode:String,
}
impl Default for Ros2Rmi_msgsMsgCommandOptions {fn default()->Self {Self {
use_group_mask:false,
group_mask:1,
use_rtsa:false,
rtsa:String::from("ON"),
use_pltzmode:false,
pltzmode:String::from("ZERODN"),
}}}
impl Ros2Rmi_msgsMsgCommandOptions {
  pub const MAX_WIRE_BYTES:usize=1036;
  pub const CARTESIAN_WIRE_BYTES:u64=1036;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0be0c508035725bf9686807cb8b5267e23f709b7aa34b0910a44ff3604670147";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if self.rtsa.len()>512||self.rtsa.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.rtsa.len())?;
n=add_size(n,1)?;
if self.pltzmode.len()>512||self.pltzmode.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.pltzmode.len())?;
if n>1036{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.use_group_mask)]);
put(out,at,&self.group_mask.to_le_bytes());
put(out,at,&[u8::from(self.use_rtsa)]);
put(out,at,&(self.rtsa.len() as u32).to_le_bytes());put(out,at,self.rtsa.as_bytes());
put(out,at,&[u8::from(self.use_pltzmode)]);
put(out,at,&(self.pltzmode.len() as u32).to_le_bytes());put(out,at,self.pltzmode.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_group_mask=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.group_mask=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_rtsa=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.rtsa=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_pltzmode=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.pltzmode=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgCommandOptions {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1036{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/Joint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgJoint {
  /// Declared positions; canonical bounds are part of CODEC_IDENTITY.
  pub positions:[f32;9],
}
impl Default for Ros2Rmi_msgsMsgJoint {fn default()->Self {Self {
positions:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Rmi_msgsMsgJoint {
  pub const MAX_WIRE_BYTES:usize=36;
  pub const CARTESIAN_WIRE_BYTES:u64=36;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0ba90adf6cc287bc306fd2a77c52c2e4d62731cfac37bb2cc116ee846249a42d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.positions {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>36{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.positions {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.positions[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgJoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>36{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/MotionInstructionOptions.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgMotionInstructionOptions {
  /// Declared use_acc; canonical bounds are part of CODEC_IDENTITY.
  pub use_acc:bool,
  /// Declared acc; canonical bounds are part of CODEC_IDENTITY.
  pub acc:u8,
  /// Declared use_offset_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub use_offset_pr_number:bool,
  /// Declared offset_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub offset_pr_number:u16,
  /// Declared use_vision_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub use_vision_pr_number:bool,
  /// Declared vision_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub vision_pr_number:u16,
  /// Declared use_wrist_joint; canonical bounds are part of CODEC_IDENTITY.
  pub use_wrist_joint:bool,
  /// Declared wrist_joint; canonical bounds are part of CODEC_IDENTITY.
  pub wrist_joint:String,
  /// Declared use_mrot; canonical bounds are part of CODEC_IDENTITY.
  pub use_mrot:bool,
  /// Declared mrot; canonical bounds are part of CODEC_IDENTITY.
  pub mrot:String,
  /// Declared use_lcb_type; canonical bounds are part of CODEC_IDENTITY.
  pub use_lcb_type:bool,
  /// Declared lcb_type; canonical bounds are part of CODEC_IDENTITY.
  pub lcb_type:String,
  /// Declared use_lcb_value; canonical bounds are part of CODEC_IDENTITY.
  pub use_lcb_value:bool,
  /// Declared lcb_value; canonical bounds are part of CODEC_IDENTITY.
  pub lcb_value:u16,
  /// Declared use_port_type; canonical bounds are part of CODEC_IDENTITY.
  pub use_port_type:bool,
  /// Declared port_type; canonical bounds are part of CODEC_IDENTITY.
  pub port_type:u8,
  /// Declared use_port_number; canonical bounds are part of CODEC_IDENTITY.
  pub use_port_number:bool,
  /// Declared port_number; canonical bounds are part of CODEC_IDENTITY.
  pub port_number:u16,
  /// Declared use_port_value; canonical bounds are part of CODEC_IDENTITY.
  pub use_port_value:bool,
  /// Declared port_value; canonical bounds are part of CODEC_IDENTITY.
  pub port_value:String,
  /// Declared use_tool_offset_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub use_tool_offset_pr_number:bool,
  /// Declared tool_offset_pr_number; canonical bounds are part of CODEC_IDENTITY.
  pub tool_offset_pr_number:u16,
  /// Declared use_alim; canonical bounds are part of CODEC_IDENTITY.
  pub use_alim:bool,
  /// Declared alim; canonical bounds are part of CODEC_IDENTITY.
  pub alim:i32,
  /// Declared use_alim_reg; canonical bounds are part of CODEC_IDENTITY.
  pub use_alim_reg:bool,
  /// Declared alim_reg; canonical bounds are part of CODEC_IDENTITY.
  pub alim_reg:u16,
  /// Declared use_no_blend; canonical bounds are part of CODEC_IDENTITY.
  pub use_no_blend:bool,
  /// Declared no_blend; canonical bounds are part of CODEC_IDENTITY.
  pub no_blend:String,
}
impl Default for Ros2Rmi_msgsMsgMotionInstructionOptions {fn default()->Self {Self {
use_acc:false,
acc:Default::default(),
use_offset_pr_number:false,
offset_pr_number:Default::default(),
use_vision_pr_number:false,
vision_pr_number:Default::default(),
use_wrist_joint:false,
wrist_joint:String::from("ON"),
use_mrot:false,
mrot:String::from("ON"),
use_lcb_type:false,
lcb_type:Default::default(),
use_lcb_value:false,
lcb_value:Default::default(),
use_port_type:false,
port_type:Default::default(),
use_port_number:false,
port_number:Default::default(),
use_port_value:false,
port_value:Default::default(),
use_tool_offset_pr_number:false,
tool_offset_pr_number:Default::default(),
use_alim:false,
alim:Default::default(),
use_alim_reg:false,
alim_reg:Default::default(),
use_no_blend:false,
no_blend:String::from("ON"),
}}}
impl Ros2Rmi_msgsMsgMotionInstructionOptions {
  pub const MAX_WIRE_BYTES:usize=2612;
  pub const CARTESIAN_WIRE_BYTES:u64=2612;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f8d9ed2e083c577b0cae8965df71aa99643d12ce5a5a6ce558894edfb756bcc6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
if self.wrist_joint.len()>512||self.wrist_joint.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.wrist_joint.len())?;
n=add_size(n,1)?;
if self.mrot.len()>512||self.mrot.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.mrot.len())?;
n=add_size(n,1)?;
if self.lcb_type.len()>512||self.lcb_type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.lcb_type.len())?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
if self.port_value.len()>512||self.port_value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.port_value.len())?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
n=add_size(n,1)?;
if self.no_blend.len()>512||self.no_blend.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.no_blend.len())?;
if n>2612{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.use_acc)]);
put(out,at,&self.acc.to_le_bytes());
put(out,at,&[u8::from(self.use_offset_pr_number)]);
put(out,at,&self.offset_pr_number.to_le_bytes());
put(out,at,&[u8::from(self.use_vision_pr_number)]);
put(out,at,&self.vision_pr_number.to_le_bytes());
put(out,at,&[u8::from(self.use_wrist_joint)]);
put(out,at,&(self.wrist_joint.len() as u32).to_le_bytes());put(out,at,self.wrist_joint.as_bytes());
put(out,at,&[u8::from(self.use_mrot)]);
put(out,at,&(self.mrot.len() as u32).to_le_bytes());put(out,at,self.mrot.as_bytes());
put(out,at,&[u8::from(self.use_lcb_type)]);
put(out,at,&(self.lcb_type.len() as u32).to_le_bytes());put(out,at,self.lcb_type.as_bytes());
put(out,at,&[u8::from(self.use_lcb_value)]);
put(out,at,&self.lcb_value.to_le_bytes());
put(out,at,&[u8::from(self.use_port_type)]);
put(out,at,&self.port_type.to_le_bytes());
put(out,at,&[u8::from(self.use_port_number)]);
put(out,at,&self.port_number.to_le_bytes());
put(out,at,&[u8::from(self.use_port_value)]);
put(out,at,&(self.port_value.len() as u32).to_le_bytes());put(out,at,self.port_value.as_bytes());
put(out,at,&[u8::from(self.use_tool_offset_pr_number)]);
put(out,at,&self.tool_offset_pr_number.to_le_bytes());
put(out,at,&[u8::from(self.use_alim)]);
put(out,at,&self.alim.to_le_bytes());
put(out,at,&[u8::from(self.use_alim_reg)]);
put(out,at,&self.alim_reg.to_le_bytes());
put(out,at,&[u8::from(self.use_no_blend)]);
put(out,at,&(self.no_blend.len() as u32).to_le_bytes());put(out,at,self.no_blend.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_acc=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.acc=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_offset_pr_number=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.offset_pr_number=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_vision_pr_number=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.vision_pr_number=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_wrist_joint=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.wrist_joint=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_mrot=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.mrot=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_lcb_type=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.lcb_type=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_lcb_value=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.lcb_value=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_port_type=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.port_type=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_port_number=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.port_number=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_port_value=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.port_value=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_tool_offset_pr_number=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.tool_offset_pr_number=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_alim=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.alim=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_alim_reg=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.alim_reg=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.use_no_blend=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.no_blend=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgMotionInstructionOptions {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2612{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<52{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/MotionType.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgMotionType {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
}
impl Default for Ros2Rmi_msgsMsgMotionType {fn default()->Self {Self {
r#type:String::from("J"),
}}}
impl Ros2Rmi_msgsMsgMotionType {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c51cc7a32f6853d13bedaf3306dd79e445de6c0d16ae0ec39034ce0ae6d19c8a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgMotionType {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/Representation.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgRepresentation {
  /// Declared representation; canonical bounds are part of CODEC_IDENTITY.
  pub representation:String,
}
impl Default for Ros2Rmi_msgsMsgRepresentation {fn default()->Self {Self {
representation:String::from("JOINT"),
}}}
impl Ros2Rmi_msgsMsgRepresentation {
  pub const MAX_WIRE_BYTES:usize=516;
  pub const CARTESIAN_WIRE_BYTES:u64=516;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b439506f2dc2135bb484b3abb308dfdf5c07367af5a4d6d0f5435ad9bba8ea6f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.representation.len()>512||self.representation.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.representation.len())?;
if n>516{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.representation.len() as u32).to_le_bytes());put(out,at,self.representation.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.representation=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgRepresentation {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>516{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/SpeedType.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgSpeedType {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
}
impl Default for Ros2Rmi_msgsMsgSpeedType {fn default()->Self {Self {
r#type:String::from("Percent"),
}}}
impl Ros2Rmi_msgsMsgSpeedType {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e4f4dbda84d8d8cd4d026b0beb52005663c7c4ee602c8d1e4d5853589777c282";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgSpeedType {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/msg/TermType.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsMsgTermType {
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:String,
}
impl Default for Ros2Rmi_msgsMsgTermType {fn default()->Self {Self {
r#type:String::from("FINE"),
}}}
impl Ros2Rmi_msgsMsgTermType {
  pub const MAX_WIRE_BYTES:usize=68;
  pub const CARTESIAN_WIRE_BYTES:u64=68;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5ddbb483126e0ab5799ed865480da457179d83a271581c1471aec940bedc4535";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.r#type.len()>64||self.r#type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.r#type.len())?;
if n>68{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.r#type.len() as u32).to_le_bytes());put(out,at,self.r#type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.r#type=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsMsgTermType {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>68{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddCall_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddCall_Request {
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared params; canonical bounds are part of CODEC_IDENTITY.
  pub params:Vec<Ros2Rmi_msgsMsgCallParam>,
}
impl Default for Ros2Rmi_msgsSrvAddCall_Request {fn default()->Self {Self {
program_name:Default::default(),
params:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddCall_Request {
  pub const MAX_WIRE_BYTES:usize=9992;
  pub const CARTESIAN_WIRE_BYTES:u64=9992;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d652cfbaef29487a0a688932ee926d7a4026c5e7ac8a0eace583b503c37bcd8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.program_name.len()>512||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
if self.params.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.params {
n=add_size(n,element.wire_size()?)?;
}
if n>9992{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&(self.params.len() as u32).to_le_bytes());
for element in &self.params {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.params=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Rmi_msgsMsgCallParam::read_fields(bytes,at)?;
value.params.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddCall_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9992{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddCall_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddCall_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddCall_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddCall_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4f8495dfc3c69ec640444fd24843a7bb3e30008d90945bdd64817d8acb7964e1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddCall_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddMotionInstruction_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddMotionInstruction_Request {
  /// Declared motion_type; canonical bounds are part of CODEC_IDENTITY.
  pub motion_type:Ros2Rmi_msgsMsgMotionType,
  /// Declared incremental; canonical bounds are part of CODEC_IDENTITY.
  pub incremental:bool,
  /// Declared representation; canonical bounds are part of CODEC_IDENTITY.
  pub representation:Ros2Rmi_msgsMsgRepresentation,
  /// Declared joint; canonical bounds are part of CODEC_IDENTITY.
  pub joint:Ros2Rmi_msgsMsgJoint,
  /// Declared cartesian; canonical bounds are part of CODEC_IDENTITY.
  pub cartesian:Ros2Rmi_msgsMsgCartesian,
  /// Declared speed_type; canonical bounds are part of CODEC_IDENTITY.
  pub speed_type:Ros2Rmi_msgsMsgSpeedType,
  /// Declared speed_value; canonical bounds are part of CODEC_IDENTITY.
  pub speed_value:u16,
  /// Declared term_type; canonical bounds are part of CODEC_IDENTITY.
  pub term_type:Ros2Rmi_msgsMsgTermType,
  /// Declared term_value; canonical bounds are part of CODEC_IDENTITY.
  pub term_value:u8,
  /// Declared via_representation; canonical bounds are part of CODEC_IDENTITY.
  pub via_representation:Ros2Rmi_msgsMsgRepresentation,
  /// Declared via_joint; canonical bounds are part of CODEC_IDENTITY.
  pub via_joint:Ros2Rmi_msgsMsgJoint,
  /// Declared via_cartesian; canonical bounds are part of CODEC_IDENTITY.
  pub via_cartesian:Ros2Rmi_msgsMsgCartesian,
  /// Declared options; canonical bounds are part of CODEC_IDENTITY.
  pub options:Ros2Rmi_msgsMsgMotionInstructionOptions,
}
impl Default for Ros2Rmi_msgsSrvAddMotionInstruction_Request {fn default()->Self {Self {
motion_type:Default::default(),
incremental:false,
representation:Default::default(),
joint:Default::default(),
cartesian:Default::default(),
speed_type:Default::default(),
speed_value:Default::default(),
term_type:Default::default(),
term_value:Default::default(),
via_representation:Default::default(),
via_joint:Default::default(),
via_cartesian:Default::default(),
options:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddMotionInstruction_Request {
  pub const MAX_WIRE_BYTES:usize=4014;
  pub const CARTESIAN_WIRE_BYTES:u64=4014;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f22c49cb2d32e6a8f3954f3d3cbf0021c87e0363b1f2ded4ac20a97a5b59391";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.motion_type.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,self.representation.wire_size()?)?;
n=add_size(n,self.joint.wire_size()?)?;
n=add_size(n,self.cartesian.wire_size()?)?;
n=add_size(n,self.speed_type.wire_size()?)?;
n=add_size(n,2)?;
n=add_size(n,self.term_type.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,self.via_representation.wire_size()?)?;
n=add_size(n,self.via_joint.wire_size()?)?;
n=add_size(n,self.via_cartesian.wire_size()?)?;
n=add_size(n,self.options.wire_size()?)?;
if n>4014{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.motion_type.write_fields(out,at);
put(out,at,&[u8::from(self.incremental)]);
self.representation.write_fields(out,at);
self.joint.write_fields(out,at);
self.cartesian.write_fields(out,at);
self.speed_type.write_fields(out,at);
put(out,at,&self.speed_value.to_le_bytes());
self.term_type.write_fields(out,at);
put(out,at,&self.term_value.to_le_bytes());
self.via_representation.write_fields(out,at);
self.via_joint.write_fields(out,at);
self.via_cartesian.write_fields(out,at);
self.options.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Rmi_msgsMsgMotionType::read_fields(bytes,at)?;
value.motion_type=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.incremental=decoded;
let decoded=Ros2Rmi_msgsMsgRepresentation::read_fields(bytes,at)?;
value.representation=decoded;
let decoded=Ros2Rmi_msgsMsgJoint::read_fields(bytes,at)?;
value.joint=decoded;
let decoded=Ros2Rmi_msgsMsgCartesian::read_fields(bytes,at)?;
value.cartesian=decoded;
let decoded=Ros2Rmi_msgsMsgSpeedType::read_fields(bytes,at)?;
value.speed_type=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.speed_value=decoded;
let decoded=Ros2Rmi_msgsMsgTermType::read_fields(bytes,at)?;
value.term_type=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.term_value=decoded;
let decoded=Ros2Rmi_msgsMsgRepresentation::read_fields(bytes,at)?;
value.via_representation=decoded;
let decoded=Ros2Rmi_msgsMsgJoint::read_fields(bytes,at)?;
value.via_joint=decoded;
let decoded=Ros2Rmi_msgsMsgCartesian::read_fields(bytes,at)?;
value.via_cartesian=decoded;
let decoded=Ros2Rmi_msgsMsgMotionInstructionOptions::read_fields(bytes,at)?;
value.options=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddMotionInstruction_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4014{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<238{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddMotionInstruction_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddMotionInstruction_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddMotionInstruction_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddMotionInstruction_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_79fee3e965fda63c3a53ad2178ff48614878acc9f6af74cc27a8c2812b68ac6b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddMotionInstruction_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetPayload_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetPayload_Request {
  /// Declared schedule_number; canonical bounds are part of CODEC_IDENTITY.
  pub schedule_number:u8,
}
impl Default for Ros2Rmi_msgsSrvAddSetPayload_Request {fn default()->Self {Self {
schedule_number:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetPayload_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_45da9f81e8c26f49490a4e6eba5c8be0ee67285124540eb1c3e1f2c3e0fd821b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.schedule_number.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.schedule_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetPayload_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetPayload_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetPayload_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddSetPayload_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetPayload_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bfc5bc7b92f292c2980496cb18fbe953ea1b49d45f7950f0125bbbf880bdf72a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetPayload_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetUFrame_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetUFrame_Request {
  /// Declared frame_number; canonical bounds are part of CODEC_IDENTITY.
  pub frame_number:u8,
}
impl Default for Ros2Rmi_msgsSrvAddSetUFrame_Request {fn default()->Self {Self {
frame_number:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetUFrame_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_64c83dd2be89b4b6dcf3b3c2ea70184971e6a359914ff1a5cd26d4b114d35475";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.frame_number.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frame_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetUFrame_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetUFrame_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetUFrame_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddSetUFrame_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetUFrame_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e344ef6efacc8d569111d1c6ef4028e1a5bb54af49d3e8f486ae89b83c674647";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetUFrame_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetUTool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetUTool_Request {
  /// Declared tool_number; canonical bounds are part of CODEC_IDENTITY.
  pub tool_number:u8,
}
impl Default for Ros2Rmi_msgsSrvAddSetUTool_Request {fn default()->Self {Self {
tool_number:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetUTool_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d1327022326cd41caed6001449517a41f589e148c38d6db30923922175f05c46";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.tool_number.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.tool_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetUTool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddSetUTool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddSetUTool_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddSetUTool_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddSetUTool_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dd4c36458027b87559200cf5f6aa72a60af2d103f663f2fa9daca47ce8020ff6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddSetUTool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddWaitDIN_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddWaitDIN_Request {
  /// Declared port_number; canonical bounds are part of CODEC_IDENTITY.
  pub port_number:u16,
  /// Declared port_value; canonical bounds are part of CODEC_IDENTITY.
  pub port_value:String,
}
impl Default for Ros2Rmi_msgsSrvAddWaitDIN_Request {fn default()->Self {Self {
port_number:Default::default(),
port_value:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddWaitDIN_Request {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c32214f1ff1b0b6980583fc3088f412e856efb5eaa9c336cf798ef8ad85c61ab";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if self.port_value.len()>512||self.port_value.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.port_value.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.port_number.to_le_bytes());
put(out,at,&(self.port_value.len() as u32).to_le_bytes());put(out,at,self.port_value.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.port_number=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.port_value=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddWaitDIN_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddWaitDIN_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddWaitDIN_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddWaitDIN_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddWaitDIN_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7d3933f573619ba49f19df7c11f1d73ef211c17581f660f8c0fbcc152cd519d4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddWaitDIN_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddWaitTime_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddWaitTime_Request {
  /// Declared time; canonical bounds are part of CODEC_IDENTITY.
  pub time:f32,
}
impl Default for Ros2Rmi_msgsSrvAddWaitTime_Request {fn default()->Self {Self {
time:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddWaitTime_Request {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8487f4b7dcfb044c72f7fb62a31ffe9cdc4321a9ebdaa0550a363e32acbadc46";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.time.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.time.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.time=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddWaitTime_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/AddWaitTime_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvAddWaitTime_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
  /// Declared sequence_id; canonical bounds are part of CODEC_IDENTITY.
  pub sequence_id:i32,
}
impl Default for Ros2Rmi_msgsSrvAddWaitTime_Response {fn default()->Self {Self {
result:Default::default(),
sequence_id:Default::default(),
}}}
impl Ros2Rmi_msgsSrvAddWaitTime_Response {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f9578c08b971f32f91d1c94b8713cafe648e25147394e20c85bb335254fa20c8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
put(out,at,&self.sequence_id.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sequence_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvAddWaitTime_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/CallCommand_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvCallCommand_Request {
  /// Declared command; canonical bounds are part of CODEC_IDENTITY.
  pub command:Ros2Rmi_msgsMsgCommand,
  /// Declared options; canonical bounds are part of CODEC_IDENTITY.
  pub options:Ros2Rmi_msgsMsgCommandOptions,
}
impl Default for Ros2Rmi_msgsSrvCallCommand_Request {fn default()->Self {Self {
command:Default::default(),
options:Default::default(),
}}}
impl Ros2Rmi_msgsSrvCallCommand_Request {
  pub const MAX_WIRE_BYTES:usize=1104;
  pub const CARTESIAN_WIRE_BYTES:u64=1104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d1f890a2beec0ca9294ff4f641d1738855d576a917ed05eb34517886c73f381c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.command.wire_size()?)?;
n=add_size(n,self.options.wire_size()?)?;
if n>1104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.command.write_fields(out,at);
self.options.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Rmi_msgsMsgCommand::read_fields(bytes,at)?;
value.command=decoded;
let decoded=Ros2Rmi_msgsMsgCommandOptions::read_fields(bytes,at)?;
value.options=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvCallCommand_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 rmi_msgs/srv/CallCommand_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Rmi_msgsSrvCallCommand_Response {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:i32,
}
impl Default for Ros2Rmi_msgsSrvCallCommand_Response {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Rmi_msgsSrvCallCommand_Response {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_64d92e6fadc651073469318ad15d2e98ec3d059c629838cefc5820b17ab6f1bc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Rmi_msgsSrvCallCommand_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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

impl Ros2Fanuc_msgsMsgIOType {pub const AI:&'static str="AI";
pub const AO:&'static str="AO";
pub const DI:&'static str="DI";
pub const DO:&'static str="DO";
pub const RI:&'static str="RI";
pub const RO:&'static str="RO";
pub const SI:&'static str="SI";
pub const SO:&'static str="SO";
pub const WI:&'static str="WI";
pub const WO:&'static str="WO";
pub const UI:&'static str="UI";
pub const UO:&'static str="UO";
pub const WSI:&'static str="WSI";
pub const WSO:&'static str="WSO";
pub const F:&'static str="F";
pub const M:&'static str="M";
pub const GI:&'static str="GI";
pub const GO:&'static str="GO";}

impl Ros2Fanuc_msgsMsgRobotStatus {pub const CONTACT_STOP_MODE_NONE:u8=0;
pub const CONTACT_STOP_MODE_SAFE:u8=1;
pub const CONTACT_STOP_MODE_STOP:u8=2;
pub const CONTACT_STOP_MODE_DSBL:u8=3;
pub const CONTACT_STOP_MODE_ESCP:u8=4;}

impl Ros2Fanuc_msgsSrvSwitchControlState_Request {pub const STOP:u32=0;
pub const START:u32=1;}

impl Ros2Rmi_msgsMsgCallParam {pub const AR:&'static str="AR";
pub const PR:&'static str="PR";
pub const SR:&'static str="SR";
pub const R:&'static str="R";
pub const P:&'static str="P";
pub const INT:&'static str="INT";
pub const INTEGER:&'static str="INT";
pub const FLOAT:&'static str="FLOAT";
pub const STRING:&'static str="String";
pub const STR:&'static str="String";}

impl Ros2Rmi_msgsMsgCommand {pub const INITIAZLIE:&'static str="INITIALIZE";
pub const INIT:&'static str="INITIALIZE";
pub const ABORT:&'static str="ABORT";
pub const PAUSE:&'static str="PAUSE";
pub const RESUME:&'static str="RESUME";}

impl Ros2Rmi_msgsMsgCommandOptions {pub const RTSA_ON:&'static str="ON";
pub const RTSA_OFF:&'static str="OFF";
pub const PLTZ_ZERODN:&'static str="ZERODN";
pub const PLTZ_ZEROUP:&'static str="ZEROUP";
pub const PLTZ_PSPIDN:&'static str="PSPIDN";
pub const PLTZ_PSPIUP:&'static str="PSPIUP";
pub const PLTZ_MSPIDN:&'static str="MSPIDN";
pub const PLTZ_MSPIUP:&'static str="MSPIUP";}

impl Ros2Rmi_msgsMsgMotionInstructionOptions {pub const LCB_TYPE_TB:&'static str="TB";
pub const LCB_TYPE_TA:&'static str="TA";
pub const LCB_TYPE_DB:&'static str="DB";
pub const PORT_TYPE_DOUT:u8=1;
pub const PORT_TYPE_DO:u8=1;
pub const PORT_TYPE_ROUT:u8=2;
pub const PORT_TYPE_RO:u8=2;}

impl Ros2Rmi_msgsMsgMotionType {pub const J:&'static str="J";
pub const JOINT:&'static str="J";
pub const L:&'static str="L";
pub const LINEAR:&'static str="L";
pub const C:&'static str="C";
pub const CIRCULAR:&'static str="C";
pub const S:&'static str="S";
pub const SPLINE:&'static str="S";}

impl Ros2Rmi_msgsMsgRepresentation {pub const JOINT:&'static str="JOINT";
pub const J:&'static str="JOINT";
pub const CARTESIAN:&'static str="CARTESIAN";
pub const CART:&'static str="CARTESIAN";
pub const C:&'static str="CARTESIAN";}

impl Ros2Rmi_msgsMsgSpeedType {pub const PERCENT:&'static str="Percent";
pub const PERCENTAGE:&'static str="Percent";
pub const MMSEC:&'static str="mmSec";
pub const INCHMIN:&'static str="InchMin";
pub const TIME:&'static str="Time";
pub const SECONDS:&'static str="Time";
pub const SEC:&'static str="Time";
pub const MSEC:&'static str="mSec";}

impl Ros2Rmi_msgsMsgTermType {pub const FINE:&'static str="FINE";
pub const CNT:&'static str="CNT";
pub const CR:&'static str="CR";
pub const CD:&'static str="CD";}
