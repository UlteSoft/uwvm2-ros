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
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/FRIState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgFRIState {
  /// Declared session_state; canonical bounds are part of CODEC_IDENTITY.
  pub session_state:i32,
  /// Declared connection_quality; canonical bounds are part of CODEC_IDENTITY.
  pub connection_quality:i32,
  /// Declared safety_state; canonical bounds are part of CODEC_IDENTITY.
  pub safety_state:i32,
  /// Declared command_mode; canonical bounds are part of CODEC_IDENTITY.
  pub command_mode:i32,
  /// Declared control_mode; canonical bounds are part of CODEC_IDENTITY.
  pub control_mode:i32,
  /// Declared operation_mode; canonical bounds are part of CODEC_IDENTITY.
  pub operation_mode:i32,
  /// Declared drive_state; canonical bounds are part of CODEC_IDENTITY.
  pub drive_state:i32,
  /// Declared overlay_type; canonical bounds are part of CODEC_IDENTITY.
  pub overlay_type:i32,
  /// Declared tracking_performance; canonical bounds are part of CODEC_IDENTITY.
  pub tracking_performance:f64,
}
impl Default for Ros2Kuka_driver_interfacesMsgFRIState {fn default()->Self {Self {
session_state:Default::default(),
connection_quality:Default::default(),
safety_state:Default::default(),
command_mode:Default::default(),
control_mode:Default::default(),
operation_mode:Default::default(),
drive_state:Default::default(),
overlay_type:Default::default(),
tracking_performance:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgFRIState {
  pub const MAX_WIRE_BYTES:usize=40;
  pub const CARTESIAN_WIRE_BYTES:u64=40;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_27be8431e4a8cda3bb4d6d5a8a69fd86a0d07a1d920799c232d63a4941ee6716";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if !self.tracking_performance.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>40{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.session_state.to_le_bytes());
put(out,at,&self.connection_quality.to_le_bytes());
put(out,at,&self.safety_state.to_le_bytes());
put(out,at,&self.command_mode.to_le_bytes());
put(out,at,&self.control_mode.to_le_bytes());
put(out,at,&self.operation_mode.to_le_bytes());
put(out,at,&self.drive_state.to_le_bytes());
put(out,at,&self.overlay_type.to_le_bytes());
put(out,at,&self.tracking_performance.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.session_state=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.connection_quality=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.safety_state=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.command_mode=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.control_mode=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.operation_mode=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.drive_state=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.overlay_type=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tracking_performance=decoded;
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgFRIState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>40{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/FRIStateArray.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgFRIStateArray {
  /// Declared robot_names; canonical bounds are part of CODEC_IDENTITY.
  pub robot_names:Vec<String>,
  /// Declared states; canonical bounds are part of CODEC_IDENTITY.
  pub states:Vec<Ros2Kuka_driver_interfacesMsgFRIState>,
}
impl Default for Ros2Kuka_driver_interfacesMsgFRIStateArray {fn default()->Self {Self {
robot_names:Default::default(),
states:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgFRIStateArray {
  pub const MAX_WIRE_BYTES:usize=1736;
  pub const CARTESIAN_WIRE_BYTES:u64=1736;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4312aa4103fd0c92fe7cd9d1201c40e327b05812cdc60130d11b8c2e0eb47449";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.robot_names.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.robot_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.states.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.states {
n=add_size(n,element.wire_size()?)?;
}
if n>1736{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.robot_names.len() as u32).to_le_bytes());
for element in &self.robot_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.states.len() as u32).to_le_bytes());
for element in &self.states {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.robot_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.robot_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Kuka_driver_interfacesMsgFRIState::read_fields(bytes,at)?;
value.states.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgFRIStateArray {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1736{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/FriConfiguration.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgFriConfiguration {
  /// Declared receive_multiplier; canonical bounds are part of CODEC_IDENTITY.
  pub receive_multiplier:i32,
  /// Declared send_period_ms; canonical bounds are part of CODEC_IDENTITY.
  pub send_period_ms:i32,
}
impl Default for Ros2Kuka_driver_interfacesMsgFriConfiguration {fn default()->Self {Self {
receive_multiplier:Default::default(),
send_period_ms:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgFriConfiguration {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a60335766ad606adc5699c833cc6bc43adc3067691bcdbf587bbfb56e8a7127e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.receive_multiplier.to_le_bytes());
put(out,at,&self.send_period_ms.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.receive_multiplier=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.send_period_ms=decoded;
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgFriConfiguration {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/HardwareEvent.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgHardwareEvent {
  /// Declared robot_name; canonical bounds are part of CODEC_IDENTITY.
  pub robot_name:String,
  /// Declared event; canonical bounds are part of CODEC_IDENTITY.
  pub event:u8,
}
impl Default for Ros2Kuka_driver_interfacesMsgHardwareEvent {fn default()->Self {Self {
robot_name:Default::default(),
event:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgHardwareEvent {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cb542e9311957712257c6d8ae273b3d2d2339760978e11e3798982c84fbc5e68";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.robot_name.len()>512||self.robot_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.robot_name.len())?;
n=add_size(n,1)?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.robot_name.len() as u32).to_le_bytes());put(out,at,self.robot_name.as_bytes());
put(out,at,&self.event.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.robot_name=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.event=decoded;
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgHardwareEvent {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/KssStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgKssStatus {
  /// Declared control_mode; canonical bounds are part of CODEC_IDENTITY.
  pub control_mode:u8,
  /// Declared cycle_time; canonical bounds are part of CODEC_IDENTITY.
  pub cycle_time:u8,
  /// Declared drives_powered; canonical bounds are part of CODEC_IDENTITY.
  pub drives_powered:bool,
  /// Declared emergency_stop; canonical bounds are part of CODEC_IDENTITY.
  pub emergency_stop:bool,
  /// Declared guard_stop; canonical bounds are part of CODEC_IDENTITY.
  pub guard_stop:bool,
  /// Declared in_motion; canonical bounds are part of CODEC_IDENTITY.
  pub in_motion:bool,
  /// Declared motion_possible; canonical bounds are part of CODEC_IDENTITY.
  pub motion_possible:bool,
  /// Declared operation_mode; canonical bounds are part of CODEC_IDENTITY.
  pub operation_mode:u8,
  /// Declared robot_stopped; canonical bounds are part of CODEC_IDENTITY.
  pub robot_stopped:bool,
}
impl Default for Ros2Kuka_driver_interfacesMsgKssStatus {fn default()->Self {Self {
control_mode:Default::default(),
cycle_time:Default::default(),
drives_powered:Default::default(),
emergency_stop:Default::default(),
guard_stop:Default::default(),
in_motion:Default::default(),
motion_possible:Default::default(),
operation_mode:Default::default(),
robot_stopped:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgKssStatus {
  pub const MAX_WIRE_BYTES:usize=9;
  pub const CARTESIAN_WIRE_BYTES:u64=9;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b28bc2f19a0f410f0cd1d5b1d59daf1d09acb3e8312b4267f622a4d119f16b82";
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
if n>9{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.control_mode.to_le_bytes());
put(out,at,&self.cycle_time.to_le_bytes());
put(out,at,&[u8::from(self.drives_powered)]);
put(out,at,&[u8::from(self.emergency_stop)]);
put(out,at,&[u8::from(self.guard_stop)]);
put(out,at,&[u8::from(self.in_motion)]);
put(out,at,&[u8::from(self.motion_possible)]);
put(out,at,&self.operation_mode.to_le_bytes());
put(out,at,&[u8::from(self.robot_stopped)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.control_mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.cycle_time=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.drives_powered=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.emergency_stop=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.guard_stop=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.in_motion=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.motion_possible=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.operation_mode=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.robot_stopped=decoded;
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgKssStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 kuka_driver_interfaces/msg/KssStatusArray.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Kuka_driver_interfacesMsgKssStatusArray {
  /// Declared robot_names; canonical bounds are part of CODEC_IDENTITY.
  pub robot_names:Vec<String>,
  /// Declared statuses; canonical bounds are part of CODEC_IDENTITY.
  pub statuses:Vec<Ros2Kuka_driver_interfacesMsgKssStatus>,
}
impl Default for Ros2Kuka_driver_interfacesMsgKssStatusArray {fn default()->Self {Self {
robot_names:Default::default(),
statuses:Default::default(),
}}}
impl Ros2Kuka_driver_interfacesMsgKssStatusArray {
  pub const MAX_WIRE_BYTES:usize=1240;
  pub const CARTESIAN_WIRE_BYTES:u64=1240;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3c8e93b369eefea89c0aa56ab657721edd041ac66e18cfbf4fc4201df60dbebd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.robot_names.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.robot_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.statuses.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.statuses {
n=add_size(n,element.wire_size()?)?;
}
if n>1240{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.robot_names.len() as u32).to_le_bytes());
for element in &self.robot_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.statuses.len() as u32).to_le_bytes());
for element in &self.statuses {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.robot_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.robot_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.statuses=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Kuka_driver_interfacesMsgKssStatus::read_fields(bytes,at)?;
value.statuses.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Kuka_driver_interfacesMsgKssStatusArray {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1240{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/msg/State.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsMsgState {
  /// Declared id; canonical bounds are part of CODEC_IDENTITY.
  pub id:u8,
  /// Declared label; canonical bounds are part of CODEC_IDENTITY.
  pub label:String,
}
impl Default for Ros2Lifecycle_msgsMsgState {fn default()->Self {Self {
id:Default::default(),
label:Default::default(),
}}}
impl Ros2Lifecycle_msgsMsgState {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ebd49e5f87ddaa204cc4072ceef9840676c6f65f67448a027e26570dae0ded94";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.label.len()>512||self.label.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.label.len())?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.id.to_le_bytes());
put(out,at,&(self.label.len() as u32).to_le_bytes());put(out,at,self.label.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.id=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.label=decoded;
Ok(value)}
}
impl Canonical for Ros2Lifecycle_msgsMsgState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/msg/Transition.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsMsgTransition {
  /// Declared id; canonical bounds are part of CODEC_IDENTITY.
  pub id:u8,
  /// Declared label; canonical bounds are part of CODEC_IDENTITY.
  pub label:String,
}
impl Default for Ros2Lifecycle_msgsMsgTransition {fn default()->Self {Self {
id:Default::default(),
label:Default::default(),
}}}
impl Ros2Lifecycle_msgsMsgTransition {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1f9bb864622854b446a37cf3c6e29e9cf41f8efa37375c5a56205b4c02ed10cb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.label.len()>512||self.label.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.label.len())?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.id.to_le_bytes());
put(out,at,&(self.label.len() as u32).to_le_bytes());put(out,at,self.label.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.id=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.label=decoded;
Ok(value)}
}
impl Canonical for Ros2Lifecycle_msgsMsgTransition {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/srv/ChangeState_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsSrvChangeState_Request {
  /// Declared transition; canonical bounds are part of CODEC_IDENTITY.
  pub transition:Ros2Lifecycle_msgsMsgTransition,
}
impl Default for Ros2Lifecycle_msgsSrvChangeState_Request {fn default()->Self {Self {
transition:Default::default(),
}}}
impl Ros2Lifecycle_msgsSrvChangeState_Request {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b04270f03616b8e581036d327d18bde62d38627ae8acf88fb77d0ed7b3b3d3d1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.transition.wire_size()?)?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.transition.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Lifecycle_msgsMsgTransition::read_fields(bytes,at)?;
value.transition=decoded;
Ok(value)}
}
impl Canonical for Ros2Lifecycle_msgsSrvChangeState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/srv/ChangeState_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsSrvChangeState_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Lifecycle_msgsSrvChangeState_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Lifecycle_msgsSrvChangeState_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d2dc541d6995976a2a1a2c03d499aa9dccaf487736b11704d778c86fddaf5032";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Lifecycle_msgsSrvChangeState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/srv/GetState_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsSrvGetState_Request {
}
impl Default for Ros2Lifecycle_msgsSrvGetState_Request {fn default()->Self {Self {
}}}
impl Ros2Lifecycle_msgsSrvGetState_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86a3d813de32971a271e062acfaf201eb718aff8d68a4e343d6fceaae2b5098d";
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
impl Canonical for Ros2Lifecycle_msgsSrvGetState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 lifecycle_msgs/srv/GetState_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Lifecycle_msgsSrvGetState_Response {
  /// Declared current_state; canonical bounds are part of CODEC_IDENTITY.
  pub current_state:Ros2Lifecycle_msgsMsgState,
}
impl Default for Ros2Lifecycle_msgsSrvGetState_Response {fn default()->Self {Self {
current_state:Default::default(),
}}}
impl Ros2Lifecycle_msgsSrvGetState_Response {
  pub const MAX_WIRE_BYTES:usize=517;
  pub const CARTESIAN_WIRE_BYTES:u64=517;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_697418ec4095e98c855b1f9d238a3bd74a59ccbbc3eb31981c4df3c3b829ea39";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.current_state.wire_size()?)?;
if n>517{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.current_state.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Lifecycle_msgsMsgState::read_fields(bytes,at)?;
value.current_state=decoded;
Ok(value)}
}
impl Canonical for Ros2Lifecycle_msgsSrvGetState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>517{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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

impl Ros2Kuka_driver_interfacesMsgKssStatus {pub const UNSPECIFIED:u8=0;
pub const JOINT_POSITION_CONTROL:u8=1;
pub const CARTESIAN_POSITION_CONTROL:u8=5;
pub const RSI_4MS:u8=1;
pub const RSI_12MS:u8=2;
pub const T1:u8=1;
pub const T2:u8=2;
pub const AUT:u8=3;
pub const EXT:u8=4;}

impl Ros2Lifecycle_msgsMsgState {pub const PRIMARY_STATE_UNKNOWN:u8=0;
pub const PRIMARY_STATE_UNCONFIGURED:u8=1;
pub const PRIMARY_STATE_INACTIVE:u8=2;
pub const PRIMARY_STATE_ACTIVE:u8=3;
pub const PRIMARY_STATE_FINALIZED:u8=4;
pub const TRANSITION_STATE_CONFIGURING:u8=10;
pub const TRANSITION_STATE_CLEANINGUP:u8=11;
pub const TRANSITION_STATE_SHUTTINGDOWN:u8=12;
pub const TRANSITION_STATE_ACTIVATING:u8=13;
pub const TRANSITION_STATE_DEACTIVATING:u8=14;
pub const TRANSITION_STATE_ERRORPROCESSING:u8=15;}

impl Ros2Lifecycle_msgsMsgTransition {pub const TRANSITION_CREATE:u8=0;
pub const TRANSITION_CONFIGURE:u8=1;
pub const TRANSITION_CLEANUP:u8=2;
pub const TRANSITION_ACTIVATE:u8=3;
pub const TRANSITION_DEACTIVATE:u8=4;
pub const TRANSITION_UNCONFIGURED_SHUTDOWN:u8=5;
pub const TRANSITION_INACTIVE_SHUTDOWN:u8=6;
pub const TRANSITION_ACTIVE_SHUTDOWN:u8=7;
pub const TRANSITION_DESTROY:u8=8;
pub const TRANSITION_ON_CONFIGURE_SUCCESS:u8=10;
pub const TRANSITION_ON_CONFIGURE_FAILURE:u8=11;
pub const TRANSITION_ON_CONFIGURE_ERROR:u8=12;
pub const TRANSITION_ON_CLEANUP_SUCCESS:u8=20;
pub const TRANSITION_ON_CLEANUP_FAILURE:u8=21;
pub const TRANSITION_ON_CLEANUP_ERROR:u8=22;
pub const TRANSITION_ON_ACTIVATE_SUCCESS:u8=30;
pub const TRANSITION_ON_ACTIVATE_FAILURE:u8=31;
pub const TRANSITION_ON_ACTIVATE_ERROR:u8=32;
pub const TRANSITION_ON_DEACTIVATE_SUCCESS:u8=40;
pub const TRANSITION_ON_DEACTIVATE_FAILURE:u8=41;
pub const TRANSITION_ON_DEACTIVATE_ERROR:u8=42;
pub const TRANSITION_ON_SHUTDOWN_SUCCESS:u8=50;
pub const TRANSITION_ON_SHUTDOWN_FAILURE:u8=51;
pub const TRANSITION_ON_SHUTDOWN_ERROR:u8=52;
pub const TRANSITION_ON_ERROR_SUCCESS:u8=60;
pub const TRANSITION_ON_ERROR_FAILURE:u8=61;
pub const TRANSITION_ON_ERROR_ERROR:u8=62;
pub const TRANSITION_CALLBACK_SUCCESS:u8=97;
pub const TRANSITION_CALLBACK_FAILURE:u8=98;
pub const TRANSITION_CALLBACK_ERROR:u8=99;}
