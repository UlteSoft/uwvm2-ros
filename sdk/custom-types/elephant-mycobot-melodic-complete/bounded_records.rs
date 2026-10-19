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
/// Complete bounded value for ROS1 mybuddy_communication/MybuddyAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddyAngles {
  /// Declared joint_0; canonical bounds are part of CODEC_IDENTITY.
  pub joint_0:f32,
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared joint_8; canonical bounds are part of CODEC_IDENTITY.
  pub joint_8:f32,
  /// Declared joint_9; canonical bounds are part of CODEC_IDENTITY.
  pub joint_9:f32,
  /// Declared joint_10; canonical bounds are part of CODEC_IDENTITY.
  pub joint_10:f32,
  /// Declared joint_11; canonical bounds are part of CODEC_IDENTITY.
  pub joint_11:f32,
  /// Declared joint_12; canonical bounds are part of CODEC_IDENTITY.
  pub joint_12:f32,
}
impl Default for Ros1Mybuddy_communicationMybuddyAngles {fn default()->Self {Self {
joint_0:Default::default(),
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
joint_8:Default::default(),
joint_9:Default::default(),
joint_10:Default::default(),
joint_11:Default::default(),
joint_12:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddyAngles {
  pub const MAX_WIRE_BYTES:usize=52;
  pub const CARTESIAN_WIRE_BYTES:u64=52;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f3c510869207e6d72e868ae35ef95d49a1eea5164c7ef1056cb3d335029563e9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_0.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_10.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_11.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_12.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>52{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_0.to_le_bytes());
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.joint_8.to_le_bytes());
put(out,at,&self.joint_9.to_le_bytes());
put(out,at,&self.joint_10.to_le_bytes());
put(out,at,&self.joint_11.to_le_bytes());
put(out,at,&self.joint_12.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_0=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_9=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_10=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_11=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_12=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddyAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>52{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<52{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/MybuddyCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddyCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mybuddy_communicationMybuddyCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddyCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a1df376733f2673e25fed814a038e00f556b82c2b8bce3755644904e50c95384";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddyCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/MybuddyGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddyGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mybuddy_communicationMybuddyGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddyGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86fdaf7d3397b26155c891e15d2ebdd903d5899a0895128e720276eb35a63854";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddyGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/MybuddyPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddyPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mybuddy_communicationMybuddyPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddyPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_33b6419700f9baf41b584bb464d0234426a04e1bd86d198fe3d66c06845cd80a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddyPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/MybuddySetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddySetAngles {
  /// Declared joint_0; canonical bounds are part of CODEC_IDENTITY.
  pub joint_0:f32,
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared joint_8; canonical bounds are part of CODEC_IDENTITY.
  pub joint_8:f32,
  /// Declared joint_9; canonical bounds are part of CODEC_IDENTITY.
  pub joint_9:f32,
  /// Declared joint_10; canonical bounds are part of CODEC_IDENTITY.
  pub joint_10:f32,
  /// Declared joint_11; canonical bounds are part of CODEC_IDENTITY.
  pub joint_11:f32,
  /// Declared joint_12; canonical bounds are part of CODEC_IDENTITY.
  pub joint_12:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mybuddy_communicationMybuddySetAngles {fn default()->Self {Self {
joint_0:Default::default(),
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
joint_8:Default::default(),
joint_9:Default::default(),
joint_10:Default::default(),
joint_11:Default::default(),
joint_12:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddySetAngles {
  pub const MAX_WIRE_BYTES:usize=53;
  pub const CARTESIAN_WIRE_BYTES:u64=53;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8f41fddfb715bfaa95c90974cf257181a9ae3e2f36969f6fdfdf3ab03b7d4486";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_0.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_10.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_11.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_12.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>53{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_0.to_le_bytes());
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.joint_8.to_le_bytes());
put(out,at,&self.joint_9.to_le_bytes());
put(out,at,&self.joint_10.to_le_bytes());
put(out,at,&self.joint_11.to_le_bytes());
put(out,at,&self.joint_12.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_0=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_9=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_10=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_11=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_12=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddySetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>53{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<53{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/MybuddySetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationMybuddySetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mybuddy_communicationMybuddySetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mybuddy_communicationMybuddySetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bf9f2ee073928aafabbd532644eb57eec70d4b864a3097e2ab1a37840abd3669";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationMybuddySetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGetAnglesRequest {
}
impl Default for Ros1Mybuddy_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Mybuddy_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7ba90c12eb91dbcc296c0d61069d0dc12c3ac576c970cd2fe9a0fed29f058e38";
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
impl Canonical for Ros1Mybuddy_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGetAnglesResponse {
  /// Declared joint_0; canonical bounds are part of CODEC_IDENTITY.
  pub joint_0:f32,
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared joint_8; canonical bounds are part of CODEC_IDENTITY.
  pub joint_8:f32,
  /// Declared joint_9; canonical bounds are part of CODEC_IDENTITY.
  pub joint_9:f32,
  /// Declared joint_10; canonical bounds are part of CODEC_IDENTITY.
  pub joint_10:f32,
  /// Declared joint_11; canonical bounds are part of CODEC_IDENTITY.
  pub joint_11:f32,
  /// Declared joint_12; canonical bounds are part of CODEC_IDENTITY.
  pub joint_12:f32,
}
impl Default for Ros1Mybuddy_communicationGetAnglesResponse {fn default()->Self {Self {
joint_0:Default::default(),
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
joint_8:Default::default(),
joint_9:Default::default(),
joint_10:Default::default(),
joint_11:Default::default(),
joint_12:Default::default(),
}}}
impl Ros1Mybuddy_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=52;
  pub const CARTESIAN_WIRE_BYTES:u64=52;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_98085b01a21ae26a74a286d790aab310530fff2a88d7a3c35e071df09abd709d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_0.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_10.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_11.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_12.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>52{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_0.to_le_bytes());
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.joint_8.to_le_bytes());
put(out,at,&self.joint_9.to_le_bytes());
put(out,at,&self.joint_10.to_le_bytes());
put(out,at,&self.joint_11.to_le_bytes());
put(out,at,&self.joint_12.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_0=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_9=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_10=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_11=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_12=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>52{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<52{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGetCoordsRequest {
}
impl Default for Ros1Mybuddy_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Mybuddy_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5b4bd6f4d17f79bf3477c2577e73355b871523291aace267fea201f5a2b43ef";
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
impl Canonical for Ros1Mybuddy_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mybuddy_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mybuddy_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_604a9a58b11d6e51faee74226c4820db7668b2054b2e48949cf9deb5958f5873";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mybuddy_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mybuddy_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7816f3fa4c8b7068eeb16e74940d2e26401df47b7bc6881d123f94b554523173";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mybuddy_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mybuddy_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b329fd687ef106ed366d80c5651fc536ad266d2b16dee8f985f1bad32738330b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mybuddy_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mybuddy_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4261e7e11f0c6065de868444a0be45d363465b0913246b06b4a2c413714d77e9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mybuddy_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mybuddy_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_975f537d88c880f8daa3459f6762dc6ec7d7eee1f580c0e79f3cb581b77cabd4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationSetAnglesRequest {
  /// Declared joint_0; canonical bounds are part of CODEC_IDENTITY.
  pub joint_0:f32,
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared joint_8; canonical bounds are part of CODEC_IDENTITY.
  pub joint_8:f32,
  /// Declared joint_9; canonical bounds are part of CODEC_IDENTITY.
  pub joint_9:f32,
  /// Declared joint_10; canonical bounds are part of CODEC_IDENTITY.
  pub joint_10:f32,
  /// Declared joint_11; canonical bounds are part of CODEC_IDENTITY.
  pub joint_11:f32,
  /// Declared joint_12; canonical bounds are part of CODEC_IDENTITY.
  pub joint_12:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mybuddy_communicationSetAnglesRequest {fn default()->Self {Self {
joint_0:Default::default(),
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
joint_8:Default::default(),
joint_9:Default::default(),
joint_10:Default::default(),
joint_11:Default::default(),
joint_12:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mybuddy_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=53;
  pub const CARTESIAN_WIRE_BYTES:u64=53;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_47d19578189892ca10e073d8d250839f1e2b39ebbb3525ed9b03a15af18eb240";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_0.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_8.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_9.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_10.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_11.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_12.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>53{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_0.to_le_bytes());
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.joint_8.to_le_bytes());
put(out,at,&self.joint_9.to_le_bytes());
put(out,at,&self.joint_10.to_le_bytes());
put(out,at,&self.joint_11.to_le_bytes());
put(out,at,&self.joint_12.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_0=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_8=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_9=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_10=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_11=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_12=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>53{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<53{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mybuddy_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mybuddy_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7371fa369708af552fd68522d7effdd00bf82bf7e2fe21469ceb210f51582f81";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mybuddy_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mybuddy_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_81f7315bebd612a9406a2805b393415b8a1956e49f3d1b8efb21f9e703b0bfe0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mybuddy_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mybuddy_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mybuddy_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mybuddy_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5c41ea9420e43a5406ddee23cee961347bfad4f83baf9ebac93d16cdb5d0b21";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mybuddy_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mecharm_communicationMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4afa3dc66ce6ffe02170c76da688514a04aa7b47008aeb8f79c44f9ff867d189";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mecharm_communicationMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a6a82e22cd0a2f24657c0a253ed00bf31c0a266e6ebaab071cfd369780818d35";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mecharm_communicationMycobotGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_07e04dad52b423c371dc5432b68a2f4272cf55328d6f0a10f091dc389d57a4bf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mecharm_communicationMycobotPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f559fbf7d5d2c872b3660132eb8c17c79ca3c2d27e975f8fb8fde1d86072a6fa";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mecharm_communicationMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bce09b709e806417595c05e0e795d689b8c53008f9916633d934f935f9389de9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationMycobotSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mecharm_communicationMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mecharm_communicationMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_68ffa6950e0934b845b2b45140612997c8276a3ac7ed4303e43cd70417491913";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGetAnglesRequest {
}
impl Default for Ros1Mecharm_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Mecharm_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6a8f60e71495c9944e1e97c308d5c992674bd46060b6f6abf2df3f9748c13123";
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
impl Canonical for Ros1Mecharm_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mecharm_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mecharm_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e792a206903dc49fab725e3b3507af3cf4712f10740e9dcceeb90dedfe6ea6e8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGetCoordsRequest {
}
impl Default for Ros1Mecharm_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Mecharm_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_55d4d1b8965ea12f9b15d4bd5a22c43793d04af452fefb84bd98bc3d7d8da8bf";
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
impl Canonical for Ros1Mecharm_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mecharm_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mecharm_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2230e517685ea625507334067ccceb8424ecf782fcd4163c551d09048e900ca8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mecharm_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mecharm_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_98284fbac3bdb61857b9175b1f82adc7265d0ed418088123d79bf75cb04c9a2c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mecharm_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mecharm_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_29cfb0267fbe68d5dafcd3c9d16a8d4dab6b21477921a48ba35135bc0c22bb32";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mecharm_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mecharm_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aa138db91e46b41659dfc5901225aaaae1c1ae0461c35d3f7a90d0cf86c9920d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mecharm_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mecharm_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_876caa88c833bfa180b8747110683c789aa238c0cda162a8e262c66fddefdd8f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mecharm_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mecharm_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7ad54edf4730457f2b2124c925b87a99bdbeddd8bfcc1e0d806f7b9dad31507b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mecharm_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mecharm_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_940a294ef91d04afeb65b88bbdd91df3a7db0a07435ced3993db3a9d0f574c6e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mecharm_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mecharm_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_68deaf663a1fe2d3d7434935f10161eb444e7ea2f79e4b746fee9bd0383fc929";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mecharm_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mecharm_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mecharm_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mecharm_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c66a6e14c4c7eba1b52d7ef57802cb6122a120f692bcc1552c51078301b1b409";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mecharm_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
}
impl Default for Ros1Myarm_communicationMyArmAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmAngles {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_eff11a03e974b4af030c2dc92d0111cf95e0fb3ce004235bada3fe358b63fd4c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Myarm_communicationMyArmCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f0df1605988924f0263805fb2b27ab7ef9e2296691ed15e03a337606362c983c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Myarm_communicationMyArmGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7875df5b50417da1cef60d79c872aff0b40a08f9bbfe80c103729d3c4c892c6f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Myarm_communicationMyArmPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_58765b94f1ecb3bb131f6480ea6830631cde9c96eeef94fd9b22598ccc9ea928";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Myarm_communicationMyArmSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
speed:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmSetAngles {
  pub const MAX_WIRE_BYTES:usize=29;
  pub const CARTESIAN_WIRE_BYTES:u64=29;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6acbfd19e3864b66e47b9da5c7c0e8890e7336e8d6c9a5657c4f5eaa5285fe42";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>29{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>29{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<29{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/MyArmSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationMyArmSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Myarm_communicationMyArmSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Myarm_communicationMyArmSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ce10c29be4cfd832d5076fead55817a0ac7ccde002d5bbbd7a2befd95e9752fd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationMyArmSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGetAnglesRequest {
}
impl Default for Ros1Myarm_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Myarm_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_18ea682d1c475321b67435e9f207a14627113ad237a38c759cfe4e1c92fded37";
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
impl Canonical for Ros1Myarm_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
}
impl Default for Ros1Myarm_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
}}}
impl Ros1Myarm_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0c68b293384d21d06e23d4aba10b4a4c2eff4e1a0eaa81903e026fe28da410c7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGetCoordsRequest {
}
impl Default for Ros1Myarm_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Myarm_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_04d9d79ad983cf4631cf65a48cae60d982cd0f234f0f4ed1afa0cc630bd63d95";
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
impl Canonical for Ros1Myarm_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Myarm_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Myarm_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_752564704c08fb2076df8eea4f6f5b89621041372472f0f99b6627fdfda831b7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Myarm_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Myarm_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_adec2e0fbf48862c2245aea4c31c59ea12abdec26b4727b14005b65006c58976";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Myarm_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Myarm_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d703a0ec6f562f1deb444313e36e6a728b4e521d1196d90a63725753d5fe9659";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Myarm_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Myarm_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_eac2f5c006a0fa86df638c52e9cfb7d2c17818ec10e32ad5277af216c6c944f4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Myarm_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Myarm_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_28533ec27c01d85c9a4644959ea8e5c3f2480f5203a9545e96e4242158f08044";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared joint_7; canonical bounds are part of CODEC_IDENTITY.
  pub joint_7:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Myarm_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
joint_7:Default::default(),
speed:Default::default(),
}}}
impl Ros1Myarm_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=29;
  pub const CARTESIAN_WIRE_BYTES:u64=29;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c15c81907fbace209686a98578019d893d86cecf4782427ac2c3858da8cd3e0a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_7.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>29{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.joint_7.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_7=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>29{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<29{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Myarm_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Myarm_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dbdbcb001f0478a4bba5ae9d7320d8dbfa4e36a5c1c130215ff5ab864c73af0f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Myarm_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Myarm_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86cfc6edb6b86f90a21818e824aea50b697e314687472e1e96e9ce5399ef66c6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 myarm_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Myarm_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Myarm_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Myarm_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_52a54371663141d4384dae8969b729baf097a0aa6105f263e78593743e44272d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Myarm_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mycobot_320_communicationMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_67ec61da0c2bf7ee461c21abbfcab14d74b84cb6c9a12430e2a306abdc65690a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mycobot_320_communicationMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1f2ada8331783764a52ed35c81c3ce9a2fdd260e2dffa2990751ce5156c3ad03";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mycobot_320_communicationMycobotGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_efb4ccdb3ce35d67174700f3d3dd7332d0f7a834c5d76b6153e389e031907984";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mycobot_320_communicationMycobotPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bb1d48067f44b7022820a6175afe05d5ba8caed9f750874a4e703389cb0d83f4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mycobot_320_communicationMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bf04b1bd24ab2f56c1835d1abe44b73fe023407c3ebdd3d35341242116a5d259";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationMycobotSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mycobot_320_communicationMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mycobot_320_communicationMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_761235ef96bcc1c687ad8dd3f6d39982d1b0a7a24eaab3bb54feb9780b9e0092";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGetAnglesRequest {
}
impl Default for Ros1Mycobot_320_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Mycobot_320_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fe13e75d9def5e6f7f831297c771cd4d319e1e9862b5026382fed23747b721b8";
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
impl Canonical for Ros1Mycobot_320_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mycobot_320_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mycobot_320_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c0cb03f21add6e566a30129ed7ce3174fa86786859c472a848c6768678adc80a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGetCoordsRequest {
}
impl Default for Ros1Mycobot_320_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Mycobot_320_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_82b2ae7ba4bf2aff8cd6c88ae344b376604624612abdd633c930695c1d1650d6";
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
impl Canonical for Ros1Mycobot_320_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mycobot_320_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mycobot_320_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_821bac75a42a37fa06f51e57c09839f3557487dc8e883310601cb2e00bb6b05d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mycobot_320_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mycobot_320_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_68d072c047f4f026459309282a40c78c5db51571126efb85b7ca9e9f121b5eb5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_320_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_320_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9c1387ec1a93d6674ac6dd735b2811957db57359e9e461dcedc70ae6fccc5544";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mycobot_320_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mycobot_320_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_23a6e68b36341b30ab13026cbddfd01b16d352105bb5faecd7c1ec6ad654ab60";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_320_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_320_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_037f53ebd0750c76a798ff17507759fcf71aa8a55496e5b151b8d977dff2c1c8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mycobot_320_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mycobot_320_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_644a7c29bae881d5f29708b552dcd8fd4102289aa396614590bca33dbda9f7da";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_320_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_320_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_57578e8427fc6a3a89aa595259e03c3498153482afd716dd44c0d09ee9f59f20";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mycobot_320_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mycobot_320_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f398444b45a64155c410a44da67043cee13993d75beabd6bc104b23ccd1d6656";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_320_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_320_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_320_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_320_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3fef1bfe9036381f8ad45bc1490e96b4917df5689ccf3a84d66e5ba5145e56c4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_320_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mycobot_communicationMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mycobot_communicationMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mycobot_communicationMycobotGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mycobot_communicationMycobotPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mycobot_communicationMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationMycobotSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mycobot_communicationMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mycobot_communicationMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGetAnglesRequest {
}
impl Default for Ros1Mycobot_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Mycobot_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54";
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
impl Canonical for Ros1Mycobot_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
}
impl Default for Ros1Mycobot_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros1Mycobot_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGetCoordsRequest {
}
impl Default for Ros1Mycobot_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Mycobot_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876";
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
impl Canonical for Ros1Mycobot_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
}
impl Default for Ros1Mycobot_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros1Mycobot_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>24{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mycobot_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mycobot_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mycobot_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mycobot_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared joint_5; canonical bounds are part of CODEC_IDENTITY.
  pub joint_5:f32,
  /// Declared joint_6; canonical bounds are part of CODEC_IDENTITY.
  pub joint_6:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mycobot_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mycobot_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_5.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_6.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.joint_5.to_le_bytes());
put(out,at,&self.joint_6.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_5=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_6=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared rz; canonical bounds are part of CODEC_IDENTITY.
  pub rz:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mycobot_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mycobot_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rz.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>26{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.rz.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rz=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mycobot_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mycobot_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mycobot_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mycobot_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mycobot_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros1Mypalletizer_communicationMypalAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalAngles {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros1Mypalletizer_communicationMypalCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalCoords {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mypalletizer_communicationMypalGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mypalletizer_communicationMypalPumpStatus {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mypalletizer_communicationMypalSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalSetAngles {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>17{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/MypalSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationMypalSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mypalletizer_communicationMypalSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mypalletizer_communicationMypalSetCoords {
  pub const MAX_WIRE_BYTES:usize=18;
  pub const CARTESIAN_WIRE_BYTES:u64=18;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>18{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationMypalSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>18{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<18{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGetAnglesRequest {
}
impl Default for Ros1Mypalletizer_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Mypalletizer_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8";
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
impl Canonical for Ros1Mypalletizer_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros1Mypalletizer_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros1Mypalletizer_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGetCoordsRequest {
}
impl Default for Ros1Mypalletizer_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Mypalletizer_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8";
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
impl Canonical for Ros1Mypalletizer_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros1Mypalletizer_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros1Mypalletizer_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>16{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Mypalletizer_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Mypalletizer_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mypalletizer_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mypalletizer_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
  /// Declared Pin1; canonical bounds are part of CODEC_IDENTITY.
  pub Pin1:i8,
  /// Declared Pin2; canonical bounds are part of CODEC_IDENTITY.
  pub Pin2:i8,
}
impl Default for Ros1Mypalletizer_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
Pin1:Default::default(),
Pin2:Default::default(),
}}}
impl Ros1Mypalletizer_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
put(out,at,&self.Pin1.to_le_bytes());
put(out,at,&self.Pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.Pin2=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mypalletizer_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mypalletizer_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Mypalletizer_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros1Mypalletizer_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_4.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>17{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.joint_4.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_4=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mypalletizer_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mypalletizer_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:i8,
}
impl Default for Ros1Mypalletizer_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros1Mypalletizer_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=18;
  pub const CARTESIAN_WIRE_BYTES:u64=18;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>18{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.model.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.model=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>18{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<18{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 mypalletizer_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Mypalletizer_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Mypalletizer_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Mypalletizer_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Mypalletizer_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
}
impl Default for Ros1Ultraarm_communicationUltraArmAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmAngles {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8fa11b8169b634ea7c786bef6ee5d8f0d42488b9841f06104ca8fd378c0fa88d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
}
impl Default for Ros1Ultraarm_communicationUltraArmCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmCoords {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4e540a1f7d66110adec19bcf3114c44bbc8574efbfd05e911de8d56cc64f24fa";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmGripperStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Ultraarm_communicationUltraArmGripperStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_60d9ded61184b6f51986afe42b42c786ffb71a13ee6af004c3836c6708b41f9c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmPumpStatus {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Ultraarm_communicationUltraArmPumpStatus {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmPumpStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f765502e0a7c0a4fe18049032fe9d314512de56bc2d52e4b1a8cbaf01fbc68e1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Ultraarm_communicationUltraArmSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
speed:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmSetAngles {
  pub const MAX_WIRE_BYTES:usize=13;
  pub const CARTESIAN_WIRE_BYTES:u64=13;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0536de7f7a45a8d0a19b16b78557ec77c4ce5f99e1141df3c617da847cbb079c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>13{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>13{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/ultraArmSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationUltraArmSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Ultraarm_communicationUltraArmSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
speed:Default::default(),
}}}
impl Ros1Ultraarm_communicationUltraArmSetCoords {
  pub const MAX_WIRE_BYTES:usize=13;
  pub const CARTESIAN_WIRE_BYTES:u64=13;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4e59dd853f51fe18f9401ec6d0fffaa50a00ba7c96a12b8d09c9bfd5ad68f818";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>13{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationUltraArmSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>13{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGetAnglesRequest {
}
impl Default for Ros1Ultraarm_communicationGetAnglesRequest {fn default()->Self {Self {
}}}
impl Ros1Ultraarm_communicationGetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fd6922aabe6ec7f82731d19efb0963753d86a8f412de96f8bc67ea2615a7c860";
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
impl Canonical for Ros1Ultraarm_communicationGetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGetAnglesResponse {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
}
impl Default for Ros1Ultraarm_communicationGetAnglesResponse {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
}}}
impl Ros1Ultraarm_communicationGetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d7048c8ef416139a11a58bd7d5b865f4b6e0b5431c8e80cdd8c4c79de67cc128";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationGetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGetCoordsRequest {
}
impl Default for Ros1Ultraarm_communicationGetCoordsRequest {fn default()->Self {Self {
}}}
impl Ros1Ultraarm_communicationGetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_40a6535b429baf0bbe0bc642ade7a8048e693633388ccd061bf12b6d795bbbd1";
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
impl Canonical for Ros1Ultraarm_communicationGetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGetCoordsResponse {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
}
impl Default for Ros1Ultraarm_communicationGetCoordsResponse {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Ultraarm_communicationGetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9f19d8d2eb7284c4cbe2587ca0438d4680287759553d908340f5053ae4f4f036";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationGetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GripperStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGripperStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Ultraarm_communicationGripperStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Ultraarm_communicationGripperStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c435d39bef04bd7022e048c0cf364d100bf7a92855f04c816d2a22f0e672d586";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationGripperStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/GripperStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationGripperStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Ultraarm_communicationGripperStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Ultraarm_communicationGripperStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fa0c99ff37acea63c5d507174f9725a9341ffaa007e722e21a8e0b93097e56d4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationGripperStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/PumpStatusRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationPumpStatusRequest {
  /// Declared Status; canonical bounds are part of CODEC_IDENTITY.
  pub Status:bool,
}
impl Default for Ros1Ultraarm_communicationPumpStatusRequest {fn default()->Self {Self {
Status:Default::default(),
}}}
impl Ros1Ultraarm_communicationPumpStatusRequest {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_03df5de115870b7679c85e5dc64f996694578f1b7b41387e531a49ee6626e8b2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Status=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationPumpStatusRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/PumpStatusResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationPumpStatusResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Ultraarm_communicationPumpStatusResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Ultraarm_communicationPumpStatusResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_693fd1ad36c534188f959fdb72df4f970432946c5706472c0bf41b27d054a4ee";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationPumpStatusResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/SetAnglesRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationSetAnglesRequest {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Ultraarm_communicationSetAnglesRequest {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
speed:Default::default(),
}}}
impl Ros1Ultraarm_communicationSetAnglesRequest {
  pub const MAX_WIRE_BYTES:usize=13;
  pub const CARTESIAN_WIRE_BYTES:u64=13;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fe8cdfa565c8aa8b77db9743af57d7c1ba14aa9dd31c8d4ca92bb7aec107bb42";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.joint_1.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_2.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.joint_3.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>13{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.joint_1.to_le_bytes());
put(out,at,&self.joint_2.to_le_bytes());
put(out,at,&self.joint_3.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_1=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joint_3=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationSetAnglesRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>13{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/SetAnglesResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationSetAnglesResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Ultraarm_communicationSetAnglesResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Ultraarm_communicationSetAnglesResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f1ecaff9a60a47ef978b0faf2b9b2b9166ae308b3df718b0536536004dc8a3b4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationSetAnglesResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/SetCoordsRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationSetCoordsRequest {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i8,
}
impl Default for Ros1Ultraarm_communicationSetCoordsRequest {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
speed:Default::default(),
}}}
impl Ros1Ultraarm_communicationSetCoordsRequest {
  pub const MAX_WIRE_BYTES:usize=13;
  pub const CARTESIAN_WIRE_BYTES:u64=13;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2a4670c426b6cf566795b0e874018769fb7e852bd104770dff3e735608c3e3d5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.z.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>13{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.z=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationSetCoordsRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>13{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<13{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 ultraarm_communication/SetCoordsResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Ultraarm_communicationSetCoordsResponse {
  /// Declared Flag; canonical bounds are part of CODEC_IDENTITY.
  pub Flag:bool,
}
impl Default for Ros1Ultraarm_communicationSetCoordsResponse {fn default()->Self {Self {
Flag:Default::default(),
}}}
impl Ros1Ultraarm_communicationSetCoordsResponse {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_02aa6c82612c5ffd43e890bb173a59245e00cd91dbc38303dcbdc0fb30683431";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.Flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.Flag=decoded;
Ok(value)}
}
impl Canonical for Ros1Ultraarm_communicationSetCoordsResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
