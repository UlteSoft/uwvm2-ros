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
/// Complete bounded value for ROS1 geometry_msgs/Point.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsPoint {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
}
impl Default for Ros1Geometry_msgsPoint {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Geometry_msgsPoint {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b";
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
impl Canonical for Ros1Geometry_msgsPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/Quaternion.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsQuaternion {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
  /// Declared w; canonical bounds are part of CODEC_IDENTITY.
  pub w:f64,
}
impl Default for Ros1Geometry_msgsQuaternion {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
w:Default::default(),
}}}
impl Ros1Geometry_msgsQuaternion {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934";
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
impl Canonical for Ros1Geometry_msgsQuaternion {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/Pose.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsPose {
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Ros1Geometry_msgsPoint,
  /// Declared orientation; canonical bounds are part of CODEC_IDENTITY.
  pub orientation:Ros1Geometry_msgsQuaternion,
}
impl Default for Ros1Geometry_msgsPose {fn default()->Self {Self {
position:Default::default(),
orientation:Default::default(),
}}}
impl Ros1Geometry_msgsPose {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471";
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
let decoded=Ros1Geometry_msgsPoint::read_fields(bytes,at)?;
value.position=decoded;
let decoded=Ros1Geometry_msgsQuaternion::read_fields(bytes,at)?;
value.orientation=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsPose {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/PoseWithCovariance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsPoseWithCovariance {
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros1Geometry_msgsPose,
  /// Declared covariance; canonical bounds are part of CODEC_IDENTITY.
  pub covariance:[f64;36],
}
impl Default for Ros1Geometry_msgsPoseWithCovariance {fn default()->Self {Self {
pose:Default::default(),
covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Geometry_msgsPoseWithCovariance {
  pub const MAX_WIRE_BYTES:usize=344;
  pub const CARTESIAN_WIRE_BYTES:u64=344;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_17b25963aafb3adfd0432febe040c9b38a531a7a667949b89e0f3937585ef8cf";
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
let decoded=Ros1Geometry_msgsPose::read_fields(bytes,at)?;
value.pose=decoded;
{let count=36usize;if count>36{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsPoseWithCovariance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>344{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<344{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/Vector3.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsVector3 {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f64,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f64,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f64,
}
impl Default for Ros1Geometry_msgsVector3 {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Geometry_msgsVector3 {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc";
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
impl Canonical for Ros1Geometry_msgsVector3 {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/Transform.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsTransform {
  /// Declared translation; canonical bounds are part of CODEC_IDENTITY.
  pub translation:Ros1Geometry_msgsVector3,
  /// Declared rotation; canonical bounds are part of CODEC_IDENTITY.
  pub rotation:Ros1Geometry_msgsQuaternion,
}
impl Default for Ros1Geometry_msgsTransform {fn default()->Self {Self {
translation:Default::default(),
rotation:Default::default(),
}}}
impl Ros1Geometry_msgsTransform {
  pub const MAX_WIRE_BYTES:usize=56;
  pub const CARTESIAN_WIRE_BYTES:u64=56;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_61aabab03e8f766ea05949c9e5f4989186d72a26f793b8d11752e53f1f300085";
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
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.translation=decoded;
let decoded=Ros1Geometry_msgsQuaternion::read_fields(bytes,at)?;
value.rotation=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsTransform {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>56{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<56{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS1 geometry_msgs/TransformStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsTransformStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared child_frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub child_frame_id:String,
  /// Declared transform; canonical bounds are part of CODEC_IDENTITY.
  pub transform:Ros1Geometry_msgsTransform,
}
impl Default for Ros1Geometry_msgsTransformStamped {fn default()->Self {Self {
header:Default::default(),
child_frame_id:Default::default(),
transform:Default::default(),
}}}
impl Ros1Geometry_msgsTransformStamped {
  pub const MAX_WIRE_BYTES:usize=460;
  pub const CARTESIAN_WIRE_BYTES:u64=460;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9237b5dc5d3beacf4310244ce746a4210b9864855b611d1bf54cf23c28d7b79a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>256||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.transform.wire_size()?)?;
if n>460{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.child_frame_id.len() as u32).to_le_bytes());put(out,at,self.child_frame_id.as_bytes());
self.transform.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.child_frame_id=decoded;
let decoded=Ros1Geometry_msgsTransform::read_fields(bytes,at)?;
value.transform=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsTransformStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>460{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<76{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/Twist.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsTwist {
  /// Declared linear; canonical bounds are part of CODEC_IDENTITY.
  pub linear:Ros1Geometry_msgsVector3,
  /// Declared angular; canonical bounds are part of CODEC_IDENTITY.
  pub angular:Ros1Geometry_msgsVector3,
}
impl Default for Ros1Geometry_msgsTwist {fn default()->Self {Self {
linear:Default::default(),
angular:Default::default(),
}}}
impl Ros1Geometry_msgsTwist {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921";
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
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.linear=decoded;
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.angular=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsTwist {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/TwistStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsTwistStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros1Geometry_msgsTwist,
}
impl Default for Ros1Geometry_msgsTwistStamped {fn default()->Self {Self {
header:Default::default(),
twist:Default::default(),
}}}
impl Ros1Geometry_msgsTwistStamped {
  pub const MAX_WIRE_BYTES:usize=192;
  pub const CARTESIAN_WIRE_BYTES:u64=192;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_27a31bcca6a50fbc6c6259b53d229872336a75628d6afd9de174eee2481946c7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>192{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.twist.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros1Geometry_msgsTwist::read_fields(bytes,at)?;
value.twist=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsTwistStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>192{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<64{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 geometry_msgs/TwistWithCovariance.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsTwistWithCovariance {
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros1Geometry_msgsTwist,
  /// Declared covariance; canonical bounds are part of CODEC_IDENTITY.
  pub covariance:[f64;36],
}
impl Default for Ros1Geometry_msgsTwistWithCovariance {fn default()->Self {Self {
twist:Default::default(),
covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Geometry_msgsTwistWithCovariance {
  pub const MAX_WIRE_BYTES:usize=336;
  pub const CARTESIAN_WIRE_BYTES:u64=336;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ff6790d181503b424d84726c8f4a7852cef14c6dd93e6d32cf3c37023f74bcdf";
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
let decoded=Ros1Geometry_msgsTwist::read_fields(bytes,at)?;
value.twist=decoded;
{let count=36usize;if count>36{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsTwistWithCovariance {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>336{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<336{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 nav_msgs/Odometry.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Nav_msgsOdometry {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared child_frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub child_frame_id:String,
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros1Geometry_msgsPoseWithCovariance,
  /// Declared twist; canonical bounds are part of CODEC_IDENTITY.
  pub twist:Ros1Geometry_msgsTwistWithCovariance,
}
impl Default for Ros1Nav_msgsOdometry {fn default()->Self {Self {
header:Default::default(),
child_frame_id:Default::default(),
pose:Default::default(),
twist:Default::default(),
}}}
impl Ros1Nav_msgsOdometry {
  pub const MAX_WIRE_BYTES:usize=1084;
  pub const CARTESIAN_WIRE_BYTES:u64=1084;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e0ff5eb7aef868891846ac7618249b12c18fcf0969653f24fdeaa7644e636ca0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>256||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.pose.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>1084{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.child_frame_id.len() as u32).to_le_bytes());put(out,at,self.child_frame_id.as_bytes());
self.pose.write_fields(out,at);
self.twist.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.child_frame_id=decoded;
let decoded=Ros1Geometry_msgsPoseWithCovariance::read_fields(bytes,at)?;
value.pose=decoded;
let decoded=Ros1Geometry_msgsTwistWithCovariance::read_fields(bytes,at)?;
value.twist=decoded;
Ok(value)}
}
impl Canonical for Ros1Nav_msgsOdometry {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1084{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<700{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/BatteryState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsBatteryState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
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
impl Default for Ros1Sensor_msgsBatteryState {fn default()->Self {Self {
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
impl Ros1Sensor_msgsBatteryState {
  pub const MAX_WIRE_BYTES:usize=832;
  pub const CARTESIAN_WIRE_BYTES:u64=832;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b176902362348cd3b7eec6ae21e2035d33df6990b4682ddf5123ef6e0339441";
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
if n>832{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
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
impl Canonical for Ros1Sensor_msgsBatteryState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>832{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<64{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/Imu.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsImu {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared orientation; canonical bounds are part of CODEC_IDENTITY.
  pub orientation:Ros1Geometry_msgsQuaternion,
  /// Declared orientation_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub orientation_covariance:[f64;9],
  /// Declared angular_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity:Ros1Geometry_msgsVector3,
  /// Declared angular_velocity_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity_covariance:[f64;9],
  /// Declared linear_acceleration; canonical bounds are part of CODEC_IDENTITY.
  pub linear_acceleration:Ros1Geometry_msgsVector3,
  /// Declared linear_acceleration_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub linear_acceleration_covariance:[f64;9],
}
impl Default for Ros1Sensor_msgsImu {fn default()->Self {Self {
header:Default::default(),
orientation:Default::default(),
orientation_covariance:core::array::from_fn(|_|Default::default()),
angular_velocity:Default::default(),
angular_velocity_covariance:core::array::from_fn(|_|Default::default()),
linear_acceleration:Default::default(),
linear_acceleration_covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Sensor_msgsImu {
  pub const MAX_WIRE_BYTES:usize=440;
  pub const CARTESIAN_WIRE_BYTES:u64=440;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_de9c3f5bb37e1dc20be73cb44f614a9ff8645c4cfbc305dc18425d8d9f130bc3";
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
if n>440{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros1Geometry_msgsQuaternion::read_fields(bytes,at)?;
value.orientation=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.orientation_covariance[i]=decoded;
}}
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.angular_velocity=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angular_velocity_covariance[i]=decoded;
}}
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.linear_acceleration=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.linear_acceleration_covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Sensor_msgsImu {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>440{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<312{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/JointState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsJointState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared name; canonical bounds are part of CODEC_IDENTITY.
  pub name:Vec<String>,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Vec<f64>,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:Vec<f64>,
  /// Declared effort; canonical bounds are part of CODEC_IDENTITY.
  pub effort:Vec<f64>,
}
impl Default for Ros1Sensor_msgsJointState {fn default()->Self {Self {
header:Default::default(),
name:Default::default(),
position:Default::default(),
velocity:Default::default(),
effort:Default::default(),
}}}
impl Ros1Sensor_msgsJointState {
  pub const MAX_WIRE_BYTES:usize=472;
  pub const CARTESIAN_WIRE_BYTES:u64=472;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f27093d33cca1405ec35a9e525a95aa4002421a579cb78c6da3be3719f3cb66e";
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
if n>472{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
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
impl Canonical for Ros1Sensor_msgsJointState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>472{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/LaserScan.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsLaserScan {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
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
impl Default for Ros1Sensor_msgsLaserScan {fn default()->Self {Self {
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
impl Ros1Sensor_msgsLaserScan {
  pub const MAX_WIRE_BYTES:usize=8372;
  pub const CARTESIAN_WIRE_BYTES:u64=8372;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f7aee00335bf7d165004957149192f58abe5a27b5c4884bdf8d1ae9be06b366b";
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
if n>8372{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
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
impl Canonical for Ros1Sensor_msgsLaserScan {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8372{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<52{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 sensor_msgs/MagneticField.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Sensor_msgsMagneticField {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared magnetic_field; canonical bounds are part of CODEC_IDENTITY.
  pub magnetic_field:Ros1Geometry_msgsVector3,
  /// Declared magnetic_field_covariance; canonical bounds are part of CODEC_IDENTITY.
  pub magnetic_field_covariance:[f64;9],
}
impl Default for Ros1Sensor_msgsMagneticField {fn default()->Self {Self {
header:Default::default(),
magnetic_field:Default::default(),
magnetic_field_covariance:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Sensor_msgsMagneticField {
  pub const MAX_WIRE_BYTES:usize=240;
  pub const CARTESIAN_WIRE_BYTES:u64=240;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1817b7ce8903797288ccf293373f81890c75af59d03d948339943a531ab754fe";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.magnetic_field.wire_size()?)?;
for element in &self.magnetic_field_covariance {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>240{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.magnetic_field=decoded;
{let count=9usize;if count>9{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.magnetic_field_covariance[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Sensor_msgsMagneticField {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>240{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<112{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 std_srvs/SetBoolRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Std_srvsSetBoolRequest {
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:bool,
}
impl Default for Ros1Std_srvsSetBoolRequest {fn default()->Self {Self {
data:Default::default(),
}}}
impl Ros1Std_srvsSetBoolRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1621dbb16e2455a199cc871a0e784642a73bc57e76a4d524778163fab97f6aac";
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
impl Canonical for Ros1Std_srvsSetBoolRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 std_srvs/SetBoolResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Std_srvsSetBoolResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Std_srvsSetBoolResponse {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros1Std_srvsSetBoolResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1e12b91618f540070e34e170e12db7659f8686cdf5fb8e1bf098b8d483522cd6";
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
impl Canonical for Ros1Std_srvsSetBoolResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 std_srvs/TriggerRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Std_srvsTriggerRequest {
}
impl Default for Ros1Std_srvsTriggerRequest {fn default()->Self {Self {
}}}
impl Ros1Std_srvsTriggerRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d4e98dca9dc9a031d6b0f51937613f4f29c36156f0f05475339149794d47db1e";
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
impl Canonical for Ros1Std_srvsTriggerRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 std_srvs/TriggerResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Std_srvsTriggerResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Std_srvsTriggerResponse {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros1Std_srvsTriggerResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e625281ddea64aff38d8647e2673b418b77474cae0501500b280cabb3d247d1d";
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
impl Canonical for Ros1Std_srvsTriggerResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_example/Turtlebot3Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_exampleTurtlebot3Goal {
  /// Declared goal; canonical bounds are part of CODEC_IDENTITY.
  pub goal:Ros1Geometry_msgsVector3,
}
impl Default for Ros1Turtlebot3_exampleTurtlebot3Goal {fn default()->Self {Self {
goal:Default::default(),
}}}
impl Ros1Turtlebot3_exampleTurtlebot3Goal {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f6eb642301e239005c1790cfd5b9ff00388d98d27a17cbb0edb103b6ac7bfad5";
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
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.goal=decoded;
Ok(value)}
}
impl Canonical for Ros1Turtlebot3_exampleTurtlebot3Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_example/Turtlebot3Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_exampleTurtlebot3Result {
  /// Declared result; canonical bounds are part of CODEC_IDENTITY.
  pub result:String,
}
impl Default for Ros1Turtlebot3_exampleTurtlebot3Result {fn default()->Self {Self {
result:Default::default(),
}}}
impl Ros1Turtlebot3_exampleTurtlebot3Result {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c749a56ed9a0c74d1b167f69cc9b4435f9549d49144ef2951a4c3db501bad528";
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
impl Canonical for Ros1Turtlebot3_exampleTurtlebot3Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_example/Turtlebot3Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_exampleTurtlebot3Feedback {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
}
impl Default for Ros1Turtlebot3_exampleTurtlebot3Feedback {fn default()->Self {Self {
state:Default::default(),
}}}
impl Ros1Turtlebot3_exampleTurtlebot3Feedback {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_077b5593a1e4c23c89c4d6a87dc1d67d8fa1bf04fa3aa440f54c8eda60b688f8";
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
impl Canonical for Ros1Turtlebot3_exampleTurtlebot3Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_msgs/SensorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_msgsSensorState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
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
impl Default for Ros1Turtlebot3_msgsSensorState {fn default()->Self {Self {
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
impl Ros1Turtlebot3_msgsSensorState {
  pub const MAX_WIRE_BYTES:usize=172;
  pub const CARTESIAN_WIRE_BYTES:u64=172;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0ef7dd1a4b965ae027b081a6b34f111731a3adfbabcc6fd547d4da7378fcb81f";
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
if n>172{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
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
impl Canonical for Ros1Turtlebot3_msgsSensorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>172{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_msgs/Sound.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_msgsSound {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
}
impl Default for Ros1Turtlebot3_msgsSound {fn default()->Self {Self {
value:Default::default(),
}}}
impl Ros1Turtlebot3_msgsSound {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0e5af3d5a2acbfff5bf5ba94c1520490fe281e70aff02cfb6ad31acd89e7536a";
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
impl Canonical for Ros1Turtlebot3_msgsSound {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 turtlebot3_msgs/VersionInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Turtlebot3_msgsVersionInfo {
  /// Declared hardware; canonical bounds are part of CODEC_IDENTITY.
  pub hardware:String,
  /// Declared firmware; canonical bounds are part of CODEC_IDENTITY.
  pub firmware:String,
  /// Declared software; canonical bounds are part of CODEC_IDENTITY.
  pub software:String,
}
impl Default for Ros1Turtlebot3_msgsVersionInfo {fn default()->Self {Self {
hardware:Default::default(),
firmware:Default::default(),
software:Default::default(),
}}}
impl Ros1Turtlebot3_msgsVersionInfo {
  pub const MAX_WIRE_BYTES:usize=780;
  pub const CARTESIAN_WIRE_BYTES:u64=780;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8f41d37cf43fda2e3649d85fcca83ad735dc5a8122f7829a98b18659bcad8b3a";
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
impl Canonical for Ros1Turtlebot3_msgsVersionInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>780{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros1Sensor_msgsBatteryState {pub const POWER_SUPPLY_STATUS_UNKNOWN:u8=0;
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
pub const POWER_SUPPLY_TECHNOLOGY_LIMN:u8=6;}

impl Ros1Turtlebot3_msgsSensorState {pub const BUMPER_FORWARD:u8=1;
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

impl Ros1Turtlebot3_msgsSound {pub const OFF:u8=0;
pub const ON:u8=1;
pub const LOW_BATTERY:u8=2;
pub const ERROR:u8=3;
pub const BUTTON1:u8=4;
pub const BUTTON2:u8=5;}
