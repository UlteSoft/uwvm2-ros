//! Genuine optional owned-RMW ROS2 Service transport. No legacy business RPC.
//!
//! A caller-driven, process-local Group owns all aliases, actual native receipts,
//! nonreused ticket history and scratch before first open. It is !Send/!Sync.
//! Only the explicit unsafe Transport seam permits future model injection; the
//! normal Wasm backend performs the exact existing numeric raw12 imports.
//! Neither native type/codec strings nor numerical receipt values grant rights.
#![no_std]
extern crate alloc;
use alloc::{rc::Rc,vec::Vec};
use core::{cell::Cell,marker::PhantomData,mem::size_of};
use roswasm::{Error,codes,typed::Canonical};
pub mod raw;
pub mod wire;
pub use raw::Wasm;
pub use wire::{Receipt,ServiceInfo,ControlEvent};
fn err(code:u32)->Error{Error::new(code)}
fn codec_error(e:Error)->Error{if e.code==0{err(22)}else{e}}
fn service_name(name:&str)->bool{let mut p=name.split('/');let a=p.next().unwrap_or("");let middle=p.next();let leaf=p.next().unwrap_or("");p.next().is_none()&&middle==Some("srv")&&a.as_bytes().first().map_or(false,|v|v.is_ascii_lowercase())&&a.bytes().all(|v|v.is_ascii_lowercase()||v.is_ascii_digit()||v==b'_')&&leaf.as_bytes().first().map_or(false,|v|v.is_ascii_uppercase())&&leaf.bytes().all(|v|v.is_ascii_alphanumeric()||v==b'_')}
fn status(code:u32)->Result<(),Error>{if code==0{Ok(())}else{Err(err(code))}}
fn allocated<T:Default>(n:usize)->Result<Vec<T>,Error>{let mut v=Vec::new();v.try_reserve_exact(n).map_err(|_|err(12))?;v.resize_with(n,T::default);Ok(v)}

/// Trusted synchronous transport contract, NOT an authority or plugin ABI.
///
/// # Safety
/// Every method must perform at most the named genuine operation, keep all
/// borrowed slices alive during its call, retain no pointer and never invoke
/// Guest callbacks. Nonzero errors preserve output. A model implementation is
/// local-test evidence only and cannot establish Guest/native qualification.
/// Implementations must not panic across an imported/retained native effect.
pub unsafe trait Transport {
 fn supported(&self)->bool;
 fn version(&mut self)->u32;
 fn business_version(&mut self)->u32;
 fn open(&mut self,alias:&[u8],out:&mut[u8;8])->u32;
 fn close(&mut self,handle:u64)->u32;
 fn activate(&mut self,role:u8,source:u64,peer:u64,out:&mut[u8;256])->u32;
 fn inspect(&mut self,source:u64,incarnation:u64,out:&mut[u8;256])->u32;
 fn submit(&mut self,source:u64,incarnation:u64,body:&[u8],out:&mut[u8;8])->u32;
 fn abandon(&mut self,source:u64,incarnation:u64,ticket:u64)->u32;
 fn respond(&mut self,source:u64,incarnation:u64,peer:u64,ticket:u64,body:&[u8])->u32;
 fn peek(&mut self,source:u64,incarnation:u64,out:&mut[u8])->u32;
 fn commit(&mut self,source:u64,incarnation:u64,sequence:u64,reservation:u64)->u32;
 fn cancel(&mut self,source:u64,incarnation:u64,sequence:u64,reservation:u64)->u32;
 fn available(&mut self,source:u64,incarnation:u64,query:u64,wait_ns:u64,out:&mut[u8;1])->u32;
 fn retire(&mut self,source:u64,incarnation:u64)->u32;
}
/// Exact owned codec adapter. A safe decoder cannot borrow its input through
/// this fixed-T function pointer. Return full private values/errors, not defaults.
pub struct Codec<T>{pub maximum:usize,pub encode:fn(&T)->Result<Vec<u8>,Error>,pub decode:fn(&[u8])->Result<T,Error>}
impl<T:Canonical> Codec<T>{pub fn canonical(maximum:usize)->Self{Self{maximum,encode:T::encode,decode:T::decode}}}
/// The genuine Service ROOT identity is independent of Request/Response IDs.
/// Host policy/type support independently proves these declared source facts.
pub struct Definition<Q,S>{pub native_type:&'static str,pub canonical_identity:&'static str,pub request:Codec<Q>,pub response:Codec<S>}
impl<Q,S> Definition<Q,S>{fn valid(&self,c:Config)->bool{
 let n=self.native_type.as_bytes();let id=self.canonical_identity.as_bytes();
 !n.is_empty()&&n.len()<=255&&n.iter().all(|b|b.is_ascii_alphanumeric()||*b==b'_'||*b==b'/')&&service_name(self.native_type)&&id.len()==73&&id.starts_with(b"UWVMROS1_")&&id[9..].iter().all(|b|b.is_ascii_digit()||(*b>=b'a'&&*b<=b'f'))&&self.request.maximum<=c.maximum_request&&self.response.maximum<=c.maximum_response
}}
/// All permanent storage ceilings are explicit. There are no default aliases,
/// clocks, policy presets or endpoint opens in this value.
#[derive(Clone,Copy,Debug)]
pub struct Config{pub endpoints:usize,pub transactions_per_endpoint:usize,pub maximum_request:usize,pub maximum_response:usize}
impl Config{fn valid(self)->bool{self.endpoints>0&&self.endpoints<=256&&self.transactions_per_endpoint>0&&self.transactions_per_endpoint<=65536&&self.maximum_request<=32768&&self.maximum_response<=32768&&self.endpoints.checked_mul(self.transactions_per_endpoint).and_then(|n|n.checked_mul(size_of::<Transaction>())).map_or(false,|n|n<=64*1024*1024)}}
/// Every cleanup result is separately retained, including a real attempted0.
#[derive(Clone,Copy,Debug,Default,PartialEq,Eq)]
pub struct Attempt{pub attempted:bool,pub code:u32}
impl Attempt{fn observed(code:u32)->Self{Self{attempted:true,code}}}
/// Preallocated per-attempt journal. `primary` never gets replaced by cleanup.
#[derive(Clone,Copy,Debug,Default)]
pub struct Diagnostic{
 pub source:u64,pub peer:u64,pub query:u64,pub primary:u32,pub role:u8,
 pub open:[Attempt;3],pub activation:Attempt,pub activation_validation:Attempt,
 pub inspect:Attempt,pub inspect_validation:Attempt,pub submit:Attempt,pub respond:Attempt,
 pub peek:Attempt,pub decode:Attempt,pub commit:Attempt,pub cancel:Attempt,pub abandon:Attempt,pub available:Attempt,
 pub native_retire:Attempt,pub raw_close:[Attempt;3],pub closed:bool,pub poisoned:bool,
 pub submissions:u64,pub responses:u64,pub peeks:u64,pub commits:u64,pub cancels:u64,
 pub last_sequence:u64,pub last_reservation:u64,pub last_committed_sequence:u64,pub last_ticket:u64,pub history_used:usize,
}
/// Native once-retire and reverse raw-owner closes, with no hidden retry.
#[derive(Clone,Copy,Debug,Default)]
pub struct CloseReport{pub primary:u32,pub native_retire:Attempt,pub raw_close:[Attempt;3]}
#[derive(Clone,Copy)]
struct Alias{bytes:[u8;255],len:u8}
impl Default for Alias{fn default()->Self{Self{bytes:[0;255],len:0}}}
impl Alias{fn checked(s:&str)->Result<Self,Error>{if s.is_empty()||s.len()>255||s.as_bytes().contains(&0){return Err(err(22));}let mut a=Self::default();a.bytes[..s.len()].copy_from_slice(s.as_bytes());a.len=s.len()as u8;Ok(a)}fn same(&self,b:&Self)->bool{self.len!=0&&self.len==b.len&&self.bytes[..self.len as usize]==b.bytes[..b.len as usize]}}
#[derive(Clone,Copy,Debug,Default,PartialEq,Eq)]
enum State{#[default]Unused,Reserved,Waiting,Delivered,Done,Abandoned,Discarded}
#[derive(Clone,Copy,Debug,Default)]
struct Transaction{ticket:u64,state:State,error:u32,info:ServiceInfo}
struct Slot{aliases:[Alias;3],handles:[u64;3],receipt:Option<Receipt>,wire:[u8;256],history:Vec<Transaction>,used:usize,diagnostic:Diagnostic}
impl Slot{fn new(history:usize)->Result<Self,Error>{Ok(Self{aliases:[Alias::default();3],handles:[0;3],receipt:None,wire:[0;256],history:allocated(history)?,used:0,diagnostic:Diagnostic::default()})}}
#[derive(Clone,Copy,Debug)]
struct Endpoint{index:usize,source:u64}
/// Opaque local correlation token. It is not a native sequence or permission.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Ticket{source:u64,value:u64}
impl Ticket{pub fn value(self)->u64{self.value}}
/// Local request status; response data arrives only by real committed delivery.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub enum RequestState{Waiting,Completed{error:u32},Abandoned}
/// A fully decoded committed native record or explicit body-free control.
#[derive(Debug)]
pub enum Delivery<T>{Value{ticket:Ticket,value:T,info:ServiceInfo},Control(ControlEvent)}
enum Prepared<T>{Value{value:T,info:ServiceInfo,index:usize,abandoned:bool},Control{event:ControlEvent,index:Option<usize>}}
/// Explicit client view. Group owns all native/raw state; dropping this view
/// cannot silently reopen/close or make an admitted request complete.
pub struct Client<Q,S>{endpoint:Endpoint,definition:Definition<Q,S>}
pub struct Server<Q,S>{endpoint:Endpoint,definition:Definition<Q,S>}
/// Fully preallocated synchronous group; safe Rust prevents concurrent/reentrant
/// mutable operations. Rc marker intentionally excludes Send/Sync. No host lock
/// or borrowed native pointer escapes an individual import.
pub struct Group<B:Transport=Wasm>{transport:B,config:Config,slots:Vec<Slot>,used:usize,packet:Vec<u8>,receipt:[u8;256],ticket:[u8;8],callback:Cell<bool>,shutdown:bool,_local:PhantomData<Rc<()>>}
impl Group<Wasm>{pub fn new(config:Config)->Result<Self,Error>{Self::prepare(config,Wasm)}}
impl<B:Transport> Group<B>{
 /// Construct the explicit trusted backend seam. The safety promises of its
 /// implementation remain the caller's obligation; models are not Native proof.
 pub fn prepare(config:Config,transport:B)->Result<Self,Error>{
  if !config.valid(){return Err(err(22));}if !transport.supported(){return Err(err(95));}
  let mut slots=Vec::new();slots.try_reserve_exact(config.endpoints).map_err(|_|err(12))?;
  for _ in 0..config.endpoints{slots.push(Slot::new(config.transactions_per_endpoint)?);}
  let maximum=core::cmp::max(config.maximum_request,config.maximum_response);let packet=allocated(wire::OVERHEAD+maximum)?;
  Ok(Self{transport,config,slots,used:0,packet,receipt:[0;256],ticket:[0;8],callback:Cell::new(false),shutdown:false,_local:PhantomData})
 }
 fn idle(&self)->Result<(),Error>{if self.callback.get(){Err(err(17))}else if self.shutdown{Err(err(125))}else{Ok(())}}
 // Physical cleanup ownership remains available after a terminal control ACK.
 // `closed` means actual once-cleanup ran; `poisoned` alone is local terminal.
 fn owner_index(&self,e:Endpoint)->Result<usize,Error>{self.idle()?;if e.index>=self.used||e.source==0||self.slots[e.index].diagnostic.source!=e.source{return Err(err(9));}if self.slots[e.index].diagnostic.closed{return Err(err(125));}Ok(e.index)}
 fn index(&self,e:Endpoint)->Result<usize,Error>{let i=self.owner_index(e)?;if self.slots[i].diagnostic.poisoned{return Err(err(125));}Ok(i)}
 fn primary(&mut self,i:usize,code:u32){if code!=0&&self.slots[i].diagnostic.primary==0{self.slots[i].diagnostic.primary=code;}}
 fn attempt_close(&mut self,i:usize)->CloseReport{
  if !self.slots[i].diagnostic.closed{
   self.slots[i].diagnostic.closed=true;
   if let Some(r)=self.slots[i].receipt{if !self.slots[i].diagnostic.native_retire.attempted{let code=self.transport.retire(r.binding.source,r.binding.incarnation);self.slots[i].diagnostic.native_retire=Attempt::observed(code);}}
   for owner in(0..3).rev(){let h=self.slots[i].handles[owner];self.slots[i].handles[owner]=0;if h!=0{let code=self.transport.close(h);self.slots[i].diagnostic.raw_close[owner]=Attempt::observed(code);}}
   let used=self.slots[i].used;for t in &mut self.slots[i].history[..used]{if t.state==State::Waiting||t.state==State::Delivered||t.state==State::Reserved{t.state=State::Abandoned;t.error=125;}}
  }
  let d=self.slots[i].diagnostic;CloseReport{primary:d.primary,native_retire:d.native_retire,raw_close:d.raw_close}
 }
 fn terminal(&mut self,i:usize,code:u32)->Error{self.primary(i,code);self.slots[i].diagnostic.poisoned=true;self.attempt_close(i);err(code)}
 fn open(&mut self,role:u8,primary:&str,peer:&str,query:Option<&str>)->Result<Endpoint,Error>{
  self.idle()?;let aliases=[Alias::checked(primary)?,Alias::checked(peer)?,match query{Some(v)=>Alias::checked(v)?,None=>Alias::default()}];
  if role!=3&&role!=4{return Err(err(95));}if role==4&&query.is_some(){return Err(err(22));}
  for a in 0..3{for b in 0..a{if aliases[a].same(&aliases[b]){return Err(err(22));}}for old in &self.slots[..self.used]{if old.aliases.iter().any(|v|aliases[a].same(v)){return Err(err(13));}}}
  if self.used==self.config.endpoints{return Err(err(90));}
  // No source row exists on ABI failure; both queries happen before any open.
  if self.transport.business_version()!=1||self.transport.version()!=1{return Err(err(95));}
  let i=self.used;self.used+=1;self.slots[i].aliases=aliases;self.slots[i].diagnostic.role=role;
  for owner in 0..3{let a=self.slots[i].aliases[owner];if a.len==0{continue;}self.ticket.fill(0);let code=self.transport.open(&a.bytes[..a.len as usize],&mut self.ticket);self.slots[i].diagnostic.open[owner]=Attempt::observed(code);if code!=0{return Err(self.terminal(i,code));}let h=u64::from_le_bytes(self.ticket);if !wire::full_handle(h)||self.slots[i].handles.contains(&h){return Err(self.terminal(i,22));}self.slots[i].handles[owner]=h;match owner{0=>self.slots[i].diagnostic.source=h,1=>self.slots[i].diagnostic.peer=h,_=>self.slots[i].diagnostic.query=h}}
  self.receipt.fill(0xa5);let source=self.slots[i].handles[0];let paired=self.slots[i].handles[1];let code=self.transport.activate(role,source,paired,&mut self.receipt);self.slots[i].diagnostic.activation=Attempt::observed(code);if code!=0{return Err(self.terminal(i,code));}
  match wire::receipt(&self.receipt,1,role,source){Ok(r)=>{self.slots[i].receipt=Some(r);self.slots[i].wire=self.receipt;self.slots[i].diagnostic.activation_validation=Attempt::observed(0);Ok(Endpoint{index:i,source})},Err(e)=>{self.slots[i].diagnostic.activation_validation=Attempt::observed(e.code);Err(self.terminal(i,e.code))}}
 }
 pub fn client<Q,S>(&mut self,definition:Definition<Q,S>,call:&str,observer:&str,availability:Option<&str>)->Result<Client<Q,S>,Error>{if !definition.valid(self.config){return Err(err(22));}let endpoint=self.open(3,call,observer,availability)?;Ok(Client{endpoint,definition})}
 pub fn server<Q,S>(&mut self,definition:Definition<Q,S>,advertise:&str,responder:&str)->Result<Server<Q,S>,Error>{if !definition.valid(self.config){return Err(err(22));}let endpoint=self.open(4,advertise,responder,None)?;Ok(Server{endpoint,definition})}
 fn inspect(&mut self,i:usize)->Result<Receipt,Error>{
  let r=self.slots[i].receipt.ok_or(err(9))?;self.receipt.fill(0xa5);let code=self.transport.inspect(r.binding.source,r.binding.incarnation,&mut self.receipt);self.slots[i].diagnostic.inspect=Attempt::observed(code);if code!=0{return Err(self.terminal(i,code));}
  let parsed=wire::receipt(&self.receipt,2,r.role,r.binding.source);let code=match parsed{Err(e)=>e.code,Ok(v)=>if v==r&&self.receipt[..6]==self.slots[i].wire[..6]&&self.receipt[7..]==self.slots[i].wire[7..]{0}else{13}};self.slots[i].diagnostic.inspect_validation=Attempt::observed(code);if code!=0{return Err(self.terminal(i,code));}Ok(r)
 }
 fn history(&self,i:usize,ticket:u64)->Option<usize>{self.slots[i].history[..self.slots[i].used].iter().position(|t|ticket!=0&&t.ticket==ticket)}
 fn ticket_index(&self,i:usize,t:Ticket)->Result<usize,Error>{if t.source!=self.slots[i].diagnostic.source{return Err(err(9));}self.history(i,t.value).ok_or(err(9))}
 fn cancel(&mut self,i:usize,r:Receipt,b:wire::Borrow,reason:u32)->Error{
  self.primary(i,reason);self.slots[i].diagnostic.cancels+=1;let code=self.transport.cancel(r.binding.source,r.binding.incarnation,b.sequence,b.reservation);self.slots[i].diagnostic.cancel=Attempt::observed(code);self.terminal(i,reason)
 }
 fn take<T>(&mut self,e:Endpoint,codec:&Codec<T>,client:bool)->Result<Option<Delivery<T>>,Error>{
  let i=self.index(e)?;let d=self.slots[i].diagnostic;
  if d.role!=(if client{3}else{4}){return Err(err(9));}
  if d.peeks==u64::MAX||d.commits==u64::MAX||d.cancels==u64::MAX||d.last_committed_sequence==u64::MAX||d.last_reservation==u64::MAX||(!client&&self.slots[i].used==self.config.transactions_per_endpoint){return Err(err(90));}
  let r=self.inspect(i)?;let capacity=wire::OVERHEAD+codec.maximum;self.packet[..capacity].fill(0xa5);self.slots[i].diagnostic.peeks+=1;
  let code=self.transport.peek(r.binding.source,r.binding.incarnation,&mut self.packet[..capacity]);self.slots[i].diagnostic.peek=Attempt::observed(code);
  if code==11{return Ok(None);}if code!=0{return Err(self.terminal(i,code));}
  let b=match wire::envelope(&self.packet[..capacity],&r,self.slots[i].diagnostic.peer,codec.maximum){Ok(v)=>v,Err(e)=>return Err(self.terminal(i,e.code))};
  if b.sequence<=d.last_committed_sequence||(client&&b.sequence!=d.last_committed_sequence+1)||b.reservation<=d.last_reservation{return Err(self.cancel(i,r,b,13));}
  self.slots[i].diagnostic.last_sequence=b.sequence;self.slots[i].diagnostic.last_reservation=b.reservation;
  let info=match wire::information(&self.packet[..capacity],&r,b){Ok(v)=>v,Err(e)=>{self.slots[i].diagnostic.decode=Attempt::observed(e.code);return Err(self.cancel(i,r,b,e.code));}};
  let existing=self.history(i,b.ticket);
  let prepared=match info{
   wire::Information::Service(native)=>{
    let(index,abandoned)=if client{match existing{Some(t)if matches!(self.slots[i].history[t].state,State::Waiting|State::Abandoned)=>(t,self.slots[i].history[t].state==State::Abandoned),_=>return Err(self.cancel(i,r,b,13))}}
    else{if existing.is_some()||self.slots[i].history[..self.slots[i].used].iter().any(|t|t.info.writer_guid==native.writer_guid&&t.info.native_sequence==native.native_sequence){return Err(self.cancel(i,r,b,13));}(self.slots[i].used,false)};
    // Full typed decode is prepared BEFORE ACK, even for an abandoned response.
    let value=match(codec.decode)(&self.packet[wire::OVERHEAD..wire::OVERHEAD+b.body_bytes]){Ok(v)=>v,Err(e)=>{let e=codec_error(e);self.slots[i].diagnostic.decode=Attempt::observed(e.code);return Err(self.cancel(i,r,b,e.code));}};
    Prepared::Value{value,info:native,index,abandoned}
   },
   wire::Information::Control(event)=>{
    if b.ticket!=0&&existing.map_or(true,|t|if client{!matches!(self.slots[i].history[t].state,State::Waiting|State::Abandoned)}else{self.slots[i].history[t].state!=State::Delivered}){return Err(self.cancel(i,r,b,13));}
    Prepared::Control{event,index:existing}
   }
  };
  self.slots[i].diagnostic.decode=Attempt::observed(0);self.slots[i].diagnostic.commits+=1;
  let code=self.transport.commit(r.binding.source,r.binding.incarnation,b.sequence,b.reservation);self.slots[i].diagnostic.commit=Attempt::observed(code);if code!=0{return Err(self.terminal(i,code));}
  self.slots[i].diagnostic.last_committed_sequence=b.sequence;self.slots[i].diagnostic.last_ticket=b.ticket;
  // ACK succeeded. No validation, allocation or fallible preparation remains.
  match prepared{
   Prepared::Value{value,info,index,abandoned}=>{
    if client{self.slots[i].history[index].info=info;if abandoned{self.slots[i].history[index].state=State::Discarded;return Ok(None);}self.slots[i].history[index].state=State::Done;}
    else{self.slots[i].used+=1;self.slots[i].history[index]=Transaction{ticket:b.ticket,state:State::Delivered,error:0,info};self.slots[i].diagnostic.history_used=self.slots[i].used;}
    Ok(Some(Delivery::Value{ticket:Ticket{source:e.source,value:b.ticket},value,info}))
   },
   Prepared::Control{event,index}=>{
    if let Some(t)=index{self.slots[i].history[t].state=State::Done;self.slots[i].history[t].error=if event.error!=0{event.error}else{125};}
    if event.flags&1!=0{
     // A committed control is a complete result. No import may follow its ACK.
     // Defer genuine once-retire/raw closes to explicit close/shutdown/Drop.
     self.primary(i,if event.error!=0{event.error}else{125});self.slots[i].diagnostic.poisoned=true;
     let used=self.slots[i].used;for t in &mut self.slots[i].history[..used]{if matches!(t.state,State::Waiting|State::Delivered|State::Reserved){t.state=State::Abandoned;t.error=125;}}
    }
    Ok(Some(Delivery::Control(event)))
   }
  }
 }
 pub fn diagnostics(&self,index:usize)->Option<Diagnostic>{if index<self.used{Some(self.slots[index].diagnostic)}else{None}}
 pub fn attempted_endpoints(&self)->usize{self.used}
 /// Reject callback reentry before any mutation; retire all entries in reverse,
 /// and retain every failure in existing preallocated endpoint diagnostics.
 pub fn shutdown(&mut self)->Result<(),Error>{if self.callback.get(){return Err(err(17));}self.shutdown=true;let mut first=0;for i in(0..self.used).rev(){let r=self.attempt_close(i);for a in core::iter::once(r.native_retire).chain(r.raw_close.into_iter()){if first==0&&a.attempted&&a.code!=0{first=a.code;}}}status(first)}
}
impl<B:Transport> Drop for Group<B>{fn drop(&mut self){let _=self.shutdown();}}
impl<Q,S> Client<Q,S>{
 pub fn receipt<B:Transport>(&self,g:&Group<B>)->Result<Receipt,Error>{let i=g.index(self.endpoint)?;g.slots[i].receipt.ok_or(err(9))}
 pub fn submit<B:Transport>(&self,g:&mut Group<B>,request:&Q)->Result<Ticket,Error>{
  let i=g.index(self.endpoint)?;if g.slots[i].used==g.config.transactions_per_endpoint||g.slots[i].diagnostic.submissions==u64::MAX{return Err(err(90));}
  let body=(self.definition.request.encode)(request).map_err(codec_error)?;if body.len()>self.definition.request.maximum{return Err(err(90));}let r=g.inspect(i)?;
  let t=g.slots[i].used;g.slots[i].used+=1;g.slots[i].history[t].state=State::Reserved;g.slots[i].diagnostic.history_used=g.slots[i].used;g.slots[i].diagnostic.submissions+=1;g.ticket.fill(0);
  let code=g.transport.submit(r.binding.source,r.binding.incarnation,&body,&mut g.ticket);g.slots[i].diagnostic.submit=Attempt::observed(code);if code!=0{return Err(g.terminal(i,code));}
  let ticket=u64::from_le_bytes(g.ticket);if ticket==0||g.history(i,ticket).is_some(){return Err(g.terminal(i,22));}
  g.slots[i].history[t]=Transaction{ticket,state:State::Waiting,error:0,info:ServiceInfo::default()};g.slots[i].diagnostic.last_ticket=ticket;Ok(Ticket{source:self.endpoint.source,value:ticket})
 }
 pub fn poll<B:Transport>(&self,g:&mut Group<B>)->Result<Option<Delivery<S>>,Error>{g.take(self.endpoint,&self.definition.response,true)}
 pub fn state<B:Transport>(&self,g:&Group<B>,ticket:Ticket)->Result<RequestState,Error>{let i=g.index(self.endpoint)?;let t=g.ticket_index(i,ticket)?;match g.slots[i].history[t].state{State::Waiting=>Ok(RequestState::Waiting),State::Abandoned|State::Discarded=>Ok(RequestState::Abandoned),State::Done=>Ok(RequestState::Completed{error:g.slots[i].history[t].error}),_=>Err(err(9))}}
 /// Local abandonment only. The complete tombstone remains; this never claims
 /// native DDS cancellation and never makes the original request replay-safe.
 pub fn abandon<B:Transport>(&self,g:&mut Group<B>,ticket:Ticket)->Result<(),Error>{let i=g.index(self.endpoint)?;let t=g.ticket_index(i,ticket)?;if g.slots[i].history[t].state!=State::Waiting{return Err(err(9));}let r=g.inspect(i)?;g.slots[i].history[t].state=State::Abandoned;let code=g.transport.abandon(r.binding.source,r.binding.incarnation,ticket.value);g.slots[i].diagnostic.abandon=Attempt::observed(code);if code!=0{return Err(g.terminal(i,code));}Ok(())}
 /// An explicit third owner queries THIS retained client. Host/native bounded
 /// waiting is the only clock used here. Positive95 remains95; no sleep/graph
 /// fallback or automatic retry is performed. Successful output must write0/1.
 pub fn available<B:Transport>(&self,g:&mut Group<B>,wait_ns:u64)->Result<bool,Error>{let i=g.index(self.endpoint)?;let query=g.slots[i].handles[2];if query==0{return Err(err(13));}if wait_ns>60_000_000_000{return Err(err(22));}let r=g.inspect(i)?;let mut output=[0xa5];let code=g.transport.available(r.binding.source,r.binding.incarnation,query,wait_ns,&mut output);g.slots[i].diagnostic.available=Attempt::observed(code);if code!=0{g.primary(i,code);return Err(err(code));}if output[0]>1{return Err(g.terminal(i,22));}Ok(output[0]!=0)}
 /// Release this actual endpoint even after a locally terminal control ACK.
 /// Cleanup is a separate caller operation; its results never replace control.
 pub fn close<B:Transport>(&self,g:&mut Group<B>)->Result<CloseReport,Error>{let i=g.owner_index(self.endpoint)?;Ok(g.attempt_close(i))}
}
impl<Q,S> Server<Q,S>{
 pub fn receipt<B:Transport>(&self,g:&Group<B>)->Result<Receipt,Error>{let i=g.index(self.endpoint)?;g.slots[i].receipt.ok_or(err(9))}
 pub fn poll<B:Transport>(&self,g:&mut Group<B>)->Result<Option<Delivery<Q>>,Error>{g.take(self.endpoint,&self.definition.request,false)}
 pub fn respond<B:Transport>(&self,g:&mut Group<B>,ticket:Ticket,response:&S)->Result<(),Error>{let i=g.index(self.endpoint)?;let t=g.ticket_index(i,ticket)?;if g.slots[i].history[t].state!=State::Delivered{return Err(err(9));}let body=(self.definition.response.encode)(response).map_err(codec_error)?;if body.len()>self.definition.response.maximum{return Err(err(90));}if g.slots[i].diagnostic.responses==u64::MAX{return Err(err(90));}let r=g.inspect(i)?;g.slots[i].history[t].state=State::Done;g.slots[i].diagnostic.responses+=1;let code=g.transport.respond(r.binding.source,r.binding.incarnation,g.slots[i].handles[1],ticket.value,&body);g.slots[i].diagnostic.respond=Attempt::observed(code);if code!=0{return Err(g.terminal(i,code));}Ok(())}
 /// Run one callback after a real committed request. Failure explicitly retires
 /// this endpoint and returns its primary; it is never an empty success reply.
 /// Panics/traps follow the producer's real unwind/abort contract; Host drain is
 /// still required if the Guest cannot run ordinary cleanup.
 pub fn serve_once<B:Transport,F>(&self,g:&mut Group<B>,callback:F)->Result<bool,Error>
 where F:FnOnce(Q,&ServiceInfo)->Result<S,Error>{
  match self.poll(g)?{
   None=>Ok(false),
   Some(Delivery::Control(c))=>Err(err(if c.error!=0{c.error}else{125})),
   Some(Delivery::Value{ticket,value,info})=>{
    struct Reset<'a>(&'a Cell<bool>);
    impl Drop for Reset<'_>{fn drop(&mut self){self.0.set(false);}}
    g.callback.set(true);
    let reset=Reset(&g.callback);
    let result=callback(value,&info);
    drop(reset);
    match result{
     Ok(response)=>match self.respond(g,ticket,&response){
      Ok(())=>Ok(true),
      Err(e)=>if g.slots[self.endpoint.index].diagnostic.closed{Err(e)}else{Err(g.terminal(self.endpoint.index,e.code))},
     },
     Err(e)=>{let e=codec_error(e);let i=g.index(self.endpoint)?;Err(g.terminal(i,e.code))},
    }
   },
  }
 }
 /// Release this actual endpoint even after a locally terminal control ACK.
 /// Cleanup is a separate caller operation; its results never replace control.
 pub fn close<B:Transport>(&self,g:&mut Group<B>)->Result<CloseReport,Error>{let i=g.owner_index(self.endpoint)?;Ok(g.attempt_close(i))}
}

#[cfg(test)]
mod contracts;
