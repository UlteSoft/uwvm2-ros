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
  pub const MAX_WIRE_BYTES:usize=216;
  pub const CARTESIAN_WIRE_BYTES:u64=216;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c4eafb6efd22d299b5c6fc21844d8bf69789cf27ee5414a1d7d2371f0b1acbbf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.positions.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.positions {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.velocities.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.velocities {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.accelerations.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accelerations {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.effort.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.effort {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
n=add_size(n,self.time_from_start.wire_size()?)?;
if n>216{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.positions=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.positions.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.velocities=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocities.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.accelerations=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.accelerations.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>216{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const MAX_WIRE_BYTES:usize=5164;
  pub const CARTESIAN_WIRE_BYTES:u64=5164;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e77c37c75123e442edb6a7f6f2a21dcbe33d8bb8fd2577f9906653a6ad5f8225";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>256||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.points.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.points {
n=add_size(n,element.wire_size()?)?;
}
if n>5164{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5164{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/action/SetMode_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsActionSetMode_Goal {
  /// Declared target_robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub target_robot_mode:i8,
  /// Declared stop_program; canonical bounds are part of CODEC_IDENTITY.
  pub stop_program:bool,
  /// Declared play_program; canonical bounds are part of CODEC_IDENTITY.
  pub play_program:bool,
}
impl Default for Ros2Ur_dashboard_msgsActionSetMode_Goal {fn default()->Self {Self {
target_robot_mode:Default::default(),
stop_program:Default::default(),
play_program:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsActionSetMode_Goal {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8a7484a60bad21933df3f847d3fd554e41098ed596dee270335ce9f092dc313d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.target_robot_mode.to_le_bytes());
put(out,at,&[u8::from(self.stop_program)]);
put(out,at,&[u8::from(self.play_program)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.target_robot_mode=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.stop_program=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.play_program=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsActionSetMode_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/action/SetMode_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsActionSetMode_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Ur_dashboard_msgsActionSetMode_Result {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsActionSetMode_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ef2a557ff27171319d46170ab16a80f5cba5fbd59e44c21cc1b704d01764b913";
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
impl Canonical for Ros2Ur_dashboard_msgsActionSetMode_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/action/SetMode_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsActionSetMode_Feedback {
  /// Declared current_robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub current_robot_mode:i8,
  /// Declared current_safety_mode; canonical bounds are part of CODEC_IDENTITY.
  pub current_safety_mode:i8,
}
impl Default for Ros2Ur_dashboard_msgsActionSetMode_Feedback {fn default()->Self {Self {
current_robot_mode:Default::default(),
current_safety_mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsActionSetMode_Feedback {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_afd74cd0a79174b53499a40b23b040ba96ec3d3e332129999525dcf6ca287ac6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current_robot_mode.to_le_bytes());
put(out,at,&self.current_safety_mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.current_robot_mode=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.current_safety_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsActionSetMode_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/OperationalMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgOperationalMode {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:String,
}
impl Default for Ros2Ur_dashboard_msgsMsgOperationalMode {fn default()->Self {Self {
mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgOperationalMode {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7a0be8522cdc20e01d382218499030176a0f6565376fadc5bbdd871a01b9316e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.mode.len()>256||self.mode.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.mode.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.mode.len() as u32).to_le_bytes());put(out,at,self.mode.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgOperationalMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/ProgramInformation.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgProgramInformation {
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:String,
  /// Declared description; canonical bounds are part of CODEC_IDENTITY.
  pub description:String,
  /// Declared created_date; canonical bounds are part of CODEC_IDENTITY.
  pub created_date:u64,
  /// Declared last_saved_date; canonical bounds are part of CODEC_IDENTITY.
  pub last_saved_date:u64,
  /// Declared last_modified_date; canonical bounds are part of CODEC_IDENTITY.
  pub last_modified_date:u64,
  /// Declared program_state; canonical bounds are part of CODEC_IDENTITY.
  pub program_state:String,
}
impl Default for Ros2Ur_dashboard_msgsMsgProgramInformation {fn default()->Self {Self {
name:Default::default(),
description:Default::default(),
created_date:Default::default(),
last_saved_date:Default::default(),
last_modified_date:Default::default(),
program_state:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgProgramInformation {
  pub const MAX_WIRE_BYTES:usize=676;
  pub const CARTESIAN_WIRE_BYTES:u64=676;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9299ce545acd0b6ff064998334820fbe503b71dc47a475ef93545d2cdb41f965";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.name.len()>128||self.name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.name.len())?;
if self.description.len()>256||self.description.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.description.len())?;
n=add_size(n,8)?;
n=add_size(n,8)?;
n=add_size(n,8)?;
if self.program_state.len()>256||self.program_state.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_state.len())?;
if n>676{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.name.len() as u32).to_le_bytes());put(out,at,self.name.as_bytes());
put(out,at,&(self.description.len() as u32).to_le_bytes());put(out,at,self.description.as_bytes());
put(out,at,&self.created_date.to_le_bytes());
put(out,at,&self.last_saved_date.to_le_bytes());
put(out,at,&self.last_modified_date.to_le_bytes());
put(out,at,&(self.program_state.len() as u32).to_le_bytes());put(out,at,self.program_state.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.name=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.description=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.created_date=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.last_saved_date=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.last_modified_date=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_state=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgProgramInformation {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>676{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/ProgramState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgProgramState {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
}
impl Default for Ros2Ur_dashboard_msgsMsgProgramState {fn default()->Self {Self {
state:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgProgramState {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1462a3c2323792afbbfc6d48a600a6ab66a50a6f6f583e57a62da2458ea81f26";
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
impl Canonical for Ros2Ur_dashboard_msgsMsgProgramState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/RobotMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgRobotMode {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:i8,
}
impl Default for Ros2Ur_dashboard_msgsMsgRobotMode {fn default()->Self {Self {
mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgRobotMode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_81a79c84083520d911de108a728ad1877de78a57ec9e27b5cc432590be4616bb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgRobotMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/SafetyMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgSafetyMode {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
}
impl Default for Ros2Ur_dashboard_msgsMsgSafetyMode {fn default()->Self {Self {
mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgSafetyMode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2eb3d5075a2d7d1765a604c1d1d198379010867d83a98d0327488ca51f5e1aac";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgSafetyMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/SafetyStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgSafetyStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:i8,
}
impl Default for Ros2Ur_dashboard_msgsMsgSafetyStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgSafetyStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1b1a7257c40d9c62e0b6291f241825d029fe38f3f3695265a182a270899a79e9";
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
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgSafetyStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/UserRole.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgUserRole {
  /// Declared role; canonical bounds are part of CODEC_IDENTITY.
  pub role:String,
}
impl Default for Ros2Ur_dashboard_msgsMsgUserRole {fn default()->Self {Self {
role:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgUserRole {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7568bc9417685a9bc56cfdd2b41ff81ab99bd8e20fbe5f9e537b5b5d0689440a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.role.len()>256||self.role.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.role.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.role.len() as u32).to_le_bytes());put(out,at,self.role.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.role=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgUserRole {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/msg/VersionInformation.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsMsgVersionInformation {
  /// Declared major; canonical bounds are part of CODEC_IDENTITY.
  pub major:u32,
  /// Declared minor; canonical bounds are part of CODEC_IDENTITY.
  pub minor:u32,
  /// Declared bugfix; canonical bounds are part of CODEC_IDENTITY.
  pub bugfix:u32,
  /// Declared build; canonical bounds are part of CODEC_IDENTITY.
  pub build:u32,
}
impl Default for Ros2Ur_dashboard_msgsMsgVersionInformation {fn default()->Self {Self {
major:Default::default(),
minor:Default::default(),
bugfix:Default::default(),
build:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsMsgVersionInformation {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_450c41c4361f1bca51f864dd9ef596851b17ebc79d4f784e7abb8b721752443d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.major.to_le_bytes());
put(out,at,&self.minor.to_le_bytes());
put(out,at,&self.bugfix.to_le_bytes());
put(out,at,&self.build.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.major=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.minor=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.bugfix=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.build=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsMsgVersionInformation {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/AddToLog_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvAddToLog_Request {
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvAddToLog_Request {fn default()->Self {Self {
message:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvAddToLog_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ecfe5bb0d0db92bc5dfdec9bf82fbe730e3b3b4d65680812d0540b9055590142";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvAddToLog_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/AddToLog_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvAddToLog_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvAddToLog_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvAddToLog_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_57b0c150a353d3fca6e96c4dc86e72f71b452663ede1eeb69e51069992d07e4c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvAddToLog_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/DownloadProgram_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvDownloadProgram_Request {
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared target_path; canonical bounds are part of CODEC_IDENTITY.
  pub target_path:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvDownloadProgram_Request {fn default()->Self {Self {
program_name:Default::default(),
target_path:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvDownloadProgram_Request {
  pub const MAX_WIRE_BYTES:usize=520;
  pub const CARTESIAN_WIRE_BYTES:u64=520;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5c84420393aae13ea448d171a7b2956a55a39f3321f504b9f3bd5b4a38960c2f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.program_name.len()>256||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
if self.target_path.len()>256||self.target_path.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.target_path.len())?;
if n>520{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&(self.target_path.len() as u32).to_le_bytes());put(out,at,self.target_path.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.target_path=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvDownloadProgram_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>520{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/DownloadProgram_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvDownloadProgram_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvDownloadProgram_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvDownloadProgram_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_46b0a73805282bad5f819f712275b7dd2ca4a6bb8c73c445ea8ae0a3d0f9e8e0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvDownloadProgram_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request {
  /// Declared target_path; canonical bounds are part of CODEC_IDENTITY.
  pub target_path:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request {fn default()->Self {Self {
target_path:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d919563629f81d5a8c10f1d2921ea16f3c912a49b8d3bcf65d17b8daa0a38140";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.target_path.len()>256||self.target_path.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.target_path.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.target_path.len() as u32).to_le_bytes());put(out,at,self.target_path.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.target_path=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared support_files_present; canonical bounds are part of CODEC_IDENTITY.
  pub support_files_present:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
support_files_present:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_76e190e6e9f1ddfff3082ea2adb0463d21ba9fb6795153fe6bbacee8a3334667";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
put(out,at,&[u8::from(self.support_files_present)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.support_files_present=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {
  /// Declared report_type; canonical bounds are part of CODEC_IDENTITY.
  pub report_type:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {fn default()->Self {Self {
report_type:String::from("SYSTEM"),
}}}
impl Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7246d97690c0c827e9cfab2f8297cf27a10e60e41ebb3ba345f361a109057099";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.report_type.len()>256||self.report_type.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.report_type.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.report_type.len() as u32).to_le_bytes());put(out,at,self.report_type.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.report_type=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared report_id; canonical bounds are part of CODEC_IDENTITY.
  pub report_id:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response {fn default()->Self {Self {
success:Default::default(),
answer:Default::default(),
report_id:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_adebf67876b5ae3377cec0aa04138c2ab73ef7c0d7942c77c6c06b248a836445";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.report_id.len()>256||self.report_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.report_id.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.report_id.len() as u32).to_le_bytes());put(out,at,self.report_id.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.report_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request {
  /// Declared dir_path; canonical bounds are part of CODEC_IDENTITY.
  pub dir_path:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request {fn default()->Self {Self {
dir_path:String::from("."),
}}}
impl Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_10b0371b77db32d75a77dcb0605b325caa1915ccfb7d0917896fd75234e6ada7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.dir_path.len()>256||self.dir_path.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.dir_path.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.dir_path.len() as u32).to_le_bytes());put(out,at,self.dir_path.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.dir_path=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared generated_file_name; canonical bounds are part of CODEC_IDENTITY.
  pub generated_file_name:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response {fn default()->Self {Self {
success:Default::default(),
answer:Default::default(),
generated_file_name:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f370623fe690f218073f8538aa9fcf41c94d4abd3e0507e09607ef11e10edf38";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.generated_file_name.len()>256||self.generated_file_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.generated_file_name.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.generated_file_name.len() as u32).to_le_bytes());put(out,at,self.generated_file_name.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.generated_file_name=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_030e5746e6a4da8f266e37d11cc3826715099ff1aa1c845a97042aec987707e3";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response {fn default()->Self {Self {
answer:Default::default(),
program_name:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3e8c7f3c8711faea01de37b9a011f34a9c6dd660cc222e46177b2c1898ec2f7e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.program_name.len()>256||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f348b5d9e04a8ba01f5277ad259c34a6816ae0693f47c49e348a5497a90a325";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared operational_mode; canonical bounds are part of CODEC_IDENTITY.
  pub operational_mode:Ros2Ur_dashboard_msgsMsgOperationalMode,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response {fn default()->Self {Self {
success:Default::default(),
answer:Default::default(),
operational_mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6c2da335fd551162020eb394b91e99aaeca17e34e7c2e98de935c81912efb5f6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,self.operational_mode.wire_size()?)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
self.operational_mode.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let decoded=Ros2Ur_dashboard_msgsMsgOperationalMode::read_fields(bytes,at)?;
value.operational_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_88193ae202f0cb02746bc5556e8e8ee277fb4fae72ee4b96a6539c980dc86f13";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:Ros2Ur_dashboard_msgsMsgVersionInformation,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
version:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response {
  pub const MAX_WIRE_BYTES:usize=277;
  pub const CARTESIAN_WIRE_BYTES:u64=277;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c8d53d0a920806e86ceb6d2ac5a692a92e62b057432554bed9dcf57bed0ff9bd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
n=add_size(n,self.version.wire_size()?)?;
if n>277{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
self.version.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let decoded=Ros2Ur_dashboard_msgsMsgVersionInformation::read_fields(bytes,at)?;
value.version=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>277{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<21{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetProgramState_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetProgramState_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetProgramState_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetProgramState_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fe42d1d1aa3d7bdbaa410bce045120d6dd841213ef10c5dd158c2b76fa157f65";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetProgramState_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetProgramState_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetProgramState_Response {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:Ros2Ur_dashboard_msgsMsgProgramState,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetProgramState_Response {fn default()->Self {Self {
state:Default::default(),
program_name:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetProgramState_Response {
  pub const MAX_WIRE_BYTES:usize=781;
  pub const CARTESIAN_WIRE_BYTES:u64=781;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b20731b8552f323837c5939f390060a6c7c92e9447df5e620891708cfce51ff4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.state.wire_size()?)?;
if self.program_name.len()>256||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>781{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.state.write_fields(out,at);
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgProgramState::read_fields(bytes,at)?;
value.state=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetProgramState_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>781{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetPrograms_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetPrograms_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetPrograms_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetPrograms_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8d7b4f3fb9a15854b59c85c5205bf20d3e0f6a1a972ccbdf77ee8377c9e40d6d";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetPrograms_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetPrograms_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetPrograms_Response {
  /// Declared programs; canonical bounds are part of CODEC_IDENTITY.
  pub programs:Vec<Ros2Ur_dashboard_msgsMsgProgramInformation>,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetPrograms_Response {fn default()->Self {Self {
programs:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetPrograms_Response {
  pub const MAX_WIRE_BYTES:usize=11081;
  pub const CARTESIAN_WIRE_BYTES:u64=11081;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_111b67c562ba0c77c2924b97058e4126055ad6deb7f99049b8caf8990b3f979c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.programs.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.programs {
n=add_size(n,element.wire_size()?)?;
}
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>11081{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.programs.len() as u32).to_le_bytes());
for element in &self.programs {
element.write_fields(out,at);
}
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.programs=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_dashboard_msgsMsgProgramInformation::read_fields(bytes,at)?;
value.programs.push(decoded);
}}
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetPrograms_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>11081{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetRobotMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetRobotMode_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetRobotMode_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetRobotMode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d1d9cd94a443b20ec0d41a8db1a6d87291657597abc014e7da0f46a58c6c5614";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetRobotMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetRobotMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetRobotMode_Response {
  /// Declared robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub robot_mode:Ros2Ur_dashboard_msgsMsgRobotMode,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetRobotMode_Response {fn default()->Self {Self {
robot_mode:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetRobotMode_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_67db750ad95b3b207d61b18513a117c868040c22c93638f07fa74645d87bcfba";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.robot_mode.wire_size()?)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.robot_mode.write_fields(out,at);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgRobotMode::read_fields(bytes,at)?;
value.robot_mode=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetRobotMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetRobotModel_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetRobotModel_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetRobotModel_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetRobotModel_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9db08f07993bd5786aea5b6bd567f9e9f49329883326ea16e6c49eef1586dc43";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetRobotModel_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetRobotModel_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetRobotModel_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared robot_model; canonical bounds are part of CODEC_IDENTITY.
  pub robot_model:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetRobotModel_Response {fn default()->Self {Self {
success:Default::default(),
answer:Default::default(),
robot_model:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetRobotModel_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_faf0124c542ef7420915527c27e99a7d26c5d6274f55d869bc9416e6b7d9b3c9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.robot_model.len()>256||self.robot_model.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.robot_model.len())?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.robot_model.len() as u32).to_le_bytes());put(out,at,self.robot_model.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.robot_model=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetRobotModel_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5866cc4b0e10508eeb45ceecf9f9b6036ba55d04c9613622e189ccde2781dcb0";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response {
  /// Declared safety_mode; canonical bounds are part of CODEC_IDENTITY.
  pub safety_mode:Ros2Ur_dashboard_msgsMsgSafetyMode,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response {fn default()->Self {Self {
safety_mode:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f65d7ff83dc97f891032d8ab16bf71a9c2326c6ae6a033fe1d6e06f011c5a40b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.safety_mode.wire_size()?)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.safety_mode.write_fields(out,at);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgSafetyMode::read_fields(bytes,at)?;
value.safety_mode=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0e2958bcd7b121682361c94b2926bcc4552d7ef07dc2af4609d04b1bb7054602";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared safety_status; canonical bounds are part of CODEC_IDENTITY.
  pub safety_status:Ros2Ur_dashboard_msgsMsgSafetyStatus,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
safety_status:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bb96aaa96f5c78b97ad80a14eb96865dc10b5a1d12524fcbcc75fc6d481c53e4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
n=add_size(n,self.safety_status.wire_size()?)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
self.safety_status.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let decoded=Ros2Ur_dashboard_msgsMsgSafetyStatus::read_fields(bytes,at)?;
value.safety_status=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bbb1ca066c77ed459352de69d566f3ea87b94fe593c31c911193416f92a73c9b";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response {
  /// Declared serial_number; canonical bounds are part of CODEC_IDENTITY.
  pub serial_number:u64,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response {fn default()->Self {Self {
serial_number:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response {
  pub const MAX_WIRE_BYTES:usize=269;
  pub const CARTESIAN_WIRE_BYTES:u64=269;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f389641d089fa6fd421348f047732de2122cd714bdb2bd4d1e611592aca349e7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>269{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.serial_number.to_le_bytes());
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.serial_number=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>269{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetUserRole_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetUserRole_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvGetUserRole_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvGetUserRole_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a331be09dd2093e4584e6dc80f83caaadd3a8e43cca6a1c8fcfa1e2ea632c9f0";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvGetUserRole_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/GetUserRole_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvGetUserRole_Response {
  /// Declared user_role; canonical bounds are part of CODEC_IDENTITY.
  pub user_role:Ros2Ur_dashboard_msgsMsgUserRole,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvGetUserRole_Response {fn default()->Self {Self {
user_role:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvGetUserRole_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ed4f53e59fbe4cd33cfbf5d190b60fbe8371dda7da9d7af6e270a882eb142df8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.user_role.wire_size()?)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.user_role.write_fields(out,at);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgUserRole::read_fields(bytes,at)?;
value.user_role=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvGetUserRole_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b6b86fc4337c36589e8781b46ddbe6992f750b565ff413ea0b069982d87a463";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared remote_control; canonical bounds are part of CODEC_IDENTITY.
  pub remote_control:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response {fn default()->Self {Self {
answer:Default::default(),
remote_control:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f2705c1be319c7c667f4f2224851b06c24b15b4211c2828fab89f09455e4c816";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.remote_control)]);
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.remote_control=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f28274c0dc01d8aa2f981f969c05a614e6b3922601d578f99c6321a4b20fc80f";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_running; canonical bounds are part of CODEC_IDENTITY.
  pub program_running:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response {fn default()->Self {Self {
answer:Default::default(),
program_running:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c57cc7ecd7e81f7359824b43884a58f65748a4c77cacbcc45be17aa9147cf499";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.program_running)]);
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.program_running=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request {
}
impl Default for Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c53d57cffc0480d66257150a8450ba90563fda5153742784831d38abcbc0b099";
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
impl Canonical for Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared program_saved; canonical bounds are part of CODEC_IDENTITY.
  pub program_saved:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response {fn default()->Self {Self {
answer:Default::default(),
program_name:Default::default(),
program_saved:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response {
  pub const MAX_WIRE_BYTES:usize=522;
  pub const CARTESIAN_WIRE_BYTES:u64=522;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aa171247f31fd00a1b8820ee21cf20d159bd10d0d2887dc96b3a4dd4d372d115";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.program_name.len()>256||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>522{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&[u8::from(self.program_saved)]);
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.program_saved=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>522{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/Load_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvLoad_Request {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvLoad_Request {fn default()->Self {Self {
filename:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvLoad_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1d0cd6cdeaaafce999a7e9924721246b9f4954f22da40fd0a27fb7b657324876";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.filename.len()>256||self.filename.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.filename.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.filename.len() as u32).to_le_bytes());put(out,at,self.filename.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.filename=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvLoad_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/Load_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvLoad_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvLoad_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvLoad_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4e694de8079133a8e789add3640dfe19805cad92991a650167c2aae7067da415";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvLoad_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/Popup_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvPopup_Request {
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
  /// Declared title; canonical bounds are part of CODEC_IDENTITY.
  pub title:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvPopup_Request {fn default()->Self {Self {
message:Default::default(),
title:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvPopup_Request {
  pub const MAX_WIRE_BYTES:usize=520;
  pub const CARTESIAN_WIRE_BYTES:u64=520;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_369e6eab4f689a846c52fcc5cb539da63fab3939fab254dd1e04c7b274d5f770";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.message.len()>256||self.message.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.message.len())?;
if self.title.len()>256||self.title.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.title.len())?;
if n>520{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.message.len() as u32).to_le_bytes());put(out,at,self.message.as_bytes());
put(out,at,&(self.title.len() as u32).to_le_bytes());put(out,at,self.title.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.message=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.title=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvPopup_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>520{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/Popup_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvPopup_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvPopup_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvPopup_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0b54f7c477c1ddf8ccb18d12d50eaf308225d4927ee377377ed354426e58f984";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvPopup_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/RawRequest_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvRawRequest_Request {
  /// Declared query; canonical bounds are part of CODEC_IDENTITY.
  pub query:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvRawRequest_Request {fn default()->Self {Self {
query:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvRawRequest_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2bf66d87b20ca2de9ed94c04c79042a50239728320b43b27c9942405795b4555";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.query.len()>256||self.query.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.query.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.query.len() as u32).to_le_bytes());put(out,at,self.query.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.query=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvRawRequest_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/RawRequest_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvRawRequest_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvRawRequest_Response {fn default()->Self {Self {
answer:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvRawRequest_Response {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7c37f54149bab54f6e7bbfed41992eb54416b31346bc889a9632d5497111267f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvRawRequest_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request {
  /// Declared operational_mode; canonical bounds are part of CODEC_IDENTITY.
  pub operational_mode:Ros2Ur_dashboard_msgsMsgOperationalMode,
}
impl Default for Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request {fn default()->Self {Self {
operational_mode:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8cc61f7c64211c46e01e52eee2c05f6e709f4f0900cf24fdfd4079b0f7581a00";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.operational_mode.wire_size()?)?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.operational_mode.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgOperationalMode::read_fields(bytes,at)?;
value.operational_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f5009de077224371e9d2f6c0aecbabd24c80148f2c7e1b1383a64f6f214b202a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/SetUserRole_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvSetUserRole_Request {
  /// Declared user_role; canonical bounds are part of CODEC_IDENTITY.
  pub user_role:Ros2Ur_dashboard_msgsMsgUserRole,
}
impl Default for Ros2Ur_dashboard_msgsSrvSetUserRole_Request {fn default()->Self {Self {
user_role:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvSetUserRole_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7bd499947803efca81401a2de496d24e7e54ba97f4f86ec2b3089146a0655f5e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.user_role.wire_size()?)?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.user_role.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_dashboard_msgsMsgUserRole::read_fields(bytes,at)?;
value.user_role=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvSetUserRole_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/SetUserRole_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvSetUserRole_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvSetUserRole_Response {fn default()->Self {Self {
success:Default::default(),
answer:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvSetUserRole_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4cab2bba5030f4e4b9e90252d64ec2e02d6eb87070b75cf43190fecd21da59fd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvSetUserRole_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/UploadProgram_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvUploadProgram_Request {
  /// Declared file_path; canonical bounds are part of CODEC_IDENTITY.
  pub file_path:String,
}
impl Default for Ros2Ur_dashboard_msgsSrvUploadProgram_Request {fn default()->Self {Self {
file_path:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvUploadProgram_Request {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f7501d0115f8d36b1942ce610b1bbc197d0276de508ac8175d539f40494f437b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.file_path.len()>256||self.file_path.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.file_path.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.file_path.len() as u32).to_le_bytes());put(out,at,self.file_path.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.file_path=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvUploadProgram_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_dashboard_msgs/srv/UploadProgram_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_dashboard_msgsSrvUploadProgram_Response {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_dashboard_msgsSrvUploadProgram_Response {fn default()->Self {Self {
answer:Default::default(),
program_name:Default::default(),
success:Default::default(),
}}}
impl Ros2Ur_dashboard_msgsSrvUploadProgram_Response {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ae9a28c1cd4729f54968d18a16e2f8f417bdc5e9bc15809de7481f1f20e6b398";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.answer.len()>256||self.answer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.answer.len())?;
if self.program_name.len()>256||self.program_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program_name.len())?;
n=add_size(n,1)?;
if n>521{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.answer.len() as u32).to_le_bytes());put(out,at,self.answer.as_bytes());
put(out,at,&(self.program_name.len() as u32).to_le_bytes());put(out,at,self.program_name.as_bytes());
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_dashboard_msgsSrvUploadProgram_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {
  /// Declared trajectory; canonical bounds are part of CODEC_IDENTITY.
  pub trajectory:Ros2Trajectory_msgsMsgJointTrajectory,
  /// Declared path_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub path_tolerance:Vec<Ros2Control_msgsMsgJointTolerance>,
  /// Declared goal_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_tolerance:Vec<Ros2Control_msgsMsgJointTolerance>,
  /// Declared goal_time_tolerance; canonical bounds are part of CODEC_IDENTITY.
  pub goal_time_tolerance:Ros2Builtin_interfacesMsgDuration,
  /// Declared until_type; canonical bounds are part of CODEC_IDENTITY.
  pub until_type:u8,
}
impl Default for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {fn default()->Self {Self {
trajectory:Default::default(),
path_tolerance:Default::default(),
goal_tolerance:Default::default(),
goal_time_tolerance:Default::default(),
until_type:Default::default(),
}}}
impl Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {
  pub const MAX_WIRE_BYTES:usize=7053;
  pub const CARTESIAN_WIRE_BYTES:u64=7053;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_78fda8f2d20c502e24b1ad4fe55f34bba41de20ff88e71b56d7a0631d7a3905a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.trajectory.wire_size()?)?;
if self.path_tolerance.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.path_tolerance {
n=add_size(n,element.wire_size()?)?;
}
if self.goal_tolerance.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.goal_tolerance {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.goal_time_tolerance.wire_size()?)?;
n=add_size(n,1)?;
if n>7053{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.trajectory.write_fields(out,at);
put(out,at,&(self.path_tolerance.len() as u32).to_le_bytes());
for element in &self.path_tolerance {
element.write_fields(out,at);
}
put(out,at,&(self.goal_tolerance.len() as u32).to_le_bytes());
for element in &self.goal_tolerance {
element.write_fields(out,at);
}
self.goal_time_tolerance.write_fields(out,at);
put(out,at,&self.until_type.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Trajectory_msgsMsgJointTrajectory::read_fields(bytes,at)?;
value.trajectory=decoded;
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.path_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.path_tolerance.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.goal_tolerance=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Control_msgsMsgJointTolerance::read_fields(bytes,at)?;
value.goal_tolerance.push(decoded);
}}
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.goal_time_tolerance=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.until_type=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>7053{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<37{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:i32,
  /// Declared until_condition_result; canonical bounds are part of CODEC_IDENTITY.
  pub until_condition_result:i32,
  /// Declared error_string; canonical bounds are part of CODEC_IDENTITY.
  pub error_string:String,
}
impl Default for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {fn default()->Self {Self {
error_code:Default::default(),
until_condition_result:Default::default(),
error_string:Default::default(),
}}}
impl Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {
  pub const MAX_WIRE_BYTES:usize=268;
  pub const CARTESIAN_WIRE_BYTES:u64=268;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_de92b9ccfb9c93f3afae9da5e8e31df9a6d6a606479fe27b9bcf14ff329bc869";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.error_string.len()>256||self.error_string.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.error_string.len())?;
if n>268{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.error_code.to_le_bytes());
put(out,at,&self.until_condition_result.to_le_bytes());
put(out,at,&(self.error_string.len() as u32).to_le_bytes());put(out,at,self.error_string.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_code=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.until_condition_result=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.error_string=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>268{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback {
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
}
impl Default for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback {fn default()->Self {Self {
header:Default::default(),
joint_names:Default::default(),
desired:Default::default(),
actual:Default::default(),
error:Default::default(),
}}}
impl Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback {
  pub const MAX_WIRE_BYTES:usize=2352;
  pub const CARTESIAN_WIRE_BYTES:u64=2352;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5a719f991f5c73784f9dc9e962ecfa0541e00daef7f747c312d4b633d63040ac";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.joint_names.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_names {
if element.len()>256||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
n=add_size(n,self.desired.wire_size()?)?;
n=add_size(n,self.actual.wire_size()?)?;
n=add_size(n,self.error.wire_size()?)?;
if n>2352{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.joint_names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_names.push(decoded);
}}
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.desired=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.actual=decoded;
let decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint::read_fields(bytes,at)?;
value.error=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2352{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<88{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/SendScript_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionSendScript_Goal {
  /// Declared program; canonical bounds are part of CODEC_IDENTITY.
  pub program:String,
  /// Declared script_name; canonical bounds are part of CODEC_IDENTITY.
  pub script_name:String,
  /// Declared start_timeout; canonical bounds are part of CODEC_IDENTITY.
  pub start_timeout:Ros2Builtin_interfacesMsgDuration,
  /// Declared fail_on_warnings; canonical bounds are part of CODEC_IDENTITY.
  pub fail_on_warnings:bool,
}
impl Default for Ros2Ur_msgsActionSendScript_Goal {fn default()->Self {Self {
program:Default::default(),
script_name:String::from(""),
start_timeout:Default::default(),
fail_on_warnings:true,
}}}
impl Ros2Ur_msgsActionSendScript_Goal {
  pub const MAX_WIRE_BYTES:usize=8465;
  pub const CARTESIAN_WIRE_BYTES:u64=8465;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c277cd00663786f95201e61baa658b92def3becc1f37e54aa6093bb11135819c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.program.len()>8192||self.program.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.program.len())?;
if self.script_name.len()>256||self.script_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.script_name.len())?;
n=add_size(n,self.start_timeout.wire_size()?)?;
n=add_size(n,1)?;
if n>8465{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.program.len() as u32).to_le_bytes());put(out,at,self.program.as_bytes());
put(out,at,&(self.script_name.len() as u32).to_le_bytes());put(out,at,self.script_name.as_bytes());
self.start_timeout.write_fields(out,at);
put(out,at,&[u8::from(self.fail_on_warnings)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>8192{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.script_name=decoded;
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.start_timeout=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.fail_on_warnings=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsActionSendScript_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8465{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/SendScript_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionSendScript_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Ur_msgsActionSendScript_Result {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Ur_msgsActionSendScript_Result {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d4c4d9721c3a576f590ba4967b2444a82969d6ccba94ddbf8c4a974535ee17d2";
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
impl Canonical for Ros2Ur_msgsActionSendScript_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/SendScript_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionSendScript_Feedback {
}
impl Default for Ros2Ur_msgsActionSendScript_Feedback {fn default()->Self {Self {
}}}
impl Ros2Ur_msgsActionSendScript_Feedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_094876d6467bd2b34f74cc6b574393117753add5254285e4e95d85b07328ef2c";
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
impl Canonical for Ros2Ur_msgsActionSendScript_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/ToolContact_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionToolContact_Goal {
}
impl Default for Ros2Ur_msgsActionToolContact_Goal {fn default()->Self {Self {
}}}
impl Ros2Ur_msgsActionToolContact_Goal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_150298ad16b1bc37d47bd3324cd4ec301ad8402c07779e8a435ce61daa7578dd";
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
impl Canonical for Ros2Ur_msgsActionToolContact_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/ToolContact_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionToolContact_Result {
}
impl Default for Ros2Ur_msgsActionToolContact_Result {fn default()->Self {Self {
}}}
impl Ros2Ur_msgsActionToolContact_Result {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b79f6080ac18b67c25131458a2af0a3c235d624c00fc5c13da534cc546c94578";
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
impl Canonical for Ros2Ur_msgsActionToolContact_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/action/ToolContact_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsActionToolContact_Feedback {
}
impl Default for Ros2Ur_msgsActionToolContact_Feedback {fn default()->Self {Self {
}}}
impl Ros2Ur_msgsActionToolContact_Feedback {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c6a11fd615f72a70a98514703de4c007a2bb161fbd8a953c5e8b899f471f4c5b";
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
impl Canonical for Ros2Ur_msgsActionToolContact_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/Analog.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgAnalog {
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:u8,
  /// Declared domain; canonical bounds are part of CODEC_IDENTITY.
  pub domain:u8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:f32,
}
impl Default for Ros2Ur_msgsMsgAnalog {fn default()->Self {Self {
pin:Default::default(),
domain:Default::default(),
state:Default::default(),
}}}
impl Ros2Ur_msgsMsgAnalog {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_68873e85836e1a65d7b67a52f5db8b3c3be082f889d2df4342386b282988f2a4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.state.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.pin.to_le_bytes());
put(out,at,&self.domain.to_le_bytes());
put(out,at,&self.state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.domain=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgAnalog {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/Digital.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgDigital {
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:u8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:bool,
}
impl Default for Ros2Ur_msgsMsgDigital {fn default()->Self {Self {
pin:Default::default(),
state:Default::default(),
}}}
impl Ros2Ur_msgsMsgDigital {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5ebd0a17ee528cff1e0a636eb0be9c7e4128d3e77be722c5f1be73df4f4c11e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>2{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.pin.to_le_bytes());
put(out,at,&[u8::from(self.state)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgDigital {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/FrictionModelParameters.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgFrictionModelParameters {
  /// Declared viscous_scale; canonical bounds are part of CODEC_IDENTITY.
  pub viscous_scale:Vec<f64>,
  /// Declared coulomb_scale; canonical bounds are part of CODEC_IDENTITY.
  pub coulomb_scale:Vec<f64>,
}
impl Default for Ros2Ur_msgsMsgFrictionModelParameters {fn default()->Self {Self {
viscous_scale:Default::default(),
coulomb_scale:Default::default(),
}}}
impl Ros2Ur_msgsMsgFrictionModelParameters {
  pub const MAX_WIRE_BYTES:usize=104;
  pub const CARTESIAN_WIRE_BYTES:u64=104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7cc9c7af77340f56b0559b4e2ad32972ff65f2a6cf89ebfbe2c48c645f8e9c89";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.viscous_scale.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.viscous_scale {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.coulomb_scale.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.coulomb_scale {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.viscous_scale.len() as u32).to_le_bytes());
for element in &self.viscous_scale {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.coulomb_scale.len() as u32).to_le_bytes());
for element in &self.coulomb_scale {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.viscous_scale=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.viscous_scale.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.coulomb_scale=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.coulomb_scale.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgFrictionModelParameters {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/IOStates.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgIOStates {
  /// Declared digital_in_states; canonical bounds are part of CODEC_IDENTITY.
  pub digital_in_states:Vec<Ros2Ur_msgsMsgDigital>,
  /// Declared digital_out_states; canonical bounds are part of CODEC_IDENTITY.
  pub digital_out_states:Vec<Ros2Ur_msgsMsgDigital>,
  /// Declared flag_states; canonical bounds are part of CODEC_IDENTITY.
  pub flag_states:Vec<Ros2Ur_msgsMsgDigital>,
  /// Declared analog_in_states; canonical bounds are part of CODEC_IDENTITY.
  pub analog_in_states:Vec<Ros2Ur_msgsMsgAnalog>,
  /// Declared analog_out_states; canonical bounds are part of CODEC_IDENTITY.
  pub analog_out_states:Vec<Ros2Ur_msgsMsgAnalog>,
}
impl Default for Ros2Ur_msgsMsgIOStates {fn default()->Self {Self {
digital_in_states:Default::default(),
digital_out_states:Default::default(),
flag_states:Default::default(),
analog_in_states:Default::default(),
analog_out_states:Default::default(),
}}}
impl Ros2Ur_msgsMsgIOStates {
  pub const MAX_WIRE_BYTES:usize=404;
  pub const CARTESIAN_WIRE_BYTES:u64=404;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c22f9f784c89b6854fa84df416ed3704e3a312b15c8fc41162e7f5f53223fef5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.digital_in_states.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.digital_in_states {
n=add_size(n,element.wire_size()?)?;
}
if self.digital_out_states.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.digital_out_states {
n=add_size(n,element.wire_size()?)?;
}
if self.flag_states.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.flag_states {
n=add_size(n,element.wire_size()?)?;
}
if self.analog_in_states.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.analog_in_states {
n=add_size(n,element.wire_size()?)?;
}
if self.analog_out_states.len()>16{return Err(invalid());}n=add_size(n,4)?;
for element in &self.analog_out_states {
n=add_size(n,element.wire_size()?)?;
}
if n>404{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.digital_in_states.len() as u32).to_le_bytes());
for element in &self.digital_in_states {
element.write_fields(out,at);
}
put(out,at,&(self.digital_out_states.len() as u32).to_le_bytes());
for element in &self.digital_out_states {
element.write_fields(out,at);
}
put(out,at,&(self.flag_states.len() as u32).to_le_bytes());
for element in &self.flag_states {
element.write_fields(out,at);
}
put(out,at,&(self.analog_in_states.len() as u32).to_le_bytes());
for element in &self.analog_in_states {
element.write_fields(out,at);
}
put(out,at,&(self.analog_out_states.len() as u32).to_le_bytes());
for element in &self.analog_out_states {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.digital_in_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_msgsMsgDigital::read_fields(bytes,at)?;
value.digital_in_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.digital_out_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_msgsMsgDigital::read_fields(bytes,at)?;
value.digital_out_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.flag_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_msgsMsgDigital::read_fields(bytes,at)?;
value.flag_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.analog_in_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_msgsMsgAnalog::read_fields(bytes,at)?;
value.analog_in_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.analog_out_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Ur_msgsMsgAnalog::read_fields(bytes,at)?;
value.analog_out_states.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgIOStates {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>404{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/MasterboardDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgMasterboardDataMsg {
  /// Declared digital_input_bits; canonical bounds are part of CODEC_IDENTITY.
  pub digital_input_bits:u32,
  /// Declared digital_output_bits; canonical bounds are part of CODEC_IDENTITY.
  pub digital_output_bits:u32,
  /// Declared analog_input_range0; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input_range0:i8,
  /// Declared analog_input_range1; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input_range1:i8,
  /// Declared analog_input0; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input0:f64,
  /// Declared analog_input1; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input1:f64,
  /// Declared analog_output_domain0; canonical bounds are part of CODEC_IDENTITY.
  pub analog_output_domain0:i8,
  /// Declared analog_output_domain1; canonical bounds are part of CODEC_IDENTITY.
  pub analog_output_domain1:i8,
  /// Declared analog_output0; canonical bounds are part of CODEC_IDENTITY.
  pub analog_output0:f64,
  /// Declared analog_output1; canonical bounds are part of CODEC_IDENTITY.
  pub analog_output1:f64,
  /// Declared masterboard_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub masterboard_temperature:f32,
  /// Declared robot_voltage_48v; canonical bounds are part of CODEC_IDENTITY.
  pub robot_voltage_48v:f32,
  /// Declared robot_current; canonical bounds are part of CODEC_IDENTITY.
  pub robot_current:f32,
  /// Declared master_io_current; canonical bounds are part of CODEC_IDENTITY.
  pub master_io_current:f32,
  /// Declared master_safety_state; canonical bounds are part of CODEC_IDENTITY.
  pub master_safety_state:u8,
  /// Declared master_onoff_state; canonical bounds are part of CODEC_IDENTITY.
  pub master_onoff_state:u8,
}
impl Default for Ros2Ur_msgsMsgMasterboardDataMsg {fn default()->Self {Self {
digital_input_bits:Default::default(),
digital_output_bits:Default::default(),
analog_input_range0:Default::default(),
analog_input_range1:Default::default(),
analog_input0:Default::default(),
analog_input1:Default::default(),
analog_output_domain0:Default::default(),
analog_output_domain1:Default::default(),
analog_output0:Default::default(),
analog_output1:Default::default(),
masterboard_temperature:Default::default(),
robot_voltage_48v:Default::default(),
robot_current:Default::default(),
master_io_current:Default::default(),
master_safety_state:Default::default(),
master_onoff_state:Default::default(),
}}}
impl Ros2Ur_msgsMsgMasterboardDataMsg {
  pub const MAX_WIRE_BYTES:usize=62;
  pub const CARTESIAN_WIRE_BYTES:u64=62;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_eb3b0aab27bb5683859356ca7cf9648e53d311c1fd32cac97f68ce1d4622a9e5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.analog_input0.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.analog_input1.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.analog_output0.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.analog_output1.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.masterboard_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.robot_voltage_48v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.robot_current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.master_io_current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>62{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.digital_input_bits.to_le_bytes());
put(out,at,&self.digital_output_bits.to_le_bytes());
put(out,at,&self.analog_input_range0.to_le_bytes());
put(out,at,&self.analog_input_range1.to_le_bytes());
put(out,at,&self.analog_input0.to_le_bytes());
put(out,at,&self.analog_input1.to_le_bytes());
put(out,at,&self.analog_output_domain0.to_le_bytes());
put(out,at,&self.analog_output_domain1.to_le_bytes());
put(out,at,&self.analog_output0.to_le_bytes());
put(out,at,&self.analog_output1.to_le_bytes());
put(out,at,&self.masterboard_temperature.to_le_bytes());
put(out,at,&self.robot_voltage_48v.to_le_bytes());
put(out,at,&self.robot_current.to_le_bytes());
put(out,at,&self.master_io_current.to_le_bytes());
put(out,at,&self.master_safety_state.to_le_bytes());
put(out,at,&self.master_onoff_state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.digital_input_bits=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.digital_output_bits=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_input_range0=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_input_range1=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_input0=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_input1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_output_domain0=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_output_domain1=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_output0=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_output1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.masterboard_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.robot_voltage_48v=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.robot_current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.master_io_current=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.master_safety_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.master_onoff_state=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgMasterboardDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>62{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<62{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/RobotModeDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgRobotModeDataMsg {
  /// Declared timestamp; canonical bounds are part of CODEC_IDENTITY.
  pub timestamp:u64,
  /// Declared is_robot_connected; canonical bounds are part of CODEC_IDENTITY.
  pub is_robot_connected:bool,
  /// Declared is_real_robot_enabled; canonical bounds are part of CODEC_IDENTITY.
  pub is_real_robot_enabled:bool,
  /// Declared is_power_on_robot; canonical bounds are part of CODEC_IDENTITY.
  pub is_power_on_robot:bool,
  /// Declared is_emergency_stopped; canonical bounds are part of CODEC_IDENTITY.
  pub is_emergency_stopped:bool,
  /// Declared is_protective_stopped; canonical bounds are part of CODEC_IDENTITY.
  pub is_protective_stopped:bool,
  /// Declared is_program_running; canonical bounds are part of CODEC_IDENTITY.
  pub is_program_running:bool,
  /// Declared is_program_paused; canonical bounds are part of CODEC_IDENTITY.
  pub is_program_paused:bool,
}
impl Default for Ros2Ur_msgsMsgRobotModeDataMsg {fn default()->Self {Self {
timestamp:Default::default(),
is_robot_connected:Default::default(),
is_real_robot_enabled:Default::default(),
is_power_on_robot:Default::default(),
is_emergency_stopped:Default::default(),
is_protective_stopped:Default::default(),
is_program_running:Default::default(),
is_program_paused:Default::default(),
}}}
impl Ros2Ur_msgsMsgRobotModeDataMsg {
  pub const MAX_WIRE_BYTES:usize=15;
  pub const CARTESIAN_WIRE_BYTES:u64=15;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0181d85868dc6251289c6f7bc723757fa573c1345b6502497e335655d328bb31";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>15{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.timestamp.to_le_bytes());
put(out,at,&[u8::from(self.is_robot_connected)]);
put(out,at,&[u8::from(self.is_real_robot_enabled)]);
put(out,at,&[u8::from(self.is_power_on_robot)]);
put(out,at,&[u8::from(self.is_emergency_stopped)]);
put(out,at,&[u8::from(self.is_protective_stopped)]);
put(out,at,&[u8::from(self.is_program_running)]);
put(out,at,&[u8::from(self.is_program_paused)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.timestamp=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_robot_connected=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_real_robot_enabled=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_power_on_robot=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_emergency_stopped=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_protective_stopped=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_program_running=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_program_paused=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgRobotModeDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>15{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<15{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/RobotStateRTMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgRobotStateRTMsg {
  /// Declared time; canonical bounds are part of CODEC_IDENTITY.
  pub time:f64,
  /// Declared q_target; canonical bounds are part of CODEC_IDENTITY.
  pub q_target:Vec<f64>,
  /// Declared qd_target; canonical bounds are part of CODEC_IDENTITY.
  pub qd_target:Vec<f64>,
  /// Declared qdd_target; canonical bounds are part of CODEC_IDENTITY.
  pub qdd_target:Vec<f64>,
  /// Declared i_target; canonical bounds are part of CODEC_IDENTITY.
  pub i_target:Vec<f64>,
  /// Declared m_target; canonical bounds are part of CODEC_IDENTITY.
  pub m_target:Vec<f64>,
  /// Declared q_actual; canonical bounds are part of CODEC_IDENTITY.
  pub q_actual:Vec<f64>,
  /// Declared qd_actual; canonical bounds are part of CODEC_IDENTITY.
  pub qd_actual:Vec<f64>,
  /// Declared i_actual; canonical bounds are part of CODEC_IDENTITY.
  pub i_actual:Vec<f64>,
  /// Declared tool_acc_values; canonical bounds are part of CODEC_IDENTITY.
  pub tool_acc_values:Vec<f64>,
  /// Declared tcp_force; canonical bounds are part of CODEC_IDENTITY.
  pub tcp_force:Vec<f64>,
  /// Declared tool_vector; canonical bounds are part of CODEC_IDENTITY.
  pub tool_vector:Vec<f64>,
  /// Declared tcp_speed; canonical bounds are part of CODEC_IDENTITY.
  pub tcp_speed:Vec<f64>,
  /// Declared digital_input_bits; canonical bounds are part of CODEC_IDENTITY.
  pub digital_input_bits:f64,
  /// Declared motor_temperatures; canonical bounds are part of CODEC_IDENTITY.
  pub motor_temperatures:Vec<f64>,
  /// Declared controller_timer; canonical bounds are part of CODEC_IDENTITY.
  pub controller_timer:f64,
  /// Declared test_value; canonical bounds are part of CODEC_IDENTITY.
  pub test_value:f64,
  /// Declared robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub robot_mode:f64,
  /// Declared joint_modes; canonical bounds are part of CODEC_IDENTITY.
  pub joint_modes:Vec<f64>,
}
impl Default for Ros2Ur_msgsMsgRobotStateRTMsg {fn default()->Self {Self {
time:Default::default(),
q_target:Default::default(),
qd_target:Default::default(),
qdd_target:Default::default(),
i_target:Default::default(),
m_target:Default::default(),
q_actual:Default::default(),
qd_actual:Default::default(),
i_actual:Default::default(),
tool_acc_values:Default::default(),
tcp_force:Default::default(),
tool_vector:Default::default(),
tcp_speed:Default::default(),
digital_input_bits:Default::default(),
motor_temperatures:Default::default(),
controller_timer:Default::default(),
test_value:Default::default(),
robot_mode:Default::default(),
joint_modes:Default::default(),
}}}
impl Ros2Ur_msgsMsgRobotStateRTMsg {
  pub const MAX_WIRE_BYTES:usize=768;
  pub const CARTESIAN_WIRE_BYTES:u64=768;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a30600c42e98b89da49d63073a5d30b0989aa0cb21f2bafec229dee10591d9cd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.time.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if self.q_target.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.q_target {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.qd_target.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.qd_target {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.qdd_target.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.qdd_target {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.i_target.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.i_target {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.m_target.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.m_target {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.q_actual.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.q_actual {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.qd_actual.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.qd_actual {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.i_actual.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.i_actual {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.tool_acc_values.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tool_acc_values {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.tcp_force.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tcp_force {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.tool_vector.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tool_vector {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if self.tcp_speed.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tcp_speed {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.digital_input_bits.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if self.motor_temperatures.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_temperatures {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if !self.controller_timer.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.test_value.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.robot_mode.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if self.joint_modes.len()>6{return Err(invalid());}n=add_size(n,4)?;
for element in &self.joint_modes {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>768{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.time.to_le_bytes());
put(out,at,&(self.q_target.len() as u32).to_le_bytes());
for element in &self.q_target {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.qd_target.len() as u32).to_le_bytes());
for element in &self.qd_target {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.qdd_target.len() as u32).to_le_bytes());
for element in &self.qdd_target {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.i_target.len() as u32).to_le_bytes());
for element in &self.i_target {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.m_target.len() as u32).to_le_bytes());
for element in &self.m_target {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.q_actual.len() as u32).to_le_bytes());
for element in &self.q_actual {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.qd_actual.len() as u32).to_le_bytes());
for element in &self.qd_actual {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.i_actual.len() as u32).to_le_bytes());
for element in &self.i_actual {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tool_acc_values.len() as u32).to_le_bytes());
for element in &self.tool_acc_values {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tcp_force.len() as u32).to_le_bytes());
for element in &self.tcp_force {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tool_vector.len() as u32).to_le_bytes());
for element in &self.tool_vector {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tcp_speed.len() as u32).to_le_bytes());
for element in &self.tcp_speed {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.digital_input_bits.to_le_bytes());
put(out,at,&(self.motor_temperatures.len() as u32).to_le_bytes());
for element in &self.motor_temperatures {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.controller_timer.to_le_bytes());
put(out,at,&self.test_value.to_le_bytes());
put(out,at,&self.robot_mode.to_le_bytes());
put(out,at,&(self.joint_modes.len() as u32).to_le_bytes());
for element in &self.joint_modes {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.time=decoded;
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.q_target=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q_target.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.qd_target=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.qd_target.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.qdd_target=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.qdd_target.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.i_target=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.i_target.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.m_target=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.m_target.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.q_actual=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q_actual.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.qd_actual=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.qd_actual.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.i_actual=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.i_actual.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.tool_acc_values=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tool_acc_values.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.tcp_force=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tcp_force.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.tool_vector=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tool_vector.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.tcp_speed=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tcp_speed.push(decoded);
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.digital_input_bits=decoded;
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.motor_temperatures=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.motor_temperatures.push(decoded);
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.controller_timer=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.test_value=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.robot_mode=decoded;
{let count=read_count(bytes,at)?;if count>6{return Err(invalid());}
value.joint_modes=Vec::with_capacity(count);
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_modes.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgRobotStateRTMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>768{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<96{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/msg/ToolDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsMsgToolDataMsg {
  /// Declared analog_input_range2; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input_range2:i8,
  /// Declared analog_input_range3; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input_range3:i8,
  /// Declared analog_input2; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input2:f64,
  /// Declared analog_input3; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input3:f64,
  /// Declared tool_voltage_48v; canonical bounds are part of CODEC_IDENTITY.
  pub tool_voltage_48v:f32,
  /// Declared tool_output_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub tool_output_voltage:u8,
  /// Declared tool_current; canonical bounds are part of CODEC_IDENTITY.
  pub tool_current:f32,
  /// Declared tool_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub tool_temperature:f32,
  /// Declared tool_mode; canonical bounds are part of CODEC_IDENTITY.
  pub tool_mode:u8,
}
impl Default for Ros2Ur_msgsMsgToolDataMsg {fn default()->Self {Self {
analog_input_range2:Default::default(),
analog_input_range3:Default::default(),
analog_input2:Default::default(),
analog_input3:Default::default(),
tool_voltage_48v:Default::default(),
tool_output_voltage:Default::default(),
tool_current:Default::default(),
tool_temperature:Default::default(),
tool_mode:Default::default(),
}}}
impl Ros2Ur_msgsMsgToolDataMsg {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1eec69666515ffbdca162a10967fce03ee4c25f0c27d56c5a812b87c9a229aa0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.analog_input2.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.analog_input3.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.tool_voltage_48v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.tool_current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tool_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.analog_input_range2.to_le_bytes());
put(out,at,&self.analog_input_range3.to_le_bytes());
put(out,at,&self.analog_input2.to_le_bytes());
put(out,at,&self.analog_input3.to_le_bytes());
put(out,at,&self.tool_voltage_48v.to_le_bytes());
put(out,at,&self.tool_output_voltage.to_le_bytes());
put(out,at,&self.tool_current.to_le_bytes());
put(out,at,&self.tool_temperature.to_le_bytes());
put(out,at,&self.tool_mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_input_range2=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.analog_input_range3=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_input2=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_input3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tool_voltage_48v=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.tool_output_voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tool_current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tool_temperature=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.tool_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsMsgToolDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request {
}
impl Default for Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request {fn default()->Self {Self {
}}}
impl Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c2d78972cb5b63138d1e780e8ff0c082dd77d5b76b09642d2c655e9f16780101";
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
impl Canonical for Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response {
  /// Declared major; canonical bounds are part of CODEC_IDENTITY.
  pub major:u32,
  /// Declared minor; canonical bounds are part of CODEC_IDENTITY.
  pub minor:u32,
  /// Declared bugfix; canonical bounds are part of CODEC_IDENTITY.
  pub bugfix:u32,
  /// Declared build; canonical bounds are part of CODEC_IDENTITY.
  pub build:u32,
}
impl Default for Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response {fn default()->Self {Self {
major:Default::default(),
minor:Default::default(),
bugfix:Default::default(),
build:Default::default(),
}}}
impl Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bbe9984c34d69f93ad8dbfe6059c6a2e1bf43dbd27eabd97cf0a9507308d814a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.major.to_le_bytes());
put(out,at,&self.minor.to_le_bytes());
put(out,at,&self.bugfix.to_le_bytes());
put(out,at,&self.build.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.major=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.minor=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.bugfix=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.build=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetAnalogOutput_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetAnalogOutput_Request {
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:Ros2Ur_msgsMsgAnalog,
}
impl Default for Ros2Ur_msgsSrvSetAnalogOutput_Request {fn default()->Self {Self {
data:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetAnalogOutput_Request {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71c3eca5c6c90758f084e275fc495feb5d1c08a467c97d941fb5864c54309bea";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.data.wire_size()?)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.data.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_msgsMsgAnalog::read_fields(bytes,at)?;
value.data=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetAnalogOutput_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetAnalogOutput_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetAnalogOutput_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetAnalogOutput_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetAnalogOutput_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_71a5f3580657cb3ca702dbb63b7af3330c922624c9e9774e47e6533e2fc0a05a";
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
impl Canonical for Ros2Ur_msgsSrvSetAnalogOutput_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetForceMode_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetForceMode_Request {
  /// Declared task_frame; canonical bounds are part of CODEC_IDENTITY.
  pub task_frame:Ros2Geometry_msgsMsgPoseStamped,
  /// Declared selection_vector_x; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_x:bool,
  /// Declared selection_vector_y; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_y:bool,
  /// Declared selection_vector_z; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_z:bool,
  /// Declared selection_vector_rx; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_rx:bool,
  /// Declared selection_vector_ry; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_ry:bool,
  /// Declared selection_vector_rz; canonical bounds are part of CODEC_IDENTITY.
  pub selection_vector_rz:bool,
  /// Declared wrench; canonical bounds are part of CODEC_IDENTITY.
  pub wrench:Ros2Geometry_msgsMsgWrench,
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:u8,
  /// Declared speed_limits; canonical bounds are part of CODEC_IDENTITY.
  pub speed_limits:Ros2Geometry_msgsMsgTwist,
  /// Declared deviation_limits; canonical bounds are part of CODEC_IDENTITY.
  pub deviation_limits:[f32;6],
  /// Declared damping_factor; canonical bounds are part of CODEC_IDENTITY.
  pub damping_factor:f32,
  /// Declared gain_scaling; canonical bounds are part of CODEC_IDENTITY.
  pub gain_scaling:f32,
}
impl Default for Ros2Ur_msgsSrvSetForceMode_Request {fn default()->Self {Self {
task_frame:Default::default(),
selection_vector_x:false,
selection_vector_y:false,
selection_vector_z:false,
selection_vector_rx:false,
selection_vector_ry:false,
selection_vector_rz:false,
wrench:Default::default(),
r#type:2,
speed_limits:Default::default(),
deviation_limits:[f32::from_bits(0x3c23d70a),f32::from_bits(0x3c23d70a),f32::from_bits(0x3c23d70a),f32::from_bits(0x3c23d70a),f32::from_bits(0x3c23d70a),f32::from_bits(0x3c23d70a)],
damping_factor:f32::from_bits(0x3ccccccd),
gain_scaling:f32::from_bits(0x3f000000),
}}}
impl Ros2Ur_msgsSrvSetForceMode_Request {
  pub const MAX_WIRE_BYTES:usize=331;
  pub const CARTESIAN_WIRE_BYTES:u64=331;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e43a6c22510a2078de3cb2dc17ba1b7184080a6aa29a1653b7986723c0cfef6e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.task_frame.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,self.wrench.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,self.speed_limits.wire_size()?)?;
for element in &self.deviation_limits {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.damping_factor.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.gain_scaling.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>331{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.task_frame.write_fields(out,at);
put(out,at,&[u8::from(self.selection_vector_x)]);
put(out,at,&[u8::from(self.selection_vector_y)]);
put(out,at,&[u8::from(self.selection_vector_z)]);
put(out,at,&[u8::from(self.selection_vector_rx)]);
put(out,at,&[u8::from(self.selection_vector_ry)]);
put(out,at,&[u8::from(self.selection_vector_rz)]);
self.wrench.write_fields(out,at);
put(out,at,&self.r#type.to_le_bytes());
self.speed_limits.write_fields(out,at);
for element in &self.deviation_limits {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.damping_factor.to_le_bytes());
put(out,at,&self.gain_scaling.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgPoseStamped::read_fields(bytes,at)?;
value.task_frame=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_x=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_y=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_z=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_rx=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_ry=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.selection_vector_rz=decoded;
let decoded=Ros2Geometry_msgsMsgWrench::read_fields(bytes,at)?;
value.wrench=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.r#type=decoded;
let decoded=Ros2Geometry_msgsMsgTwist::read_fields(bytes,at)?;
value.speed_limits=decoded;
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.deviation_limits[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.damping_factor=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.gain_scaling=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetForceMode_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>331{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<203{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetForceMode_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetForceMode_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetForceMode_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetForceMode_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f5438d426b7b68557ac8f43dc8efc2e2106782ec0cd4fd0dee7cdd69ddc7de8";
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
impl Canonical for Ros2Ur_msgsSrvSetForceMode_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetFrictionModelParameters_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetFrictionModelParameters_Request {
  /// Declared parameters; canonical bounds are part of CODEC_IDENTITY.
  pub parameters:Ros2Ur_msgsMsgFrictionModelParameters,
}
impl Default for Ros2Ur_msgsSrvSetFrictionModelParameters_Request {fn default()->Self {Self {
parameters:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetFrictionModelParameters_Request {
  pub const MAX_WIRE_BYTES:usize=104;
  pub const CARTESIAN_WIRE_BYTES:u64=104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e44ad15f9c45d5a65eb13e6bc6e973bb20eebdafa04a5f2e8bcf4b0b54ce91c2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.parameters.wire_size()?)?;
if n>104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.parameters.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Ur_msgsMsgFrictionModelParameters::read_fields(bytes,at)?;
value.parameters=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetFrictionModelParameters_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetFrictionModelParameters_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetFrictionModelParameters_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetFrictionModelParameters_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetFrictionModelParameters_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1caca29dda3dc32360fd010dfb352b6feb54bba66ee16edd2f4353e3382207b3";
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
impl Canonical for Ros2Ur_msgsSrvSetFrictionModelParameters_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetGravity_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetGravity_Request {
  /// Declared gravity; canonical bounds are part of CODEC_IDENTITY.
  pub gravity:Ros2Geometry_msgsMsgVector3Stamped,
}
impl Default for Ros2Ur_msgsSrvSetGravity_Request {fn default()->Self {Self {
gravity:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetGravity_Request {
  pub const MAX_WIRE_BYTES:usize=164;
  pub const CARTESIAN_WIRE_BYTES:u64=164;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3d7fed6ae8accbddd5b09f93c3e49f2b9f99cae4cf80a951eb31ace30ec5a834";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.gravity.wire_size()?)?;
if n>164{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.gravity.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Geometry_msgsMsgVector3Stamped::read_fields(bytes,at)?;
value.gravity=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetGravity_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>164{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetGravity_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetGravity_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:String,
}
impl Default for Ros2Ur_msgsSrvSetGravity_Response {fn default()->Self {Self {
success:Default::default(),
status:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetGravity_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_84694df7fbb4fd15bfecaf1aae683ef1795b0c4941d1987d951467448da5f793";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.status.len()>256||self.status.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.status.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.success)]);
put(out,at,&(self.status.len() as u32).to_le_bytes());put(out,at,self.status.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetGravity_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetIO_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetIO_Request {
  /// Declared fun; canonical bounds are part of CODEC_IDENTITY.
  pub fun:i8,
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:i8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:f32,
}
impl Default for Ros2Ur_msgsSrvSetIO_Request {fn default()->Self {Self {
fun:Default::default(),
pin:Default::default(),
state:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetIO_Request {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c8ac4dddeb474521b4c701a0229e99af954416b7b8d7f32d2a19629d38b378c7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.state.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.fun.to_le_bytes());
put(out,at,&self.pin.to_le_bytes());
put(out,at,&self.state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.fun=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetIO_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetIO_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetIO_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetIO_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetIO_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bd32373034093ccdab140a5ba8ce8a420ef2ce441430e8306b2484f0ca535090";
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
impl Canonical for Ros2Ur_msgsSrvSetIO_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetPayload_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetPayload_Request {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared center_of_gravity; canonical bounds are part of CODEC_IDENTITY.
  pub center_of_gravity:Ros2Geometry_msgsMsgVector3,
  /// Declared ixx; canonical bounds are part of CODEC_IDENTITY.
  pub ixx:f64,
  /// Declared iyy; canonical bounds are part of CODEC_IDENTITY.
  pub iyy:f64,
  /// Declared izz; canonical bounds are part of CODEC_IDENTITY.
  pub izz:f64,
  /// Declared ixy; canonical bounds are part of CODEC_IDENTITY.
  pub ixy:f64,
  /// Declared ixz; canonical bounds are part of CODEC_IDENTITY.
  pub ixz:f64,
  /// Declared iyz; canonical bounds are part of CODEC_IDENTITY.
  pub iyz:f64,
  /// Declared transition_time; canonical bounds are part of CODEC_IDENTITY.
  pub transition_time:Ros2Builtin_interfacesMsgDuration,
}
impl Default for Ros2Ur_msgsSrvSetPayload_Request {fn default()->Self {Self {
mass:Default::default(),
center_of_gravity:Default::default(),
ixx:Default::default(),
iyy:Default::default(),
izz:Default::default(),
ixy:Default::default(),
ixz:Default::default(),
iyz:Default::default(),
transition_time:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetPayload_Request {
  pub const MAX_WIRE_BYTES:usize=84;
  pub const CARTESIAN_WIRE_BYTES:u64=84;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e76e256e1accf2d742c4cf529e5af1e830db75f3db703a1f368d28077a87e735";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.center_of_gravity.wire_size()?)?;
if !self.ixx.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.iyy.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.izz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.ixy.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.ixz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.iyz.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,self.transition_time.wire_size()?)?;
if n>84{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
self.center_of_gravity.write_fields(out,at);
put(out,at,&self.ixx.to_le_bytes());
put(out,at,&self.iyy.to_le_bytes());
put(out,at,&self.izz.to_le_bytes());
put(out,at,&self.ixy.to_le_bytes());
put(out,at,&self.ixz.to_le_bytes());
put(out,at,&self.iyz.to_le_bytes());
self.transition_time.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=Ros2Geometry_msgsMsgVector3::read_fields(bytes,at)?;
value.center_of_gravity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixx=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iyy=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.izz=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixy=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ixz=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.iyz=decoded;
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.transition_time=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetPayload_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>84{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<84{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetPayload_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetPayload_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetPayload_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetPayload_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_036dbaa7d42b6cba6ad29a082e078401d507292483d260ba532f7d6947c1624c";
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
impl Canonical for Ros2Ur_msgsSrvSetPayload_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetSpeedSliderFraction_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetSpeedSliderFraction_Request {
  /// Declared speed_slider_fraction; canonical bounds are part of CODEC_IDENTITY.
  pub speed_slider_fraction:f64,
}
impl Default for Ros2Ur_msgsSrvSetSpeedSliderFraction_Request {fn default()->Self {Self {
speed_slider_fraction:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetSpeedSliderFraction_Request {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d8f19fb240e4b70cad74c5111cdaebad14b480f21da953c95d474064b091abd8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.speed_slider_fraction.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.speed_slider_fraction.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed_slider_fraction=decoded;
Ok(value)}
}
impl Canonical for Ros2Ur_msgsSrvSetSpeedSliderFraction_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ur_msgs/srv/SetSpeedSliderFraction_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ur_msgsSrvSetSpeedSliderFraction_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros2Ur_msgsSrvSetSpeedSliderFraction_Response {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Ur_msgsSrvSetSpeedSliderFraction_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ab1ad5ab4b1ed302710ec055dff7644cc6e966af919ff311eaea6cddcf7dbb9";
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
impl Canonical for Ros2Ur_msgsSrvSetSpeedSliderFraction_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Ur_dashboard_msgsMsgOperationalMode {pub const MANUAL:&'static str="MANUAL";
pub const AUTOMATIC:&'static str="AUTOMATIC";}

impl Ros2Ur_dashboard_msgsMsgProgramInformation {pub const PROGRAM_STATE_DRAFT:&'static str="DRAFT";
pub const PROGRAM_STATE_FINAL:&'static str="FINAL";}

impl Ros2Ur_dashboard_msgsMsgProgramState {pub const STOPPED:&'static str="STOPPED";
pub const PLAYING:&'static str="PLAYING";
pub const PAUSED:&'static str="PAUSED";}

impl Ros2Ur_dashboard_msgsMsgRobotMode {pub const NO_CONTROLLER:i8=-1;
pub const DISCONNECTED:i8=0;
pub const CONFIRM_SAFETY:i8=1;
pub const BOOTING:i8=2;
pub const POWER_OFF:i8=3;
pub const POWER_ON:i8=4;
pub const IDLE:i8=5;
pub const BACKDRIVE:i8=6;
pub const RUNNING:i8=7;
pub const UPDATING_FIRMWARE:i8=8;}

impl Ros2Ur_dashboard_msgsMsgSafetyMode {pub const NORMAL:u8=1;
pub const REDUCED:u8=2;
pub const PROTECTIVE_STOP:u8=3;
pub const RECOVERY:u8=4;
pub const SAFEGUARD_STOP:u8=5;
pub const SYSTEM_EMERGENCY_STOP:u8=6;
pub const ROBOT_EMERGENCY_STOP:u8=7;
pub const VIOLATION:u8=8;
pub const FAULT:u8=9;
pub const VALIDATE_JOINT_ID:u8=10;
pub const UNDEFINED_SAFETY_MODE:u8=11;
pub const AUTOMATIC_MODE_SAFEGUARD_STOP:u8=12;
pub const SYSTEM_THREE_POSITION_ENABLING_STOP:u8=13;}

impl Ros2Ur_dashboard_msgsMsgSafetyStatus {pub const NORMAL:i8=1;
pub const REDUCED:i8=2;
pub const PROTECTIVE_STOP:i8=3;
pub const RECOVERY:i8=4;
pub const SAFEGUARD_STOP:i8=5;
pub const SYSTEM_EMERGENCY_STOP:i8=6;
pub const ROBOT_EMERGENCY_STOP:i8=7;
pub const VIOLATION:i8=8;
pub const FAULT:i8=9;
pub const VALIDATE_JOINT_ID:i8=10;
pub const UNDEFINED_SAFETY_MODE:i8=11;
pub const AUTOMATIC_MODE_SAFEGUARD_STOP:i8=12;
pub const SYSTEM_THREE_POSITION_ENABLING_STOP:i8=13;
pub const TP_THREE_POSITION_ENABLING_STOP:i8=14;
pub const IMMI_EMERGENCY_STOP:i8=15;
pub const IMMI_SAFEGUARD_STOP:i8=16;
pub const PROFISAFE_WAITING_FOR_PARAMETERS:i8=17;
pub const PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP:i8=18;
pub const PROFISAFE_SAFEGUARD_STOP:i8=19;
pub const PROFISAFE_EMERGENCY_STOP:i8=20;
pub const SAFETY_API_SAFEGUARD_STOP:i8=22;
pub const IO_PLANE_STOP:i8=23;}

impl Ros2Ur_dashboard_msgsMsgUserRole {pub const PROGRAMMER:&'static str="PROGRAMMER";
pub const OPERATOR:&'static str="OPERATOR";
pub const NONE:&'static str="NONE";
pub const LOCKED:&'static str="LOCKED";
pub const RESTRICTED:&'static str="RESTRICTED";}

impl Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {pub const CONTROLLER:&'static str="controller";
pub const SOFTWARE:&'static str="software";
pub const SYSTEM:&'static str="system";}

impl Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {pub const TOOL_CONTACT:u8=0;}

impl Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {pub const SUCCESSFUL:i32=0;
pub const INVALID_GOAL:i32=-1;
pub const INVALID_JOINTS:i32=-2;
pub const OLD_HEADER_TIMESTAMP:i32=-3;
pub const PATH_TOLERANCE_VIOLATED:i32=-4;
pub const GOAL_TOLERANCE_VIOLATED:i32=-5;
pub const TRIGGERED:i32=0;
pub const NOT_TRIGGERED:i32=1;}

impl Ros2Ur_msgsMsgAnalog {pub const CURRENT:u8=0;
pub const VOLTAGE:u8=1;}

impl Ros2Ur_msgsMsgToolDataMsg {pub const ANALOG_INPUT_RANGE_CURRENT:i8=0;
pub const ANALOG_INPUT_RANGE_VOLTAGE:i8=1;
pub const TOOL_BOOTLOADER_MODE:u8=249;
pub const TOOL_RUNNING_MODE:u8=253;
pub const TOOL_IDLE_MODE:u8=255;}

impl Ros2Ur_msgsSrvSetForceMode_Request {pub const TCP_TO_ORIGIN:u8=1;
pub const NO_TRANSFORM:u8=2;
pub const TCP_VELOCITY_TO_X_Y:u8=3;}

impl Ros2Ur_msgsSrvSetIO_Request {pub const PIN_AOUT0:i8=0;
pub const PIN_AOUT1:i8=1;
pub const PIN_DOUT0:i8=0;
pub const PIN_DOUT1:i8=1;
pub const PIN_DOUT2:i8=2;
pub const PIN_DOUT3:i8=3;
pub const PIN_DOUT4:i8=4;
pub const PIN_DOUT5:i8=5;
pub const PIN_DOUT6:i8=6;
pub const PIN_DOUT7:i8=7;
pub const PIN_CONF_OUT0:i8=8;
pub const PIN_CONF_OUT1:i8=9;
pub const PIN_CONF_OUT2:i8=10;
pub const PIN_CONF_OUT3:i8=11;
pub const PIN_CONF_OUT4:i8=12;
pub const PIN_CONF_OUT5:i8=13;
pub const PIN_CONF_OUT6:i8=14;
pub const PIN_CONF_OUT7:i8=15;
pub const PIN_TOOL_DOUT0:i8=16;
pub const PIN_TOOL_DOUT1:i8=17;
pub const FUN_SET_DIGITAL_OUT:i8=1;
pub const FUN_SET_FLAG:i8=2;
pub const FUN_SET_ANALOG_OUT:i8=3;
pub const FUN_SET_TOOL_VOLTAGE:i8=4;
pub const STATE_OFF:i8=0;
pub const STATE_ON:i8=1;
pub const STATE_TOOL_VOLTAGE_0V:i8=0;
pub const STATE_TOOL_VOLTAGE_12V:i8=12;
pub const STATE_TOOL_VOLTAGE_24V:i8=24;}
