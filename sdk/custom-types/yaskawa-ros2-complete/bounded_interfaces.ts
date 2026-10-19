/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_GoalCodec,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_ResultCodec,Ros2Control_msgsActionFollowJointTrajectory_Feedback,Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec,Ros2Industrial_msgsSrvCmdJointTrajectory_Request,Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCodec,Ros2Industrial_msgsSrvCmdJointTrajectory_Response,Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCodec,Ros2Industrial_msgsSrvGetRobotInfo_Request,Ros2Industrial_msgsSrvGetRobotInfo_RequestCodec,Ros2Industrial_msgsSrvGetRobotInfo_Response,Ros2Industrial_msgsSrvGetRobotInfo_ResponseCodec,Ros2Industrial_msgsSrvSetDrivePower_Request,Ros2Industrial_msgsSrvSetDrivePower_RequestCodec,Ros2Industrial_msgsSrvSetDrivePower_Response,Ros2Industrial_msgsSrvSetDrivePower_ResponseCodec,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCodec,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCodec,Ros2Industrial_msgsSrvStartMotion_Request,Ros2Industrial_msgsSrvStartMotion_RequestCodec,Ros2Industrial_msgsSrvStartMotion_Response,Ros2Industrial_msgsSrvStartMotion_ResponseCodec,Ros2Industrial_msgsSrvStopMotion_Request,Ros2Industrial_msgsSrvStopMotion_RequestCodec,Ros2Industrial_msgsSrvStopMotion_Response,Ros2Industrial_msgsSrvStopMotion_ResponseCodec,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCodec,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCodec,Ros2Motoros2_interfacesSrvListInformJobs_Request,Ros2Motoros2_interfacesSrvListInformJobs_RequestCodec,Ros2Motoros2_interfacesSrvListInformJobs_Response,Ros2Motoros2_interfacesSrvListInformJobs_ResponseCodec,Ros2Motoros2_interfacesSrvQueueTrajPoint_Request,Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCodec,Ros2Motoros2_interfacesSrvQueueTrajPoint_Response,Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCodec,Ros2Motoros2_interfacesSrvReadGroupIO_Request,Ros2Motoros2_interfacesSrvReadGroupIO_RequestCodec,Ros2Motoros2_interfacesSrvReadGroupIO_Response,Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCodec,Ros2Motoros2_interfacesSrvReadMRegister_Request,Ros2Motoros2_interfacesSrvReadMRegister_RequestCodec,Ros2Motoros2_interfacesSrvReadMRegister_Response,Ros2Motoros2_interfacesSrvReadMRegister_ResponseCodec,Ros2Motoros2_interfacesSrvReadSingleIO_Request,Ros2Motoros2_interfacesSrvReadSingleIO_RequestCodec,Ros2Motoros2_interfacesSrvReadSingleIO_Response,Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCodec,Ros2Motoros2_interfacesSrvResetError_Request,Ros2Motoros2_interfacesSrvResetError_RequestCodec,Ros2Motoros2_interfacesSrvResetError_Response,Ros2Motoros2_interfacesSrvResetError_ResponseCodec,Ros2Motoros2_interfacesSrvSelectMotionTool_Request,Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCodec,Ros2Motoros2_interfacesSrvSelectMotionTool_Response,Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCodec,Ros2Motoros2_interfacesSrvStartPointQueueMode_Request,Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCodec,Ros2Motoros2_interfacesSrvStartPointQueueMode_Response,Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCodec,Ros2Motoros2_interfacesSrvStartTrajMode_Request,Ros2Motoros2_interfacesSrvStartTrajMode_RequestCodec,Ros2Motoros2_interfacesSrvStartTrajMode_Response,Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCodec,Ros2Motoros2_interfacesSrvWriteGroupIO_Request,Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCodec,Ros2Motoros2_interfacesSrvWriteGroupIO_Response,Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCodec,Ros2Motoros2_interfacesSrvWriteMRegister_Request,Ros2Motoros2_interfacesSrvWriteMRegister_RequestCodec,Ros2Motoros2_interfacesSrvWriteMRegister_Response,Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCodec,Ros2Motoros2_interfacesSrvWriteSingleIO_Request,Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCodec,Ros2Motoros2_interfacesSrvWriteSingleIO_Response,Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCodec,Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_RequestCodec,Ros2Std_srvsSrvTrigger_Response,Ros2Std_srvsSrvTrigger_ResponseCodec} from "./bounded_records";

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

/** Complete custom control_msgs/action/FollowJointTrajectory client. Role aliases never imply grants. */
export class Ros2Control_msgsActionFollowJointTrajectoryClient extends BoundedActionClient<Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Control_msgsActionFollowJointTrajectory_GoalCodec(),new Ros2Control_msgsActionFollowJointTrajectory_ResultCodec(),new Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec(),(value:Ros2Control_msgsActionFollowJointTrajectory_Goal):bool=>value.validValue(),4104,4128,cancelAny,capacity);}}

/** Complete custom control_msgs/action/FollowJointTrajectory server with matching-ID response codecs. */
export class Ros2Control_msgsActionFollowJointTrajectoryServer extends BoundedActionServer<Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Control_msgsActionFollowJointTrajectory_GoalCodec(),new Ros2Control_msgsActionFollowJointTrajectory_ResultCodec(),new Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec(),(value:Ros2Control_msgsActionFollowJointTrajectory_Result):bool=>value.validValue(),(value:Ros2Control_msgsActionFollowJointTrajectory_Feedback):bool=>value.validValue(),20920,capacity);}}

/** Complete industrial_msgs/srv/CmdJointTrajectory typed call and real-submit client. */
export class Ros2Industrial_msgsSrvCmdJointTrajectoryClient extends BoundedServiceClient<Ros2Industrial_msgsSrvCmdJointTrajectory_Request,Ros2Industrial_msgsSrvCmdJointTrajectory_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCodec(),new Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCodec(),(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/CmdJointTrajectory incoming request/response owner. */
export class Ros2Industrial_msgsSrvCmdJointTrajectoryServer extends BoundedServiceServer<Ros2Industrial_msgsSrvCmdJointTrajectory_Request,Ros2Industrial_msgsSrvCmdJointTrajectory_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvCmdJointTrajectory_RequestCodec(),new Ros2Industrial_msgsSrvCmdJointTrajectory_ResponseCodec(),(value:Ros2Industrial_msgsSrvCmdJointTrajectory_Response):bool=>value.validValue(),4164,capacity);}}

/** Complete industrial_msgs/srv/GetRobotInfo typed call and real-submit client. */
export class Ros2Industrial_msgsSrvGetRobotInfoClient extends BoundedServiceClient<Ros2Industrial_msgsSrvGetRobotInfo_Request,Ros2Industrial_msgsSrvGetRobotInfo_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvGetRobotInfo_RequestCodec(),new Ros2Industrial_msgsSrvGetRobotInfo_ResponseCodec(),(value:Ros2Industrial_msgsSrvGetRobotInfo_Request):bool=>value.validValue(),32768,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/GetRobotInfo incoming request/response owner. */
export class Ros2Industrial_msgsSrvGetRobotInfoServer extends BoundedServiceServer<Ros2Industrial_msgsSrvGetRobotInfo_Request,Ros2Industrial_msgsSrvGetRobotInfo_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvGetRobotInfo_RequestCodec(),new Ros2Industrial_msgsSrvGetRobotInfo_ResponseCodec(),(value:Ros2Industrial_msgsSrvGetRobotInfo_Response):bool=>value.validValue(),0,capacity);}}

/** Complete industrial_msgs/srv/SetDrivePower typed call and real-submit client. */
export class Ros2Industrial_msgsSrvSetDrivePowerClient extends BoundedServiceClient<Ros2Industrial_msgsSrvSetDrivePower_Request,Ros2Industrial_msgsSrvSetDrivePower_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvSetDrivePower_RequestCodec(),new Ros2Industrial_msgsSrvSetDrivePower_ResponseCodec(),(value:Ros2Industrial_msgsSrvSetDrivePower_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/SetDrivePower incoming request/response owner. */
export class Ros2Industrial_msgsSrvSetDrivePowerServer extends BoundedServiceServer<Ros2Industrial_msgsSrvSetDrivePower_Request,Ros2Industrial_msgsSrvSetDrivePower_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvSetDrivePower_RequestCodec(),new Ros2Industrial_msgsSrvSetDrivePower_ResponseCodec(),(value:Ros2Industrial_msgsSrvSetDrivePower_Response):bool=>value.validValue(),1,capacity);}}

/** Complete industrial_msgs/srv/SetRemoteLoggerLevel typed call and real-submit client. */
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient extends BoundedServiceClient<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCodec(),new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCodec(),(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/SetRemoteLoggerLevel incoming request/response owner. */
export class Ros2Industrial_msgsSrvSetRemoteLoggerLevelServer extends BoundedServiceServer<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request,Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_RequestCodec(),new Ros2Industrial_msgsSrvSetRemoteLoggerLevel_ResponseCodec(),(value:Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response):bool=>value.validValue(),1,capacity);}}

/** Complete industrial_msgs/srv/StartMotion typed call and real-submit client. */
export class Ros2Industrial_msgsSrvStartMotionClient extends BoundedServiceClient<Ros2Industrial_msgsSrvStartMotion_Request,Ros2Industrial_msgsSrvStartMotion_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvStartMotion_RequestCodec(),new Ros2Industrial_msgsSrvStartMotion_ResponseCodec(),(value:Ros2Industrial_msgsSrvStartMotion_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/StartMotion incoming request/response owner. */
export class Ros2Industrial_msgsSrvStartMotionServer extends BoundedServiceServer<Ros2Industrial_msgsSrvStartMotion_Request,Ros2Industrial_msgsSrvStartMotion_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvStartMotion_RequestCodec(),new Ros2Industrial_msgsSrvStartMotion_ResponseCodec(),(value:Ros2Industrial_msgsSrvStartMotion_Response):bool=>value.validValue(),0,capacity);}}

/** Complete industrial_msgs/srv/StopMotion typed call and real-submit client. */
export class Ros2Industrial_msgsSrvStopMotionClient extends BoundedServiceClient<Ros2Industrial_msgsSrvStopMotion_Request,Ros2Industrial_msgsSrvStopMotion_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Industrial_msgsSrvStopMotion_RequestCodec(),new Ros2Industrial_msgsSrvStopMotion_ResponseCodec(),(value:Ros2Industrial_msgsSrvStopMotion_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete industrial_msgs/srv/StopMotion incoming request/response owner. */
export class Ros2Industrial_msgsSrvStopMotionServer extends BoundedServiceServer<Ros2Industrial_msgsSrvStopMotion_Request,Ros2Industrial_msgsSrvStopMotion_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Industrial_msgsSrvStopMotion_RequestCodec(),new Ros2Industrial_msgsSrvStopMotion_ResponseCodec(),(value:Ros2Industrial_msgsSrvStopMotion_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/GetActiveAlarmInfo typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCodec(),new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request):bool=>value.validValue(),32768,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/GetActiveAlarmInfo incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvGetActiveAlarmInfoServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request,Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_RequestCodec(),new Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/ListInformJobs typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvListInformJobsClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvListInformJobs_Request,Ros2Motoros2_interfacesSrvListInformJobs_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvListInformJobs_RequestCodec(),new Ros2Motoros2_interfacesSrvListInformJobs_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvListInformJobs_Request):bool=>value.validValue(),8780,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/ListInformJobs incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvListInformJobsServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvListInformJobs_Request,Ros2Motoros2_interfacesSrvListInformJobs_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvListInformJobs_RequestCodec(),new Ros2Motoros2_interfacesSrvListInformJobs_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvListInformJobs_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/QueueTrajPoint typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvQueueTrajPointClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request,Ros2Motoros2_interfacesSrvQueueTrajPoint_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCodec(),new Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Request):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/QueueTrajPoint incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvQueueTrajPointServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvQueueTrajPoint_Request,Ros2Motoros2_interfacesSrvQueueTrajPoint_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvQueueTrajPoint_RequestCodec(),new Ros2Motoros2_interfacesSrvQueueTrajPoint_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvQueueTrajPoint_Response):bool=>value.validValue(),1228,capacity);}}

/** Complete motoros2_interfaces/srv/ReadGroupIO typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvReadGroupIOClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvReadGroupIO_Request,Ros2Motoros2_interfacesSrvReadGroupIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvReadGroupIO_RequestCodec(),new Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadGroupIO_Request):bool=>value.validValue(),522,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/ReadGroupIO incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvReadGroupIOServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvReadGroupIO_Request,Ros2Motoros2_interfacesSrvReadGroupIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvReadGroupIO_RequestCodec(),new Ros2Motoros2_interfacesSrvReadGroupIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadGroupIO_Response):bool=>value.validValue(),4,capacity);}}

/** Complete motoros2_interfaces/srv/ReadMRegister typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvReadMRegisterClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvReadMRegister_Request,Ros2Motoros2_interfacesSrvReadMRegister_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvReadMRegister_RequestCodec(),new Ros2Motoros2_interfacesSrvReadMRegister_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadMRegister_Request):bool=>value.validValue(),523,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/ReadMRegister incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvReadMRegisterServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvReadMRegister_Request,Ros2Motoros2_interfacesSrvReadMRegister_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvReadMRegister_RequestCodec(),new Ros2Motoros2_interfacesSrvReadMRegister_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadMRegister_Response):bool=>value.validValue(),4,capacity);}}

/** Complete motoros2_interfaces/srv/ReadSingleIO typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvReadSingleIOClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvReadSingleIO_Request,Ros2Motoros2_interfacesSrvReadSingleIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvReadSingleIO_RequestCodec(),new Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadSingleIO_Request):bool=>value.validValue(),525,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/ReadSingleIO incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvReadSingleIOServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvReadSingleIO_Request,Ros2Motoros2_interfacesSrvReadSingleIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvReadSingleIO_RequestCodec(),new Ros2Motoros2_interfacesSrvReadSingleIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvReadSingleIO_Response):bool=>value.validValue(),4,capacity);}}

/** Complete motoros2_interfaces/srv/ResetError typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvResetErrorClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvResetError_Request,Ros2Motoros2_interfacesSrvResetError_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvResetError_RequestCodec(),new Ros2Motoros2_interfacesSrvResetError_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvResetError_Request):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/ResetError incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvResetErrorServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvResetError_Request,Ros2Motoros2_interfacesSrvResetError_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvResetError_RequestCodec(),new Ros2Motoros2_interfacesSrvResetError_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvResetError_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/SelectMotionTool typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvSelectMotionToolClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvSelectMotionTool_Request,Ros2Motoros2_interfacesSrvSelectMotionTool_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCodec(),new Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/SelectMotionTool incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvSelectMotionToolServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvSelectMotionTool_Request,Ros2Motoros2_interfacesSrvSelectMotionTool_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvSelectMotionTool_RequestCodec(),new Ros2Motoros2_interfacesSrvSelectMotionTool_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvSelectMotionTool_Response):bool=>value.validValue(),8,capacity);}}

/** Complete motoros2_interfaces/srv/StartPointQueueMode typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvStartPointQueueModeClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request,Ros2Motoros2_interfacesSrvStartPointQueueMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCodec(),new Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Request):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/StartPointQueueMode incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvStartPointQueueModeServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvStartPointQueueMode_Request,Ros2Motoros2_interfacesSrvStartPointQueueMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvStartPointQueueMode_RequestCodec(),new Ros2Motoros2_interfacesSrvStartPointQueueMode_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvStartPointQueueMode_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/StartTrajMode typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvStartTrajModeClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvStartTrajMode_Request,Ros2Motoros2_interfacesSrvStartTrajMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvStartTrajMode_RequestCodec(),new Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvStartTrajMode_Request):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/StartTrajMode incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvStartTrajModeServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvStartTrajMode_Request,Ros2Motoros2_interfacesSrvStartTrajMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvStartTrajMode_RequestCodec(),new Ros2Motoros2_interfacesSrvStartTrajMode_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvStartTrajMode_Response):bool=>value.validValue(),0,capacity);}}

/** Complete motoros2_interfaces/srv/WriteGroupIO typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvWriteGroupIOClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvWriteGroupIO_Request,Ros2Motoros2_interfacesSrvWriteGroupIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/WriteGroupIO incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvWriteGroupIOServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvWriteGroupIO_Request,Ros2Motoros2_interfacesSrvWriteGroupIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvWriteGroupIO_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteGroupIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteGroupIO_Response):bool=>value.validValue(),5,capacity);}}

/** Complete motoros2_interfaces/srv/WriteMRegister typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvWriteMRegisterClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvWriteMRegister_Request,Ros2Motoros2_interfacesSrvWriteMRegister_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvWriteMRegister_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteMRegister_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/WriteMRegister incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvWriteMRegisterServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvWriteMRegister_Request,Ros2Motoros2_interfacesSrvWriteMRegister_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvWriteMRegister_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteMRegister_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteMRegister_Response):bool=>value.validValue(),6,capacity);}}

/** Complete motoros2_interfaces/srv/WriteSingleIO typed call and real-submit client. */
export class Ros2Motoros2_interfacesSrvWriteSingleIOClient extends BoundedServiceClient<Ros2Motoros2_interfacesSrvWriteSingleIO_Request,Ros2Motoros2_interfacesSrvWriteSingleIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete motoros2_interfaces/srv/WriteSingleIO incoming request/response owner. */
export class Ros2Motoros2_interfacesSrvWriteSingleIOServer extends BoundedServiceServer<Ros2Motoros2_interfacesSrvWriteSingleIO_Request,Ros2Motoros2_interfacesSrvWriteSingleIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Motoros2_interfacesSrvWriteSingleIO_RequestCodec(),new Ros2Motoros2_interfacesSrvWriteSingleIO_ResponseCodec(),(value:Ros2Motoros2_interfacesSrvWriteSingleIO_Response):bool=>value.validValue(),8,capacity);}}

/** Complete std_srvs/srv/Trigger typed call and real-submit client. */
export class Ros2Std_srvsSrvTriggerClient extends BoundedServiceClient<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Std_srvsSrvTrigger_RequestCodec(),new Ros2Std_srvsSrvTrigger_ResponseCodec(),(value:Ros2Std_srvsSrvTrigger_Request):bool=>value.validValue(),517,observerAlias,capacity);}}

/** Complete std_srvs/srv/Trigger incoming request/response owner. */
export class Ros2Std_srvsSrvTriggerServer extends BoundedServiceServer<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Std_srvsSrvTrigger_RequestCodec(),new Ros2Std_srvsSrvTrigger_ResponseCodec(),(value:Ros2Std_srvsSrvTrigger_Response):bool=>value.validValue(),0,capacity);}}
