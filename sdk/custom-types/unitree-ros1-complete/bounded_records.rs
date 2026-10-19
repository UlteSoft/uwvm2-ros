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
/// Complete bounded value for ROS1 unitree_legged_msgs/BmsCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsBmsCmd {
  /// Declared off; canonical bounds are part of CODEC_IDENTITY.
  pub off:u8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u8;3],
}
impl Default for Ros1Unitree_legged_msgsBmsCmd {fn default()->Self {Self {
off:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Unitree_legged_msgsBmsCmd {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_75732753b156fff8db3ba7e39541444bed2b71e139389a9b5bfbc49a4fa14928";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
for element in &self.reserve {
n=add_size(n,1)?;
}
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.off.to_le_bytes());
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.off=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsBmsCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/BmsState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsBmsState {
  /// Declared version_h; canonical bounds are part of CODEC_IDENTITY.
  pub version_h:u8,
  /// Declared version_l; canonical bounds are part of CODEC_IDENTITY.
  pub version_l:u8,
  /// Declared bms_status; canonical bounds are part of CODEC_IDENTITY.
  pub bms_status:u8,
  /// Declared SOC; canonical bounds are part of CODEC_IDENTITY.
  pub SOC:u8,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:i32,
  /// Declared cycle; canonical bounds are part of CODEC_IDENTITY.
  pub cycle:u16,
  /// Declared BQ_NTC; canonical bounds are part of CODEC_IDENTITY.
  pub BQ_NTC:[i8;2],
  /// Declared MCU_NTC; canonical bounds are part of CODEC_IDENTITY.
  pub MCU_NTC:[i8;2],
  /// Declared cell_vol; canonical bounds are part of CODEC_IDENTITY.
  pub cell_vol:[u16;10],
}
impl Default for Ros1Unitree_legged_msgsBmsState {fn default()->Self {Self {
version_h:Default::default(),
version_l:Default::default(),
bms_status:Default::default(),
SOC:Default::default(),
current:Default::default(),
cycle:Default::default(),
BQ_NTC:core::array::from_fn(|_|Default::default()),
MCU_NTC:core::array::from_fn(|_|Default::default()),
cell_vol:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Unitree_legged_msgsBmsState {
  pub const MAX_WIRE_BYTES:usize=34;
  pub const CARTESIAN_WIRE_BYTES:u64=34;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ef0f93501f54d0ff760039d86bd1ba1739cb0c888b3a873611679484ccec094a";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,2)?;
for element in &self.BQ_NTC {
n=add_size(n,1)?;
}
for element in &self.MCU_NTC {
n=add_size(n,1)?;
}
for element in &self.cell_vol {
n=add_size(n,2)?;
}
if n>34{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.version_h.to_le_bytes());
put(out,at,&self.version_l.to_le_bytes());
put(out,at,&self.bms_status.to_le_bytes());
put(out,at,&self.SOC.to_le_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.cycle.to_le_bytes());
for element in &self.BQ_NTC {
put(out,at,&element.to_le_bytes());
}
for element in &self.MCU_NTC {
put(out,at,&element.to_le_bytes());
}
for element in &self.cell_vol {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_h=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_l=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.bms_status=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.SOC=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.current=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cycle=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.BQ_NTC[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.MCU_NTC[i]=decoded;
}}
{let count=10usize;if count>10{return Err(invalid());}
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cell_vol[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsBmsState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>34{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<34{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/Cartesian.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsCartesian {
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared z; canonical bounds are part of CODEC_IDENTITY.
  pub z:f32,
}
impl Default for Ros1Unitree_legged_msgsCartesian {fn default()->Self {Self {
x:Default::default(),
y:Default::default(),
z:Default::default(),
}}}
impl Ros1Unitree_legged_msgsCartesian {
  pub const MAX_WIRE_BYTES:usize=12;
  pub const CARTESIAN_WIRE_BYTES:u64=12;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_43c7c62ba2a689751a0a15e326c5c9690a9b1f166b6726a0f803b98d2b3fa722";
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
impl Canonical for Ros1Unitree_legged_msgsCartesian {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>12{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/LED.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsLED {
  /// Declared r; canonical bounds are part of CODEC_IDENTITY.
  pub r:u8,
  /// Declared g; canonical bounds are part of CODEC_IDENTITY.
  pub g:u8,
  /// Declared b; canonical bounds are part of CODEC_IDENTITY.
  pub b:u8,
}
impl Default for Ros1Unitree_legged_msgsLED {fn default()->Self {Self {
r:Default::default(),
g:Default::default(),
b:Default::default(),
}}}
impl Ros1Unitree_legged_msgsLED {
  pub const MAX_WIRE_BYTES:usize=3;
  pub const CARTESIAN_WIRE_BYTES:u64=3;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b4efac59eaf4d3f09c069e4833dd82b5422980f6a9f27db84950243a47164ea5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>3{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.r.to_le_bytes());
put(out,at,&self.g.to_le_bytes());
put(out,at,&self.b.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.r=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.g=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.b=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsLED {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<3{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/HighCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsHighCmd {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared levelFlag; canonical bounds are part of CODEC_IDENTITY.
  pub levelFlag:u8,
  /// Declared frameReserve; canonical bounds are part of CODEC_IDENTITY.
  pub frameReserve:u8,
  /// Declared SN; canonical bounds are part of CODEC_IDENTITY.
  pub SN:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandWidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandWidth:u16,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared gaitType; canonical bounds are part of CODEC_IDENTITY.
  pub gaitType:u8,
  /// Declared speedLevel; canonical bounds are part of CODEC_IDENTITY.
  pub speedLevel:u8,
  /// Declared footRaiseHeight; canonical bounds are part of CODEC_IDENTITY.
  pub footRaiseHeight:f32,
  /// Declared bodyHeight; canonical bounds are part of CODEC_IDENTITY.
  pub bodyHeight:f32,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:[f32;2],
  /// Declared euler; canonical bounds are part of CODEC_IDENTITY.
  pub euler:[f32;3],
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:[f32;2],
  /// Declared yawSpeed; canonical bounds are part of CODEC_IDENTITY.
  pub yawSpeed:f32,
  /// Declared bms; canonical bounds are part of CODEC_IDENTITY.
  pub bms:Ros1Unitree_legged_msgsBmsCmd,
  /// Declared led; canonical bounds are part of CODEC_IDENTITY.
  pub led:[Ros1Unitree_legged_msgsLED;4],
  /// Declared wirelessRemote; canonical bounds are part of CODEC_IDENTITY.
  pub wirelessRemote:[u8;40],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros1Unitree_legged_msgsHighCmd {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
levelFlag:Default::default(),
frameReserve:Default::default(),
SN:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandWidth:Default::default(),
mode:Default::default(),
gaitType:Default::default(),
speedLevel:Default::default(),
footRaiseHeight:Default::default(),
bodyHeight:Default::default(),
position:core::array::from_fn(|_|Default::default()),
euler:core::array::from_fn(|_|Default::default()),
velocity:core::array::from_fn(|_|Default::default()),
yawSpeed:Default::default(),
bms:Default::default(),
led:core::array::from_fn(|_|Default::default()),
wirelessRemote:core::array::from_fn(|_|Default::default()),
reserve:Default::default(),
crc:Default::default(),
}}}
impl Ros1Unitree_legged_msgsHighCmd {
  pub const MAX_WIRE_BYTES:usize=129;
  pub const CARTESIAN_WIRE_BYTES:u64=129;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_ec6ec8937b2629a4441adc4a21eed83fca940b18d46b970d96e05c957ac90b38";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.SN {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.footRaiseHeight.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.bodyHeight.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.euler {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.yawSpeed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.bms.wire_size()?)?;
for element in &self.led {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.wirelessRemote {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>129{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.levelFlag.to_le_bytes());
put(out,at,&self.frameReserve.to_le_bytes());
for element in &self.SN {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandWidth.to_le_bytes());
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.gaitType.to_le_bytes());
put(out,at,&self.speedLevel.to_le_bytes());
put(out,at,&self.footRaiseHeight.to_le_bytes());
put(out,at,&self.bodyHeight.to_le_bytes());
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
for element in &self.euler {
put(out,at,&element.to_le_bytes());
}
for element in &self.velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.yawSpeed.to_le_bytes());
self.bms.write_fields(out,at);
for element in &self.led {
element.write_fields(out,at);
}
for element in &self.wirelessRemote {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.reserve.to_le_bytes());
put(out,at,&self.crc.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.head[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.levelFlag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frameReserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.SN[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandWidth=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gaitType=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speedLevel=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.footRaiseHeight=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.bodyHeight=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.euler[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yawSpeed=decoded;
let decoded=Ros1Unitree_legged_msgsBmsCmd::read_fields(bytes,at)?;
value.bms=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsLED::read_fields(bytes,at)?;
value.led[i]=decoded;
}}
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wirelessRemote[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsHighCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>129{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<129{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/IMU.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsIMU {
  /// Declared quaternion; canonical bounds are part of CODEC_IDENTITY.
  pub quaternion:[f32;4],
  /// Declared gyroscope; canonical bounds are part of CODEC_IDENTITY.
  pub gyroscope:[f32;3],
  /// Declared accelerometer; canonical bounds are part of CODEC_IDENTITY.
  pub accelerometer:[f32;3],
  /// Declared rpy; canonical bounds are part of CODEC_IDENTITY.
  pub rpy:[f32;3],
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:i8,
}
impl Default for Ros1Unitree_legged_msgsIMU {fn default()->Self {Self {
quaternion:core::array::from_fn(|_|Default::default()),
gyroscope:core::array::from_fn(|_|Default::default()),
accelerometer:core::array::from_fn(|_|Default::default()),
rpy:core::array::from_fn(|_|Default::default()),
temperature:Default::default(),
}}}
impl Ros1Unitree_legged_msgsIMU {
  pub const MAX_WIRE_BYTES:usize=53;
  pub const CARTESIAN_WIRE_BYTES:u64=53;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_29d9ec99e676fe8747665da588b78c8ab77a3197e281ba892ab389d2d81fa276";
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
n=add_size(n,1)?;
if n>53{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
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
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.temperature=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsIMU {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>53{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<53{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/MotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsMotorState {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:f32,
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:f32,
  /// Declared ddq; canonical bounds are part of CODEC_IDENTITY.
  pub ddq:f32,
  /// Declared tauEst; canonical bounds are part of CODEC_IDENTITY.
  pub tauEst:f32,
  /// Declared q_raw; canonical bounds are part of CODEC_IDENTITY.
  pub q_raw:f32,
  /// Declared dq_raw; canonical bounds are part of CODEC_IDENTITY.
  pub dq_raw:f32,
  /// Declared ddq_raw; canonical bounds are part of CODEC_IDENTITY.
  pub ddq_raw:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:i8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;2],
}
impl Default for Ros1Unitree_legged_msgsMotorState {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
ddq:Default::default(),
tauEst:Default::default(),
q_raw:Default::default(),
dq_raw:Default::default(),
ddq_raw:Default::default(),
temperature:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Unitree_legged_msgsMotorState {
  pub const MAX_WIRE_BYTES:usize=38;
  pub const CARTESIAN_WIRE_BYTES:u64=38;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_be540cb22db5f08b6b9a8082777c76244e80f3148e9cfd63f510e4b2acdc0abf";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if !self.q.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ddq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tauEst.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.q_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ddq_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>38{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.ddq.to_le_bytes());
put(out,at,&self.tauEst.to_le_bytes());
put(out,at,&self.q_raw.to_le_bytes());
put(out,at,&self.dq_raw.to_le_bytes());
put(out,at,&self.ddq_raw.to_le_bytes());
put(out,at,&self.temperature.to_le_bytes());
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
value.tauEst=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q_raw=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.dq_raw=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.ddq_raw=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.temperature=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>38{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<38{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/HighState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsHighState {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared levelFlag; canonical bounds are part of CODEC_IDENTITY.
  pub levelFlag:u8,
  /// Declared frameReserve; canonical bounds are part of CODEC_IDENTITY.
  pub frameReserve:u8,
  /// Declared SN; canonical bounds are part of CODEC_IDENTITY.
  pub SN:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandWidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandWidth:u16,
  /// Declared imu; canonical bounds are part of CODEC_IDENTITY.
  pub imu:Ros1Unitree_legged_msgsIMU,
  /// Declared motorState; canonical bounds are part of CODEC_IDENTITY.
  pub motorState:[Ros1Unitree_legged_msgsMotorState;20],
  /// Declared bms; canonical bounds are part of CODEC_IDENTITY.
  pub bms:Ros1Unitree_legged_msgsBmsState,
  /// Declared footForce; canonical bounds are part of CODEC_IDENTITY.
  pub footForce:[i16;4],
  /// Declared footForceEst; canonical bounds are part of CODEC_IDENTITY.
  pub footForceEst:[i16;4],
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared progress; canonical bounds are part of CODEC_IDENTITY.
  pub progress:f32,
  /// Declared gaitType; canonical bounds are part of CODEC_IDENTITY.
  pub gaitType:u8,
  /// Declared footRaiseHeight; canonical bounds are part of CODEC_IDENTITY.
  pub footRaiseHeight:f32,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:[f32;3],
  /// Declared bodyHeight; canonical bounds are part of CODEC_IDENTITY.
  pub bodyHeight:f32,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:[f32;3],
  /// Declared yawSpeed; canonical bounds are part of CODEC_IDENTITY.
  pub yawSpeed:f32,
  /// Declared rangeObstacle; canonical bounds are part of CODEC_IDENTITY.
  pub rangeObstacle:[f32;4],
  /// Declared footPosition2Body; canonical bounds are part of CODEC_IDENTITY.
  pub footPosition2Body:[Ros1Unitree_legged_msgsCartesian;4],
  /// Declared footSpeed2Body; canonical bounds are part of CODEC_IDENTITY.
  pub footSpeed2Body:[Ros1Unitree_legged_msgsCartesian;4],
  /// Declared wirelessRemote; canonical bounds are part of CODEC_IDENTITY.
  pub wirelessRemote:[u8;40],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros1Unitree_legged_msgsHighState {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
levelFlag:Default::default(),
frameReserve:Default::default(),
SN:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandWidth:Default::default(),
imu:Default::default(),
motorState:core::array::from_fn(|_|Default::default()),
bms:Default::default(),
footForce:core::array::from_fn(|_|Default::default()),
footForceEst:core::array::from_fn(|_|Default::default()),
mode:Default::default(),
progress:Default::default(),
gaitType:Default::default(),
footRaiseHeight:Default::default(),
position:core::array::from_fn(|_|Default::default()),
bodyHeight:Default::default(),
velocity:core::array::from_fn(|_|Default::default()),
yawSpeed:Default::default(),
rangeObstacle:core::array::from_fn(|_|Default::default()),
footPosition2Body:core::array::from_fn(|_|Default::default()),
footSpeed2Body:core::array::from_fn(|_|Default::default()),
wirelessRemote:core::array::from_fn(|_|Default::default()),
reserve:Default::default(),
crc:Default::default(),
}}}
impl Ros1Unitree_legged_msgsHighState {
  pub const MAX_WIRE_BYTES:usize=1087;
  pub const CARTESIAN_WIRE_BYTES:u64=1087;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_82ee0efaab21b5cb89e99f5c0f5daec583b48dd5feb2dd6a7e09ba9d9110b53d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.SN {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
n=add_size(n,self.imu.wire_size()?)?;
for element in &self.motorState {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.bms.wire_size()?)?;
for element in &self.footForce {
n=add_size(n,2)?;
}
for element in &self.footForceEst {
n=add_size(n,2)?;
}
n=add_size(n,1)?;
if !self.progress.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.footRaiseHeight.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.bodyHeight.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.yawSpeed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.rangeObstacle {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.footPosition2Body {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.footSpeed2Body {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.wirelessRemote {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>1087{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.levelFlag.to_le_bytes());
put(out,at,&self.frameReserve.to_le_bytes());
for element in &self.SN {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandWidth.to_le_bytes());
self.imu.write_fields(out,at);
for element in &self.motorState {
element.write_fields(out,at);
}
self.bms.write_fields(out,at);
for element in &self.footForce {
put(out,at,&element.to_le_bytes());
}
for element in &self.footForceEst {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.progress.to_le_bytes());
put(out,at,&self.gaitType.to_le_bytes());
put(out,at,&self.footRaiseHeight.to_le_bytes());
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bodyHeight.to_le_bytes());
for element in &self.velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.yawSpeed.to_le_bytes());
for element in &self.rangeObstacle {
put(out,at,&element.to_le_bytes());
}
for element in &self.footPosition2Body {
element.write_fields(out,at);
}
for element in &self.footSpeed2Body {
element.write_fields(out,at);
}
for element in &self.wirelessRemote {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.reserve.to_le_bytes());
put(out,at,&self.crc.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.head[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.levelFlag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frameReserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.SN[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandWidth=decoded;
let decoded=Ros1Unitree_legged_msgsIMU::read_fields(bytes,at)?;
value.imu=decoded;
{let count=20usize;if count>20{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsMotorState::read_fields(bytes,at)?;
value.motorState[i]=decoded;
}}
let decoded=Ros1Unitree_legged_msgsBmsState::read_fields(bytes,at)?;
value.bms=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.footForce[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.footForceEst[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.progress=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gaitType=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.footRaiseHeight=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.bodyHeight=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yawSpeed=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.rangeObstacle[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.footPosition2Body[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.footSpeed2Body[i]=decoded;
}}
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wirelessRemote[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsHighState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1087{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1087{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/MotorCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsMotorCmd {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:f32,
  /// Declared dq; canonical bounds are part of CODEC_IDENTITY.
  pub dq:f32,
  /// Declared tau; canonical bounds are part of CODEC_IDENTITY.
  pub tau:f32,
  /// Declared Kp; canonical bounds are part of CODEC_IDENTITY.
  pub Kp:f32,
  /// Declared Kd; canonical bounds are part of CODEC_IDENTITY.
  pub Kd:f32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;3],
}
impl Default for Ros1Unitree_legged_msgsMotorCmd {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
tau:Default::default(),
Kp:Default::default(),
Kd:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Unitree_legged_msgsMotorCmd {
  pub const MAX_WIRE_BYTES:usize=33;
  pub const CARTESIAN_WIRE_BYTES:u64=33;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_632b25f209e72857182d92752aa72e3ab47a45e895cc352ee6dc05f1c1168083";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if !self.q.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tau.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.Kp.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.Kd.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>33{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.tau.to_le_bytes());
put(out,at,&self.Kp.to_le_bytes());
put(out,at,&self.Kd.to_le_bytes());
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
value.tau=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.Kp=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.Kd=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsMotorCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>33{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<33{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/LowCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsLowCmd {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared levelFlag; canonical bounds are part of CODEC_IDENTITY.
  pub levelFlag:u8,
  /// Declared frameReserve; canonical bounds are part of CODEC_IDENTITY.
  pub frameReserve:u8,
  /// Declared SN; canonical bounds are part of CODEC_IDENTITY.
  pub SN:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandWidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandWidth:u16,
  /// Declared motorCmd; canonical bounds are part of CODEC_IDENTITY.
  pub motorCmd:[Ros1Unitree_legged_msgsMotorCmd;20],
  /// Declared bms; canonical bounds are part of CODEC_IDENTITY.
  pub bms:Ros1Unitree_legged_msgsBmsCmd,
  /// Declared wirelessRemote; canonical bounds are part of CODEC_IDENTITY.
  pub wirelessRemote:[u8;40],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros1Unitree_legged_msgsLowCmd {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
levelFlag:Default::default(),
frameReserve:Default::default(),
SN:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandWidth:Default::default(),
motorCmd:core::array::from_fn(|_|Default::default()),
bms:Default::default(),
wirelessRemote:core::array::from_fn(|_|Default::default()),
reserve:Default::default(),
crc:Default::default(),
}}}
impl Ros1Unitree_legged_msgsLowCmd {
  pub const MAX_WIRE_BYTES:usize=734;
  pub const CARTESIAN_WIRE_BYTES:u64=734;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_11d3ac7c497f01fbf6624c39ec71fa613c462cd62acb20c1f263cdde672e9181";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.SN {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
for element in &self.motorCmd {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.bms.wire_size()?)?;
for element in &self.wirelessRemote {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>734{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.levelFlag.to_le_bytes());
put(out,at,&self.frameReserve.to_le_bytes());
for element in &self.SN {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandWidth.to_le_bytes());
for element in &self.motorCmd {
element.write_fields(out,at);
}
self.bms.write_fields(out,at);
for element in &self.wirelessRemote {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.reserve.to_le_bytes());
put(out,at,&self.crc.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.head[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.levelFlag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frameReserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.SN[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandWidth=decoded;
{let count=20usize;if count>20{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsMotorCmd::read_fields(bytes,at)?;
value.motorCmd[i]=decoded;
}}
let decoded=Ros1Unitree_legged_msgsBmsCmd::read_fields(bytes,at)?;
value.bms=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wirelessRemote[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsLowCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>734{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<734{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 unitree_legged_msgs/LowState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Unitree_legged_msgsLowState {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared levelFlag; canonical bounds are part of CODEC_IDENTITY.
  pub levelFlag:u8,
  /// Declared frameReserve; canonical bounds are part of CODEC_IDENTITY.
  pub frameReserve:u8,
  /// Declared SN; canonical bounds are part of CODEC_IDENTITY.
  pub SN:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandWidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandWidth:u16,
  /// Declared imu; canonical bounds are part of CODEC_IDENTITY.
  pub imu:Ros1Unitree_legged_msgsIMU,
  /// Declared motorState; canonical bounds are part of CODEC_IDENTITY.
  pub motorState:[Ros1Unitree_legged_msgsMotorState;20],
  /// Declared bms; canonical bounds are part of CODEC_IDENTITY.
  pub bms:Ros1Unitree_legged_msgsBmsState,
  /// Declared footForce; canonical bounds are part of CODEC_IDENTITY.
  pub footForce:[i16;4],
  /// Declared footForceEst; canonical bounds are part of CODEC_IDENTITY.
  pub footForceEst:[i16;4],
  /// Declared tick; canonical bounds are part of CODEC_IDENTITY.
  pub tick:u32,
  /// Declared wirelessRemote; canonical bounds are part of CODEC_IDENTITY.
  pub wirelessRemote:[u8;40],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
  /// Declared eeForceRaw; canonical bounds are part of CODEC_IDENTITY.
  pub eeForceRaw:[Ros1Unitree_legged_msgsCartesian;4],
  /// Declared eeForce; canonical bounds are part of CODEC_IDENTITY.
  pub eeForce:[Ros1Unitree_legged_msgsCartesian;4],
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:Ros1Unitree_legged_msgsCartesian,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:Ros1Unitree_legged_msgsCartesian,
  /// Declared velocity_w; canonical bounds are part of CODEC_IDENTITY.
  pub velocity_w:Ros1Unitree_legged_msgsCartesian,
}
impl Default for Ros1Unitree_legged_msgsLowState {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
levelFlag:Default::default(),
frameReserve:Default::default(),
SN:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandWidth:Default::default(),
imu:Default::default(),
motorState:core::array::from_fn(|_|Default::default()),
bms:Default::default(),
footForce:core::array::from_fn(|_|Default::default()),
footForceEst:core::array::from_fn(|_|Default::default()),
tick:Default::default(),
wirelessRemote:core::array::from_fn(|_|Default::default()),
reserve:Default::default(),
crc:Default::default(),
eeForceRaw:core::array::from_fn(|_|Default::default()),
eeForce:core::array::from_fn(|_|Default::default()),
position:Default::default(),
velocity:Default::default(),
velocity_w:Default::default(),
}}}
impl Ros1Unitree_legged_msgsLowState {
  pub const MAX_WIRE_BYTES:usize=1069;
  pub const CARTESIAN_WIRE_BYTES:u64=1069;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_37425ed49c0eebd762b6084ee413c7706fc43defc524414d3f50cdd2d25552c0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.SN {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
n=add_size(n,self.imu.wire_size()?)?;
for element in &self.motorState {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.bms.wire_size()?)?;
for element in &self.footForce {
n=add_size(n,2)?;
}
for element in &self.footForceEst {
n=add_size(n,2)?;
}
n=add_size(n,4)?;
for element in &self.wirelessRemote {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
for element in &self.eeForceRaw {
n=add_size(n,element.wire_size()?)?;
}
for element in &self.eeForce {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.position.wire_size()?)?;
n=add_size(n,self.velocity.wire_size()?)?;
n=add_size(n,self.velocity_w.wire_size()?)?;
if n>1069{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.levelFlag.to_le_bytes());
put(out,at,&self.frameReserve.to_le_bytes());
for element in &self.SN {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandWidth.to_le_bytes());
self.imu.write_fields(out,at);
for element in &self.motorState {
element.write_fields(out,at);
}
self.bms.write_fields(out,at);
for element in &self.footForce {
put(out,at,&element.to_le_bytes());
}
for element in &self.footForceEst {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.tick.to_le_bytes());
for element in &self.wirelessRemote {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.reserve.to_le_bytes());
put(out,at,&self.crc.to_le_bytes());
for element in &self.eeForceRaw {
element.write_fields(out,at);
}
for element in &self.eeForce {
element.write_fields(out,at);
}
self.position.write_fields(out,at);
self.velocity.write_fields(out,at);
self.velocity_w.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.head[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.levelFlag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frameReserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.SN[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandWidth=decoded;
let decoded=Ros1Unitree_legged_msgsIMU::read_fields(bytes,at)?;
value.imu=decoded;
{let count=20usize;if count>20{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsMotorState::read_fields(bytes,at)?;
value.motorState[i]=decoded;
}}
let decoded=Ros1Unitree_legged_msgsBmsState::read_fields(bytes,at)?;
value.bms=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.footForce[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.footForceEst[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.tick=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wirelessRemote[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.eeForceRaw[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.eeForce[i]=decoded;
}}
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.position=decoded;
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.velocity=decoded;
let decoded=Ros1Unitree_legged_msgsCartesian::read_fields(bytes,at)?;
value.velocity_w=decoded;
Ok(value)}
}
impl Canonical for Ros1Unitree_legged_msgsLowState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1069{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1069{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
