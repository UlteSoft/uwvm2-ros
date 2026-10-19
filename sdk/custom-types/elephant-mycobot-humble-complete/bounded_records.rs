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
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmAngles {
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
impl Default for Ros2Mecharm_interfacesMsgMecharmAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_37d12e0fc3b7bbb50bdcee0ca624a5c637f9554f3c64d640f538aeda57417331";
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
impl Canonical for Ros2Mecharm_interfacesMsgMecharmAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmCoords {
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
impl Default for Ros2Mecharm_interfacesMsgMecharmCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e1bb96b1908c796e2a89eb4dd2402a6cc02bcc5077c52268bb378a52dd3029ed";
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
impl Canonical for Ros2Mecharm_interfacesMsgMecharmCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmGripperStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mecharm_interfacesMsgMecharmGripperStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2123b61d7b9ec218ee074473c26e5ec82fde818f40796047fc369cb84516e046";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesMsgMecharmGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmPumpStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mecharm_interfacesMsgMecharmPumpStatus {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d44c84c1fdd3f4db92d9975545348bfd727320e63c9fbfde5adf0b81ca04add";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesMsgMecharmPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmSetAngles {
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
impl Default for Ros2Mecharm_interfacesMsgMecharmSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_54b634e4c5d0bc9fdf04a98d31816e24d3a9c5eb497bb38fb16e2de593cf81f5";
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
impl Canonical for Ros2Mecharm_interfacesMsgMecharmSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/msg/MecharmSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesMsgMecharmSetCoords {
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
impl Default for Ros2Mecharm_interfacesMsgMecharmSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros2Mecharm_interfacesMsgMecharmSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_44944ee22cd4254167083cfdcd7377ee2f59dbe27414932c980670a471a6e7c2";
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
impl Canonical for Ros2Mecharm_interfacesMsgMecharmSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGetAngles_Request {
}
impl Default for Ros2Mecharm_interfacesSrvGetAngles_Request {fn default()->Self {Self {
}}}
impl Ros2Mecharm_interfacesSrvGetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_95b1d7786c03c6fec98fec0706088481fe9c7aa48c9f33f4325031d0437d97df";
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
impl Canonical for Ros2Mecharm_interfacesSrvGetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGetAngles_Response {
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
impl Default for Ros2Mecharm_interfacesSrvGetAngles_Response {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvGetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b1a035847ac7a888053ba8de7494abb6d2cdc01f61e4d0a51cb3ef6f479adf61";
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
impl Canonical for Ros2Mecharm_interfacesSrvGetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGetCoords_Request {
}
impl Default for Ros2Mecharm_interfacesSrvGetCoords_Request {fn default()->Self {Self {
}}}
impl Ros2Mecharm_interfacesSrvGetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_353827e7125cfa6c829f1138cc33901099a26cf73a8d045cf806b427994ccef1";
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
impl Canonical for Ros2Mecharm_interfacesSrvGetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGetCoords_Response {
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
impl Default for Ros2Mecharm_interfacesSrvGetCoords_Response {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvGetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f7fcb8c4fdb47c06e93a8f67613490f7ab0ef2af7efb02092874722670b36c75";
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
impl Canonical for Ros2Mecharm_interfacesSrvGetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GripperStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGripperStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mecharm_interfacesSrvGripperStatus_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvGripperStatus_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_044bf47303f5da0d0e17e795b8567b714d55680e8144c8e2e71ff7032859dfbb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvGripperStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/GripperStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvGripperStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mecharm_interfacesSrvGripperStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvGripperStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5aeee3c79bb6ebe4a4cb77f54552e97dcc155c65151a5965761a360d41acbea3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvGripperStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/PumpStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvPumpStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mecharm_interfacesSrvPumpStatus_Request {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvPumpStatus_Request {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f80774682b7b1d447ff2f8dd7f6ac2f917dd4f71ccd3b1685a57b579fa41d6b9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvPumpStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/PumpStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvPumpStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mecharm_interfacesSrvPumpStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvPumpStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_935c0e2aaca3f1e8aad8e4487484292b3d30ccdff180a9aaa50f454f75ecff1b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvPumpStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/SetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvSetAngles_Request {
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
impl Default for Ros2Mecharm_interfacesSrvSetAngles_Request {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvSetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86897875ef9a609098bc1eabba63aec7d960de380db98370e7a67626b2253eeb";
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
impl Canonical for Ros2Mecharm_interfacesSrvSetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/SetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvSetAngles_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mecharm_interfacesSrvSetAngles_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvSetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4782fe6a137dee35bf39d863be0aea118f61dcef092c11b013c44c6f4c129620";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvSetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/SetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvSetCoords_Request {
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
impl Default for Ros2Mecharm_interfacesSrvSetCoords_Request {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvSetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a0daefe14076b73f1a64924ffc15a6b9647abe1b1887377e642f41b1a7920916";
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
impl Canonical for Ros2Mecharm_interfacesSrvSetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mecharm_interfaces/srv/SetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mecharm_interfacesSrvSetCoords_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mecharm_interfacesSrvSetCoords_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mecharm_interfacesSrvSetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_43d02cfe61b6fb08f035e1f8ea78496cdcb488ed4c916f528415dce4ab810b7f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mecharm_interfacesSrvSetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotAngles {
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
impl Default for Ros2Mycobot_interfacesMsgMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_38f099339ed5ee9fe56a5b44c3f0849e2ff7ff721b2a95b5cf49e3b92f6a9d82";
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
impl Canonical for Ros2Mycobot_interfacesMsgMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotCoords {
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
impl Default for Ros2Mycobot_interfacesMsgMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8d9606c561db7680e816f5a19fd148c374f271acee8b261f7d060f370e0399d7";
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
impl Canonical for Ros2Mycobot_interfacesMsgMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotGripperStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mycobot_interfacesMsgMycobotGripperStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4dc3ab413af3da01e26a3289ef9e5ef39488d00483631d701b4d617d9c9ba33d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesMsgMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotPumpStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mycobot_interfacesMsgMycobotPumpStatus {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_860004dc3975ab44b5a1b0a77a9ff01ab58bfc71d7142d842d3bf9c636365e58";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesMsgMycobotPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotSetAngles {
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
impl Default for Ros2Mycobot_interfacesMsgMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_798576c5f0b03e46e5cc5d2edf81bc7042042482ef574643ca136621346d731e";
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
impl Canonical for Ros2Mycobot_interfacesMsgMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/msg/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesMsgMycobotSetCoords {
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
impl Default for Ros2Mycobot_interfacesMsgMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros2Mycobot_interfacesMsgMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_462b7629172037a6959d08984896980c8c07ae5b6364c6d848b172dea23cab49";
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
impl Canonical for Ros2Mycobot_interfacesMsgMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGetAngles_Request {
}
impl Default for Ros2Mycobot_interfacesSrvGetAngles_Request {fn default()->Self {Self {
}}}
impl Ros2Mycobot_interfacesSrvGetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ea433bdb6e3cb77923720a56bec24aaec62c88480248a4ed425370092474d38e";
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
impl Canonical for Ros2Mycobot_interfacesSrvGetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGetAngles_Response {
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
impl Default for Ros2Mycobot_interfacesSrvGetAngles_Response {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvGetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_14d6e2d8a6f7234719fae06d8807c651965387e252160df555ede0a070123b8a";
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
impl Canonical for Ros2Mycobot_interfacesSrvGetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGetCoords_Request {
}
impl Default for Ros2Mycobot_interfacesSrvGetCoords_Request {fn default()->Self {Self {
}}}
impl Ros2Mycobot_interfacesSrvGetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_31ef03eefc143e9d040572e378a5803f44f68bac6d03fbb44ca27f87fb5fb076";
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
impl Canonical for Ros2Mycobot_interfacesSrvGetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGetCoords_Response {
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
impl Default for Ros2Mycobot_interfacesSrvGetCoords_Response {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvGetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c9886b5cc5f3c48116a1c884f75ab4af49be46e2c0476ffc3aa8167ff96f7787";
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
impl Canonical for Ros2Mycobot_interfacesSrvGetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GripperStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGripperStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mycobot_interfacesSrvGripperStatus_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvGripperStatus_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6c5ae66e66f12834b7d623e07a5b3ef2b8127cd9cd83cb36d6793a68d921fcd5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvGripperStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/GripperStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvGripperStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_interfacesSrvGripperStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvGripperStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_530f1e1acaa8cda6b43f8a8c0a38f5376eaebce3e99f7fb194a28143bed2d1ab";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvGripperStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/PumpStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvPumpStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mycobot_interfacesSrvPumpStatus_Request {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvPumpStatus_Request {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_50b7fa285b93dd9e6b3b0eafda0afeb5911ab6e3b79d58f03f1cf1f76e732cd0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvPumpStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/PumpStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvPumpStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_interfacesSrvPumpStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvPumpStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b347313e1d4430f4e78a72fb8c35d53a39d1d391290373f84f1f8b2b44375eb0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvPumpStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/SetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvSetAngles_Request {
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
impl Default for Ros2Mycobot_interfacesSrvSetAngles_Request {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvSetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a06efd8b6af878d55c26fda8a7177a0b6c8b687f7e1b9f2e17e2ecd19c440310";
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
impl Canonical for Ros2Mycobot_interfacesSrvSetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/SetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvSetAngles_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_interfacesSrvSetAngles_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvSetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_af4daae72e9ec28c8848b76b556c9387e65ba9698d8f0f6e5ba17522472ab665";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvSetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/SetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvSetCoords_Request {
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
impl Default for Ros2Mycobot_interfacesSrvSetCoords_Request {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
model:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvSetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=26;
  pub const CARTESIAN_WIRE_BYTES:u64=26;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_76a98d5db6f98255046f13855dc7468049894805d83e3d3a7fbdaace40292213";
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
impl Canonical for Ros2Mycobot_interfacesSrvSetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>26{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<26{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_interfaces/srv/SetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_interfacesSrvSetCoords_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_interfacesSrvSetCoords_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_interfacesSrvSetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_75e3ae0e6db60a275b08e450c420969aa1ee05d0f34864e8fb6535d2d13613b9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_interfacesSrvSetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/msg/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesMsgMycobotAngles {
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
impl Default for Ros2Mycobot_pro450_interfacesMsgMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesMsgMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2351398be27155ac63b8a3d15ffd0d3088dd7cf284182a9b494791b7780aa6da";
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
impl Canonical for Ros2Mycobot_pro450_interfacesMsgMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/msg/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesMsgMycobotCoords {
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
impl Default for Ros2Mycobot_pro450_interfacesMsgMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesMsgMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b8513545b0975757c6b7f756a5cedfe8aa0c5d28d0e573adfde001b452e923de";
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
impl Canonical for Ros2Mycobot_pro450_interfacesMsgMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/msg/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1d097932f2e6b9b4e73ff838b5223b585b230e4edc5c229b310eb00404f6e784";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/msg/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles {
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
impl Default for Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_23d208d7fa8f7b7488be7c7db487698db4b227ef65e99c55d996901d27c3e591";
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
impl Canonical for Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/msg/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords {
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
}
impl Default for Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_60f40a2f6a577e5785fd69e0cbe9128b301aeaec6b70fd678c71e7f9a5426cea";
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
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetAngles_Request {
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGetAngles_Request {fn default()->Self {Self {
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d2d8197675b9dadd50ae1a22b165a84e2e76b293b28cc70bec211225c60626c4";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetAngles_Response {
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
impl Default for Ros2Mycobot_pro450_interfacesSrvGetAngles_Response {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_13056bbe3bdef4feb8488783e1c5cf725d1fb94006f36f2221f61081084227f8";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetCoords_Request {
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGetCoords_Request {fn default()->Self {Self {
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bda408ece90e671ce58ae428f79f01c1429c3b3aa3c1ef2efe9dc045faa9e3c2";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetCoords_Response {
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
impl Default for Ros2Mycobot_pro450_interfacesSrvGetCoords_Response {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CARTESIAN_WIRE_BYTES:u64=24;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_01e8a3ef4c048100af32d92cd027769e32eacd82dba36dc3bde30c70cb44afdf";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>24{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request {
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request {fn default()->Self {Self {
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_17cb5e3826409b661c2ad4bad8fde2b4c826dde95d8600e1a17bcf4908eca125";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response {
  /// Declared gripper_angle; canonical bounds are part of CODEC_IDENTITY.
  pub gripper_angle:i8,
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response {fn default()->Self {Self {
gripper_angle:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6b210e18d0696735196fb2e166b7168165dba6f03b97759a437fc8dc08ab2676";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.gripper_angle.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gripper_angle=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_edc3352d37effe3923c55a024d6acac85a3cb49350974ea35934efa41555ccac";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_65166c2943fd319555cc1a2dcdaf4475b1e7779960907fe5ab6d1b5dc5536fc2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/SetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvSetAngles_Request {
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
impl Default for Ros2Mycobot_pro450_interfacesSrvSetAngles_Request {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
joint_5:Default::default(),
joint_6:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvSetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ec2659577304938ca4f78c1b4add50190766d8d1b551397668ef7ad03e2ccb09";
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
impl Canonical for Ros2Mycobot_pro450_interfacesSrvSetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/SetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvSetAngles_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_pro450_interfacesSrvSetAngles_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvSetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_28dc59c06324913c1ccbe286ca05b9f47585653a8f086fc5d99a01f6e9ee08b4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvSetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/SetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvSetCoords_Request {
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
}
impl Default for Ros2Mycobot_pro450_interfacesSrvSetCoords_Request {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
ry:Default::default(),
rz:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvSetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e0c550200472f77aa582a46a5149c05c95648615a306f254599f820992890c81";
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
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvSetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mycobot_pro450_interfaces/srv/SetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mycobot_pro450_interfacesSrvSetCoords_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mycobot_pro450_interfacesSrvSetCoords_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mycobot_pro450_interfacesSrvSetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9a5ee934233519095e1b28568490f8d958c319e2544976fae0b7de53a54a3f51";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mycobot_pro450_interfacesSrvSetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros2Mypalletizer_interfacesMsgMypalAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalAngles {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0a2dc324515448310b0f026fb26c6c5c6da452e5e7067096e315abe3ab86a35d";
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
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros2Mypalletizer_interfacesMsgMypalCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalCoords {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_839491548a07528380403fa31c89889ce82f28b2f82e6fd9474835659714b4f7";
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
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalGripperStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mypalletizer_interfacesMsgMypalGripperStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f6e4ad6193272d6401bbbfcc20c0f38b6a147a81d9de1ad956d7c154f29daeb0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalPumpStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalPumpStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mypalletizer_interfacesMsgMypalPumpStatus {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalPumpStatus {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_008dfd13cad42029330909fb7dc68392ba7f6678ad1baa14a41aad1e759f523c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalPumpStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalSetAngles {
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
impl Default for Ros2Mypalletizer_interfacesMsgMypalSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalSetAngles {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b9fdd6ac956fdeb4934ff41982e616f7c0c45b11169b6d60ba74fbcc61458bb5";
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
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/msg/MypalSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesMsgMypalSetCoords {
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
}
impl Default for Ros2Mypalletizer_interfacesMsgMypalSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesMsgMypalSetCoords {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0c1b65f543cdd239a5a56a7839ac607c5310cd4145ef6a6da5639a9af8ba4c79";
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
if n>17{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
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
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesMsgMypalSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGetAngles_Request {
}
impl Default for Ros2Mypalletizer_interfacesSrvGetAngles_Request {fn default()->Self {Self {
}}}
impl Ros2Mypalletizer_interfacesSrvGetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c8124a1063d47b24dae2e279f51d4e48ebf9936a5e305b81b53387a4be344f78";
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
impl Canonical for Ros2Mypalletizer_interfacesSrvGetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGetAngles_Response {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros2Mypalletizer_interfacesSrvGetAngles_Response {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvGetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_33812a320c14891a5bc76a86ccbb0a5507cfbcc3706e01bc304263edabc6f75d";
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
impl Canonical for Ros2Mypalletizer_interfacesSrvGetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGetCoords_Request {
}
impl Default for Ros2Mypalletizer_interfacesSrvGetCoords_Request {fn default()->Self {Self {
}}}
impl Ros2Mypalletizer_interfacesSrvGetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b3db3a0e7d2aed097027798a897caf833f9cc1b8bb027f98c0db016968fa4440";
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
impl Canonical for Ros2Mypalletizer_interfacesSrvGetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGetCoords_Response {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros2Mypalletizer_interfacesSrvGetCoords_Response {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvGetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9d237cfa03c4b3c17d99fe0a9c08d1cafa3e8d0e404191e2169eea8683c54c84";
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
impl Canonical for Ros2Mypalletizer_interfacesSrvGetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GripperStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGripperStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Mypalletizer_interfacesSrvGripperStatus_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvGripperStatus_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b6d4c5991599ec4ac56e5f65dacd5a1ab89f073e9f07dc2b03692f4289fdcc9d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvGripperStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/GripperStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvGripperStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mypalletizer_interfacesSrvGripperStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvGripperStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_bd0c427dc564ff7201019c6d33c7d0c743a65d263c67eb420b5e653d48e4daf4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvGripperStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/PumpStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvPumpStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
  /// Declared pin1; canonical bounds are part of CODEC_IDENTITY.
  pub pin1:i8,
  /// Declared pin2; canonical bounds are part of CODEC_IDENTITY.
  pub pin2:i8,
}
impl Default for Ros2Mypalletizer_interfacesSrvPumpStatus_Request {fn default()->Self {Self {
status:Default::default(),
pin1:Default::default(),
pin2:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvPumpStatus_Request {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6b14af554cb5c65c9411b2c6a76d49cb7cbe06dedcd26d47804d00d12ccf2e2f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
put(out,at,&self.pin1.to_le_bytes());
put(out,at,&self.pin2.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pin2=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvPumpStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/PumpStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvPumpStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mypalletizer_interfacesSrvPumpStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvPumpStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_187a703ce648bc82a276961d82ed8abd5365d8db7af3bb211e590620e1bb2b45";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvPumpStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/SetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvSetAngles_Request {
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
impl Default for Ros2Mypalletizer_interfacesSrvSetAngles_Request {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvSetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3997bb19c183b59fbca1675695ea5ee548a247a34f9682dae3fb68fdf1a155ec";
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
impl Canonical for Ros2Mypalletizer_interfacesSrvSetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/SetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvSetAngles_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mypalletizer_interfacesSrvSetAngles_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvSetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a7952f549b35aa140adac4f18bbe8538c0f6230aa884459091cd74720ab757c9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvSetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/SetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvSetCoords_Request {
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
}
impl Default for Ros2Mypalletizer_interfacesSrvSetCoords_Request {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvSetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=17;
  pub const CARTESIAN_WIRE_BYTES:u64=17;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c72f4f1cb71f1ad1076bcc89499abbcfc5eab173fadd19c9a266baf1c32d1541";
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
if n>17{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
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
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvSetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>17{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 mypalletizer_interfaces/srv/SetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Mypalletizer_interfacesSrvSetCoords_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Mypalletizer_interfacesSrvSetCoords_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Mypalletizer_interfacesSrvSetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0cedaf9087ba0ae3726b9dd0d304665a226fc6df2ee068f6ac2b1c12ce88cb5b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Mypalletizer_interfacesSrvSetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/msg/MycobotAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesMsgMycobotAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros2Ultraarm_p1_interfacesMsgMycobotAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesMsgMycobotAngles {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_75965ba712c12f465c2424b540359dbc4c88e2e85aa772698ee040db7c32a945";
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
impl Canonical for Ros2Ultraarm_p1_interfacesMsgMycobotAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/msg/MycobotCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesMsgMycobotCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros2Ultraarm_p1_interfacesMsgMycobotCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesMsgMycobotCoords {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c79774c9af7fb4c150cb18c2510df52bbc7bad3c05b604abf90e19087aa05874";
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
impl Canonical for Ros2Ultraarm_p1_interfacesMsgMycobotCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/msg/MycobotGripperStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1efb83ff22c5eb211fe92355cb5358cb991a373693d33180362e43a2ef56d22f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/msg/MycobotSetAngles.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i32,
}
impl Default for Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles {
  pub const MAX_WIRE_BYTES:usize=20;
  pub const CARTESIAN_WIRE_BYTES:u64=20;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cb53aab11e200533386b764a423f86bd7a24c65105236945c6b27ba7a9c3221c";
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
n=add_size(n,4)?;
if n>20{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/msg/MycobotSetCoords.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i32,
}
impl Default for Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords {
  pub const MAX_WIRE_BYTES:usize=20;
  pub const CARTESIAN_WIRE_BYTES:u64=20;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fca21ab6eed31f03f0f7c9c6010d893f709f1eebb1bf07c1911452da1c50bbee";
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
n=add_size(n,4)?;
if n>20{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
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
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetAngles_Request {
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetAngles_Request {fn default()->Self {Self {
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b2ff68f6919df813ddf49ad9337c2345e3c4e70242ad81f6440d5cd1cd612ff9";
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
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetAngles_Response {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetAngles_Response {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cc8b4259c50ed82d6f3b3bbb9470ac089b127b70ace12457cb41fccb7ae67cbc";
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
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetCoords_Request {
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetCoords_Request {fn default()->Self {Self {
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0ed9a0f7124cb00e62b71aecaedb60ea8832a94aaf4dc8f1de943d91dbf74e01";
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
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetCoords_Response {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetCoords_Response {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_47ef794ffd2a3a4125ce3c9cbbd6b0d85fbb42cbfc888dd8fec39020cd454565";
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
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetGripperValue_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request {
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request {fn default()->Self {Self {
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dad3d4af78250b2b8e44424ee1bb532b1e24bd9b867417fa428dec761fcf4be9";
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
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GetGripperValue_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response {
  /// Declared gripper_angle; canonical bounds are part of CODEC_IDENTITY.
  pub gripper_angle:i8,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response {fn default()->Self {Self {
gripper_angle:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3b3830d2ae5b4327771923acab70739d05dd3f9c8e2b3b060f6a3136a9e6c647";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.gripper_angle.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gripper_angle=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GripperStatus_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:bool,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request {fn default()->Self {Self {
status:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cdaff7754e24b6910102060d2af379d4bcc9c3a647604ccdf2dc8b4a50d854b8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.status)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.status=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/GripperStatus_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_55826c68885c7382e5a9e8a0ec35b312af3b2e974236d80518564c3fb8b9fa48";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/SetAngles_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvSetAngles_Request {
  /// Declared joint_1; canonical bounds are part of CODEC_IDENTITY.
  pub joint_1:f32,
  /// Declared joint_2; canonical bounds are part of CODEC_IDENTITY.
  pub joint_2:f32,
  /// Declared joint_3; canonical bounds are part of CODEC_IDENTITY.
  pub joint_3:f32,
  /// Declared joint_4; canonical bounds are part of CODEC_IDENTITY.
  pub joint_4:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i32,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvSetAngles_Request {fn default()->Self {Self {
joint_1:Default::default(),
joint_2:Default::default(),
joint_3:Default::default(),
joint_4:Default::default(),
speed:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvSetAngles_Request {
  pub const MAX_WIRE_BYTES:usize=20;
  pub const CARTESIAN_WIRE_BYTES:u64=20;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4184d40dd5cef87f13e913d349e37b0661905c5c9f0d8c5f9cc965ea272be025";
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
n=add_size(n,4)?;
if n>20{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvSetAngles_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/SetAngles_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvSetAngles_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvSetAngles_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvSetAngles_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_de517cd68c164d4214aed8ad92f7bb9ee49f24c7e43d96c18e193095b0b23e1a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvSetAngles_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/SetCoords_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvSetCoords_Request {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i32,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvSetCoords_Request {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
rx:Default::default(),
speed:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvSetCoords_Request {
  pub const MAX_WIRE_BYTES:usize=20;
  pub const CARTESIAN_WIRE_BYTES:u64=20;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_df9914ced1ef5f92f6102595de772ed076be380360b8fd5732cf7f0d4c0ec3e3";
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
n=add_size(n,4)?;
if n>20{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.z.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
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
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvSetCoords_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>20{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 ultraarm_p1_interfaces/srv/SetCoords_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Ultraarm_p1_interfacesSrvSetCoords_Response {
  /// Declared flag; canonical bounds are part of CODEC_IDENTITY.
  pub flag:bool,
}
impl Default for Ros2Ultraarm_p1_interfacesSrvSetCoords_Response {fn default()->Self {Self {
flag:Default::default(),
}}}
impl Ros2Ultraarm_p1_interfacesSrvSetCoords_Response {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c66cfcc79da5bbc9a19b077761cfd3e89b3dae56ae604205de6316aa37935d7f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.flag)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.flag=decoded;
Ok(value)}
}
impl Canonical for Ros2Ultraarm_p1_interfacesSrvSetCoords_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
