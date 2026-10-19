/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCodec,Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse,Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCodec,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCodec,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCodec,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCodec,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCodec,Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest,Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCodec,Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse,Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCodec,Ros1Abb_robot_msgsGetFileContentsRequest,Ros1Abb_robot_msgsGetFileContentsRequestCodec,Ros1Abb_robot_msgsGetFileContentsResponse,Ros1Abb_robot_msgsGetFileContentsResponseCodec,Ros1Abb_robot_msgsGetIOSignalRequest,Ros1Abb_robot_msgsGetIOSignalRequestCodec,Ros1Abb_robot_msgsGetIOSignalResponse,Ros1Abb_robot_msgsGetIOSignalResponseCodec,Ros1Abb_robot_msgsGetRAPIDBoolRequest,Ros1Abb_robot_msgsGetRAPIDBoolRequestCodec,Ros1Abb_robot_msgsGetRAPIDBoolResponse,Ros1Abb_robot_msgsGetRAPIDBoolResponseCodec,Ros1Abb_robot_msgsGetRAPIDDnumRequest,Ros1Abb_robot_msgsGetRAPIDDnumRequestCodec,Ros1Abb_robot_msgsGetRAPIDDnumResponse,Ros1Abb_robot_msgsGetRAPIDDnumResponseCodec,Ros1Abb_robot_msgsGetRAPIDNumRequest,Ros1Abb_robot_msgsGetRAPIDNumRequestCodec,Ros1Abb_robot_msgsGetRAPIDNumResponse,Ros1Abb_robot_msgsGetRAPIDNumResponseCodec,Ros1Abb_robot_msgsGetRAPIDStringRequest,Ros1Abb_robot_msgsGetRAPIDStringRequestCodec,Ros1Abb_robot_msgsGetRAPIDStringResponse,Ros1Abb_robot_msgsGetRAPIDStringResponseCodec,Ros1Abb_robot_msgsGetRAPIDSymbolRequest,Ros1Abb_robot_msgsGetRAPIDSymbolRequestCodec,Ros1Abb_robot_msgsGetRAPIDSymbolResponse,Ros1Abb_robot_msgsGetRAPIDSymbolResponseCodec,Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest,Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCodec,Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse,Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCodec,Ros1Abb_robot_msgsGetSpeedRatioRequest,Ros1Abb_robot_msgsGetSpeedRatioRequestCodec,Ros1Abb_robot_msgsGetSpeedRatioResponse,Ros1Abb_robot_msgsGetSpeedRatioResponseCodec,Ros1Abb_robot_msgsSetFileContentsRequest,Ros1Abb_robot_msgsSetFileContentsRequestCodec,Ros1Abb_robot_msgsSetFileContentsResponse,Ros1Abb_robot_msgsSetFileContentsResponseCodec,Ros1Abb_robot_msgsSetIOSignalRequest,Ros1Abb_robot_msgsSetIOSignalRequestCodec,Ros1Abb_robot_msgsSetIOSignalResponse,Ros1Abb_robot_msgsSetIOSignalResponseCodec,Ros1Abb_robot_msgsSetRAPIDBoolRequest,Ros1Abb_robot_msgsSetRAPIDBoolRequestCodec,Ros1Abb_robot_msgsSetRAPIDBoolResponse,Ros1Abb_robot_msgsSetRAPIDBoolResponseCodec,Ros1Abb_robot_msgsSetRAPIDDnumRequest,Ros1Abb_robot_msgsSetRAPIDDnumRequestCodec,Ros1Abb_robot_msgsSetRAPIDDnumResponse,Ros1Abb_robot_msgsSetRAPIDDnumResponseCodec,Ros1Abb_robot_msgsSetRAPIDNumRequest,Ros1Abb_robot_msgsSetRAPIDNumRequestCodec,Ros1Abb_robot_msgsSetRAPIDNumResponse,Ros1Abb_robot_msgsSetRAPIDNumResponseCodec,Ros1Abb_robot_msgsSetRAPIDStringRequest,Ros1Abb_robot_msgsSetRAPIDStringRequestCodec,Ros1Abb_robot_msgsSetRAPIDStringResponse,Ros1Abb_robot_msgsSetRAPIDStringResponseCodec,Ros1Abb_robot_msgsSetRAPIDSymbolRequest,Ros1Abb_robot_msgsSetRAPIDSymbolRequestCodec,Ros1Abb_robot_msgsSetRAPIDSymbolResponse,Ros1Abb_robot_msgsSetRAPIDSymbolResponseCodec,Ros1Abb_robot_msgsSetSpeedRatioRequest,Ros1Abb_robot_msgsSetSpeedRatioRequestCodec,Ros1Abb_robot_msgsSetSpeedRatioResponse,Ros1Abb_robot_msgsSetSpeedRatioResponseCodec,Ros1Abb_robot_msgsTriggerWithResultCodeRequest,Ros1Abb_robot_msgsTriggerWithResultCodeRequestCodec,Ros1Abb_robot_msgsTriggerWithResultCodeResponse,Ros1Abb_robot_msgsTriggerWithResultCodeResponseCodec,Ros1Control_msgsFollowJointTrajectoryGoal,Ros1Control_msgsFollowJointTrajectoryGoalCodec,Ros1Control_msgsFollowJointTrajectoryResult,Ros1Control_msgsFollowJointTrajectoryResultCodec,Ros1Control_msgsFollowJointTrajectoryFeedback,Ros1Control_msgsFollowJointTrajectoryFeedbackCodec} from "./bounded_records";

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

/** Complete abb_rapid_sm_addin_msgs/GetEGMSettings typed call and real-submit client. */
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient extends BoundedServiceClient<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest):bool=>value.validValue(),1285,observerAlias,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/GetEGMSettings incoming request/response owner. */
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsServer extends BoundedServiceServer<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse):bool=>value.validValue(),68,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetEGMSettings typed call and real-submit client. */
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient extends BoundedServiceClient<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetEGMSettings incoming request/response owner. */
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsServer extends BoundedServiceServer<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest,Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse):bool=>value.validValue(),835,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetRAPIDRoutine typed call and real-submit client. */
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient extends BoundedServiceClient<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetRAPIDRoutine incoming request/response owner. */
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineServer extends BoundedServiceServer<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest,Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse):bool=>value.validValue(),584,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetSGCommand typed call and real-submit client. */
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient extends BoundedServiceClient<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest,Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_rapid_sm_addin_msgs/SetSGCommand incoming request/response owner. */
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandServer extends BoundedServiceServer<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest,Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCodec(),new Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCodec(),(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse):bool=>value.validValue(),73,capacity);}}

/** Complete abb_robot_msgs/GetFileContents typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetFileContentsClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetFileContentsRequest,Ros1Abb_robot_msgsGetFileContentsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetFileContentsRequestCodec(),new Ros1Abb_robot_msgsGetFileContentsResponseCodec(),(value:Ros1Abb_robot_msgsGetFileContentsRequest):bool=>value.validValue(),4618,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetFileContents incoming request/response owner. */
export class Ros1Abb_robot_msgsGetFileContentsServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetFileContentsRequest,Ros1Abb_robot_msgsGetFileContentsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetFileContentsRequestCodec(),new Ros1Abb_robot_msgsGetFileContentsResponseCodec(),(value:Ros1Abb_robot_msgsGetFileContentsResponse):bool=>value.validValue(),516,capacity);}}

/** Complete abb_robot_msgs/GetIOSignal typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetIOSignalClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetIOSignalRequest,Ros1Abb_robot_msgsGetIOSignalResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetIOSignalRequestCodec(),new Ros1Abb_robot_msgsGetIOSignalResponseCodec(),(value:Ros1Abb_robot_msgsGetIOSignalRequest):bool=>value.validValue(),1034,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetIOSignal incoming request/response owner. */
export class Ros1Abb_robot_msgsGetIOSignalServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetIOSignalRequest,Ros1Abb_robot_msgsGetIOSignalResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetIOSignalRequestCodec(),new Ros1Abb_robot_msgsGetIOSignalResponseCodec(),(value:Ros1Abb_robot_msgsGetIOSignalResponse):bool=>value.validValue(),516,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDBool typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRAPIDBoolClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRAPIDBoolRequest,Ros1Abb_robot_msgsGetRAPIDBoolResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRAPIDBoolRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDBoolResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDBoolRequest):bool=>value.validValue(),519,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDBool incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRAPIDBoolServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRAPIDBoolRequest,Ros1Abb_robot_msgsGetRAPIDBoolResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRAPIDBoolRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDBoolResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDBoolResponse):bool=>value.validValue(),204,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDDnum typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRAPIDDnumClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRAPIDDnumRequest,Ros1Abb_robot_msgsGetRAPIDDnumResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRAPIDDnumRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDDnumResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDDnumRequest):bool=>value.validValue(),526,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDDnum incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRAPIDDnumServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRAPIDDnumRequest,Ros1Abb_robot_msgsGetRAPIDDnumResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRAPIDDnumRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDDnumResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDDnumResponse):bool=>value.validValue(),204,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDNum typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRAPIDNumClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRAPIDNumRequest,Ros1Abb_robot_msgsGetRAPIDNumResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRAPIDNumRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDNumResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDNumRequest):bool=>value.validValue(),522,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDNum incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRAPIDNumServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRAPIDNumRequest,Ros1Abb_robot_msgsGetRAPIDNumResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRAPIDNumRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDNumResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDNumResponse):bool=>value.validValue(),204,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDString typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRAPIDStringClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRAPIDStringRequest,Ros1Abb_robot_msgsGetRAPIDStringResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRAPIDStringRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDStringResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDStringRequest):bool=>value.validValue(),1034,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDString incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRAPIDStringServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRAPIDStringRequest,Ros1Abb_robot_msgsGetRAPIDStringResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRAPIDStringRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDStringResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDStringResponse):bool=>value.validValue(),204,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDSymbol typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRAPIDSymbolClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRAPIDSymbolRequest,Ros1Abb_robot_msgsGetRAPIDSymbolResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRAPIDSymbolRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDSymbolResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDSymbolRequest):bool=>value.validValue(),1034,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRAPIDSymbol incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRAPIDSymbolServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRAPIDSymbolRequest,Ros1Abb_robot_msgsGetRAPIDSymbolResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRAPIDSymbolRequestCodec(),new Ros1Abb_robot_msgsGetRAPIDSymbolResponseCodec(),(value:Ros1Abb_robot_msgsGetRAPIDSymbolResponse):bool=>value.validValue(),204,capacity);}}

/** Complete abb_robot_msgs/GetRobotControllerDescription typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest,Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCodec(),new Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCodec(),(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest):bool=>value.validValue(),4618,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetRobotControllerDescription incoming request/response owner. */
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest,Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCodec(),new Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCodec(),(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse):bool=>value.validValue(),0,capacity);}}

/** Complete abb_robot_msgs/GetSpeedRatio typed call and real-submit client. */
export class Ros1Abb_robot_msgsGetSpeedRatioClient extends BoundedServiceClient<Ros1Abb_robot_msgsGetSpeedRatioRequest,Ros1Abb_robot_msgsGetSpeedRatioResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsGetSpeedRatioRequestCodec(),new Ros1Abb_robot_msgsGetSpeedRatioResponseCodec(),(value:Ros1Abb_robot_msgsGetSpeedRatioRequest):bool=>value.validValue(),519,observerAlias,capacity);}}

/** Complete abb_robot_msgs/GetSpeedRatio incoming request/response owner. */
export class Ros1Abb_robot_msgsGetSpeedRatioServer extends BoundedServiceServer<Ros1Abb_robot_msgsGetSpeedRatioRequest,Ros1Abb_robot_msgsGetSpeedRatioResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsGetSpeedRatioRequestCodec(),new Ros1Abb_robot_msgsGetSpeedRatioResponseCodec(),(value:Ros1Abb_robot_msgsGetSpeedRatioResponse):bool=>value.validValue(),0,capacity);}}

/** Complete abb_robot_msgs/SetFileContents typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetFileContentsClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetFileContentsRequest,Ros1Abb_robot_msgsSetFileContentsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetFileContentsRequestCodec(),new Ros1Abb_robot_msgsSetFileContentsResponseCodec(),(value:Ros1Abb_robot_msgsSetFileContentsRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetFileContents incoming request/response owner. */
export class Ros1Abb_robot_msgsSetFileContentsServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetFileContentsRequest,Ros1Abb_robot_msgsSetFileContentsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetFileContentsRequestCodec(),new Ros1Abb_robot_msgsSetFileContentsResponseCodec(),(value:Ros1Abb_robot_msgsSetFileContentsResponse):bool=>value.validValue(),4616,capacity);}}

/** Complete abb_robot_msgs/SetIOSignal typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetIOSignalClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetIOSignalRequest,Ros1Abb_robot_msgsSetIOSignalResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetIOSignalRequestCodec(),new Ros1Abb_robot_msgsSetIOSignalResponseCodec(),(value:Ros1Abb_robot_msgsSetIOSignalRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetIOSignal incoming request/response owner. */
export class Ros1Abb_robot_msgsSetIOSignalServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetIOSignalRequest,Ros1Abb_robot_msgsSetIOSignalResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetIOSignalRequestCodec(),new Ros1Abb_robot_msgsSetIOSignalResponseCodec(),(value:Ros1Abb_robot_msgsSetIOSignalResponse):bool=>value.validValue(),1032,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDBool typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetRAPIDBoolClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetRAPIDBoolRequest,Ros1Abb_robot_msgsSetRAPIDBoolResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetRAPIDBoolRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDBoolResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDBoolRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDBool incoming request/response owner. */
export class Ros1Abb_robot_msgsSetRAPIDBoolServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetRAPIDBoolRequest,Ros1Abb_robot_msgsSetRAPIDBoolResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetRAPIDBoolRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDBoolResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDBoolResponse):bool=>value.validValue(),205,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDDnum typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetRAPIDDnumClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetRAPIDDnumRequest,Ros1Abb_robot_msgsSetRAPIDDnumResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetRAPIDDnumRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDDnumResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDDnumRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDDnum incoming request/response owner. */
export class Ros1Abb_robot_msgsSetRAPIDDnumServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetRAPIDDnumRequest,Ros1Abb_robot_msgsSetRAPIDDnumResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetRAPIDDnumRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDDnumResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDDnumResponse):bool=>value.validValue(),212,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDNum typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetRAPIDNumClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetRAPIDNumRequest,Ros1Abb_robot_msgsSetRAPIDNumResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetRAPIDNumRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDNumResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDNumRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDNum incoming request/response owner. */
export class Ros1Abb_robot_msgsSetRAPIDNumServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetRAPIDNumRequest,Ros1Abb_robot_msgsSetRAPIDNumResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetRAPIDNumRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDNumResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDNumResponse):bool=>value.validValue(),208,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDString typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetRAPIDStringClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetRAPIDStringRequest,Ros1Abb_robot_msgsSetRAPIDStringResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetRAPIDStringRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDStringResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDStringRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDString incoming request/response owner. */
export class Ros1Abb_robot_msgsSetRAPIDStringServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetRAPIDStringRequest,Ros1Abb_robot_msgsSetRAPIDStringResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetRAPIDStringRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDStringResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDStringResponse):bool=>value.validValue(),720,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDSymbol typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetRAPIDSymbolClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetRAPIDSymbolRequest,Ros1Abb_robot_msgsSetRAPIDSymbolResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetRAPIDSymbolRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDSymbolResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDSymbolRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetRAPIDSymbol incoming request/response owner. */
export class Ros1Abb_robot_msgsSetRAPIDSymbolServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetRAPIDSymbolRequest,Ros1Abb_robot_msgsSetRAPIDSymbolResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetRAPIDSymbolRequestCodec(),new Ros1Abb_robot_msgsSetRAPIDSymbolResponseCodec(),(value:Ros1Abb_robot_msgsSetRAPIDSymbolResponse):bool=>value.validValue(),720,capacity);}}

/** Complete abb_robot_msgs/SetSpeedRatio typed call and real-submit client. */
export class Ros1Abb_robot_msgsSetSpeedRatioClient extends BoundedServiceClient<Ros1Abb_robot_msgsSetSpeedRatioRequest,Ros1Abb_robot_msgsSetSpeedRatioResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsSetSpeedRatioRequestCodec(),new Ros1Abb_robot_msgsSetSpeedRatioResponseCodec(),(value:Ros1Abb_robot_msgsSetSpeedRatioRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/SetSpeedRatio incoming request/response owner. */
export class Ros1Abb_robot_msgsSetSpeedRatioServer extends BoundedServiceServer<Ros1Abb_robot_msgsSetSpeedRatioRequest,Ros1Abb_robot_msgsSetSpeedRatioResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsSetSpeedRatioRequestCodec(),new Ros1Abb_robot_msgsSetSpeedRatioResponseCodec(),(value:Ros1Abb_robot_msgsSetSpeedRatioResponse):bool=>value.validValue(),1,capacity);}}

/** Complete abb_robot_msgs/TriggerWithResultCode typed call and real-submit client. */
export class Ros1Abb_robot_msgsTriggerWithResultCodeClient extends BoundedServiceClient<Ros1Abb_robot_msgsTriggerWithResultCodeRequest,Ros1Abb_robot_msgsTriggerWithResultCodeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Abb_robot_msgsTriggerWithResultCodeRequestCodec(),new Ros1Abb_robot_msgsTriggerWithResultCodeResponseCodec(),(value:Ros1Abb_robot_msgsTriggerWithResultCodeRequest):bool=>value.validValue(),518,observerAlias,capacity);}}

/** Complete abb_robot_msgs/TriggerWithResultCode incoming request/response owner. */
export class Ros1Abb_robot_msgsTriggerWithResultCodeServer extends BoundedServiceServer<Ros1Abb_robot_msgsTriggerWithResultCodeRequest,Ros1Abb_robot_msgsTriggerWithResultCodeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Abb_robot_msgsTriggerWithResultCodeRequestCodec(),new Ros1Abb_robot_msgsTriggerWithResultCodeResponseCodec(),(value:Ros1Abb_robot_msgsTriggerWithResultCodeResponse):bool=>value.validValue(),0,capacity);}}

/** Complete custom control_msgs/FollowJointTrajectory client. Role aliases never imply grants. */
export class Ros1Control_msgsFollowJointTrajectoryClient extends BoundedActionClient<Ros1Control_msgsFollowJointTrajectoryGoal,Ros1Control_msgsFollowJointTrajectoryResult,Ros1Control_msgsFollowJointTrajectoryFeedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,1,send,cancelOwn,feedback,result,new Ros1Control_msgsFollowJointTrajectoryGoalCodec(),new Ros1Control_msgsFollowJointTrajectoryResultCodec(),new Ros1Control_msgsFollowJointTrajectoryFeedbackCodec(),(value:Ros1Control_msgsFollowJointTrajectoryGoal):bool=>value.validValue(),4104,2124,cancelAny,capacity);}}

/** Complete custom control_msgs/FollowJointTrajectory server with matching-ID response codecs. */
export class Ros1Control_msgsFollowJointTrajectoryServer extends BoundedActionServer<Ros1Control_msgsFollowJointTrajectoryGoal,Ros1Control_msgsFollowJointTrajectoryResult,Ros1Control_msgsFollowJointTrajectoryFeedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Control_msgsFollowJointTrajectoryGoalCodec(),new Ros1Control_msgsFollowJointTrajectoryResultCodec(),new Ros1Control_msgsFollowJointTrajectoryFeedbackCodec(),(value:Ros1Control_msgsFollowJointTrajectoryResult):bool=>value.validValue(),(value:Ros1Control_msgsFollowJointTrajectoryFeedback):bool=>value.validValue(),6392,capacity);}}
