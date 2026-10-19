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
  pub const MAX_WIRE_BYTES:usize=280;
  pub const CARTESIAN_WIRE_BYTES:u64=280;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a512b971211baa8604440e02a046b586e7970293242607002ceb89196c9d99b1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.positions.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.positions {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocities.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocities {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.accelerations.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accelerations {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.time_from_start.wire_size()?)?;
if n>280{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.positions=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.positions.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocities.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.accelerations=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.accelerations.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>280{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=1484;
  pub const CARTESIAN_WIRE_BYTES:u64=1484;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e32bd7016b5fefcab6adeb42072e2d56c45d4b9f7cb372bdb5bfac97abf8cb77";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.points.len()>1{return Err(invalid());}n=add_size(n,4)?;
for element in &self.points {
n=add_size(n,element.wire_size()?)?;
}
if n>1484{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>1{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1484{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=844;
  pub const CARTESIAN_WIRE_BYTES:u64=844;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b4b6ba78f11918fcdc138544f7ef1a140d1fc51b385349ab1ad5716472c3daf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.transforms.len()>1{return Err(invalid());}n=add_size(n,4)?;
for element in &self.transforms {
n=add_size(n,element.wire_size()?)?;
}
if self.velocities.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocities {
n=add_size(n,element.wire_size()?)?;
}
if self.accelerations.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accelerations {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.time_from_start.wire_size()?)?;
if n>844{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>1{return Err(invalid());}
value.transforms=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTransform::read_fields(bytes,at)?;
value.transforms.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.velocities.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>844{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=2048;
  pub const CARTESIAN_WIRE_BYTES:u64=2048;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b822a2d5db9b83e916dcc9db4932952a314773ef53c9fbaf825902e0eb7c8cb5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.points.len()>1{return Err(invalid());}n=add_size(n,4)?;
for element in &self.points {
n=add_size(n,element.wire_size()?)?;
}
if n>2048{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>1{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2048{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=156;
  pub const CARTESIAN_WIRE_BYTES:u64=156;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>128||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.acceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>156{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&self.position.to_le_bytes());
put(out,at,&self.velocity.to_le_bytes());
put(out,at,&self.acceleration.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>156{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=158;
  pub const CARTESIAN_WIRE_BYTES:u64=158;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8a10f659b38feae6dfcf89818d35a164743b02c0e5cfe2d86aae7eda56e914fb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.joint_name.len()>128||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
n=add_size(n,2)?;
if !self.position.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.acceleration.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>158{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>158{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=8580;
  pub const CARTESIAN_WIRE_BYTES:u64=8580;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fa8334eb129cc91c50febaccfebb371f6fbdd82188d6301823344d6664300018";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trajectory.wire_size()?)?;
n=add_size(n,self.multi_dof_trajectory.wire_size()?)?;
if self.path_tolerance.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.component_path_tolerance.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.component_path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.goal_tolerance.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.component_goal_tolerance.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.component_goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.goal_time_tolerance.wire_size()?)?;
if n>8580{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.component_path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointComponentTolerance::read_fields(bytes,at)?;
value.component_path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.goal_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.goal_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8580{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<64{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=264;
  pub const CARTESIAN_WIRE_BYTES:u64=264;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_df1e08076a5c725b865cc8807462d9ccf9577df11cbb71fcc0d280d77d0e5649";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.error_string.len()>256||self.error_string.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_string.len())?;
if n>264{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.error_code.to_le_bytes());
put(out,at,&(self.error_string.len() as u32).to_le_bytes());put(out,at,self.error_string.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_code=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_string=decoded;
Ok(value)}
}
impl Canonical for Ros2Control_msgsActionFollowJointTrajectory_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>264{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=4708;
  pub const CARTESIAN_WIRE_BYTES:u64=4708;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ae2175027c3e45997d272b9610d15ef52cc665d23c0a371892d10b3d100e3529";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.desired.wire_size()?)?;
n=add_size(n,self.actual.wire_size()?)?;
n=add_size(n,self.error.wire_size()?)?;
if self.multi_dof_joint_names.len()>1{return Err(invalid());}n=add_size(n,4)?;
for element in &self.multi_dof_joint_names {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.multi_dof_desired.wire_size()?)?;
n=add_size(n,self.multi_dof_actual.wire_size()?)?;
n=add_size(n,self.multi_dof_error.wire_size()?)?;
if n>4708{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.desired=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.actual=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.error=decoded;
{let count=read_count(bytes,at)?;if count>1{return Err(invalid());}
value.multi_dof_joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4708{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<152{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=1404;
  pub const CARTESIAN_WIRE_BYTES:u64=1404;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bba20c36db70a98b594ba603d05b8a979038fb95b701221ba3b8689f55b06b11";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.name.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.name {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.position.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocity.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if (self.name.len()!=0&&self.position.len()!=0&&self.name.len()!=self.position.len())||(self.name.len()!=0&&self.velocity.len()!=0&&self.name.len()!=self.velocity.len())||(self.name.len()!=0&&self.effort.len()!=0&&self.name.len()!=self.effort.len())||(self.position.len()!=0&&self.velocity.len()!=0&&self.position.len()!=self.velocity.len())||(self.position.len()!=0&&self.effort.len()!=0&&self.position.len()!=self.effort.len())||(self.velocity.len()!=0&&self.effort.len()!=0&&self.velocity.len()!=self.effort.len()){return Err(invalid());}
if n>1404{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.name=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.position=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.velocity=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1404{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
