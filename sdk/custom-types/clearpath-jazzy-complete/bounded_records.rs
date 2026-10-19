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
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxCalibrate_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal {
}
impl Default for Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal {fn default()->Self {Self {
}}}
impl Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CARTESIAN_WIRE_BYTES:u64=0;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_61b01a6234b41f96464de9449b3e8d1b955decf6afa12aa3ba94062c98cec681";
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
impl Canonical for Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>0{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxCalibrate_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxCalibrate_Result {
  /// Declared offset; canonical bounds are part of CODEC_IDENTITY.
  pub offset:Vec<f32>,
}
impl Default for Ros2Clearpath_motor_msgsActionLynxCalibrate_Result {fn default()->Self {Self {
offset:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsActionLynxCalibrate_Result {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_686450363b8b15eba1e9e9e9ff05c9aacfb95bb0d113eaa7c443e4c70d55d41f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.offset.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.offset {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.offset.len() as u32).to_le_bytes());
for element in &self.offset {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.offset=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.offset.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsActionLynxCalibrate_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxCalibrate_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback {
  /// Declared iteration; canonical bounds are part of CODEC_IDENTITY.
  pub iteration:Vec<u16>,
}
impl Default for Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback {fn default()->Self {Self {
iteration:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ebe7badd3f72982079911d8aa72c4ddd27ff0a12d2f948e5970649317a592b53";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.iteration.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.iteration {
n=add_size(n,2)?;
}
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.iteration.len() as u32).to_le_bytes());
for element in &self.iteration {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.iteration=Vec::with_capacity(count);
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.iteration.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxUpdate_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxUpdate_Goal {
  /// Declared file; canonical bounds are part of CODEC_IDENTITY.
  pub file:String,
}
impl Default for Ros2Clearpath_motor_msgsActionLynxUpdate_Goal {fn default()->Self {Self {
file:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsActionLynxUpdate_Goal {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f0362f8a90391e8ffda2e8f3b798769b644091058361882c4e9457196f007565";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.file.len()>256||self.file.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.file.len())?;
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.file.len() as u32).to_le_bytes());put(out,at,self.file.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.file=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsActionLynxUpdate_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxUpdate_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxUpdate_Result {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:Vec<bool>,
}
impl Default for Ros2Clearpath_motor_msgsActionLynxUpdate_Result {fn default()->Self {Self {
success:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsActionLynxUpdate_Result {
  pub const MAX_WIRE_BYTES:usize=6;
  pub const CARTESIAN_WIRE_BYTES:u64=6;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_126fe871dbcb036e485d20e8ccdc8e9fb7be01757f539bfba895aabf44d341d8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.success.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.success {
n=add_size(n,1)?;
}
if n>6{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.success.len() as u32).to_le_bytes());
for element in &self.success {
put(out,at,&[u8::from(*element)]);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.success=Vec::with_capacity(count);
for i in 0..count {
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.success.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsActionLynxUpdate_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>6{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/action/LynxUpdate_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback {
  /// Declared progress; canonical bounds are part of CODEC_IDENTITY.
  pub progress:Vec<f32>,
}
impl Default for Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback {fn default()->Self {Self {
progress:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2179ec9dee627234a1a31e9681ff214cc45ca837349cd8e407e019a354121b19";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.progress.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.progress {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.progress.len() as u32).to_le_bytes());
for element in &self.progress {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.progress=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.progress.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
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
/// Complete bounded value for ROS2 clearpath_control_msgs/msg/DeviceInfo.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_control_msgsMsgDeviceInfo {
  /// Declared device_id; canonical bounds are part of CODEC_IDENTITY.
  pub device_id:String,
  /// Declared device_class; canonical bounds are part of CODEC_IDENTITY.
  pub device_class:String,
  /// Declared model; canonical bounds are part of CODEC_IDENTITY.
  pub model:String,
  /// Declared manufacturer; canonical bounds are part of CODEC_IDENTITY.
  pub manufacturer:String,
  /// Declared firmware_version; canonical bounds are part of CODEC_IDENTITY.
  pub firmware_version:String,
  /// Declared hardware_version; canonical bounds are part of CODEC_IDENTITY.
  pub hardware_version:String,
  /// Declared serial_number; canonical bounds are part of CODEC_IDENTITY.
  pub serial_number:String,
}
impl Default for Ros2Clearpath_control_msgsMsgDeviceInfo {fn default()->Self {Self {
device_id:Default::default(),
device_class:Default::default(),
model:Default::default(),
manufacturer:Default::default(),
firmware_version:Default::default(),
hardware_version:Default::default(),
serial_number:Default::default(),
}}}
impl Ros2Clearpath_control_msgsMsgDeviceInfo {
  pub const MAX_WIRE_BYTES:usize=1820;
  pub const CARTESIAN_WIRE_BYTES:u64=1820;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9111f5d4b5324c36eb85f01e6fdbc3c589479ff5b64a8fa7b9d467aa14675c3e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.device_id.len()>256||self.device_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.device_id.len())?;
if self.device_class.len()>256||self.device_class.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.device_class.len())?;
if self.model.len()>256||self.model.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.model.len())?;
if self.manufacturer.len()>256||self.manufacturer.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.manufacturer.len())?;
if self.firmware_version.len()>256||self.firmware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware_version.len())?;
if self.hardware_version.len()>256||self.hardware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.hardware_version.len())?;
if self.serial_number.len()>256||self.serial_number.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.serial_number.len())?;
if n>1820{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.device_id.len() as u32).to_le_bytes());put(out,at,self.device_id.as_bytes());
put(out,at,&(self.device_class.len() as u32).to_le_bytes());put(out,at,self.device_class.as_bytes());
put(out,at,&(self.model.len() as u32).to_le_bytes());put(out,at,self.model.as_bytes());
put(out,at,&(self.manufacturer.len() as u32).to_le_bytes());put(out,at,self.manufacturer.as_bytes());
put(out,at,&(self.firmware_version.len() as u32).to_le_bytes());put(out,at,self.firmware_version.as_bytes());
put(out,at,&(self.hardware_version.len() as u32).to_le_bytes());put(out,at,self.hardware_version.as_bytes());
put(out,at,&(self.serial_number.len() as u32).to_le_bytes());put(out,at,self.serial_number.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.device_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.device_class=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.model=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.manufacturer=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.firmware_version=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.hardware_version=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.serial_number=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_control_msgsMsgDeviceInfo {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1820{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_control_msgs/msg/StreamControl.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_control_msgsMsgStreamControl {
  /// Declared stream; canonical bounds are part of CODEC_IDENTITY.
  pub stream:String,
  /// Declared enabled; canonical bounds are part of CODEC_IDENTITY.
  pub enabled:bool,
}
impl Default for Ros2Clearpath_control_msgsMsgStreamControl {fn default()->Self {Self {
stream:Default::default(),
enabled:Default::default(),
}}}
impl Ros2Clearpath_control_msgsMsgStreamControl {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_26208e266c9185bfb0e715407ab0fe313f2d47e6bc9f54594a102c3a09d34734";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.stream.len()>256||self.stream.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.stream.len())?;
n=add_size(n,1)?;
if n>261{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.stream.len() as u32).to_le_bytes());put(out,at,self.stream.as_bytes());
put(out,at,&[u8::from(self.enabled)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.stream=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.enabled=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_control_msgsMsgStreamControl {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxDebug.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxDebug {
  /// Declared can_id; canonical bounds are part of CODEC_IDENTITY.
  pub can_id:u8,
  /// Declared joint_name; canonical bounds are part of CODEC_IDENTITY.
  pub joint_name:String,
  /// Declared fault_frequency; canonical bounds are part of CODEC_IDENTITY.
  pub fault_frequency:u16,
  /// Declared accumulator_flags; canonical bounds are part of CODEC_IDENTITY.
  pub accumulator_flags:u32,
  /// Declared accumulators; canonical bounds are part of CODEC_IDENTITY.
  pub accumulators:Vec<f32>,
  /// Declared currents; canonical bounds are part of CODEC_IDENTITY.
  pub currents:Vec<f32>,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxDebug {fn default()->Self {Self {
can_id:Default::default(),
joint_name:Default::default(),
fault_frequency:Default::default(),
accumulator_flags:Default::default(),
accumulators:Default::default(),
currents:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxDebug {
  pub const MAX_WIRE_BYTES:usize=291;
  pub const CARTESIAN_WIRE_BYTES:u64=291;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0341ae040f1e8a1f980bde84498c4a4f8b9c55c3806839a73727c1579ab4416e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.joint_name.len()>256||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
n=add_size(n,2)?;
n=add_size(n,4)?;
if self.accumulators.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.accumulators {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.currents.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.currents {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>291{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.can_id.to_le_bytes());
put(out,at,&(self.joint_name.len() as u32).to_le_bytes());put(out,at,self.joint_name.as_bytes());
put(out,at,&self.fault_frequency.to_le_bytes());
put(out,at,&self.accumulator_flags.to_le_bytes());
put(out,at,&(self.accumulators.len() as u32).to_le_bytes());
for element in &self.accumulators {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.currents.len() as u32).to_le_bytes());
for element in &self.currents {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.can_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_name=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.fault_frequency=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.accumulator_flags=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.accumulators=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.accumulators.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.currents=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.currents.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxDebug {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>291{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<19{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxFeedback {
  /// Declared can_id; canonical bounds are part of CODEC_IDENTITY.
  pub can_id:u8,
  /// Declared joint_name; canonical bounds are part of CODEC_IDENTITY.
  pub joint_name:String,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:f32,
  /// Declared voltage; canonical bounds are part of CODEC_IDENTITY.
  pub voltage:f32,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:f32,
  /// Declared travel; canonical bounds are part of CODEC_IDENTITY.
  pub travel:f32,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxFeedback {fn default()->Self {Self {
can_id:Default::default(),
joint_name:Default::default(),
current:Default::default(),
voltage:Default::default(),
velocity:Default::default(),
travel:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxFeedback {
  pub const MAX_WIRE_BYTES:usize=277;
  pub const CARTESIAN_WIRE_BYTES:u64=277;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5d92c7085019ebf00ba93788b5b2f4055ad40b2c6714575a66fb75cf314429eb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.joint_name.len()>256||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
if !self.current.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.voltage.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.velocity.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.travel.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>277{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.can_id.to_le_bytes());
put(out,at,&(self.joint_name.len() as u32).to_le_bytes());put(out,at,self.joint_name.as_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.voltage.to_le_bytes());
put(out,at,&self.velocity.to_le_bytes());
put(out,at,&self.travel.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.can_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_name=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.current=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.voltage=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.travel=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>277{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<21{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxMotorProtection.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxMotorProtection {
  /// Declared can_id; canonical bounds are part of CODEC_IDENTITY.
  pub can_id:u8,
  /// Declared joint_name; canonical bounds are part of CODEC_IDENTITY.
  pub joint_name:String,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:u8,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxMotorProtection {fn default()->Self {Self {
can_id:Default::default(),
joint_name:Default::default(),
state:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxMotorProtection {
  pub const MAX_WIRE_BYTES:usize=262;
  pub const CARTESIAN_WIRE_BYTES:u64=262;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f824abfa97f6f24934e04094d66170167f19a14fb933f325cbef4ed6fca16630";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.joint_name.len()>256||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
n=add_size(n,1)?;
if n>262{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.can_id.to_le_bytes());
put(out,at,&(self.joint_name.len() as u32).to_le_bytes());put(out,at,self.joint_name.as_bytes());
put(out,at,&self.state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.can_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_name=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxMotorProtection {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>262{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<6{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxMultiDebug.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxMultiDebug {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:Vec<Ros2Clearpath_motor_msgsMsgLynxDebug>,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxMultiDebug {fn default()->Self {Self {
header:Default::default(),
drivers:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxMultiDebug {
  pub const MAX_WIRE_BYTES:usize=1308;
  pub const CARTESIAN_WIRE_BYTES:u64=1308;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_056abd2153e7f1a6df223bf5daba581e6c218bb7ef365444fc5c3171dfc62e95";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.drivers.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.drivers {
n=add_size(n,element.wire_size()?)?;
}
if n>1308{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Clearpath_motor_msgsMsgLynxDebug::read_fields(bytes,at)?;
value.drivers.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxMultiDebug {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1308{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxMultiFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxMultiFeedback {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:Vec<Ros2Clearpath_motor_msgsMsgLynxFeedback>,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxMultiFeedback {fn default()->Self {Self {
header:Default::default(),
drivers:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxMultiFeedback {
  pub const MAX_WIRE_BYTES:usize=1252;
  pub const CARTESIAN_WIRE_BYTES:u64=1252;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2c83cade6bec4d766fd2cff97338642b3d2065d41e9968ccf5054c6f231242c7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.drivers.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.drivers {
n=add_size(n,element.wire_size()?)?;
}
if n>1252{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Clearpath_motor_msgsMsgLynxFeedback::read_fields(bytes,at)?;
value.drivers.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxMultiFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1252{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxStatus {
  /// Declared can_id; canonical bounds are part of CODEC_IDENTITY.
  pub can_id:u8,
  /// Declared joint_name; canonical bounds are part of CODEC_IDENTITY.
  pub joint_name:String,
  /// Declared firmware_version; canonical bounds are part of CODEC_IDENTITY.
  pub firmware_version:String,
  /// Declared motor_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub motor_temperature:f32,
  /// Declared mcu_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub mcu_temperature:f32,
  /// Declared pcb_temperature; canonical bounds are part of CODEC_IDENTITY.
  pub pcb_temperature:f32,
  /// Declared stopped; canonical bounds are part of CODEC_IDENTITY.
  pub stopped:bool,
  /// Declared status_flags; canonical bounds are part of CODEC_IDENTITY.
  pub status_flags:u32,
  /// Declared warning_flags; canonical bounds are part of CODEC_IDENTITY.
  pub warning_flags:u32,
  /// Declared error_flags; canonical bounds are part of CODEC_IDENTITY.
  pub error_flags:u32,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxStatus {fn default()->Self {Self {
can_id:Default::default(),
joint_name:Default::default(),
firmware_version:Default::default(),
motor_temperature:Default::default(),
mcu_temperature:Default::default(),
pcb_temperature:Default::default(),
stopped:Default::default(),
status_flags:Default::default(),
warning_flags:Default::default(),
error_flags:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxStatus {
  pub const MAX_WIRE_BYTES:usize=546;
  pub const CARTESIAN_WIRE_BYTES:u64=546;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b68a7ab9bc5312c59d2ef86e49313468b631e8cacdf52468374b67f4eced5c6d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.joint_name.len()>256||self.joint_name.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.joint_name.len())?;
if self.firmware_version.len()>256||self.firmware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware_version.len())?;
if !self.motor_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.mcu_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.pcb_temperature.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>546{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.can_id.to_le_bytes());
put(out,at,&(self.joint_name.len() as u32).to_le_bytes());put(out,at,self.joint_name.as_bytes());
put(out,at,&(self.firmware_version.len() as u32).to_le_bytes());put(out,at,self.firmware_version.as_bytes());
put(out,at,&self.motor_temperature.to_le_bytes());
put(out,at,&self.mcu_temperature.to_le_bytes());
put(out,at,&self.pcb_temperature.to_le_bytes());
put(out,at,&[u8::from(self.stopped)]);
put(out,at,&self.status_flags.to_le_bytes());
put(out,at,&self.warning_flags.to_le_bytes());
put(out,at,&self.error_flags.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.can_id=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.joint_name=decoded;
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.firmware_version=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.motor_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.mcu_temperature=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pcb_temperature=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.stopped=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.status_flags=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.warning_flags=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_flags=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>546{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<34{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxMultiStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxMultiStatus {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared drivers; canonical bounds are part of CODEC_IDENTITY.
  pub drivers:Vec<Ros2Clearpath_motor_msgsMsgLynxStatus>,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxMultiStatus {fn default()->Self {Self {
header:Default::default(),
drivers:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxMultiStatus {
  pub const MAX_WIRE_BYTES:usize=2328;
  pub const CARTESIAN_WIRE_BYTES:u64=2328;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_595cd0847b4332e14cc067b916166e9a6e1d1e12a7a68cdb7e1cd414a9ea9ce1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.drivers.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.drivers {
n=add_size(n,element.wire_size()?)?;
}
if n>2328{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=Ros2Clearpath_motor_msgsMsgLynxStatus::read_fields(bytes,at)?;
value.drivers.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxMultiStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2328{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_motor_msgs/msg/LynxSystemProtection.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_motor_msgsMsgLynxSystemProtection {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared system_state; canonical bounds are part of CODEC_IDENTITY.
  pub system_state:u8,
  /// Declared motor_states; canonical bounds are part of CODEC_IDENTITY.
  pub motor_states:Vec<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>,
}
impl Default for Ros2Clearpath_motor_msgsMsgLynxSystemProtection {fn default()->Self {Self {
header:Default::default(),
system_state:Default::default(),
motor_states:Default::default(),
}}}
impl Ros2Clearpath_motor_msgsMsgLynxSystemProtection {
  pub const MAX_WIRE_BYTES:usize=669;
  pub const CARTESIAN_WIRE_BYTES:u64=669;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4d662de03c34bb633a122399be1a1aaea6187550c83231bf6d8a06b95fa82d9f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,1)?;
if self.motor_states.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_states {
n=add_size(n,element.wire_size()?)?;
}
if n>669{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.system_state.to_le_bytes());
put(out,at,&(self.motor_states.len() as u32).to_le_bytes());
for element in &self.motor_states {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.system_state=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.motor_states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Clearpath_motor_msgsMsgLynxMotorProtection::read_fields(bytes,at)?;
value.motor_states.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_motor_msgsMsgLynxSystemProtection {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>669{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/DisplayStatus.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgDisplayStatus {
  /// Declared string1; canonical bounds are part of CODEC_IDENTITY.
  pub string1:String,
  /// Declared string2; canonical bounds are part of CODEC_IDENTITY.
  pub string2:String,
}
impl Default for Ros2Clearpath_platform_msgsMsgDisplayStatus {fn default()->Self {Self {
string1:Default::default(),
string2:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgDisplayStatus {
  pub const MAX_WIRE_BYTES:usize=106;
  pub const CARTESIAN_WIRE_BYTES:u64=106;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e4585117414581491304a26d528b44c770799f622c3f1cde037078f5cb2462c2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.string1.len()>49||self.string1.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.string1.len())?;
if self.string2.len()>49||self.string2.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.string2.len())?;
if n>106{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.string1.len() as u32).to_le_bytes());put(out,at,self.string1.as_bytes());
put(out,at,&(self.string2.len() as u32).to_le_bytes());put(out,at,self.string2.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>49{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.string1=decoded;
let size=read_count(bytes,at)?;if size>49{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.string2=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgDisplayStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>106{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a7d96f2242d992baf96f31bd8f93088547f92c72912c241c2d237f7714800cc8";
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
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f975a3ec1fca4f8add9523d4f9f99963be520c0e2982e644551b71cbd4b8a27";
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
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/PinoutCommand.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgPinoutCommand {
  /// Declared rails; canonical bounds are part of CODEC_IDENTITY.
  pub rails:Vec<bool>,
  /// Declared outputs; canonical bounds are part of CODEC_IDENTITY.
  pub outputs:Vec<u32>,
}
impl Default for Ros2Clearpath_platform_msgsMsgPinoutCommand {fn default()->Self {Self {
rails:Default::default(),
outputs:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgPinoutCommand {
  pub const MAX_WIRE_BYTES:usize=18;
  pub const CARTESIAN_WIRE_BYTES:u64=18;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b26c2baf1e64cefa889c4ac6bd6b7f4647d9247eca0626b83cd726547a4e80ff";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.rails.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.rails {
n=add_size(n,1)?;
}
if self.outputs.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.outputs {
n=add_size(n,4)?;
}
if n>18{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.rails.len() as u32).to_le_bytes());
for element in &self.rails {
put(out,at,&[u8::from(*element)]);
}
put(out,at,&(self.outputs.len() as u32).to_le_bytes());
for element in &self.outputs {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.rails=Vec::with_capacity(count);
for i in 0..count {
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rails.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.outputs=Vec::with_capacity(count);
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.outputs.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgPinoutCommand {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>18{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/PinoutState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgPinoutState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared rails; canonical bounds are part of CODEC_IDENTITY.
  pub rails:Vec<bool>,
  /// Declared inputs; canonical bounds are part of CODEC_IDENTITY.
  pub inputs:Vec<bool>,
  /// Declared outputs; canonical bounds are part of CODEC_IDENTITY.
  pub outputs:Vec<bool>,
  /// Declared output_periods; canonical bounds are part of CODEC_IDENTITY.
  pub output_periods:Vec<u32>,
}
impl Default for Ros2Clearpath_platform_msgsMsgPinoutState {fn default()->Self {Self {
header:Default::default(),
rails:Default::default(),
inputs:Default::default(),
outputs:Default::default(),
output_periods:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgPinoutState {
  pub const MAX_WIRE_BYTES:usize=170;
  pub const CARTESIAN_WIRE_BYTES:u64=170;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a936c6d91440709d50fa9b0be8fb567a9ce793e84f57e5379f6a160c8cbf73e7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.rails.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.rails {
n=add_size(n,1)?;
}
if self.inputs.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.inputs {
n=add_size(n,1)?;
}
if self.outputs.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.outputs {
n=add_size(n,1)?;
}
if self.output_periods.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.output_periods {
n=add_size(n,4)?;
}
if n>170{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.rails.len() as u32).to_le_bytes());
for element in &self.rails {
put(out,at,&[u8::from(*element)]);
}
put(out,at,&(self.inputs.len() as u32).to_le_bytes());
for element in &self.inputs {
put(out,at,&[u8::from(*element)]);
}
put(out,at,&(self.outputs.len() as u32).to_le_bytes());
for element in &self.outputs {
put(out,at,&[u8::from(*element)]);
}
put(out,at,&(self.output_periods.len() as u32).to_le_bytes());
for element in &self.output_periods {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.rails=Vec::with_capacity(count);
for i in 0..count {
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rails.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.inputs=Vec::with_capacity(count);
for i in 0..count {
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.inputs.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.outputs=Vec::with_capacity(count);
for i in 0..count {
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.outputs.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.output_periods=Vec::with_capacity(count);
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.output_periods.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgPinoutState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>170{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3fc241d6b81da2f80bfce22933b81786b5e582715dc66cd238fa40b239e7aecc";
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
}
impl Default for Ros2Clearpath_platform_msgsMsgStatus {fn default()->Self {Self {
header:Default::default(),
hardware_id:Default::default(),
firmware_version:Default::default(),
mcu_uptime:Default::default(),
connection_uptime:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgStatus {
  pub const MAX_WIRE_BYTES:usize=676;
  pub const CARTESIAN_WIRE_BYTES:u64=676;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1bd99cc11ea9efcb51eeba8fc1d599960c16e958a8adf33a56d47fe27be87782";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.hardware_id.len()>256||self.hardware_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.hardware_id.len())?;
if self.firmware_version.len()>256||self.firmware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware_version.len())?;
n=add_size(n,self.mcu_uptime.wire_size()?)?;
n=add_size(n,self.connection_uptime.wire_size()?)?;
if n>676{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.hardware_id.len() as u32).to_le_bytes());put(out,at,self.hardware_id.as_bytes());
put(out,at,&(self.firmware_version.len() as u32).to_le_bytes());put(out,at,self.firmware_version.as_bytes());
self.mcu_uptime.write_fields(out,at);
self.connection_uptime.write_fields(out,at);
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
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgStatus {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>676{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 clearpath_platform_msgs/msg/Temperature.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsMsgTemperature {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared temperatures; canonical bounds are part of CODEC_IDENTITY.
  pub temperatures:Vec<f32>,
}
impl Default for Ros2Clearpath_platform_msgsMsgTemperature {fn default()->Self {Self {
header:Default::default(),
temperatures:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsMsgTemperature {
  pub const MAX_WIRE_BYTES:usize=152;
  pub const CARTESIAN_WIRE_BYTES:u64=152;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b4ae4d6ace9045ce96b0931f08d3b2c7ec358ab9c6da7022afd9ea489f9b0466";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.temperatures.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.temperatures {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>152{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.temperatures.len() as u32).to_le_bytes());
for element in &self.temperatures {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.temperatures=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.temperatures.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsMsgTemperature {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>152{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_control_msgs/srv/PowerControl_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_control_msgsSrvPowerControl_Request {
  /// Declared power_on; canonical bounds are part of CODEC_IDENTITY.
  pub power_on:bool,
}
impl Default for Ros2Clearpath_control_msgsSrvPowerControl_Request {fn default()->Self {Self {
power_on:Default::default(),
}}}
impl Ros2Clearpath_control_msgsSrvPowerControl_Request {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ad49d42d8895b3bd219799ab9cc080cf09d23aa4bca4c1172d19bc18fa416c4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.power_on)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.power_on=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_control_msgsSrvPowerControl_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_control_msgs/srv/PowerControl_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_control_msgsSrvPowerControl_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Clearpath_control_msgsSrvPowerControl_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Clearpath_control_msgsSrvPowerControl_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6d74d07d6a8ff1c41ed4ca13939d54e9e642f423dc2ff6598665238ebbf10a8b";
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
impl Canonical for Ros2Clearpath_control_msgsSrvPowerControl_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 clearpath_platform_msgs/srv/SetPinout_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsSrvSetPinout_Request {
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:bool,
  /// Declared toggle_period; canonical bounds are part of CODEC_IDENTITY.
  pub toggle_period:u32,
}
impl Default for Ros2Clearpath_platform_msgsSrvSetPinout_Request {fn default()->Self {Self {
state:Default::default(),
toggle_period:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsSrvSetPinout_Request {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3a3a2f8f16279c2e7a057353cd1f7f11e1a65e887dd3ce0a4b7f584708afb238";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,4)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.state)]);
put(out,at,&self.toggle_period.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.state=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.toggle_period=decoded;
Ok(value)}
}
impl Canonical for Ros2Clearpath_platform_msgsSrvSetPinout_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 clearpath_platform_msgs/srv/SetPinout_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Clearpath_platform_msgsSrvSetPinout_Response {
  /// Declared success; canonical bounds are part of CODEC_IDENTITY.
  pub success:bool,
  /// Declared message; canonical bounds are part of CODEC_IDENTITY.
  pub message:String,
}
impl Default for Ros2Clearpath_platform_msgsSrvSetPinout_Response {fn default()->Self {Self {
success:Default::default(),
message:Default::default(),
}}}
impl Ros2Clearpath_platform_msgsSrvSetPinout_Response {
  pub const MAX_WIRE_BYTES:usize=261;
  pub const CARTESIAN_WIRE_BYTES:u64=261;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b643ef5de2dfb725906ef8f11d79601400953211b47220be7a6fcc05d1be8ee3";
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
impl Canonical for Ros2Clearpath_platform_msgsSrvSetPinout_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>261{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Clearpath_motor_msgsMsgLynxDebug {pub const ACCUMULATOR_FLAG_ABOVE_RATED:u8=0;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_1:u8=1;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_2:u8=2;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_3:u8=3;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_4:u8=4;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_5:u8=5;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_6:u8=6;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_7:u8=7;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_8:u8=8;
pub const ACCUMULATOR_FLAG_CONTINUOUS_1_9:u8=9;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_0:u8=10;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_1:u8=11;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_2:u8=12;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_3:u8=13;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_4:u8=14;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_5:u8=15;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_6:u8=16;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_7:u8=17;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_8:u8=18;
pub const ACCUMULATOR_FLAG_CONTINUOUS_2_9:u8=19;
pub const ACCUMULATOR_FLAG_CONTINUOUS_3_0:u8=20;
pub const ACCUMULATOR_ABOVE_RATED:u8=0;
pub const ACCUMULATOR_CONTINUOUS_1_1:u8=1;
pub const ACCUMULATOR_CONTINUOUS_1_5:u8=2;
pub const ACCUMULATOR_CONTINUOUS_2_0:u8=3;
pub const ACCUMULATOR_CONTINUOUS_2_5:u8=4;
pub const ACCUMULATOR_CONTINUOUS_3_0:u8=5;
pub const CURRENT_ID_FB:u8=0;
pub const CURRENT_ID_FF:u8=1;
pub const CURRENT_ID_REF:u8=2;
pub const CURRENT_IQ_FB:u8=3;
pub const CURRENT_IQ_FF:u8=4;
pub const CURRENT_IQ_REF:u8=5;}

impl Ros2Clearpath_motor_msgsMsgLynxMotorProtection {pub const NORMAL:u8=0;
pub const THROTTLED:u8=1;
pub const OVERHEATED:u8=2;
pub const ERROR:u8=3;}

impl Ros2Clearpath_motor_msgsMsgLynxStatus {pub const STATUS_FLAG_ADC_CALIBRATED:u8=0;
pub const STATUS_FLAG_FOC_ENABLED:u8=1;
pub const STATUS_FLAG_CALIBRATION_REQUESTED:u8=2;
pub const STATUS_FLAG_CALIBRATION_CANCELLED:u8=3;
pub const STATUS_FLAG_ESTOPPED:u8=4;
pub const WARNING_FLAG_MOTOR_THERMISTOR:u8=0;
pub const WARNING_FLAG_PCB_THERMISTOR:u8=1;
pub const WARNING_FLAG_PHASE:u8=2;
pub const WARNING_FLAG_PHASE_A:u8=3;
pub const WARNING_FLAG_PHASE_B:u8=4;
pub const WARNING_FLAG_PHASE_C:u8=5;
pub const WARNING_FLAG_ENCODER_INDEX:u8=6;
pub const WARNING_FLAG_ENCODER_OUTPUT_A:u8=7;
pub const WARNING_FLAG_ENCODER_OUTPUT_B:u8=8;
pub const ERROR_FLAG_NOT_CALIBRATED:u8=0;
pub const ERROR_FLAG_MOTOR_FAULT:u8=1;
pub const ERROR_FLAG_MOTOR_STALLING:u8=2;
pub const ERROR_FLAG_MOTOR_THERMISTOR:u8=3;
pub const ERROR_FLAG_PCB_THERMISTOR:u8=4;
pub const ERROR_FLAG_PHASE:u8=5;
pub const ERROR_FLAG_PHASE_A:u8=6;
pub const ERROR_FLAG_PHASE_B:u8=7;
pub const ERROR_FLAG_PHASE_C:u8=8;
pub const ERROR_FLAG_ENCODER_POWER:u8=9;
pub const ERROR_FLAG_ENCODER_INDEX:u8=10;
pub const ERROR_FLAG_ENCODER_OUTPUT_A:u8=11;
pub const ERROR_FLAG_ENCODER_OUTPUT_B:u8=12;}

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

impl Ros2Clearpath_platform_msgsMsgFans {pub const CC01_FAN1:u8=0;
pub const CC01_FAN2:u8=1;
pub const CC01_FAN3:u8=2;
pub const CC01_FAN4:u8=3;
pub const CC01_FAN5:u8=4;
pub const CC01_FAN6:u8=5;
pub const CC01_FAN7:u8=6;
pub const CC01_FAN8:u8=7;
pub const R100_EQUIPMENT_BAY_INTAKE:u8=0;
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
pub const W200_LIGHTS_REAR_RIGHT:u8=3;
pub const A300_LIGHTS_FRONT_RIGHT:u8=0;
pub const A300_LIGHTS_FRONT_LEFT:u8=1;
pub const A300_LIGHTS_REAR_LEFT:u8=2;
pub const A300_LIGHTS_REAR_RIGHT:u8=3;}

impl Ros2Clearpath_platform_msgsMsgPinoutCommand {pub const CC01_USER_PWR_CTRL:u8=0;
pub const D100_AUX_OUT1:u8=0;
pub const D100_AUX_OUT2:u8=1;
pub const D100_AUX_OUT3:u8=2;
pub const D150_AUX_OUT1:u8=0;
pub const D150_AUX_OUT2:u8=1;
pub const D150_AUX_OUT3:u8=2;
pub const CC01_AUX_OUT1:u8=0;
pub const CC01_AUX_OUT2:u8=1;
pub const CC01_AUX_OUT3:u8=2;
pub const CC01_GPO0:u8=3;
pub const CC01_GPO1:u8=4;
pub const CC01_GPO2:u8=5;
pub const CC01_GPO3:u8=6;}

impl Ros2Clearpath_platform_msgsMsgPinoutState {pub const CC01_USER_PWR_CTRL:u8=0;
pub const D100_AUX_IN1:u8=0;
pub const D100_AUX_IN2:u8=1;
pub const D100_AUX_IN3:u8=2;
pub const D150_AUX_IN1:u8=0;
pub const D150_AUX_IN2:u8=1;
pub const D150_AUX_IN3:u8=2;
pub const CC01_AUX_IN1:u8=0;
pub const CC01_AUX_IN2:u8=1;
pub const CC01_AUX_IN3:u8=2;
pub const CC01_GPI0:u8=3;
pub const CC01_GPI1:u8=4;
pub const CC01_GPI2:u8=5;
pub const CC01_GPI3:u8=6;
pub const D100_AUX_OUT1:u8=0;
pub const D100_AUX_OUT2:u8=1;
pub const D100_AUX_OUT3:u8=2;
pub const D150_AUX_OUT1:u8=0;
pub const D150_AUX_OUT2:u8=1;
pub const D150_AUX_OUT3:u8=2;
pub const CC01_AUX_OUT1:u8=0;
pub const CC01_AUX_OUT2:u8=1;
pub const CC01_AUX_OUT3:u8=2;
pub const CC01_GPO0:u8=3;
pub const CC01_GPO1:u8=4;
pub const CC01_GPO2:u8=5;
pub const CC01_GPO3:u8=6;}

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
pub const CC01_BATTERY_VOLTAGE:u8=0;
pub const CC01_USER_BATTERY_VOLTAGE:u8=1;
pub const CC01_USER_24V_VOLTAGE:u8=2;
pub const CC01_USER_12VB_VOLTAGE:u8=3;
pub const CC01_SYSTEM_12V_VOLTAGE:u8=4;
pub const CC01_EXPANSION_VOLTAGE:u8=5;
pub const CC01_BREAKOUT_24V_AUX_VOLTAGE:u8=6;
pub const CC01_BREAKOUT_12V_AUX_VOLTAGE:u8=7;
pub const CC01_BREAKOUT_USER_12VA_VOLTAGE:u8=8;
pub const CC01_BREAKOUT_LYNX1_VOLTAGE:u8=9;
pub const CC01_BREAKOUT_LYNX2_VOLTAGE:u8=10;
pub const CC01_BREAKOUT_LYNX3_VOLTAGE:u8=11;
pub const CC01_BREAKOUT_LYNX4_VOLTAGE:u8=12;
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
pub const A200_CURRENTS_SIZE:u8=3;
pub const CC01_BATTERY_PACK_CURRENT:u8=0;
pub const CC01_USER_BATTERY_CURRENT:u8=1;
pub const CC01_AUX_HES_CURRENT:u8=2;
pub const CC01_SYSTEM_12V_CURRENT:u8=3;
pub const CC01_24V_AUX_CURRENT:u8=4;
pub const CC01_12V_AUX_CURRENT:u8=5;
pub const CC01_BREAKOUT_USER_24V_CURRENT:u8=6;
pub const CC01_BREAKOUT_USER_12VA_CURRENT:u8=7;
pub const CC01_BREAKOUT_USER_12VB_CURRENT:u8=8;
pub const CC01_BREAKOUT_USER_LYNX1_CURRENT:u8=9;
pub const CC01_BREAKOUT_USER_LYNX2_CURRENT:u8=10;
pub const CC01_BREAKOUT_USER_LYNX3_CURRENT:u8=11;
pub const CC01_BREAKOUT_USER_LYNX4_CURRENT:u8=12;}

impl Ros2Clearpath_platform_msgsMsgTemperature {pub const J100_MCU:u8=0;
pub const J100_PCB:u8=1;
pub const D100_MCU:u8=0;
pub const D100_PCB:u8=1;
pub const D150_MCU:u8=0;
pub const D150_PCB:u8=1;
pub const W200_MCU:u8=0;
pub const R100_MCU:u8=0;
pub const R100_PCB:u8=1;
pub const CC01_MCU:u8=0;
pub const CC01_FAN1:u8=1;
pub const CC01_FAN2:u8=2;
pub const CC01_FAN3:u8=3;
pub const CC01_FAN4:u8=4;
pub const CC01_FAN5:u8=5;
pub const CC01_FAN6:u8=6;
pub const CC01_FAN7:u8=7;
pub const CC01_FAN8:u8=8;
pub const CC01_5V_INDUCTOR:u8=9;
pub const CC01_MAIN_GND_LUG:u8=10;
pub const CC01_24V_DCDC:u8=11;
pub const CC01_12V_DCDC:u8=12;
pub const CC01_EXT_1:u8=13;
pub const CC01_EXT_2:u8=14;
pub const CC01_EXT_3:u8=15;
pub const CC01_EXT_4:u8=16;
pub const CC01_BREAKOUT_1:u8=17;
pub const CC01_BREAKOUT_2:u8=18;}
