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
/// Complete bounded value for ROS2 geometry_msgs/msg/PoseWithCovariance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgPoseWithCovariance {
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros2Geometry_msgsMsgPose,
  /// Declared covariance; canonical bounds are part of CODEC_IDENTITY.
  pub covariance:[f64;36],
}
impl Default for Ros2Geometry_msgsMsgPoseWithCovariance {fn default()->Self {Self {
pose:Default::default(),
covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Geometry_msgsMsgPoseWithCovariance {
  pub const MAX_WIRE_BYTES:usize=344;
  pub const CARTESIAN_WIRE_BYTES:u64=344;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8eb3e25c45769eb29691011c05315ed50f2b97eff488f9e8ee2dc70bddab4dc2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.pose.wire_size()?)?;
for element in &self.covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>344{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.pose.write_fields(out,at);
for element in &self.covariance {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgPose::read_fields(bytes,at)?;
value.pose=decoded;
{let count=36usize;if count>36{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgPoseWithCovariance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>344{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<344{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 geometry_msgs/msg/PoseWithCovarianceStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgPoseWithCovarianceStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros2Geometry_msgsMsgPoseWithCovariance,
}
impl Default for Ros2Geometry_msgsMsgPoseWithCovarianceStamped {fn default()->Self {Self {
header:Default::default(),
pose:Default::default(),
}}}
impl Ros2Geometry_msgsMsgPoseWithCovarianceStamped {
  pub const MAX_WIRE_BYTES:usize=420;
  pub const CARTESIAN_WIRE_BYTES:u64=420;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ea380172cde0ee5fef319aff50d92a80c314510e3d12efec45e9ae1f74974af3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.pose.wire_size()?)?;
if n>420{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.pose.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgPoseWithCovariance::read_fields(bytes,at)?;
value.pose=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgPoseWithCovarianceStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>420{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<356{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 isaac_ros_apriltag_interfaces/msg/AprilTagDetection.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection {
  /// Declared family; canonical bounds are part of CODEC_IDENTITY.
  pub family:String,
  /// Declared id; canonical bounds are part of CODEC_IDENTITY.
  pub id:i32,
  /// Declared center; canonical bounds are part of CODEC_IDENTITY.
  pub center:Ros2Geometry_msgsMsgPoint,
  /// Declared corners; canonical bounds are part of CODEC_IDENTITY.
  pub corners:[Ros2Geometry_msgsMsgPoint;4],
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros2Geometry_msgsMsgPoseWithCovarianceStamped,
}
impl Default for Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection {fn default()->Self {Self {
family:Default::default(),
id:Default::default(),
center:Default::default(),
corners:core::array::from_fn(|_|Default::default()),
pose:Default::default(),
}}}
impl Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection {
  pub const MAX_WIRE_BYTES:usize=612;
  pub const CARTESIAN_WIRE_BYTES:u64=612;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d840f0d9239a948ce5d17bbee6e248db8151ff31070667e506e9c1647ac6fe7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.family.len()>64||self.family.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.family.len())?;
n=add_size(n,4)?;
n=add_size(n,self.center.wire_size()?)?;
for element in &self.corners {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.pose.wire_size()?)?;
if n>612{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.family.len() as u32).to_le_bytes());put(out,at,self.family.as_bytes());
put(out,at,&self.id.to_le_bytes());
self.center.write_fields(out,at);
for element in &self.corners {
element.write_fields(out,at);
}
self.pose.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.family=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.id=decoded;
let decoded=Ros2Geometry_msgsMsgPoint::read_fields(bytes,at)?;
value.center=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Geometry_msgsMsgPoint::read_fields(bytes,at)?;
value.corners[i]=decoded;
}}
let decoded=Ros2Geometry_msgsMsgPoseWithCovarianceStamped::read_fields(bytes,at)?;
value.pose=decoded;
Ok(value)}
}
impl Canonical for Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>612{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<484{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared detections; canonical bounds are part of CODEC_IDENTITY.
  pub detections:Vec<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>,
}
impl Default for Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray {fn default()->Self {Self {
header:Default::default(),
detections:Default::default(),
}}}
impl Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray {
  pub const MAX_WIRE_BYTES:usize=9872;
  pub const CARTESIAN_WIRE_BYTES:u64=9872;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_60359e405aedd2a627e60bc35e849989fe5ed1d426afb2a884036aef93bfe0a5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.detections.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.detections {
n=add_size(n,element.wire_size()?)?;
}
if n>9872{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.detections.len() as u32).to_le_bytes());
for element in &self.detections {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.detections=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection::read_fields(bytes,at)?;
value.detections.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9872{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
