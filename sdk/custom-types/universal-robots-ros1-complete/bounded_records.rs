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
/// Complete bounded value for ROS1 geometry_msgs/PoseStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsPoseStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros1Std_msgsHeader,
  /// Declared pose; canonical bounds are part of CODEC_IDENTITY.
  pub pose:Ros1Geometry_msgsPose,
}
impl Default for Ros1Geometry_msgsPoseStamped {fn default()->Self {Self {
header:Default::default(),
pose:Default::default(),
}}}
impl Ros1Geometry_msgsPoseStamped {
  pub const MAX_WIRE_BYTES:usize=200;
  pub const CARTESIAN_WIRE_BYTES:u64=200;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bb59c4e82f561266c0f10500bfbbd1a809e9c1122ecbf559a812326af3eb2e95";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,self.pose.wire_size()?)?;
if n>200{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
self.pose.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Std_msgsHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=Ros1Geometry_msgsPose::read_fields(bytes,at)?;
value.pose=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsPoseStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>200{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<72{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS1 geometry_msgs/Wrench.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Geometry_msgsWrench {
  /// Declared force; canonical bounds are part of CODEC_IDENTITY.
  pub force:Ros1Geometry_msgsVector3,
  /// Declared torque; canonical bounds are part of CODEC_IDENTITY.
  pub torque:Ros1Geometry_msgsVector3,
}
impl Default for Ros1Geometry_msgsWrench {fn default()->Self {Self {
force:Default::default(),
torque:Default::default(),
}}}
impl Ros1Geometry_msgsWrench {
  pub const MAX_WIRE_BYTES:usize=48;
  pub const CARTESIAN_WIRE_BYTES:u64=48;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b0938c4e084df68cf7c46445070ac3e9305d06f678275845aec6055db77617c3";
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
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.force=decoded;
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.torque=decoded;
Ok(value)}
}
impl Canonical for Ros1Geometry_msgsWrench {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>48{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/AddToLogRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsAddToLogRequest {
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Ur_dashboard_msgsAddToLogRequest {fn default()->Self {Self {
message:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsAddToLogRequest {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_faf6c1ab9c749a05ae3f4e6196cc1c3574560daae1463ebe8684bf13ea938ac4";
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
impl Canonical for Ros1Ur_dashboard_msgsAddToLogRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/AddToLogResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsAddToLogResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsAddToLogResponse {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsAddToLogResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d495f7609d04abde44cb35ec5114618a2875e0a30986d2f7cc62c8ce99354e7c";
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
impl Canonical for Ros1Ur_dashboard_msgsAddToLogResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetLoadedProgramRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetLoadedProgramRequest {
}
impl Default for Ros1Ur_dashboard_msgsGetLoadedProgramRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsGetLoadedProgramRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_54eecd0b652707f9c16dcbacbef05902112db993500404e1adaf26618e1d3a1a";
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
impl Canonical for Ros1Ur_dashboard_msgsGetLoadedProgramRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetLoadedProgramResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetLoadedProgramResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsGetLoadedProgramResponse {fn default()->Self {Self {
answer:Default::default(),
program_name:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsGetLoadedProgramResponse {
  pub const MAX_WIRE_BYTES:usize=521;
  pub const CARTESIAN_WIRE_BYTES:u64=521;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1680747345d40412a4731c48f08c402ac97fb12dd871fb320c2a9d002453f87a";
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
impl Canonical for Ros1Ur_dashboard_msgsGetLoadedProgramResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>521{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetProgramStateRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetProgramStateRequest {
}
impl Default for Ros1Ur_dashboard_msgsGetProgramStateRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsGetProgramStateRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cf14c6f86ef1904f80e88117dc2f2d29a003b52e21bc6569795771f04cf072a2";
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
impl Canonical for Ros1Ur_dashboard_msgsGetProgramStateRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/ProgramState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsProgramState {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:String,
}
impl Default for Ros1Ur_dashboard_msgsProgramState {fn default()->Self {Self {
state:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsProgramState {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_97a57db64f20abcb9929b5296c75d66d994f2e9e26449810197c657694d3850e";
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
impl Canonical for Ros1Ur_dashboard_msgsProgramState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetProgramStateResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetProgramStateResponse {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:Ros1Ur_dashboard_msgsProgramState,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsGetProgramStateResponse {fn default()->Self {Self {
state:Default::default(),
program_name:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsGetProgramStateResponse {
  pub const MAX_WIRE_BYTES:usize=781;
  pub const CARTESIAN_WIRE_BYTES:u64=781;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_366caf89303bfff48d3c0f89aa2a640c3d29af5a0c19d76ceccf26d50c3ab2ed";
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
let decoded=Ros1Ur_dashboard_msgsProgramState::read_fields(bytes,at)?;
value.state=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.program_name=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_dashboard_msgsGetProgramStateResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>781{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetRobotModeRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetRobotModeRequest {
}
impl Default for Ros1Ur_dashboard_msgsGetRobotModeRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsGetRobotModeRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b9ffe283d7dbcb85879dbf6964d393f5d44661227bb26e1acc34308edb288609";
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
impl Canonical for Ros1Ur_dashboard_msgsGetRobotModeRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/RobotMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsRobotMode {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:i8,
}
impl Default for Ros1Ur_dashboard_msgsRobotMode {fn default()->Self {Self {
mode:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsRobotMode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b2772853eff2ec9b849b6de4a90ce98487a8eff49d1371283dce503908702066";
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
impl Canonical for Ros1Ur_dashboard_msgsRobotMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetRobotModeResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetRobotModeResponse {
  /// Declared robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub robot_mode:Ros1Ur_dashboard_msgsRobotMode,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsGetRobotModeResponse {fn default()->Self {Self {
robot_mode:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsGetRobotModeResponse {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f6c0d0d03a956a34b00fd63540226b6abb8b106e2a35cb912b4886a5c4cecc3f";
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
let decoded=Ros1Ur_dashboard_msgsRobotMode::read_fields(bytes,at)?;
value.robot_mode=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_dashboard_msgsGetRobotModeResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetSafetyModeRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetSafetyModeRequest {
}
impl Default for Ros1Ur_dashboard_msgsGetSafetyModeRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsGetSafetyModeRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f697e2ff2bcd56523563060a46bca2e1b7af487128f05562fcb18bfa73cbca8b";
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
impl Canonical for Ros1Ur_dashboard_msgsGetSafetyModeRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/SafetyMode.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsSafetyMode {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
}
impl Default for Ros1Ur_dashboard_msgsSafetyMode {fn default()->Self {Self {
mode:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsSafetyMode {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5a0c2737166cf2e2bd4c17b46c597f674b8d806b802d4e86ee0d76fc55f29cf7";
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
impl Canonical for Ros1Ur_dashboard_msgsSafetyMode {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/GetSafetyModeResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsGetSafetyModeResponse {
  /// Declared safety_mode; canonical bounds are part of CODEC_IDENTITY.
  pub safety_mode:Ros1Ur_dashboard_msgsSafetyMode,
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsGetSafetyModeResponse {fn default()->Self {Self {
safety_mode:Default::default(),
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsGetSafetyModeResponse {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_70121af922588eaab9a34855e3da0cde837b1449d2a2d6c28addc70916df1a15";
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
let decoded=Ros1Ur_dashboard_msgsSafetyMode::read_fields(bytes,at)?;
value.safety_mode=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_dashboard_msgsGetSafetyModeResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsInRemoteControlRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsInRemoteControlRequest {
}
impl Default for Ros1Ur_dashboard_msgsIsInRemoteControlRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsIsInRemoteControlRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b3c39f36f953904eb582d8c5f91161f2864782d4a4ab209a7dab32a117175da";
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
impl Canonical for Ros1Ur_dashboard_msgsIsInRemoteControlRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsInRemoteControlResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsInRemoteControlResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared in_remote_control; canonical bounds are part of CODEC_IDENTITY.
  pub in_remote_control:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsIsInRemoteControlResponse {fn default()->Self {Self {
answer:Default::default(),
in_remote_control:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsIsInRemoteControlResponse {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6c4af27571b48a5b8e8f1f857b2c02a333e7b4b360f8f3645a39630594c1663a";
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
put(out,at,&[u8::from(self.in_remote_control)]);
put(out,at,&[u8::from(self.success)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.answer=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.in_remote_control=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_dashboard_msgsIsInRemoteControlResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsProgramRunningRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsProgramRunningRequest {
}
impl Default for Ros1Ur_dashboard_msgsIsProgramRunningRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsIsProgramRunningRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0be481297028886e15f07ace4178ca0aec595724cd83ab01d0155e7c6aa6666f";
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
impl Canonical for Ros1Ur_dashboard_msgsIsProgramRunningRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsProgramRunningResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsProgramRunningResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_running; canonical bounds are part of CODEC_IDENTITY.
  pub program_running:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsIsProgramRunningResponse {fn default()->Self {Self {
answer:Default::default(),
program_running:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsIsProgramRunningResponse {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d9de4f7e7071d142c09e3184d4a6f29d4044d003ad11ba3b72e1129ecf144ac";
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
impl Canonical for Ros1Ur_dashboard_msgsIsProgramRunningResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsProgramSavedRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsProgramSavedRequest {
}
impl Default for Ros1Ur_dashboard_msgsIsProgramSavedRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_dashboard_msgsIsProgramSavedRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_45ce1466c35adc812096bfb8b2cd84dfa87940109831c9f797b5786afa14f5a8";
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
impl Canonical for Ros1Ur_dashboard_msgsIsProgramSavedRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/IsProgramSavedResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsIsProgramSavedResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared program_name; canonical bounds are part of CODEC_IDENTITY.
  pub program_name:String,
  /// Declared program_saved; canonical bounds are part of CODEC_IDENTITY.
  pub program_saved:bool,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsIsProgramSavedResponse {fn default()->Self {Self {
answer:Default::default(),
program_name:Default::default(),
program_saved:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsIsProgramSavedResponse {
  pub const MAX_WIRE_BYTES:usize=522;
  pub const CARTESIAN_WIRE_BYTES:u64=522;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_eb53e077fe7b216e27fc57f838666a54a84e479e02e508d41df5cff9f0637a52";
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
impl Canonical for Ros1Ur_dashboard_msgsIsProgramSavedResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>522{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<10{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/LoadRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsLoadRequest {
  /// Declared filename; canonical bounds are part of CODEC_IDENTITY.
  pub filename:String,
}
impl Default for Ros1Ur_dashboard_msgsLoadRequest {fn default()->Self {Self {
filename:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsLoadRequest {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dfbde7b2376c7c9ba3926e4803f16b22f7ade3b2f768a25886a1490656edd2bd";
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
impl Canonical for Ros1Ur_dashboard_msgsLoadRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/LoadResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsLoadResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsLoadResponse {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsLoadResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0ed3b3f2a0066248734e64c0706e52ebf07113569a98baf249a99adfff59be02";
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
impl Canonical for Ros1Ur_dashboard_msgsLoadResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/PopupRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsPopupRequest {
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Ur_dashboard_msgsPopupRequest {fn default()->Self {Self {
message:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsPopupRequest {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_11556bfb811a28141b6c5e9c11720920a389d735b8c09ed1a8f47d05f293e90d";
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
impl Canonical for Ros1Ur_dashboard_msgsPopupRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/PopupResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsPopupResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_dashboard_msgsPopupResponse {fn default()->Self {Self {
answer:Default::default(),
success:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsPopupResponse {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_33ac5b2128d6e7c2e562af7e2b8a8e907c16c07034b377920cba4fbd04ca9e14";
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
impl Canonical for Ros1Ur_dashboard_msgsPopupResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/RawRequestRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsRawRequestRequest {
  /// Declared query; canonical bounds are part of CODEC_IDENTITY.
  pub query:String,
}
impl Default for Ros1Ur_dashboard_msgsRawRequestRequest {fn default()->Self {Self {
query:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsRawRequestRequest {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ca8e3fdb45888bf4feb3c0f4ed05b7b9499a74493aaa0a8727f9f513922adfb4";
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
impl Canonical for Ros1Ur_dashboard_msgsRawRequestRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/RawRequestResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsRawRequestResponse {
  /// Declared answer; canonical bounds are part of CODEC_IDENTITY.
  pub answer:String,
}
impl Default for Ros1Ur_dashboard_msgsRawRequestResponse {fn default()->Self {Self {
answer:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsRawRequestResponse {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_76dbb0c8d0143bd4d39bbf66c6866b0771013fe67fbad76887c6490c91f39e80";
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
impl Canonical for Ros1Ur_dashboard_msgsRawRequestResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/SetModeGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsSetModeGoal {
  /// Declared target_robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub target_robot_mode:Ros1Ur_dashboard_msgsRobotMode,
  /// Declared stop_program; canonical bounds are part of CODEC_IDENTITY.
  pub stop_program:bool,
  /// Declared play_program; canonical bounds are part of CODEC_IDENTITY.
  pub play_program:bool,
}
impl Default for Ros1Ur_dashboard_msgsSetModeGoal {fn default()->Self {Self {
target_robot_mode:Default::default(),
stop_program:Default::default(),
play_program:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsSetModeGoal {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_27ba58b57af47e8efff15a70fb0562d5904d019c17e39a499bf003730434a528";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.target_robot_mode.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.target_robot_mode.write_fields(out,at);
put(out,at,&[u8::from(self.stop_program)]);
put(out,at,&[u8::from(self.play_program)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Ur_dashboard_msgsRobotMode::read_fields(bytes,at)?;
value.target_robot_mode=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.stop_program=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.play_program=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_dashboard_msgsSetModeGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/SetModeResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsSetModeResult {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros1Ur_dashboard_msgsSetModeResult {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsSetModeResult {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e558d01061183cabca7022bd66aeb1958e14dfc62226a701c86020e73ffa1114";
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
impl Canonical for Ros1Ur_dashboard_msgsSetModeResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_dashboard_msgs/SetModeFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_dashboard_msgsSetModeFeedback {
  /// Declared current_robot_mode; canonical bounds are part of CODEC_IDENTITY.
  pub current_robot_mode:i8,
  /// Declared current_safety_mode; canonical bounds are part of CODEC_IDENTITY.
  pub current_safety_mode:i8,
}
impl Default for Ros1Ur_dashboard_msgsSetModeFeedback {fn default()->Self {Self {
current_robot_mode:Default::default(),
current_safety_mode:Default::default(),
}}}
impl Ros1Ur_dashboard_msgsSetModeFeedback {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d29b507dcc5d72ee086c8a5a9a7a762d677ed408ec02dbe661abc17e2de5c7e2";
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
impl Canonical for Ros1Ur_dashboard_msgsSetModeFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/Analog.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsAnalog {
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:u8,
  /// Declared domain; canonical bounds are part of CODEC_IDENTITY.
  pub domain:u8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:f32,
}
impl Default for Ros1Ur_msgsAnalog {fn default()->Self {Self {
pin:Default::default(),
domain:Default::default(),
state:Default::default(),
}}}
impl Ros1Ur_msgsAnalog {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_638dc3b30def8193a501b4c1c778792a60c8c683a96880e9fa6db6c01774f6a7";
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
impl Canonical for Ros1Ur_msgsAnalog {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/Digital.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsDigital {
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:u8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:bool,
}
impl Default for Ros1Ur_msgsDigital {fn default()->Self {Self {
pin:Default::default(),
state:Default::default(),
}}}
impl Ros1Ur_msgsDigital {
  pub const MAX_WIRE_BYTES:usize=2;
  pub const CARTESIAN_WIRE_BYTES:u64=2;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c5840cfc5e26fe2f18817e46a0322a92e02ca21983920c71ebeb585769f5622c";
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
impl Canonical for Ros1Ur_msgsDigital {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<2{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/GetRobotSoftwareVersionRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsGetRobotSoftwareVersionRequest {
}
impl Default for Ros1Ur_msgsGetRobotSoftwareVersionRequest {fn default()->Self {Self {
}}}
impl Ros1Ur_msgsGetRobotSoftwareVersionRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bbcdd98ee8d951a8a4fa873ad1c755f16c5cce14ceb7a614343dd44ccf84b9c8";
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
impl Canonical for Ros1Ur_msgsGetRobotSoftwareVersionRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/GetRobotSoftwareVersionResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsGetRobotSoftwareVersionResponse {
  /// Declared major; canonical bounds are part of CODEC_IDENTITY.
  pub major:u32,
  /// Declared minor; canonical bounds are part of CODEC_IDENTITY.
  pub minor:u32,
  /// Declared bugfix; canonical bounds are part of CODEC_IDENTITY.
  pub bugfix:u32,
  /// Declared build; canonical bounds are part of CODEC_IDENTITY.
  pub build:u32,
}
impl Default for Ros1Ur_msgsGetRobotSoftwareVersionResponse {fn default()->Self {Self {
major:Default::default(),
minor:Default::default(),
bugfix:Default::default(),
build:Default::default(),
}}}
impl Ros1Ur_msgsGetRobotSoftwareVersionResponse {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_36ab51ce55e3630f8ca71f364f4c5cc1f4b010f678777e4fd7b03dd480f92e64";
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
impl Canonical for Ros1Ur_msgsGetRobotSoftwareVersionResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/IOStates.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsIOStates {
  /// Declared digital_in_states; canonical bounds are part of CODEC_IDENTITY.
  pub digital_in_states:Vec<Ros1Ur_msgsDigital>,
  /// Declared digital_out_states; canonical bounds are part of CODEC_IDENTITY.
  pub digital_out_states:Vec<Ros1Ur_msgsDigital>,
  /// Declared flag_states; canonical bounds are part of CODEC_IDENTITY.
  pub flag_states:Vec<Ros1Ur_msgsDigital>,
  /// Declared analog_in_states; canonical bounds are part of CODEC_IDENTITY.
  pub analog_in_states:Vec<Ros1Ur_msgsAnalog>,
  /// Declared analog_out_states; canonical bounds are part of CODEC_IDENTITY.
  pub analog_out_states:Vec<Ros1Ur_msgsAnalog>,
}
impl Default for Ros1Ur_msgsIOStates {fn default()->Self {Self {
digital_in_states:Default::default(),
digital_out_states:Default::default(),
flag_states:Default::default(),
analog_in_states:Default::default(),
analog_out_states:Default::default(),
}}}
impl Ros1Ur_msgsIOStates {
  pub const MAX_WIRE_BYTES:usize=404;
  pub const CARTESIAN_WIRE_BYTES:u64=404;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4b10f6efdd2f129267c407bc1f8caed03776fef6d49f0a1b9dfbf2451ad8f033";
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
let decoded=Ros1Ur_msgsDigital::read_fields(bytes,at)?;
value.digital_in_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.digital_out_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Ur_msgsDigital::read_fields(bytes,at)?;
value.digital_out_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.flag_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Ur_msgsDigital::read_fields(bytes,at)?;
value.flag_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.analog_in_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Ur_msgsAnalog::read_fields(bytes,at)?;
value.analog_in_states.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>16{return Err(invalid());}
value.analog_out_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Ur_msgsAnalog::read_fields(bytes,at)?;
value.analog_out_states.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros1Ur_msgsIOStates {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>404{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/MasterboardDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsMasterboardDataMsg {
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
  /// Declared robot_voltage_48V; canonical bounds are part of CODEC_IDENTITY.
  pub robot_voltage_48V:f32,
  /// Declared robot_current; canonical bounds are part of CODEC_IDENTITY.
  pub robot_current:f32,
  /// Declared master_io_current; canonical bounds are part of CODEC_IDENTITY.
  pub master_io_current:f32,
  /// Declared master_safety_state; canonical bounds are part of CODEC_IDENTITY.
  pub master_safety_state:u8,
  /// Declared master_onoff_state; canonical bounds are part of CODEC_IDENTITY.
  pub master_onoff_state:u8,
}
impl Default for Ros1Ur_msgsMasterboardDataMsg {fn default()->Self {Self {
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
robot_voltage_48V:Default::default(),
robot_current:Default::default(),
master_io_current:Default::default(),
master_safety_state:Default::default(),
master_onoff_state:Default::default(),
}}}
impl Ros1Ur_msgsMasterboardDataMsg {
  pub const MAX_WIRE_BYTES:usize=62;
  pub const CARTESIAN_WIRE_BYTES:u64=62;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f80f92b90cb33dd5b118df81447dfcc33c1ab230a91bb915a1be684401da265f";
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
if !self.robot_voltage_48V.is_finite(){return Err(invalid());}
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
put(out,at,&self.robot_voltage_48V.to_le_bytes());
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
value.robot_voltage_48V=decoded;
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
impl Canonical for Ros1Ur_msgsMasterboardDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>62{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<62{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/RobotModeDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsRobotModeDataMsg {
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
impl Default for Ros1Ur_msgsRobotModeDataMsg {fn default()->Self {Self {
timestamp:Default::default(),
is_robot_connected:Default::default(),
is_real_robot_enabled:Default::default(),
is_power_on_robot:Default::default(),
is_emergency_stopped:Default::default(),
is_protective_stopped:Default::default(),
is_program_running:Default::default(),
is_program_paused:Default::default(),
}}}
impl Ros1Ur_msgsRobotModeDataMsg {
  pub const MAX_WIRE_BYTES:usize=15;
  pub const CARTESIAN_WIRE_BYTES:u64=15;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_64c4247d865e0731b13ca13ce2e355df664cabf7f3d646051d9d3ef48b05a231";
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
impl Canonical for Ros1Ur_msgsRobotModeDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>15{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<15{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/RobotStateRTMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsRobotStateRTMsg {
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
impl Default for Ros1Ur_msgsRobotStateRTMsg {fn default()->Self {Self {
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
impl Ros1Ur_msgsRobotStateRTMsg {
  pub const MAX_WIRE_BYTES:usize=768;
  pub const CARTESIAN_WIRE_BYTES:u64=768;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e3d93b6141f749b93c229d8eabebbeb2782a16621fea4d7d39096794e7a67ce7";
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
impl Canonical for Ros1Ur_msgsRobotStateRTMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>768{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<96{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetAnalogOutputRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetAnalogOutputRequest {
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:Ros1Ur_msgsAnalog,
}
impl Default for Ros1Ur_msgsSetAnalogOutputRequest {fn default()->Self {Self {
data:Default::default(),
}}}
impl Ros1Ur_msgsSetAnalogOutputRequest {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1830a63c8853ebe8842f0e8e6870fbdee136dfe78726ba581fd1c54f910c4789";
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
let decoded=Ros1Ur_msgsAnalog::read_fields(bytes,at)?;
value.data=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_msgsSetAnalogOutputRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetAnalogOutputResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetAnalogOutputResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_msgsSetAnalogOutputResponse {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros1Ur_msgsSetAnalogOutputResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bb84c969f904082699f67d385b093b0c6de97bfacc33eab86932f4564a823580";
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
impl Canonical for Ros1Ur_msgsSetAnalogOutputResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetForceModeRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetForceModeRequest {
  /// Declared task_frame; canonical bounds are part of CODEC_IDENTITY.
  pub task_frame:Ros1Geometry_msgsPoseStamped,
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
  pub wrench:Ros1Geometry_msgsWrench,
  /// Declared type; canonical bounds are part of CODEC_IDENTITY.
  pub r#type:u8,
  /// Declared speed_limits; canonical bounds are part of CODEC_IDENTITY.
  pub speed_limits:Ros1Geometry_msgsTwist,
  /// Declared deviation_limits; canonical bounds are part of CODEC_IDENTITY.
  pub deviation_limits:[f32;6],
  /// Declared damping_factor; canonical bounds are part of CODEC_IDENTITY.
  pub damping_factor:f32,
  /// Declared gain_scaling; canonical bounds are part of CODEC_IDENTITY.
  pub gain_scaling:f32,
}
impl Default for Ros1Ur_msgsSetForceModeRequest {fn default()->Self {Self {
task_frame:Default::default(),
selection_vector_x:Default::default(),
selection_vector_y:Default::default(),
selection_vector_z:Default::default(),
selection_vector_rx:Default::default(),
selection_vector_ry:Default::default(),
selection_vector_rz:Default::default(),
wrench:Default::default(),
r#type:Default::default(),
speed_limits:Default::default(),
deviation_limits:core::array::from_fn(|_|Default::default()),
damping_factor:Default::default(),
gain_scaling:Default::default(),
}}}
impl Ros1Ur_msgsSetForceModeRequest {
  pub const MAX_WIRE_BYTES:usize=335;
  pub const CARTESIAN_WIRE_BYTES:u64=335;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c148d6eb37258e10f5d16e76e41c334fc57a3d751c0c35c47be4cabf4c015a6b";
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
if n>335{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros1Geometry_msgsPoseStamped::read_fields(bytes,at)?;
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
let decoded=Ros1Geometry_msgsWrench::read_fields(bytes,at)?;
value.wrench=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.r#type=decoded;
let decoded=Ros1Geometry_msgsTwist::read_fields(bytes,at)?;
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
impl Canonical for Ros1Ur_msgsSetForceModeRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>335{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<207{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetForceModeResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetForceModeResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_msgsSetForceModeResponse {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros1Ur_msgsSetForceModeResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bb1eeb0b38d81d642066bed1510245423f62d25983a2de89e985d1fd1932308c";
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
impl Canonical for Ros1Ur_msgsSetForceModeResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetIORequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetIORequest {
  /// Declared fun; canonical bounds are part of CODEC_IDENTITY.
  pub fun:i8,
  /// Declared pin; canonical bounds are part of CODEC_IDENTITY.
  pub pin:i8,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:f32,
}
impl Default for Ros1Ur_msgsSetIORequest {fn default()->Self {Self {
fun:Default::default(),
pin:Default::default(),
state:Default::default(),
}}}
impl Ros1Ur_msgsSetIORequest {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3dbd2f05686ddaac16b37dc8ec98303acb672d176be34fc906cf32f2d4cd1872";
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
impl Canonical for Ros1Ur_msgsSetIORequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetIOResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetIOResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_msgsSetIOResponse {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros1Ur_msgsSetIOResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7fc137aa3db3178a450b8b2396b7821c911c79656e52bf48814745c261aecf61";
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
impl Canonical for Ros1Ur_msgsSetIOResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetPayloadRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetPayloadRequest {
  /// Declared mass; canonical bounds are part of CODEC_IDENTITY.
  pub mass:f32,
  /// Declared center_of_gravity; canonical bounds are part of CODEC_IDENTITY.
  pub center_of_gravity:Ros1Geometry_msgsVector3,
}
impl Default for Ros1Ur_msgsSetPayloadRequest {fn default()->Self {Self {
mass:Default::default(),
center_of_gravity:Default::default(),
}}}
impl Ros1Ur_msgsSetPayloadRequest {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_63ae68b44ce6aaad37cbda779fb51dee78923df27c582b4aab4104b16a307ee3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.mass.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.center_of_gravity.wire_size()?)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mass.to_le_bytes());
self.center_of_gravity.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mass=decoded;
let decoded=Ros1Geometry_msgsVector3::read_fields(bytes,at)?;
value.center_of_gravity=decoded;
Ok(value)}
}
impl Canonical for Ros1Ur_msgsSetPayloadRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetPayloadResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetPayloadResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_msgsSetPayloadResponse {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros1Ur_msgsSetPayloadResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a3e991f1357f2d3017ccbb7636a6248e8774434d723f5f6c77ff251dd61fce73";
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
impl Canonical for Ros1Ur_msgsSetPayloadResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetSpeedSliderFractionRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetSpeedSliderFractionRequest {
  /// Declared speed_slider_fraction; canonical bounds are part of CODEC_IDENTITY.
  pub speed_slider_fraction:f64,
}
impl Default for Ros1Ur_msgsSetSpeedSliderFractionRequest {fn default()->Self {Self {
speed_slider_fraction:Default::default(),
}}}
impl Ros1Ur_msgsSetSpeedSliderFractionRequest {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f192d6d655b2ed7725bf5afe6977a352870abfd4d07e0c3380a09d280a0a0a32";
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
impl Canonical for Ros1Ur_msgsSetSpeedSliderFractionRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/SetSpeedSliderFractionResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsSetSpeedSliderFractionResponse {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
}
impl Default for Ros1Ur_msgsSetSpeedSliderFractionResponse {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros1Ur_msgsSetSpeedSliderFractionResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cb08c6758ce2eeeac93bc579d7895b19b84e74b7e2d19d87831023df579288b0";
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
impl Canonical for Ros1Ur_msgsSetSpeedSliderFractionResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ur_msgs/ToolDataMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ur_msgsToolDataMsg {
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
impl Default for Ros1Ur_msgsToolDataMsg {fn default()->Self {Self {
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
impl Ros1Ur_msgsToolDataMsg {
  pub const MAX_WIRE_BYTES:usize=32;
  pub const CARTESIAN_WIRE_BYTES:u64=32;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1fad1aba069bc474306dc421a62950ffeb3a0837668f08a9adf41ac1bc2bb390";
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
impl Canonical for Ros1Ur_msgsToolDataMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>32{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<32{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros1Ur_dashboard_msgsProgramState {pub const STOPPED:&'static str="STOPPED";
pub const PLAYING:&'static str="PLAYING";
pub const PAUSED:&'static str="PAUSED";}

impl Ros1Ur_dashboard_msgsRobotMode {pub const NO_CONTROLLER:i8=-1;
pub const DISCONNECTED:i8=0;
pub const CONFIRM_SAFETY:i8=1;
pub const BOOTING:i8=2;
pub const POWER_OFF:i8=3;
pub const POWER_ON:i8=4;
pub const IDLE:i8=5;
pub const BACKDRIVE:i8=6;
pub const RUNNING:i8=7;
pub const UPDATING_FIRMWARE:i8=8;}

impl Ros1Ur_dashboard_msgsSafetyMode {pub const NORMAL:u8=1;
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

impl Ros1Ur_msgsAnalog {pub const CURRENT:u8=0;
pub const VOLTAGE:u8=1;}

impl Ros1Ur_msgsSetForceModeRequest {pub const TCP_TO_ORIGIN:u8=1;
pub const NO_TRANSFORM:u8=2;
pub const TCP_VELOCITY_TO_X_Y:u8=3;}

impl Ros1Ur_msgsSetIORequest {pub const PIN_AOUT0:i8=0;
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

impl Ros1Ur_msgsToolDataMsg {pub const ANALOG_INPUT_RANGE_CURRENT:i8=0;
pub const ANALOG_INPUT_RANGE_VOLTAGE:i8=1;
pub const TOOL_BOOTLOADER_MODE:u8=249;
pub const TOOL_RUNNING_MODE:u8=253;
pub const TOOL_IDLE_MODE:u8=255;}
