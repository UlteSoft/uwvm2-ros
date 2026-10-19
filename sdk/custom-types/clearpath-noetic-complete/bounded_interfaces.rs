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

/// Complete canonical component bound for clearpath_camera_msgs/InspectPOIGoal.
impl Component for Ros1Clearpath_camera_msgsInspectPOIGoal {const MAX_WIRE:usize=264;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_camera_msgs/InspectPOIResult.
impl Component for Ros1Clearpath_camera_msgsInspectPOIResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_camera_msgs/InspectPOIFeedback.
impl Component for Ros1Clearpath_camera_msgsInspectPOIFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalDockGoal.
impl Component for Ros1Clearpath_dock_msgsLocalDockGoal {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalDockResult.
impl Component for Ros1Clearpath_dock_msgsLocalDockResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalDockFeedback.
impl Component for Ros1Clearpath_dock_msgsLocalDockFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalUndockGoal.
impl Component for Ros1Clearpath_dock_msgsLocalUndockGoal {const MAX_WIRE:usize=4;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalUndockResult.
impl Component for Ros1Clearpath_dock_msgsLocalUndockResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/LocalUndockFeedback.
impl Component for Ros1Clearpath_dock_msgsLocalUndockFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/NetworkDockGoal.
impl Component for Ros1Clearpath_dock_msgsNetworkDockGoal {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/NetworkDockResult.
impl Component for Ros1Clearpath_dock_msgsNetworkDockResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/NetworkDockFeedback.
impl Component for Ros1Clearpath_dock_msgsNetworkDockFeedback {const MAX_WIRE:usize=3806;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/RadiusDockGoal.
impl Component for Ros1Clearpath_dock_msgsRadiusDockGoal {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/RadiusDockResult.
impl Component for Ros1Clearpath_dock_msgsRadiusDockResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/RadiusDockFeedback.
impl Component for Ros1Clearpath_dock_msgsRadiusDockFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/SurveyBaseStationGoal.
impl Component for Ros1Clearpath_localization_msgsSurveyBaseStationGoal {const MAX_WIRE:usize=4;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/SurveyBaseStationResult.
impl Component for Ros1Clearpath_localization_msgsSurveyBaseStationResult {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/SurveyBaseStationFeedback.
impl Component for Ros1Clearpath_localization_msgsSurveyBaseStationFeedback {const MAX_WIRE:usize=4;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/RunScheduleByUuidGoal.
impl Component for Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoal {const MAX_WIRE:usize=268;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/RunScheduleByUuidResult.
impl Component for Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResult {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/RunScheduleByUuidFeedback.
impl Component for Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedback {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteMissionByUuidGoal.
impl Component for Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoal {const MAX_WIRE:usize=521;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteMissionByUuidResult.
impl Component for Ros1Clearpath_navigation_msgsExecuteMissionByUuidResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteMissionByUuidFeedback.
impl Component for Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkGoToGoal.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoal {const MAX_WIRE:usize=3610;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkGoToResult.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkGoToResult {const MAX_WIRE:usize=262;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkGoToFeedback.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedback {const MAX_WIRE:usize=3806;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkMissionByUuidGoal.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoal {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkMissionByUuidResult.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResult {const MAX_WIRE:usize=6968;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/ExecuteNetworkMissionByUuidFeedback.
impl Component for Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedback {const MAX_WIRE:usize=10512;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/GetWaypointPathGoal.
impl Component for Ros1Clearpath_navigation_msgsGetWaypointPathGoal {const MAX_WIRE:usize=874;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/GetWaypointPathResult.
impl Component for Ros1Clearpath_navigation_msgsGetWaypointPathResult {const MAX_WIRE:usize=828;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/GetWaypointPathFeedback.
impl Component for Ros1Clearpath_navigation_msgsGetWaypointPathFeedback {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/MissionGoal.
impl Component for Ros1Clearpath_navigation_msgsMissionGoal {const MAX_WIRE:usize=16687;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/MissionResult.
impl Component for Ros1Clearpath_navigation_msgsMissionResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/MissionFeedback.
impl Component for Ros1Clearpath_navigation_msgsMissionFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/NetworkGoToPOIGoal.
impl Component for Ros1Clearpath_navigation_msgsNetworkGoToPOIGoal {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/NetworkGoToPOIResult.
impl Component for Ros1Clearpath_navigation_msgsNetworkGoToPOIResult {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/NetworkGoToPOIFeedback.
impl Component for Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedback {const MAX_WIRE:usize=3806;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RecordPathGoal.
impl Component for Ros1Clearpath_navigation_msgsRecordPathGoal {const MAX_WIRE:usize=16;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RecordPathResult.
impl Component for Ros1Clearpath_navigation_msgsRecordPathResult {const MAX_WIRE:usize=36;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RecordPathFeedback.
impl Component for Ros1Clearpath_navigation_msgsRecordPathFeedback {const MAX_WIRE:usize=36;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkGoToPlannerGoal.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoal {const MAX_WIRE:usize=3610;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkGoToPlannerResult.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResult {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkGoToPlannerFeedback.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedback {const MAX_WIRE:usize=196;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkMissionPreviewerGoal.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoal {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkMissionPreviewerResult.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResult {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/RunNetworkMissionPreviewerFeedback.
impl Component for Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedback {const MAX_WIRE:usize=7096;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SmoothPathGoal.
impl Component for Ros1Clearpath_navigation_msgsSmoothPathGoal {const MAX_WIRE:usize=580;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SmoothPathResult.
impl Component for Ros1Clearpath_navigation_msgsSmoothPathResult {const MAX_WIRE:usize=556;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SmoothPathFeedback.
impl Component for Ros1Clearpath_navigation_msgsSmoothPathFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/UITaskGoal.
impl Component for Ros1Clearpath_navigation_msgsUITaskGoal {const MAX_WIRE:usize=804;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/UITaskResult.
impl Component for Ros1Clearpath_navigation_msgsUITaskResult {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/UITaskFeedback.
impl Component for Ros1Clearpath_navigation_msgsUITaskFeedback {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_camera_msgs/q62AreaZoomRequest.
impl Component for Ros1Clearpath_camera_msgsQ62AreaZoomRequest {const MAX_WIRE:usize=12;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_camera_msgs/q62AreaZoomResponse.
impl Component for Ros1Clearpath_camera_msgsQ62AreaZoomResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_control_msgs/SetControlModeRequest.
impl Component for Ros1Clearpath_control_msgsSetControlModeRequest {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_control_msgs/SetControlModeResponse.
impl Component for Ros1Clearpath_control_msgsSetControlModeResponse {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/AddDockRequest.
impl Component for Ros1Clearpath_dock_msgsAddDockRequest {const MAX_WIRE:usize=1204;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/AddDockResponse.
impl Component for Ros1Clearpath_dock_msgsAddDockResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/ExportDataRequest.
impl Component for Ros1Clearpath_dock_msgsExportDataRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/ExportDataResponse.
impl Component for Ros1Clearpath_dock_msgsExportDataResponse {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/GetDockByUuidRequest.
impl Component for Ros1Clearpath_dock_msgsGetDockByUuidRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/GetDockByUuidResponse.
impl Component for Ros1Clearpath_dock_msgsGetDockByUuidResponse {const MAX_WIRE:usize=1316;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/ImportDataRequest.
impl Component for Ros1Clearpath_dock_msgsImportDataRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/ImportDataResponse.
impl Component for Ros1Clearpath_dock_msgsImportDataResponse {const MAX_WIRE:usize=2412;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/RemoveDockRequest.
impl Component for Ros1Clearpath_dock_msgsRemoveDockRequest {const MAX_WIRE:usize=392;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/RemoveDockResponse.
impl Component for Ros1Clearpath_dock_msgsRemoveDockResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/SetDockLocationByIdRequest.
impl Component for Ros1Clearpath_dock_msgsSetDockLocationByIdRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/SetDockLocationByIdResponse.
impl Component for Ros1Clearpath_dock_msgsSetDockLocationByIdResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/SetDockLocationByNameRequest.
impl Component for Ros1Clearpath_dock_msgsSetDockLocationByNameRequest {const MAX_WIRE:usize=132;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_dock_msgs/SetDockLocationByNameResponse.
impl Component for Ros1Clearpath_dock_msgsSetDockLocationByNameResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertCartesianToLatLonRequest.
impl Component for Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest {const MAX_WIRE:usize=200;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertCartesianToLatLonResponse.
impl Component for Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse {const MAX_WIRE:usize=244;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertCartesianToLatLonArrayRequest.
impl Component for Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest {const MAX_WIRE:usize=404;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertCartesianToLatLonArrayResponse.
impl Component for Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse {const MAX_WIRE:usize=492;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertLatLonToCartesianRequest.
impl Component for Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest {const MAX_WIRE:usize=244;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertLatLonToCartesianResponse.
impl Component for Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse {const MAX_WIRE:usize=200;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertLatLonToCartesianArrayRequest.
impl Component for Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest {const MAX_WIRE:usize=492;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ConvertLatLonToCartesianArrayResponse.
impl Component for Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse {const MAX_WIRE:usize=404;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ResetLocalizationRequest.
impl Component for Ros1Clearpath_localization_msgsResetLocalizationRequest {const MAX_WIRE:usize=5;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/ResetLocalizationResponse.
impl Component for Ros1Clearpath_localization_msgsResetLocalizationResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/SetDatumRequest.
impl Component for Ros1Clearpath_localization_msgsSetDatumRequest {const MAX_WIRE:usize=8;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_localization_msgs/SetDatumResponse.
impl Component for Ros1Clearpath_localization_msgsSetDatumResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/DeleteLogRequest.
impl Component for Ros1Clearpath_logger_msgsDeleteLogRequest {const MAX_WIRE:usize=262;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/DeleteLogResponse.
impl Component for Ros1Clearpath_logger_msgsDeleteLogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/DownloadLogRequest.
impl Component for Ros1Clearpath_logger_msgsDownloadLogRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/DownloadLogResponse.
impl Component for Ros1Clearpath_logger_msgsDownloadLogResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/GetAllLogsRequest.
impl Component for Ros1Clearpath_logger_msgsGetAllLogsRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/GetAllLogsResponse.
impl Component for Ros1Clearpath_logger_msgsGetAllLogsResponse {const MAX_WIRE:usize=14132;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordCustomEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordCustomEventRequest {const MAX_WIRE:usize=916;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordCustomEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordCustomEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordErrorEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordErrorEventRequest {const MAX_WIRE:usize=524;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordErrorEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordErrorEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordEventRequest {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordLocationEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordLocationEventRequest {const MAX_WIRE:usize=1680;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordLocationEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordLocationEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordMediaEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordMediaEventRequest {const MAX_WIRE:usize=652;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordMediaEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordMediaEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordPowerEventRequest.
impl Component for Ros1Clearpath_logger_msgsRecordPowerEventRequest {const MAX_WIRE:usize=5;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/RecordPowerEventResponse.
impl Component for Ros1Clearpath_logger_msgsRecordPowerEventResponse {const MAX_WIRE:usize=3190;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/StartRecordingRequest.
impl Component for Ros1Clearpath_logger_msgsStartRecordingRequest {const MAX_WIRE:usize=132;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/StartRecordingResponse.
impl Component for Ros1Clearpath_logger_msgsStartRecordingResponse {const MAX_WIRE:usize=7064;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/StopRecordingRequest.
impl Component for Ros1Clearpath_logger_msgsStopRecordingRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_logger_msgs/StopRecordingResponse.
impl Component for Ros1Clearpath_logger_msgsStopRecordingResponse {const MAX_WIRE:usize=7064;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkEdgeRequest.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest {const MAX_WIRE:usize=793;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkEdgeResponse.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse {const MAX_WIRE:usize=1652;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkPathRequest.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest {const MAX_WIRE:usize=301;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkPathResponse.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkPointRequest.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest {const MAX_WIRE:usize=1325;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddNetworkPointResponse.
impl Component for Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse {const MAX_WIRE:usize=276;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddRemoveByIdRequest.
impl Component for Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest {const MAX_WIRE:usize=524;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddRemoveByIdResponse.
impl Component for Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddRemoveTagRequest.
impl Component for Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/AddRemoveTagResponse.
impl Component for Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse {const MAX_WIRE:usize=524;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCloneMissionRequest {const MAX_WIRE:usize=521;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCloneMissionResponse {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneNetworkMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest {const MAX_WIRE:usize=521;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CloneNetworkMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ClonePointOfInterestRequest.
impl Component for Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ClonePointOfInterestResponse.
impl Component for Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateMissionRequest {const MAX_WIRE:usize=1964;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateMissionResponse {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest {const MAX_WIRE:usize=1756;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateNetworkMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest {const MAX_WIRE:usize=2234;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateNetworkMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreatePointOfInterestRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest {const MAX_WIRE:usize=949;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreatePointOfInterestResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateTaskRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateTaskRequest {const MAX_WIRE:usize=2769;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateTaskResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateTaskResponse {const MAX_WIRE:usize=1457;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateWaypointRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateWaypointRequest {const MAX_WIRE:usize=1220;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateWaypointResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateWaypointResponse {const MAX_WIRE:usize=3350;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateWaypointFromPoiRequest.
impl Component for Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/CreateWaypointFromPoiResponse.
impl Component for Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse {const MAX_WIRE:usize=3350;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteByIdRequest.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteByIdRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteByIdResponse.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteByIdResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteEverythingRequest.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteEverythingResponse.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteMultipleRequest.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest {const MAX_WIRE:usize=784;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/DeleteMultipleResponse.
impl Component for Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse {const MAX_WIRE:usize=524;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ExportDataRequest.
impl Component for Ros1Clearpath_mission_manager_msgsExportDataRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ExportDataResponse.
impl Component for Ros1Clearpath_mission_manager_msgsExportDataResponse {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllMissionsRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllMissionsResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse {const MAX_WIRE:usize=26676;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllNetworkMapsRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllNetworkMapsResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse {const MAX_WIRE:usize=5220;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllNetworkMissionsRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllNetworkMissionsResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse {const MAX_WIRE:usize=27216;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllPointsOfInterestRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllPointsOfInterestResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse {const MAX_WIRE:usize=2422;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllTasksRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllTasksRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllTasksResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllTasksResponse {const MAX_WIRE:usize=2918;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllWaypointsRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetAllWaypointsResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse {const MAX_WIRE:usize=6704;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetEverythingRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetEverythingRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetEverythingResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetEverythingResponse {const MAX_WIRE:usize=32768;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetMissionRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetMissionResponse {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetNetworkMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetNetworkMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetPointOfInterestRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetPointOfInterestResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetTaskRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetTaskRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetTaskResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetTaskResponse {const MAX_WIRE:usize=1457;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetWaypointRequest.
impl Component for Ros1Clearpath_mission_manager_msgsGetWaypointRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/GetWaypointResponse.
impl Component for Ros1Clearpath_mission_manager_msgsGetWaypointResponse {const MAX_WIRE:usize=3350;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportDataRequest.
impl Component for Ros1Clearpath_mission_manager_msgsImportDataRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportDataResponse.
impl Component for Ros1Clearpath_mission_manager_msgsImportDataResponse {const MAX_WIRE:usize=32768;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsImportMissionRequest {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsImportMissionResponse {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportNetworkMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ImportNetworkMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/SplitNetworkEdgeRequest.
impl Component for Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest {const MAX_WIRE:usize=537;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/SplitNetworkEdgeResponse.
impl Component for Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateMissionRequest {const MAX_WIRE:usize=2224;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateMissionResponse {const MAX_WIRE:usize=13336;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkEdgeRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest {const MAX_WIRE:usize=1052;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkEdgeResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse {const MAX_WIRE:usize=824;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkEdgesRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest {const MAX_WIRE:usize=820;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkEdgesResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse {const MAX_WIRE:usize=1652;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest {const MAX_WIRE:usize=400;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse {const MAX_WIRE:usize=2608;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkMissionRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest {const MAX_WIRE:usize=2494;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkMissionResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse {const MAX_WIRE:usize=13606;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkPointRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest {const MAX_WIRE:usize=536;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkPointResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse {const MAX_WIRE:usize=276;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkPointsRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest {const MAX_WIRE:usize=824;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateNetworkPointsResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse {const MAX_WIRE:usize=556;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdatePointOfInterestRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdatePointOfInterestResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse {const MAX_WIRE:usize=1209;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateTaskRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateTaskRequest {const MAX_WIRE:usize=1457;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateTaskResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateTaskResponse {const MAX_WIRE:usize=1457;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateWaypointRequest.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest {const MAX_WIRE:usize=956;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/UpdateWaypointResponse.
impl Component for Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse {const MAX_WIRE:usize=3350;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ValidateNetworkMapRequest.
impl Component for Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_manager_msgs/ValidateNetworkMapResponse.
impl Component for Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse {const MAX_WIRE:usize=790;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/CloneScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest {const MAX_WIRE:usize=520;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/CloneScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse {const MAX_WIRE:usize=944;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/CreateScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest {const MAX_WIRE:usize=684;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/CreateScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse {const MAX_WIRE:usize=944;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/EnableScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/EnableScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/ExportDataRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsExportDataRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/ExportDataResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsExportDataResponse {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetAllSchedulesRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetAllSchedulesResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse {const MAX_WIRE:usize=1892;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetNextScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest {const MAX_WIRE:usize=0;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetNextScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse {const MAX_WIRE:usize=268;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/GetScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse {const MAX_WIRE:usize=944;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/ImportDataRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsImportDataRequest {const MAX_WIRE:usize=260;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/ImportDataResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsImportDataResponse {const MAX_WIRE:usize=1892;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/UpdateScheduleRequest.
impl Component for Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest {const MAX_WIRE:usize=944;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_mission_scheduler_msgs/UpdateScheduleResponse.
impl Component for Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse {const MAX_WIRE:usize=944;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/AutonomyTaskSrvRequest.
impl Component for Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest {const MAX_WIRE:usize=804;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/AutonomyTaskSrvResponse.
impl Component for Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse {const MAX_WIRE:usize=2;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetDelayCompensationRequest.
impl Component for Ros1Clearpath_navigation_msgsSetDelayCompensationRequest {const MAX_WIRE:usize=3;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetDelayCompensationResponse.
impl Component for Ros1Clearpath_navigation_msgsSetDelayCompensationResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetPathShifterRequest.
impl Component for Ros1Clearpath_navigation_msgsSetPathShifterRequest {const MAX_WIRE:usize=9;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetPathShifterResponse.
impl Component for Ros1Clearpath_navigation_msgsSetPathShifterResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetPathSmootherRequest.
impl Component for Ros1Clearpath_navigation_msgsSetPathSmootherRequest {const MAX_WIRE:usize=9;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetPathSmootherResponse.
impl Component for Ros1Clearpath_navigation_msgsSetPathSmootherResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetStopDistanceRequest.
impl Component for Ros1Clearpath_navigation_msgsSetStopDistanceRequest {const MAX_WIRE:usize=5;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_navigation_msgs/SetStopDistanceResponse.
impl Component for Ros1Clearpath_navigation_msgsSetStopDistanceResponse {const MAX_WIRE:usize=261;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddCommunicationWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest {const MAX_WIRE:usize=1961;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddCommunicationWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddHeartbeatWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest {const MAX_WIRE:usize=1441;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddHeartbeatWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddInclinationWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest {const MAX_WIRE:usize=1449;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddInclinationWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNodeStatusWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest {const MAX_WIRE:usize=1438;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNodeStatusWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNodeletStatusWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest {const MAX_WIRE:usize=1698;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNodeletStatusWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNumPointsWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest {const MAX_WIRE:usize=1441;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddNumPointsWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddOdomCovarianceWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest {const MAX_WIRE:usize=1449;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddOdomCovarianceWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddRtkFixWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest {const MAX_WIRE:usize=1443;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddRtkFixWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddTopicDataWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest {const MAX_WIRE:usize=1441;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/AddTopicDataWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/RemoveWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsRemoveWatchdogRequest {const MAX_WIRE:usize=132;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/RemoveWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsRemoveWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateCommunicationWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest {const MAX_WIRE:usize=2221;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateCommunicationWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateHeartbeatWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest {const MAX_WIRE:usize=1701;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateHeartbeatWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateInclinationWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest {const MAX_WIRE:usize=1709;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateInclinationWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNodeStatusWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest {const MAX_WIRE:usize=1698;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNodeStatusWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNodeletStatusWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest {const MAX_WIRE:usize=1958;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNodeletStatusWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNumPointsWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest {const MAX_WIRE:usize=1701;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateNumPointsWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateOdomCovarianceWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest {const MAX_WIRE:usize=1709;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateOdomCovarianceWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateRtkFixWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest {const MAX_WIRE:usize=1703;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateRtkFixWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateTopicDataWatchdogRequest.
impl Component for Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest {const MAX_WIRE:usize=1701;const ROS_VERSION:u8=1;}

/// Complete canonical component bound for clearpath_safety_msgs/UpdateTopicDataWatchdogResponse.
impl Component for Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse {const MAX_WIRE:usize=1;const ROS_VERSION:u8=1;}

/// Complete custom clearpath_camera_msgs/InspectPOI client; four roles plus optional cancel-any.
pub type Ros1Clearpath_camera_msgsInspectPOIClient=ActionClient<Ros1Clearpath_camera_msgsInspectPOIGoal,Ros1Clearpath_camera_msgsInspectPOIResult,Ros1Clearpath_camera_msgsInspectPOIFeedback>;

/// Complete custom clearpath_camera_msgs/InspectPOI server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_camera_msgsInspectPOIServer=ActionServer<Ros1Clearpath_camera_msgsInspectPOIGoal,Ros1Clearpath_camera_msgsInspectPOIResult,Ros1Clearpath_camera_msgsInspectPOIFeedback>;

/// Complete custom clearpath_dock_msgs/LocalDock client; four roles plus optional cancel-any.
pub type Ros1Clearpath_dock_msgsLocalDockClient=ActionClient<Ros1Clearpath_dock_msgsLocalDockGoal,Ros1Clearpath_dock_msgsLocalDockResult,Ros1Clearpath_dock_msgsLocalDockFeedback>;

/// Complete custom clearpath_dock_msgs/LocalDock server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_dock_msgsLocalDockServer=ActionServer<Ros1Clearpath_dock_msgsLocalDockGoal,Ros1Clearpath_dock_msgsLocalDockResult,Ros1Clearpath_dock_msgsLocalDockFeedback>;

/// Complete custom clearpath_dock_msgs/LocalUndock client; four roles plus optional cancel-any.
pub type Ros1Clearpath_dock_msgsLocalUndockClient=ActionClient<Ros1Clearpath_dock_msgsLocalUndockGoal,Ros1Clearpath_dock_msgsLocalUndockResult,Ros1Clearpath_dock_msgsLocalUndockFeedback>;

/// Complete custom clearpath_dock_msgs/LocalUndock server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_dock_msgsLocalUndockServer=ActionServer<Ros1Clearpath_dock_msgsLocalUndockGoal,Ros1Clearpath_dock_msgsLocalUndockResult,Ros1Clearpath_dock_msgsLocalUndockFeedback>;

/// Complete custom clearpath_dock_msgs/NetworkDock client; four roles plus optional cancel-any.
pub type Ros1Clearpath_dock_msgsNetworkDockClient=ActionClient<Ros1Clearpath_dock_msgsNetworkDockGoal,Ros1Clearpath_dock_msgsNetworkDockResult,Ros1Clearpath_dock_msgsNetworkDockFeedback>;

/// Complete custom clearpath_dock_msgs/NetworkDock server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_dock_msgsNetworkDockServer=ActionServer<Ros1Clearpath_dock_msgsNetworkDockGoal,Ros1Clearpath_dock_msgsNetworkDockResult,Ros1Clearpath_dock_msgsNetworkDockFeedback>;

/// Complete custom clearpath_dock_msgs/RadiusDock client; four roles plus optional cancel-any.
pub type Ros1Clearpath_dock_msgsRadiusDockClient=ActionClient<Ros1Clearpath_dock_msgsRadiusDockGoal,Ros1Clearpath_dock_msgsRadiusDockResult,Ros1Clearpath_dock_msgsRadiusDockFeedback>;

/// Complete custom clearpath_dock_msgs/RadiusDock server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_dock_msgsRadiusDockServer=ActionServer<Ros1Clearpath_dock_msgsRadiusDockGoal,Ros1Clearpath_dock_msgsRadiusDockResult,Ros1Clearpath_dock_msgsRadiusDockFeedback>;

/// Complete custom clearpath_localization_msgs/SurveyBaseStation client; four roles plus optional cancel-any.
pub type Ros1Clearpath_localization_msgsSurveyBaseStationClient=ActionClient<Ros1Clearpath_localization_msgsSurveyBaseStationGoal,Ros1Clearpath_localization_msgsSurveyBaseStationResult,Ros1Clearpath_localization_msgsSurveyBaseStationFeedback>;

/// Complete custom clearpath_localization_msgs/SurveyBaseStation server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_localization_msgsSurveyBaseStationServer=ActionServer<Ros1Clearpath_localization_msgsSurveyBaseStationGoal,Ros1Clearpath_localization_msgsSurveyBaseStationResult,Ros1Clearpath_localization_msgsSurveyBaseStationFeedback>;

/// Complete custom clearpath_mission_scheduler_msgs/RunScheduleByUuid client; four roles plus optional cancel-any.
pub type Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient=ActionClient<Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoal,Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResult,Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedback>;

/// Complete custom clearpath_mission_scheduler_msgs/RunScheduleByUuid server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidServer=ActionServer<Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoal,Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResult,Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteMissionByUuid client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient=ActionClient<Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoal,Ros1Clearpath_navigation_msgsExecuteMissionByUuidResult,Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteMissionByUuid server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsExecuteMissionByUuidServer=ActionServer<Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoal,Ros1Clearpath_navigation_msgsExecuteMissionByUuidResult,Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteNetworkGoTo client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient=ActionClient<Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoal,Ros1Clearpath_navigation_msgsExecuteNetworkGoToResult,Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteNetworkGoTo server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsExecuteNetworkGoToServer=ActionServer<Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoal,Ros1Clearpath_navigation_msgsExecuteNetworkGoToResult,Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient=ActionClient<Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoal,Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResult,Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedback>;

/// Complete custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidServer=ActionServer<Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoal,Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResult,Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedback>;

/// Complete custom clearpath_navigation_msgs/GetWaypointPath client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsGetWaypointPathClient=ActionClient<Ros1Clearpath_navigation_msgsGetWaypointPathGoal,Ros1Clearpath_navigation_msgsGetWaypointPathResult,Ros1Clearpath_navigation_msgsGetWaypointPathFeedback>;

/// Complete custom clearpath_navigation_msgs/GetWaypointPath server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsGetWaypointPathServer=ActionServer<Ros1Clearpath_navigation_msgsGetWaypointPathGoal,Ros1Clearpath_navigation_msgsGetWaypointPathResult,Ros1Clearpath_navigation_msgsGetWaypointPathFeedback>;

/// Complete custom clearpath_navigation_msgs/Mission client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsMissionClient=ActionClient<Ros1Clearpath_navigation_msgsMissionGoal,Ros1Clearpath_navigation_msgsMissionResult,Ros1Clearpath_navigation_msgsMissionFeedback>;

/// Complete custom clearpath_navigation_msgs/Mission server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsMissionServer=ActionServer<Ros1Clearpath_navigation_msgsMissionGoal,Ros1Clearpath_navigation_msgsMissionResult,Ros1Clearpath_navigation_msgsMissionFeedback>;

/// Complete custom clearpath_navigation_msgs/NetworkGoToPOI client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsNetworkGoToPOIClient=ActionClient<Ros1Clearpath_navigation_msgsNetworkGoToPOIGoal,Ros1Clearpath_navigation_msgsNetworkGoToPOIResult,Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedback>;

/// Complete custom clearpath_navigation_msgs/NetworkGoToPOI server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsNetworkGoToPOIServer=ActionServer<Ros1Clearpath_navigation_msgsNetworkGoToPOIGoal,Ros1Clearpath_navigation_msgsNetworkGoToPOIResult,Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedback>;

/// Complete custom clearpath_navigation_msgs/RecordPath client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsRecordPathClient=ActionClient<Ros1Clearpath_navigation_msgsRecordPathGoal,Ros1Clearpath_navigation_msgsRecordPathResult,Ros1Clearpath_navigation_msgsRecordPathFeedback>;

/// Complete custom clearpath_navigation_msgs/RecordPath server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsRecordPathServer=ActionServer<Ros1Clearpath_navigation_msgsRecordPathGoal,Ros1Clearpath_navigation_msgsRecordPathResult,Ros1Clearpath_navigation_msgsRecordPathFeedback>;

/// Complete custom clearpath_navigation_msgs/RunNetworkGoToPlanner client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient=ActionClient<Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoal,Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResult,Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedback>;

/// Complete custom clearpath_navigation_msgs/RunNetworkGoToPlanner server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerServer=ActionServer<Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoal,Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResult,Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedback>;

/// Complete custom clearpath_navigation_msgs/RunNetworkMissionPreviewer client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient=ActionClient<Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoal,Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResult,Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedback>;

/// Complete custom clearpath_navigation_msgs/RunNetworkMissionPreviewer server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerServer=ActionServer<Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoal,Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResult,Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedback>;

/// Complete custom clearpath_navigation_msgs/SmoothPath client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsSmoothPathClient=ActionClient<Ros1Clearpath_navigation_msgsSmoothPathGoal,Ros1Clearpath_navigation_msgsSmoothPathResult,Ros1Clearpath_navigation_msgsSmoothPathFeedback>;

/// Complete custom clearpath_navigation_msgs/SmoothPath server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsSmoothPathServer=ActionServer<Ros1Clearpath_navigation_msgsSmoothPathGoal,Ros1Clearpath_navigation_msgsSmoothPathResult,Ros1Clearpath_navigation_msgsSmoothPathFeedback>;

/// Complete custom clearpath_navigation_msgs/UITask client; four roles plus optional cancel-any.
pub type Ros1Clearpath_navigation_msgsUITaskClient=ActionClient<Ros1Clearpath_navigation_msgsUITaskGoal,Ros1Clearpath_navigation_msgsUITaskResult,Ros1Clearpath_navigation_msgsUITaskFeedback>;

/// Complete custom clearpath_navigation_msgs/UITask server; feedback1/terminal2..4 remain distinct.
pub type Ros1Clearpath_navigation_msgsUITaskServer=ActionServer<Ros1Clearpath_navigation_msgsUITaskGoal,Ros1Clearpath_navigation_msgsUITaskResult,Ros1Clearpath_navigation_msgsUITaskFeedback>;

/// Typed clearpath_camera_msgs/q62AreaZoom call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_camera_msgsQ62AreaZoomClient=ServiceClient<Ros1Clearpath_camera_msgsQ62AreaZoomRequest,Ros1Clearpath_camera_msgsQ62AreaZoomResponse>;

/// Typed clearpath_camera_msgs/q62AreaZoom advertiser/responder with independent grants.
pub type Ros1Clearpath_camera_msgsQ62AreaZoomServer=ServiceServer<Ros1Clearpath_camera_msgsQ62AreaZoomRequest,Ros1Clearpath_camera_msgsQ62AreaZoomResponse>;

/// Typed clearpath_control_msgs/SetControlMode call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_control_msgsSetControlModeClient=ServiceClient<Ros1Clearpath_control_msgsSetControlModeRequest,Ros1Clearpath_control_msgsSetControlModeResponse>;

/// Typed clearpath_control_msgs/SetControlMode advertiser/responder with independent grants.
pub type Ros1Clearpath_control_msgsSetControlModeServer=ServiceServer<Ros1Clearpath_control_msgsSetControlModeRequest,Ros1Clearpath_control_msgsSetControlModeResponse>;

/// Typed clearpath_dock_msgs/AddDock call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsAddDockClient=ServiceClient<Ros1Clearpath_dock_msgsAddDockRequest,Ros1Clearpath_dock_msgsAddDockResponse>;

/// Typed clearpath_dock_msgs/AddDock advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsAddDockServer=ServiceServer<Ros1Clearpath_dock_msgsAddDockRequest,Ros1Clearpath_dock_msgsAddDockResponse>;

/// Typed clearpath_dock_msgs/ExportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsExportDataClient=ServiceClient<Ros1Clearpath_dock_msgsExportDataRequest,Ros1Clearpath_dock_msgsExportDataResponse>;

/// Typed clearpath_dock_msgs/ExportData advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsExportDataServer=ServiceServer<Ros1Clearpath_dock_msgsExportDataRequest,Ros1Clearpath_dock_msgsExportDataResponse>;

/// Typed clearpath_dock_msgs/GetDockByUuid call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsGetDockByUuidClient=ServiceClient<Ros1Clearpath_dock_msgsGetDockByUuidRequest,Ros1Clearpath_dock_msgsGetDockByUuidResponse>;

/// Typed clearpath_dock_msgs/GetDockByUuid advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsGetDockByUuidServer=ServiceServer<Ros1Clearpath_dock_msgsGetDockByUuidRequest,Ros1Clearpath_dock_msgsGetDockByUuidResponse>;

/// Typed clearpath_dock_msgs/ImportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsImportDataClient=ServiceClient<Ros1Clearpath_dock_msgsImportDataRequest,Ros1Clearpath_dock_msgsImportDataResponse>;

/// Typed clearpath_dock_msgs/ImportData advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsImportDataServer=ServiceServer<Ros1Clearpath_dock_msgsImportDataRequest,Ros1Clearpath_dock_msgsImportDataResponse>;

/// Typed clearpath_dock_msgs/RemoveDock call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsRemoveDockClient=ServiceClient<Ros1Clearpath_dock_msgsRemoveDockRequest,Ros1Clearpath_dock_msgsRemoveDockResponse>;

/// Typed clearpath_dock_msgs/RemoveDock advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsRemoveDockServer=ServiceServer<Ros1Clearpath_dock_msgsRemoveDockRequest,Ros1Clearpath_dock_msgsRemoveDockResponse>;

/// Typed clearpath_dock_msgs/SetDockLocationById call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsSetDockLocationByIdClient=ServiceClient<Ros1Clearpath_dock_msgsSetDockLocationByIdRequest,Ros1Clearpath_dock_msgsSetDockLocationByIdResponse>;

/// Typed clearpath_dock_msgs/SetDockLocationById advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsSetDockLocationByIdServer=ServiceServer<Ros1Clearpath_dock_msgsSetDockLocationByIdRequest,Ros1Clearpath_dock_msgsSetDockLocationByIdResponse>;

/// Typed clearpath_dock_msgs/SetDockLocationByName call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_dock_msgsSetDockLocationByNameClient=ServiceClient<Ros1Clearpath_dock_msgsSetDockLocationByNameRequest,Ros1Clearpath_dock_msgsSetDockLocationByNameResponse>;

/// Typed clearpath_dock_msgs/SetDockLocationByName advertiser/responder with independent grants.
pub type Ros1Clearpath_dock_msgsSetDockLocationByNameServer=ServiceServer<Ros1Clearpath_dock_msgsSetDockLocationByNameRequest,Ros1Clearpath_dock_msgsSetDockLocationByNameResponse>;

/// Typed clearpath_localization_msgs/ConvertCartesianToLatLon call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient=ServiceClient<Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest,Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse>;

/// Typed clearpath_localization_msgs/ConvertCartesianToLatLon advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsConvertCartesianToLatLonServer=ServiceServer<Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest,Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse>;

/// Typed clearpath_localization_msgs/ConvertCartesianToLatLonArray call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient=ServiceClient<Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest,Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse>;

/// Typed clearpath_localization_msgs/ConvertCartesianToLatLonArray advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayServer=ServiceServer<Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest,Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse>;

/// Typed clearpath_localization_msgs/ConvertLatLonToCartesian call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient=ServiceClient<Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest,Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse>;

/// Typed clearpath_localization_msgs/ConvertLatLonToCartesian advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsConvertLatLonToCartesianServer=ServiceServer<Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest,Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse>;

/// Typed clearpath_localization_msgs/ConvertLatLonToCartesianArray call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient=ServiceClient<Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest,Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse>;

/// Typed clearpath_localization_msgs/ConvertLatLonToCartesianArray advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayServer=ServiceServer<Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest,Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse>;

/// Typed clearpath_localization_msgs/ResetLocalization call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsResetLocalizationClient=ServiceClient<Ros1Clearpath_localization_msgsResetLocalizationRequest,Ros1Clearpath_localization_msgsResetLocalizationResponse>;

/// Typed clearpath_localization_msgs/ResetLocalization advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsResetLocalizationServer=ServiceServer<Ros1Clearpath_localization_msgsResetLocalizationRequest,Ros1Clearpath_localization_msgsResetLocalizationResponse>;

/// Typed clearpath_localization_msgs/SetDatum call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_localization_msgsSetDatumClient=ServiceClient<Ros1Clearpath_localization_msgsSetDatumRequest,Ros1Clearpath_localization_msgsSetDatumResponse>;

/// Typed clearpath_localization_msgs/SetDatum advertiser/responder with independent grants.
pub type Ros1Clearpath_localization_msgsSetDatumServer=ServiceServer<Ros1Clearpath_localization_msgsSetDatumRequest,Ros1Clearpath_localization_msgsSetDatumResponse>;

/// Typed clearpath_logger_msgs/DeleteLog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsDeleteLogClient=ServiceClient<Ros1Clearpath_logger_msgsDeleteLogRequest,Ros1Clearpath_logger_msgsDeleteLogResponse>;

/// Typed clearpath_logger_msgs/DeleteLog advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsDeleteLogServer=ServiceServer<Ros1Clearpath_logger_msgsDeleteLogRequest,Ros1Clearpath_logger_msgsDeleteLogResponse>;

/// Typed clearpath_logger_msgs/DownloadLog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsDownloadLogClient=ServiceClient<Ros1Clearpath_logger_msgsDownloadLogRequest,Ros1Clearpath_logger_msgsDownloadLogResponse>;

/// Typed clearpath_logger_msgs/DownloadLog advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsDownloadLogServer=ServiceServer<Ros1Clearpath_logger_msgsDownloadLogRequest,Ros1Clearpath_logger_msgsDownloadLogResponse>;

/// Typed clearpath_logger_msgs/GetAllLogs call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsGetAllLogsClient=ServiceClient<Ros1Clearpath_logger_msgsGetAllLogsRequest,Ros1Clearpath_logger_msgsGetAllLogsResponse>;

/// Typed clearpath_logger_msgs/GetAllLogs advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsGetAllLogsServer=ServiceServer<Ros1Clearpath_logger_msgsGetAllLogsRequest,Ros1Clearpath_logger_msgsGetAllLogsResponse>;

/// Typed clearpath_logger_msgs/RecordCustomEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordCustomEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordCustomEventRequest,Ros1Clearpath_logger_msgsRecordCustomEventResponse>;

/// Typed clearpath_logger_msgs/RecordCustomEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordCustomEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordCustomEventRequest,Ros1Clearpath_logger_msgsRecordCustomEventResponse>;

/// Typed clearpath_logger_msgs/RecordErrorEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordErrorEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordErrorEventRequest,Ros1Clearpath_logger_msgsRecordErrorEventResponse>;

/// Typed clearpath_logger_msgs/RecordErrorEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordErrorEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordErrorEventRequest,Ros1Clearpath_logger_msgsRecordErrorEventResponse>;

/// Typed clearpath_logger_msgs/RecordEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordEventRequest,Ros1Clearpath_logger_msgsRecordEventResponse>;

/// Typed clearpath_logger_msgs/RecordEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordEventRequest,Ros1Clearpath_logger_msgsRecordEventResponse>;

/// Typed clearpath_logger_msgs/RecordLocationEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordLocationEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordLocationEventRequest,Ros1Clearpath_logger_msgsRecordLocationEventResponse>;

/// Typed clearpath_logger_msgs/RecordLocationEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordLocationEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordLocationEventRequest,Ros1Clearpath_logger_msgsRecordLocationEventResponse>;

/// Typed clearpath_logger_msgs/RecordMediaEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordMediaEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordMediaEventRequest,Ros1Clearpath_logger_msgsRecordMediaEventResponse>;

/// Typed clearpath_logger_msgs/RecordMediaEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordMediaEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordMediaEventRequest,Ros1Clearpath_logger_msgsRecordMediaEventResponse>;

/// Typed clearpath_logger_msgs/RecordPowerEvent call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsRecordPowerEventClient=ServiceClient<Ros1Clearpath_logger_msgsRecordPowerEventRequest,Ros1Clearpath_logger_msgsRecordPowerEventResponse>;

/// Typed clearpath_logger_msgs/RecordPowerEvent advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsRecordPowerEventServer=ServiceServer<Ros1Clearpath_logger_msgsRecordPowerEventRequest,Ros1Clearpath_logger_msgsRecordPowerEventResponse>;

/// Typed clearpath_logger_msgs/StartRecording call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsStartRecordingClient=ServiceClient<Ros1Clearpath_logger_msgsStartRecordingRequest,Ros1Clearpath_logger_msgsStartRecordingResponse>;

/// Typed clearpath_logger_msgs/StartRecording advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsStartRecordingServer=ServiceServer<Ros1Clearpath_logger_msgsStartRecordingRequest,Ros1Clearpath_logger_msgsStartRecordingResponse>;

/// Typed clearpath_logger_msgs/StopRecording call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_logger_msgsStopRecordingClient=ServiceClient<Ros1Clearpath_logger_msgsStopRecordingRequest,Ros1Clearpath_logger_msgsStopRecordingResponse>;

/// Typed clearpath_logger_msgs/StopRecording advertiser/responder with independent grants.
pub type Ros1Clearpath_logger_msgsStopRecordingServer=ServiceServer<Ros1Clearpath_logger_msgsStopRecordingRequest,Ros1Clearpath_logger_msgsStopRecordingResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkEdge call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient=ServiceClient<Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkEdge advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkEdgeServer=ServiceServer<Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkPath call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkPathClient=ServiceClient<Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest,Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkPath advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkPathServer=ServiceServer<Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest,Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkPoint call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkPointClient=ServiceClient<Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest,Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse>;

/// Typed clearpath_mission_manager_msgs/AddNetworkPoint advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsAddNetworkPointServer=ServiceServer<Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest,Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse>;

/// Typed clearpath_mission_manager_msgs/AddRemoveById call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient=ServiceClient<Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest,Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse>;

/// Typed clearpath_mission_manager_msgs/AddRemoveById advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsAddRemoveByIdServer=ServiceServer<Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest,Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse>;

/// Typed clearpath_mission_manager_msgs/AddRemoveTag call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsAddRemoveTagClient=ServiceClient<Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest,Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse>;

/// Typed clearpath_mission_manager_msgs/AddRemoveTag advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsAddRemoveTagServer=ServiceServer<Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest,Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse>;

/// Typed clearpath_mission_manager_msgs/CloneMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCloneMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCloneMissionRequest,Ros1Clearpath_mission_manager_msgsCloneMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CloneMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCloneMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCloneMissionRequest,Ros1Clearpath_mission_manager_msgsCloneMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CloneNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest,Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/CloneNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCloneNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest,Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/CloneNetworkMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CloneNetworkMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCloneNetworkMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/ClonePointOfInterest call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient=ServiceClient<Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/ClonePointOfInterest advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsClonePointOfInterestServer=ServiceServer<Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/CreateMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateMissionRequest,Ros1Clearpath_mission_manager_msgsCreateMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CreateMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateMissionRequest,Ros1Clearpath_mission_manager_msgsCreateMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CreateNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/CreateNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/CreateNetworkMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CreateNetworkMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateNetworkMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/CreatePointOfInterest call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/CreatePointOfInterest advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreatePointOfInterestServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/CreateTask call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateTaskClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateTaskRequest,Ros1Clearpath_mission_manager_msgsCreateTaskResponse>;

/// Typed clearpath_mission_manager_msgs/CreateTask advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateTaskServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateTaskRequest,Ros1Clearpath_mission_manager_msgsCreateTaskResponse>;

/// Typed clearpath_mission_manager_msgs/CreateWaypoint call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateWaypointClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateWaypointRequest,Ros1Clearpath_mission_manager_msgsCreateWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/CreateWaypoint advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateWaypointServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateWaypointRequest,Ros1Clearpath_mission_manager_msgsCreateWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/CreateWaypointFromPoi call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient=ServiceClient<Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest,Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse>;

/// Typed clearpath_mission_manager_msgs/CreateWaypointFromPoi advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiServer=ServiceServer<Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest,Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteById call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsDeleteByIdClient=ServiceClient<Ros1Clearpath_mission_manager_msgsDeleteByIdRequest,Ros1Clearpath_mission_manager_msgsDeleteByIdResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteById advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsDeleteByIdServer=ServiceServer<Ros1Clearpath_mission_manager_msgsDeleteByIdRequest,Ros1Clearpath_mission_manager_msgsDeleteByIdResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteEverything call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsDeleteEverythingClient=ServiceClient<Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest,Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteEverything advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsDeleteEverythingServer=ServiceServer<Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest,Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteMultiple call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsDeleteMultipleClient=ServiceClient<Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest,Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse>;

/// Typed clearpath_mission_manager_msgs/DeleteMultiple advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsDeleteMultipleServer=ServiceServer<Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest,Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse>;

/// Typed clearpath_mission_manager_msgs/ExportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsExportDataClient=ServiceClient<Ros1Clearpath_mission_manager_msgsExportDataRequest,Ros1Clearpath_mission_manager_msgsExportDataResponse>;

/// Typed clearpath_mission_manager_msgs/ExportData advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsExportDataServer=ServiceServer<Ros1Clearpath_mission_manager_msgsExportDataRequest,Ros1Clearpath_mission_manager_msgsExportDataResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllMissions call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllMissionsClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest,Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllMissions advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllMissionsServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest,Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllNetworkMaps call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest,Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllNetworkMaps advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest,Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllNetworkMissions call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest,Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllNetworkMissions advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest,Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllPointsOfInterest call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest,Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllPointsOfInterest advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest,Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllTasks call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllTasksClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllTasksRequest,Ros1Clearpath_mission_manager_msgsGetAllTasksResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllTasks advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllTasksServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllTasksRequest,Ros1Clearpath_mission_manager_msgsGetAllTasksResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllWaypoints call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest,Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse>;

/// Typed clearpath_mission_manager_msgs/GetAllWaypoints advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetAllWaypointsServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest,Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse>;

/// Typed clearpath_mission_manager_msgs/GetEverything call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetEverythingClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetEverythingRequest,Ros1Clearpath_mission_manager_msgsGetEverythingResponse>;

/// Typed clearpath_mission_manager_msgs/GetEverything advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetEverythingServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetEverythingRequest,Ros1Clearpath_mission_manager_msgsGetEverythingResponse>;

/// Typed clearpath_mission_manager_msgs/GetMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetMissionRequest,Ros1Clearpath_mission_manager_msgsGetMissionResponse>;

/// Typed clearpath_mission_manager_msgs/GetMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetMissionRequest,Ros1Clearpath_mission_manager_msgsGetMissionResponse>;

/// Typed clearpath_mission_manager_msgs/GetNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest,Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/GetNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest,Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/GetNetworkMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/GetNetworkMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetNetworkMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/GetPointOfInterest call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest,Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/GetPointOfInterest advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetPointOfInterestServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest,Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/GetTask call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetTaskClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetTaskRequest,Ros1Clearpath_mission_manager_msgsGetTaskResponse>;

/// Typed clearpath_mission_manager_msgs/GetTask advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetTaskServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetTaskRequest,Ros1Clearpath_mission_manager_msgsGetTaskResponse>;

/// Typed clearpath_mission_manager_msgs/GetWaypoint call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsGetWaypointClient=ServiceClient<Ros1Clearpath_mission_manager_msgsGetWaypointRequest,Ros1Clearpath_mission_manager_msgsGetWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/GetWaypoint advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsGetWaypointServer=ServiceServer<Ros1Clearpath_mission_manager_msgsGetWaypointRequest,Ros1Clearpath_mission_manager_msgsGetWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/ImportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsImportDataClient=ServiceClient<Ros1Clearpath_mission_manager_msgsImportDataRequest,Ros1Clearpath_mission_manager_msgsImportDataResponse>;

/// Typed clearpath_mission_manager_msgs/ImportData advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsImportDataServer=ServiceServer<Ros1Clearpath_mission_manager_msgsImportDataRequest,Ros1Clearpath_mission_manager_msgsImportDataResponse>;

/// Typed clearpath_mission_manager_msgs/ImportMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsImportMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsImportMissionRequest,Ros1Clearpath_mission_manager_msgsImportMissionResponse>;

/// Typed clearpath_mission_manager_msgs/ImportMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsImportMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsImportMissionRequest,Ros1Clearpath_mission_manager_msgsImportMissionResponse>;

/// Typed clearpath_mission_manager_msgs/ImportNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsImportNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest,Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/ImportNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsImportNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest,Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/ImportNetworkMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/ImportNetworkMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsImportNetworkMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/SplitNetworkEdge call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient=ServiceClient<Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/SplitNetworkEdge advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeServer=ServiceServer<Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateMissionRequest,Ros1Clearpath_mission_manager_msgsUpdateMissionResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateMissionRequest,Ros1Clearpath_mission_manager_msgsUpdateMissionResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkEdge call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkEdge advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkEdges call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkEdges advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkMission call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkMission advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkPoint call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkPoint advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkPointServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkPoints call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateNetworkPoints advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest,Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse>;

/// Typed clearpath_mission_manager_msgs/UpdatePointOfInterest call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/UpdatePointOfInterest advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest,Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateTask call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateTaskClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateTaskRequest,Ros1Clearpath_mission_manager_msgsUpdateTaskResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateTask advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateTaskServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateTaskRequest,Ros1Clearpath_mission_manager_msgsUpdateTaskResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateWaypoint call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsUpdateWaypointClient=ServiceClient<Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest,Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/UpdateWaypoint advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsUpdateWaypointServer=ServiceServer<Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest,Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse>;

/// Typed clearpath_mission_manager_msgs/ValidateNetworkMap call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient=ServiceClient<Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse>;

/// Typed clearpath_mission_manager_msgs/ValidateNetworkMap advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_manager_msgsValidateNetworkMapServer=ServiceServer<Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest,Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse>;

/// Typed clearpath_mission_scheduler_msgs/CloneSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest,Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/CloneSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsCloneScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest,Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/CreateSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest,Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/CreateSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsCreateScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest,Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/EnableSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest,Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/EnableSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsEnableScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest,Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/ExportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsExportDataClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsExportDataRequest,Ros1Clearpath_mission_scheduler_msgsExportDataResponse>;

/// Typed clearpath_mission_scheduler_msgs/ExportData advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsExportDataServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsExportDataRequest,Ros1Clearpath_mission_scheduler_msgsExportDataResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetAllSchedules call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest,Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetAllSchedules advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest,Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetNextSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest,Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetNextSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsGetNextScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest,Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsGetScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest,Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/GetSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsGetScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest,Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/ImportData call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsImportDataClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsImportDataRequest,Ros1Clearpath_mission_scheduler_msgsImportDataResponse>;

/// Typed clearpath_mission_scheduler_msgs/ImportData advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsImportDataServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsImportDataRequest,Ros1Clearpath_mission_scheduler_msgsImportDataResponse>;

/// Typed clearpath_mission_scheduler_msgs/UpdateSchedule call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient=ServiceClient<Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest,Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse>;

/// Typed clearpath_mission_scheduler_msgs/UpdateSchedule advertiser/responder with independent grants.
pub type Ros1Clearpath_mission_scheduler_msgsUpdateScheduleServer=ServiceServer<Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest,Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse>;

/// Typed clearpath_navigation_msgs/AutonomyTaskSrv call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient=ServiceClient<Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest,Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse>;

/// Typed clearpath_navigation_msgs/AutonomyTaskSrv advertiser/responder with independent grants.
pub type Ros1Clearpath_navigation_msgsAutonomyTaskSrvServer=ServiceServer<Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest,Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse>;

/// Typed clearpath_navigation_msgs/SetDelayCompensation call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_navigation_msgsSetDelayCompensationClient=ServiceClient<Ros1Clearpath_navigation_msgsSetDelayCompensationRequest,Ros1Clearpath_navigation_msgsSetDelayCompensationResponse>;

/// Typed clearpath_navigation_msgs/SetDelayCompensation advertiser/responder with independent grants.
pub type Ros1Clearpath_navigation_msgsSetDelayCompensationServer=ServiceServer<Ros1Clearpath_navigation_msgsSetDelayCompensationRequest,Ros1Clearpath_navigation_msgsSetDelayCompensationResponse>;

/// Typed clearpath_navigation_msgs/SetPathShifter call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_navigation_msgsSetPathShifterClient=ServiceClient<Ros1Clearpath_navigation_msgsSetPathShifterRequest,Ros1Clearpath_navigation_msgsSetPathShifterResponse>;

/// Typed clearpath_navigation_msgs/SetPathShifter advertiser/responder with independent grants.
pub type Ros1Clearpath_navigation_msgsSetPathShifterServer=ServiceServer<Ros1Clearpath_navigation_msgsSetPathShifterRequest,Ros1Clearpath_navigation_msgsSetPathShifterResponse>;

/// Typed clearpath_navigation_msgs/SetPathSmoother call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_navigation_msgsSetPathSmootherClient=ServiceClient<Ros1Clearpath_navigation_msgsSetPathSmootherRequest,Ros1Clearpath_navigation_msgsSetPathSmootherResponse>;

/// Typed clearpath_navigation_msgs/SetPathSmoother advertiser/responder with independent grants.
pub type Ros1Clearpath_navigation_msgsSetPathSmootherServer=ServiceServer<Ros1Clearpath_navigation_msgsSetPathSmootherRequest,Ros1Clearpath_navigation_msgsSetPathSmootherResponse>;

/// Typed clearpath_navigation_msgs/SetStopDistance call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_navigation_msgsSetStopDistanceClient=ServiceClient<Ros1Clearpath_navigation_msgsSetStopDistanceRequest,Ros1Clearpath_navigation_msgsSetStopDistanceResponse>;

/// Typed clearpath_navigation_msgs/SetStopDistance advertiser/responder with independent grants.
pub type Ros1Clearpath_navigation_msgsSetStopDistanceServer=ServiceServer<Ros1Clearpath_navigation_msgsSetStopDistanceRequest,Ros1Clearpath_navigation_msgsSetStopDistanceResponse>;

/// Typed clearpath_safety_msgs/AddCommunicationWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest,Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddCommunicationWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddCommunicationWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest,Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddHeartbeatWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest,Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddHeartbeatWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddHeartbeatWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest,Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddInclinationWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddInclinationWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest,Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddInclinationWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddInclinationWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest,Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNodeStatusWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest,Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNodeStatusWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddNodeStatusWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest,Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNodeletStatusWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest,Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNodeletStatusWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest,Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNumPointsWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest,Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddNumPointsWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddNumPointsWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest,Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddOdomCovarianceWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest,Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddOdomCovarianceWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest,Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddRtkFixWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest,Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddRtkFixWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddRtkFixWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest,Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddTopicDataWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest,Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse>;

/// Typed clearpath_safety_msgs/AddTopicDataWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsAddTopicDataWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest,Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse>;

/// Typed clearpath_safety_msgs/RemoveWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsRemoveWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsRemoveWatchdogRequest,Ros1Clearpath_safety_msgsRemoveWatchdogResponse>;

/// Typed clearpath_safety_msgs/RemoveWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsRemoveWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsRemoveWatchdogRequest,Ros1Clearpath_safety_msgsRemoveWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateCommunicationWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest,Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateCommunicationWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest,Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateHeartbeatWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest,Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateHeartbeatWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest,Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateInclinationWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest,Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateInclinationWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateInclinationWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest,Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNodeStatusWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNodeStatusWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNodeletStatusWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNodeletStatusWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNumPointsWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateNumPointsWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest,Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateOdomCovarianceWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest,Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateOdomCovarianceWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest,Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateRtkFixWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest,Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateRtkFixWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest,Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateTopicDataWatchdog call/real-submit owner; every alias is explicit.
pub type Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient=ServiceClient<Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest,Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse>;

/// Typed clearpath_safety_msgs/UpdateTopicDataWatchdog advertiser/responder with independent grants.
pub type Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogServer=ServiceServer<Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest,Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse>;
