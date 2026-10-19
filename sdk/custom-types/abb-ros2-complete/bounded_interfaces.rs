//! Generated typed custom RPC owners. Enable roswasm's alloc and selected ROS feature.
//! Declare this module beside bounded_records; no builtin factory is reinterpreted.
//! All aliases are explicit. Operations are caller-driven and never retried.
extern crate alloc;
use alloc::{vec::Vec,rc::Rc};
use core::{cell::Cell,marker::PhantomData};
use roswasm::{Backend,Capability,Error,Operation,codes};
use roswasm::typed::Canonical;
use roswasm::events::{self,Kind};
use super::bounded_records::*;

/// A generated component's finite bound and actual ROS version, not a grant.
pub trait Component:Canonical { const MAX_WIRE:usize; const ROS_VERSION:u8; }
fn invalid()->Error{Error::new(codes::INVALID)}
fn id(bytes:&[u8])->Result<u64,Error>{let value=u64::from_le_bytes(bytes.get(..8).ok_or(invalid())?.try_into().map_err(|_|invalid())?);if value==0{Err(invalid())}else{Ok(value)}}
fn frame(ticket:u64,kind:Option<u8>,body:&[u8])->Result<Vec<u8>,Error>{if ticket==0{return Err(invalid());}let mut wire=Vec::with_capacity(9+body.len());wire.extend_from_slice(&ticket.to_le_bytes());if let Some(value)=kind{wire.push(value);}wire.extend_from_slice(body);Ok(wire)}
fn remove(values:&mut Vec<u64>,ticket:u64)->bool{if let Some(i)=values.iter().position(|v|*v==ticket){values.swap_remove(i);true}else{false}}
fn owned(values:&[u64],ticket:u64)->Result<(),Error>{if ticket==0||!values.contains(&ticket){Err(Error::new(codes::BAD_HANDLE))}else{Ok(())}}

/// Every owned cleanup result, preserving a separate primary operation failure.
#[derive(Debug)]
pub struct Cleanup {pub primary:u32,pub statuses:Vec<Result<(),Error>>}
/// Construction refusal plus every attempted rollback, with no automatic reopen.
#[derive(Debug)]
pub struct OpenError {pub primary:Error,pub rollback:Cleanup}
struct Owners {caps:Vec<Capability>,busy:Rc<Cell<bool>>,capacity:usize}
struct Pass(Rc<Cell<bool>>);
impl Drop for Pass{fn drop(&mut self){self.0.set(false);}}
impl Owners {
 fn open(backend:Backend,version:u8,aliases:&[&str],capacity:usize)->Result<Self,OpenError>{
  let mut value=Self{caps:Vec::new(),busy:Rc::new(Cell::new(false)),capacity};
  let result=(||{if !(1..=1024).contains(&capacity)||!(1..=2).contains(&version)||aliases.is_empty()||aliases.iter().enumerate().any(|(i,a)|a.is_empty()||a.len()>255||a.as_bytes().contains(&0)||aliases[..i].contains(a)){return Err(invalid());}
   if backend.abi_version()!=1{return Err(Error::new(codes::UNSUPPORTED));}for alias in aliases{let cap=Capability::open(backend,alias)?;if value.caps.iter().any(|old|old.raw_handle().ok()==cap.raw_handle().ok()){return Err(Error::new(codes::CONFLICT));}value.caps.push(cap);}Ok(())})();
  match result{Ok(())=>Ok(value),Err(primary)=>{let rollback=value.cleanup(primary.code,false);Err(OpenError{primary,rollback})}}
 }
 fn cap(&self,index:usize)->Result<&Capability,Error>{let cap=self.caps.get(index).ok_or(Error::new(codes::BAD_HANDLE))?;cap.raw_handle()?;Ok(cap)}
 fn idle(&self)->Result<(),Error>{if self.busy.get(){return Err(Error::new(codes::CONFLICT));}self.cap(0)?;Ok(())}
 fn enter(&self)->Result<Pass,Error>{self.idle()?;self.busy.set(true);Ok(Pass(Rc::clone(&self.busy)))}
 fn cleanup(&mut self,primary:u32,revoke:bool)->Cleanup{let mut statuses=Vec::new();if self.busy.get(){statuses.push(Err(Error::new(codes::CONFLICT)));return Cleanup{primary,statuses};}for cap in self.caps.iter_mut().rev(){if cap.is_open(){statuses.push(if revoke{cap.revoke()}else{cap.close()});}}Cleanup{primary,statuses}}
 fn request(&self,index:usize,op:Operation,input:&[u8],maximum:usize)->Result<Vec<u8>,Error>{let mut output=alloc::vec![0u8;maximum];let used=self.cap(index)?.request(op,input,&mut output)?;output.truncate(used);Ok(output)}
 fn zero(&self,index:usize,op:Operation,input:&[u8])->Result<(),Error>{if self.request(index,op,input,0)?.is_empty(){Ok(())}else{Err(invalid())}}
 fn poll(&self,maximum:usize)->Result<Vec<u8>,Error>{let mut bytes=alloc::vec![0u8;maximum];let used=self.cap(0)?.poll(&mut bytes)?;bytes.truncate(used);Ok(bytes)}
}

/// A successfully decoded asynchronous completion; admission itself is not completion.
pub enum Completion<T>{Response{request_id:u64,value:T},Failure{request_id:u64,error:u32}}
/// Custom service owner. Optional observer_alias opens a distinct response/error
/// observer. That observer is exclusive to this facade and starts unconsumed.
pub struct ServiceClient<Q:Component,S:Component>{owners:Owners,pending:Vec<u64>,binding:Option<(u64,u64,u64)>,marker:PhantomData<(Q,S)>}
impl<Q:Component,S:Component> ServiceClient<Q,S>{
 /// Open explicitly granted call and optional observer roles exactly once.
 pub fn open(backend:Backend,alias:&str,observer_alias:Option<&str>,capacity:usize)->Result<Self,OpenError>{let aliases=if let Some(o)=observer_alias{alloc::vec![alias,o]}else{alloc::vec![alias]};let owners=Owners::open(backend,Q::ROS_VERSION,&aliases,capacity)?;Ok(Self{owners,pending:Vec::new(),binding:None,marker:PhantomData})}
 /// Perform one synchronous call and decode only a complete generated response.
 pub fn call(&self,value:&Q)->Result<S,Error>{self.owners.idle()?;S::decode(&self.owners.request(0,Operation::ServiceCall,&value.encode()?,S::MAX_WIRE)?)}
 /// Admit a copied request using the real service_submit import, never deferred call().
 pub fn submit(&mut self,value:&Q)->Result<u64,Error>{self.owners.idle()?;if self.owners.caps.len()!=2{return Err(Error::new(codes::DENIED));}if self.pending.len()==self.owners.capacity{return Err(Error::new(codes::TOO_LARGE));}let observer=self.owners.cap(1)?;let ticket=self.owners.cap(0)?.submit_service(observer,&value.encode()?)?;if self.pending.contains(&ticket){return Err(invalid());}self.pending.push(ticket);Ok(ticket)}
 /// Poll the existing SDK unified decoder; enforce source/backend/stream and
 /// owned correlation before committing a typed response or numeric failure.
 pub fn poll_completion(&mut self)->Result<Completion<S>,Error>{self.owners.idle()?;if self.owners.caps.len()!=2{return Err(Error::new(codes::DENIED));}let source=self.owners.cap(0)?.raw_handle()?;let observer=self.owners.cap(1)?;let mut bytes=alloc::vec![0u8;64+S::MAX_WIRE];let n=observer.poll_event_v1(&mut bytes)?;let event=events::decode(&bytes[..n])?;
  let (last,instance,session)=self.binding.unwrap_or((0,event.instance,event.session));if event.backend!=Q::ROS_VERSION||event.source!=source||event.sequence!=last.checked_add(1).ok_or(invalid())?||event.instance!=instance||event.session!=session{return Err(invalid());}self.binding=Some((event.sequence,instance,session));
  if event.kind==Kind::ResourceClosed{return Err(Error::new(codes::BAD_HANDLE));}owned(&self.pending,event.correlation)?;
  let result=match event.kind{Kind::ServiceResponse=>Completion::Response{request_id:event.correlation,value:S::decode(event.body)?},Kind::ResourceError=>Completion::Failure{request_id:event.correlation,error:event.error},_=>return Err(invalid())};remove(&mut self.pending,event.correlation);Ok(result)}
 /// Attempt every close once; no retry after a host retirement failure.
 pub fn close_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,false);if !self.owners.busy.get(){self.pending.clear();}report}
 /// Explicitly revoke existing roles; this creates no stronger permission.
 pub fn revoke_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,true);if !self.owners.busy.get(){self.pending.clear();}report}
}

/// Typed incoming service, with separate advertise/respond ownership.
pub struct ServiceServer<Q:Component,S:Component>{owners:Owners,live:Vec<u64>,marker:PhantomData<(Q,S)>}
impl<Q:Component,S:Component> ServiceServer<Q,S>{
 /// Open both roles before activating advertisement; rollback remains inspectable.
 pub fn open(backend:Backend,advertise:&str,respond:&str,capacity:usize)->Result<Self,OpenError>{let mut owners=Owners::open(backend,Q::ROS_VERSION,&[advertise,respond],capacity)?;if let Err(primary)=owners.cap(0).and_then(Capability::advertise_service){let rollback=owners.cleanup(primary.code,false);return Err(OpenError{primary,rollback});}Ok(Self{owners,live:Vec::new(),marker:PhantomData})}
 fn poll_inner(&mut self)->Result<(u64,Q),Error>{if self.live.len()==self.owners.capacity{return Err(Error::new(codes::TOO_LARGE));}let bytes=self.owners.poll(Q::MAX_WIRE+8)?;let ticket=id(&bytes)?;if self.live.contains(&ticket){return Err(invalid());}let value=Q::decode(&bytes[8..])?;self.live.push(ticket);Ok((ticket,value))}
 /// Poll at most one complete request. Again11 leaves all owner tables unchanged.
 pub fn poll_once(&mut self)->Result<(u64,Q),Error>{self.owners.idle()?;self.poll_inner()}
 /// Respond only to a ticket delivered here; host still verifies principal/deadline.
 pub fn respond(&mut self,ticket:u64,value:&S)->Result<(),Error>{owned(&self.live,ticket)?;self.owners.zero(1,Operation::ServiceRespond,&frame(ticket,None,&value.encode()?)?)?;remove(&mut self.live,ticket);Ok(())}
 /// Run one local handler. Error/unwind releases the lease; no response is replayed.
 pub fn spin_once<F:FnOnce(u64,Q)->Result<S,Error>>(&mut self,handler:F)->Result<(),Error>{let _pass=self.owners.enter()?;let(ticket,value)=self.poll_inner()?;let response=handler(ticket,value)?;self.respond(ticket,&response)}
 /// Observe every role close, retaining the primary error separately.
 pub fn close_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,false);if !self.owners.busy.get(){self.live.clear();}report}
 /// Deliberately revoke the two granted aliases.
 pub fn revoke_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,true);if !self.owners.busy.get(){self.live.clear();}report}
}

/// Four mandatory action roles and an optional independently granted cancel-any role.
pub struct ActionClient<G:Component,R:Component,F:Component>{owners:Owners,goals:Vec<u64>,marker:PhantomData<(G,R,F)>}
impl<G:Component,R:Component,F:Component> ActionClient<G,R,F>{
 /// Role order is send, cancel-own, feedback, result; cancel-any is separate.
 pub fn open(backend:Backend,aliases:[&str;4],cancel_any:Option<&str>,capacity:usize)->Result<Self,OpenError>{let mut selected=aliases.to_vec();if let Some(alias)=cancel_any{selected.push(alias);}Ok(Self{owners:Owners::open(backend,G::ROS_VERSION,&selected,capacity)?,goals:Vec::new(),marker:PhantomData})}
 /// Send once, admitting a unique nonzero locally owned goal.
 pub fn send_goal(&mut self,value:&G)->Result<u64,Error>{self.owners.idle()?;if self.goals.len()==self.owners.capacity{return Err(Error::new(codes::TOO_LARGE));}let bytes=self.owners.request(0,Operation::ActionSendGoal,&value.encode()?,8)?;if bytes.len()!=8{return Err(invalid());}let ticket=id(&bytes)?;if self.goals.contains(&ticket){return Err(invalid());}self.goals.push(ticket);Ok(ticket)}
 fn cancel(&self,index:usize,op:Operation,ticket:u64)->Result<bool,Error>{let bytes=self.owners.request(index,op,&ticket.to_le_bytes(),1)?;if bytes.len()!=1||bytes[0]>1{return Err(invalid());}Ok(bytes[0]!=0)}
 /// Cancellation acknowledgement retains the goal until its terminal result.
 pub fn cancel_own(&self,ticket:u64)->Result<bool,Error>{self.owners.idle()?;owned(&self.goals,ticket)?;self.cancel(1,Operation::ActionCancelOwnGoal,ticket)}
 /// Requires the fifth role. Zero explicitly means cancel-all, never cancel-own.
 pub fn cancel_any(&self,ticket:u64)->Result<bool,Error>{self.owners.idle()?;if self.owners.caps.len()!=5{return Err(Error::new(codes::DENIED));}self.cancel(4,Operation::ActionCancelAnyGoal,ticket)}
 /// Explicit cancel-all still requires the fifth independently granted role.
 pub fn cancel_all(&self)->Result<bool,Error>{self.cancel_any(0)}
 /// Receive feedback only for an owned ID and matching complete envelope.
 pub fn feedback(&self,ticket:u64)->Result<F,Error>{self.owners.idle()?;owned(&self.goals,ticket)?;let bytes=self.owners.request(2,Operation::ActionReceiveFeedback,&ticket.to_le_bytes(),F::MAX_WIRE+8)?;if id(&bytes)?!=ticket{return Err(invalid());}F::decode(&bytes[8..])}
 /// A fully decoded terminal kind2/3/4 removes exactly one local goal.
 pub fn result(&mut self,ticket:u64)->Result<(u8,R),Error>{self.owners.idle()?;owned(&self.goals,ticket)?;let bytes=self.owners.request(3,Operation::ActionReceiveResult,&ticket.to_le_bytes(),R::MAX_WIRE+9)?;if bytes.len()<9||id(&bytes)?!=ticket||!(2..=4).contains(&bytes[8]){return Err(invalid());}let value=R::decode(&bytes[9..])?;remove(&mut self.goals,ticket);Ok((bytes[8],value))}
 /// Checked close attempts every acquired role exactly once.
 pub fn close_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,false);self.goals.clear();report}
 /// Checked revoke never silently promotes cancel-own to cancel-any.
 pub fn revoke_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,true);self.goals.clear();report}
}

/// Custom incoming action owner; feedback retains, successful terminal response retires.
pub struct ActionServer<G:Component,R:Component,F:Component>{owners:Owners,live:Vec<u64>,marker:PhantomData<(G,R,F)>}
impl<G:Component,R:Component,F:Component> ActionServer<G,R,F>{
 /// Activate the action only after both distinct aliases are open.
 pub fn open(backend:Backend,advertise:&str,respond:&str,capacity:usize)->Result<Self,OpenError>{let mut owners=Owners::open(backend,G::ROS_VERSION,&[advertise,respond],capacity)?;if let Err(primary)=owners.zero(0,Operation::ActionAdvertise,&[]){let rollback=owners.cleanup(primary.code,false);return Err(OpenError{primary,rollback});}Ok(Self{owners,live:Vec::new(),marker:PhantomData})}
 fn poll_inner(&mut self)->Result<(u64,G),Error>{if self.live.len()==self.owners.capacity{return Err(Error::new(codes::TOO_LARGE));}let bytes=self.owners.poll(G::MAX_WIRE+8)?;let ticket=id(&bytes)?;if self.live.contains(&ticket){return Err(invalid());}let value=G::decode(&bytes[8..])?;self.live.push(ticket);Ok((ticket,value))}
 /// Poll one complete goal; no native callback enters Rust.
 pub fn poll_once(&mut self)->Result<(u64,G),Error>{self.owners.idle()?;self.poll_inner()}
 /// Send feedback for one locally received live goal.
 pub fn respond_feedback(&self,ticket:u64,value:&F)->Result<(),Error>{owned(&self.live,ticket)?;self.owners.zero(1,Operation::ActionRespond,&frame(ticket,Some(1),&value.encode()?)?)}
 /// Send terminal success2/abort3/cancel4; remove only after actual host success.
 pub fn respond_result(&mut self,ticket:u64,kind:u8,value:&R)->Result<(),Error>{owned(&self.live,ticket)?;if !(2..=4).contains(&kind){return Err(invalid());}self.owners.zero(1,Operation::ActionRespond,&frame(ticket,Some(kind),&value.encode()?)?)?;remove(&mut self.live,ticket);Ok(())}
 /// Dispatch once under a local lease; the handler owns no capability and may
 /// return a decision for a later explicit response, with no implicit terminal reply.
 pub fn spin_once<T,H:FnOnce(u64,G)->Result<T,Error>>(&mut self,handler:H)->Result<T,Error>{let _pass=self.owners.enter()?;let(ticket,value)=self.poll_inner()?;handler(ticket,value)}
 /// Inspect every close; dropping this non-Clone owner is only best-effort cleanup.
 pub fn close_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,false);if !self.owners.busy.get(){self.live.clear();}report}
 /// Revoke existing aliases with separately observable cleanup errors.
 pub fn revoke_checked(&mut self,primary:u32)->Cleanup{let report=self.owners.cleanup(primary,true);if !self.owners.busy.get(){self.live.clear();}report}
}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {const MAX_WIRE:usize=68;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {const MAX_WIRE:usize=1285;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {const MAX_WIRE:usize=835;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {const MAX_WIRE:usize=584;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {const MAX_WIRE:usize=73;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response.
impl Component for Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetFileContents_Request.
impl Component for Ros2Abb_robot_msgsSrvGetFileContents_Request {const MAX_WIRE:usize=516;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetFileContents_Response.
impl Component for Ros2Abb_robot_msgsSrvGetFileContents_Response {const MAX_WIRE:usize=4618;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetIOSignal_Request.
impl Component for Ros2Abb_robot_msgsSrvGetIOSignal_Request {const MAX_WIRE:usize=516;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetIOSignal_Response.
impl Component for Ros2Abb_robot_msgsSrvGetIOSignal_Response {const MAX_WIRE:usize=1034;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDBool_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {const MAX_WIRE:usize=204;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDBool_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {const MAX_WIRE:usize=519;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDDnum_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {const MAX_WIRE:usize=204;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDDnum_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {const MAX_WIRE:usize=526;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDNum_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {const MAX_WIRE:usize=204;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDNum_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {const MAX_WIRE:usize=522;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDString_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDString_Request {const MAX_WIRE:usize=204;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDString_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDString_Response {const MAX_WIRE:usize=1034;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDSymbol_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {const MAX_WIRE:usize=204;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRAPIDSymbol_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {const MAX_WIRE:usize=1034;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRobotControllerDescription_Request.
impl Component for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {const MAX_WIRE:usize=0;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetRobotControllerDescription_Response.
impl Component for Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {const MAX_WIRE:usize=4618;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetSpeedRatio_Request.
impl Component for Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {const MAX_WIRE:usize=0;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/GetSpeedRatio_Response.
impl Component for Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {const MAX_WIRE:usize=519;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetFileContents_Request.
impl Component for Ros2Abb_robot_msgsSrvSetFileContents_Request {const MAX_WIRE:usize=4616;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetFileContents_Response.
impl Component for Ros2Abb_robot_msgsSrvSetFileContents_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetIOSignal_Request.
impl Component for Ros2Abb_robot_msgsSrvSetIOSignal_Request {const MAX_WIRE:usize=1032;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetIOSignal_Response.
impl Component for Ros2Abb_robot_msgsSrvSetIOSignal_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDBool_Request.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {const MAX_WIRE:usize=205;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDBool_Response.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDDnum_Request.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {const MAX_WIRE:usize=212;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDDnum_Response.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDNum_Request.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {const MAX_WIRE:usize=208;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDNum_Response.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDString_Request.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDString_Request {const MAX_WIRE:usize=720;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDString_Response.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDString_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDSymbol_Request.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {const MAX_WIRE:usize=720;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetRAPIDSymbol_Response.
impl Component for Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetSpeedRatio_Request.
impl Component for Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {const MAX_WIRE:usize=1;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/SetSpeedRatio_Response.
impl Component for Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/TriggerWithResultCode_Request.
impl Component for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {const MAX_WIRE:usize=0;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for abb_robot_msgs/srv/TriggerWithResultCode_Response.
impl Component for Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {const MAX_WIRE:usize=518;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for control_msgs/action/FollowJointTrajectory_Goal.
impl Component for Ros2Control_msgsActionFollowJointTrajectory_Goal {const MAX_WIRE:usize=20920;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for control_msgs/action/FollowJointTrajectory_Result.
impl Component for Ros2Control_msgsActionFollowJointTrajectory_Result {const MAX_WIRE:usize=4104;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for control_msgs/action/FollowJointTrajectory_Feedback.
impl Component for Ros2Control_msgsActionFollowJointTrajectory_Feedback {const MAX_WIRE:usize=4128;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for std_srvs/srv/Trigger_Request.
impl Component for Ros2Std_srvsSrvTrigger_Request {const MAX_WIRE:usize=0;const ROS_VERSION:u8=2;}

/// Complete canonical component bound for std_srvs/srv/Trigger_Response.
impl Component for Ros2Std_srvsSrvTrigger_Response {const MAX_WIRE:usize=517;const ROS_VERSION:u8=2;}

/// Typed abb_rapid_sm_addin_msgs/srv/GetEGMSettings call/real-submit owner; every alias is explicit.
pub type Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient=ServiceClient<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request,Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/GetEGMSettings advertiser/responder with independent grants.
pub type Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsServer=ServiceServer<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request,Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetEGMSettings call/real-submit owner; every alias is explicit.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient=ServiceClient<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetEGMSettings advertiser/responder with independent grants.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsServer=ServiceServer<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine call/real-submit owner; every alias is explicit.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient=ServiceClient<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine advertiser/responder with independent grants.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineServer=ServiceServer<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetSGCommand call/real-submit owner; every alias is explicit.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient=ServiceClient<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>;

/// Typed abb_rapid_sm_addin_msgs/srv/SetSGCommand advertiser/responder with independent grants.
pub type Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandServer=ServiceServer<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request,Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>;

/// Typed abb_robot_msgs/srv/GetFileContents call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetFileContentsClient=ServiceClient<Ros2Abb_robot_msgsSrvGetFileContents_Request,Ros2Abb_robot_msgsSrvGetFileContents_Response>;

/// Typed abb_robot_msgs/srv/GetFileContents advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetFileContentsServer=ServiceServer<Ros2Abb_robot_msgsSrvGetFileContents_Request,Ros2Abb_robot_msgsSrvGetFileContents_Response>;

/// Typed abb_robot_msgs/srv/GetIOSignal call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetIOSignalClient=ServiceClient<Ros2Abb_robot_msgsSrvGetIOSignal_Request,Ros2Abb_robot_msgsSrvGetIOSignal_Response>;

/// Typed abb_robot_msgs/srv/GetIOSignal advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetIOSignalServer=ServiceServer<Ros2Abb_robot_msgsSrvGetIOSignal_Request,Ros2Abb_robot_msgsSrvGetIOSignal_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDBool call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRAPIDBoolClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request,Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDBool advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRAPIDBoolServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request,Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDDnum call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRAPIDDnumClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request,Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDDnum advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRAPIDDnumServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request,Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDNum call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRAPIDNumClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request,Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDNum advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRAPIDNumServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request,Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDString call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRAPIDStringClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRAPIDString_Request,Ros2Abb_robot_msgsSrvGetRAPIDString_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDString advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRAPIDStringServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRAPIDString_Request,Ros2Abb_robot_msgsSrvGetRAPIDString_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDSymbol call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request,Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>;

/// Typed abb_robot_msgs/srv/GetRAPIDSymbol advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRAPIDSymbolServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request,Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>;

/// Typed abb_robot_msgs/srv/GetRobotControllerDescription call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient=ServiceClient<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request,Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>;

/// Typed abb_robot_msgs/srv/GetRobotControllerDescription advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionServer=ServiceServer<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request,Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>;

/// Typed abb_robot_msgs/srv/GetSpeedRatio call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvGetSpeedRatioClient=ServiceClient<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request,Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>;

/// Typed abb_robot_msgs/srv/GetSpeedRatio advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvGetSpeedRatioServer=ServiceServer<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request,Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>;

/// Typed abb_robot_msgs/srv/SetFileContents call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetFileContentsClient=ServiceClient<Ros2Abb_robot_msgsSrvSetFileContents_Request,Ros2Abb_robot_msgsSrvSetFileContents_Response>;

/// Typed abb_robot_msgs/srv/SetFileContents advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetFileContentsServer=ServiceServer<Ros2Abb_robot_msgsSrvSetFileContents_Request,Ros2Abb_robot_msgsSrvSetFileContents_Response>;

/// Typed abb_robot_msgs/srv/SetIOSignal call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetIOSignalClient=ServiceClient<Ros2Abb_robot_msgsSrvSetIOSignal_Request,Ros2Abb_robot_msgsSrvSetIOSignal_Response>;

/// Typed abb_robot_msgs/srv/SetIOSignal advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetIOSignalServer=ServiceServer<Ros2Abb_robot_msgsSrvSetIOSignal_Request,Ros2Abb_robot_msgsSrvSetIOSignal_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDBool call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetRAPIDBoolClient=ServiceClient<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request,Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDBool advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetRAPIDBoolServer=ServiceServer<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request,Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDDnum call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetRAPIDDnumClient=ServiceClient<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request,Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDDnum advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetRAPIDDnumServer=ServiceServer<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request,Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDNum call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetRAPIDNumClient=ServiceClient<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request,Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDNum advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetRAPIDNumServer=ServiceServer<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request,Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDString call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetRAPIDStringClient=ServiceClient<Ros2Abb_robot_msgsSrvSetRAPIDString_Request,Ros2Abb_robot_msgsSrvSetRAPIDString_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDString advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetRAPIDStringServer=ServiceServer<Ros2Abb_robot_msgsSrvSetRAPIDString_Request,Ros2Abb_robot_msgsSrvSetRAPIDString_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDSymbol call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient=ServiceClient<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request,Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>;

/// Typed abb_robot_msgs/srv/SetRAPIDSymbol advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetRAPIDSymbolServer=ServiceServer<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request,Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>;

/// Typed abb_robot_msgs/srv/SetSpeedRatio call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvSetSpeedRatioClient=ServiceClient<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request,Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>;

/// Typed abb_robot_msgs/srv/SetSpeedRatio advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvSetSpeedRatioServer=ServiceServer<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request,Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>;

/// Typed abb_robot_msgs/srv/TriggerWithResultCode call/real-submit owner; every alias is explicit.
pub type Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient=ServiceClient<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request,Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>;

/// Typed abb_robot_msgs/srv/TriggerWithResultCode advertiser/responder with independent grants.
pub type Ros2Abb_robot_msgsSrvTriggerWithResultCodeServer=ServiceServer<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request,Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>;

/// Complete custom control_msgs/action/FollowJointTrajectory client; four roles plus optional cancel-any.
pub type Ros2Control_msgsActionFollowJointTrajectoryClient=ActionClient<Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_Feedback>;

/// Complete custom control_msgs/action/FollowJointTrajectory server; feedback1/terminal2..4 remain distinct.
pub type Ros2Control_msgsActionFollowJointTrajectoryServer=ActionServer<Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_Feedback>;

/// Typed std_srvs/srv/Trigger call/real-submit owner; every alias is explicit.
pub type Ros2Std_srvsSrvTriggerClient=ServiceClient<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response>;

/// Typed std_srvs/srv/Trigger advertiser/responder with independent grants.
pub type Ros2Std_srvsSrvTriggerServer=ServiceServer<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response>;
