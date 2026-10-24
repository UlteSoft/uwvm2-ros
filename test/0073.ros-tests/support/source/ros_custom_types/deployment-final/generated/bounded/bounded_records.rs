//! Generated bounded-v2 canonical values; no I/O, threads, clocks or authority.
//! Enable the existing SDK alloc feature. Containers/UTF-8 are validated before
//! output allocation; decoder lengths are checked before bounded allocations.
//! Empty interface components are zero-sized semantic values, not native byte copies.
extern crate alloc;
use alloc::{vec::Vec,string::String};
use roswasm::{Error,codes};
use roswasm::typed::Canonical;
fn invalid()->Error {Error::new(codes::INVALID)}
fn take<'a>(bytes:&'a [u8],at:&mut usize,n:usize)->Result<&'a [u8],Error>{if *at>bytes.len()||n>bytes.len()-*at{return Err(invalid());}let start=*at;*at+=n;Ok(&bytes[start..*at])}
fn read_count(bytes:&[u8],at:&mut usize)->Result<usize,Error>{Ok(u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?) as usize)}
fn put(out:&mut [u8],at:&mut usize,bytes:&[u8]){out[*at..*at+bytes.len()].copy_from_slice(bytes);*at+=bytes.len();}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/Point.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2Point {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:i32,
  /// Declared enabled; canonical bounds are part of CODEC_IDENTITY.
  pub enabled:bool,
}
impl Default for Ros1Uwvm_ros_bounded_v2Point {fn default()->Self {Self {
value:Default::default(),
enabled:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2Point {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=4;
n+=1;
if n>5{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
put(out,at,&[u8::from(self.enabled)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.enabled=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2Point {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<5||bytes.len()>5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/Envelope.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2Envelope {
  /// Declared label; canonical bounds are part of CODEC_IDENTITY.
  pub label:String,
  /// Declared samples; canonical bounds are part of CODEC_IDENTITY.
  pub samples:Vec<u16>,
  /// Declared points; canonical bounds are part of CODEC_IDENTITY.
  pub points:Vec<Ros1Uwvm_ros_bounded_v2Point>,
  /// Declared labels; canonical bounds are part of CODEC_IDENTITY.
  pub labels:[String;2],
}
impl Default for Ros1Uwvm_ros_bounded_v2Envelope {fn default()->Self {Self {
label:Default::default(),
samples:Default::default(),
points:Default::default(),
labels:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Uwvm_ros_bounded_v2Envelope {
  pub const MAX_WIRE_BYTES:usize=120;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.label.len()>32||self.label.as_bytes().contains(&0){return Err(invalid());}n+=4+self.label.len();
if self.samples.len()>8{return Err(invalid());}n+=4;
for element in &self.samples {
n+=2;
}
if self.points.len()>4{return Err(invalid());}n+=4;
for element in &self.points {
n+=element.wire_size()?;
}
for element in &self.labels {
if element.len()>16||element.as_bytes().contains(&0){return Err(invalid());}n+=4+element.len();
}
if n>120{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.label.len() as u32).to_le_bytes());put(out,at,self.label.as_bytes());
put(out,at,&(self.samples.len() as u32).to_le_bytes());
for element in &self.samples {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.points.len() as u32).to_le_bytes());
for element in &self.points {
element.write_fields(out,at);
}
for element in &self.labels {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>32{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.label=decoded;
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.samples=Vec::with_capacity(count);
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.samples.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.points=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros1Uwvm_ros_bounded_v2Point::read_fields(bytes,at)?;
value.points.push(decoded);
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let size=read_count(bytes,at)?;if size>16{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.labels[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2Envelope {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<20||bytes.len()>120{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/EchoRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2EchoRequest {
  /// Declared sample; canonical bounds are part of CODEC_IDENTITY.
  pub sample:Ros1Uwvm_ros_bounded_v2Envelope,
  /// Declared limit; canonical bounds are part of CODEC_IDENTITY.
  pub limit:u32,
}
impl Default for Ros1Uwvm_ros_bounded_v2EchoRequest {fn default()->Self {Self {
sample:Default::default(),
limit:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2EchoRequest {
  pub const MAX_WIRE_BYTES:usize=124;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=self.sample.wire_size()?;
n+=4;
if n>124{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.sample.write_fields(out,at);
put(out,at,&self.limit.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Uwvm_ros_bounded_v2Envelope::read_fields(bytes,at)?;
value.sample=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.limit=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2EchoRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<24||bytes.len()>124{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/EchoResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2EchoResponse {
  /// Declared accepted; canonical bounds are part of CODEC_IDENTITY.
  pub accepted:bool,
  /// Declared echoed; canonical bounds are part of CODEC_IDENTITY.
  pub echoed:Ros1Uwvm_ros_bounded_v2Envelope,
}
impl Default for Ros1Uwvm_ros_bounded_v2EchoResponse {fn default()->Self {Self {
accepted:Default::default(),
echoed:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2EchoResponse {
  pub const MAX_WIRE_BYTES:usize=121;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=1;
n+=self.echoed.wire_size()?;
if n>121{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.accepted)]);
self.echoed.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.accepted=decoded;
let decoded=Ros1Uwvm_ros_bounded_v2Envelope::read_fields(bytes,at)?;
value.echoed=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2EchoResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<21||bytes.len()>121{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/EmptyRequest.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2EmptyRequest {
}
impl Default for Ros1Uwvm_ros_bounded_v2EmptyRequest {fn default()->Self {Self {
}}}
impl Ros1Uwvm_ros_bounded_v2EmptyRequest {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2EmptyRequest {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<0||bytes.len()>0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/EmptyResponse.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2EmptyResponse {
}
impl Default for Ros1Uwvm_ros_bounded_v2EmptyResponse {fn default()->Self {Self {
}}}
impl Ros1Uwvm_ros_bounded_v2EmptyResponse {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2EmptyResponse {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<0||bytes.len()>0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/DigestGoal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2DigestGoal {
  /// Declared sample; canonical bounds are part of CODEC_IDENTITY.
  pub sample:Ros1Uwvm_ros_bounded_v2Envelope,
}
impl Default for Ros1Uwvm_ros_bounded_v2DigestGoal {fn default()->Self {Self {
sample:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2DigestGoal {
  pub const MAX_WIRE_BYTES:usize=120;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=self.sample.wire_size()?;
if n>120{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.sample.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros1Uwvm_ros_bounded_v2Envelope::read_fields(bytes,at)?;
value.sample=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2DigestGoal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<20||bytes.len()>120{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/DigestResult.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2DigestResult {
  /// Declared accepted; canonical bounds are part of CODEC_IDENTITY.
  pub accepted:bool,
  /// Declared sum; canonical bounds are part of CODEC_IDENTITY.
  pub sum:u32,
  /// Declared note; canonical bounds are part of CODEC_IDENTITY.
  pub note:String,
}
impl Default for Ros1Uwvm_ros_bounded_v2DigestResult {fn default()->Self {Self {
accepted:Default::default(),
sum:Default::default(),
note:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2DigestResult {
  pub const MAX_WIRE_BYTES:usize=41;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=1;
n+=4;
if self.note.len()>32||self.note.as_bytes().contains(&0){return Err(invalid());}n+=4+self.note.len();
if n>41{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.accepted)]);
put(out,at,&self.sum.to_le_bytes());
put(out,at,&(self.note.len() as u32).to_le_bytes());put(out,at,self.note.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.accepted=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sum=decoded;
let size=read_count(bytes,at)?;if size>32{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.note=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2DigestResult {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<9||bytes.len()>41{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS1 uwvm_ros_bounded_v2/DigestFeedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_ros_bounded_v2DigestFeedback {
  /// Declared processed; canonical bounds are part of CODEC_IDENTITY.
  pub processed:u32,
  /// Declared phase; canonical bounds are part of CODEC_IDENTITY.
  pub phase:String,
}
impl Default for Ros1Uwvm_ros_bounded_v2DigestFeedback {fn default()->Self {Self {
processed:Default::default(),
phase:Default::default(),
}}}
impl Ros1Uwvm_ros_bounded_v2DigestFeedback {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=4;
if self.phase.len()>16||self.phase.as_bytes().contains(&0){return Err(invalid());}n+=4+self.phase.len();
if n>24{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.processed.to_le_bytes());
put(out,at,&(self.phase.len() as u32).to_le_bytes());put(out,at,self.phase.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.processed=decoded;
let size=read_count(bytes,at)?;if size>16{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.phase=decoded;
Ok(value)}
}
impl Canonical for Ros1Uwvm_ros_bounded_v2DigestFeedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<8||bytes.len()>24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/msg/Point.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2MsgPoint {
  /// Declared value; canonical bounds are part of CODEC_IDENTITY.
  pub value:i32,
  /// Declared enabled; canonical bounds are part of CODEC_IDENTITY.
  pub enabled:bool,
}
impl Default for Ros2Uwvm_ros_bounded_v2MsgPoint {fn default()->Self {Self {
value:Default::default(),
enabled:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2MsgPoint {
  pub const MAX_WIRE_BYTES:usize=5;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=4;
n+=1;
if n>5{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.value.to_le_bytes());
put(out,at,&[u8::from(self.enabled)]);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=i32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.value=decoded;
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.enabled=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2MsgPoint {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<5||bytes.len()>5{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/msg/Envelope.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2MsgEnvelope {
  /// Declared label; canonical bounds are part of CODEC_IDENTITY.
  pub label:String,
  /// Declared samples; canonical bounds are part of CODEC_IDENTITY.
  pub samples:Vec<u16>,
  /// Declared points; canonical bounds are part of CODEC_IDENTITY.
  pub points:Vec<Ros2Uwvm_ros_bounded_v2MsgPoint>,
  /// Declared labels; canonical bounds are part of CODEC_IDENTITY.
  pub labels:[String;2],
}
impl Default for Ros2Uwvm_ros_bounded_v2MsgEnvelope {fn default()->Self {Self {
label:Default::default(),
samples:Default::default(),
points:Default::default(),
labels:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Uwvm_ros_bounded_v2MsgEnvelope {
  pub const MAX_WIRE_BYTES:usize=120;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if self.label.len()>32||self.label.as_bytes().contains(&0){return Err(invalid());}n+=4+self.label.len();
if self.samples.len()>8{return Err(invalid());}n+=4;
for element in &self.samples {
n+=2;
}
if self.points.len()>4{return Err(invalid());}n+=4;
for element in &self.points {
n+=element.wire_size()?;
}
for element in &self.labels {
if element.len()>16||element.as_bytes().contains(&0){return Err(invalid());}n+=4+element.len();
}
if n>120{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&(self.label.len() as u32).to_le_bytes());put(out,at,self.label.as_bytes());
put(out,at,&(self.samples.len() as u32).to_le_bytes());
for element in &self.samples {
put(out,at,&element.to_le_bytes());
}
put(out,at,&(self.points.len() as u32).to_le_bytes());
for element in &self.points {
element.write_fields(out,at);
}
for element in &self.labels {
put(out,at,&(element.len() as u32).to_le_bytes());put(out,at,element.as_bytes());
}
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let size=read_count(bytes,at)?;if size>32{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.label=decoded;
{let count=read_count(bytes,at)?;if count>8{return Err(invalid());}
value.samples=Vec::with_capacity(count);
for i in 0..count {
let decoded=u16::from_le_bytes(take(bytes,at,2)?.try_into().map_err(|_|invalid())?);
value.samples.push(decoded);
}}
{let count=read_count(bytes,at)?;if count>4{return Err(invalid());}
value.points=Vec::with_capacity(count);
for i in 0..count {
let decoded=Ros2Uwvm_ros_bounded_v2MsgPoint::read_fields(bytes,at)?;
value.points.push(decoded);
}}
{let count=2usize;if count>2{return Err(invalid());}
for i in 0..count {
let size=read_count(bytes,at)?;if size>16{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.labels[i]=decoded;
}}
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2MsgEnvelope {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<20||bytes.len()>120{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/srv/Echo_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2SrvEcho_Request {
  /// Declared sample; canonical bounds are part of CODEC_IDENTITY.
  pub sample:Ros2Uwvm_ros_bounded_v2MsgEnvelope,
  /// Declared limit; canonical bounds are part of CODEC_IDENTITY.
  pub limit:u32,
}
impl Default for Ros2Uwvm_ros_bounded_v2SrvEcho_Request {fn default()->Self {Self {
sample:Default::default(),
limit:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2SrvEcho_Request {
  pub const MAX_WIRE_BYTES:usize=124;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=self.sample.wire_size()?;
n+=4;
if n>124{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.sample.write_fields(out,at);
put(out,at,&self.limit.to_le_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope::read_fields(bytes,at)?;
value.sample=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.limit=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2SrvEcho_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<24||bytes.len()>124{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/srv/Echo_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2SrvEcho_Response {
  /// Declared accepted; canonical bounds are part of CODEC_IDENTITY.
  pub accepted:bool,
  /// Declared echoed; canonical bounds are part of CODEC_IDENTITY.
  pub echoed:Ros2Uwvm_ros_bounded_v2MsgEnvelope,
}
impl Default for Ros2Uwvm_ros_bounded_v2SrvEcho_Response {fn default()->Self {Self {
accepted:Default::default(),
echoed:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2SrvEcho_Response {
  pub const MAX_WIRE_BYTES:usize=121;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=1;
n+=self.echoed.wire_size()?;
if n>121{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.accepted)]);
self.echoed.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.accepted=decoded;
let decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope::read_fields(bytes,at)?;
value.echoed=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2SrvEcho_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<21||bytes.len()>121{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/srv/Empty_Request.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {
}
impl Default for Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {fn default()->Self {Self {
}}}
impl Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2SrvEmpty_Request {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<0||bytes.len()>0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/srv/Empty_Response.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {
}
impl Default for Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {fn default()->Self {Self {
}}}
impl Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {
  pub const MAX_WIRE_BYTES:usize=0;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
if n>0{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2SrvEmpty_Response {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<0||bytes.len()>0{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/action/Digest_Goal.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {
  /// Declared sample; canonical bounds are part of CODEC_IDENTITY.
  pub sample:Ros2Uwvm_ros_bounded_v2MsgEnvelope,
}
impl Default for Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {fn default()->Self {Self {
sample:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {
  pub const MAX_WIRE_BYTES:usize=120;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=self.sample.wire_size()?;
if n>120{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
self.sample.write_fields(out,at);
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=Ros2Uwvm_ros_bounded_v2MsgEnvelope::read_fields(bytes,at)?;
value.sample=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2ActionDigest_Goal {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<20||bytes.len()>120{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/action/Digest_Result.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2ActionDigest_Result {
  /// Declared accepted; canonical bounds are part of CODEC_IDENTITY.
  pub accepted:bool,
  /// Declared sum; canonical bounds are part of CODEC_IDENTITY.
  pub sum:u32,
  /// Declared note; canonical bounds are part of CODEC_IDENTITY.
  pub note:String,
}
impl Default for Ros2Uwvm_ros_bounded_v2ActionDigest_Result {fn default()->Self {Self {
accepted:Default::default(),
sum:Default::default(),
note:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2ActionDigest_Result {
  pub const MAX_WIRE_BYTES:usize=41;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=1;
n+=4;
if self.note.len()>32||self.note.as_bytes().contains(&0){return Err(invalid());}n+=4+self.note.len();
if n>41{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&[u8::from(self.accepted)]);
put(out,at,&self.sum.to_le_bytes());
put(out,at,&(self.note.len() as u32).to_le_bytes());put(out,at,self.note.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;
value.accepted=decoded;
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.sum=decoded;
let size=read_count(bytes,at)?;if size>32{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.note=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2ActionDigest_Result {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<9||bytes.len()>41{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
/// Complete bounded value for ROS2 uwvm_ros_bounded_v2/action/Digest_Feedback.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {
  /// Declared processed; canonical bounds are part of CODEC_IDENTITY.
  pub processed:u32,
  /// Declared phase; canonical bounds are part of CODEC_IDENTITY.
  pub phase:String,
}
impl Default for Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {fn default()->Self {Self {
processed:Default::default(),
phase:Default::default(),
}}}
impl Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {
  pub const MAX_WIRE_BYTES:usize=24;
  pub const CODEC_IDENTITY:&'static str="UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1";
  /// Validate all fields and measure only their initialized canonical prefix.
  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;
n+=4;
if self.phase.len()>16||self.phase.as_bytes().contains(&0){return Err(invalid());}n+=4+self.phase.len();
if n>24{return Err(invalid());}Ok(n)}
  /// Encode after complete validation; output remains unchanged on refusal.
  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}
  fn write_fields(&self,out:&mut [u8],at:&mut usize){
put(out,at,&self.processed.to_le_bytes());
put(out,at,&(self.phase.len() as u32).to_le_bytes());put(out,at,self.phase.as_bytes());
}
  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();
let decoded=u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?);
value.processed=decoded;
let size=read_count(bytes,at)?;if size>16{return Err(invalid());}let raw=take(bytes,at,size)?;if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();
value.phase=decoded;
Ok(value)}
}
impl Canonical for Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback {
  /// Allocate one exact validated canonical output; never native serialization.
  fn encode(&self)->Result<Vec<u8>,Error>{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}
  fn decode(bytes:&[u8])->Result<Self,Error>{if bytes.len()<8||bytes.len()>24{return Err(invalid());}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){return Err(invalid());}Ok(value)}
}
