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
/// Complete bounded value for ROS2 uwvm_limx_bridge/msg/NativeHeader.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_limx_bridgeMsgNativeHeader {
  /// Declared seq; canonical bounds are part of CODEC_IDENTITY.
  pub seq:u32,
  /// Declared stamp_sec; canonical bounds are part of CODEC_IDENTITY.
  pub stamp_sec:u32,
  /// Declared stamp_nsec; canonical bounds are part of CODEC_IDENTITY.
  pub stamp_nsec:u32,
  /// Declared frame_id; canonical bounds are part of CODEC_IDENTITY.
  pub frame_id:String,
}
impl Default for Ros2Uwvm_limx_bridgeMsgNativeHeader {fn default()->Self {Self {
seq:Default::default(),
stamp_sec:Default::default(),
stamp_nsec:Default::default(),
frame_id:Default::default(),
}}}
impl Ros2Uwvm_limx_bridgeMsgNativeHeader {
  pub const MAX_WIRE_BYTES:usize=144;
  pub const CARTESIAN_WIRE_BYTES:u64=144;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,4)?;
n=add_size(n,4)?;
if self.frame_id.len()>128||self.frame_id.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.frame_id.len())?;
if n>144{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.seq.to_le_bytes());
put(out,at,&self.stamp_sec.to_le_bytes());
put(out,at,&self.stamp_nsec.to_le_bytes());
put(out,at,&(self.frame_id.len() as u32).to_le_bytes());put(out,at,self.frame_id.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.seq=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.stamp_sec=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.stamp_nsec=decoded;
let size=read_count(bytes,at)?;if size>128{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.frame_id=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_limx_bridgeMsgNativeHeader {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>144{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_limx_bridge/msg/ImuData.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_limx_bridgeMsgImuData {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Uwvm_limx_bridgeMsgNativeHeader,
  /// Declared imustamp; canonical bounds are part of CODEC_IDENTITY.
  pub imustamp:u64,
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:u32,
  /// Declared euler; canonical bounds are part of CODEC_IDENTITY.
  pub euler:[f64;3],
  /// Declared quat; canonical bounds are part of CODEC_IDENTITY.
  pub quat:[f64;4],
  /// Declared acc; canonical bounds are part of CODEC_IDENTITY.
  pub acc:[f64;3],
  /// Declared gyro; canonical bounds are part of CODEC_IDENTITY.
  pub gyro:[f64;3],
}
impl Default for Ros2Uwvm_limx_bridgeMsgImuData {fn default()->Self {Self {
header:Default::default(),
imustamp:Default::default(),
status:Default::default(),
euler:core::array::from_fn(|_|Default::default()),
quat:core::array::from_fn(|_|Default::default()),
acc:core::array::from_fn(|_|Default::default()),
gyro:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Uwvm_limx_bridgeMsgImuData {
  pub const MAX_WIRE_BYTES:usize=260;
  pub const CARTESIAN_WIRE_BYTES:u64=260;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
n=add_size(n,8)?;
n=add_size(n,4)?;
for element in &self.euler {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.quat {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.acc {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
for element in &self.gyro {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,8)?;
}
if n>260{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&self.imustamp.to_le_bytes());
put(out,at,&self.status.to_le_bytes());
for element in &self.euler {
put(out,at,&element.to_le_bytes());
}
for element in &self.quat {
put(out,at,&element.to_le_bytes());
}
for element in &self.acc {
put(out,at,&element.to_le_bytes());
}
for element in &self.gyro {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader::read_fields(bytes,at)?;
value.header=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.imustamp=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.euler[i]=decoded;
}}
{let count=4usize;if count>4{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.quat[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.acc[i]=decoded;
}}
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=f64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.gyro[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_limx_bridgeMsgImuData {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>260{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<132{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_limx_bridge/msg/JointState.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_limx_bridgeMsgJointState {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Uwvm_limx_bridgeMsgNativeHeader,
  /// Declared names; canonical bounds are part of CODEC_IDENTITY.
  pub names:Vec<String>,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:Vec<f32>,
  /// Declared v; canonical bounds are part of CODEC_IDENTITY.
  pub v:Vec<f32>,
  /// Declared vd; canonical bounds are part of CODEC_IDENTITY.
  pub vd:Vec<f32>,
  /// Declared tau; canonical bounds are part of CODEC_IDENTITY.
  pub tau:Vec<f32>,
  /// Declared na; canonical bounds are part of CODEC_IDENTITY.
  pub na:u32,
}
impl Default for Ros2Uwvm_limx_bridgeMsgJointState {fn default()->Self {Self {
header:Default::default(),
names:Default::default(),
q:Default::default(),
v:Default::default(),
vd:Default::default(),
tau:Default::default(),
na:Default::default(),
}}}
impl Ros2Uwvm_limx_bridgeMsgJointState {
  pub const MAX_WIRE_BYTES:usize=5544;
  pub const CARTESIAN_WIRE_BYTES:u64=5544;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.names.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.q.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.q {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.v.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.v {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.vd.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.vd {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.tau.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tau {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
n=add_size(n,4)?;
if n>5544{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.names.len() as u32).to_le_bytes());
for element in &self.names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.q.len() as u32).to_le_bytes());
for element in &self.q {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.v.len() as u32).to_le_bytes());
for element in &self.v {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.vd.len() as u32).to_le_bytes());
for element in &self.vd {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tau.len() as u32).to_le_bytes());
for element in &self.tau {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.na.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.q=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.v=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.v.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.vd=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.vd.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.tau=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau.push(decoded);
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.na=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_limx_bridgeMsgJointState {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5544{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<40{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_limx_bridge/msg/JointCmd.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_limx_bridgeMsgJointCmd {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Uwvm_limx_bridgeMsgNativeHeader,
  /// Declared names; canonical bounds are part of CODEC_IDENTITY.
  pub names:Vec<String>,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:Vec<f32>,
  /// Declared v; canonical bounds are part of CODEC_IDENTITY.
  pub v:Vec<f32>,
  /// Declared tau; canonical bounds are part of CODEC_IDENTITY.
  pub tau:Vec<f32>,
  /// Declared kp; canonical bounds are part of CODEC_IDENTITY.
  pub kp:Vec<f32>,
  /// Declared kd; canonical bounds are part of CODEC_IDENTITY.
  pub kd:Vec<f32>,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:Vec<u8>,
  /// Declared na; canonical bounds are part of CODEC_IDENTITY.
  pub na:u32,
}
impl Default for Ros2Uwvm_limx_bridgeMsgJointCmd {fn default()->Self {Self {
header:Default::default(),
names:Default::default(),
q:Default::default(),
v:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
mode:Default::default(),
na:Default::default(),
}}}
impl Ros2Uwvm_limx_bridgeMsgJointCmd {
  pub const MAX_WIRE_BYTES:usize=5872;
  pub const CARTESIAN_WIRE_BYTES:u64=5872;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.names.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.q.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.q {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.v.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.v {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.tau.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tau {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.kp.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.kp {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.kd.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.kd {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.mode.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.mode {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
if n>5872{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.names.len() as u32).to_le_bytes());
for element in &self.names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.q.len() as u32).to_le_bytes());
for element in &self.q {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.v.len() as u32).to_le_bytes());
for element in &self.v {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tau.len() as u32).to_le_bytes());
for element in &self.tau {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.kp.len() as u32).to_le_bytes());
for element in &self.kp {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.kd.len() as u32).to_le_bytes());
for element in &self.kd {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.mode.len() as u32).to_le_bytes());
for element in &self.mode {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.na.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.q=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.v=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.v.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.tau=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.kp=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kp.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.kd=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kd.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.mode=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode.push(decoded);
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.na=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_limx_bridgeMsgJointCmd {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5872{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<48{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_limx_bridge/msg/JointCmdLimx.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_limx_bridgeMsgJointCmdLimx {
  /// Declared header; canonical bounds are part of CODEC_IDENTITY.
  pub header:Ros2Uwvm_limx_bridgeMsgNativeHeader,
  /// Declared names; canonical bounds are part of CODEC_IDENTITY.
  pub names:Vec<String>,
  /// Declared q; canonical bounds are part of CODEC_IDENTITY.
  pub q:Vec<f32>,
  /// Declared v; canonical bounds are part of CODEC_IDENTITY.
  pub v:Vec<f32>,
  /// Declared tau; canonical bounds are part of CODEC_IDENTITY.
  pub tau:Vec<f32>,
  /// Declared kp; canonical bounds are part of CODEC_IDENTITY.
  pub kp:Vec<f32>,
  /// Declared kd; canonical bounds are part of CODEC_IDENTITY.
  pub kd:Vec<f32>,
  /// Declared mode; canonical bounds are part of CODEC_IDENTITY.
  pub mode:Vec<u8>,
  /// Declared na; canonical bounds are part of CODEC_IDENTITY.
  pub na:u32,
  /// Declared parallel_solver_mode; canonical bounds are part of CODEC_IDENTITY.
  pub parallel_solver_mode:Vec<u8>,
}
impl Default for Ros2Uwvm_limx_bridgeMsgJointCmdLimx {fn default()->Self {Self {
header:Default::default(),
names:Default::default(),
q:Default::default(),
v:Default::default(),
tau:Default::default(),
kp:Default::default(),
kd:Default::default(),
mode:Default::default(),
na:Default::default(),
parallel_solver_mode:Default::default(),
}}}
impl Ros2Uwvm_limx_bridgeMsgJointCmdLimx {
  pub const MAX_WIRE_BYTES:usize=5940;
  pub const CARTESIAN_WIRE_BYTES:u64=5940;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,self.header.wire_size()?)?;
if self.names.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.names {
if element.len()>64||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if self.q.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.q {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.v.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.v {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.tau.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.tau {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.kp.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.kp {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.kd.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.kd {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if self.mode.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.mode {
n=add_size(n,1)?;
}
n=add_size(n,4)?;
if self.parallel_solver_mode.len()>64{return Err(invalid());}n=add_size(n,4)?;
for element in &self.parallel_solver_mode {
n=add_size(n,1)?;
}
if n>5940{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.header.write_fields(out,at);
put(out,at,&(self.names.len() as u32).to_le_bytes());
for element in &self.names {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
put(out,at,&(self.q.len() as u32).to_le_bytes());
for element in &self.q {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.v.len() as u32).to_le_bytes());
for element in &self.v {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.tau.len() as u32).to_le_bytes());
for element in &self.tau {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.kp.len() as u32).to_le_bytes());
for element in &self.kp {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.kd.len() as u32).to_le_bytes());
for element in &self.kd {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.mode.len() as u32).to_le_bytes());
for element in &self.mode {
put(out,at,&element.to_le_bytes());
}
put(out,at,&self.na.to_le_bytes());
put(out,at,&(self.parallel_solver_mode.len() as u32).to_le_bytes());
for element in &self.parallel_solver_mode {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader::read_fields(bytes,at)?;
value.header=decoded;
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.names=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.names.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.q=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.q.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.v=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.v.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.tau=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.tau.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.kp=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kp.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.kd=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.kd.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.mode=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.mode.push(decoded);
}}
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.na=decoded;
{let count=read_count(bytes,at)?;if count>64{return Err(invalid());}
value.parallel_solver_mode=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.parallel_solver_mode.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_limx_bridgeMsgJointCmdLimx {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>5940{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<52{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
