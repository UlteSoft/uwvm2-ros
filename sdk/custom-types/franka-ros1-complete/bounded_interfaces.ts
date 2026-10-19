/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros1Franka_gripperGraspGoal,Ros1Franka_gripperGraspGoalCodec,Ros1Franka_gripperGraspResult,Ros1Franka_gripperGraspResultCodec,Ros1Franka_gripperGraspFeedback,Ros1Franka_gripperGraspFeedbackCodec,Ros1Franka_gripperHomingGoal,Ros1Franka_gripperHomingGoalCodec,Ros1Franka_gripperHomingResult,Ros1Franka_gripperHomingResultCodec,Ros1Franka_gripperHomingFeedback,Ros1Franka_gripperHomingFeedbackCodec,Ros1Franka_gripperMoveGoal,Ros1Franka_gripperMoveGoalCodec,Ros1Franka_gripperMoveResult,Ros1Franka_gripperMoveResultCodec,Ros1Franka_gripperMoveFeedback,Ros1Franka_gripperMoveFeedbackCodec,Ros1Franka_gripperStopGoal,Ros1Franka_gripperStopGoalCodec,Ros1Franka_gripperStopResult,Ros1Franka_gripperStopResultCodec,Ros1Franka_gripperStopFeedback,Ros1Franka_gripperStopFeedbackCodec,Ros1Franka_msgsErrorRecoveryGoal,Ros1Franka_msgsErrorRecoveryGoalCodec,Ros1Franka_msgsErrorRecoveryResult,Ros1Franka_msgsErrorRecoveryResultCodec,Ros1Franka_msgsErrorRecoveryFeedback,Ros1Franka_msgsErrorRecoveryFeedbackCodec,Ros1Franka_msgsSetCartesianImpedanceRequest,Ros1Franka_msgsSetCartesianImpedanceRequestCodec,Ros1Franka_msgsSetCartesianImpedanceResponse,Ros1Franka_msgsSetCartesianImpedanceResponseCodec,Ros1Franka_msgsSetEEFrameRequest,Ros1Franka_msgsSetEEFrameRequestCodec,Ros1Franka_msgsSetEEFrameResponse,Ros1Franka_msgsSetEEFrameResponseCodec,Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest,Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCodec,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCodec,Ros1Franka_msgsSetFullCollisionBehaviorRequest,Ros1Franka_msgsSetFullCollisionBehaviorRequestCodec,Ros1Franka_msgsSetFullCollisionBehaviorResponse,Ros1Franka_msgsSetFullCollisionBehaviorResponseCodec,Ros1Franka_msgsSetJointImpedanceRequest,Ros1Franka_msgsSetJointImpedanceRequestCodec,Ros1Franka_msgsSetJointImpedanceResponse,Ros1Franka_msgsSetJointImpedanceResponseCodec,Ros1Franka_msgsSetKFrameRequest,Ros1Franka_msgsSetKFrameRequestCodec,Ros1Franka_msgsSetKFrameResponse,Ros1Franka_msgsSetKFrameResponseCodec,Ros1Franka_msgsSetLoadRequest,Ros1Franka_msgsSetLoadRequestCodec,Ros1Franka_msgsSetLoadResponse,Ros1Franka_msgsSetLoadResponseCodec} from "./bounded_records";

/** Complete typed result. Inspect status before value; required is retained
 * only when the actual SDK import reports its output-length sentinel written. */
export class RPCReply<T> extends Reply {
 value:T|null=null;id:u64=0;kind:u8=0;event:UnifiedEvent|null=null;
 constructor(reply:Reply=new Reply()){super(reply.status,reply.bytes,reply.required,reply.requiredValid);}
}
/** Primary failure remains separate from every attempted retirement result. */
export class RPCCleanup {constructor(public primary:u32,public statuses:Array<u32>){} }
function rpcId(bytes:Uint8Array):u64{return bytes.length<8?0:load<u64>(bytes.dataStart);}
function rpcFrame(id:u64,body:Uint8Array,kind:i32=-1):Uint8Array{const prefix=kind<0?8:9,out=new Uint8Array(prefix+body.length);store<u64>(out.dataStart,id);if(kind>=0)out[8]=<u8>kind;out.set(body,prefix);return out;}
function rpcRemove(values:Array<u64>,id:u64):void{const at=values.indexOf(id);if(at>=0){values[at]=values[values.length-1];values.pop();}}
class RPCOwners {
 private caps:Array<Capability>=[];busy:bool=false;status:u32=0;
 constructionCleanup:RPCCleanup=new RPCCleanup(0,[]);
 constructor(api:Api,version:u32,aliases:Array<string>,public capacity:i32){
  if(api.backend!=version||capacity<1||capacity>1024||!aliases.length)this.status=Status.Invalid;
  for(let i=0;!this.status&&i<aliases.length;++i)if(!aliases[i].length||aliases[i].indexOf('\0')>=0||!validUnicode(aliases[i])||Uint8Array.wrap(String.UTF8.encode(aliases[i])).length>255||aliases.indexOf(aliases[i])!=i)this.status=Status.Invalid;
  for(let i=0;!this.status&&i<aliases.length;++i){const cap=new Capability(api);this.status=cap.open(aliases[i]);if(!this.status)this.caps.push(cap);}
  if(this.status)this.constructionCleanup=this.cleanup(this.status);
 }
 cap(index:i32):Capability{return this.caps[index];}
 check():u32{if(this.busy)return Status.Conflict;if(this.status)return this.status;return this.caps.length&&this.caps[0].isOpen()?0:Status.BadHandle;}
 available(index:i32):bool{return index>=0&&index<this.caps.length&&this.caps[index].isOpen();}
 cleanup(primary:u32=0,revoke:bool=false):RPCCleanup{if(this.busy)return new RPCCleanup(primary,[Status.Conflict]);const out=new Array<u32>();for(let i=this.caps.length-1;i>=0;--i)if(this.caps[i].isOpen())out.push(revoke?this.caps[i].revoke():this.caps[i].close());return new RPCCleanup(primary,out);}
 zero(index:i32,op:Operation,wire:Uint8Array):u32{const r=this.cap(index).request(op,wire,0);return r.status?r.status:r.bytes.length?Status.Invalid:0;}
}

/** Generic implementation instantiated only with the generated complete codecs.
 * Each validator runs before encoding, including zero-wire nested-empty values. */
export class BoundedServiceClient<Q,S> {
 private owners:RPCOwners;private pending:Array<u64>=[];
 private sequence:u64=0;private instance:u64=0;private session:u64=0;
 constructor(api:Api,private version:u32,alias:string,private requestCodec:Codec<Q>,private responseCodec:Codec<S>,private requestValid:(value:Q)=>bool,private responseMax:i32,observerAlias:string='',capacity:i32=8){this.owners=new RPCOwners(api,version,observerAlias.length?[alias,observerAlias]:[alias],capacity);}
 /** One synchronous call. No timeout/disconnect/TooLarge replay is attempted. */
 call(value:Q):RPCReply<S>{let status=this.owners.check();if(!status&&!this.requestValid(value))status=Status.Invalid;if(status)return new RPCReply<S>(new Reply(status));const r=this.owners.cap(0).request(Operation.service_call,this.requestCodec.encode(value),this.responseMax),out=new RPCReply<S>(r);if(!r.status){if(!this.responseCodec.valid(r.bytes))out.status=Status.Invalid;else out.value=this.responseCodec.decode(r.bytes);}return out;}
 /** Real service_submit with the explicitly opened independent observer. The
  * returned id means admission only. Observer polling is exclusive to this owner. */
 submit(value:Q):RPCReply<S>{let status=this.owners.check();if(!status&&!this.owners.available(1))status=Status.Denied;if(!status&&this.pending.length==this.owners.capacity)status=Status.TooLarge;if(!status&&!this.requestValid(value))status=Status.Invalid;const out=new RPCReply<S>(new Reply(status));if(status)return out;const admitted=this.owners.cap(0).submitService(this.owners.cap(1),this.requestCodec.encode(value));out.status=admitted.status;out.id=admitted.handle;if(!out.status){if(!out.id||this.pending.indexOf(out.id)>=0)out.status=Status.Invalid;else this.pending.push(out.id);}return out;}
 /** Use the existing SDK envelope decoder before source/stream/correlation and
  * component checks. Source retirement leaves pending IDs until explicit close. */
 pollCompletion():RPCReply<S>{let status=this.owners.check();if(!status&&!this.owners.available(1))status=Status.Denied;if(status)return new RPCReply<S>(new Reply(status));const r=this.owners.cap(1).pollEventV1(64+this.responseMax),out=new RPCReply<S>(r);if(r.status)return out;const event=decodeUnifiedEvent(r.bytes);if(event===null){out.status=Status.Invalid;return out;}
  if(event.backend!=this.version||event.source!=this.owners.cap(0).handle||this.sequence==u64.MAX_VALUE||event.sequence!=this.sequence+1||(this.sequence&&(event.instance!=this.instance||event.session!=this.session))){out.status=Status.Invalid;return out;}
  this.sequence=event.sequence;this.instance=event.instance;this.session=event.session;out.event=event;out.id=event.correlation;
  if(event.kind==EventKind.ResourceClosed){out.status=Status.BadHandle;return out;}if(this.pending.indexOf(event.correlation)<0){out.status=Status.Invalid;return out;}
  if(event.kind==EventKind.ResourceError){out.status=event.error;rpcRemove(this.pending,event.correlation);return out;}
  if(event.kind!=EventKind.ServiceResponse||!this.responseCodec.valid(event.body)){out.status=Status.Invalid;return out;}out.value=this.responseCodec.decode(event.body);rpcRemove(this.pending,event.correlation);return out;
 }
 /** Preserve all partial open cleanup facts independently of the primary status. */
 constructionCleanup():RPCCleanup{return this.owners.constructionCleanup;}
 /** Close each role once, preserving observer/source retirement errors. */
 closeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary);if(!this.owners.busy)this.pending.length=0;return r;}
 /** Explicitly revoke only these already granted aliases. */
 revokeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary,true);if(!this.owners.busy)this.pending.length=0;return r;}
}

/** Incoming custom service implementation. Host still validates exact responder,
 * ticket principal, deadline and one-shot ownership; no ticket is a capability. */
export class BoundedServiceServer<Q,S> {
 private owners:RPCOwners;private live:Array<u64>=[];
 constructor(api:Api,version:u32,advertise:string,respond:string,private input:Codec<Q>,private output:Codec<S>,private outputValid:(value:S)=>bool,private inputMax:i32,capacity:i32=8){this.owners=new RPCOwners(api,version,[advertise,respond],capacity);if(!this.owners.status){this.owners.status=this.owners.cap(0).advertiseService();if(this.owners.status)this.owners.constructionCleanup=this.owners.cleanup(this.owners.status);}}
 private pollInner():RPCReply<Q>{if(this.live.length==this.owners.capacity)return new RPCReply<Q>(new Reply(Status.TooLarge));const r=this.owners.cap(0).poll(this.inputMax+8),out=new RPCReply<Q>(r);if(!r.status){out.id=rpcId(r.bytes);const body=r.bytes.slice(8);if(!out.id||this.live.indexOf(out.id)>=0||!this.input.valid(body))out.status=Status.Invalid;else{out.value=this.input.decode(body);this.live.push(out.id);}}return out;}
 /** Poll at most one complete request; all malformed values leave outputs absent. */
 pollOnce():RPCReply<Q>{const status=this.owners.check();return status?new RPCReply<Q>(new Reply(status)):this.pollInner();}
 /** Respond once to a locally delivered ticket. Refusal retains local ownership. */
 respond(id:u64,value:S):u32{if(this.live.indexOf(id)<0)return Status.BadHandle;if(!this.outputValid(value))return Status.Invalid;const status=this.owners.zero(1,Operation.service_respond,rpcFrame(id,this.output.encode(value)));if(!status)rpcRemove(this.live,id);return status;}
 /** A normal callback may respond but cannot poll/close/revoke this owner.
  * Callback traps abort Wasm; normal error/decode/idle paths always release busy. */
 spinOnce(handler:(id:u64,value:Q)=>S):u32{const status=this.owners.check();if(status)return status;this.owners.busy=true;const request=this.pollInner();let result=request.status;if(!result)result=this.respond(request.id,handler(request.id,changetype<Q>(request.value)));this.owners.busy=false;return result;}
 /** Inspect partial construction rollback separately. */
 constructionCleanup():RPCCleanup{return this.owners.constructionCleanup;}
 /** Close both roles only after callback preflight. */
 closeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary);if(!this.owners.busy)this.live.length=0;return r;}
 /** Deliberately revoke both existing aliases. */
 revokeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary,true);if(!this.owners.busy)this.live.length=0;return r;}
}

/** Custom action owner with four distinct required roles and optional cancel-any. */
export class BoundedActionClient<G,R,F> {
 private owners:RPCOwners;private goals:Array<u64>=[];
 constructor(api:Api,version:u32,send:string,cancelOwn:string,feedback:string,result:string,private goal:Codec<G>,private resultCodec:Codec<R>,private feedbackCodec:Codec<F>,private goalValid:(value:G)=>bool,private resultMax:i32,private feedbackMax:i32,cancelAny:string='',capacity:i32=8){this.owners=new RPCOwners(api,version,cancelAny.length?[send,cancelOwn,feedback,result,cancelAny]:[send,cancelOwn,feedback,result],capacity);}
 /** Send a complete generated goal; capacity checked before any import. */
 sendGoal(value:G):RPCReply<R>{let status=this.owners.check();if(!status&&this.goals.length==this.owners.capacity)status=Status.TooLarge;if(!status&&!this.goalValid(value))status=Status.Invalid;const out=new RPCReply<R>(new Reply(status));if(status)return out;const r=this.owners.cap(0).request(Operation.action_send_goal,this.goal.encode(value),8);out.status=r.status;if(!r.status){out.id=rpcId(r.bytes);if(r.bytes.length!=8||!out.id||this.goals.indexOf(out.id)>=0)out.status=Status.Invalid;else this.goals.push(out.id);}return out;}
 private cancel(index:i32,operation:Operation,id:u64):RPCReply<R>{const r=this.owners.cap(index).request(operation,rpcFrame(id,new Uint8Array(0)),1),out=new RPCReply<R>(r);out.id=id;if(!r.status&&(r.bytes.length!=1||r.bytes[0]>1))out.status=Status.Invalid;return out;}
 /** Cancellation acknowledgement does not retire a local goal. */
 cancelOwn(id:u64):RPCReply<R>{let status=this.owners.check();if(!status&&this.goals.indexOf(id)<0)status=Status.BadHandle;return status?new RPCReply<R>(new Reply(status)):this.cancel(1,Operation.action_cancel_own_goal,id);}
 /** Zero means cancel-all and still requires the explicit fifth role. */
 cancelAny(id:u64):RPCReply<R>{let status=this.owners.check();if(!status&&!this.owners.available(4))status=Status.Denied;return status?new RPCReply<R>(new Reply(status)):this.cancel(4,Operation.action_cancel_any_goal,id);}
 /** Explicit cancel-all preserves the independent fifth-role requirement. */
 cancelAll():RPCReply<R>{return this.cancelAny(0);}
 /** Receive complete matching-ID feedback, retaining goal ownership on every refusal. */
 feedback(id:u64):RPCReply<F>{let status=this.owners.check();if(!status&&this.goals.indexOf(id)<0)status=Status.BadHandle;if(status)return new RPCReply<F>(new Reply(status));const r=this.owners.cap(2).request(Operation.action_receive_feedback,rpcFrame(id,new Uint8Array(0)),8+this.feedbackMax),out=new RPCReply<F>(r);out.id=id;if(!r.status){const body=r.bytes.slice(8);if(rpcId(r.bytes)!=id||!this.feedbackCodec.valid(body))out.status=Status.Invalid;else out.value=this.feedbackCodec.decode(body);}return out;}
 /** Only decoded terminal kinds2/3/4 remove one locally owned goal. */
 result(id:u64):RPCReply<R>{let status=this.owners.check();if(!status&&this.goals.indexOf(id)<0)status=Status.BadHandle;if(status)return new RPCReply<R>(new Reply(status));const r=this.owners.cap(3).request(Operation.action_receive_result,rpcFrame(id,new Uint8Array(0)),9+this.resultMax),out=new RPCReply<R>(r);out.id=id;if(!r.status){const body=r.bytes.slice(9);if(r.bytes.length<9||rpcId(r.bytes)!=id||r.bytes[8]<2||r.bytes[8]>4||!this.resultCodec.valid(body))out.status=Status.Invalid;else{out.kind=r.bytes[8];out.value=this.resultCodec.decode(body);rpcRemove(this.goals,id);}}return out;}
 /** Observe every acquisition rollback result. */
 constructionCleanup():RPCCleanup{return this.owners.constructionCleanup;}
 /** Close every role once. */
 closeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary);if(!this.owners.busy)this.goals.length=0;return r;}
 /** Revoke only existing role aliases. */
 revokeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary,true);if(!this.owners.busy)this.goals.length=0;return r;}
}

/** Incoming custom action: feedback1 retains; accepted terminal2/3/4 retires. */
export class BoundedActionServer<G,R,F> {
 private owners:RPCOwners;private live:Array<u64>=[];
 constructor(api:Api,version:u32,advertise:string,respond:string,private goal:Codec<G>,private resultCodec:Codec<R>,private feedbackCodec:Codec<F>,private resultValid:(value:R)=>bool,private feedbackValid:(value:F)=>bool,private goalMax:i32,capacity:i32=8){this.owners=new RPCOwners(api,version,[advertise,respond],capacity);if(!this.owners.status){this.owners.status=this.owners.zero(0,Operation.action_advertise,new Uint8Array(0));if(this.owners.status)this.owners.constructionCleanup=this.owners.cleanup(this.owners.status);}}
 private pollInner():RPCReply<G>{if(this.live.length==this.owners.capacity)return new RPCReply<G>(new Reply(Status.TooLarge));const r=this.owners.cap(0).poll(8+this.goalMax),out=new RPCReply<G>(r);if(!r.status){out.id=rpcId(r.bytes);const body=r.bytes.slice(8);if(!out.id||this.live.indexOf(out.id)>=0||!this.goal.valid(body))out.status=Status.Invalid;else{out.value=this.goal.decode(body);this.live.push(out.id);}}return out;}
 /** Poll once; nested dispatch returns Conflict before dequeue. */
 pollOnce():RPCReply<G>{const status=this.owners.check();return status?new RPCReply<G>(new Reply(status)):this.pollInner();}
 /** Respond only to a goal delivered to this owner. */
 respondFeedback(id:u64,value:F):u32{if(this.live.indexOf(id)<0)return Status.BadHandle;if(!this.feedbackValid(value))return Status.Invalid;return this.owners.zero(1,Operation.action_respond,rpcFrame(id,this.feedbackCodec.encode(value),1));}
 /** Validate terminal kind and component before any response import. */
 respondResult(id:u64,kind:u8,value:R):u32{if(this.live.indexOf(id)<0)return Status.BadHandle;if(kind<2||kind>4||!this.resultValid(value))return Status.Invalid;const status=this.owners.zero(1,Operation.action_respond,rpcFrame(id,this.resultCodec.encode(value),kind));if(!status)rpcRemove(this.live,id);return status;}
 /** Handler runs locally under a lease and explicitly chooses later responses. */
 spinOnce(handler:(id:u64,value:G)=>void):u32{const status=this.owners.check();if(status)return status;this.owners.busy=true;const request=this.pollInner();if(!request.status)handler(request.id,changetype<G>(request.value));this.owners.busy=false;return request.status;}
 /** Inspect original partial construction cleanup. */
 constructionCleanup():RPCCleanup{return this.owners.constructionCleanup;}
 /** Close both roles, preserving primary and every cleanup status. */
 closeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary);if(!this.owners.busy)this.live.length=0;return r;}
 /** Explicitly revoke the existing advertise/respond aliases. */
 revokeChecked(primary:u32=0):RPCCleanup{const r=this.owners.cleanup(primary,true);if(!this.owners.busy)this.live.length=0;return r;}
}

/** Complete custom franka_gripper/Grasp client. Role aliases never imply grants. */
export class Ros1Franka_gripperGraspClient extends BoundedActionClient<Ros1Franka_gripperGraspGoal,Ros1Franka_gripperGraspResult,Ros1Franka_gripperGraspFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Franka_gripperGraspGoalCodec(),new Ros1Franka_gripperGraspResultCodec(),new Ros1Franka_gripperGraspFeedbackCodec(),(value:Ros1Franka_gripperGraspGoal):bool=>value.validValue(),261,0,cancelAny,capacity);}}

/** Complete custom franka_gripper/Grasp server with matching-ID response codecs. */
export class Ros1Franka_gripperGraspServer extends BoundedActionServer<Ros1Franka_gripperGraspGoal,Ros1Franka_gripperGraspResult,Ros1Franka_gripperGraspFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_gripperGraspGoalCodec(),new Ros1Franka_gripperGraspResultCodec(),new Ros1Franka_gripperGraspFeedbackCodec(),(value:Ros1Franka_gripperGraspResult):bool=>value.validValue(),(value:Ros1Franka_gripperGraspFeedback):bool=>value.validValue(),40,capacity);}}

/** Complete custom franka_gripper/Homing client. Role aliases never imply grants. */
export class Ros1Franka_gripperHomingClient extends BoundedActionClient<Ros1Franka_gripperHomingGoal,Ros1Franka_gripperHomingResult,Ros1Franka_gripperHomingFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Franka_gripperHomingGoalCodec(),new Ros1Franka_gripperHomingResultCodec(),new Ros1Franka_gripperHomingFeedbackCodec(),(value:Ros1Franka_gripperHomingGoal):bool=>value.validValue(),261,0,cancelAny,capacity);}}

/** Complete custom franka_gripper/Homing server with matching-ID response codecs. */
export class Ros1Franka_gripperHomingServer extends BoundedActionServer<Ros1Franka_gripperHomingGoal,Ros1Franka_gripperHomingResult,Ros1Franka_gripperHomingFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_gripperHomingGoalCodec(),new Ros1Franka_gripperHomingResultCodec(),new Ros1Franka_gripperHomingFeedbackCodec(),(value:Ros1Franka_gripperHomingResult):bool=>value.validValue(),(value:Ros1Franka_gripperHomingFeedback):bool=>value.validValue(),0,capacity);}}

/** Complete custom franka_gripper/Move client. Role aliases never imply grants. */
export class Ros1Franka_gripperMoveClient extends BoundedActionClient<Ros1Franka_gripperMoveGoal,Ros1Franka_gripperMoveResult,Ros1Franka_gripperMoveFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Franka_gripperMoveGoalCodec(),new Ros1Franka_gripperMoveResultCodec(),new Ros1Franka_gripperMoveFeedbackCodec(),(value:Ros1Franka_gripperMoveGoal):bool=>value.validValue(),261,0,cancelAny,capacity);}}

/** Complete custom franka_gripper/Move server with matching-ID response codecs. */
export class Ros1Franka_gripperMoveServer extends BoundedActionServer<Ros1Franka_gripperMoveGoal,Ros1Franka_gripperMoveResult,Ros1Franka_gripperMoveFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_gripperMoveGoalCodec(),new Ros1Franka_gripperMoveResultCodec(),new Ros1Franka_gripperMoveFeedbackCodec(),(value:Ros1Franka_gripperMoveResult):bool=>value.validValue(),(value:Ros1Franka_gripperMoveFeedback):bool=>value.validValue(),16,capacity);}}

/** Complete custom franka_gripper/Stop client. Role aliases never imply grants. */
export class Ros1Franka_gripperStopClient extends BoundedActionClient<Ros1Franka_gripperStopGoal,Ros1Franka_gripperStopResult,Ros1Franka_gripperStopFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Franka_gripperStopGoalCodec(),new Ros1Franka_gripperStopResultCodec(),new Ros1Franka_gripperStopFeedbackCodec(),(value:Ros1Franka_gripperStopGoal):bool=>value.validValue(),261,0,cancelAny,capacity);}}

/** Complete custom franka_gripper/Stop server with matching-ID response codecs. */
export class Ros1Franka_gripperStopServer extends BoundedActionServer<Ros1Franka_gripperStopGoal,Ros1Franka_gripperStopResult,Ros1Franka_gripperStopFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_gripperStopGoalCodec(),new Ros1Franka_gripperStopResultCodec(),new Ros1Franka_gripperStopFeedbackCodec(),(value:Ros1Franka_gripperStopResult):bool=>value.validValue(),(value:Ros1Franka_gripperStopFeedback):bool=>value.validValue(),0,capacity);}}

/** Complete custom franka_msgs/ErrorRecovery client. Role aliases never imply grants. */
export class Ros1Franka_msgsErrorRecoveryClient extends BoundedActionClient<Ros1Franka_msgsErrorRecoveryGoal,Ros1Franka_msgsErrorRecoveryResult,Ros1Franka_msgsErrorRecoveryFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Franka_msgsErrorRecoveryGoalCodec(),new Ros1Franka_msgsErrorRecoveryResultCodec(),new Ros1Franka_msgsErrorRecoveryFeedbackCodec(),(value:Ros1Franka_msgsErrorRecoveryGoal):bool=>value.validValue(),0,0,cancelAny,capacity);}}

/** Complete custom franka_msgs/ErrorRecovery server with matching-ID response codecs. */
export class Ros1Franka_msgsErrorRecoveryServer extends BoundedActionServer<Ros1Franka_msgsErrorRecoveryGoal,Ros1Franka_msgsErrorRecoveryResult,Ros1Franka_msgsErrorRecoveryFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsErrorRecoveryGoalCodec(),new Ros1Franka_msgsErrorRecoveryResultCodec(),new Ros1Franka_msgsErrorRecoveryFeedbackCodec(),(value:Ros1Franka_msgsErrorRecoveryResult):bool=>value.validValue(),(value:Ros1Franka_msgsErrorRecoveryFeedback):bool=>value.validValue(),0,capacity);}}

/** Complete franka_msgs/SetCartesianImpedance typed call and real-submit client. */
export class Ros1Franka_msgsSetCartesianImpedanceClient extends BoundedServiceClient<Ros1Franka_msgsSetCartesianImpedanceRequest,Ros1Franka_msgsSetCartesianImpedanceResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetCartesianImpedanceRequestCodec(),new Ros1Franka_msgsSetCartesianImpedanceResponseCodec(),(value:Ros1Franka_msgsSetCartesianImpedanceRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetCartesianImpedance incoming request/response owner. */
export class Ros1Franka_msgsSetCartesianImpedanceServer extends BoundedServiceServer<Ros1Franka_msgsSetCartesianImpedanceRequest,Ros1Franka_msgsSetCartesianImpedanceResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetCartesianImpedanceRequestCodec(),new Ros1Franka_msgsSetCartesianImpedanceResponseCodec(),(value:Ros1Franka_msgsSetCartesianImpedanceResponse):bool=>value.validValue(),48,capacity);}}

/** Complete franka_msgs/SetEEFrame typed call and real-submit client. */
export class Ros1Franka_msgsSetEEFrameClient extends BoundedServiceClient<Ros1Franka_msgsSetEEFrameRequest,Ros1Franka_msgsSetEEFrameResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetEEFrameRequestCodec(),new Ros1Franka_msgsSetEEFrameResponseCodec(),(value:Ros1Franka_msgsSetEEFrameRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetEEFrame incoming request/response owner. */
export class Ros1Franka_msgsSetEEFrameServer extends BoundedServiceServer<Ros1Franka_msgsSetEEFrameRequest,Ros1Franka_msgsSetEEFrameResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetEEFrameRequestCodec(),new Ros1Franka_msgsSetEEFrameResponseCodec(),(value:Ros1Franka_msgsSetEEFrameResponse):bool=>value.validValue(),128,capacity);}}

/** Complete franka_msgs/SetForceTorqueCollisionBehavior typed call and real-submit client. */
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient extends BoundedServiceClient<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCodec(),new Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCodec(),(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetForceTorqueCollisionBehavior incoming request/response owner. */
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorServer extends BoundedServiceServer<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest,Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCodec(),new Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCodec(),(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse):bool=>value.validValue(),208,capacity);}}

/** Complete franka_msgs/SetFullCollisionBehavior typed call and real-submit client. */
export class Ros1Franka_msgsSetFullCollisionBehaviorClient extends BoundedServiceClient<Ros1Franka_msgsSetFullCollisionBehaviorRequest,Ros1Franka_msgsSetFullCollisionBehaviorResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetFullCollisionBehaviorRequestCodec(),new Ros1Franka_msgsSetFullCollisionBehaviorResponseCodec(),(value:Ros1Franka_msgsSetFullCollisionBehaviorRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetFullCollisionBehavior incoming request/response owner. */
export class Ros1Franka_msgsSetFullCollisionBehaviorServer extends BoundedServiceServer<Ros1Franka_msgsSetFullCollisionBehaviorRequest,Ros1Franka_msgsSetFullCollisionBehaviorResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetFullCollisionBehaviorRequestCodec(),new Ros1Franka_msgsSetFullCollisionBehaviorResponseCodec(),(value:Ros1Franka_msgsSetFullCollisionBehaviorResponse):bool=>value.validValue(),416,capacity);}}

/** Complete franka_msgs/SetJointImpedance typed call and real-submit client. */
export class Ros1Franka_msgsSetJointImpedanceClient extends BoundedServiceClient<Ros1Franka_msgsSetJointImpedanceRequest,Ros1Franka_msgsSetJointImpedanceResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetJointImpedanceRequestCodec(),new Ros1Franka_msgsSetJointImpedanceResponseCodec(),(value:Ros1Franka_msgsSetJointImpedanceRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetJointImpedance incoming request/response owner. */
export class Ros1Franka_msgsSetJointImpedanceServer extends BoundedServiceServer<Ros1Franka_msgsSetJointImpedanceRequest,Ros1Franka_msgsSetJointImpedanceResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetJointImpedanceRequestCodec(),new Ros1Franka_msgsSetJointImpedanceResponseCodec(),(value:Ros1Franka_msgsSetJointImpedanceResponse):bool=>value.validValue(),56,capacity);}}

/** Complete franka_msgs/SetKFrame typed call and real-submit client. */
export class Ros1Franka_msgsSetKFrameClient extends BoundedServiceClient<Ros1Franka_msgsSetKFrameRequest,Ros1Franka_msgsSetKFrameResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetKFrameRequestCodec(),new Ros1Franka_msgsSetKFrameResponseCodec(),(value:Ros1Franka_msgsSetKFrameRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetKFrame incoming request/response owner. */
export class Ros1Franka_msgsSetKFrameServer extends BoundedServiceServer<Ros1Franka_msgsSetKFrameRequest,Ros1Franka_msgsSetKFrameResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetKFrameRequestCodec(),new Ros1Franka_msgsSetKFrameResponseCodec(),(value:Ros1Franka_msgsSetKFrameResponse):bool=>value.validValue(),128,capacity);}}

/** Complete franka_msgs/SetLoad typed call and real-submit client. */
export class Ros1Franka_msgsSetLoadClient extends BoundedServiceClient<Ros1Franka_msgsSetLoadRequest,Ros1Franka_msgsSetLoadResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Franka_msgsSetLoadRequestCodec(),new Ros1Franka_msgsSetLoadResponseCodec(),(value:Ros1Franka_msgsSetLoadRequest):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/SetLoad incoming request/response owner. */
export class Ros1Franka_msgsSetLoadServer extends BoundedServiceServer<Ros1Franka_msgsSetLoadRequest,Ros1Franka_msgsSetLoadResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Franka_msgsSetLoadRequestCodec(),new Ros1Franka_msgsSetLoadResponseCodec(),(value:Ros1Franka_msgsSetLoadResponse):bool=>value.validValue(),104,capacity);}}
