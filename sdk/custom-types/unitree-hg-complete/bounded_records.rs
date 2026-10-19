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
/// Complete bounded value for ROS2 unitree_hg/msg/BmsCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgBmsCmd {
  /// Declared cmd; canonical bounds are part of CODEC_IDENTITY.
  pub cmd:u8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u8;40],
}
impl Default for Ros2Unitree_hgMsgBmsCmd {fn default()->Self {Self {
cmd:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgBmsCmd {
  pub const MAX_WIRE_BYTES:usize=41;
  pub const CARTESIAN_WIRE_BYTES:u64=41;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a267c92d22aa04a15f7d68c87b7638b15a368ac973f49a42021dc91f991b13de";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
for element in &self.reserve {
n=add_size(n,1)?;
}
if n>41{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.cmd.to_le_bytes());
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.cmd=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgBmsCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>41{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<41{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/BmsState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgBmsState {
  /// Declared version_high; canonical bounds are part of CODEC_IDENTITY.
  pub version_high:u8,
  /// Declared version_low; canonical bounds are part of CODEC_IDENTITY.
  pub version_low:u8,
  /// Declared fn; canonical bounds are part of CODEC_IDENTITY.
  pub r#fn:u8,
  /// Declared cell_vol; canonical bounds are part of CODEC_IDENTITY.
  pub cell_vol:[u16;40],
  /// Declared bmsvoltage; canonical bounds are part of CODEC_IDENTITY.
  pub bmsvoltage:[u32;3],
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:i32,
  /// Declared soc; canonical bounds are part of CODEC_IDENTITY.
  pub soc:u8,
  /// Declared soh; canonical bounds are part of CODEC_IDENTITY.
  pub soh:u8,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:[i16;12],
  /// Declared cycle; canonical bounds are part of CODEC_IDENTITY.
  pub cycle:u16,
  /// Declared manufacturer_date; canonical bounds are part of CODEC_IDENTITY.
  pub manufacturer_date:u16,
  /// Declared bmsstate; canonical bounds are part of CODEC_IDENTITY.
  pub bmsstate:[u32;5],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;3],
}
impl Default for Ros2Unitree_hgMsgBmsState {fn default()->Self {Self {
version_high:Default::default(),
version_low:Default::default(),
r#fn:Default::default(),
cell_vol:core::array::from_fn(|_|Default::default()),
bmsvoltage:core::array::from_fn(|_|Default::default()),
current:Default::default(),
soc:Default::default(),
soh:Default::default(),
temperature:core::array::from_fn(|_|Default::default()),
cycle:Default::default(),
manufacturer_date:Default::default(),
bmsstate:core::array::from_fn(|_|Default::default()),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgBmsState {
  pub const MAX_WIRE_BYTES:usize=161;
  pub const CARTESIAN_WIRE_BYTES:u64=161;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b7ac163c4ab4786d37a9ec027767258adcfc06637ed8b4ef67e30c517397761d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.cell_vol {
n=add_size(n,2)?;
}
for element in &self.bmsvoltage {
n=add_size(n,4)?;
}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.temperature {
n=add_size(n,2)?;
}
n=add_size(n,2)?;
n=add_size(n,2)?;
for element in &self.bmsstate {
n=add_size(n,4)?;
}
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>161{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.version_high.to_le_bytes());
put(out,at,&self.version_low.to_le_bytes());
put(out,at,&self.r#fn.to_le_bytes());
for element in &self.cell_vol {
put(out,at,&element.to_le_bytes());
}
for element in &self.bmsvoltage {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.soc.to_le_bytes());
put(out,at,&self.soh.to_le_bytes());
for element in &self.temperature {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.cycle.to_le_bytes());
put(out,at,&self.manufacturer_date.to_le_bytes());
for element in &self.bmsstate {
put(out,at,&element.to_le_bytes());
}
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_high=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_low=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.r#fn=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cell_vol[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.bmsvoltage[i]=decoded;
}}
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.current=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.soc=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.soh=decoded;
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.temperature[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cycle=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.manufacturer_date=decoded;
{let count=5usize;if count>5{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.bmsstate[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgBmsState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>161{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<161{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/MotorCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgMotorCmd {
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
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
}
impl Default for Ros2Unitree_hgMsgMotorCmd {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
reserve:Default::default(),
}}}
impl Ros2Unitree_hgMsgMotorCmd {
  pub const MAX_WIRE_BYTES:usize=25;
  pub const CARTESIAN_WIRE_BYTES:u64=25;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f3e472df5d061a1515ac185f87b01f702ef2f2bbeff39e42c6c7088d1da26cbb";
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
put(out,at,&self.reserve.to_le_bytes());
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
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgMotorCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>25{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<25{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/HandCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgHandCmd {
  /// Declared motor_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub motor_cmd:Vec<Ros2Unitree_hgMsgMotorCmd>,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;4],
}
impl Default for Ros2Unitree_hgMsgHandCmd {fn default()->Self {Self {
motor_cmd:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgHandCmd {
  pub const MAX_WIRE_BYTES:usize=895;
  pub const CARTESIAN_WIRE_BYTES:u64=895;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2ebc0e32e831a48e21f0b15f39cb477d999bf3a415e7863713ce37416020a835";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.motor_cmd.len()>35{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_cmd {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>895{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.motor_cmd.len() as u32).to_le_bytes());
for element in &self.motor_cmd {
element.write_fields(out,at);
}
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>35{return Err(invalid());}
value.motor_cmd=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Unitree_hgMsgMotorCmd::read_fields(bytes,at)?;
value.motor_cmd.push(decoded);
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgHandCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>895{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/MotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgMotorState {
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
  pub temperature:[i16;2],
  /// Declared vol; canonical bounds are part of CODEC_IDENTITY.
  pub vol:f32,
  /// Declared sensor; canonical bounds are part of CODEC_IDENTITY.
  pub sensor:[u32;2],
  /// Declared motorstate; canonical bounds are part of CODEC_IDENTITY.
  pub motorstate:u32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;4],
}
impl Default for Ros2Unitree_hgMsgMotorState {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
ddq:Default::default(),
tau_est:Default::default(),
temperature:core::array::from_fn(|_|Default::default()),
vol:Default::default(),
sensor:core::array::from_fn(|_|Default::default()),
motorstate:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgMotorState {
  pub const MAX_WIRE_BYTES:usize=53;
  pub const CARTESIAN_WIRE_BYTES:u64=53;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_65497908fe0136a5c90f11b4a2cda2110507bcc8805f9347ac82be3b32fe5934";
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
for element in &self.temperature {
n=add_size(n,2)?;
}
if !self.vol.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.sensor {
n=add_size(n,4)?;
}
n=add_size(n,4)?;
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>53{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.ddq.to_le_bytes());
put(out,at,&self.tau_est.to_le_bytes());
for element in &self.temperature {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.vol.to_le_bytes());
for element in &self.sensor {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.motorstate.to_le_bytes());
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
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.temperature[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.vol=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sensor[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.motorstate=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>53{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<53{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/PressSensorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgPressSensorState {
  /// Declared pressure; canonical bounds are part of CODEC_IDENTITY.
  pub pressure:[f32;12],
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:[f32;12],
  /// Declared lost; canonical bounds are part of CODEC_IDENTITY.
  pub lost:u32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
}
impl Default for Ros2Unitree_hgMsgPressSensorState {fn default()->Self {Self {
pressure:core::array::from_fn(|_|Default::default()),
temperature:core::array::from_fn(|_|Default::default()),
lost:Default::default(),
reserve:Default::default(),
}}}
impl Ros2Unitree_hgMsgPressSensorState {
  pub const MAX_WIRE_BYTES:usize=104;
  pub const CARTESIAN_WIRE_BYTES:u64=104;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4abc296d4acd21132c0c1d185e3fafd9f0db8836a5c16ed040ebcc6127380fe5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.pressure {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.temperature {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>104{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.pressure {
put(out,at,&element.to_le_bytes());
}
for element in &self.temperature {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.lost.to_le_bytes());
put(out,at,&self.reserve.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pressure[i]=decoded;
}}
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.temperature[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.lost=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgPressSensorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>104{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<104{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/IMUState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgIMUState {
  /// Declared quaternion; canonical bounds are part of CODEC_IDENTITY.
  pub quaternion:[f32;4],
  /// Declared gyroscope; canonical bounds are part of CODEC_IDENTITY.
  pub gyroscope:[f32;3],
  /// Declared accelerometer; canonical bounds are part of CODEC_IDENTITY.
  pub accelerometer:[f32;3],
  /// Declared rpy; canonical bounds are part of CODEC_IDENTITY.
  pub rpy:[f32;3],
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:i16,
}
impl Default for Ros2Unitree_hgMsgIMUState {fn default()->Self {Self {
quaternion:core::array::from_fn(|_|Default::default()),
gyroscope:core::array::from_fn(|_|Default::default()),
accelerometer:core::array::from_fn(|_|Default::default()),
rpy:core::array::from_fn(|_|Default::default()),
temperature:Default::default(),
}}}
impl Ros2Unitree_hgMsgIMUState {
  pub const MAX_WIRE_BYTES:usize=54;
  pub const CARTESIAN_WIRE_BYTES:u64=54;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d83832e8c27273785113c8388f7f22ea1251d6198817255f13a7e542afb26420";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.quaternion {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.gyroscope {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.accelerometer {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.rpy {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
n=add_size(n,2)?;
if n>54{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.quaternion {
put(out,at,&element.to_le_bytes());
}
for element in &self.gyroscope {
put(out,at,&element.to_le_bytes());
}
for element in &self.accelerometer {
put(out,at,&element.to_le_bytes());
}
for element in &self.rpy {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.temperature.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.quaternion[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.gyroscope[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.accelerometer[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rpy[i]=decoded;
}}
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.temperature=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgIMUState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>54{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<54{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/HandState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgHandState {
  /// Declared motor_state; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state:Vec<Ros2Unitree_hgMsgMotorState>,
  /// Declared press_sensor_state; canonical bounds are part of CODEC_IDENTITY.
  pub press_sensor_state:Vec<Ros2Unitree_hgMsgPressSensorState>,
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Unitree_hgMsgIMUState,
  /// Declared power_v; canonical bounds are part of CODEC_IDENTITY.
  pub power_v:f32,
  /// Declared power_a; canonical bounds are part of CODEC_IDENTITY.
  pub power_a:f32,
  /// Declared system_v; canonical bounds are part of CODEC_IDENTITY.
  pub system_v:f32,
  /// Declared device_v; canonical bounds are part of CODEC_IDENTITY.
  pub device_v:f32,
  /// Declared error; canonical bounds are part of CODEC_IDENTITY.
  pub error:[u32;2],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;2],
}
impl Default for Ros2Unitree_hgMsgHandState {fn default()->Self {Self {
motor_state:Default::default(),
press_sensor_state:Default::default(),
imu_state:Default::default(),
power_v:Default::default(),
power_a:Default::default(),
system_v:Default::default(),
device_v:Default::default(),
error:core::array::from_fn(|_|Default::default()),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgHandState {
  pub const MAX_WIRE_BYTES:usize=2365;
  pub const CARTESIAN_WIRE_BYTES:u64=2365;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_5721fdfc155f15ca331dbd398f07cb909196b07e93cabdb6dfbb67b10d4abc4a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.motor_state.len()>35{return Err(invalid());}n=add_size(n,4)?;
for element in &self.motor_state {
n=add_size(n,element.wire_size()?)?;
}
if self.press_sensor_state.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.press_sensor_state {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.imu_state.wire_size()?)?;
if !self.power_v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.power_a.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.system_v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.device_v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.error {
n=add_size(n,4)?;
}
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>2365{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.motor_state.len() as u32).to_le_bytes());
for element in &self.motor_state {
element.write_fields(out,at);
}
put(out,at,&(self.press_sensor_state.len() as u32).to_le_bytes());
for element in &self.press_sensor_state {
element.write_fields(out,at);
}
self.imu_state.write_fields(out,at);
put(out,at,&self.power_v.to_le_bytes());
put(out,at,&self.power_a.to_le_bytes());
put(out,at,&self.system_v.to_le_bytes());
put(out,at,&self.device_v.to_le_bytes());
for element in &self.error {
put(out,at,&element.to_le_bytes());
}
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>35{return Err(invalid());}
value.motor_state=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Unitree_hgMsgMotorState::read_fields(bytes,at)?;
value.motor_state.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.press_sensor_state=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Unitree_hgMsgPressSensorState::read_fields(bytes,at)?;
value.press_sensor_state.push(decoded);
}}
let decoded=Ros2Unitree_hgMsgIMUState::read_fields(bytes,at)?;
value.imu_state=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.power_v=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.power_a=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.system_v=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.device_v=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgHandState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2365{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<94{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/LowCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgLowCmd {
  /// Declared mode_pr; canonical bounds are part of CODEC_IDENTITY.
  pub mode_pr:u8,
  /// Declared mode_machine; canonical bounds are part of CODEC_IDENTITY.
  pub mode_machine:u8,
  /// Declared motor_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub motor_cmd:[Ros2Unitree_hgMsgMotorCmd;35],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;4],
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros2Unitree_hgMsgLowCmd {fn default()->Self {Self {
mode_pr:Default::default(),
mode_machine:Default::default(),
motor_cmd:core::array::from_fn(|_|Default::default()),
reserve:core::array::from_fn(|_|Default::default()),
crc:Default::default(),
}}}
impl Ros2Unitree_hgMsgLowCmd {
  pub const MAX_WIRE_BYTES:usize=897;
  pub const CARTESIAN_WIRE_BYTES:u64=897;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8303ac6e2e3273251662d31daf966a11ced5b5ac10b93aa6253eebc56f4ec461";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.motor_cmd {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.reserve {
n=add_size(n,4)?;
}
n=add_size(n,4)?;
if n>897{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode_pr.to_le_bytes());
put(out,at,&self.mode_machine.to_le_bytes());
for element in &self.motor_cmd {
element.write_fields(out,at);
}
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.crc.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode_pr=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode_machine=decoded;
{let count=35usize;if count>35{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Unitree_hgMsgMotorCmd::read_fields(bytes,at)?;
value.motor_cmd[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgLowCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>897{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<897{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/LowState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgLowState {
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared mode_pr; canonical bounds are part of CODEC_IDENTITY.
  pub mode_pr:u8,
  /// Declared mode_machine; canonical bounds are part of CODEC_IDENTITY.
  pub mode_machine:u8,
  /// Declared tick; canonical bounds are part of CODEC_IDENTITY.
  pub tick:u32,
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Unitree_hgMsgIMUState,
  /// Declared motor_state; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state:[Ros2Unitree_hgMsgMotorState;35],
  /// Declared wireless_remote; canonical bounds are part of CODEC_IDENTITY.
  pub wireless_remote:[u8;40],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;4],
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros2Unitree_hgMsgLowState {fn default()->Self {Self {
version:core::array::from_fn(|_|Default::default()),
mode_pr:Default::default(),
mode_machine:Default::default(),
tick:Default::default(),
imu_state:Default::default(),
motor_state:core::array::from_fn(|_|Default::default()),
wireless_remote:core::array::from_fn(|_|Default::default()),
reserve:core::array::from_fn(|_|Default::default()),
crc:Default::default(),
}}}
impl Ros2Unitree_hgMsgLowState {
  pub const MAX_WIRE_BYTES:usize=1983;
  pub const CARTESIAN_WIRE_BYTES:u64=1983;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7eed2692672f536a476643f1d3a07affc8e5b3272663e3d282c199fff1c2fa1d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,self.imu_state.wire_size()?)?;
for element in &self.motor_state {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.wireless_remote {
n=add_size(n,1)?;
}
for element in &self.reserve {
n=add_size(n,4)?;
}
n=add_size(n,4)?;
if n>1983{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.mode_pr.to_le_bytes());
put(out,at,&self.mode_machine.to_le_bytes());
put(out,at,&self.tick.to_le_bytes());
self.imu_state.write_fields(out,at);
for element in &self.motor_state {
element.write_fields(out,at);
}
for element in &self.wireless_remote {
put(out,at,&element.to_le_bytes());
}
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.crc.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode_pr=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode_machine=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.tick=decoded;
let decoded=Ros2Unitree_hgMsgIMUState::read_fields(bytes,at)?;
value.imu_state=decoded;
{let count=35usize;if count>35{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Unitree_hgMsgMotorState::read_fields(bytes,at)?;
value.motor_state[i]=decoded;
}}
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wireless_remote[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgLowState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1983{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1983{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_hg/msg/MainBoardState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_hgMsgMainBoardState {
  /// Declared fan_state; canonical bounds are part of CODEC_IDENTITY.
  pub fan_state:[u16;6],
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:[i16;6],
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:[f32;6],
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:[u32;6],
}
impl Default for Ros2Unitree_hgMsgMainBoardState {fn default()->Self {Self {
fan_state:core::array::from_fn(|_|Default::default()),
temperature:core::array::from_fn(|_|Default::default()),
value:core::array::from_fn(|_|Default::default()),
state:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_hgMsgMainBoardState {
  pub const MAX_WIRE_BYTES:usize=72;
  pub const CARTESIAN_WIRE_BYTES:u64=72;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2a27a979aadc7bef33a757e169680faa632936c2634ee787f0c9f25cb08535a6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.fan_state {
n=add_size(n,2)?;
}
for element in &self.temperature {
n=add_size(n,2)?;
}
for element in &self.value {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.state {
n=add_size(n,4)?;
}
if n>72{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.fan_state {
put(out,at,&element.to_le_bytes());
}
for element in &self.temperature {
put(out,at,&element.to_le_bytes());
}
for element in &self.value {
put(out,at,&element.to_le_bytes());
}
for element in &self.state {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.fan_state[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.temperature[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.value[i]=decoded;
}}
{let count=6usize;if count>6{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.state[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_hgMsgMainBoardState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>72{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<72{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
