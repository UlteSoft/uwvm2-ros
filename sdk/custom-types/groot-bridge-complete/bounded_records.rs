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
/// Complete bounded value for ROS2 uwvm_groot_bridge/msg/RGBFrame.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_groot_bridgeMsgRGBFrame {
  /// Declared key; canonical bounds are part of CODEC_IDENTITY.
  pub key:String,
  /// Declared shape; canonical bounds are part of CODEC_IDENTITY.
  pub shape:[u32;5],
  /// Declared pixels; canonical bounds are part of CODEC_IDENTITY.
  pub pixels:Vec<u8>,
}
impl Default for Ros2Uwvm_groot_bridgeMsgRGBFrame {fn default()->Self {Self {
key:Default::default(),
shape:core::array::from_fn(|_|Default::default()),
pixels:Default::default(),
}}}
impl Ros2Uwvm_groot_bridgeMsgRGBFrame {
  pub const MAX_WIRE_BYTES:usize=860;
  pub const CARTESIAN_WIRE_BYTES:u64=860;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_db7172fe7a7f0672c907f31ca0b1cb6286b804246d62e0414ac4313ae1096364";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.key.len()>64||self.key.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.key.len())?;
for element in &self.shape {
n=add_size(n,4)?;
}
if self.pixels.len()>768{return Err(invalid());}n=add_size(n,4)?;
for element in &self.pixels {
n=add_size(n,1)?;
}
if n>860{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.key.len() as u32).to_le_bytes());put(out,at,self.key.as_bytes());
for element in &self.shape {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.pixels.len() as u32).to_le_bytes());
for element in &self.pixels {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.key=decoded;
{let count=5usize;if count>5{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.shape[i]=decoded;
}}
{let count=read_count(bytes,at)?;if count>768{return Err(invalid());}
value.pixels=Vec::with_capacity(count);
for i in 0..count {
let decoded=u8::from_le_bytes(take(bytes,at,1)?.try_into().map_err(|_|invalid())?);
value.pixels.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_groot_bridgeMsgRGBFrame {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>860{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<28{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_groot_bridge/msg/FloatTensor.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_groot_bridgeMsgFloatTensor {
  /// Declared key; canonical bounds are part of CODEC_IDENTITY.
  pub key:String,
  /// Declared shape; canonical bounds are part of CODEC_IDENTITY.
  pub shape:[u32;3],
  /// Declared values; canonical bounds are part of CODEC_IDENTITY.
  pub values:Vec<f32>,
}
impl Default for Ros2Uwvm_groot_bridgeMsgFloatTensor {fn default()->Self {Self {
key:Default::default(),
shape:core::array::from_fn(|_|Default::default()),
values:Default::default(),
}}}
impl Ros2Uwvm_groot_bridgeMsgFloatTensor {
  pub const MAX_WIRE_BYTES:usize=1364;
  pub const CARTESIAN_WIRE_BYTES:u64=1364;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_a091d322ab737d2a9415f889fb78a18df7aec6ce74cfab0dcd5bb2a61ae3abae";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.key.len()>64||self.key.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.key.len())?;
for element in &self.shape {
n=add_size(n,4)?;
}
if self.values.len()>320{return Err(invalid());}n=add_size(n,4)?;
for element in &self.values {
if !element.is_finite(){return Err(invalid());}
n=add_size(n,4)?;
}
if n>1364{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.key.len() as u32).to_le_bytes());put(out,at,self.key.as_bytes());
for element in &self.shape {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.values.len() as u32).to_le_bytes());
for element in &self.values {
put(out,at,&element.to_le_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.key=decoded;
{let count=3usize;if count>3{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.shape[i]=decoded;
}}
{let count=read_count(bytes,at)?;if count>320{return Err(invalid());}
value.values=Vec::with_capacity(count);
for i in 0..count {
let decoded=f32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
if !decoded.is_finite(){return Err(invalid());}
value.values.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_groot_bridgeMsgFloatTensor {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1364{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_groot_bridge/msg/TextSequence.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_groot_bridgeMsgTextSequence {
  /// Declared key; canonical bounds are part of CODEC_IDENTITY.
  pub key:String,
  /// Declared shape; canonical bounds are part of CODEC_IDENTITY.
  pub shape:[u32;2],
  /// Declared texts; canonical bounds are part of CODEC_IDENTITY.
  pub texts:Vec<String>,
}
impl Default for Ros2Uwvm_groot_bridgeMsgTextSequence {fn default()->Self {Self {
key:Default::default(),
shape:core::array::from_fn(|_|Default::default()),
texts:Default::default(),
}}}
impl Ros2Uwvm_groot_bridgeMsgTextSequence {
  pub const MAX_WIRE_BYTES:usize=1120;
  pub const CARTESIAN_WIRE_BYTES:u64=1120;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_895be974f2560f0afff7614f5400ff7205e013cda269825a8443628621c8e80d";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.key.len()>64||self.key.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,self.key.len())?;
for element in &self.shape {
n=add_size(n,4)?;
}
if self.texts.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.texts {
if element.len()>256||element.as_bytes().contains(&0){return Err(invalid());}n=add_size(n,4)?;n=add_size(n,element.len())?;
}
if n>1120{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.key.len() as u32).to_le_bytes());put(out,at,self.key.as_bytes());
for element in &self.shape {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.texts.len() as u32).to_le_bytes());
for element in &self.texts {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>64{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.key=decoded;
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.shape[i]=decoded;
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.texts=Vec::with_capacity(count);
for i in 0..count {
let size=read_count(bytes,at)?;if size>256{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.texts.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_groot_bridgeMsgTextSequence {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>1120{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<16{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_groot_bridge/srv/InferAction_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_groot_bridgeSrvInferAction_Request {
  /// Declared application_nonce; canonical bounds are part of CODEC_IDENTITY.
  pub application_nonce:u64,
  /// Declared video; canonical bounds are part of CODEC_IDENTITY.
  pub video:Vec<Ros2Uwvm_groot_bridgeMsgRGBFrame>,
  /// Declared state; canonical bounds are part of CODEC_IDENTITY.
  pub state:Vec<Ros2Uwvm_groot_bridgeMsgFloatTensor>,
  /// Declared language; canonical bounds are part of CODEC_IDENTITY.
  pub language:Vec<Ros2Uwvm_groot_bridgeMsgTextSequence>,
}
impl Default for Ros2Uwvm_groot_bridgeSrvInferAction_Request {fn default()->Self {Self {
application_nonce:Default::default(),
video:Default::default(),
state:Default::default(),
language:Default::default(),
}}}
impl Ros2Uwvm_groot_bridgeSrvInferAction_Request {
  pub const MAX_WIRE_BYTES:usize=10296;
  pub const CARTESIAN_WIRE_BYTES:u64=10296;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_78f85929fb8646e9a92f17cafa3e8296fde42aaf49a56b271531ebad869a0fa6";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,8)?;
if self.video.len()>3{return Err(invalid());}n=add_size(n,4)?;
for element in &self.video {
n=add_size(n,element.wire_size()?)?;
}
if self.state.len()>4{return Err(invalid());}n=add_size(n,4)?;
for element in &self.state {
n=add_size(n,element.wire_size()?)?;
}
if self.language.len()>2{return Err(invalid());}n=add_size(n,4)?;
for element in &self.language {
n=add_size(n,element.wire_size()?)?;
}
if n>10296{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.application_nonce.to_le_bytes());
put(out,at,&(self.video.len() as u32).to_le_bytes());
for element in &self.video {
element.write_fields(out,at);
}
put(out,at,&(self.state.len() as u32).to_le_bytes());
for element in &self.state {
element.write_fields(out,at);
}
put(out,at,&(self.language.len() as u32).to_le_bytes());
for element in &self.language {
element.write_fields(out,at);
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.application_nonce=decoded;
{let count=read_count(bytes,at)?;if count>3{return Err(invalid());}
value.video=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_groot_bridgeMsgRGBFrame::read_fields(bytes,at)?;
value.video.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.state=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_groot_bridgeMsgFloatTensor::read_fields(bytes,at)?;
value.state.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>2{return Err(invalid());}
value.language=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_groot_bridgeMsgTextSequence::read_fields(bytes,at)?;
value.language.push(decoded);
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_groot_bridgeSrvInferAction_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>10296{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<20{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_groot_bridge/srv/InferAction_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_groot_bridgeSrvInferAction_Response {
  /// Declared status; canonical bounds are part of CODEC_IDENTITY.
  pub status:u32,
  /// Declared application_nonce; canonical bounds are part of CODEC_IDENTITY.
  pub application_nonce:u64,
  /// Declared action; canonical bounds are part of CODEC_IDENTITY.
  pub action:Vec<Ros2Uwvm_groot_bridgeMsgFloatTensor>,
  /// Declared info_is_empty; canonical bounds are part of CODEC_IDENTITY.
  pub info_is_empty:bool,
}
impl Default for Ros2Uwvm_groot_bridgeSrvInferAction_Response {fn default()->Self {Self {
status:Default::default(),
application_nonce:Default::default(),
action:Default::default(),
info_is_empty:Default::default(),
}}}
impl Ros2Uwvm_groot_bridgeSrvInferAction_Response {
  pub const MAX_WIRE_BYTES:usize=10929;
  pub const CARTESIAN_WIRE_BYTES:u64=10929;
  pub const AGGREGATE_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_787281a2adea187ae43ff951a414dd7563b3e08318414e5199e1950b8143a3ce";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n=add_size(n,4)?;
n=add_size(n,8)?;
if self.action.len()>8{return Err(invalid());}n=add_size(n,4)?;
for element in &self.action {
n=add_size(n,element.wire_size()?)?;
}
n=add_size(n,1)?;
if n>10929{return Err(Error::new(codes::TOO_LARGE));}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.status.to_le_bytes());
put(out,at,&self.application_nonce.to_le_bytes());
put(out,at,&(self.action.len() as u32).to_le_bytes());
for element in &self.action {
element.write_fields(out,at);
}
put(out,at,&[u8::from(self.info_is_empty)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.status=decoded;
let decoded=u64::from_le_bytes(take(bytes,at,8)?.try_into().map_err(|_|invalid())?);
value.application_nonce=decoded;
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.action=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_groot_bridgeMsgFloatTensor::read_fields(bytes,at)?;
value.action.push(decoded);
}}
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.info_is_empty=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_groot_bridgeSrvInferAction_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()>10929{return Err(Error::new(codes::TOO_LARGE));}if bytes.len()<17{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
