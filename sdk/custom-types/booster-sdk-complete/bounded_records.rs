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
/// Complete bounded value for ROS2 uwvm_booster_sdk_bridge/msg/ImuState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_booster_sdk_bridgeMsgImuState {
  /// Declared rpy; canonical bounds are part of CODEC_IDENTITY.
  pub rpy:[f32;3],
  /// Declared gyro; canonical bounds are part of CODEC_IDENTITY.
  pub gyro:[f32;3],
  /// Declared acc; canonical bounds are part of CODEC_IDENTITY.
  pub acc:[f32;3],
}
impl Default for Ros2Uwvm_booster_sdk_bridgeMsgImuState {fn default()->Self {Self {
rpy:core::array::from_fn(|_|Default::default()),
gyro:core::array::from_fn(|_|Default::default()),
acc:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Uwvm_booster_sdk_bridgeMsgImuState {
  pub const MAX_WIRE_BYTES:usize=36;
  pub const CARTESIAN_WIRE_BYTES:u64=36;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04";
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
impl Canonical for Ros2Uwvm_booster_sdk_bridgeMsgImuState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>36{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_booster_sdk_bridge/msg/MotorCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
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
impl Default for Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
weight:Default::default(),
}}}
impl Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4";
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
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
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
impl Canonical for Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_booster_sdk_bridge/msg/MotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_booster_sdk_bridgeMsgMotorState {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:f32,
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:f32,
  /// Declared ddq; canonical bounds are part of CODEC_IDENTITY.
  pub ddq:f32,
  /// Declared tau_est; canonical bounds are part of CODEC_IDENTITY.
  pub tau_est:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:u8,
  /// Declared lost; canonical bounds are part of CODEC_IDENTITY.
  pub lost:u32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;2],
}
impl Default for Ros2Uwvm_booster_sdk_bridgeMsgMotorState {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
ddq:Default::default(),
tau_est:Default::default(),
temperature:Default::default(),
lost:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Uwvm_booster_sdk_bridgeMsgMotorState {
  pub const MAX_WIRE_BYTES:usize=30;
  pub const CARTESIAN_WIRE_BYTES:u64=30;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af";
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
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
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
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
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
impl Canonical for Ros2Uwvm_booster_sdk_bridgeMsgMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>30{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<30{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_booster_sdk_bridge/msg/LowCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_booster_sdk_bridgeMsgLowCmd {
  /// Declared cmd_type; canonical bounds are part of CODEC_IDENTITY.
  pub cmd_type:i32,
  /// Declared motor_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub motor_cmd:Vec<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>,
}
impl Default for Ros2Uwvm_booster_sdk_bridgeMsgLowCmd {fn default()->Self {Self {
cmd_type:Default::default(),
motor_cmd:Default::default(),
}}}
impl Ros2Uwvm_booster_sdk_bridgeMsgLowCmd {
  pub const MAX_WIRE_BYTES:usize=733;
  pub const CARTESIAN_WIRE_BYTES:u64=733;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
if self.motor_cmd.len()>29{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_cmd {
n=add_size(n,element.wire_size()?)?;
}
if n>733{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.cmd_type=decoded;
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_cmd=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd::read_fields(bytes,at)?;
value.motor_cmd.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_booster_sdk_bridgeMsgLowCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>733{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_booster_sdk_bridge/msg/LowState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_booster_sdk_bridgeMsgLowState {
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Uwvm_booster_sdk_bridgeMsgImuState,
  /// Declared motor_state_parallel; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state_parallel:Vec<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>,
  /// Declared motor_state_serial; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state_serial:Vec<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>,
}
impl Default for Ros2Uwvm_booster_sdk_bridgeMsgLowState {fn default()->Self {Self {
imu_state:Default::default(),
motor_state_parallel:Default::default(),
motor_state_serial:Default::default(),
}}}
impl Ros2Uwvm_booster_sdk_bridgeMsgLowState {
  pub const MAX_WIRE_BYTES:usize=1784;
  pub const CARTESIAN_WIRE_BYTES:u64=1784;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58";
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
let decoded=Ros2Uwvm_booster_sdk_bridgeMsgImuState::read_fields(bytes,at)?;
value.imu_state=decoded;
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_state_parallel=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorState::read_fields(bytes,at)?;
value.motor_state_parallel.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>29{return Err(invalid());}
value.motor_state_serial=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorState::read_fields(bytes,at)?;
value.motor_state_serial.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_booster_sdk_bridgeMsgLowState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1784{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
