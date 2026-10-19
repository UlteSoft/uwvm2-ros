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
  pub const MAX_WIRE_BYTES:usize=956;
  pub const CARTESIAN_WIRE_BYTES:u64=956;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c10d3ce1f2824c33d728d0006fb83134330d0332af122f179cf2dac32181a2ca";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.child_frame_id.len()>128||self.child_frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.child_frame_id.len())?;
n=add_size(n,self.pose.wire_size()?)?;
n=add_size(n,self.twist.wire_size()?)?;
if n>956{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
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
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>956{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<700{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutBmsStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutBmsStatus {
  /// Declared SOC; canonical bounds are part of CODEC_IDENTITY.
  pub SOC:f64,
  /// Declared SOH; canonical bounds are part of CODEC_IDENTITY.
  pub SOH:f64,
  /// Declared battery_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub battery_voltage:f64,
  /// Declared battery_current; canonical bounds are part of CODEC_IDENTITY.
  pub battery_current:f64,
  /// Declared battery_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub battery_temperature:f64,
  /// Declared Alarm_Status_1; canonical bounds are part of CODEC_IDENTITY.
  pub Alarm_Status_1:u8,
  /// Declared Alarm_Status_2; canonical bounds are part of CODEC_IDENTITY.
  pub Alarm_Status_2:u8,
  /// Declared Warning_Status_1; canonical bounds are part of CODEC_IDENTITY.
  pub Warning_Status_1:u8,
  /// Declared Warning_Status_2; canonical bounds are part of CODEC_IDENTITY.
  pub Warning_Status_2:u8,
}
impl Default for Ros1Scout_msgsScoutBmsStatus {fn default()->Self {Self {
SOC:Default::default(),
SOH:Default::default(),
battery_voltage:Default::default(),
battery_current:Default::default(),
battery_temperature:Default::default(),
Alarm_Status_1:Default::default(),
Alarm_Status_2:Default::default(),
Warning_Status_1:Default::default(),
Warning_Status_2:Default::default(),
}}}
impl Ros1Scout_msgsScoutBmsStatus {
  pub const MAX_WIRE_BYTES:usize=44;
  pub const CARTESIAN_WIRE_BYTES:u64=44;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6921538c08e86ab5b9ecdbf974c6cb5b580488f4ed96ccbdb8b7824d51e9cf95";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.SOC.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.SOH.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.battery_voltage.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.battery_current.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.battery_temperature.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>44{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.SOC.to_le_bytes());
put(out,at,&self.SOH.to_le_bytes());
put(out,at,&self.battery_voltage.to_le_bytes());
put(out,at,&self.battery_current.to_le_bytes());
put(out,at,&self.battery_temperature.to_le_bytes());
put(out,at,&self.Alarm_Status_1.to_le_bytes());
put(out,at,&self.Alarm_Status_2.to_le_bytes());
put(out,at,&self.Warning_Status_1.to_le_bytes());
put(out,at,&self.Warning_Status_2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.SOC=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.SOH=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery_voltage=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery_current=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery_temperature=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Alarm_Status_1=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Alarm_Status_2=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Warning_Status_1=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Warning_Status_2=decoded;
Ok(value)}
}
impl Canonical for Ros1Scout_msgsScoutBmsStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>44{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutDriverState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutDriverState {
  /// Declared driver_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub driver_voltage:f64,
  /// Declared driver_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub driver_temperature:f64,
  /// Declared driver_state; canonical bounds are part of CODEC_IDENTITY.
  pub driver_state:u8,
}
impl Default for Ros1Scout_msgsScoutDriverState {fn default()->Self {Self {
driver_voltage:Default::default(),
driver_temperature:Default::default(),
driver_state:Default::default(),
}}}
impl Ros1Scout_msgsScoutDriverState {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4c56c71ae621b2b8822d742894d19946492002453c93b72faccb4f2abc9d4275";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.driver_voltage.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.driver_temperature.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
if n>17{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.driver_voltage.to_le_bytes());
put(out,at,&self.driver_temperature.to_le_bytes());
put(out,at,&self.driver_state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.driver_voltage=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.driver_temperature=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.driver_state=decoded;
Ok(value)}
}
impl Canonical for Ros1Scout_msgsScoutDriverState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutLightCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutLightCmd {
  /// Declared enable_cmd_light_control; canonical bounds are part of CODEC_IDENTITY.
  pub enable_cmd_light_control:bool,
  /// Declared front_mode; canonical bounds are part of CODEC_IDENTITY.
  pub front_mode:u8,
  /// Declared front_custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub front_custom_value:u8,
  /// Declared rear_mode; canonical bounds are part of CODEC_IDENTITY.
  pub rear_mode:u8,
  /// Declared rear_custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub rear_custom_value:u8,
}
impl Default for Ros1Scout_msgsScoutLightCmd {fn default()->Self {Self {
enable_cmd_light_control:Default::default(),
front_mode:Default::default(),
front_custom_value:Default::default(),
rear_mode:Default::default(),
rear_custom_value:Default::default(),
}}}
impl Ros1Scout_msgsScoutLightCmd {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cf08cccf454f88111d5cc4a4b1ace08de72cf1ae9a9400c23a148126da95942a";
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
put(out,at,&[u8::from(self.enable_cmd_light_control)]);
put(out,at,&self.front_mode.to_le_bytes());
put(out,at,&self.front_custom_value.to_le_bytes());
put(out,at,&self.rear_mode.to_le_bytes());
put(out,at,&self.rear_custom_value.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.enable_cmd_light_control=decoded;
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
impl Canonical for Ros1Scout_msgsScoutLightCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutLightState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutLightState {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared custom_value; canonical bounds are part of CODEC_IDENTITY.
  pub custom_value:u8,
}
impl Default for Ros1Scout_msgsScoutLightState {fn default()->Self {Self {
mode:Default::default(),
custom_value:Default::default(),
}}}
impl Ros1Scout_msgsScoutLightState {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_af55048f9200cfa80fc419664204a515f628cc610a352ab3ed8443dd7ac0ae51";
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
impl Canonical for Ros1Scout_msgsScoutLightState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutMotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutMotorState {
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f64,
  /// Declared rpm; canonical bounds are part of CODEC_IDENTITY.
  pub rpm:f64,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:f64,
  /// Declared motor_pose; canonical bounds are part of CODEC_IDENTITY.
  pub motor_pose:f64,
}
impl Default for Ros1Scout_msgsScoutMotorState {fn default()->Self {Self {
current:Default::default(),
rpm:Default::default(),
temperature:Default::default(),
motor_pose:Default::default(),
}}}
impl Ros1Scout_msgsScoutMotorState {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ca67a704d7bb4ba6b2839db1fdb8a75bc753868ec871d101c24c85d8202a79b7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.rpm.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.temperature.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.motor_pose.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>32{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.rpm.to_le_bytes());
put(out,at,&self.temperature.to_le_bytes());
put(out,at,&self.motor_pose.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rpm=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.temperature=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.motor_pose=decoded;
Ok(value)}
}
impl Canonical for Ros1Scout_msgsScoutMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutRsStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutRsStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared stick_left_h; canonical bounds are part of CODEC_IDENTITY.
  pub stick_left_h:i8,
  /// Declared stick_left_v; canonical bounds are part of CODEC_IDENTITY.
  pub stick_left_v:i8,
  /// Declared stick_right_h; canonical bounds are part of CODEC_IDENTITY.
  pub stick_right_h:i8,
  /// Declared stick_right_v; canonical bounds are part of CODEC_IDENTITY.
  pub stick_right_v:i8,
  /// Declared swa; canonical bounds are part of CODEC_IDENTITY.
  pub swa:u8,
  /// Declared swb; canonical bounds are part of CODEC_IDENTITY.
  pub swb:u8,
  /// Declared swc; canonical bounds are part of CODEC_IDENTITY.
  pub swc:u8,
  /// Declared swd; canonical bounds are part of CODEC_IDENTITY.
  pub swd:u8,
  /// Declared var_a; canonical bounds are part of CODEC_IDENTITY.
  pub var_a:u8,
}
impl Default for Ros1Scout_msgsScoutRsStatus {fn default()->Self {Self {
header:Default::default(),
stick_left_h:Default::default(),
stick_left_v:Default::default(),
stick_right_h:Default::default(),
stick_right_v:Default::default(),
swa:Default::default(),
swb:Default::default(),
swc:Default::default(),
swd:Default::default(),
var_a:Default::default(),
}}}
impl Ros1Scout_msgsScoutRsStatus {
  pub const MAX_WIRE_BYTES:usize=153;
  pub const CARTESIAN_WIRE_BYTES:u64=153;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_deb068b75fb629df3724c141ed3be9f6ee8a4aac7727cdaa28769cb61211f7de";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>153{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.stick_left_h.to_le_bytes());
put(out,at,&self.stick_left_v.to_le_bytes());
put(out,at,&self.stick_right_h.to_le_bytes());
put(out,at,&self.stick_right_v.to_le_bytes());
put(out,at,&self.swa.to_le_bytes());
put(out,at,&self.swb.to_le_bytes());
put(out,at,&self.swc.to_le_bytes());
put(out,at,&self.swd.to_le_bytes());
put(out,at,&self.var_a.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_left_h=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_left_v=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_right_h=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.stick_right_v=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swa=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swb=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swc=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.swd=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.var_a=decoded;
Ok(value)}
}
impl Canonical for Ros1Scout_msgsScoutRsStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>153{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 scout_msgs/ScoutStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Scout_msgsScoutStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared linear_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub linear_velocity:f64,
  /// Declared angular_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub angular_velocity:f64,
  /// Declared lateral_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub lateral_velocity:f64,
  /// Declared base_state; canonical bounds are part of CODEC_IDENTITY.
  pub base_state:u8,
  /// Declared control_mode; canonical bounds are part of CODEC_IDENTITY.
  pub control_mode:u8,
  /// Declared fault_code; canonical bounds are part of CODEC_IDENTITY.
  pub fault_code:u16,
  /// Declared battery_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub battery_voltage:f64,
  /// Declared motor_states; canonical bounds are part of CODEC_IDENTITY.
  pub motor_states:[Ros1Scout_msgsScoutMotorState;4],
  /// Declared driver_states; canonical bounds are part of CODEC_IDENTITY.
  pub driver_states:[Ros1Scout_msgsScoutDriverState;4],
  /// Declared light_control_enabled; canonical bounds are part of CODEC_IDENTITY.
  pub light_control_enabled:bool,
  /// Declared front_light_state; canonical bounds are part of CODEC_IDENTITY.
  pub front_light_state:Ros1Scout_msgsScoutLightState,
  /// Declared rear_light_state; canonical bounds are part of CODEC_IDENTITY.
  pub rear_light_state:Ros1Scout_msgsScoutLightState,
}
impl Default for Ros1Scout_msgsScoutStatus {fn default()->Self {Self {
header:Default::default(),
linear_velocity:Default::default(),
angular_velocity:Default::default(),
lateral_velocity:Default::default(),
base_state:Default::default(),
control_mode:Default::default(),
fault_code:Default::default(),
battery_voltage:Default::default(),
motor_states:core::array::from_fn(|_|Default::default()),
driver_states:core::array::from_fn(|_|Default::default()),
light_control_enabled:Default::default(),
front_light_state:Default::default(),
rear_light_state:Default::default(),
}}}
impl Ros1Scout_msgsScoutStatus {
  pub const MAX_WIRE_BYTES:usize=381;
  pub const CARTESIAN_WIRE_BYTES:u64=381;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ff62854d4de0aae3dfbbc312822c6f912294f0feb04a7fff943df602d40bcbe2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if !self.linear_velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.angular_velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.lateral_velocity.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,2)?;
if !self.battery_voltage.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
for element in &self.motor_states {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.driver_states {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,1)?;
n=add_size(n,self.front_light_state.wire_size()?)?;
n=add_size(n,self.rear_light_state.wire_size()?)?;
if n>381{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.linear_velocity.to_le_bytes());
put(out,at,&self.angular_velocity.to_le_bytes());
put(out,at,&self.lateral_velocity.to_le_bytes());
put(out,at,&self.base_state.to_le_bytes());
put(out,at,&self.control_mode.to_le_bytes());
put(out,at,&self.fault_code.to_le_bytes());
put(out,at,&self.battery_voltage.to_le_bytes());
for element in &self.motor_states {
element.write_fields(out,at);
}
for element in &self.driver_states {
element.write_fields(out,at);
}
put(out,at,&[u8::from(self.light_control_enabled)]);
self.front_light_state.write_fields(out,at);
self.rear_light_state.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.linear_velocity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.angular_velocity=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lateral_velocity=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.base_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.control_mode=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.fault_code=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.battery_voltage=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Scout_msgsScoutMotorState::read_fields(bytes,at)?;
value.motor_states[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Scout_msgsScoutDriverState::read_fields(bytes,at)?;
value.driver_states[i]=decoded;
}}
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.light_control_enabled=decoded;
let decoded=Ros1Scout_msgsScoutLightState::read_fields(bytes,at)?;
value.front_light_state=decoded;
let decoded=Ros1Scout_msgsScoutLightState::read_fields(bytes,at)?;
value.rear_light_state=decoded;
Ok(value)}
}
impl Canonical for Ros1Scout_msgsScoutStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>381{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<253{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros1Scout_msgsScoutLightCmd {pub const LIGHT_CONST_OFF:u8=0;
pub const LIGHT_CONST_ON:u8=1;
pub const LIGHT_BREATH:u8=2;
pub const LIGHT_CUSTOM:u8=3;}

impl Ros1Scout_msgsScoutStatus {pub const MOTOR_ID_FRONT_RIGHT:i8=0;
pub const MOTOR_ID_FRONT_LEFT:i8=1;
pub const MOTOR_ID_REAR_RIGHT:i8=2;
pub const MOTOR_ID_REAR_LEFT:i8=3;
pub const LIGHT_ID_FRONT:i8=0;
pub const LIGHT_ID_REAR:i8=1;}
