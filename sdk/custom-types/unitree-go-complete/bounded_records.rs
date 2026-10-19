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
/// Complete bounded value for ROS2 unitree_go/msg/AudioData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgAudioData {
  /// Declared time_frame; canonical bounds are part of CODEC_IDENTITY.
  pub time_frame:u64,
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:Vec<u8>,
}
impl Default for Ros2Unitree_goMsgAudioData {fn default()->Self {Self {
time_frame:Default::default(),
data:Default::default(),
}}}
impl Ros2Unitree_goMsgAudioData {
  pub const MAX_WIRE_BYTES:usize=2060;
  pub const CARTESIAN_WIRE_BYTES:u64=2060;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_aa14877a076c7fc6b662efa965cc8a75f42b3f84990a078c7c3c5def01d2c9fe";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.data.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.data {
n=add_size(n,1)?;
}
if n>2060{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.time_frame.to_le_bytes());
put(out,at,&(self.data.len() as u32).to_le_bytes());
for element in &self.data {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.time_frame=decoded;
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.data=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.data.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgAudioData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>2060{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/BmsCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgBmsCmd {
  /// Declared off; canonical bounds are part of CODEC_IDENTITY.
  pub off:u8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u8;3],
}
impl Default for Ros2Unitree_goMsgBmsCmd {fn default()->Self {Self {
off:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgBmsCmd {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d212c1ab0c92d64801a3b10c3a4d79517ea24a830db3d85ca2bb18e1e0ea5339";
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
impl Canonical for Ros2Unitree_goMsgBmsCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/BmsState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgBmsState {
  /// Declared version_high; canonical bounds are part of CODEC_IDENTITY.
  pub version_high:u8,
  /// Declared version_low; canonical bounds are part of CODEC_IDENTITY.
  pub version_low:u8,
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:u8,
  /// Declared soc; canonical bounds are part of CODEC_IDENTITY.
  pub soc:u8,
  /// Declared current; canonical bounds are part of CODEC_IDENTITY.
  pub current:i32,
  /// Declared cycle; canonical bounds are part of CODEC_IDENTITY.
  pub cycle:u16,
  /// Declared bq_ntc; canonical bounds are part of CODEC_IDENTITY.
  pub bq_ntc:[i8;2],
  /// Declared mcu_ntc; canonical bounds are part of CODEC_IDENTITY.
  pub mcu_ntc:[i8;2],
  /// Declared cell_vol; canonical bounds are part of CODEC_IDENTITY.
  pub cell_vol:[u16;15],
}
impl Default for Ros2Unitree_goMsgBmsState {fn default()->Self {Self {
version_high:Default::default(),
version_low:Default::default(),
status:Default::default(),
soc:Default::default(),
current:Default::default(),
cycle:Default::default(),
bq_ntc:core::array::from_fn(|_|Default::default()),
mcu_ntc:core::array::from_fn(|_|Default::default()),
cell_vol:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgBmsState {
  pub const MAX_WIRE_BYTES:usize=44;
  pub const CARTESIAN_WIRE_BYTES:u64=44;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_7e4d5e3c0a2eddddf39c68b73bff5f7721e1623b0a9322dce23d3e551a07f5b0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,2)?;
for element in &self.bq_ntc {
n=add_size(n,1)?;
}
for element in &self.mcu_ntc {
n=add_size(n,1)?;
}
for element in &self.cell_vol {
n=add_size(n,2)?;
}
if n>44{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.version_high.to_le_bytes());
put(out,at,&self.version_low.to_le_bytes());
put(out,at,&self.status.to_le_bytes());
put(out,at,&self.soc.to_le_bytes());
put(out,at,&self.current.to_le_bytes());
put(out,at,&self.cycle.to_le_bytes());
for element in &self.bq_ntc {
put(out,at,&element.to_le_bytes());
}
for element in &self.mcu_ntc {
put(out,at,&element.to_le_bytes());
}
for element in &self.cell_vol {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_high=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version_low=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.soc=decoded;
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.current=decoded;
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cycle=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.bq_ntc[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mcu_ntc[i]=decoded;
}}
{let count=15usize;if count>15{return Err(invalid());}
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.cell_vol[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgBmsState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>44{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<44{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/Error.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgError {
  /// Declared source; canonical bounds are part of CODEC_IDENTITY.
  pub source:u32,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:u32,
}
impl Default for Ros2Unitree_goMsgError {fn default()->Self {Self {
source:Default::default(),
state:Default::default(),
}}}
impl Ros2Unitree_goMsgError {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_c3e572e08288b6ed0b32575e645fe546f92b5091d62e57db01ec00d27f5e4afd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>8{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.source.to_le_bytes());
put(out,at,&self.state.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.source=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.state=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgError {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/Go2FrontVideoData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgGo2FrontVideoData {
  /// Declared time_frame; canonical bounds are part of CODEC_IDENTITY.
  pub time_frame:u64,
  /// Declared video720p; canonical bounds are part of CODEC_IDENTITY.
  pub video720p:Vec<u8>,
  /// Declared video360p; canonical bounds are part of CODEC_IDENTITY.
  pub video360p:Vec<u8>,
  /// Declared video180p; canonical bounds are part of CODEC_IDENTITY.
  pub video180p:Vec<u8>,
}
impl Default for Ros2Unitree_goMsgGo2FrontVideoData {fn default()->Self {Self {
time_frame:Default::default(),
video720p:Default::default(),
video360p:Default::default(),
video180p:Default::default(),
}}}
impl Ros2Unitree_goMsgGo2FrontVideoData {
  pub const MAX_WIRE_BYTES:usize=3092;
  pub const CARTESIAN_WIRE_BYTES:u64=3092;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_322b6bd4e563436838fe6a05174d173c0d95eb48108b889dca4f61ce6cc71c60";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.video720p.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.video720p {
n=add_size(n,1)?;
}
if self.video360p.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.video360p {
n=add_size(n,1)?;
}
if self.video180p.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.video180p {
n=add_size(n,1)?;
}
if n>3092{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.time_frame.to_le_bytes());
put(out,at,&(self.video720p.len() as u32).to_le_bytes());
for element in &self.video720p {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.video360p.len() as u32).to_le_bytes());
for element in &self.video360p {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.video180p.len() as u32).to_le_bytes());
for element in &self.video180p {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.time_frame=decoded;
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.video720p=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.video720p.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.video360p=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.video360p.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.video180p=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.video180p.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgGo2FrontVideoData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>3092{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/HeightMap.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgHeightMap {
  /// Declared stamp; canonical bounds are part of CODEC_IDENTITY.
  pub stamp:f64,
  /// Declared frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub frame_id:String,
  /// Declared resolution; canonical bounds are part of CODEC_IDENTITY.
  pub resolution:f32,
  /// Declared width; canonical bounds are part of CODEC_IDENTITY.
  pub width:u32,
  /// Declared height; canonical bounds are part of CODEC_IDENTITY.
  pub height:u32,
  /// Declared origin; canonical bounds are part of CODEC_IDENTITY.
  pub origin:[f32;2],
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:Vec<f32>,
}
impl Default for Ros2Unitree_goMsgHeightMap {fn default()->Self {Self {
stamp:Default::default(),
frame_id:Default::default(),
resolution:Default::default(),
width:Default::default(),
height:Default::default(),
origin:core::array::from_fn(|_|Default::default()),
data:Default::default(),
}}}
impl Ros2Unitree_goMsgHeightMap {
  pub const MAX_WIRE_BYTES:usize=8356;
  pub const CARTESIAN_WIRE_BYTES:u64=8356;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_f4aed8c4e1ba382c418ca83a10ae617355f1169757128be8b7cca46611bb0012";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.stamp.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if self.frame_id.len()>128||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if !self.resolution.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
for element in &self.origin {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.data.len()>2048{return Err(invalid());}n=add_size(n,4)?;
for element in &self.data {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>8356{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.stamp.to_le_bytes());
put(out,at,&(self.frame_id.len() as u32).to_le_bytes());put(out,at,self.frame_id.as_bytes());
put(out,at,&self.resolution.to_le_bytes());
put(out,at,&self.width.to_le_bytes());
put(out,at,&self.height.to_le_bytes());
for element in &self.origin {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.data.len() as u32).to_le_bytes());
for element in &self.data {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.stamp=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.resolution=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.width=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.height=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.origin[i]=decoded;
}}
{let count=read_count(bytes,at)?;if count>2048{return Err(invalid());}
value.data=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.data.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgHeightMap {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8356{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<36{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/IMUState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgIMUState {
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
impl Default for Ros2Unitree_goMsgIMUState {fn default()->Self {Self {
quaternion:core::array::from_fn(|_|Default::default()),
gyroscope:core::array::from_fn(|_|Default::default()),
accelerometer:core::array::from_fn(|_|Default::default()),
rpy:core::array::from_fn(|_|Default::default()),
temperature:Default::default(),
}}}
impl Ros2Unitree_goMsgIMUState {
  pub const MAX_WIRE_BYTES:usize=53;
  pub const CARTESIAN_WIRE_BYTES:u64=53;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5326e97c3bfaa06962ad1989d1aa13fa0e60af15d39d9ef6da6ac0a37261bbc";
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
impl Canonical for Ros2Unitree_goMsgIMUState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>53{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<53{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/InterfaceConfig.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgInterfaceConfig {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:u8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u8;2],
}
impl Default for Ros2Unitree_goMsgInterfaceConfig {fn default()->Self {Self {
mode:Default::default(),
value:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgInterfaceConfig {
  pub const MAX_WIRE_BYTES:usize=4;
  pub const CARTESIAN_WIRE_BYTES:u64=4;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9b96ed6672f6a3e039edb28dec8384cd195116dda929d5ba7be85ef7e7668110";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.reserve {
n=add_size(n,1)?;
}
if n>4{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.value.to_le_bytes());
for element in &self.reserve {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgInterfaceConfig {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/LidarState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgLidarState {
  /// Declared stamp; canonical bounds are part of CODEC_IDENTITY.
  pub stamp:f64,
  /// Declared firmware_version; canonical bounds are part of CODEC_IDENTITY.
  pub firmware_version:String,
  /// Declared software_version; canonical bounds are part of CODEC_IDENTITY.
  pub software_version:String,
  /// Declared sdk_version; canonical bounds are part of CODEC_IDENTITY.
  pub sdk_version:String,
  /// Declared sys_rotation_speed; canonical bounds are part of CODEC_IDENTITY.
  pub sys_rotation_speed:f32,
  /// Declared com_rotation_speed; canonical bounds are part of CODEC_IDENTITY.
  pub com_rotation_speed:f32,
  /// Declared error_state; canonical bounds are part of CODEC_IDENTITY.
  pub error_state:u8,
  /// Declared cloud_frequency; canonical bounds are part of CODEC_IDENTITY.
  pub cloud_frequency:f32,
  /// Declared cloud_packet_loss_rate; canonical bounds are part of CODEC_IDENTITY.
  pub cloud_packet_loss_rate:f32,
  /// Declared cloud_size; canonical bounds are part of CODEC_IDENTITY.
  pub cloud_size:u32,
  /// Declared cloud_scan_num; canonical bounds are part of CODEC_IDENTITY.
  pub cloud_scan_num:u32,
  /// Declared imu_frequency; canonical bounds are part of CODEC_IDENTITY.
  pub imu_frequency:f32,
  /// Declared imu_packet_loss_rate; canonical bounds are part of CODEC_IDENTITY.
  pub imu_packet_loss_rate:f32,
  /// Declared imu_rpy; canonical bounds are part of CODEC_IDENTITY.
  pub imu_rpy:[f32;3],
  /// Declared serial_recv_stamp; canonical bounds are part of CODEC_IDENTITY.
  pub serial_recv_stamp:f64,
  /// Declared serial_buffer_size; canonical bounds are part of CODEC_IDENTITY.
  pub serial_buffer_size:u32,
  /// Declared serial_buffer_read; canonical bounds are part of CODEC_IDENTITY.
  pub serial_buffer_read:u32,
}
impl Default for Ros2Unitree_goMsgLidarState {fn default()->Self {Self {
stamp:Default::default(),
firmware_version:Default::default(),
software_version:Default::default(),
sdk_version:Default::default(),
sys_rotation_speed:Default::default(),
com_rotation_speed:Default::default(),
error_state:Default::default(),
cloud_frequency:Default::default(),
cloud_packet_loss_rate:Default::default(),
cloud_size:Default::default(),
cloud_scan_num:Default::default(),
imu_frequency:Default::default(),
imu_packet_loss_rate:Default::default(),
imu_rpy:core::array::from_fn(|_|Default::default()),
serial_recv_stamp:Default::default(),
serial_buffer_size:Default::default(),
serial_buffer_read:Default::default(),
}}}
impl Ros2Unitree_goMsgLidarState {
  pub const MAX_WIRE_BYTES:usize=465;
  pub const CARTESIAN_WIRE_BYTES:u64=465;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_edc9253eb66e466ae5a893b830b0bc5c9102ef18d2bbed7f7d9ead67bcb1a8e6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.stamp.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
if self.firmware_version.len()>128||self.firmware_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.firmware_version.len())?;
if self.software_version.len()>128||self.software_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.software_version.len())?;
if self.sdk_version.len()>128||self.sdk_version.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.sdk_version.len())?;
if !self.sys_rotation_speed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.com_rotation_speed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.cloud_frequency.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.cloud_packet_loss_rate.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if !self.imu_frequency.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.imu_packet_loss_rate.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.imu_rpy {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.serial_recv_stamp.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>465{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.stamp.to_le_bytes());
put(out,at,&(self.firmware_version.len() as u32).to_le_bytes());put(out,at,self.firmware_version.as_bytes());
put(out,at,&(self.software_version.len() as u32).to_le_bytes());put(out,at,self.software_version.as_bytes());
put(out,at,&(self.sdk_version.len() as u32).to_le_bytes());put(out,at,self.sdk_version.as_bytes());
put(out,at,&self.sys_rotation_speed.to_le_bytes());
put(out,at,&self.com_rotation_speed.to_le_bytes());
put(out,at,&self.error_state.to_le_bytes());
put(out,at,&self.cloud_frequency.to_le_bytes());
put(out,at,&self.cloud_packet_loss_rate.to_le_bytes());
put(out,at,&self.cloud_size.to_le_bytes());
put(out,at,&self.cloud_scan_num.to_le_bytes());
put(out,at,&self.imu_frequency.to_le_bytes());
put(out,at,&self.imu_packet_loss_rate.to_le_bytes());
for element in &self.imu_rpy {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.serial_recv_stamp.to_le_bytes());
put(out,at,&self.serial_buffer_size.to_le_bytes());
put(out,at,&self.serial_buffer_read.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.stamp=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.firmware_version=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.software_version=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.sdk_version=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.sys_rotation_speed=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.com_rotation_speed=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.error_state=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cloud_frequency=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.cloud_packet_loss_rate=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.cloud_size=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.cloud_scan_num=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.imu_frequency=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.imu_packet_loss_rate=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.imu_rpy[i]=decoded;
}}
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.serial_recv_stamp=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.serial_buffer_size=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.serial_buffer_read=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgLidarState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>465{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<81{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/MotorCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgMotorCmd {
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
  pub reserve:[u32;3],
}
impl Default for Ros2Unitree_goMsgMotorCmd {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgMotorCmd {
  pub const MAX_WIRE_BYTES:usize=33;
  pub const CARTESIAN_WIRE_BYTES:u64=33;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_e5b88c02a6d339ae4ae2706678a3d82f89904f024b6068f4fd174e1e045b0989";
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
put(out,at,&self.kp.to_le_bytes());
put(out,at,&self.kd.to_le_bytes());
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
value.kp=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kd=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgMotorCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>33{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<33{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/LowCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgLowCmd {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared level_flag; canonical bounds are part of CODEC_IDENTITY.
  pub level_flag:u8,
  /// Declared frame_reserve; canonical bounds are part of CODEC_IDENTITY.
  pub frame_reserve:u8,
  /// Declared sn; canonical bounds are part of CODEC_IDENTITY.
  pub sn:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandwidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandwidth:u16,
  /// Declared motor_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub motor_cmd:[Ros2Unitree_goMsgMotorCmd;20],
  /// Declared bms_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub bms_cmd:Ros2Unitree_goMsgBmsCmd,
  /// Declared wireless_remote; canonical bounds are part of CODEC_IDENTITY.
  pub wireless_remote:[u8;40],
  /// Declared led; canonical bounds are part of CODEC_IDENTITY.
  pub led:[u8;12],
  /// Declared fan; canonical bounds are part of CODEC_IDENTITY.
  pub fan:[u8;2],
  /// Declared gpio; canonical bounds are part of CODEC_IDENTITY.
  pub gpio:u8,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros2Unitree_goMsgLowCmd {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
level_flag:Default::default(),
frame_reserve:Default::default(),
sn:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandwidth:Default::default(),
motor_cmd:core::array::from_fn(|_|Default::default()),
bms_cmd:Default::default(),
wireless_remote:core::array::from_fn(|_|Default::default()),
led:core::array::from_fn(|_|Default::default()),
fan:core::array::from_fn(|_|Default::default()),
gpio:Default::default(),
reserve:Default::default(),
crc:Default::default(),
}}}
impl Ros2Unitree_goMsgLowCmd {
  pub const MAX_WIRE_BYTES:usize=749;
  pub const CARTESIAN_WIRE_BYTES:u64=749;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_efd061d06d38ecc686edb95d1a891c26860b16da1e35747fc97eeaf4a5d93d63";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.sn {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
for element in &self.motor_cmd {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.bms_cmd.wire_size()?)?;
for element in &self.wireless_remote {
n=add_size(n,1)?;
}
for element in &self.led {
n=add_size(n,1)?;
}
for element in &self.fan {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>749{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.level_flag.to_le_bytes());
put(out,at,&self.frame_reserve.to_le_bytes());
for element in &self.sn {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandwidth.to_le_bytes());
for element in &self.motor_cmd {
element.write_fields(out,at);
}
self.bms_cmd.write_fields(out,at);
for element in &self.wireless_remote {
put(out,at,&element.to_le_bytes());
}
for element in &self.led {
put(out,at,&element.to_le_bytes());
}
for element in &self.fan {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.gpio.to_le_bytes());
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
value.level_flag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frame_reserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sn[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandwidth=decoded;
{let count=20usize;if count>20{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Unitree_goMsgMotorCmd::read_fields(bytes,at)?;
value.motor_cmd[i]=decoded;
}}
let decoded=Ros2Unitree_goMsgBmsCmd::read_fields(bytes,at)?;
value.bms_cmd=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wireless_remote[i]=decoded;
}}
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.led[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.fan[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gpio=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgLowCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>749{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<749{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/MotorState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgMotorState {
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
  /// Declared q_raw; canonical bounds are part of CODEC_IDENTITY.
  pub q_raw:f32,
  /// Declared dq_raw; canonical bounds are part of CODEC_IDENTITY.
  pub dq_raw:f32,
  /// Declared ddq_raw; canonical bounds are part of CODEC_IDENTITY.
  pub ddq_raw:f32,
  /// Declared temperature; canonical bounds are part of CODEC_IDENTITY.
  pub temperature:i8,
  /// Declared lost; canonical bounds are part of CODEC_IDENTITY.
  pub lost:u32,
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:[u32;2],
}
impl Default for Ros2Unitree_goMsgMotorState {fn default()->Self {Self {
mode:Default::default(),
q:Default::default(),
dq:Default::default(),
ddq:Default::default(),
tau_est:Default::default(),
q_raw:Default::default(),
dq_raw:Default::default(),
ddq_raw:Default::default(),
temperature:Default::default(),
lost:Default::default(),
reserve:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgMotorState {
  pub const MAX_WIRE_BYTES:usize=42;
  pub const CARTESIAN_WIRE_BYTES:u64=42;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b0b5e638b9fd9eec6d0019422d417f80c60e61d47f4750fe9adac8c5084a2a62";
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
if !self.q_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.dq_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ddq_raw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,4)?;
for element in &self.reserve {
n=add_size(n,4)?;
}
if n>42{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.q.to_le_bytes());
put(out,at,&self.dq.to_le_bytes());
put(out,at,&self.ddq.to_le_bytes());
put(out,at,&self.tau_est.to_le_bytes());
put(out,at,&self.q_raw.to_le_bytes());
put(out,at,&self.dq_raw.to_le_bytes());
put(out,at,&self.ddq_raw.to_le_bytes());
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
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.lost=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgMotorState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>42{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<42{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/LowState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgLowState {
  /// Declared head; canonical bounds are part of CODEC_IDENTITY.
  pub head:[u8;2],
  /// Declared level_flag; canonical bounds are part of CODEC_IDENTITY.
  pub level_flag:u8,
  /// Declared frame_reserve; canonical bounds are part of CODEC_IDENTITY.
  pub frame_reserve:u8,
  /// Declared sn; canonical bounds are part of CODEC_IDENTITY.
  pub sn:[u32;2],
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u32;2],
  /// Declared bandwidth; canonical bounds are part of CODEC_IDENTITY.
  pub bandwidth:u16,
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Unitree_goMsgIMUState,
  /// Declared motor_state; canonical bounds are part of CODEC_IDENTITY.
  pub motor_state:[Ros2Unitree_goMsgMotorState;20],
  /// Declared bms_state; canonical bounds are part of CODEC_IDENTITY.
  pub bms_state:Ros2Unitree_goMsgBmsState,
  /// Declared foot_force; canonical bounds are part of CODEC_IDENTITY.
  pub foot_force:[i16;4],
  /// Declared foot_force_est; canonical bounds are part of CODEC_IDENTITY.
  pub foot_force_est:[i16;4],
  /// Declared tick; canonical bounds are part of CODEC_IDENTITY.
  pub tick:u32,
  /// Declared wireless_remote; canonical bounds are part of CODEC_IDENTITY.
  pub wireless_remote:[u8;40],
  /// Declared bit_flag; canonical bounds are part of CODEC_IDENTITY.
  pub bit_flag:u8,
  /// Declared adc_reel; canonical bounds are part of CODEC_IDENTITY.
  pub adc_reel:f32,
  /// Declared temperature_ntc1; canonical bounds are part of CODEC_IDENTITY.
  pub temperature_ntc1:i8,
  /// Declared temperature_ntc2; canonical bounds are part of CODEC_IDENTITY.
  pub temperature_ntc2:i8,
  /// Declared power_v; canonical bounds are part of CODEC_IDENTITY.
  pub power_v:f32,
  /// Declared power_a; canonical bounds are part of CODEC_IDENTITY.
  pub power_a:f32,
  /// Declared fan_frequency; canonical bounds are part of CODEC_IDENTITY.
  pub fan_frequency:[u16;4],
  /// Declared reserve; canonical bounds are part of CODEC_IDENTITY.
  pub reserve:u32,
  /// Declared crc; canonical bounds are part of CODEC_IDENTITY.
  pub crc:u32,
}
impl Default for Ros2Unitree_goMsgLowState {fn default()->Self {Self {
head:core::array::from_fn(|_|Default::default()),
level_flag:Default::default(),
frame_reserve:Default::default(),
sn:core::array::from_fn(|_|Default::default()),
version:core::array::from_fn(|_|Default::default()),
bandwidth:Default::default(),
imu_state:Default::default(),
motor_state:core::array::from_fn(|_|Default::default()),
bms_state:Default::default(),
foot_force:core::array::from_fn(|_|Default::default()),
foot_force_est:core::array::from_fn(|_|Default::default()),
tick:Default::default(),
wireless_remote:core::array::from_fn(|_|Default::default()),
bit_flag:Default::default(),
adc_reel:Default::default(),
temperature_ntc1:Default::default(),
temperature_ntc2:Default::default(),
power_v:Default::default(),
power_a:Default::default(),
fan_frequency:core::array::from_fn(|_|Default::default()),
reserve:Default::default(),
crc:Default::default(),
}}}
impl Ros2Unitree_goMsgLowState {
  pub const MAX_WIRE_BYTES:usize=1050;
  pub const CARTESIAN_WIRE_BYTES:u64=1050;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_56c01d52b26d7b2ef1fa7475fc8a353777ed6952a6f1ceee01da81f1edad3cab";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.head {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
for element in &self.sn {
n=add_size(n,4)?;
}
for element in &self.version {
n=add_size(n,4)?;
}
n=add_size(n,2)?;
n=add_size(n,self.imu_state.wire_size()?)?;
for element in &self.motor_state {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,self.bms_state.wire_size()?)?;
for element in &self.foot_force {
n=add_size(n,2)?;
}
for element in &self.foot_force_est {
n=add_size(n,2)?;
}
n=add_size(n,4)?;
for element in &self.wireless_remote {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
if !self.adc_reel.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.power_v.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.power_a.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.fan_frequency {
n=add_size(n,2)?;
}
n=add_size(n,4)?;
n=add_size(n,4)?;
if n>1050{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.head {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.level_flag.to_le_bytes());
put(out,at,&self.frame_reserve.to_le_bytes());
for element in &self.sn {
put(out,at,&element.to_le_bytes());
}
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bandwidth.to_le_bytes());
self.imu_state.write_fields(out,at);
for element in &self.motor_state {
element.write_fields(out,at);
}
self.bms_state.write_fields(out,at);
for element in &self.foot_force {
put(out,at,&element.to_le_bytes());
}
for element in &self.foot_force_est {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.tick.to_le_bytes());
for element in &self.wireless_remote {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.bit_flag.to_le_bytes());
put(out,at,&self.adc_reel.to_le_bytes());
put(out,at,&self.temperature_ntc1.to_le_bytes());
put(out,at,&self.temperature_ntc2.to_le_bytes());
put(out,at,&self.power_v.to_le_bytes());
put(out,at,&self.power_a.to_le_bytes());
for element in &self.fan_frequency {
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
value.level_flag=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.frame_reserve=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sn[i]=decoded;
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.bandwidth=decoded;
let decoded=Ros2Unitree_goMsgIMUState::read_fields(bytes,at)?;
value.imu_state=decoded;
{let count=20usize;if count>20{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Unitree_goMsgMotorState::read_fields(bytes,at)?;
value.motor_state[i]=decoded;
}}
let decoded=Ros2Unitree_goMsgBmsState::read_fields(bytes,at)?;
value.bms_state=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.foot_force[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.foot_force_est[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.tick=decoded;
{let count=40usize;if count>40{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.wireless_remote[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.bit_flag=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.adc_reel=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.temperature_ntc1=decoded;
let decoded=i8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.temperature_ntc2=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.power_v=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.power_a=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.fan_frequency[i]=decoded;
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.reserve=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.crc=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgLowState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1050{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1050{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/MotorCmds.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgMotorCmds {
  /// Declared cmds; canonical bounds are part of CODEC_IDENTITY.
  pub cmds:Vec<Ros2Unitree_goMsgMotorCmd>,
}
impl Default for Ros2Unitree_goMsgMotorCmds {fn default()->Self {Self {
cmds:Default::default(),
}}}
impl Ros2Unitree_goMsgMotorCmds {
  pub const MAX_WIRE_BYTES:usize=664;
  pub const CARTESIAN_WIRE_BYTES:u64=664;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_59f6b09147eb868cad93205c27cdd339e55f5928c8eef32da11ec5675c61a671";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.cmds.len()>20{return Err(invalid());}n=add_size(n,4)?;
for element in &self.cmds {
n=add_size(n,element.wire_size()?)?;
}
if n>664{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.cmds.len() as u32).to_le_bytes());
for element in &self.cmds {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>20{return Err(invalid());}
value.cmds=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Unitree_goMsgMotorCmd::read_fields(bytes,at)?;
value.cmds.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgMotorCmds {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>664{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/MotorStates.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgMotorStates {
  /// Declared states; canonical bounds are part of CODEC_IDENTITY.
  pub states:Vec<Ros2Unitree_goMsgMotorState>,
}
impl Default for Ros2Unitree_goMsgMotorStates {fn default()->Self {Self {
states:Default::default(),
}}}
impl Ros2Unitree_goMsgMotorStates {
  pub const MAX_WIRE_BYTES:usize=844;
  pub const CARTESIAN_WIRE_BYTES:u64=844;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0843528158199493384c36a65b0fb1e15244d21160c33aac82f9a87718c08178";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.states.len()>20{return Err(invalid());}n=add_size(n,4)?;
for element in &self.states {
n=add_size(n,element.wire_size()?)?;
}
if n>844{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.states.len() as u32).to_le_bytes());
for element in &self.states {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=read_count(bytes,at)?;if count>20{return Err(invalid());}
value.states=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Unitree_goMsgMotorState::read_fields(bytes,at)?;
value.states.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgMotorStates {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>844{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<4{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/PathPoint.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgPathPoint {
  /// Declared t_from_start; canonical bounds are part of CODEC_IDENTITY.
  pub t_from_start:f32,
  /// Declared x; canonical bounds are part of CODEC_IDENTITY.
  pub x:f32,
  /// Declared y; canonical bounds are part of CODEC_IDENTITY.
  pub y:f32,
  /// Declared yaw; canonical bounds are part of CODEC_IDENTITY.
  pub yaw:f32,
  /// Declared vx; canonical bounds are part of CODEC_IDENTITY.
  pub vx:f32,
  /// Declared vy; canonical bounds are part of CODEC_IDENTITY.
  pub vy:f32,
  /// Declared vyaw; canonical bounds are part of CODEC_IDENTITY.
  pub vyaw:f32,
}
impl Default for Ros2Unitree_goMsgPathPoint {fn default()->Self {Self {
t_from_start:Default::default(),
x:Default::default(),
y:Default::default(),
yaw:Default::default(),
vx:Default::default(),
vy:Default::default(),
vyaw:Default::default(),
}}}
impl Ros2Unitree_goMsgPathPoint {
  pub const MAX_WIRE_BYTES:usize=28;
  pub const CARTESIAN_WIRE_BYTES:u64=28;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_35eb1cc629dcecbace17b342029c287310c30d4e6d988740bf2c212a1251cee4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.t_from_start.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.x.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.y.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.yaw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.vx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.vy.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.vyaw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if n>28{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.t_from_start.to_le_bytes());
put(out,at,&self.x.to_le_bytes());
put(out,at,&self.y.to_le_bytes());
put(out,at,&self.yaw.to_le_bytes());
put(out,at,&self.vx.to_le_bytes());
put(out,at,&self.vy.to_le_bytes());
put(out,at,&self.vyaw.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.t_from_start=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.x=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.y=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yaw=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.vx=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.vy=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.vyaw=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgPathPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>28{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/Req.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgReq {
  /// Declared uuid; canonical bounds are part of CODEC_IDENTITY.
  pub uuid:String,
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Unitree_goMsgReq {fn default()->Self {Self {
uuid:Default::default(),
body:Default::default(),
}}}
impl Ros2Unitree_goMsgReq {
  pub const MAX_WIRE_BYTES:usize=4232;
  pub const CARTESIAN_WIRE_BYTES:u64=4232;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_43e0126993336d81a3f6a6706979c19f3a193f6a1ef504cadcd864be8bfaf84e";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.uuid.len()>128||self.uuid.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.uuid.len())?;
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>4232{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.uuid.len() as u32).to_le_bytes());put(out,at,self.uuid.as_bytes());
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.uuid=decoded;
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgReq {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>4232{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/Res.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgRes {
  /// Declared uuid; canonical bounds are part of CODEC_IDENTITY.
  pub uuid:String,
  /// Declared data; canonical bounds are part of CODEC_IDENTITY.
  pub data:Vec<u8>,
  /// Declared body; canonical bounds are part of CODEC_IDENTITY.
  pub body:String,
}
impl Default for Ros2Unitree_goMsgRes {fn default()->Self {Self {
uuid:Default::default(),
data:Default::default(),
body:Default::default(),
}}}
impl Ros2Unitree_goMsgRes {
  pub const MAX_WIRE_BYTES:usize=5260;
  pub const CARTESIAN_WIRE_BYTES:u64=5260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_53c1d85c0cb17cfdd7d3799d6e92602a70700e8c55da4045f501a6a12ff55bfb";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.uuid.len()>128||self.uuid.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.uuid.len())?;
if self.data.len()>1024{return Err(invalid());}n=add_size(n,4)?;
for element in &self.data {
n=add_size(n,1)?;
}
if self.body.len()>4096||self.body.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.body.len())?;
if n>5260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.uuid.len() as u32).to_le_bytes());put(out,at,self.uuid.as_bytes());
put(out,at,&(self.data.len() as u32).to_le_bytes());
for element in &self.data {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.body.len() as u32).to_le_bytes());put(out,at,self.body.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.uuid=decoded;
{let count=read_count(bytes,at)?;if count>1024{return Err(invalid());}
value.data=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.data.push(decoded);
}}
let size=read_count(bytes,at)?;if size>4096{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.body=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgRes {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<12{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/SportModeCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgSportModeCmd {
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared gait_type; canonical bounds are part of CODEC_IDENTITY.
  pub gait_type:u8,
  /// Declared speed_level; canonical bounds are part of CODEC_IDENTITY.
  pub speed_level:u8,
  /// Declared foot_raise_height; canonical bounds are part of CODEC_IDENTITY.
  pub foot_raise_height:f32,
  /// Declared body_height; canonical bounds are part of CODEC_IDENTITY.
  pub body_height:f32,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:[f32;2],
  /// Declared euler; canonical bounds are part of CODEC_IDENTITY.
  pub euler:[f32;3],
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:[f32;2],
  /// Declared yaw_speed; canonical bounds are part of CODEC_IDENTITY.
  pub yaw_speed:f32,
  /// Declared bms_cmd; canonical bounds are part of CODEC_IDENTITY.
  pub bms_cmd:Ros2Unitree_goMsgBmsCmd,
  /// Declared path_point; canonical bounds are part of CODEC_IDENTITY.
  pub path_point:[Ros2Unitree_goMsgPathPoint;30],
}
impl Default for Ros2Unitree_goMsgSportModeCmd {fn default()->Self {Self {
mode:Default::default(),
gait_type:Default::default(),
speed_level:Default::default(),
foot_raise_height:Default::default(),
body_height:Default::default(),
position:core::array::from_fn(|_|Default::default()),
euler:core::array::from_fn(|_|Default::default()),
velocity:core::array::from_fn(|_|Default::default()),
yaw_speed:Default::default(),
bms_cmd:Default::default(),
path_point:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgSportModeCmd {
  pub const MAX_WIRE_BYTES:usize=887;
  pub const CARTESIAN_WIRE_BYTES:u64=887;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_cf90a065007b9b8cedde3f0b58aa79da3b39a1456dc1b5297f4fe799d92ff22c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.foot_raise_height.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.body_height.is_finite(){return Err(invalid());}
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
if !self.yaw_speed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,self.bms_cmd.wire_size()?)?;
for element in &self.path_point {
n=add_size(n,element.wire_size()?)?;
}
if n>887{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.gait_type.to_le_bytes());
put(out,at,&self.speed_level.to_le_bytes());
put(out,at,&self.foot_raise_height.to_le_bytes());
put(out,at,&self.body_height.to_le_bytes());
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
for element in &self.euler {
put(out,at,&element.to_le_bytes());
}
for element in &self.velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.yaw_speed.to_le_bytes());
self.bms_cmd.write_fields(out,at);
for element in &self.path_point {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gait_type=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.speed_level=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.foot_raise_height=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.body_height=decoded;
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
value.yaw_speed=decoded;
let decoded=Ros2Unitree_goMsgBmsCmd::read_fields(bytes,at)?;
value.bms_cmd=decoded;
{let count=30usize;if count>30{return Err(invalid());}
for i in 0..count {
let decoded=Ros2Unitree_goMsgPathPoint::read_fields(bytes,at)?;
value.path_point[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgSportModeCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>887{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<887{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/TimeSpec.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgTimeSpec {
  /// Declared sec; canonical bounds are part of CODEC_IDENTITY.
  pub sec:i32,
  /// Declared nanosec; canonical bounds are part of CODEC_IDENTITY.
  pub nanosec:u32,
}
impl Default for Ros2Unitree_goMsgTimeSpec {fn default()->Self {Self {
sec:Default::default(),
nanosec:Default::default(),
}}}
impl Ros2Unitree_goMsgTimeSpec {
  pub const MAX_WIRE_BYTES:usize=8;
  pub const CARTESIAN_WIRE_BYTES:u64=8;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9ad3c6ed5a9ff0bba22593b50e1d777ae2f63480a01a744d777ee6b7dad0ecc4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
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
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgTimeSpec {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>8{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<8{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/SportModeState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgSportModeState {
  /// Declared stamp; canonical bounds are part of CODEC_IDENTITY.
  pub stamp:Ros2Unitree_goMsgTimeSpec,
  /// Declared error_code; canonical bounds are part of CODEC_IDENTITY.
  pub error_code:u32,
  /// Declared imu_state; canonical bounds are part of CODEC_IDENTITY.
  pub imu_state:Ros2Unitree_goMsgIMUState,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:u8,
  /// Declared progress; canonical bounds are part of CODEC_IDENTITY.
  pub progress:f32,
  /// Declared gait_type; canonical bounds are part of CODEC_IDENTITY.
  pub gait_type:u8,
  /// Declared foot_raise_height; canonical bounds are part of CODEC_IDENTITY.
  pub foot_raise_height:f32,
  /// Declared position; canonical bounds are part of CODEC_IDENTITY.
  pub position:[f32;3],
  /// Declared body_height; canonical bounds are part of CODEC_IDENTITY.
  pub body_height:f32,
  /// Declared velocity; canonical bounds are part of CODEC_IDENTITY.
  pub velocity:[f32;3],
  /// Declared yaw_speed; canonical bounds are part of CODEC_IDENTITY.
  pub yaw_speed:f32,
  /// Declared range_obstacle; canonical bounds are part of CODEC_IDENTITY.
  pub range_obstacle:[f32;4],
  /// Declared foot_force; canonical bounds are part of CODEC_IDENTITY.
  pub foot_force:[i16;4],
  /// Declared foot_position_body; canonical bounds are part of CODEC_IDENTITY.
  pub foot_position_body:[f32;12],
  /// Declared foot_speed_body; canonical bounds are part of CODEC_IDENTITY.
  pub foot_speed_body:[f32;12],
}
impl Default for Ros2Unitree_goMsgSportModeState {fn default()->Self {Self {
stamp:Default::default(),
error_code:Default::default(),
imu_state:Default::default(),
mode:Default::default(),
progress:Default::default(),
gait_type:Default::default(),
foot_raise_height:Default::default(),
position:core::array::from_fn(|_|Default::default()),
body_height:Default::default(),
velocity:core::array::from_fn(|_|Default::default()),
yaw_speed:Default::default(),
range_obstacle:core::array::from_fn(|_|Default::default()),
foot_force:core::array::from_fn(|_|Default::default()),
foot_position_body:core::array::from_fn(|_|Default::default()),
foot_speed_body:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Unitree_goMsgSportModeState {
  pub const MAX_WIRE_BYTES:usize=227;
  pub const CARTESIAN_WIRE_BYTES:u64=227;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_2935b9d838087267840b379582862a15c2e1a72fe8ad2bcbf8f19af2f8054760";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.stamp.wire_size()?)?;
n=add_size(n,4)?;
n=add_size(n,self.imu_state.wire_size()?)?;
n=add_size(n,1)?;
if !self.progress.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,1)?;
if !self.foot_raise_height.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.position {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.body_height.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.velocity {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if !self.yaw_speed.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.range_obstacle {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.foot_force {
n=add_size(n,2)?;
}
for element in &self.foot_position_body {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
for element in &self.foot_speed_body {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>227{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.stamp.write_fields(out,at);
put(out,at,&self.error_code.to_le_bytes());
self.imu_state.write_fields(out,at);
put(out,at,&self.mode.to_le_bytes());
put(out,at,&self.progress.to_le_bytes());
put(out,at,&self.gait_type.to_le_bytes());
put(out,at,&self.foot_raise_height.to_le_bytes());
for element in &self.position {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.body_height.to_le_bytes());
for element in &self.velocity {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.yaw_speed.to_le_bytes());
for element in &self.range_obstacle {
put(out,at,&element.to_le_bytes());
}
for element in &self.foot_force {
put(out,at,&element.to_le_bytes());
}
for element in &self.foot_position_body {
put(out,at,&element.to_le_bytes());
}
for element in &self.foot_speed_body {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Unitree_goMsgTimeSpec::read_fields(bytes,at)?;
value.stamp=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.error_code=decoded;
let decoded=Ros2Unitree_goMsgIMUState::read_fields(bytes,at)?;
value.imu_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.progress=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.gait_type=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.foot_raise_height=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.position[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.body_height=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.velocity[i]=decoded;
}}
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yaw_speed=decoded;
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.range_obstacle[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=i16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.foot_force[i]=decoded;
}}
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.foot_position_body[i]=decoded;
}}
{let count=12usize;if count>12{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.foot_speed_body[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgSportModeState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>227{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<227{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/UwbState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgUwbState {
  /// Declared version; canonical bounds are part of CODEC_IDENTITY.
  pub version:[u8;2],
  /// Declared channel; canonical bounds are part of CODEC_IDENTITY.
  pub channel:u8,
  /// Declared joy_mode; canonical bounds are part of CODEC_IDENTITY.
  pub joy_mode:u8,
  /// Declared orientation_est; canonical bounds are part of CODEC_IDENTITY.
  pub orientation_est:f32,
  /// Declared pitch_est; canonical bounds are part of CODEC_IDENTITY.
  pub pitch_est:f32,
  /// Declared distance_est; canonical bounds are part of CODEC_IDENTITY.
  pub distance_est:f32,
  /// Declared yaw_est; canonical bounds are part of CODEC_IDENTITY.
  pub yaw_est:f32,
  /// Declared tag_roll; canonical bounds are part of CODEC_IDENTITY.
  pub tag_roll:f32,
  /// Declared tag_pitch; canonical bounds are part of CODEC_IDENTITY.
  pub tag_pitch:f32,
  /// Declared tag_yaw; canonical bounds are part of CODEC_IDENTITY.
  pub tag_yaw:f32,
  /// Declared base_roll; canonical bounds are part of CODEC_IDENTITY.
  pub base_roll:f32,
  /// Declared base_pitch; canonical bounds are part of CODEC_IDENTITY.
  pub base_pitch:f32,
  /// Declared base_yaw; canonical bounds are part of CODEC_IDENTITY.
  pub base_yaw:f32,
  /// Declared joystick; canonical bounds are part of CODEC_IDENTITY.
  pub joystick:[f32;2],
  /// Declared error_state; canonical bounds are part of CODEC_IDENTITY.
  pub error_state:u8,
  /// Declared buttons; canonical bounds are part of CODEC_IDENTITY.
  pub buttons:u8,
  /// Declared enabled_from_app; canonical bounds are part of CODEC_IDENTITY.
  pub enabled_from_app:u8,
}
impl Default for Ros2Unitree_goMsgUwbState {fn default()->Self {Self {
version:core::array::from_fn(|_|Default::default()),
channel:Default::default(),
joy_mode:Default::default(),
orientation_est:Default::default(),
pitch_est:Default::default(),
distance_est:Default::default(),
yaw_est:Default::default(),
tag_roll:Default::default(),
tag_pitch:Default::default(),
tag_yaw:Default::default(),
base_roll:Default::default(),
base_pitch:Default::default(),
base_yaw:Default::default(),
joystick:core::array::from_fn(|_|Default::default()),
error_state:Default::default(),
buttons:Default::default(),
enabled_from_app:Default::default(),
}}}
impl Ros2Unitree_goMsgUwbState {
  pub const MAX_WIRE_BYTES:usize=55;
  pub const CARTESIAN_WIRE_BYTES:u64=55;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_017fbafd61a006aeaf6cb80456ac89f429d30a562da2e0d394c6cfb406e61a34";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
for element in &self.version {
n=add_size(n,1)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
if !self.orientation_est.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.pitch_est.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.distance_est.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.yaw_est.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tag_roll.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tag_pitch.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.tag_yaw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.base_roll.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.base_pitch.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.base_yaw.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
for element in &self.joystick {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
n=add_size(n,1)?;
n=add_size(n,1)?;
n=add_size(n,1)?;
if n>55{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
for element in &self.version {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.channel.to_le_bytes());
put(out,at,&self.joy_mode.to_le_bytes());
put(out,at,&self.orientation_est.to_le_bytes());
put(out,at,&self.pitch_est.to_le_bytes());
put(out,at,&self.distance_est.to_le_bytes());
put(out,at,&self.yaw_est.to_le_bytes());
put(out,at,&self.tag_roll.to_le_bytes());
put(out,at,&self.tag_pitch.to_le_bytes());
put(out,at,&self.tag_yaw.to_le_bytes());
put(out,at,&self.base_roll.to_le_bytes());
put(out,at,&self.base_pitch.to_le_bytes());
put(out,at,&self.base_yaw.to_le_bytes());
for element in &self.joystick {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.error_state.to_le_bytes());
put(out,at,&self.buttons.to_le_bytes());
put(out,at,&self.enabled_from_app.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.version[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.channel=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.joy_mode=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.orientation_est=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.pitch_est=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.distance_est=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.yaw_est=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tag_roll=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tag_pitch=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tag_yaw=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.base_roll=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.base_pitch=decoded;
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.base_yaw=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.joystick[i]=decoded;
}}
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.error_state=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.buttons=decoded;
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.enabled_from_app=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgUwbState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>55{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<55{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/UwbSwitch.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgUwbSwitch {
  /// Declared enabled; canonical bounds are part of CODEC_IDENTITY.
  pub enabled:u8,
}
impl Default for Ros2Unitree_goMsgUwbSwitch {fn default()->Self {Self {
enabled:Default::default(),
}}}
impl Ros2Unitree_goMsgUwbSwitch {
  pub const MAX_WIRE_BYTES:usize=1;
  pub const CARTESIAN_WIRE_BYTES:u64=1;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4546618797829b3911d1533d8d6bacc9da969d40d7752424de34bbd3de4bede5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,1)?;
if n>1{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.enabled.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.enabled=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgUwbSwitch {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<1{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 unitree_go/msg/WirelessController.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Unitree_goMsgWirelessController {
  /// Declared lx; canonical bounds are part of CODEC_IDENTITY.
  pub lx:f32,
  /// Declared ly; canonical bounds are part of CODEC_IDENTITY.
  pub ly:f32,
  /// Declared rx; canonical bounds are part of CODEC_IDENTITY.
  pub rx:f32,
  /// Declared ry; canonical bounds are part of CODEC_IDENTITY.
  pub ry:f32,
  /// Declared keys; canonical bounds are part of CODEC_IDENTITY.
  pub keys:u16,
}
impl Default for Ros2Unitree_goMsgWirelessController {fn default()->Self {Self {
lx:Default::default(),
ly:Default::default(),
rx:Default::default(),
ry:Default::default(),
keys:Default::default(),
}}}
impl Ros2Unitree_goMsgWirelessController {
  pub const MAX_WIRE_BYTES:usize=18;
  pub const CARTESIAN_WIRE_BYTES:u64=18;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_536e69ce5459227cb68b3c6d484a96bfa40e1400aab820f8ad0d0d62ff6ba438";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if !self.lx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ly.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.rx.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
if !self.ry.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
n=add_size(n,2)?;
if n>18{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.lx.to_le_bytes());
put(out,at,&self.ly.to_le_bytes());
put(out,at,&self.rx.to_le_bytes());
put(out,at,&self.ry.to_le_bytes());
put(out,at,&self.keys.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
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
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.keys=decoded;
Ok(value)}
}
impl Canonical for Ros2Unitree_goMsgWirelessController {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>18{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<18{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
