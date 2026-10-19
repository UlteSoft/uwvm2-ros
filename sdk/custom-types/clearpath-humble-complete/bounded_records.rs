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
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/PumaFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgPumaFeedback {
  /// Declared device_number; canonical bounds are part of CODEC_IDENTITY.
  pub device_number:u8,
  /// Declared device_name; canonical bounds are part of CODEC_IDENTITY.
  pub device_name:String,
  /// Declared duty_cycle; canonical bounds are part of CODEC_IDENTITY.
  pub duty_cycle:f32,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f32,
  /// Declared travel; canonical bounds are part of CODEC_IDENTITY.
  pub travel:f64,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:f64,
  /// Declared setpoint; canonical bounds are part of CODEC_IDENTITY.
  pub setpoint:f64,
}
impl Default for Ros2Clearpath_motor_msgsMsgPumaFeedback {fn default()->Self {Self {
device_number:Default::default(),
device_name:Default::default(),
duty_cycle:Default::default(),
current:Default::default(),
travel:Default::default(),
speed:Default::default(),
setpoint:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgPumaFeedback {
  pub const MAX_WIRE_BYTES:usize=293;
  pub const CARTESIAN_WIRE_BYTES:u64=293;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c020162b07a55ac5f41df6021c0d3053d7302863f4e544a738b6cc5bc4dc9659";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.device_name.len()>256||self.device_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.device_name.len())?;
if !self.duty_cycle.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.travel.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.speed.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if !self.setpoint.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if n>293{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.device_number.to_le_bytes());
put(out,at,&(self.device_name.len() as u32).to_le_bytes());put(out,at,self.device_name.as_bytes());
put(out,at,&self.duty_cycle.to_le_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.travel.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.setpoint.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.device_number=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.device_name=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.duty_cycle=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.travel=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.speed=decoded;
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.setpoint=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgPumaFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>293{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<37{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/PumaMultiFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgPumaMultiFeedback {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers_feedback; canonical bounds are part of CODEC_IDENTITY.
  pub drivers_feedback:Vec<Ros2Clearpath_motor_msgsMsgPumaFeedback>,
}
impl Default for Ros2Clearpath_motor_msgsMsgPumaMultiFeedback {fn default()->Self {Self {
header:Default::default(),
drivers_feedback:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgPumaMultiFeedback {
  pub const MAX_WIRE_BYTES:usize=1316;
  pub const CARTESIAN_WIRE_BYTES:u64=1316;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_638309564580ad23d585799dcd46156e1e4261ebf9febfbb91754c250907c947";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.drivers_feedback.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.drivers_feedback {
n=add_size(n,element.wire_size()?)?;
}
if n>1316{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.drivers_feedback.len() as u32).to_le_bytes());
for element in &self.drivers_feedback {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.drivers_feedback=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Clearpath_motor_msgsMsgPumaFeedback::read_fields(bytes,at)?;
value.drivers_feedback.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgPumaMultiFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1316{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/PumaStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgPumaStatus {
  /// Declared device_number; canonical bounds are part of CODEC_IDENTITY.
  pub device_number:u8,
  /// Declared device_name; canonical bounds are part of CODEC_IDENTITY.
  pub device_name:String,
  /// Declared bus_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub bus_voltage:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:f32,
  /// Declared output_voltage; canonical bounds are part of CODEC_IDENTITY.
  pub output_voltage:f32,
  /// Declared analog_input; canonical bounds are part of CODEC_IDENTITY.
  pub analog_input:f32,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared fault; canonical bounds are part of CODEC_IDENTITY.
  pub fault:u8,
}
impl Default for Ros2Clearpath_motor_msgsMsgPumaStatus {fn default()->Self {Self {
device_number:Default::default(),
device_name:Default::default(),
bus_voltage:Default::default(),
temperature:Default::default(),
output_voltage:Default::default(),
analog_input:Default::default(),
mode:Default::default(),
fault:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgPumaStatus {
  pub const MAX_WIRE_BYTES:usize=279;
  pub const CARTESIAN_WIRE_BYTES:u64=279;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a027229db2c8500f707d97064548b27e77e24ae4840ad9722d2dc74726099da8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.device_name.len()>256||self.device_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.device_name.len())?;
if !self.bus_voltage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.output_voltage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.analog_input.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>279{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.device_number.to_le_bytes());
put(out,at,&(self.device_name.len() as u32).to_le_bytes());put(out,at,self.device_name.as_bytes());
put(out,at,&self.bus_voltage.to_le_bytes());
put(out,at,&self.temperature.to_le_bytes());
put(out,at,&self.output_voltage.to_le_bytes());
put(out,at,&self.analog_input.to_le_bytes());
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.fault.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.device_number=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.device_name=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.bus_voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.output_voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.analog_input=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.fault=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgPumaStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>279{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<23{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/PumaMultiStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgPumaMultiStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:Vec<Ros2Clearpath_motor_msgsMsgPumaStatus>,
}
impl Default for Ros2Clearpath_motor_msgsMsgPumaMultiStatus {fn default()->Self {Self {
header:Default::default(),
drivers:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgPumaMultiStatus {
  pub const MAX_WIRE_BYTES:usize=1260;
  pub const CARTESIAN_WIRE_BYTES:u64=1260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_30a0210fea4fffe07371c898c365ef9696501a7541935dd80770671a7163c8f2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.drivers.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.drivers {
n=add_size(n,element.wire_size()?)?;
}
if n>1260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.drivers.len() as u32).to_le_bytes());
for element in &self.drivers {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.drivers=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Clearpath_motor_msgsMsgPumaStatus::read_fields(bytes,at)?;
value.drivers.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgPumaMultiStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Drive.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgDrive {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:i8,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:[f32;2],
}
impl Default for Ros2Clearpath_platform_msgsMsgDrive {fn default()->Self {Self {
mode:Default::default(),
drivers:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Clearpath_platform_msgsMsgDrive {
  pub const MAX_WIRE_BYTES:usize=9;
  pub const CARTESIAN_WIRE_BYTES:u64=9;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
for element in &self.drivers {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>9{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
for element in &self.drivers {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.drivers[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgDrive {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>9{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<9{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/DriveFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgDriveFeedback {
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f32,
  /// Declared duty_cycle; canonical bounds are part of CODEC_IDENTITY.
  pub duty_cycle:f32,
  /// Declared bridge_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub bridge_temperature:f32,
  /// Declared motor_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub motor_temperature:f32,
  /// Declared measured_velocity; canonical bounds are part of CODEC_IDENTITY.
  pub measured_velocity:f32,
  /// Declared measured_travel; canonical bounds are part of CODEC_IDENTITY.
  pub measured_travel:f32,
  /// Declared driver_fault; canonical bounds are part of CODEC_IDENTITY.
  pub driver_fault:bool,
}
impl Default for Ros2Clearpath_platform_msgsMsgDriveFeedback {fn default()->Self {Self {
current:Default::default(),
duty_cycle:Default::default(),
bridge_temperature:Default::default(),
motor_temperature:Default::default(),
measured_velocity:Default::default(),
measured_travel:Default::default(),
driver_fault:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgDriveFeedback {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.duty_cycle.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.bridge_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.motor_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.measured_velocity.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.measured_travel.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.duty_cycle.to_le_bytes());
put(out,at,&self.bridge_temperature.to_le_bytes());
put(out,at,&self.motor_temperature.to_le_bytes());
put(out,at,&self.measured_velocity.to_le_bytes());
put(out,at,&self.measured_travel.to_le_bytes());
put(out,at,&[u8::from(self.driver_fault)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.duty_cycle=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.bridge_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.motor_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.measured_velocity=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.measured_travel=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.driver_fault=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgDriveFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Fans.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgFans {
  /// Declared fans; canonical bounds are part of CODEC_IDENTITY.
  pub fans:Vec<u8>,
}
impl Default for Ros2Clearpath_platform_msgsMsgFans {fn default()->Self {Self {
fans:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgFans {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_addc15a9b6db4f7a0eba3545f02ece0800e48796f71e387b4d249af861f95317";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.fans.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.fans {
n=add_size(n,1)?;
}
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.fans.len() as u32).to_le_bytes());
for element in &self.fans {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.fans=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.fans.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgFans {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgFeedback {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:[Ros2Clearpath_platform_msgsMsgDriveFeedback;2],
  /// Declared commanded_mode; canonical bounds are part of CODEC_IDENTITY.
  pub commanded_mode:i8,
  /// Declared actual_mode; canonical bounds are part of CODEC_IDENTITY.
  pub actual_mode:i8,
}
impl Default for Ros2Clearpath_platform_msgsMsgFeedback {fn default()->Self {Self {
header:Default::default(),
drivers:core::array::from_fn(|_|Default::default()),
commanded_mode:Default::default(),
actual_mode:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgFeedback {
  pub const MAX_WIRE_BYTES:usize=192;
  pub const CARTESIAN_WIRE_BYTES:u64=192;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
for element in &self.drivers {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>192{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
for element in &self.drivers {
element.write_fields(out,at);
}
put(out,at,&self.commanded_mode.to_le_bytes());
put(out,at,&self.actual_mode.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Clearpath_platform_msgsMsgDriveFeedback::read_fields(bytes,at)?;
value.drivers[i]=decoded;
}}
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.commanded_mode=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.actual_mode=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>192{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<64{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/RGB.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgRGB {
  /// Declared red; canonical bounds are part of CODEC_IDENTITY.
  pub red:u8,
  /// Declared green; canonical bounds are part of CODEC_IDENTITY.
  pub green:u8,
  /// Declared blue; canonical bounds are part of CODEC_IDENTITY.
  pub blue:u8,
}
impl Default for Ros2Clearpath_platform_msgsMsgRGB {fn default()->Self {Self {
red:Default::default(),
green:Default::default(),
blue:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgRGB {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.red.to_le_bytes());
put(out,at,&self.green.to_le_bytes());
put(out,at,&self.blue.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.red=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.green=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.blue=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgRGB {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Lights.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgLights {
  /// Declared lights; canonical bounds are part of CODEC_IDENTITY.
  pub lights:Vec<Ros2Clearpath_platform_msgsMsgRGB>,
}
impl Default for Ros2Clearpath_platform_msgsMsgLights {fn default()->Self {Self {
lights:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgLights {
  pub const MAX_WIRE_BYTES:usize=10;
  pub const CARTESIAN_WIRE_BYTES:u64=10;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_14da254234c9c897e39bf4ee0db8faad0798d23fe7017195f35f88fb6e0b3332";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.lights.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.lights {
n=add_size(n,element.wire_size()?)?;
}
if n>10{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.lights.len() as u32).to_le_bytes());
for element in &self.lights {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.lights=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Clearpath_platform_msgsMsgRGB::read_fields(bytes,at)?;
value.lights.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgLights {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>10{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Power.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgPower {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared shore_power_connected; canonical bounds are part of CODEC_IDENTITY.
  pub shore_power_connected:i8,
  /// Declared battery_connected; canonical bounds are part of CODEC_IDENTITY.
  pub battery_connected:i8,
  /// Declared power_12v_user_nominal; canonical bounds are part of CODEC_IDENTITY.
  pub power_12v_user_nominal:i8,
  /// Declared charger_connected; canonical bounds are part of CODEC_IDENTITY.
  pub charger_connected:i8,
  /// Declared charging_complete; canonical bounds are part of CODEC_IDENTITY.
  pub charging_complete:i8,
  /// Declared measured_voltages; canonical bounds are part of CODEC_IDENTITY.
  pub measured_voltages:Vec<f32>,
  /// Declared measured_currents; canonical bounds are part of CODEC_IDENTITY.
  pub measured_currents:Vec<f32>,
}
impl Default for Ros2Clearpath_platform_msgsMsgPower {fn default()->Self {Self {
header:Default::default(),
shore_power_connected:Default::default(),
battery_connected:Default::default(),
power_12v_user_nominal:Default::default(),
charger_connected:Default::default(),
charging_complete:Default::default(),
measured_voltages:Default::default(),
measured_currents:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgPower {
  pub const MAX_WIRE_BYTES:usize=169;
  pub const CARTESIAN_WIRE_BYTES:u64=169;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e97ba89dea149ff43d0134038985ea45d2f072af592ea128aa5502a053478d22";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if self.measured_voltages.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.measured_voltages {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.measured_currents.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.measured_currents {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>169{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.shore_power_connected.to_le_bytes());
put(out,at,&self.battery_connected.to_le_bytes());
put(out,at,&self.power_12v_user_nominal.to_le_bytes());
put(out,at,&self.charger_connected.to_le_bytes());
put(out,at,&self.charging_complete.to_le_bytes());
put(out,at,&(self.measured_voltages.len() as u32).to_le_bytes());
for element in &self.measured_voltages {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.measured_currents.len() as u32).to_le_bytes());
for element in &self.measured_currents {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.shore_power_connected=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.battery_connected=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.power_12v_user_nominal=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.charger_connected=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.charging_complete=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.measured_voltages=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.measured_voltages.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.measured_currents=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.measured_currents.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgPower {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>169{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Status.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared hardware_id; canonical bounds are part of CODEC_IDENTITY.
  pub hardware_id:String,
  /// Declared firmware_version; canonical bounds are part of CODEC_IDENTITY.
  pub firmware_version:String,
  /// Declared mcu_uptime; canonical bounds are part of CODEC_IDENTITY.
  pub mcu_uptime:Ros2Builtin_interfacesMsgDuration,
  /// Declared connection_uptime; canonical bounds are part of CODEC_IDENTITY.
  pub connection_uptime:Ros2Builtin_interfacesMsgDuration,
  /// Declared pcb_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub pcb_temperature:f32,
  /// Declared mcu_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub mcu_temperature:f32,
}
impl Default for Ros2Clearpath_platform_msgsMsgStatus {fn default()->Self {Self {
header:Default::default(),
hardware_id:Default::default(),
firmware_version:Default::default(),
mcu_uptime:Default::default(),
connection_uptime:Default::default(),
pcb_temperature:Default::default(),
mcu_temperature:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgStatus {
  pub const MAX_WIRE_BYTES:usize=684;
  pub const CARTESIAN_WIRE_BYTES:u64=684;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_331afb70728e70e0391da1b2a0f2b97fdc8742a363a2b007606121ad778dd5c1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.hardware_id.len()>256||self.hardware_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.hardware_id.len())?;
if self.firmware_version.len()>256||self.firmware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware_version.len())?;
n=add_size(n,self.mcu_uptime.wire_size()?)?;
n=add_size(n,self.connection_uptime.wire_size()?)?;
if !self.pcb_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.mcu_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>684{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.hardware_id.len() as u32).to_le_bytes());put(out,at,self.hardware_id.as_bytes());
put(out,at,&(self.firmware_version.len() as u32).to_le_bytes());put(out,at,self.firmware_version.as_bytes());
self.mcu_uptime.write_fields(out,at);
self.connection_uptime.write_fields(out,at);
put(out,at,&self.pcb_temperature.to_le_bytes());
put(out,at,&self.mcu_temperature.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.hardware_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.firmware_version=decoded;
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.mcu_uptime=decoded;
let decoded=Ros2Builtin_interfacesMsgDuration::read_fields(bytes,at)?;
value.connection_uptime=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pcb_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mcu_temperature=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>684{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/StopStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgStopStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared stop_power_status; canonical bounds are part of CODEC_IDENTITY.
  pub stop_power_status:bool,
  /// Declared external_stop_present; canonical bounds are part of CODEC_IDENTITY.
  pub external_stop_present:bool,
  /// Declared needs_reset; canonical bounds are part of CODEC_IDENTITY.
  pub needs_reset:bool,
}
impl Default for Ros2Clearpath_platform_msgsMsgStopStatus {fn default()->Self {Self {
header:Default::default(),
stop_power_status:Default::default(),
external_stop_present:Default::default(),
needs_reset:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgStopStatus {
  pub const MAX_WIRE_BYTES:usize=143;
  pub const CARTESIAN_WIRE_BYTES:u64=143;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>143{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&[u8::from(self.stop_power_status)]);
put(out,at,&[u8::from(self.external_stop_present)]);
put(out,at,&[u8::from(self.needs_reset)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.stop_power_status=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.external_stop_present=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.needs_reset=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgStopStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>143{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<15{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsSrvConfigureMcu_Request {
  /// Declared domain_id; canonical bounds are part of CODEC_IDENTITY.
  pub domain_id:u8,
  /// Declared robot_namespace; canonical bounds are part of CODEC_IDENTITY.
  pub robot_namespace:String,
}
impl Default for Ros2Clearpath_platform_msgsSrvConfigureMcu_Request {fn default()->Self {Self {
domain_id:Default::default(),
robot_namespace:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsSrvConfigureMcu_Request {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.robot_namespace.len()>256||self.robot_namespace.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.robot_namespace.len())?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.domain_id.to_le_bytes());
put(out,at,&(self.robot_namespace.len() as u32).to_le_bytes());put(out,at,self.robot_namespace.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.domain_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.robot_namespace=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsSrvConfigureMcu_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsSrvConfigureMcu_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Clearpath_platform_msgsSrvConfigureMcu_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsSrvConfigureMcu_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad";
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
impl Canonical for Ros2Clearpath_platform_msgsSrvConfigureMcu_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Clearpath_motor_msgsMsgPumaStatus {pub const MODE_VOLTAGE:u8=0;
pub const MODE_CURRENT:u8=1;
pub const MODE_SPEED:u8=2;
pub const MODE_POSITION:u8=3;
pub const MODE_VCOMP:u8=4;
pub const FAULT_CURRENT:u8=1;
pub const FAULT_TEMPERATURE:u8=2;
pub const FAULT_BUS_VOLTAGE:u8=4;
pub const FAULT_BRIDGE_DRIVER:u8=8;}

impl Ros2Clearpath_platform_msgsMsgDrive {pub const MODE_VELOCITY:i8=0;
pub const MODE_PWM:i8=1;
pub const MODE_NONE:i8=-1;
pub const LEFT:i8=0;
pub const RIGHT:i8=1;}

impl Ros2Clearpath_platform_msgsMsgFans {pub const R100_EQUIPMENT_BAY_INTAKE:u8=0;
pub const R100_EQUIPMENT_BAY_EXHAUST:u8=1;
pub const R100_CHARGER_BAY_INTAKE:u8=2;
pub const R100_CHARGER_BAY_EXHAUST:u8=3;
pub const R100_USER_BAY_INTAKE:u8=4;
pub const R100_USER_BAY_EXHAUST:u8=5;
pub const FAN_OFF:u8=0;
pub const FAN_ON_HIGH:u8=1;
pub const FAN_ON_LOW:u8=2;}

impl Ros2Clearpath_platform_msgsMsgLights {pub const D100_LIGHTS_REAR_LEFT:u8=0;
pub const D100_LIGHTS_FRONT_LEFT:u8=1;
pub const D100_LIGHTS_FRONT_RIGHT:u8=2;
pub const D100_LIGHTS_REAR_RIGHT:u8=3;
pub const D150_LIGHTS_REAR_LEFT:u8=0;
pub const D150_LIGHTS_FRONT_LEFT:u8=1;
pub const D150_LIGHTS_FRONT_RIGHT:u8=2;
pub const D150_LIGHTS_REAR_RIGHT:u8=3;
pub const R100_LIGHTS_FRONT_PORT_UPPER:u8=0;
pub const R100_LIGHTS_FRONT_PORT_LOWER:u8=1;
pub const R100_LIGHTS_FRONT_STARBOARD_UPPER:u8=2;
pub const R100_LIGHTS_FRONT_STARBOARD_LOWER:u8=3;
pub const R100_LIGHTS_REAR_PORT_UPPER:u8=4;
pub const R100_LIGHTS_REAR_PORT_LOWER:u8=5;
pub const R100_LIGHTS_REAR_STARBOARD_UPPER:u8=6;
pub const R100_LIGHTS_REAR_STARBOARD_LOWER:u8=7;
pub const W200_LIGHTS_FRONT_LEFT:u8=0;
pub const W200_LIGHTS_FRONT_RIGHT:u8=1;
pub const W200_LIGHTS_REAR_LEFT:u8=2;
pub const W200_LIGHTS_REAR_RIGHT:u8=3;}

impl Ros2Clearpath_platform_msgsMsgPower {pub const NOT_APPLICABLE:i8=-1;
pub const J100_MEASURED_BATTERY:u8=0;
pub const J100_MEASURED_5V:u8=1;
pub const J100_MEASURED_12V:u8=2;
pub const D100_MEASURED_BATTERY:u8=0;
pub const D100_MEASURED_5V:u8=1;
pub const D100_MEASURED_12V:u8=2;
pub const D150_MEASURED_BATTERY:u8=0;
pub const D150_MEASURED_5V:u8=1;
pub const D150_MEASURED_12V:u8=2;
pub const W200_MEASURED_BATTERY:u8=0;
pub const W200_MEASURED_12V:u8=1;
pub const W200_MEASURED_24V:u8=2;
pub const W200_MEASURED_48V:u8=3;
pub const R100_MEASURED_BATTERY:u8=0;
pub const R100_MEASURED_5V:u8=1;
pub const R100_MEASURED_12V:u8=2;
pub const R100_MEASURED_INVERTER:u8=3;
pub const R100_MEASURED_FRONT_AXLE:u8=4;
pub const R100_MEASURED_REAR_AXLE:u8=5;
pub const R100_MEASURED_LIGHT:u8=6;
pub const A200_BATTERY_VOLTAGE:u8=0;
pub const A200_LEFT_DRIVER_VOLTAGE:u8=1;
pub const A200_RIGHT_DRIVER_VOLTAGE:u8=2;
pub const A200_VOLTAGES_SIZE:u8=3;
pub const J100_TOTAL_CURRENT:u8=0;
pub const J100_COMPUTER_CURRENT:u8=1;
pub const J100_DRIVE_CURRENT:u8=2;
pub const J100_USER_CURRENT:u8=3;
pub const D100_TOTAL_CURRENT:u8=0;
pub const D100_COMPUTER_CURRENT:u8=1;
pub const D150_TOTAL_CURRENT:u8=0;
pub const D150_COMPUTER_CURRENT:u8=1;
pub const W200_TOTAL_CURRENT:u8=0;
pub const W200_COMPUTER_CURRENT:u8=1;
pub const W200_12V_CURRENT:u8=2;
pub const W200_24V_CURRENT:u8=3;
pub const R100_TOTAL_CURRENT:u8=0;
pub const A200_MCU_AND_USER_PORT_CURRENT:u8=0;
pub const A200_LEFT_DRIVER_CURRENT:u8=1;
pub const A200_RIGHT_DRIVER_CURRENT:u8=2;
pub const A200_CURRENTS_SIZE:u8=3;}
