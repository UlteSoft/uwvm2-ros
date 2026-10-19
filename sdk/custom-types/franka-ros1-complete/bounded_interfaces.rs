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

/// Complete canonical component bound for franka_gripper/GraspGoal.
impl Component for Ros1Franka_gripperGraspGoal {const MAX_WIRE:usize=40;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/GraspResult.
impl Component for Ros1Franka_gripperGraspResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/GraspFeedback.
impl Component for Ros1Franka_gripperGraspFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/HomingGoal.
impl Component for Ros1Franka_gripperHomingGoal {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/HomingResult.
impl Component for Ros1Franka_gripperHomingResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/HomingFeedback.
impl Component for Ros1Franka_gripperHomingFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/MoveGoal.
impl Component for Ros1Franka_gripperMoveGoal {const MAX_WIRE:usize=16;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/MoveResult.
impl Component for Ros1Franka_gripperMoveResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/MoveFeedback.
impl Component for Ros1Franka_gripperMoveFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/StopGoal.
impl Component for Ros1Franka_gripperStopGoal {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/StopResult.
impl Component for Ros1Franka_gripperStopResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_gripper/StopFeedback.
impl Component for Ros1Franka_gripperStopFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/ErrorRecoveryGoal.
impl Component for Ros1Franka_msgsErrorRecoveryGoal {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/ErrorRecoveryResult.
impl Component for Ros1Franka_msgsErrorRecoveryResult {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/ErrorRecoveryFeedback.
impl Component for Ros1Franka_msgsErrorRecoveryFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetCartesianImpedanceRequest.
impl Component for Ros1Franka_msgsSetCartesianImpedanceRequest {const MAX_WIRE:usize=48;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetCartesianImpedanceResponse.
impl Component for Ros1Franka_msgsSetCartesianImpedanceResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetEEFrameRequest.
impl Component for Ros1Franka_msgsSetEEFrameRequest {const MAX_WIRE:usize=128;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetEEFrameResponse.
impl Component for Ros1Franka_msgsSetEEFrameResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetForceTorqueCollisionBehaviorRequest.
impl Component for Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {const MAX_WIRE:usize=208;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetForceTorqueCollisionBehaviorResponse.
impl Component for Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetFullCollisionBehaviorRequest.
impl Component for Ros1Franka_msgsSetFullCollisionBehaviorRequest {const MAX_WIRE:usize=416;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetFullCollisionBehaviorResponse.
impl Component for Ros1Franka_msgsSetFullCollisionBehaviorResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetJointImpedanceRequest.
impl Component for Ros1Franka_msgsSetJointImpedanceRequest {const MAX_WIRE:usize=56;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetJointImpedanceResponse.
impl Component for Ros1Franka_msgsSetJointImpedanceResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetKFrameRequest.
impl Component for Ros1Franka_msgsSetKFrameRequest {const MAX_WIRE:usize=128;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetKFrameResponse.
impl Component for Ros1Franka_msgsSetKFrameResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetLoadRequest.
impl Component for Ros1Franka_msgsSetLoadRequest {const MAX_WIRE:usize=104;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for franka_msgs/SetLoadResponse.
impl Component for Ros1Franka_msgsSetLoadResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete custom franka_gripper/Grasp client; four roles plus optional cancel-any.
pub type Ros1Franka_gripperGraspClient=ActionClient<Ros1Franka_gripperGraspGoal,Ros1Franka_gripperGraspResult,Ros1Franka_gripperGraspFeedback>;

/// Complete custom franka_gripper/Grasp server; feedback1/terminal2..4 remain distinct.
pub type Ros1Franka_gripperGraspServer=ActionServer<Ros1Franka_gripperGraspGoal,Ros1Franka_gripperGraspResult,Ros1Franka_gripperGraspFeedback>;

/// Complete custom franka_gripper/Homing client; four roles plus optional cancel-any.
pub type Ros1Franka_gripperHomingClient=ActionClient<Ros1Franka_gripperHomingGoal,Ros1Franka_gripperHomingResult,Ros1Franka_gripperHomingFeedback>;

/// Complete custom franka_gripper/Homing server; feedback1/terminal2..4 remain distinct.
pub type Ros1Franka_gripperHomingServer=ActionServer<Ros1Franka_gripperHomingGoal,Ros1Franka_gripperHomingResult,Ros1Franka_gripperHomingFeedback>;

/// Complete custom franka_gripper/Move client; four roles plus optional cancel-any.
pub type Ros1Franka_gripperMoveClient=ActionClient<Ros1Franka_gripperMoveGoal,Ros1Franka_gripperMoveResult,Ros1Franka_gripperMoveFeedback>;

/// Complete custom franka_gripper/Move server; feedback1/terminal2..4 remain distinct.
pub type Ros1Franka_gripperMoveServer=ActionServer<Ros1Franka_gripperMoveGoal,Ros1Franka_gripperMoveResult,Ros1Franka_gripperMoveFeedback>;

/// Complete custom franka_gripper/Stop client; four roles plus optional cancel-any.
pub type Ros1Franka_gripperStopClient=ActionClient<Ros1Franka_gripperStopGoal,Ros1Franka_gripperStopResult,Ros1Franka_gripperStopFeedback>;

/// Complete custom franka_gripper/Stop server; feedback1/terminal2..4 remain distinct.
pub type Ros1Franka_gripperStopServer=ActionServer<Ros1Franka_gripperStopGoal,Ros1Franka_gripperStopResult,Ros1Franka_gripperStopFeedback>;

/// Complete custom franka_msgs/ErrorRecovery client; four roles plus optional cancel-any.
pub type Ros1Franka_msgsErrorRecoveryClient=ActionClient<Ros1Franka_msgsErrorRecoveryGoal,Ros1Franka_msgsErrorRecoveryResult,Ros1Franka_msgsErrorRecoveryFeedback>;

/// Complete custom franka_msgs/ErrorRecovery server; feedback1/terminal2..4 remain distinct.
pub type Ros1Franka_msgsErrorRecoveryServer=ActionServer<Ros1Franka_msgsErrorRecoveryGoal,Ros1Franka_msgsErrorRecoveryResult,Ros1Franka_msgsErrorRecoveryFeedback>;

/// Typed franka_msgs/SetCartesianImpedance call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetCartesianImpedanceClient=ServiceClient<Ros1Franka_msgsSetCartesianImpedanceRequest,Ros1Franka_msgsSetCartesianImpedanceResponse>;

/// Typed franka_msgs/SetCartesianImpedance advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetCartesianImpedanceServer=ServiceServer<Ros1Franka_msgsSetCartesianImpedanceRequest,Ros1Franka_msgsSetCartesianImpedanceResponse>;

/// Typed franka_msgs/SetEEFrame call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetEEFrameClient=ServiceClient<Ros1Franka_msgsSetEEFrameRequest,Ros1Franka_msgsSetEEFrameResponse>;

/// Typed franka_msgs/SetEEFrame advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetEEFrameServer=ServiceServer<Ros1Franka_msgsSetEEFrameRequest,Ros1Franka_msgsSetEEFrameResponse>;

/// Typed franka_msgs/SetForceTorqueCollisionBehavior call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient=ServiceClient<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>;

/// Typed franka_msgs/SetForceTorqueCollisionBehavior advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetForceTorqueCollisionBehaviorServer=ServiceServer<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>;

/// Typed franka_msgs/SetFullCollisionBehavior call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetFullCollisionBehaviorClient=ServiceClient<Ros1Franka_msgsSetFullCollisionBehaviorRequest,Ros1Franka_msgsSetFullCollisionBehaviorResponse>;

/// Typed franka_msgs/SetFullCollisionBehavior advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetFullCollisionBehaviorServer=ServiceServer<Ros1Franka_msgsSetFullCollisionBehaviorRequest,Ros1Franka_msgsSetFullCollisionBehaviorResponse>;

/// Typed franka_msgs/SetJointImpedance call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetJointImpedanceClient=ServiceClient<Ros1Franka_msgsSetJointImpedanceRequest,Ros1Franka_msgsSetJointImpedanceResponse>;

/// Typed franka_msgs/SetJointImpedance advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetJointImpedanceServer=ServiceServer<Ros1Franka_msgsSetJointImpedanceRequest,Ros1Franka_msgsSetJointImpedanceResponse>;

/// Typed franka_msgs/SetKFrame call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetKFrameClient=ServiceClient<Ros1Franka_msgsSetKFrameRequest,Ros1Franka_msgsSetKFrameResponse>;

/// Typed franka_msgs/SetKFrame advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetKFrameServer=ServiceServer<Ros1Franka_msgsSetKFrameRequest,Ros1Franka_msgsSetKFrameResponse>;

/// Typed franka_msgs/SetLoad call/real-submit owner; every alias is explicit.
pub type Ros1Franka_msgsSetLoadClient=ServiceClient<Ros1Franka_msgsSetLoadRequest,Ros1Franka_msgsSetLoadResponse>;

/// Typed franka_msgs/SetLoad advertiser/responder with independent grants.
pub type Ros1Franka_msgsSetLoadServer=ServiceServer<Ros1Franka_msgsSetLoadRequest,Ros1Franka_msgsSetLoadResponse>;
