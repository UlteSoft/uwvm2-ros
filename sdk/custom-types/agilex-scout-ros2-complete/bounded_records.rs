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
  pub const MAX_WIRE_BYTES:usize=952;
  pub const CARTESIAN_WIRE_BYTES:u64=952;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fdfdd74c211c22bfd167c3e33a098fe21198cb095bbd0313fc765f38b1f334ec";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>128||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.pose.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>952{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>952{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<696{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 scout_msgs/msg/ScoutActuatorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Scout_msgsMsgScoutActuatorState {
  /// Declared rpm; canonical bounds are part of CODEC_IDENTITY.
  pub rpm:i16,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f64,
  /// Declared driver_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub driver_voltage:f32,
  /// Declared driver_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub driver_temperature:f32,
  /// Declared motor_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub motor_temperature:i8,
}
impl Default for Ros2Scout_msgsMsgScoutActuatorState {fn default()->Self {Self {
rpm:Default::default(),
current:Default::default(),
driver_voltage:Default::default(),
driver_temperature:Default::default(),
motor_temperature:Default::default(),
}}}
impl Ros2Scout_msgsMsgScoutActuatorState {
  pub const MAX_WIRE_BYTES:usize=19;
  pub const CARTESIAN_WIRE_BYTES:u64=19;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e7955c07aedaf42f8d97f873d60ae5c0dfc562faa1d64848f11b333f797350c6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,2)?;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.driver_voltage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.driver_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>19{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.rpm.to_le_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.driver_voltage.to_le_bytes());
put(out,at,&self.driver_temperature.to_le_bytes());
put(out,at,&self.motor_temperature.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.rpm=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.driver_voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.driver_temperature=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.motor_temperature=decoded;
Ok(value)}
}
impl Canonical for Ros2Scout_msgsMsgScoutActuatorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>19{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<19{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 scout_msgs/msg/ScoutLightCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Scout_msgsMsgScoutLightCmd {
  /// Declared cmd_ctrl_allowed; canonical bounds are part of CODEC_IDENTITY.
  pub cmd_ctrl_allowed:bool,
  /// Declared front_mode; canonical bounds are part of CODEC_IDENTITY.
  pub front_mode:u8,
  /// Declared front_custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub front_custom_value:u8,
  /// Declared rear_mode; canonical bounds are part of CODEC_IDENTITY.
  pub rear_mode:u8,
  /// Declared rear_custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub rear_custom_value:u8,
}
impl Default for Ros2Scout_msgsMsgScoutLightCmd {fn default()->Self {Self {
cmd_ctrl_allowed:Default::default(),
front_mode:Default::default(),
front_custom_value:Default::default(),
rear_mode:Default::default(),
rear_custom_value:Default::default(),
}}}
impl Ros2Scout_msgsMsgScoutLightCmd {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a82206813682fbf1a6b41009c12191ad9ce47c98444d8ea9792e7b212ee602d7";
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
put(out,at,&[u8::from(self.cmd_ctrl_allowed)]);
put(out,at,&self.front_mode.to_le_bytes());
put(out,at,&self.front_custom_value.to_le_bytes());
put(out,at,&self.rear_mode.to_le_bytes());
put(out,at,&self.rear_custom_value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.cmd_ctrl_allowed=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front_mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.front_custom_value=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.rear_mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.rear_custom_value=decoded;
Ok(value)}
}
impl Canonical for Ros2Scout_msgsMsgScoutLightCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 scout_msgs/msg/ScoutLightState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Scout_msgsMsgScoutLightState {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub custom_value:u8,
}
impl Default for Ros2Scout_msgsMsgScoutLightState {fn default()->Self {Self {
mode:Default::default(),
custom_value:Default::default(),
}}}
impl Ros2Scout_msgsMsgScoutLightState {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4f42b9365cc76673b156e725aac9448e0f0d806105d3717cdb2628c384a6d76a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.custom_value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.custom_value=decoded;
Ok(value)}
}
impl Canonical for Ros2Scout_msgsMsgScoutLightState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 scout_msgs/msg/ScoutRCState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Scout_msgsMsgScoutRCState {
  /// Declared swa; canonical bounds are part of CODEC_IDENTITY.
  pub swa:u8,
  /// Declared swb; canonical bounds are part of CODEC_IDENTITY.
  pub swb:u8,
  /// Declared swc; canonical bounds are part of CODEC_IDENTITY.
  pub swc:u8,
  /// Declared swd; canonical bounds are part of CODEC_IDENTITY.
  pub swd:u8,
  /// Declared stick_right_v; canonical bounds are part of CODEC_IDENTITY.
  pub stick_right_v:i8,
  /// Declared stick_right_h; canonical bounds are part of CODEC_IDENTITY.
  pub stick_right_h:i8,
  /// Declared stick_left_v; canonical bounds are part of CODEC_IDENTITY.
  pub stick_left_v:i8,
  /// Declared stick_left_h; canonical bounds are part of CODEC_IDENTITY.
  pub stick_left_h:i8,
  /// Declared var_a; canonical bounds are part of CODEC_IDENTITY.
  pub var_a:i8,
}
impl Default for Ros2Scout_msgsMsgScoutRCState {fn default()->Self {Self {
swa:Default::default(),
swb:Default::default(),
swc:Default::default(),
swd:Default::default(),
stick_right_v:Default::default(),
stick_right_h:Default::default(),
stick_left_v:Default::default(),
stick_left_h:Default::default(),
var_a:Default::default(),
}}}
impl Ros2Scout_msgsMsgScoutRCState {
  pub const MAX_WIRE_BYTES:usize=9;
  pub const CARTESIAN_WIRE_BYTES:u64=9;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ba13e4c728041d14352c021fe5c1c5eee5d531c84ac64c61d944d6192dcd4298";
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
put(out,at,&self.swa.to_le_bytes());
put(out,at,&self.swb.to_le_bytes());
put(out,at,&self.swc.to_le_bytes());
put(out,at,&self.swd.to_le_bytes());
put(out,at,&self.stick_right_v.to_le_bytes());
put(out,at,&self.stick_right_h.to_le_bytes());
put(out,at,&self.stick_left_v.to_le_bytes());
put(out,at,&self.stick_left_h.to_le_bytes());
put(out,at,&self.var_a.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swa=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swb=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swc=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swd=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_right_v=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_right_h=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_left_v=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_left_h=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.var_a=decoded;
Ok(value)}
}
impl Canonical for Ros2Scout_msgsMsgScoutRCState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 scout_msgs/msg/ScoutStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Scout_msgsMsgScoutStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared linear_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub linear_velocity:f64,
  /// Declared angular_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity:f64,
  /// Declared vehicle_state; canonical bounds are part of CODEC_IDENTITY.
  pub vehicle_state:u8,
  /// Declared control_mode; canonical bounds are part of CODEC_IDENTITY.
  pub control_mode:u8,
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:u16,
  /// Declared battery_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub battery_voltage:f64,
  /// Declared actuator_states; canonical bounds are part of CODEC_IDENTITY.
  pub actuator_states:[Ros2Scout_msgsMsgScoutActuatorState;4],
  /// Declared light_control_enabled; canonical bounds are part of CODEC_IDENTITY.
  pub light_control_enabled:bool,
  /// Declared front_light_state; canonical bounds are part of CODEC_IDENTITY.
  pub front_light_state:Ros2Scout_msgsMsgScoutLightState,
  /// Declared rear_light_state; canonical bounds are part of CODEC_IDENTITY.
  pub rear_light_state:Ros2Scout_msgsMsgScoutLightState,
}
impl Default for Ros2Scout_msgsMsgScoutStatus {fn default()->Self {Self {
header:Default::default(),
linear_velocity:Default::default(),
angular_velocity:Default::default(),
vehicle_state:Default::default(),
control_mode:Default::default(),
error_code:Default::default(),
battery_voltage:Default::default(),
actuator_states:core::array::from_fn(|_|Default::default()),
light_control_enabled:Default::default(),
front_light_state:Default::default(),
rear_light_state:Default::default(),
}}}
impl Ros2Scout_msgsMsgScoutStatus {
  pub const MAX_WIRE_BYTES:usize=249;
  pub const CARTESIAN_WIRE_BYTES:u64=249;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_75db4db1dcc07210d543b8e6bd6b6a7aadbf23db8043ff372a3b224c5c235b86";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if !self.linear_velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.angular_velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
if !self.battery_voltage.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.actuator_states {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,1)?;
n=add_size(n,self.front_light_state.wire_size()?)?;
n=add_size(n,self.rear_light_state.wire_size()?)?;
if n>249{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.linear_velocity.to_le_bytes());
put(out,at,&self.angular_velocity.to_le_bytes());
put(out,at,&self.vehicle_state.to_le_bytes());
put(out,at,&self.control_mode.to_le_bytes());
put(out,at,&self.error_code.to_le_bytes());
put(out,at,&self.battery_voltage.to_le_bytes());
for element in &self.actuator_states {
element.write_fields(out,at);
}
put(out,at,&[u8::from(self.light_control_enabled)]);
self.front_light_state.write_fields(out,at);
self.rear_light_state.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.linear_velocity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angular_velocity=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.vehicle_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.control_mode=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.error_code=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery_voltage=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Scout_msgsMsgScoutActuatorState::read_fields(bytes,at)?;
value.actuator_states[i]=decoded;
}}
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.light_control_enabled=decoded;
let decoded=Ros2Scout_msgsMsgScoutLightState::read_fields(bytes,at)?;
value.front_light_state=decoded;
let decoded=Ros2Scout_msgsMsgScoutLightState::read_fields(bytes,at)?;
value.rear_light_state=decoded;
Ok(value)}
}
impl Canonical for Ros2Scout_msgsMsgScoutStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>249{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<121{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Scout_msgsMsgScoutLightCmd {pub const LIGHT_CONST_OFF:u8=0;
pub const LIGHT_CONST_ON:u8=1;
pub const LIGHT_BREATH:u8=2;
pub const LIGHT_CUSTOM:u8=3;}

impl Ros2Scout_msgsMsgScoutStatus {pub const MOTOR_ID_FRONT_RIGHT:i8=0;
pub const MOTOR_ID_FRONT_LEFT:i8=1;
pub const MOTOR_ID_REAR_RIGHT:i8=2;
pub const MOTOR_ID_REAR_LEFT:i8=3;
pub const LIGHT_ID_FRONT:i8=0;
pub const LIGHT_ID_REAR:i8=1;}
