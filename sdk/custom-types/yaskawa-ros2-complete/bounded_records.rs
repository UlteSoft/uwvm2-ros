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
/// Complete bounded value for ROS2 geometry_msgs/msg/TransformStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgTransformStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared child_frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub child_frame_id:String,
  /// Declared transform; canonical bounds are part of CODEC_IDENTITY.
  pub transform:Ros2Geometry_msgsMsgTransform,
}
impl Default for Ros2Geometry_msgsMsgTransformStamped {fn default()->Self {Self {
header:Default::default(),
child_frame_id:Default::default(),
transform:Default::default(),
}}}
impl Ros2Geometry_msgsMsgTransformStamped {
  pub const MAX_WIRE_BYTES:usize=648;
  pub const CARTESIAN_WIRE_BYTES:u64=648;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_be553b5a9c73b687cde56ff1c381e3c67168fd2c0366830be3c65382c5bbd792";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>512||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.transform.wire_size()?)?;
if n>648{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.child_frame_id.len() as u32).to_le_bytes());put(out,at,self.child_frame_id.as_bytes());
self.transform.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.child_frame_id=decoded;
let decoded=Ros2Geometry_msgsMsgTransform::read_fields(bytes,at)?;
value.transform=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTransformStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>648{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<72{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/DebugLevel.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgDebugLevel {
  /// Declared val; canonical bounds are part of CODEC_IDENTITY.
  pub val:u8,
}
impl Default for Ros2Industrial_msgsMsgDebugLevel {fn default()->Self {Self {
val:Default::default(),
}}}
impl Ros2Industrial_msgsMsgDebugLevel {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_989475cc5d981d8c5ab91fb091235871f7a1c7d87d246eb2250086e6c3c37c79";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.val.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.val=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgDebugLevel {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/DeviceInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgDeviceInfo {
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:String,
  /// Declared serial_number; canonical bounds are part of CODEC_IDENTITY.
  pub serial_number:String,
  /// Declared hw_version; canonical bounds are part of CODEC_IDENTITY.
  pub hw_version:String,
  /// Declared sw_version; canonical bounds are part of CODEC_IDENTITY.
  pub sw_version:String,
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:String,
}
impl Default for Ros2Industrial_msgsMsgDeviceInfo {fn default()->Self {Self {
model:Default::default(),
serial_number:Default::default(),
hw_version:Default::default(),
sw_version:Default::default(),
address:Default::default(),
}}}
impl Ros2Industrial_msgsMsgDeviceInfo {
  pub const MAX_WIRE_BYTES:usize=2580;
  pub const CARTESIAN_WIRE_BYTES:u64=2580;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f9a5962800e17495f34dbb73f37373df22f900e528c630497417f78cdecb283b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.model.len()>512||self.model.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.model.len())?;
if self.serial_number.len()>512||self.serial_number.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.serial_number.len())?;
if self.hw_version.len()>512||self.hw_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.hw_version.len())?;
if self.sw_version.len()>512||self.sw_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.sw_version.len())?;
if self.address.len()>512||self.address.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.address.len())?;
if n>2580{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.model.len() as u32).to_le_bytes());put(out,at,self.model.as_bytes());
put(out,at,&(self.serial_number.len() as u32).to_le_bytes());put(out,at,self.serial_number.as_bytes());
put(out,at,&(self.hw_version.len() as u32).to_le_bytes());put(out,at,self.hw_version.as_bytes());
put(out,at,&(self.sw_version.len() as u32).to_le_bytes());put(out,at,self.sw_version.as_bytes());
put(out,at,&(self.address.len() as u32).to_le_bytes());put(out,at,self.address.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.model=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.serial_number=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.hw_version=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.sw_version=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.address=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgDeviceInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2580{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/RobotMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgRobotMode {
  /// Declared val; canonical bounds are part of CODEC_IDENTITY.
  pub val:i8,
}
impl Default for Ros2Industrial_msgsMsgRobotMode {fn default()->Self {Self {
val:Default::default(),
}}}
impl Ros2Industrial_msgsMsgRobotMode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4f9173c5e27815ca33be603b8645a22420bda3ab308962127060d2413d092c4c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.val.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.val=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgRobotMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/TriState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgTriState {
  /// Declared val; canonical bounds are part of CODEC_IDENTITY.
  pub val:i8,
}
impl Default for Ros2Industrial_msgsMsgTriState {fn default()->Self {Self {
val:Default::default(),
}}}
impl Ros2Industrial_msgsMsgTriState {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b7ae62ce19d642ad46e962ffb50cd76c43c96ac673ac36d690f99c1dd8d2a841";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.val.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.val=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgTriState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/RobotStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgRobotStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:Ros2Industrial_msgsMsgRobotMode,
  /// Declared e_stopped; canonical bounds are part of CODEC_IDENTITY.
  pub e_stopped:Ros2Industrial_msgsMsgTriState,
  /// Declared drives_powered; canonical bounds are part of CODEC_IDENTITY.
  pub drives_powered:Ros2Industrial_msgsMsgTriState,
  /// Declared motion_possible; canonical bounds are part of CODEC_IDENTITY.
  pub motion_possible:Ros2Industrial_msgsMsgTriState,
  /// Declared in_motion; canonical bounds are part of CODEC_IDENTITY.
  pub in_motion:Ros2Industrial_msgsMsgTriState,
  /// Declared in_error; canonical bounds are part of CODEC_IDENTITY.
  pub in_error:Ros2Industrial_msgsMsgTriState,
  /// Declared error_codes; canonical bounds are part of CODEC_IDENTITY.
  pub error_codes:Vec<i32>,
}
impl Default for Ros2Industrial_msgsMsgRobotStatus {fn default()->Self {Self {
header:Default::default(),
mode:Default::default(),
e_stopped:Default::default(),
drives_powered:Default::default(),
motion_possible:Default::default(),
in_motion:Default::default(),
in_error:Default::default(),
error_codes:Default::default(),
}}}
impl Ros2Industrial_msgsMsgRobotStatus {
  pub const MAX_WIRE_BYTES:usize=150;
  pub const CARTESIAN_WIRE_BYTES:u64=150;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1086f16ba5d614f3d87c3ab195240b5a0683ef490f2e92434e4ea3bdbe7554ba";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.mode.wire_size()?)?;
n=add_size(n,self.e_stopped.wire_size()?)?;
n=add_size(n,self.drives_powered.wire_size()?)?;
n=add_size(n,self.motion_possible.wire_size()?)?;
n=add_size(n,self.in_motion.wire_size()?)?;
n=add_size(n,self.in_error.wire_size()?)?;
if self.error_codes.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.error_codes {
n=add_size(n,4)?;
}
if n>150{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.mode.write_fields(out,at);
self.e_stopped.write_fields(out,at);
self.drives_powered.write_fields(out,at);
self.motion_possible.write_fields(out,at);
self.in_motion.write_fields(out,at);
self.in_error.write_fields(out,at);
put(out,at,&(self.error_codes.len() as u32).to_le_bytes());
for element in &self.error_codes {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Industrial_msgsMsgRobotMode::read_fields(bytes,at)?;
value.mode=decoded;
let decoded=Ros2Industrial_msgsMsgTriState::read_fields(bytes,at)?;
value.e_stopped=decoded;
let decoded=Ros2Industrial_msgsMsgTriState::read_fields(bytes,at)?;
value.drives_powered=decoded;
let decoded=Ros2Industrial_msgsMsgTriState::read_fields(bytes,at)?;
value.motion_possible=decoded;
let decoded=Ros2Industrial_msgsMsgTriState::read_fields(bytes,at)?;
value.in_motion=decoded;
let decoded=Ros2Industrial_msgsMsgTriState::read_fields(bytes,at)?;
value.in_error=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.error_codes=Vec::with_capacity(count);
for i in 0..count {
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_codes.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgRobotStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>150{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<22{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/msg/ServiceReturnCode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsMsgServiceReturnCode {
  /// Declared val; canonical bounds are part of CODEC_IDENTITY.
  pub val:i8,
}
impl Default for Ros2Industrial_msgsMsgServiceReturnCode {fn default()->Self {Self {
val:Default::default(),
}}}
impl Ros2Industrial_msgsMsgServiceReturnCode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_62eb4415a63d7686f16ea343193238838c78aa8e9b9786a65c92fb227e03d3e5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.val.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.val=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsMsgServiceReturnCode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/CmdJointTrajectory_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvCmdJointTrajectory_Request {
  /// Declared trajectory; canonical bounds are part of CODEC_IDENTITY.
  pub trajectory:Ros2Trajectory_msgsMsgJointTrajectory,
}
impl Default for Ros2Industrial_msgsSrvCmdJointTrajectory_Request {fn default()->Self {Self {
trajectory:Default::default(),
}}}
impl Ros2Industrial_msgsSrvCmdJointTrajectory_Request {
  pub const MAX_WIRE_BYTES:usize=4164;
  pub const CARTESIAN_WIRE_BYTES:u64=4164;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1def3934b243546a4591b7be1ef2eb27002fbd9d3725302016e3b1846805149d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trajectory.wire_size()?)?;
if n>4164{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trajectory.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Trajectory_msgsMsgJointTrajectory::read_fields(bytes,at)?;
value.trajectory=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvCmdJointTrajectory_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4164{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/CmdJointTrajectory_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvCmdJointTrajectory_Response {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvCmdJointTrajectory_Response {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvCmdJointTrajectory_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1e4d064ef659e5d1cc73bdd90f82657a16087c96e0f75b632e2be2be884bca8b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.code.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvCmdJointTrajectory_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/GetRobotInfo_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvGetRobotInfo_Request {
}
impl Default for Ros2Industrial_msgsSrvGetRobotInfo_Request {fn default()->Self {Self {
}}}
impl Ros2Industrial_msgsSrvGetRobotInfo_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_688a272f6ee39921013c6c39a7b0b0c4e2cda5119787088234103518ba25c1d2";
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
impl Canonical for Ros2Industrial_msgsSrvGetRobotInfo_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/GetRobotInfo_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvGetRobotInfo_Response {
  /// Declared controller; canonical bounds are part of CODEC_IDENTITY.
  pub controller:Ros2Industrial_msgsMsgDeviceInfo,
  /// Declared robots; canonical bounds are part of CODEC_IDENTITY.
  pub robots:Vec<Ros2Industrial_msgsMsgDeviceInfo>,
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvGetRobotInfo_Response {fn default()->Self {Self {
controller:Default::default(),
robots:Default::default(),
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvGetRobotInfo_Response {
  pub const MAX_WIRE_BYTES:usize=32768;
  pub const CARTESIAN_WIRE_BYTES:u64=43865;
  pub const AGGREGATE_WIRE_BYTES:usize=32768;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6d37cdcd8afc918329d7856e682d7fcdae2931832474a9194143c21079ed4152";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.controller.wire_size()?)?;
if self.robots.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.robots {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.code.wire_size()?)?;
if n>32768{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.controller.write_fields(out,at);
put(out,at,&(self.robots.len() as u32).to_le_bytes());
for element in &self.robots {
element.write_fields(out,at);
}
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgDeviceInfo::read_fields(bytes,at)?;
value.controller=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.robots=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Industrial_msgsMsgDeviceInfo::read_fields(bytes,at)?;
value.robots.push(decoded);
}}
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvGetRobotInfo_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32768{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/SetDrivePower_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvSetDrivePower_Request {
  /// Declared drive_power; canonical bounds are part of CODEC_IDENTITY.
  pub drive_power:bool,
}
impl Default for Ros2Industrial_msgsSrvSetDrivePower_Request {fn default()->Self {Self {
drive_power:Default::default(),
}}}
impl Ros2Industrial_msgsSrvSetDrivePower_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b4fc5e109e131a97820ab5856b34839c572ac0b0c256d70457c176e401844f96";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.drive_power)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.drive_power=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvSetDrivePower_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/SetDrivePower_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvSetDrivePower_Response {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvSetDrivePower_Response {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvSetDrivePower_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_050fb9a727ebedd240e9023b9a2379a5d72e0d9fa335bead0c40d783abb22cff";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.code.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvSetDrivePower_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request {
  /// Declared level; canonical bounds are part of CODEC_IDENTITY.
  pub level:Ros2Industrial_msgsMsgDebugLevel,
}
impl Default for Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request {fn default()->Self {Self {
level:Default::default(),
}}}
impl Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bbadd39c05cab90b14f4b1fad23ff05ac83325f6cc57810cd649e04fa29fe014";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.level.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.level.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgDebugLevel::read_fields(bytes,at)?;
value.level=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b8681d031a4bb5a67b842277af2e2156e8339bdef23be88d2b64e296a7e3d63b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.code.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/StartMotion_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvStartMotion_Request {
}
impl Default for Ros2Industrial_msgsSrvStartMotion_Request {fn default()->Self {Self {
}}}
impl Ros2Industrial_msgsSrvStartMotion_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0a96fb9489f863612a91bf82f995f2f28bcb7fb403bde74c617925c789e4dcd8";
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
impl Canonical for Ros2Industrial_msgsSrvStartMotion_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/StartMotion_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvStartMotion_Response {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvStartMotion_Response {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvStartMotion_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3bb3650fbe638a6d300bcd9fcd5acf574bd54d00a7368edd3014bf44dcf66c16";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.code.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvStartMotion_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/StopMotion_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvStopMotion_Request {
}
impl Default for Ros2Industrial_msgsSrvStopMotion_Request {fn default()->Self {Self {
}}}
impl Ros2Industrial_msgsSrvStopMotion_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fe5a7cf0da30270eab5cfb1ab8c1fbd37d2b4c4f91ef29662a0ddcac39e6ef49";
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
impl Canonical for Ros2Industrial_msgsSrvStopMotion_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 industrial_msgs/srv/StopMotion_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Industrial_msgsSrvStopMotion_Response {
  /// Declared code; canonical bounds are part of CODEC_IDENTITY.
  pub code:Ros2Industrial_msgsMsgServiceReturnCode,
}
impl Default for Ros2Industrial_msgsSrvStopMotion_Response {fn default()->Self {Self {
code:Default::default(),
}}}
impl Ros2Industrial_msgsSrvStopMotion_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c26bda240d86becc5b8b4ea80380eba3602092a5ec4a06818dbfbd38c24015b5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.code.wire_size()?)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.code.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Industrial_msgsMsgServiceReturnCode::read_fields(bytes,at)?;
value.code=decoded;
Ok(value)}
}
impl Canonical for Ros2Industrial_msgsSrvStopMotion_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/AlarmCauseRemedy.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgAlarmCauseRemedy {
  /// Declared cause; canonical bounds are part of CODEC_IDENTITY.
  pub cause:String,
  /// Declared remedy; canonical bounds are part of CODEC_IDENTITY.
  pub remedy:String,
}
impl Default for Ros2Motoros2_interfacesMsgAlarmCauseRemedy {fn default()->Self {Self {
cause:Default::default(),
remedy:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgAlarmCauseRemedy {
  pub const MAX_WIRE_BYTES:usize=1032;
  pub const CARTESIAN_WIRE_BYTES:u64=1032;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f977609022f4ca593b9089d0038d1c28d1bc4b533e272f38c3379495f326ddf5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.cause.len()>512||self.cause.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.cause.len())?;
if self.remedy.len()>512||self.remedy.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.remedy.len())?;
if n>1032{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.cause.len() as u32).to_le_bytes());put(out,at,self.cause.as_bytes());
put(out,at,&(self.remedy.len() as u32).to_le_bytes());put(out,at,self.remedy.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.cause=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.remedy=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgAlarmCauseRemedy {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1032{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/AlarmInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgAlarmInfo {
  /// Declared number; canonical bounds are part of CODEC_IDENTITY.
  pub number:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared sub_code; canonical bounds are part of CODEC_IDENTITY.
  pub sub_code:i32,
  /// Declared datetime; canonical bounds are part of CODEC_IDENTITY.
  pub datetime:Ros2Builtin_interfacesMsgTime,
  /// Declared contents; canonical bounds are part of CODEC_IDENTITY.
  pub contents:String,
  /// Declared description; canonical bounds are part of CODEC_IDENTITY.
  pub description:String,
  /// Declared help; canonical bounds are part of CODEC_IDENTITY.
  pub help:Vec<Ros2Motoros2_interfacesMsgAlarmCauseRemedy>,
}
impl Default for Ros2Motoros2_interfacesMsgAlarmInfo {fn default()->Self {Self {
number:Default::default(),
message:Default::default(),
sub_code:Default::default(),
datetime:Default::default(),
contents:Default::default(),
description:Default::default(),
help:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgAlarmInfo {
  pub const MAX_WIRE_BYTES:usize=25248;
  pub const CARTESIAN_WIRE_BYTES:u64=25248;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7cf3fa3037f28f295dd62009e708ea3e46a80f83471d0a0f9fb55af125c0c27f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,4)?;
n=add_size(n,self.datetime.wire_size()?)?;
if self.contents.len()>4096||self.contents.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.contents.len())?;
if self.description.len()>4096||self.description.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.description.len())?;
if self.help.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.help {
n=add_size(n,element.wire_size()?)?;
}
if n>25248{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.number.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&self.sub_code.to_le_bytes());
self.datetime.write_fields(out,at);
put(out,at,&(self.contents.len() as u32).to_le_bytes());put(out,at,self.contents.as_bytes());
put(out,at,&(self.description.len() as u32).to_le_bytes());put(out,at,self.description.as_bytes());
put(out,at,&(self.help.len() as u32).to_le_bytes());
for element in &self.help {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.number=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sub_code=decoded;
let decoded=Ros2Builtin_interfacesMsgTime::read_fields(bytes,at)?;
value.datetime=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.contents=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.description=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.help=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Motoros2_interfacesMsgAlarmCauseRemedy::read_fields(bytes,at)?;
value.help.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgAlarmInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25248{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/ErrorInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgErrorInfo {
  /// Declared number; canonical bounds are part of CODEC_IDENTITY.
  pub number:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared sub_code; canonical bounds are part of CODEC_IDENTITY.
  pub sub_code:i32,
  /// Declared datetime; canonical bounds are part of CODEC_IDENTITY.
  pub datetime:Ros2Builtin_interfacesMsgTime,
}
impl Default for Ros2Motoros2_interfacesMsgErrorInfo {fn default()->Self {Self {
number:Default::default(),
message:Default::default(),
sub_code:Default::default(),
datetime:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgErrorInfo {
  pub const MAX_WIRE_BYTES:usize=532;
  pub const CARTESIAN_WIRE_BYTES:u64=532;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1cfe60c0af6bc157db84c4d9e0b20a8f2e5c78fd4afd6908d32b3217f3734b67";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,4)?;
n=add_size(n,self.datetime.wire_size()?)?;
if n>532{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.number.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&self.sub_code.to_le_bytes());
self.datetime.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.number=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sub_code=decoded;
let decoded=Ros2Builtin_interfacesMsgTime::read_fields(bytes,at)?;
value.datetime=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgErrorInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>532{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/InformJobCrudResultCodes.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {
}
impl Default for Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f5ed037b83926b48a88fa486ed8db5f5d3e39e4a486d4a90190ffd6a70452613";
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
impl Canonical for Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/InitTrajEnum.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgInitTrajEnum {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Motoros2_interfacesMsgInitTrajEnum {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgInitTrajEnum {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9279979f9e3570f48d6e23bf812a4680e3deed9af3309019efdff5464d7b7763";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgInitTrajEnum {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/IoResultCodes.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgIoResultCodes {
}
impl Default for Ros2Motoros2_interfacesMsgIoResultCodes {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesMsgIoResultCodes {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ef9ab829ae79d4e58336a56259b6beb2ad26a8b1b4070013d9004ac4ab269915";
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
impl Canonical for Ros2Motoros2_interfacesMsgIoResultCodes {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/MotionReadyEnum.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgMotionReadyEnum {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Motoros2_interfacesMsgMotionReadyEnum {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgMotionReadyEnum {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c8d4c2b0eba665e01cddc730421a3a42c5b344f4de35ae0a32204891603326d6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgMotionReadyEnum {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/QueueResultEnum.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgQueueResultEnum {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Motoros2_interfacesMsgQueueResultEnum {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgQueueResultEnum {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_146c6ce3f275b9e4a81622d73cca3d2298cf9f7bb95a783c0dea6279cb5b4f23";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgQueueResultEnum {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/msg/SelectionResultCodes.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesMsgSelectionResultCodes {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u32,
}
impl Default for Ros2Motoros2_interfacesMsgSelectionResultCodes {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesMsgSelectionResultCodes {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_46168b1f851163985e75842d9d795c5715160864a7736e7f92f95818c9435602";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesMsgSelectionResultCodes {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request {
}
impl Default for Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aadcd63f3bc47424c38767afdc1589561e3c90e8aeae9bdceef88b1033d57a7e";
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
impl Canonical for Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared result_message; canonical bounds are part of CODEC_IDENTITY.
  pub result_message:String,
  /// Declared alarms; canonical bounds are part of CODEC_IDENTITY.
  pub alarms:Vec<Ros2Motoros2_interfacesMsgAlarmInfo>,
  /// Declared errors; canonical bounds are part of CODEC_IDENTITY.
  pub errors:Vec<Ros2Motoros2_interfacesMsgErrorInfo>,
}
impl Default for Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response {fn default()->Self {Self {
result_code:Default::default(),
result_message:Default::default(),
alarms:Default::default(),
errors:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response {
  pub const MAX_WIRE_BYTES:usize=32768;
  pub const CARTESIAN_WIRE_BYTES:u64=413008;
  pub const AGGREGATE_WIRE_BYTES:usize=32768;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e11bdbb95b6ded88d035696260c58dad32bcf3604aa4e89dd41c0dab26541567";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.result_message.len()>512||self.result_message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.result_message.len())?;
if self.alarms.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.alarms {
n=add_size(n,element.wire_size()?)?;
}
if self.errors.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.errors {
n=add_size(n,element.wire_size()?)?;
}
if n>32768{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.result_message.len() as u32).to_le_bytes());put(out,at,self.result_message.as_bytes());
put(out,at,&(self.alarms.len() as u32).to_le_bytes());
for element in &self.alarms {
element.write_fields(out,at);
}
put(out,at,&(self.errors.len() as u32).to_le_bytes());
for element in &self.errors {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.result_message=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.alarms=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Motoros2_interfacesMsgAlarmInfo::read_fields(bytes,at)?;
value.alarms.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.errors=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Motoros2_interfacesMsgErrorInfo::read_fields(bytes,at)?;
value.errors.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32768{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ListInformJobs_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvListInformJobs_Request {
}
impl Default for Ros2Motoros2_interfacesSrvListInformJobs_Request {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesSrvListInformJobs_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e77eedadef98d98321b1757b88284022afcdb1ffef52b98820b0c4ba596d643c";
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
impl Canonical for Ros2Motoros2_interfacesSrvListInformJobs_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ListInformJobs_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvListInformJobs_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared names; canonical bounds are part of CODEC_IDENTITY.
  pub names:Vec<String>,
}
impl Default for Ros2Motoros2_interfacesSrvListInformJobs_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
names:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvListInformJobs_Response {
  pub const MAX_WIRE_BYTES:usize=8780;
  pub const CARTESIAN_WIRE_BYTES:u64=8780;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_49021f0abc00f8df1ef49605a244cb6bba8d8b45ece5365ae35d747d2506eadc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if self.names.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.names {
if element.len()>512||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if n>8780{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&(self.names.len() as u32).to_le_bytes());
for element in &self.names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.names.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvListInformJobs_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8780{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/QueueTrajPoint_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvQueueTrajPoint_Request {
  /// Declared joint_names; canonical bounds are part of CODEC_IDENTITY.
  pub joint_names:Vec<String>,
  /// Declared point; canonical bounds are part of CODEC_IDENTITY.
  pub point:Ros2Trajectory_msgsMsgJointTrajectoryPoint,
}
impl Default for Ros2Motoros2_interfacesSrvQueueTrajPoint_Request {fn default()->Self {Self {
joint_names:Default::default(),
point:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvQueueTrajPoint_Request {
  pub const MAX_WIRE_BYTES:usize=1228;
  pub const CARTESIAN_WIRE_BYTES:u64=1228;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3d37f9981d6abaca33ec0b5378fa3a1ac301791cc9be94d5bdcf4ea6aa81b8d0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.joint_names.len()>12{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.point.wire_size()?)?;
if n>1228{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.joint_names.len() as u32).to_le_bytes());
for element in &self.joint_names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
self.point.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>12{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.point=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvQueueTrajPoint_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1228{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/QueueTrajPoint_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvQueueTrajPoint_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:Ros2Motoros2_interfacesMsgQueueResultEnum,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Motoros2_interfacesSrvQueueTrajPoint_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvQueueTrajPoint_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d4887ed24d39332a5ccfd4a3d072004dd8f314fe5c166bf0bd57520a826cbccd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.result_code.wire_size()?)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.result_code.write_fields(out,at);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Motoros2_interfacesMsgQueueResultEnum::read_fields(bytes,at)?;
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvQueueTrajPoint_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadGroupIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadGroupIO_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
}
impl Default for Ros2Motoros2_interfacesSrvReadGroupIO_Request {fn default()->Self {Self {
address:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadGroupIO_Request {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6ee09ac379e169fc23b81a551e9a9ed21c3f82b5010859984e911b17152beb7f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadGroupIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadGroupIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadGroupIO_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
}
impl Default for Ros2Motoros2_interfacesSrvReadGroupIO_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadGroupIO_Response {
  pub const MAX_WIRE_BYTES:usize=522;
  pub const CARTESIAN_WIRE_BYTES:u64=522;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_292fcf6e8c46a2509ee4cc5238d443157f37fed0de898367248b967e9001ea0d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>522{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadGroupIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>522{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadMRegister_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadMRegister_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
}
impl Default for Ros2Motoros2_interfacesSrvReadMRegister_Request {fn default()->Self {Self {
address:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadMRegister_Request {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f31f0983466054f5ac5dcb2ab80ab7721d606a681b833ebb376cc87f96054f6f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadMRegister_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadMRegister_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadMRegister_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Motoros2_interfacesSrvReadMRegister_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadMRegister_Response {
  pub const MAX_WIRE_BYTES:usize=523;
  pub const CARTESIAN_WIRE_BYTES:u64=523;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8d6ded2876adfa2079711205cb0110a82ead6f51db05f266bf285308cd815a04";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
n=add_size(n,2)?;
if n>523{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadMRegister_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>523{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<11{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadSingleIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadSingleIO_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
}
impl Default for Ros2Motoros2_interfacesSrvReadSingleIO_Request {fn default()->Self {Self {
address:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadSingleIO_Request {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_30e28db303b772f26b2b3cf6165a0ff79037d037d60e8115b4ee8e177f17e8bb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadSingleIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ReadSingleIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvReadSingleIO_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:i32,
}
impl Default for Ros2Motoros2_interfacesSrvReadSingleIO_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvReadSingleIO_Response {
  pub const MAX_WIRE_BYTES:usize=525;
  pub const CARTESIAN_WIRE_BYTES:u64=525;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b42013571274e85769ffac4d47129de8b76e4a675bdef3efefebc9ee19e92082";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
n=add_size(n,4)?;
if n>525{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvReadSingleIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>525{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ResetError_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvResetError_Request {
}
impl Default for Ros2Motoros2_interfacesSrvResetError_Request {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesSrvResetError_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2737855d0608478946e2d9ab7f06c92585199c1082ec8406a012f50ad56beae3";
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
impl Canonical for Ros2Motoros2_interfacesSrvResetError_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/ResetError_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvResetError_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Motoros2_interfacesSrvResetError_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvResetError_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0af14ee02c157ecc39b143225a580e475473306389ccc3ba198cf22ca47d82f3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.result_code.wire_size()?)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.result_code.write_fields(out,at);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum::read_fields(bytes,at)?;
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvResetError_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/SelectMotionTool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvSelectMotionTool_Request {
  /// Declared group_number; canonical bounds are part of CODEC_IDENTITY.
  pub group_number:u32,
  /// Declared tool_number; canonical bounds are part of CODEC_IDENTITY.
  pub tool_number:u32,
}
impl Default for Ros2Motoros2_interfacesSrvSelectMotionTool_Request {fn default()->Self {Self {
group_number:Default::default(),
tool_number:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvSelectMotionTool_Request {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e6c884ae0a97337c1e441efe099b7869dc95205fcbca0a8082d4d9b06f5b438c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.group_number.to_le_bytes());
put(out,at,&self.tool_number.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.group_number=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.tool_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvSelectMotionTool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/SelectMotionTool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvSelectMotionTool_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:Ros2Motoros2_interfacesMsgSelectionResultCodes,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Motoros2_interfacesSrvSelectMotionTool_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvSelectMotionTool_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_088349a49c14c52282c25b428628462c48618c3b5202df560dcc5c9bed742640";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.result_code.wire_size()?)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.result_code.write_fields(out,at);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Motoros2_interfacesMsgSelectionResultCodes::read_fields(bytes,at)?;
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvSelectMotionTool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/StartPointQueueMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvStartPointQueueMode_Request {
}
impl Default for Ros2Motoros2_interfacesSrvStartPointQueueMode_Request {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesSrvStartPointQueueMode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_58a4827fc6535ef0ffc64ee25c5c4af75aa0fe199fa0350a88a95d2e991164a1";
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
impl Canonical for Ros2Motoros2_interfacesSrvStartPointQueueMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/StartPointQueueMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvStartPointQueueMode_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Motoros2_interfacesSrvStartPointQueueMode_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvStartPointQueueMode_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bc553d5640e1c4644a5d5bdc2dfcefc9dbe789ef1e2a23dd620f08963467b6ff";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.result_code.wire_size()?)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.result_code.write_fields(out,at);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum::read_fields(bytes,at)?;
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvStartPointQueueMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/StartTrajMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvStartTrajMode_Request {
}
impl Default for Ros2Motoros2_interfacesSrvStartTrajMode_Request {fn default()->Self {Self {
}}}
impl Ros2Motoros2_interfacesSrvStartTrajMode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0b8b2ad5db5901047035e1070b675618afc516a1ae8c3308cfdcad1174d34789";
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
impl Canonical for Ros2Motoros2_interfacesSrvStartTrajMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/StartTrajMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvStartTrajMode_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:Ros2Motoros2_interfacesMsgMotionReadyEnum,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Motoros2_interfacesSrvStartTrajMode_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvStartTrajMode_Response {
  pub const MAX_WIRE_BYTES:usize=518;
  pub const CARTESIAN_WIRE_BYTES:u64=518;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_170e25737e6a2b06f5d5d9f331d748c97cf0306284271757953a4a801c722939";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.result_code.wire_size()?)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>518{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.result_code.write_fields(out,at);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Motoros2_interfacesMsgMotionReadyEnum::read_fields(bytes,at)?;
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvStartTrajMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>518{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteGroupIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteGroupIO_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
}
impl Default for Ros2Motoros2_interfacesSrvWriteGroupIO_Request {fn default()->Self {Self {
address:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteGroupIO_Request {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2d348ed71408a0dd1dd28ee2ed2ca1db56bb07a14546d0fe13ccf64e8553d52c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteGroupIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteGroupIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteGroupIO_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Motoros2_interfacesSrvWriteGroupIO_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteGroupIO_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_06769934711b27fd32cdcbf849504e160189ee82ea32e0e71a2903af47c5d766";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteGroupIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteMRegister_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteMRegister_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u16,
}
impl Default for Ros2Motoros2_interfacesSrvWriteMRegister_Request {fn default()->Self {Self {
address:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteMRegister_Request {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8df4536be27f0632052725d5616fd5b047f49fc306fc7735854d9de4fb15d4ef";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,2)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteMRegister_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteMRegister_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteMRegister_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Motoros2_interfacesSrvWriteMRegister_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteMRegister_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a61cc0596a7545ef4836bdc64dc9d6b4ed99b0abd18a726eb37e6215f51c729a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteMRegister_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteSingleIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteSingleIO_Request {
  /// Declared address; canonical bounds are part of CODEC_IDENTITY.
  pub address:u32,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:i32,
}
impl Default for Ros2Motoros2_interfacesSrvWriteSingleIO_Request {fn default()->Self {Self {
address:Default::default(),
value:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteSingleIO_Request {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6361affcd8b7ebaaa1fe6b5806e64a7c18f7a96a1178614cfce487de813a6392";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.address.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.address=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteSingleIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 motoros2_interfaces/srv/WriteSingleIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Motoros2_interfacesSrvWriteSingleIO_Response {
  /// Declared result_code; canonical bounds are part of CODEC_IDENTITY.
  pub result_code:u32,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Motoros2_interfacesSrvWriteSingleIO_Response {fn default()->Self {Self {
result_code:Default::default(),
message:Default::default(),
success:Default::default(),
}}}
impl Ros2Motoros2_interfacesSrvWriteSingleIO_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_12328db372a572fe9b0c7207331a7971d5d7c9033376e3be410db579b85fb794";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.message.len()>512||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.result_code.to_le_bytes());
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.result_code=decoded;
let size=read_count(bytes,at)?;if size>512{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Motoros2_interfacesSrvWriteSingleIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 tf2_msgs/msg/TFMessage.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Tf2_msgsMsgTFMessage {
  /// Declared transforms; canonical bounds are part of CODEC_IDENTITY.
  pub transforms:Vec<Ros2Geometry_msgsMsgTransformStamped>,
}
impl Default for Ros2Tf2_msgsMsgTFMessage {fn default()->Self {Self {
transforms:Default::default(),
}}}
impl Ros2Tf2_msgsMsgTFMessage {
  pub const MAX_WIRE_BYTES:usize=10372;
  pub const CARTESIAN_WIRE_BYTES:u64=10372;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4428597a9c2783fd8f0f083a16e8fd1b3d286487f4800c55fcc5571e7f79e65a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.transforms.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.transforms {
n=add_size(n,element.wire_size()?)?;
}
if n>10372{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.transforms.len() as u32).to_le_bytes());
for element in &self.transforms {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.transforms=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTransformStamped::read_fields(bytes,at)?;
value.transforms.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Tf2_msgsMsgTFMessage {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>10372{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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

impl Ros2Industrial_msgsMsgDebugLevel {pub const DEBUG:u8=5;
pub const INFO:u8=4;
pub const WARN:u8=3;
pub const ERROR:u8=2;
pub const FATAL:u8=1;
pub const NONE:u8=0;}

impl Ros2Industrial_msgsMsgRobotMode {pub const UNKNOWN:i8=-1;
pub const MANUAL:i8=1;
pub const AUTO:i8=2;}

impl Ros2Industrial_msgsMsgTriState {pub const UNKNOWN:i8=-1;
pub const TRUE:i8=1;
pub const ON:i8=1;
pub const ENABLED:i8=1;
pub const HIGH:i8=1;
pub const CLOSED:i8=1;
pub const FALSE:i8=0;
pub const OFF:i8=0;
pub const DISABLED:i8=0;
pub const LOW:i8=0;
pub const OPEN:i8=0;}

impl Ros2Industrial_msgsMsgServiceReturnCode {pub const SUCCESS:i8=1;
pub const FAILURE:i8=-1;}

impl Ros2Motoros2_interfacesMsgInformJobCrudResultCodes {pub const RC_OK:u32=1;
pub const STR_OK:&'static str="Success";
pub const RC_ERR_REFRESHING_JOB_LIST:u32=10;
pub const STR_ERR_REFRESHING_JOB_LIST:&'static str="Error refreshing job list";
pub const RC_ERR_RETRIEVING_FILE_COUNT:u32=11;
pub const STR_ERR_RETRIEVING_FILE_COUNT:&'static str="Error retrieving file count";
pub const RC_ERR_TOO_MANY_JOBS:u32=12;
pub const STR_ERR_TOO_MANY_JOBS:&'static str="Too many jobs";
pub const RC_ERR_RETRIEVING_JOB_NAME_FROM_LIST:u32=13;
pub const STR_ERR_RETRIEVING_JOB_NAME_FROM_LIST:&'static str="Failed retrieving job name from list";
pub const RC_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE:u32=14;
pub const STR_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE:&'static str="Error adding job name to service response";}

impl Ros2Motoros2_interfacesMsgInitTrajEnum {pub const INIT_TRAJ_OK:u16=0;
pub const INIT_TRAJ_OK_STR:&'static str="OK";
pub const INIT_TRAJ_UNSPECIFIED:u16=200;
pub const INIT_TRAJ_UNSPECIFIED_STR:&'static str="Unspecified failure";
pub const INIT_TRAJ_TOO_SMALL:u16=201;
pub const INIT_TRAJ_TOO_SMALL_STR:&'static str="Trajectory must contain at least two points.";
pub const INIT_TRAJ_TOO_BIG:u16=202;
pub const INIT_TRAJ_TOO_BIG_STR:&'static str="Trajectory contains too many points (Not enough memory).";
pub const INIT_TRAJ_ALREADY_IN_MOTION:u16=203;
pub const INIT_TRAJ_ALREADY_IN_MOTION_STR:&'static str="Already running a trajectory.";
pub const INIT_TRAJ_INVALID_STARTING_POS:u16=204;
pub const INIT_TRAJ_INVALID_STARTING_POS_STR:&'static str="The first point must match the robot's current position.";
pub const INIT_TRAJ_INVALID_VELOCITY:u16=205;
pub const INIT_TRAJ_INVALID_VELOCITY_STR:&'static str="The commanded velocity is too high.";
pub const INIT_TRAJ_INVALID_JOINTNAME:u16=206;
pub const INIT_TRAJ_INVALID_JOINTNAME_STR:&'static str="Invalid joint name specified. Check motoros2_config.yaml.";
pub const INIT_TRAJ_INCOMPLETE_JOINTLIST:u16=207;
pub const INIT_TRAJ_INCOMPLETE_JOINTLIST_STR:&'static str="Trajectory must contain data for all joints.";
pub const INIT_TRAJ_INVALID_TIME:u16=208;
pub const INIT_TRAJ_INVALID_TIME_STR:&'static str="Invalid time in trajectory.";
pub const INIT_TRAJ_WRONG_MODE:u16=209;
pub const INIT_TRAJ_WRONG_MODE_STR:&'static str="Must call start_traj_mode service";
pub const INIT_TRAJ_BACKWARD_TIME:u16=210;
pub const INIT_TRAJ_BACKWARD_TIME_STR:&'static str="Trajectory message contains waypoints that are not strictly increasing in time.";
pub const INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS:u16=211;
pub const INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS_STR:&'static str="Trajectory did not contain position data for all axes.";
pub const INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES:u16=212;
pub const INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES_STR:&'static str="Trajectory did not contain velocity data for all axes.";
pub const INIT_TRAJ_INVALID_ENDING_VELOCITY:u16=213;
pub const INIT_TRAJ_INVALID_ENDING_VELOCITY_STR:&'static str="The final point in the trajectory must have zero velocity.";
pub const INIT_TRAJ_INVALID_ENDING_ACCELERATION:u16=214;
pub const INIT_TRAJ_INVALID_ENDING_ACCELERATION_STR:&'static str="The final point in the trajectory must have zero acceleration.";
pub const INIT_TRAJ_DUPLICATE_JOINT_NAME:u16=215;
pub const INIT_TRAJ_DUPLICATE_JOINT_NAME_STR:&'static str="The trajectory contains duplicate joint names.";}

impl Ros2Motoros2_interfacesMsgIoResultCodes {pub const OK:u32=0;
pub const OK_STR:&'static str="Success";
pub const READ_ADDRESS_INVALID:u32=1001;
pub const READ_ADDRESS_INVALID_STR:&'static str="Address cannot be read from (out of readable range)";
pub const WRITE_ADDRESS_INVALID:u32=1002;
pub const WRITE_ADDRESS_INVALID_STR:&'static str="Address cannot be written to (out of writable range)";
pub const WRITE_VALUE_INVALID:u32=1003;
pub const WRITE_VALUE_INVALID_STR:&'static str="Illegal value for the type of IO element addressed";
pub const READ_API_ERROR:u32=1004;
pub const READ_API_ERROR_STR:&'static str="The MotoPlus function mpReadIO returned -1. No further information is available";
pub const WRITE_API_ERROR:u32=1005;
pub const WRITE_API_ERROR_STR:&'static str="The MotoPlus function mpWriteIO returned -1. No further information is available";
pub const UNKNOWN_API_ERROR_STR:&'static str="Unknown error accessing I/O. No further information is available";}

impl Ros2Motoros2_interfacesMsgMotionReadyEnum {pub const READY:u16=1;
pub const READY_STR:&'static str="Ready";
pub const NOT_READY_UNSPECIFIED:u16=100;
pub const NOT_READY_UNSPECIFIED_STR:&'static str="Unspecified";
pub const NOT_READY_ALARM:u16=101;
pub const NOT_READY_ALARM_STR:&'static str="The controller has an active Alarm";
pub const NOT_READY_ERROR:u16=102;
pub const NOT_READY_ERROR_STR:&'static str="The controller has an active Error";
pub const NOT_READY_ESTOP:u16=103;
pub const NOT_READY_ESTOP_STR:&'static str="The controller is in E-Stop";
pub const NOT_READY_NOT_PLAY:u16=104;
pub const NOT_READY_NOT_PLAY_STR:&'static str="The teach pendant must not be in TEACH mode";
pub const NOT_READY_NOT_REMOTE:u16=105;
pub const NOT_READY_NOT_REMOTE_STR:&'static str="The teach pendant must be in REMOTE mode";
pub const NOT_READY_SERVO_OFF:u16=106;
pub const NOT_READY_SERVO_OFF_STR:&'static str="Servo power is OFF. Please call start_traj_mode or start_point_queue_mode service";
pub const NOT_READY_HOLD:u16=107;
pub const NOT_READY_HOLD_STR:&'static str="The controller is in HOLD";
pub const NOT_READY_JOB_NOT_STARTED:u16=108;
pub const NOT_READY_JOB_NOT_STARTED_STR:&'static str="The INIT_ROS job has not started. Please call start_traj_mode or start_point_queue_mode service";
pub const NOT_READY_NOT_ON_WAIT_CMD:u16=109;
pub const NOT_READY_NOT_ON_WAIT_CMD_STR:&'static str="INFORM job is not on a WAIT command. Check the format of INIT_ROS";
pub const NOT_READY_PFL_ACTIVE:u16=111;
pub const NOT_READY_PFL_ACTIVE_STR:&'static str="A PFL event has occurred. Please return the robot to a safe state";
pub const NOT_READY_INC_MOVE_ERROR:u16=112;
pub const NOT_READY_INC_MOVE_ERROR_STR:&'static str="The controller is in an error state. Please call reset_error to attempt to clear the error";
pub const NOT_READY_OTHER_PROGRAM_RUNNING:u16=113;
pub const NOT_READY_OTHER_PROGRAM_RUNNING_STR:&'static str="Controller is running another job";
pub const NOT_READY_OTHER_TRAJ_MODE_ACTIVE:u16=114;
pub const NOT_READY_OTHER_TRAJ_MODE_ACTIVE_STR:&'static str="Another trajectory mode is already active. Please call stop_traj_mode service";
pub const NOT_READY_NOT_CONT_CYCLE_MODE:u16=115;
pub const NOT_READY_NOT_CONT_CYCLE_MODE_STR:&'static str="AUTO cycle mode not set. Please set the cycle mode to AUTO";
pub const NOT_READY_MAJOR_ALARM:u16=116;
pub const NOT_READY_MAJOR_ALARM_STR:&'static str="Major Alarms can't be reset. Please check the teach pendant";
pub const NOT_READY_ECO_MODE:u16=117;
pub const NOT_READY_ECO_MODE_STR:&'static str="Couldn't disable eco mode";
pub const NOT_READY_SERVO_ON_TIMEOUT:u16=118;
pub const NOT_READY_SERVO_ON_TIMEOUT_STR:&'static str="Timed out while waiting for servo on";}

impl Ros2Motoros2_interfacesMsgQueueResultEnum {pub const SUCCESS:u16=1;
pub const SUCCESS_STR:&'static str="";
pub const WRONG_MODE:u16=2;
pub const WRONG_MODE_STR:&'static str="Must call start_point_queue_mode service";
pub const INIT_FAILURE:u16=3;
pub const INIT_FAILURE_STR:&'static str="Failed to initialize the streaming trajectory";
pub const BUSY:u16=4;
pub const BUSY_STR:&'static str="Busy processing the previous trajectory point. Please resend the next point shortly";
pub const INVALID_JOINT_LIST:u16=5;
pub const INVALID_JOINT_LIST_STR:&'static str="Point must contain data for all joints";
pub const UNABLE_TO_PROCESS_POINT:u16=6;
pub const UNABLE_TO_PROCESS_POINT_STR:&'static str="Error while processing the trajectory point. Check debug log for more details";}

impl Ros2Motoros2_interfacesMsgSelectionResultCodes {pub const OK:u32=0;
pub const OK_STR:&'static str="";
pub const INVALID_CONTROLLER_STATE:u32=400;
pub const INVALID_CONTROLLER_STATE_STR:&'static str="Pendant is not in REMOTE mode";
pub const INVALID_CONTROL_GROUP:u32=401;
pub const INVALID_CONTROL_GROUP_STR:&'static str="Invalid control group";
pub const INVALID_SELECTION_INDEX:u32=402;
pub const INVALID_SELECTION_INDEX_STR:&'static str="Invalid tool selection";}
