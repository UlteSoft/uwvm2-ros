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
/// Complete bounded value for ROS2 booster_interface/msg/BoosterApiReqMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgBoosterApiReqMsg {
  /// Declared api_id; canonical bounds are part of CODEC_IDENTITY.
  pub api_id:i64,
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Booster_interfaceMsgBoosterApiReqMsg {fn default()->Self {Self {
api_id:Default::default(),
body:Default::default(),
}}}
impl Ros2Booster_interfaceMsgBoosterApiReqMsg {
  pub const MAX_WIRE_BYTES:usize=4108;
  pub const CARTESIAN_WIRE_BYTES:u64=4108;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>4108{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.api_id.to_le_bytes());
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.api_id=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgBoosterApiReqMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4108{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/BoosterApiRespMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgBoosterApiRespMsg {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:i64,
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Booster_interfaceMsgBoosterApiRespMsg {fn default()->Self {Self {
status:Default::default(),
body:Default::default(),
}}}
impl Ros2Booster_interfaceMsgBoosterApiRespMsg {
  pub const MAX_WIRE_BYTES:usize=4108;
  pub const CARTESIAN_WIRE_BYTES:u64=4108;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>4108{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.status.to_le_bytes());
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgBoosterApiRespMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4108{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/ButtonEventMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgButtonEventMsg {
  /// Declared event; canonical bounds are part of CODEC_IDENTITY.
  pub event:i8,
  /// Declared button; canonical bounds are part of CODEC_IDENTITY.
  pub button:i32,
}
impl Default for Ros2Booster_interfaceMsgButtonEventMsg {fn default()->Self {Self {
event:Default::default(),
button:Default::default(),
}}}
impl Ros2Booster_interfaceMsgButtonEventMsg {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,4)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.event.to_le_bytes());
put(out,at,&self.button.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.event=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.button=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgButtonEventMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/FallDownState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgFallDownState {
  /// Declared fall_down_state; canonical bounds are part of CODEC_IDENTITY.
  pub fall_down_state:u32,
  /// Declared is_recovery_available; canonical bounds are part of CODEC_IDENTITY.
  pub is_recovery_available:bool,
}
impl Default for Ros2Booster_interfaceMsgFallDownState {fn default()->Self {Self {
fall_down_state:Default::default(),
is_recovery_available:Default::default(),
}}}
impl Ros2Booster_interfaceMsgFallDownState {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CARTESIAN_WIRE_BYTES:u64=5;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,1)?;
if n>5{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.fall_down_state.to_le_bytes());
put(out,at,&[u8::from(self.is_recovery_available)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.fall_down_state=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.is_recovery_available=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgFallDownState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/HandParam.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgHandParam {
  /// Declared angle; canonical bounds are part of CODEC_IDENTITY.
  pub angle:i32,
  /// Declared force; canonical bounds are part of CODEC_IDENTITY.
  pub force:i32,
  /// Declared speed; canonical bounds are part of CODEC_IDENTITY.
  pub speed:i32,
  /// Declared seq; canonical bounds are part of CODEC_IDENTITY.
  pub seq:i32,
}
impl Default for Ros2Booster_interfaceMsgHandParam {fn default()->Self {Self {
angle:Default::default(),
force:Default::default(),
speed:Default::default(),
seq:Default::default(),
}}}
impl Ros2Booster_interfaceMsgHandParam {
  pub const MAX_WIRE_BYTES:usize=16;
  pub const CARTESIAN_WIRE_BYTES:u64=16;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158";
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
put(out,at,&self.angle.to_le_bytes());
put(out,at,&self.force.to_le_bytes());
put(out,at,&self.speed.to_le_bytes());
put(out,at,&self.seq.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.angle=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.force=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.speed=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.seq=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgHandParam {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>16{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/HandCommand.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgHandCommand {
  /// Declared hand_param; canonical bounds are part of CODEC_IDENTITY.
  pub hand_param:Vec<Ros2Booster_interfaceMsgHandParam>,
  /// Declared force_mode; canonical bounds are part of CODEC_IDENTITY.
  pub force_mode:i32,
  /// Declared hand_index; canonical bounds are part of CODEC_IDENTITY.
  pub hand_index:i32,
  /// Declared hand_type; canonical bounds are part of CODEC_IDENTITY.
  pub hand_type:i32,
}
impl Default for Ros2Booster_interfaceMsgHandCommand {fn default()->Self {Self {
hand_param:Default::default(),
force_mode:Default::default(),
hand_index:Default::default(),
hand_type:Default::default(),
}}}
impl Ros2Booster_interfaceMsgHandCommand {
  pub const MAX_WIRE_BYTES:usize=528;
  pub const CARTESIAN_WIRE_BYTES:u64=528;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.hand_param.len()>32{return Err(invalid());}n=add_size(n,4)?;
for element in &self.hand_param {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>528{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.hand_param.len() as u32).to_le_bytes());
for element in &self.hand_param {
element.write_fields(out,at);
}
put(out,at,&self.force_mode.to_le_bytes());
put(out,at,&self.hand_index.to_le_bytes());
put(out,at,&self.hand_type.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>32{return Err(invalid());}
value.hand_param=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Booster_interfaceMsgHandParam::read_fields(bytes,at)?;
value.hand_param.push(decoded);
}}
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.force_mode=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.hand_index=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.hand_type=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgHandCommand {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>528{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/HandDdsMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgHandDdsMsg {
  /// Declared hands_vec; canonical bounds are part of CODEC_IDENTITY.
  pub hands_vec:Vec<Ros2Booster_interfaceMsgHandCommand>,
}
impl Default for Ros2Booster_interfaceMsgHandDdsMsg {fn default()->Self {Self {
hands_vec:Default::default(),
}}}
impl Ros2Booster_interfaceMsgHandDdsMsg {
  pub const MAX_WIRE_BYTES:usize=1060;
  pub const CARTESIAN_WIRE_BYTES:u64=1060;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.hands_vec.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.hands_vec {
n=add_size(n,element.wire_size()?)?;
}
if n>1060{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.hands_vec.len() as u32).to_le_bytes());
for element in &self.hands_vec {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.hands_vec=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Booster_interfaceMsgHandCommand::read_fields(bytes,at)?;
value.hands_vec.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgHandDdsMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1060{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/ImuState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgImuState {
  /// Declared rpy; canonical bounds are part of CODEC_IDENTITY.
  pub rpy:[f32;3],
  /// Declared gyro; canonical bounds are part of CODEC_IDENTITY.
  pub gyro:[f32;3],
  /// Declared acc; canonical bounds are part of CODEC_IDENTITY.
  pub acc:[f32;3],
}
impl Default for Ros2Booster_interfaceMsgImuState {fn default()->Self {Self {
rpy:core::array::from_fn(|_|Default::default()),
gyro:core::array::from_fn(|_|Default::default()),
acc:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Booster_interfaceMsgImuState {
  pub const MAX_WIRE_BYTES:usize=36;
  pub const CARTESIAN_WIRE_BYTES:u64=36;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.rpy {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.gyro {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.acc {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>36{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.rpy {
put(out,at,&element.to_le_bytes());
}
for element in &self.gyro {
put(out,at,&element.to_le_bytes());
}
for element in &self.acc {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rpy[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.gyro[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.acc[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgImuState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>36{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/MotorCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgMotorCmd {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:i8,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:f32,
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:f32,
  /// Declared tau; canonical bounds are part of CODEC_IDENTITY.
  pub tau:f32,
  /// Declared kp; canonical bounds are part of CODEC_IDENTITY.
  pub kp:f32,
  /// Declared kd; canonical bounds are part of CODEC_IDENTITY.
  pub kd:f32,
  /// Declared weight; canonical bounds are part of CODEC_IDENTITY.
  pub weight:f32,
}
impl Default for Ros2Booster_interfaceMsgMotorCmd {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
weight:Default::default(),
}}}
impl Ros2Booster_interfaceMsgMotorCmd {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if !self.q.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tau.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.kp.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.kd.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.weight.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>25{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.tau.to_le_bytes());
put(out,at,&self.kp.to_le_bytes());
put(out,at,&self.kd.to_le_bytes());
put(out,at,&self.weight.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dq=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kp=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kd=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.weight=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgMotorCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/LowCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgLowCmd {
  /// Declared cmd_type; canonical bounds are part of CODEC_IDENTITY.
  pub cmd_type:i8,
  /// Declared motor_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub motor_cmd:Vec<Ros2Booster_interfaceMsgMotorCmd>,
}
impl Default for Ros2Booster_interfaceMsgLowCmd {fn default()->Self {Self {
cmd_type:Default::default(),
motor_cmd:Default::default(),
}}}
impl Ros2Booster_interfaceMsgLowCmd {
  pub const MAX_WIRE_BYTES:usize=730;
  pub const CARTESIAN_WIRE_BYTES:u64=730;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if self.motor_cmd.len()>29{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_cmd {
n=add_size(n,element.wire_size()?)?;
}
if n>730{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.cmd_type.to_le_bytes());
put(out,at,&(self.motor_cmd.len() as u32).to_le_bytes());
for element in &self.motor_cmd {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.cmd_type=decoded;
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_cmd=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Booster_interfaceMsgMotorCmd::read_fields(bytes,at)?;
value.motor_cmd.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgLowCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>730{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/MotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgMotorState {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:i8,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:f32,
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:f32,
  /// Declared ddq; canonical bounds are part of CODEC_IDENTITY.
  pub ddq:f32,
  /// Declared tau_est; canonical bounds are part of CODEC_IDENTITY.
  pub tau_est:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:i8,
  /// Declared lost; canonical bounds are part of CODEC_IDENTITY.
  pub lost:u32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;2],
}
impl Default for Ros2Booster_interfaceMsgMotorState {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
ddq:Default::default(),
tau_est:Default::default(),
temperature:Default::default(),
lost:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Booster_interfaceMsgMotorState {
  pub const MAX_WIRE_BYTES:usize=30;
  pub const CARTESIAN_WIRE_BYTES:u64=30;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if !self.q.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ddq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tau_est.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>30{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.ddq.to_le_bytes());
put(out,at,&self.tau_est.to_le_bytes());
put(out,at,&self.temperature.to_le_bytes());
put(out,at,&self.lost.to_le_bytes());
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dq=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ddq=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau_est=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.temperature=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.lost=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>30{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/LowState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgLowState {
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Booster_interfaceMsgImuState,
  /// Declared motor_state_parallel; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state_parallel:Vec<Ros2Booster_interfaceMsgMotorState>,
  /// Declared motor_state_serial; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state_serial:Vec<Ros2Booster_interfaceMsgMotorState>,
}
impl Default for Ros2Booster_interfaceMsgLowState {fn default()->Self {Self {
imu_state:Default::default(),
motor_state_parallel:Default::default(),
motor_state_serial:Default::default(),
}}}
impl Ros2Booster_interfaceMsgLowState {
  pub const MAX_WIRE_BYTES:usize=1784;
  pub const CARTESIAN_WIRE_BYTES:u64=1784;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.imu_state.wire_size()?)?;
if self.motor_state_parallel.len()>29{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_state_parallel {
n=add_size(n,element.wire_size()?)?;
}
if self.motor_state_serial.len()>29{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_state_serial {
n=add_size(n,element.wire_size()?)?;
}
if n>1784{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.imu_state.write_fields(out,at);
put(out,at,&(self.motor_state_parallel.len() as u32).to_le_bytes());
for element in &self.motor_state_parallel {
element.write_fields(out,at);
}
put(out,at,&(self.motor_state_serial.len() as u32).to_le_bytes());
for element in &self.motor_state_serial {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Booster_interfaceMsgImuState::read_fields(bytes,at)?;
value.imu_state=decoded;
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_state_parallel=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Booster_interfaceMsgMotorState::read_fields(bytes,at)?;
value.motor_state_parallel.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_state_serial=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Booster_interfaceMsgMotorState::read_fields(bytes,at)?;
value.motor_state_serial.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgLowState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1784{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/Odometer.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgOdometer {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared theta; canonical bounds are part of CODEC_IDENTITY.
  pub theta:f32,
}
impl Default for Ros2Booster_interfaceMsgOdometer {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
theta:Default::default(),
}}}
impl Ros2Booster_interfaceMsgOdometer {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.theta.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>12{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.theta.to_le_bytes());
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
value.theta=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgOdometer {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/RawBytesMsg.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgRawBytesMsg {
  /// Declared msg; canonical bounds are part of CODEC_IDENTITY.
  pub msg:Vec<u8>,
}
impl Default for Ros2Booster_interfaceMsgRawBytesMsg {fn default()->Self {Self {
msg:Default::default(),
}}}
impl Ros2Booster_interfaceMsgRawBytesMsg {
  pub const MAX_WIRE_BYTES:usize=2052;
  pub const CARTESIAN_WIRE_BYTES:u64=2052;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.msg.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.msg {
n=add_size(n,1)?;
}
if n>2052{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.msg.len() as u32).to_le_bytes());
for element in &self.msg {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.msg=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.msg.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgRawBytesMsg {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2052{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
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
/// Complete bounded value for ROS2 booster_interface/msg/RawBytesStamped.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgRawBytesStamped {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Std_msgsMsgHeader,
  /// Declared msg; canonical bounds are part of CODEC_IDENTITY.
  pub msg:Vec<u8>,
}
impl Default for Ros2Booster_interfaceMsgRawBytesStamped {fn default()->Self {Self {
header:Default::default(),
msg:Default::default(),
}}}
impl Ros2Booster_interfaceMsgRawBytesStamped {
  pub const MAX_WIRE_BYTES:usize=2192;
  pub const CARTESIAN_WIRE_BYTES:u64=2192;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.msg.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.msg {
n=add_size(n,1)?;
}
if n>2192{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.msg.len() as u32).to_le_bytes());
for element in &self.msg {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Std_msgsMsgHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.msg=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.msg.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgRawBytesStamped {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2192{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/msg/RemoteControllerState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceMsgRemoteControllerState {
  /// Declared event; canonical bounds are part of CODEC_IDENTITY.
  pub event:u32,
  /// Declared lx; canonical bounds are part of CODEC_IDENTITY.
  pub lx:f32,
  /// Declared ly; canonical bounds are part of CODEC_IDENTITY.
  pub ly:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared a; canonical bounds are part of CODEC_IDENTITY.
  pub a:bool,
  /// Declared b; canonical bounds are part of CODEC_IDENTITY.
  pub b:bool,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:bool,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:bool,
  /// Declared lb; canonical bounds are part of CODEC_IDENTITY.
  pub lb:bool,
  /// Declared rb; canonical bounds are part of CODEC_IDENTITY.
  pub rb:bool,
  /// Declared lt; canonical bounds are part of CODEC_IDENTITY.
  pub lt:bool,
  /// Declared rt; canonical bounds are part of CODEC_IDENTITY.
  pub rt:bool,
  /// Declared ls; canonical bounds are part of CODEC_IDENTITY.
  pub ls:bool,
  /// Declared rs; canonical bounds are part of CODEC_IDENTITY.
  pub rs:bool,
  /// Declared back; canonical bounds are part of CODEC_IDENTITY.
  pub back:bool,
  /// Declared start; canonical bounds are part of CODEC_IDENTITY.
  pub start:bool,
  /// Declared hat_c; canonical bounds are part of CODEC_IDENTITY.
  pub hat_c:bool,
  /// Declared hat_u; canonical bounds are part of CODEC_IDENTITY.
  pub hat_u:bool,
  /// Declared hat_d; canonical bounds are part of CODEC_IDENTITY.
  pub hat_d:bool,
  /// Declared hat_l; canonical bounds are part of CODEC_IDENTITY.
  pub hat_l:bool,
  /// Declared hat_r; canonical bounds are part of CODEC_IDENTITY.
  pub hat_r:bool,
  /// Declared hat_lu; canonical bounds are part of CODEC_IDENTITY.
  pub hat_lu:bool,
  /// Declared hat_ld; canonical bounds are part of CODEC_IDENTITY.
  pub hat_ld:bool,
  /// Declared hat_ru; canonical bounds are part of CODEC_IDENTITY.
  pub hat_ru:bool,
  /// Declared hat_rd; canonical bounds are part of CODEC_IDENTITY.
  pub hat_rd:bool,
  /// Declared hat_pos; canonical bounds are part of CODEC_IDENTITY.
  pub hat_pos:u8,
}
impl Default for Ros2Booster_interfaceMsgRemoteControllerState {fn default()->Self {Self {
event:Default::default(),
lx:Default::default(),
ly:Default::default(),
rx:Default::default(),
ry:Default::default(),
a:Default::default(),
b:Default::default(),
x:Default::default(),
y:Default::default(),
lb:Default::default(),
rb:Default::default(),
lt:Default::default(),
rt:Default::default(),
ls:Default::default(),
rs:Default::default(),
back:Default::default(),
start:Default::default(),
hat_c:Default::default(),
hat_u:Default::default(),
hat_d:Default::default(),
hat_l:Default::default(),
hat_r:Default::default(),
hat_lu:Default::default(),
hat_ld:Default::default(),
hat_ru:Default::default(),
hat_rd:Default::default(),
hat_pos:Default::default(),
}}}
impl Ros2Booster_interfaceMsgRemoteControllerState {
  pub const MAX_WIRE_BYTES:usize=42;
  pub const CARTESIAN_WIRE_BYTES:u64=42;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if !self.lx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ly.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>42{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.event.to_le_bytes());
put(out,at,&self.lx.to_le_bytes());
put(out,at,&self.ly.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&[u8::from(self.a)]);
put(out,at,&[u8::from(self.b)]);
put(out,at,&[u8::from(self.x)]);
put(out,at,&[u8::from(self.y)]);
put(out,at,&[u8::from(self.lb)]);
put(out,at,&[u8::from(self.rb)]);
put(out,at,&[u8::from(self.lt)]);
put(out,at,&[u8::from(self.rt)]);
put(out,at,&[u8::from(self.ls)]);
put(out,at,&[u8::from(self.rs)]);
put(out,at,&[u8::from(self.back)]);
put(out,at,&[u8::from(self.start)]);
put(out,at,&[u8::from(self.hat_c)]);
put(out,at,&[u8::from(self.hat_u)]);
put(out,at,&[u8::from(self.hat_d)]);
put(out,at,&[u8::from(self.hat_l)]);
put(out,at,&[u8::from(self.hat_r)]);
put(out,at,&[u8::from(self.hat_lu)]);
put(out,at,&[u8::from(self.hat_ld)]);
put(out,at,&[u8::from(self.hat_ru)]);
put(out,at,&[u8::from(self.hat_rd)]);
put(out,at,&self.hat_pos.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.event=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.lx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ly=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ry=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.a=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.b=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.x=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.y=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.lb=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rb=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.lt=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rt=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.ls=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.rs=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.back=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.start=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_c=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_u=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_d=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_l=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_r=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_lu=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_ld=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_ru=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.hat_rd=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.hat_pos=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceMsgRemoteControllerState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>42{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<42{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/srv/AgentService_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceSrvAgentService_Request {
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Booster_interfaceSrvAgentService_Request {fn default()->Self {Self {
body:Default::default(),
}}}
impl Ros2Booster_interfaceSrvAgentService_Request {
  pub const MAX_WIRE_BYTES:usize=4100;
  pub const CARTESIAN_WIRE_BYTES:u64=4100;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>4100{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceSrvAgentService_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4100{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/srv/AgentService_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceSrvAgentService_Response {
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Booster_interfaceSrvAgentService_Response {fn default()->Self {Self {
body:Default::default(),
}}}
impl Ros2Booster_interfaceSrvAgentService_Response {
  pub const MAX_WIRE_BYTES:usize=4100;
  pub const CARTESIAN_WIRE_BYTES:u64=4100;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>4100{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceSrvAgentService_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4100{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/srv/RpcService_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceSrvRpcService_Request {
  /// Declared msg; canonical bounds are part of CODEC_IDENTITY.
  pub msg:Ros2Booster_interfaceMsgBoosterApiReqMsg,
}
impl Default for Ros2Booster_interfaceSrvRpcService_Request {fn default()->Self {Self {
msg:Default::default(),
}}}
impl Ros2Booster_interfaceSrvRpcService_Request {
  pub const MAX_WIRE_BYTES:usize=4108;
  pub const CARTESIAN_WIRE_BYTES:u64=4108;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.msg.wire_size()?)?;
if n>4108{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.msg.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Booster_interfaceMsgBoosterApiReqMsg::read_fields(bytes,at)?;
value.msg=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceSrvRpcService_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4108{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 booster_interface/srv/RpcService_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Booster_interfaceSrvRpcService_Response {
  /// Declared msg; canonical bounds are part of CODEC_IDENTITY.
  pub msg:Ros2Booster_interfaceMsgBoosterApiRespMsg,
}
impl Default for Ros2Booster_interfaceSrvRpcService_Response {fn default()->Self {Self {
msg:Default::default(),
}}}
impl Ros2Booster_interfaceSrvRpcService_Response {
  pub const MAX_WIRE_BYTES:usize=4108;
  pub const CARTESIAN_WIRE_BYTES:u64=4108;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.msg.wire_size()?)?;
if n>4108{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.msg.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Booster_interfaceMsgBoosterApiRespMsg::read_fields(bytes,at)?;
value.msg=decoded;
Ok(value)}
}
impl Canonical for Ros2Booster_interfaceSrvRpcService_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4108{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}

impl Ros2Booster_interfaceMsgButtonEventMsg {pub const PRESS_DOWN:i8=0;
pub const PRESS_UP:i8=1;
pub const SINGLE_CLICK:i8=0;
pub const DOUBLE_CLICK:i8=1;
pub const TRIPLE_CLICK:i8=0;
pub const LONG_PRESS_START:i8=1;
pub const LONG_PRESS_HOLD:i8=0;
pub const LONG_PRESS_END:i8=1;}

impl Ros2Booster_interfaceMsgFallDownState {pub const IS_READY:u32=0;
pub const IS_FALLING:u32=1;
pub const HAS_FALLEN:u32=2;
pub const IS_GETTING_UP:u32=3;}

impl Ros2Booster_interfaceMsgLowCmd {pub const CMD_TYPE_PARALLEL:i8=0;
pub const CMD_TYPE_SERIAL:i8=1;}
