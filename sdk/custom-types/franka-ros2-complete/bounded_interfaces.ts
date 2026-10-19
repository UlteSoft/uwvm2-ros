/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros2Franka_msgsActionErrorRecovery_Goal,Ros2Franka_msgsActionErrorRecovery_GoalCodec,Ros2Franka_msgsActionErrorRecovery_Result,Ros2Franka_msgsActionErrorRecovery_ResultCodec,Ros2Franka_msgsActionErrorRecovery_Feedback,Ros2Franka_msgsActionErrorRecovery_FeedbackCodec,Ros2Franka_msgsActionGrasp_Goal,Ros2Franka_msgsActionGrasp_GoalCodec,Ros2Franka_msgsActionGrasp_Result,Ros2Franka_msgsActionGrasp_ResultCodec,Ros2Franka_msgsActionGrasp_Feedback,Ros2Franka_msgsActionGrasp_FeedbackCodec,Ros2Franka_msgsActionHoming_Goal,Ros2Franka_msgsActionHoming_GoalCodec,Ros2Franka_msgsActionHoming_Result,Ros2Franka_msgsActionHoming_ResultCodec,Ros2Franka_msgsActionHoming_Feedback,Ros2Franka_msgsActionHoming_FeedbackCodec,Ros2Franka_msgsActionMove_Goal,Ros2Franka_msgsActionMove_GoalCodec,Ros2Franka_msgsActionMove_Result,Ros2Franka_msgsActionMove_ResultCodec,Ros2Franka_msgsActionMove_Feedback,Ros2Franka_msgsActionMove_FeedbackCodec,Ros2Franka_msgsActionPTPMotion_Goal,Ros2Franka_msgsActionPTPMotion_GoalCodec,Ros2Franka_msgsActionPTPMotion_Result,Ros2Franka_msgsActionPTPMotion_ResultCodec,Ros2Franka_msgsActionPTPMotion_Feedback,Ros2Franka_msgsActionPTPMotion_FeedbackCodec,Ros2Franka_msgsSrvSetCartesianStiffness_Request,Ros2Franka_msgsSrvSetCartesianStiffness_RequestCodec,Ros2Franka_msgsSrvSetCartesianStiffness_Response,Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCodec,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCodec,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCodec,Ros2Franka_msgsSrvSetFullCollisionBehavior_Request,Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCodec,Ros2Franka_msgsSrvSetFullCollisionBehavior_Response,Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCodec,Ros2Franka_msgsSrvSetJointStiffness_Request,Ros2Franka_msgsSrvSetJointStiffness_RequestCodec,Ros2Franka_msgsSrvSetJointStiffness_Response,Ros2Franka_msgsSrvSetJointStiffness_ResponseCodec,Ros2Franka_msgsSrvSetLoad_Request,Ros2Franka_msgsSrvSetLoad_RequestCodec,Ros2Franka_msgsSrvSetLoad_Response,Ros2Franka_msgsSrvSetLoad_ResponseCodec,Ros2Franka_msgsSrvSetStiffnessFrame_Request,Ros2Franka_msgsSrvSetStiffnessFrame_RequestCodec,Ros2Franka_msgsSrvSetStiffnessFrame_Response,Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCodec,Ros2Franka_msgsSrvSetTCPFrame_Request,Ros2Franka_msgsSrvSetTCPFrame_RequestCodec,Ros2Franka_msgsSrvSetTCPFrame_Response,Ros2Franka_msgsSrvSetTCPFrame_ResponseCodec,Ros2Franka_spine_msgsActionMoveAbsolute_Goal,Ros2Franka_spine_msgsActionMoveAbsolute_GoalCodec,Ros2Franka_spine_msgsActionMoveAbsolute_Result,Ros2Franka_spine_msgsActionMoveAbsolute_ResultCodec,Ros2Franka_spine_msgsActionMoveAbsolute_Feedback,Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCodec,Ros2Franka_spine_msgsSrvFaultReset_Request,Ros2Franka_spine_msgsSrvFaultReset_RequestCodec,Ros2Franka_spine_msgsSrvFaultReset_Response,Ros2Franka_spine_msgsSrvFaultReset_ResponseCodec,Ros2Franka_spine_msgsSrvGetParameters_Request,Ros2Franka_spine_msgsSrvGetParameters_RequestCodec,Ros2Franka_spine_msgsSrvGetParameters_Response,Ros2Franka_spine_msgsSrvGetParameters_ResponseCodec,Ros2Franka_spine_msgsSrvGetPosition_Request,Ros2Franka_spine_msgsSrvGetPosition_RequestCodec,Ros2Franka_spine_msgsSrvGetPosition_Response,Ros2Franka_spine_msgsSrvGetPosition_ResponseCodec,Ros2Franka_spine_msgsSrvGetSpineState_Request,Ros2Franka_spine_msgsSrvGetSpineState_RequestCodec,Ros2Franka_spine_msgsSrvGetSpineState_Response,Ros2Franka_spine_msgsSrvGetSpineState_ResponseCodec,Ros2Franka_spine_msgsSrvHalt_Request,Ros2Franka_spine_msgsSrvHalt_RequestCodec,Ros2Franka_spine_msgsSrvHalt_Response,Ros2Franka_spine_msgsSrvHalt_ResponseCodec,Ros2Franka_spine_msgsSrvSwitchOff_Request,Ros2Franka_spine_msgsSrvSwitchOff_RequestCodec,Ros2Franka_spine_msgsSrvSwitchOff_Response,Ros2Franka_spine_msgsSrvSwitchOff_ResponseCodec,Ros2Franka_spine_msgsSrvSwitchOn_Request,Ros2Franka_spine_msgsSrvSwitchOn_RequestCodec,Ros2Franka_spine_msgsSrvSwitchOn_Response,Ros2Franka_spine_msgsSrvSwitchOn_ResponseCodec} from "./bounded_records";

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

/** Complete custom franka_msgs/action/ErrorRecovery client. Role aliases never imply grants. */
export class Ros2Franka_msgsActionErrorRecoveryClient extends BoundedActionClient<Ros2Franka_msgsActionErrorRecovery_Goal,Ros2Franka_msgsActionErrorRecovery_Result,Ros2Franka_msgsActionErrorRecovery_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_msgsActionErrorRecovery_GoalCodec(),new Ros2Franka_msgsActionErrorRecovery_ResultCodec(),new Ros2Franka_msgsActionErrorRecovery_FeedbackCodec(),(value:Ros2Franka_msgsActionErrorRecovery_Goal):bool=>value.validValue(),0,0,cancelAny,capacity);}}

/** Complete custom franka_msgs/action/ErrorRecovery server with matching-ID response codecs. */
export class Ros2Franka_msgsActionErrorRecoveryServer extends BoundedActionServer<Ros2Franka_msgsActionErrorRecovery_Goal,Ros2Franka_msgsActionErrorRecovery_Result,Ros2Franka_msgsActionErrorRecovery_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsActionErrorRecovery_GoalCodec(),new Ros2Franka_msgsActionErrorRecovery_ResultCodec(),new Ros2Franka_msgsActionErrorRecovery_FeedbackCodec(),(value:Ros2Franka_msgsActionErrorRecovery_Result):bool=>value.validValue(),(value:Ros2Franka_msgsActionErrorRecovery_Feedback):bool=>value.validValue(),0,capacity);}}

/** Complete custom franka_msgs/action/Grasp client. Role aliases never imply grants. */
export class Ros2Franka_msgsActionGraspClient extends BoundedActionClient<Ros2Franka_msgsActionGrasp_Goal,Ros2Franka_msgsActionGrasp_Result,Ros2Franka_msgsActionGrasp_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_msgsActionGrasp_GoalCodec(),new Ros2Franka_msgsActionGrasp_ResultCodec(),new Ros2Franka_msgsActionGrasp_FeedbackCodec(),(value:Ros2Franka_msgsActionGrasp_Goal):bool=>value.validValue(),261,8,cancelAny,capacity);}}

/** Complete custom franka_msgs/action/Grasp server with matching-ID response codecs. */
export class Ros2Franka_msgsActionGraspServer extends BoundedActionServer<Ros2Franka_msgsActionGrasp_Goal,Ros2Franka_msgsActionGrasp_Result,Ros2Franka_msgsActionGrasp_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsActionGrasp_GoalCodec(),new Ros2Franka_msgsActionGrasp_ResultCodec(),new Ros2Franka_msgsActionGrasp_FeedbackCodec(),(value:Ros2Franka_msgsActionGrasp_Result):bool=>value.validValue(),(value:Ros2Franka_msgsActionGrasp_Feedback):bool=>value.validValue(),40,capacity);}}

/** Complete custom franka_msgs/action/Homing client. Role aliases never imply grants. */
export class Ros2Franka_msgsActionHomingClient extends BoundedActionClient<Ros2Franka_msgsActionHoming_Goal,Ros2Franka_msgsActionHoming_Result,Ros2Franka_msgsActionHoming_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_msgsActionHoming_GoalCodec(),new Ros2Franka_msgsActionHoming_ResultCodec(),new Ros2Franka_msgsActionHoming_FeedbackCodec(),(value:Ros2Franka_msgsActionHoming_Goal):bool=>value.validValue(),261,8,cancelAny,capacity);}}

/** Complete custom franka_msgs/action/Homing server with matching-ID response codecs. */
export class Ros2Franka_msgsActionHomingServer extends BoundedActionServer<Ros2Franka_msgsActionHoming_Goal,Ros2Franka_msgsActionHoming_Result,Ros2Franka_msgsActionHoming_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsActionHoming_GoalCodec(),new Ros2Franka_msgsActionHoming_ResultCodec(),new Ros2Franka_msgsActionHoming_FeedbackCodec(),(value:Ros2Franka_msgsActionHoming_Result):bool=>value.validValue(),(value:Ros2Franka_msgsActionHoming_Feedback):bool=>value.validValue(),0,capacity);}}

/** Complete custom franka_msgs/action/Move client. Role aliases never imply grants. */
export class Ros2Franka_msgsActionMoveClient extends BoundedActionClient<Ros2Franka_msgsActionMove_Goal,Ros2Franka_msgsActionMove_Result,Ros2Franka_msgsActionMove_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_msgsActionMove_GoalCodec(),new Ros2Franka_msgsActionMove_ResultCodec(),new Ros2Franka_msgsActionMove_FeedbackCodec(),(value:Ros2Franka_msgsActionMove_Goal):bool=>value.validValue(),261,8,cancelAny,capacity);}}

/** Complete custom franka_msgs/action/Move server with matching-ID response codecs. */
export class Ros2Franka_msgsActionMoveServer extends BoundedActionServer<Ros2Franka_msgsActionMove_Goal,Ros2Franka_msgsActionMove_Result,Ros2Franka_msgsActionMove_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsActionMove_GoalCodec(),new Ros2Franka_msgsActionMove_ResultCodec(),new Ros2Franka_msgsActionMove_FeedbackCodec(),(value:Ros2Franka_msgsActionMove_Result):bool=>value.validValue(),(value:Ros2Franka_msgsActionMove_Feedback):bool=>value.validValue(),16,capacity);}}

/** Complete custom franka_msgs/action/PTPMotion client. Role aliases never imply grants. */
export class Ros2Franka_msgsActionPTPMotionClient extends BoundedActionClient<Ros2Franka_msgsActionPTPMotion_Goal,Ros2Franka_msgsActionPTPMotion_Result,Ros2Franka_msgsActionPTPMotion_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_msgsActionPTPMotion_GoalCodec(),new Ros2Franka_msgsActionPTPMotion_ResultCodec(),new Ros2Franka_msgsActionPTPMotion_FeedbackCodec(),(value:Ros2Franka_msgsActionPTPMotion_Goal):bool=>value.validValue(),261,1,cancelAny,capacity);}}

/** Complete custom franka_msgs/action/PTPMotion server with matching-ID response codecs. */
export class Ros2Franka_msgsActionPTPMotionServer extends BoundedActionServer<Ros2Franka_msgsActionPTPMotion_Goal,Ros2Franka_msgsActionPTPMotion_Result,Ros2Franka_msgsActionPTPMotion_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsActionPTPMotion_GoalCodec(),new Ros2Franka_msgsActionPTPMotion_ResultCodec(),new Ros2Franka_msgsActionPTPMotion_FeedbackCodec(),(value:Ros2Franka_msgsActionPTPMotion_Result):bool=>value.validValue(),(value:Ros2Franka_msgsActionPTPMotion_Feedback):bool=>value.validValue(),128,capacity);}}

/** Complete franka_msgs/srv/SetCartesianStiffness typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetCartesianStiffnessClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetCartesianStiffness_Request,Ros2Franka_msgsSrvSetCartesianStiffness_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetCartesianStiffness_RequestCodec(),new Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCodec(),(value:Ros2Franka_msgsSrvSetCartesianStiffness_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetCartesianStiffness incoming request/response owner. */
export class Ros2Franka_msgsSrvSetCartesianStiffnessServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetCartesianStiffness_Request,Ros2Franka_msgsSrvSetCartesianStiffness_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetCartesianStiffness_RequestCodec(),new Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCodec(),(value:Ros2Franka_msgsSrvSetCartesianStiffness_Response):bool=>value.validValue(),48,capacity);}}

/** Complete franka_msgs/srv/SetForceTorqueCollisionBehavior typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCodec(),new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCodec(),(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetForceTorqueCollisionBehavior incoming request/response owner. */
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request,Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCodec(),new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCodec(),(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response):bool=>value.validValue(),208,capacity);}}

/** Complete franka_msgs/srv/SetFullCollisionBehavior typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetFullCollisionBehaviorClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request,Ros2Franka_msgsSrvSetFullCollisionBehavior_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCodec(),new Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCodec(),(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetFullCollisionBehavior incoming request/response owner. */
export class Ros2Franka_msgsSrvSetFullCollisionBehaviorServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request,Ros2Franka_msgsSrvSetFullCollisionBehavior_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCodec(),new Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCodec(),(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Response):bool=>value.validValue(),416,capacity);}}

/** Complete franka_msgs/srv/SetJointStiffness typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetJointStiffnessClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetJointStiffness_Request,Ros2Franka_msgsSrvSetJointStiffness_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetJointStiffness_RequestCodec(),new Ros2Franka_msgsSrvSetJointStiffness_ResponseCodec(),(value:Ros2Franka_msgsSrvSetJointStiffness_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetJointStiffness incoming request/response owner. */
export class Ros2Franka_msgsSrvSetJointStiffnessServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetJointStiffness_Request,Ros2Franka_msgsSrvSetJointStiffness_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetJointStiffness_RequestCodec(),new Ros2Franka_msgsSrvSetJointStiffness_ResponseCodec(),(value:Ros2Franka_msgsSrvSetJointStiffness_Response):bool=>value.validValue(),56,capacity);}}

/** Complete franka_msgs/srv/SetLoad typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetLoadClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetLoad_Request,Ros2Franka_msgsSrvSetLoad_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetLoad_RequestCodec(),new Ros2Franka_msgsSrvSetLoad_ResponseCodec(),(value:Ros2Franka_msgsSrvSetLoad_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetLoad incoming request/response owner. */
export class Ros2Franka_msgsSrvSetLoadServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetLoad_Request,Ros2Franka_msgsSrvSetLoad_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetLoad_RequestCodec(),new Ros2Franka_msgsSrvSetLoad_ResponseCodec(),(value:Ros2Franka_msgsSrvSetLoad_Response):bool=>value.validValue(),104,capacity);}}

/** Complete franka_msgs/srv/SetStiffnessFrame typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetStiffnessFrameClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetStiffnessFrame_Request,Ros2Franka_msgsSrvSetStiffnessFrame_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetStiffnessFrame_RequestCodec(),new Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCodec(),(value:Ros2Franka_msgsSrvSetStiffnessFrame_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetStiffnessFrame incoming request/response owner. */
export class Ros2Franka_msgsSrvSetStiffnessFrameServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetStiffnessFrame_Request,Ros2Franka_msgsSrvSetStiffnessFrame_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetStiffnessFrame_RequestCodec(),new Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCodec(),(value:Ros2Franka_msgsSrvSetStiffnessFrame_Response):bool=>value.validValue(),128,capacity);}}

/** Complete franka_msgs/srv/SetTCPFrame typed call and real-submit client. */
export class Ros2Franka_msgsSrvSetTCPFrameClient extends BoundedServiceClient<Ros2Franka_msgsSrvSetTCPFrame_Request,Ros2Franka_msgsSrvSetTCPFrame_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_msgsSrvSetTCPFrame_RequestCodec(),new Ros2Franka_msgsSrvSetTCPFrame_ResponseCodec(),(value:Ros2Franka_msgsSrvSetTCPFrame_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete franka_msgs/srv/SetTCPFrame incoming request/response owner. */
export class Ros2Franka_msgsSrvSetTCPFrameServer extends BoundedServiceServer<Ros2Franka_msgsSrvSetTCPFrame_Request,Ros2Franka_msgsSrvSetTCPFrame_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_msgsSrvSetTCPFrame_RequestCodec(),new Ros2Franka_msgsSrvSetTCPFrame_ResponseCodec(),(value:Ros2Franka_msgsSrvSetTCPFrame_Response):bool=>value.validValue(),128,capacity);}}

/** Complete custom franka_spine_msgs/action/MoveAbsolute client. Role aliases never imply grants. */
export class Ros2Franka_spine_msgsActionMoveAbsoluteClient extends BoundedActionClient<Ros2Franka_spine_msgsActionMoveAbsolute_Goal,Ros2Franka_spine_msgsActionMoveAbsolute_Result,Ros2Franka_spine_msgsActionMoveAbsolute_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Franka_spine_msgsActionMoveAbsolute_GoalCodec(),new Ros2Franka_spine_msgsActionMoveAbsolute_ResultCodec(),new Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCodec(),(value:Ros2Franka_spine_msgsActionMoveAbsolute_Goal):bool=>value.validValue(),521,8,cancelAny,capacity);}}

/** Complete custom franka_spine_msgs/action/MoveAbsolute server with matching-ID response codecs. */
export class Ros2Franka_spine_msgsActionMoveAbsoluteServer extends BoundedActionServer<Ros2Franka_spine_msgsActionMoveAbsolute_Goal,Ros2Franka_spine_msgsActionMoveAbsolute_Result,Ros2Franka_spine_msgsActionMoveAbsolute_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsActionMoveAbsolute_GoalCodec(),new Ros2Franka_spine_msgsActionMoveAbsolute_ResultCodec(),new Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCodec(),(value:Ros2Franka_spine_msgsActionMoveAbsolute_Result):bool=>value.validValue(),(value:Ros2Franka_spine_msgsActionMoveAbsolute_Feedback):bool=>value.validValue(),32,capacity);}}

/** Complete franka_spine_msgs/srv/FaultReset typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvFaultResetClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvFaultReset_Request,Ros2Franka_spine_msgsSrvFaultReset_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvFaultReset_RequestCodec(),new Ros2Franka_spine_msgsSrvFaultReset_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvFaultReset_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/FaultReset incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvFaultResetServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvFaultReset_Request,Ros2Franka_spine_msgsSrvFaultReset_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvFaultReset_RequestCodec(),new Ros2Franka_spine_msgsSrvFaultReset_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvFaultReset_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/GetParameters typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvGetParametersClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvGetParameters_Request,Ros2Franka_spine_msgsSrvGetParameters_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvGetParameters_RequestCodec(),new Ros2Franka_spine_msgsSrvGetParameters_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetParameters_Request):bool=>value.validValue(),277,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/GetParameters incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvGetParametersServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvGetParameters_Request,Ros2Franka_spine_msgsSrvGetParameters_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvGetParameters_RequestCodec(),new Ros2Franka_spine_msgsSrvGetParameters_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetParameters_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/GetPosition typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvGetPositionClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvGetPosition_Request,Ros2Franka_spine_msgsSrvGetPosition_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvGetPosition_RequestCodec(),new Ros2Franka_spine_msgsSrvGetPosition_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetPosition_Request):bool=>value.validValue(),9,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/GetPosition incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvGetPositionServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvGetPosition_Request,Ros2Franka_spine_msgsSrvGetPosition_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvGetPosition_RequestCodec(),new Ros2Franka_spine_msgsSrvGetPosition_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetPosition_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/GetSpineState typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvGetSpineStateClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvGetSpineState_Request,Ros2Franka_spine_msgsSrvGetSpineState_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvGetSpineState_RequestCodec(),new Ros2Franka_spine_msgsSrvGetSpineState_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetSpineState_Request):bool=>value.validValue(),781,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/GetSpineState incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvGetSpineStateServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvGetSpineState_Request,Ros2Franka_spine_msgsSrvGetSpineState_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvGetSpineState_RequestCodec(),new Ros2Franka_spine_msgsSrvGetSpineState_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvGetSpineState_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/Halt typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvHaltClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvHalt_Request,Ros2Franka_spine_msgsSrvHalt_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvHalt_RequestCodec(),new Ros2Franka_spine_msgsSrvHalt_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvHalt_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/Halt incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvHaltServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvHalt_Request,Ros2Franka_spine_msgsSrvHalt_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvHalt_RequestCodec(),new Ros2Franka_spine_msgsSrvHalt_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvHalt_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/SwitchOff typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvSwitchOffClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvSwitchOff_Request,Ros2Franka_spine_msgsSrvSwitchOff_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvSwitchOff_RequestCodec(),new Ros2Franka_spine_msgsSrvSwitchOff_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvSwitchOff_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/SwitchOff incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvSwitchOffServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvSwitchOff_Request,Ros2Franka_spine_msgsSrvSwitchOff_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvSwitchOff_RequestCodec(),new Ros2Franka_spine_msgsSrvSwitchOff_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvSwitchOff_Response):bool=>value.validValue(),0,capacity);}}

/** Complete franka_spine_msgs/srv/SwitchOn typed call and real-submit client. */
export class Ros2Franka_spine_msgsSrvSwitchOnClient extends BoundedServiceClient<Ros2Franka_spine_msgsSrvSwitchOn_Request,Ros2Franka_spine_msgsSrvSwitchOn_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Franka_spine_msgsSrvSwitchOn_RequestCodec(),new Ros2Franka_spine_msgsSrvSwitchOn_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvSwitchOn_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete franka_spine_msgs/srv/SwitchOn incoming request/response owner. */
export class Ros2Franka_spine_msgsSrvSwitchOnServer extends BoundedServiceServer<Ros2Franka_spine_msgsSrvSwitchOn_Request,Ros2Franka_spine_msgsSrvSwitchOn_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Franka_spine_msgsSrvSwitchOn_RequestCodec(),new Ros2Franka_spine_msgsSrvSwitchOn_ResponseCodec(),(value:Ros2Franka_spine_msgsSrvSwitchOn_Response):bool=>value.validValue(),0,capacity);}}
