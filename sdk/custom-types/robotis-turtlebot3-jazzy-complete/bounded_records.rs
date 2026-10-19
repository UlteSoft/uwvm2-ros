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
  pub const MAX_WIRE_BYTES:usize=456;
  pub const CARTESIAN_WIRE_BYTES:u64=456;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1cb6033612e0632fe87e6cf44bf5a899220370c372ba25b989a066171925ee7a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>256||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.transform.wire_size()?)?;
if n>456{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.child_frame_id=decoded;
let decoded=Ros2Geometry_msgsMsgTransform::read_fields(bytes,at)?;
value.transform=decoded;
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTransformStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>456{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<72{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 geometry_msgs/msg/TwistWithCovariance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Geometry_msgsMsgTwistWithCovariance {
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros2Geometry_msgsMsgTwist,
  /// Declared covariance; canonical bounds are part of CODEC_IDENTITY.
  pub covariance:[f64;36],
}
impl Default for Ros2Geometry_msgsMsgTwistWithCovariance {fn default()->Self {Self {
twist:Default::default(),
covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Geometry_msgsMsgTwistWithCovariance {
  pub const MAX_WIRE_BYTES:usize=336;
  pub const CARTESIAN_WIRE_BYTES:u64=336;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_96c9fddad7b7c240e850d8c27ddb5f8343764cac791ef9e210196f7941aaf404";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.twist.wire_size()?)?;
for element in &self.covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>336{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.twist.write_fields(out,at);
for element in &self.covariance {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.twist=decoded;
{let count=36usize;if count>36{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Geometry_msgsMsgTwistWithCovariance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>336{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<336{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 nav_msgs/msg/Odometry.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Nav_msgsMsgOdometry {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared child_frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub child_frame_id:String,
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros2Geometry_msgsMsgPoseWithCovariance,
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros2Geometry_msgsMsgTwistWithCovariance,
}
impl Default for Ros2Nav_msgsMsgOdometry {fn default()->Self {Self {
header:Default::default(),
child_frame_id:Default::default(),
pose:Default::default(),
twist:Default::default(),
}}}
impl Ros2Nav_msgsMsgOdometry {
  pub const MAX_WIRE_BYTES:usize=1080;
  pub const CARTESIAN_WIRE_BYTES:u64=1080;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4eb9aa4edb3d288080deaae0326c26a75923e69d40c9b10219baf6d441ea8111";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>256||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.pose.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>1080{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.child_frame_id.len() as u32).to_le_bytes());put(out,at,self.child_frame_id.as_bytes());
self.pose.write_fields(out,at);
self.twist.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.child_frame_id=decoded;
let decoded=Ros2Geometry_msgsMsgPoseWithCovariance::read_fields(bytes,at)?;
value.pose=decoded;
let decoded=Ros2Geometry_msgsMsgTwistWithCovariance::read_fields(bytes,at)?;
value.twist=decoded;
Ok(value)}
}
impl Canonical for Ros2Nav_msgsMsgOdometry {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1080{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<696{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 sensor_msgs/msg/BatteryState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Sensor_msgsMsgBatteryState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared voltage; canonical bounds are part of CODEC_IDENTITY.
  pub voltage:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:f32,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f32,
  /// Declared charge; canonical bounds are part of CODEC_IDENTITY.
  pub charge:f32,
  /// Declared capacity; canonical bounds are part of CODEC_IDENTITY.
  pub capacity:f32,
  /// Declared design_capacity; canonical bounds are part of CODEC_IDENTITY.
  pub design_capacity:f32,
  /// Declared percentage; canonical bounds are part of CODEC_IDENTITY.
  pub percentage:f32,
  /// Declared power_supply_status; canonical bounds are part of CODEC_IDENTITY.
  pub power_supply_status:u8,
  /// Declared power_supply_health; canonical bounds are part of CODEC_IDENTITY.
  pub power_supply_health:u8,
  /// Declared power_supply_technology; canonical bounds are part of CODEC_IDENTITY.
  pub power_supply_technology:u8,
  /// Declared present; canonical bounds are part of CODEC_IDENTITY.
  pub present:bool,
  /// Declared cell_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub cell_voltage:Vec<f32>,
  /// Declared cell_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub cell_temperature:Vec<f32>,
  /// Declared location; canonical bounds are part of CODEC_IDENTITY.
  pub location:String,
  /// Declared serial_number; canonical bounds are part of CODEC_IDENTITY.
  pub serial_number:String,
}
impl Default for Ros2Sensor_msgsMsgBatteryState {fn default()->Self {Self {
header:Default::default(),
voltage:Default::default(),
temperature:Default::default(),
current:Default::default(),
charge:Default::default(),
capacity:Default::default(),
design_capacity:Default::default(),
percentage:Default::default(),
power_supply_status:Default::default(),
power_supply_health:Default::default(),
power_supply_technology:Default::default(),
present:Default::default(),
cell_voltage:Default::default(),
cell_temperature:Default::default(),
location:Default::default(),
serial_number:Default::default(),
}}}
impl Ros2Sensor_msgsMsgBatteryState {
  pub const MAX_WIRE_BYTES:usize=828;
  pub const CARTESIAN_WIRE_BYTES:u64=828;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9083c57dde00bba912db4dd736027cb92bbed2b1a79f965570dee86e1ff5a96e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if !self.voltage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.charge.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.capacity.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.design_capacity.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.percentage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if self.cell_voltage.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.cell_voltage {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.cell_temperature.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.cell_temperature {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.location.len()>256||self.location.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.location.len())?;
if self.serial_number.len()>256||self.serial_number.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.serial_number.len())?;
if n>828{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.voltage.to_le_bytes());
put(out,at,&self.temperature.to_le_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.charge.to_le_bytes());
put(out,at,&self.capacity.to_le_bytes());
put(out,at,&self.design_capacity.to_le_bytes());
put(out,at,&self.percentage.to_le_bytes());
put(out,at,&self.power_supply_status.to_le_bytes());
put(out,at,&self.power_supply_health.to_le_bytes());
put(out,at,&self.power_supply_technology.to_le_bytes());
put(out,at,&[u8::from(self.present)]);
put(out,at,&(self.cell_voltage.len() as u32).to_le_bytes());
for element in &self.cell_voltage {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.cell_temperature.len() as u32).to_le_bytes());
for element in &self.cell_temperature {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.location.len() as u32).to_le_bytes());put(out,at,self.location.as_bytes());
put(out,at,&(self.serial_number.len() as u32).to_le_bytes());put(out,at,self.serial_number.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.charge=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.capacity=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.design_capacity=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.percentage=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.power_supply_status=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.power_supply_health=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.power_supply_technology=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.present=decoded;
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.cell_voltage=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cell_voltage.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.cell_temperature=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cell_temperature.push(decoded);
}}
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.location=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.serial_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Sensor_msgsMsgBatteryState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>828{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<60{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 sensor_msgs/msg/Imu.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Sensor_msgsMsgImu {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared orientation; canonical bounds are part of CODEC_IDENTITY.
  pub orientation:Ros2Geometry_msgsMsgQuaternion,
  /// Declared orientation_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub orientation_covariance:[f64;9],
  /// Declared angular_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity:Ros2Geometry_msgsMsgVector3,
  /// Declared angular_velocity_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity_covariance:[f64;9],
  /// Declared linear_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub linear_acceleration:Ros2Geometry_msgsMsgVector3,
  /// Declared linear_acceleration_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub linear_acceleration_covariance:[f64;9],
}
impl Default for Ros2Sensor_msgsMsgImu {fn default()->Self {Self {
header:Default::default(),
orientation:Default::default(),
orientation_covariance:core::array::from_fn(|_|Default::default()),
angular_velocity:Default::default(),
angular_velocity_covariance:core::array::from_fn(|_|Default::default()),
linear_acceleration:Default::default(),
linear_acceleration_covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Sensor_msgsMsgImu {
  pub const MAX_WIRE_BYTES:usize=436;
  pub const CARTESIAN_WIRE_BYTES:u64=436;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e38d1c936a2a4a870b2f069af8d1796f69e6ce7b8cae0a792925ed2f6c0cae1b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.orientation.wire_size()?)?;
for element in &self.orientation_covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.angular_velocity.wire_size()?)?;
for element in &self.angular_velocity_covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.linear_acceleration.wire_size()?)?;
for element in &self.linear_acceleration_covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>436{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.orientation.write_fields(out,at);
for element in &self.orientation_covariance {
put(out,at,&element.to_le_bytes());
}
self.angular_velocity.write_fields(out,at);
for element in &self.angular_velocity_covariance {
put(out,at,&element.to_le_bytes());
}
self.linear_acceleration.write_fields(out,at);
for element in &self.linear_acceleration_covariance {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgQuaternion::read_fields(bytes,at)?;
value.orientation=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.orientation_covariance[i]=decoded;
}}
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.angular_velocity=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angular_velocity_covariance[i]=decoded;
}}
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.linear_acceleration=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.linear_acceleration_covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Sensor_msgsMsgImu {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>436{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<308{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=468;
  pub const CARTESIAN_WIRE_BYTES:u64=468;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5c4d1ef0128c748e20f6b30c9c3bb5ef6e3c529a0b05b81764ac4774ce765bcf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.name.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.name {
if element.len()>128||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.position.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocity.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if (self.name.len()!=0&&self.position.len()!=0&&self.name.len()!=self.position.len())||(self.name.len()!=0&&self.velocity.len()!=0&&self.name.len()!=self.velocity.len())||(self.name.len()!=0&&self.effort.len()!=0&&self.name.len()!=self.effort.len())||(self.position.len()!=0&&self.velocity.len()!=0&&self.position.len()!=self.velocity.len())||(self.position.len()!=0&&self.effort.len()!=0&&self.position.len()!=self.effort.len())||(self.velocity.len()!=0&&self.effort.len()!=0&&self.velocity.len()!=self.effort.len()){return Err(invalid());}
if n>468{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.name=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.position=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.velocity=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>468{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 sensor_msgs/msg/LaserScan.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Sensor_msgsMsgLaserScan {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared angle_min; canonical bounds are part of CODEC_IDENTITY.
  pub angle_min:f32,
  /// Declared angle_max; canonical bounds are part of CODEC_IDENTITY.
  pub angle_max:f32,
  /// Declared angle_increment; canonical bounds are part of CODEC_IDENTITY.
  pub angle_increment:f32,
  /// Declared time_increment; canonical bounds are part of CODEC_IDENTITY.
  pub time_increment:f32,
  /// Declared scan_time; canonical bounds are part of CODEC_IDENTITY.
  pub scan_time:f32,
  /// Declared range_min; canonical bounds are part of CODEC_IDENTITY.
  pub range_min:f32,
  /// Declared range_max; canonical bounds are part of CODEC_IDENTITY.
  pub range_max:f32,
  /// Declared ranges; canonical bounds are part of CODEC_IDENTITY.
  pub ranges:Vec<f32>,
  /// Declared intensities; canonical bounds are part of CODEC_IDENTITY.
  pub intensities:Vec<f32>,
}
impl Default for Ros2Sensor_msgsMsgLaserScan {fn default()->Self {Self {
header:Default::default(),
angle_min:Default::default(),
angle_max:Default::default(),
angle_increment:Default::default(),
time_increment:Default::default(),
scan_time:Default::default(),
range_min:Default::default(),
range_max:Default::default(),
ranges:Default::default(),
intensities:Default::default(),
}}}
impl Ros2Sensor_msgsMsgLaserScan {
  pub const MAX_WIRE_BYTES:usize=8368;
  pub const CARTESIAN_WIRE_BYTES:u64=8368;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_89faa8e14fc7f79a62a576d35c0bb377caf0cbaef6a6f181a5af2da0ead6cce4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if !self.angle_min.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.angle_max.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.angle_increment.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.time_increment.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.scan_time.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.range_min.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.range_max.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if self.ranges.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.ranges {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.intensities.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.intensities {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>8368{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.angle_min.to_le_bytes());
put(out,at,&self.angle_max.to_le_bytes());
put(out,at,&self.angle_increment.to_le_bytes());
put(out,at,&self.time_increment.to_le_bytes());
put(out,at,&self.scan_time.to_le_bytes());
put(out,at,&self.range_min.to_le_bytes());
put(out,at,&self.range_max.to_le_bytes());
put(out,at,&(self.ranges.len() as u32).to_le_bytes());
for element in &self.ranges {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.intensities.len() as u32).to_le_bytes());
for element in &self.intensities {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angle_min=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angle_max=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angle_increment=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.time_increment=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.scan_time=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.range_min=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.range_max=decoded;
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.ranges=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ranges.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.intensities=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.intensities.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Sensor_msgsMsgLaserScan {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8368{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 sensor_msgs/msg/MagneticField.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Sensor_msgsMsgMagneticField {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared magnetic_field; canonical bounds are part of CODEC_IDENTITY.
  pub magnetic_field:Ros2Geometry_msgsMsgVector3,
  /// Declared magnetic_field_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub magnetic_field_covariance:[f64;9],
}
impl Default for Ros2Sensor_msgsMsgMagneticField {fn default()->Self {Self {
header:Default::default(),
magnetic_field:Default::default(),
magnetic_field_covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Sensor_msgsMsgMagneticField {
  pub const MAX_WIRE_BYTES:usize=236;
  pub const CARTESIAN_WIRE_BYTES:u64=236;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_56ae1437e0fbe7a6722ecf4795483d15f4c616e2e891372c30fa78f18be2cf8b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.magnetic_field.wire_size()?)?;
for element in &self.magnetic_field_covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>236{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.magnetic_field.write_fields(out,at);
for element in &self.magnetic_field_covariance {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.magnetic_field=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.magnetic_field_covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Sensor_msgsMsgMagneticField {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>236{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<108{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 std_srvs/srv/SetBool_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Std_srvsSrvSetBool_Request {
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:bool,
}
impl Default for Ros2Std_srvsSrvSetBool_Request {fn default()->Self {Self {
data:Default::default(),
}}}
impl Ros2Std_srvsSrvSetBool_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2523e7915bb78fe01b68f2a856611b32a88f624a6e197a6ff312b613e8c65910";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.data)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.data=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_srvsSrvSetBool_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 std_srvs/srv/SetBool_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Std_srvsSrvSetBool_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Std_srvsSrvSetBool_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Std_srvsSrvSetBool_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f2d46a8440538094221a151716d8ec44db02455f59845c50aaf4e7964c27ebe4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_srvsSrvSetBool_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_58806c592727ad1011b32edb6bca3ca32d8e5dbe7f2fa20c86220daf64a10392";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Std_srvsSrvTrigger_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/action/Patrol_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsActionPatrol_Goal {
  /// Declared goal; canonical bounds are part of CODEC_IDENTITY.
  pub goal:Ros2Geometry_msgsMsgVector3,
}
impl Default for Ros2Turtlebot3_msgsActionPatrol_Goal {fn default()->Self {Self {
goal:Default::default(),
}}}
impl Ros2Turtlebot3_msgsActionPatrol_Goal {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_929abdbc422698464043e7e8d12c2a5edd418dcd52bbde4a542b33e6862ba96f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.goal.wire_size()?)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.goal.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.goal=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsActionPatrol_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/action/Patrol_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsActionPatrol_Result {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:String,
}
impl Default for Ros2Turtlebot3_msgsActionPatrol_Result {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros2Turtlebot3_msgsActionPatrol_Result {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1d1fa277be54e1df3bd314a632ff80d79f22a306fc232219a7c23cce7a7703a3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.result.len()>256||self.result.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.result.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.result.len() as u32).to_le_bytes());put(out,at,self.result.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.result=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsActionPatrol_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/action/Patrol_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsActionPatrol_Feedback {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
}
impl Default for Ros2Turtlebot3_msgsActionPatrol_Feedback {fn default()->Self {Self {
state:Default::default(),
}}}
impl Ros2Turtlebot3_msgsActionPatrol_Feedback {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7c2f3824653a3048e1b93fa95584543a01efae8bb67253d3371a2988407a601e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>256||self.state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.state.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());put(out,at,self.state.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsActionPatrol_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/msg/SensorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsMsgSensorState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared bumper; canonical bounds are part of CODEC_IDENTITY.
  pub bumper:u8,
  /// Declared cliff; canonical bounds are part of CODEC_IDENTITY.
  pub cliff:f32,
  /// Declared sonar; canonical bounds are part of CODEC_IDENTITY.
  pub sonar:f32,
  /// Declared illumination; canonical bounds are part of CODEC_IDENTITY.
  pub illumination:f32,
  /// Declared led; canonical bounds are part of CODEC_IDENTITY.
  pub led:u8,
  /// Declared button; canonical bounds are part of CODEC_IDENTITY.
  pub button:u8,
  /// Declared torque; canonical bounds are part of CODEC_IDENTITY.
  pub torque:bool,
  /// Declared left_encoder; canonical bounds are part of CODEC_IDENTITY.
  pub left_encoder:i32,
  /// Declared right_encoder; canonical bounds are part of CODEC_IDENTITY.
  pub right_encoder:i32,
  /// Declared battery; canonical bounds are part of CODEC_IDENTITY.
  pub battery:f32,
}
impl Default for Ros2Turtlebot3_msgsMsgSensorState {fn default()->Self {Self {
header:Default::default(),
bumper:Default::default(),
cliff:Default::default(),
sonar:Default::default(),
illumination:Default::default(),
led:Default::default(),
button:Default::default(),
torque:Default::default(),
left_encoder:Default::default(),
right_encoder:Default::default(),
battery:Default::default(),
}}}
impl Ros2Turtlebot3_msgsMsgSensorState {
  pub const MAX_WIRE_BYTES:usize=168;
  pub const CARTESIAN_WIRE_BYTES:u64=168;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7974ef7ca70ababf0e3f7a5e4099335a1398933833260798482f155291a62358";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
if !self.cliff.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.sonar.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.illumination.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if !self.battery.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>168{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.bumper.to_le_bytes());
put(out,at,&self.cliff.to_le_bytes());
put(out,at,&self.sonar.to_le_bytes());
put(out,at,&self.illumination.to_le_bytes());
put(out,at,&self.led.to_le_bytes());
put(out,at,&self.button.to_le_bytes());
put(out,at,&[u8::from(self.torque)]);
put(out,at,&self.left_encoder.to_le_bytes());
put(out,at,&self.right_encoder.to_le_bytes());
put(out,at,&self.battery.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.bumper=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cliff=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.sonar=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.illumination=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.led=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.button=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.torque=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.left_encoder=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.right_encoder=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsMsgSensorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>168{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/msg/Sound.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsMsgSound {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
}
impl Default for Ros2Turtlebot3_msgsMsgSound {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Turtlebot3_msgsMsgSound {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e8bd9a5fdf84ad1df0f264bd13a0e8966918de19ab62e91c7524dd8fc17a4e4b";
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
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsMsgSound {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/msg/VersionInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsMsgVersionInfo {
  /// Declared hardware; canonical bounds are part of CODEC_IDENTITY.
  pub hardware:String,
  /// Declared firmware; canonical bounds are part of CODEC_IDENTITY.
  pub firmware:String,
  /// Declared software; canonical bounds are part of CODEC_IDENTITY.
  pub software:String,
}
impl Default for Ros2Turtlebot3_msgsMsgVersionInfo {fn default()->Self {Self {
hardware:Default::default(),
firmware:Default::default(),
software:Default::default(),
}}}
impl Ros2Turtlebot3_msgsMsgVersionInfo {
  pub const MAX_WIRE_BYTES:usize=780;
  pub const CARTESIAN_WIRE_BYTES:u64=780;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7668c77b01a05eeb8ba3c5f259f3f3a9dfad3d4fdf8aa566d65f6e8f1c0f2d6a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.hardware.len()>256||self.hardware.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.hardware.len())?;
if self.firmware.len()>256||self.firmware.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware.len())?;
if self.software.len()>256||self.software.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.software.len())?;
if n>780{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.hardware.len() as u32).to_le_bytes());put(out,at,self.hardware.as_bytes());
put(out,at,&(self.firmware.len() as u32).to_le_bytes());put(out,at,self.firmware.as_bytes());
put(out,at,&(self.software.len() as u32).to_le_bytes());put(out,at,self.software.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.hardware=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.firmware=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.software=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsMsgVersionInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>780{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Dqn_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvDqn_Request {
  /// Declared action; canonical bounds are part of CODEC_IDENTITY.
  pub action:u8,
  /// Declared init; canonical bounds are part of CODEC_IDENTITY.
  pub init:bool,
}
impl Default for Ros2Turtlebot3_msgsSrvDqn_Request {fn default()->Self {Self {
action:Default::default(),
init:Default::default(),
}}}
impl Ros2Turtlebot3_msgsSrvDqn_Request {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_14042e65db9f4feef7a43bfb44255795470bb0ed91a3cf301ebec98c2d4e2d0e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.action.to_le_bytes());
put(out,at,&[u8::from(self.init)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.action=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.init=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsSrvDqn_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Dqn_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvDqn_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:Vec<f32>,
  /// Declared reward; canonical bounds are part of CODEC_IDENTITY.
  pub reward:f32,
  /// Declared done; canonical bounds are part of CODEC_IDENTITY.
  pub done:bool,
}
impl Default for Ros2Turtlebot3_msgsSrvDqn_Response {fn default()->Self {Self {
state:Default::default(),
reward:Default::default(),
done:Default::default(),
}}}
impl Ros2Turtlebot3_msgsSrvDqn_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d256e9c87d7959917f07e7cfcc0049fc18f79f547d83b72bd388a63c1417c6e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.state.len()>128{return Err(invalid());}n=add_size(n,4)?;
for element in &self.state {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.reward.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.state.len() as u32).to_le_bytes());
for element in &self.state {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.reward.to_le_bytes());
put(out,at,&[u8::from(self.done)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>128{return Err(invalid());}
value.state=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.state.push(decoded);
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.reward=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.done=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsSrvDqn_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Goal_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvGoal_Request {
}
impl Default for Ros2Turtlebot3_msgsSrvGoal_Request {fn default()->Self {Self {
}}}
impl Ros2Turtlebot3_msgsSrvGoal_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f76094f3cf00772bb80b59e846f1160ae6255fc0d931bbcba362104081698ded";
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
impl Canonical for Ros2Turtlebot3_msgsSrvGoal_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Goal_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvGoal_Response {
  /// Declared pose_x; canonical bounds are part of CODEC_IDENTITY.
  pub pose_x:f32,
  /// Declared pose_y; canonical bounds are part of CODEC_IDENTITY.
  pub pose_y:f32,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Turtlebot3_msgsSrvGoal_Response {fn default()->Self {Self {
pose_x:Default::default(),
pose_y:Default::default(),
success:Default::default(),
}}}
impl Ros2Turtlebot3_msgsSrvGoal_Response {
  pub const MAX_WIRE_BYTES:usize=9;
  pub const CARTESIAN_WIRE_BYTES:u64=9;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8b9022a7fb8c68a0b063b080cd05c230b653169badbe205bcd8051b8020f7990";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.pose_x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.pose_y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>9{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.pose_x.to_le_bytes());
put(out,at,&self.pose_y.to_le_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pose_x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pose_y=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsSrvGoal_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Sound_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvSound_Request {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
}
impl Default for Ros2Turtlebot3_msgsSrvSound_Request {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros2Turtlebot3_msgsSrvSound_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0dcba95f5ba646d9a24b0054ba603daf1d1e5e4e9a6ffe7387cd61bff2e4151a";
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
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsSrvSound_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 turtlebot3_msgs/srv/Sound_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Turtlebot3_msgsSrvSound_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Turtlebot3_msgsSrvSound_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Turtlebot3_msgsSrvSound_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bc5a5fd3e2bf733a278b8a1315c8905b27fff49b4844df238d5574149068998e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Turtlebot3_msgsSrvSound_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Sensor_msgsMsgBatteryState {pub const POWER_SUPPLY_STATUS_UNKNOWN:u8=0;
pub const POWER_SUPPLY_STATUS_CHARGING:u8=1;
pub const POWER_SUPPLY_STATUS_DISCHARGING:u8=2;
pub const POWER_SUPPLY_STATUS_NOT_CHARGING:u8=3;
pub const POWER_SUPPLY_STATUS_FULL:u8=4;
pub const POWER_SUPPLY_HEALTH_UNKNOWN:u8=0;
pub const POWER_SUPPLY_HEALTH_GOOD:u8=1;
pub const POWER_SUPPLY_HEALTH_OVERHEAT:u8=2;
pub const POWER_SUPPLY_HEALTH_DEAD:u8=3;
pub const POWER_SUPPLY_HEALTH_OVERVOLTAGE:u8=4;
pub const POWER_SUPPLY_HEALTH_UNSPEC_FAILURE:u8=5;
pub const POWER_SUPPLY_HEALTH_COLD:u8=6;
pub const POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE:u8=7;
pub const POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE:u8=8;
pub const POWER_SUPPLY_TECHNOLOGY_UNKNOWN:u8=0;
pub const POWER_SUPPLY_TECHNOLOGY_NIMH:u8=1;
pub const POWER_SUPPLY_TECHNOLOGY_LION:u8=2;
pub const POWER_SUPPLY_TECHNOLOGY_LIPO:u8=3;
pub const POWER_SUPPLY_TECHNOLOGY_LIFE:u8=4;
pub const POWER_SUPPLY_TECHNOLOGY_NICD:u8=5;
pub const POWER_SUPPLY_TECHNOLOGY_LIMN:u8=6;
pub const POWER_SUPPLY_TECHNOLOGY_TERNARY:u8=7;
pub const POWER_SUPPLY_TECHNOLOGY_VRLA:u8=8;}

impl Ros2Turtlebot3_msgsMsgSensorState {pub const BUMPER_FORWARD:u8=1;
pub const BUMPER_BACKWARD:u8=2;
pub const CLIFF:u8=1;
pub const SONAR:u8=1;
pub const ILLUMINATION:u8=1;
pub const BUTTON0:u8=1;
pub const BUTTON1:u8=2;
pub const ERROR_LEFT_MOTOR:u8=1;
pub const ERROR_RIGHT_MOTOR:u8=2;
pub const TORQUE_ON:u8=1;
pub const TORQUE_OFF:u8=2;}

impl Ros2Turtlebot3_msgsMsgSound {pub const OFF:u8=0;
pub const ON:u8=1;
pub const LOW_BATTERY:u8=2;
pub const ERROR:u8=3;
pub const BUTTON1:u8=4;
pub const BUTTON2:u8=5;}
