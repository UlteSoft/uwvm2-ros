//! Actual UMRE256/UMRC160/URMI192 bounded little-endian parsing. No RMW/native
//! struct layout or pointer cast is guessed. Real zero GID/timestamps are data.
use roswasm::{Error,codes};
pub const ENDPOINT:usize=256;
pub const CONTROL:usize=160;
pub const METADATA:usize=192;
pub const OVERHEAD:usize=352;
fn invalid()->Error{Error::new(codes::INVALID)}
fn le(b:&[u8],at:usize,n:usize)->u64{let mut v=0;for i in 0..n{v|=(b[at+i]as u64)<<(8*i);}v}
fn zero(b:&[u8])->bool{b.iter().all(|v|*v==0)}
/// A full Runtime-issued generation/index capability handle, not a pointer.
pub fn full_handle(h:u64)->bool{h as u32!=0&&(h>>32)as u32!=0}
/// Actual normalized finite/saturated time representation in a QoS receipt.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Time{pub sec:u64,pub nanosec:u64}
/// Both independently queried native QoS profiles are retained. These values
/// report the admitted native endpoint; Guest values never authorize/change it.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Qos{pub history:u32,pub reliability:u32,pub durability:u32,pub liveliness:u32,pub depth:u64,pub deadline:Time,pub lifespan:Time,pub lease:Time,pub avoid_ros_namespace_conventions:bool}
fn qos(b:&[u8])->Result<Qos,Error>{
 let q=Qos{history:le(b,0,4)as u32,reliability:le(b,4,4)as u32,durability:le(b,8,4)as u32,liveliness:le(b,12,4)as u32,depth:le(b,16,8),deadline:Time{sec:le(b,24,8),nanosec:le(b,32,8)},lifespan:Time{sec:le(b,40,8),nanosec:le(b,48,8)},lease:Time{sec:le(b,56,8),nanosec:le(b,64,8)},avoid_ros_namespace_conventions:b[72]!=0};
 if !(1..=2).contains(&q.history)||!(1..=2).contains(&q.reliability)||!(1..=2).contains(&q.durability)||!(1..=2).contains(&q.liveliness)||(q.history==1&&q.depth==0)||q.deadline.nanosec>=1_000_000_000||q.lifespan.nanosec>=1_000_000_000||q.lease.nanosec>=1_000_000_000||b[72]>1||!zero(&b[73..80]){return Err(invalid());}Ok(q)
}
/// Full source/incarnation binding copied from actual authenticated receipt.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Binding{pub source:u64,pub instance:u64,pub runtime_generation:u64,pub session:u64,pub incarnation:u64}
impl Binding{fn matches(&self,b:&[u8])->bool{b[7]==2&&le(b,16,8)==self.source&&le(b,24,8)==self.instance&&le(b,32,8)==self.runtime_generation&&le(b,40,8)==self.session&&le(b,48,8)==self.incarnation}}
/// Original UMRE value. An absent client GID is explicitly extent0; a present
/// all-zero GID is permitted, and neither is fabricated as a request writer GUID.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Receipt{pub binding:Binding,pub role:u8,pub qos:[Qos;2],pub native_gid_bytes:u8,pub native_gid:[u8;32]}
pub fn receipt(b:&[u8;256],operation:u8,role:u8,source:u64)->Result<Receipt,Error>{
 if le(b,0,4)!=0x45524d55||le(b,4,2)!=1||b[6]!=operation||b[7]!=2||le(b,8,4)!=256||b[12]!=role||!(role==3||role==4)||b[13]!=2||b[14]>32||b[15]!=0||!zero(&b[56..64])||(role==4&&b[14]!=0)||!zero(&b[224+b[14]as usize..256]){return Err(invalid());}
 let binding=Binding{source:le(b,16,8),instance:le(b,24,8),runtime_generation:le(b,32,8),session:le(b,40,8),incarnation:le(b,48,8)};
 if source!=binding.source||!full_handle(source)||binding.instance==0||binding.runtime_generation==0||binding.session==0||binding.incarnation==0{return Err(invalid());}
 let mut gid=[0;32];gid.copy_from_slice(&b[224..256]);Ok(Receipt{binding,role,qos:[qos(&b[64..144])?,qos(&b[144..224])?],native_gid_bytes:b[14],native_gid:gid})
}
/// Same-take service metadata. Native sequence and times retain their signed
/// bits; the Gateway ticket and accepted-stream sequence are separate identities.
#[derive(Clone,Copy,Debug,Default,PartialEq,Eq)]
pub struct ServiceInfo{pub source_timestamp:i64,pub received_timestamp:i64,pub native_sequence:i64,pub writer_guid:[u8;16]}
/// A complete authenticated borrow prefix, sufficient only for exact cancel/ACK.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Borrow{pub sequence:u64,pub reservation:u64,pub ticket:u64,pub body_bytes:usize}
/// A real source control record has no invented native service metadata.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct ControlEvent{pub kind:u8,pub flags:u8,pub error:u32,pub ticket:u64}
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub enum Information{Service(ServiceInfo),Control(ControlEvent)}
pub fn envelope(b:&[u8],r:&Receipt,peer:u64,maximum:usize)->Result<Borrow,Error>{
 if b.len()<OVERHEAD||le(b,0,4)!=0x43524d55||le(b,4,2)!=1||b[6]!=5||le(b,8,4)!=160||b[12]!=r.role||b[13]!=16||!zero(&b[14..16])||!zero(&b[128..160])||!r.binding.matches(b)||le(b,56,8)!=peer||le(b,72,8)==0||le(b,80,8)==0||le(b,88,8)!=0||le(b,96,8)!=b.len()as u64||le(b,112,8)!=0||le(b,120,8)!=0{return Err(invalid());}
 let n=le(b,104,8);if n<192||n>(b.len()-CONTROL)as u64||n-192>maximum as u64{return Err(Error::new(codes::TOO_LARGE));}
 Ok(Borrow{sequence:le(b,72,8),reservation:le(b,80,8),ticket:le(b,64,8),body_bytes:(n-192)as usize})
}
pub fn information(packet:&[u8],r:&Receipt,e:Borrow)->Result<Information,Error>{
 if packet.len()<OVERHEAD{return Err(invalid());}let b=&packet[CONTROL..OVERHEAD];
 if le(b,0,4)!=0x494d5255||le(b,4,2)!=1||le(b,8,4)!=192||!r.binding.matches(b)||le(b,56,8)!=e.sequence||le(b,64,8)!=e.ticket||le(b,112,8)!=e.body_bytes as u64||b[123]!=0||!zero(&b[176..192]){return Err(invalid());}
 let kind=b[6];let flags=le(b,12,4);let control=kind==6||kind==7;
 if control{
  let c=ControlEvent{kind,flags:b[122],error:le(b,124,4)as u32,ticket:e.ticket};
  if flags!=0||e.body_bytes!=0||b[120]!=0||b[121]!=0||!zero(&b[128..176])||le(b,72,8)!=0||le(b,80,8)!=0||le(b,88,8)!=u64::MAX||le(b,96,8)!=u64::MAX||le(b,104,8)!=0||c.flags&!3!=0||(kind==6&&c.error==0)||(kind==7&&c.flags&1==0){return Err(invalid());}return Ok(Information::Control(c));
 }
 if b[121]!=16{return Err(Error::new(codes::UNSUPPORTED));}
 if kind!=(if r.role==3{3}else{2})||flags!=35||e.ticket==0||le(b,124,4)!=0||b[122]!=0||b[120]!=0||!zero(&b[128..160])||le(b,88,8)!=u64::MAX||le(b,96,8)!=u64::MAX{return Err(invalid());}
 let mut writer=[0;16];writer.copy_from_slice(&b[160..176]);Ok(Information::Service(ServiceInfo{source_timestamp:le(b,72,8)as i64,received_timestamp:le(b,80,8)as i64,native_sequence:le(b,104,8)as i64,writer_guid:writer}))
}
