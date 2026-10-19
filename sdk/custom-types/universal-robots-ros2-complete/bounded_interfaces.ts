/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros2Ur_dashboard_msgsActionSetMode_Goal,Ros2Ur_dashboard_msgsActionSetMode_GoalCodec,Ros2Ur_dashboard_msgsActionSetMode_Result,Ros2Ur_dashboard_msgsActionSetMode_ResultCodec,Ros2Ur_dashboard_msgsActionSetMode_Feedback,Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec,Ros2Ur_dashboard_msgsSrvAddToLog_Request,Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec,Ros2Ur_dashboard_msgsSrvAddToLog_Response,Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec,Ros2Ur_dashboard_msgsSrvDownloadProgram_Request,Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec,Ros2Ur_dashboard_msgsSrvDownloadProgram_Response,Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec,Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response,Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetProgramState_Request,Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec,Ros2Ur_dashboard_msgsSrvGetProgramState_Response,Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetPrograms_Request,Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec,Ros2Ur_dashboard_msgsSrvGetPrograms_Response,Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetRobotMode_Request,Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec,Ros2Ur_dashboard_msgsSrvGetRobotMode_Response,Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetRobotModel_Request,Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec,Ros2Ur_dashboard_msgsSrvGetRobotModel_Response,Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request,Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec,Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response,Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request,Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec,Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response,Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec,Ros2Ur_dashboard_msgsSrvGetUserRole_Request,Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec,Ros2Ur_dashboard_msgsSrvGetUserRole_Response,Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec,Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request,Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec,Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response,Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec,Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request,Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec,Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response,Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec,Ros2Ur_dashboard_msgsSrvLoad_Request,Ros2Ur_dashboard_msgsSrvLoad_RequestCodec,Ros2Ur_dashboard_msgsSrvLoad_Response,Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec,Ros2Ur_dashboard_msgsSrvPopup_Request,Ros2Ur_dashboard_msgsSrvPopup_RequestCodec,Ros2Ur_dashboard_msgsSrvPopup_Response,Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec,Ros2Ur_dashboard_msgsSrvRawRequest_Request,Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec,Ros2Ur_dashboard_msgsSrvRawRequest_Response,Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec,Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec,Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response,Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec,Ros2Ur_dashboard_msgsSrvSetUserRole_Request,Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec,Ros2Ur_dashboard_msgsSrvSetUserRole_Response,Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec,Ros2Ur_dashboard_msgsSrvUploadProgram_Request,Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec,Ros2Ur_dashboard_msgsSrvUploadProgram_Response,Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal,Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result,Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback,Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec,Ros2Ur_msgsActionSendScript_Goal,Ros2Ur_msgsActionSendScript_GoalCodec,Ros2Ur_msgsActionSendScript_Result,Ros2Ur_msgsActionSendScript_ResultCodec,Ros2Ur_msgsActionSendScript_Feedback,Ros2Ur_msgsActionSendScript_FeedbackCodec,Ros2Ur_msgsActionToolContact_Goal,Ros2Ur_msgsActionToolContact_GoalCodec,Ros2Ur_msgsActionToolContact_Result,Ros2Ur_msgsActionToolContact_ResultCodec,Ros2Ur_msgsActionToolContact_Feedback,Ros2Ur_msgsActionToolContact_FeedbackCodec,Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request,Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec,Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response,Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec,Ros2Ur_msgsSrvSetAnalogOutput_Request,Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec,Ros2Ur_msgsSrvSetAnalogOutput_Response,Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec,Ros2Ur_msgsSrvSetForceMode_Request,Ros2Ur_msgsSrvSetForceMode_RequestCodec,Ros2Ur_msgsSrvSetForceMode_Response,Ros2Ur_msgsSrvSetForceMode_ResponseCodec,Ros2Ur_msgsSrvSetFrictionModelParameters_Request,Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec,Ros2Ur_msgsSrvSetFrictionModelParameters_Response,Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec,Ros2Ur_msgsSrvSetGravity_Request,Ros2Ur_msgsSrvSetGravity_RequestCodec,Ros2Ur_msgsSrvSetGravity_Response,Ros2Ur_msgsSrvSetGravity_ResponseCodec,Ros2Ur_msgsSrvSetIO_Request,Ros2Ur_msgsSrvSetIO_RequestCodec,Ros2Ur_msgsSrvSetIO_Response,Ros2Ur_msgsSrvSetIO_ResponseCodec,Ros2Ur_msgsSrvSetPayload_Request,Ros2Ur_msgsSrvSetPayload_RequestCodec,Ros2Ur_msgsSrvSetPayload_Response,Ros2Ur_msgsSrvSetPayload_ResponseCodec,Ros2Ur_msgsSrvSetSpeedSliderFraction_Request,Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec,Ros2Ur_msgsSrvSetSpeedSliderFraction_Response,Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec} from "./bounded_records";

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

/** Complete custom ur_dashboard_msgs/action/SetMode client. Role aliases never imply grants. */
export class Ros2Ur_dashboard_msgsActionSetModeClient extends BoundedActionClient<Ros2Ur_dashboard_msgsActionSetMode_Goal,Ros2Ur_dashboard_msgsActionSetMode_Result,Ros2Ur_dashboard_msgsActionSetMode_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Ur_dashboard_msgsActionSetMode_GoalCodec(),new Ros2Ur_dashboard_msgsActionSetMode_ResultCodec(),new Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec(),(value:Ros2Ur_dashboard_msgsActionSetMode_Goal):bool=>value.validValue(),261,2,cancelAny,capacity);}}

/** Complete custom ur_dashboard_msgs/action/SetMode server with matching-ID response codecs. */
export class Ros2Ur_dashboard_msgsActionSetModeServer extends BoundedActionServer<Ros2Ur_dashboard_msgsActionSetMode_Goal,Ros2Ur_dashboard_msgsActionSetMode_Result,Ros2Ur_dashboard_msgsActionSetMode_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsActionSetMode_GoalCodec(),new Ros2Ur_dashboard_msgsActionSetMode_ResultCodec(),new Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec(),(value:Ros2Ur_dashboard_msgsActionSetMode_Result):bool=>value.validValue(),(value:Ros2Ur_dashboard_msgsActionSetMode_Feedback):bool=>value.validValue(),3,capacity);}}

/** Complete ur_dashboard_msgs/srv/AddToLog typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvAddToLogClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvAddToLog_Request,Ros2Ur_dashboard_msgsSrvAddToLog_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec(),new Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvAddToLog_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/AddToLog incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvAddToLogServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvAddToLog_Request,Ros2Ur_dashboard_msgsSrvAddToLog_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec(),new Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvAddToLog_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadProgram typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgramClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request,Ros2Ur_dashboard_msgsSrvDownloadProgram_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadProgram incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgramServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request,Ros2Ur_dashboard_msgsSrvDownloadProgram_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Response):bool=>value.validValue(),520,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadSupportFile typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec(),new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadSupportFile incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFileServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request,Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec(),new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateFlightReport typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateFlightReport incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReportServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request,Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateSupportFile typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateSupportFile incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFileServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request,Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetLoadedProgram typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetLoadedProgram incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgramServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request,Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetOperationalMode typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalModeClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetOperationalMode incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalModeServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPolyScopeVersion typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request):bool=>value.validValue(),277,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPolyScopeVersion incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request,Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetProgramState typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetProgramStateClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetProgramState_Request,Ros2Ur_dashboard_msgsSrvGetProgramState_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Request):bool=>value.validValue(),781,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetProgramState incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetProgramStateServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetProgramState_Request,Ros2Ur_dashboard_msgsSrvGetProgramState_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPrograms typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetProgramsClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetPrograms_Request,Ros2Ur_dashboard_msgsSrvGetPrograms_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Request):bool=>value.validValue(),11081,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPrograms incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetProgramsServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetPrograms_Request,Ros2Ur_dashboard_msgsSrvGetPrograms_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotMode typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModeClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request,Ros2Ur_dashboard_msgsSrvGetRobotMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotMode incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModeServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request,Ros2Ur_dashboard_msgsSrvGetRobotMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotModel typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModelClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request,Ros2Ur_dashboard_msgsSrvGetRobotModel_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotModel incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModelServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request,Ros2Ur_dashboard_msgsSrvGetRobotModel_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyMode typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyModeClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request,Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyMode incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyModeServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request,Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyStatus typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyStatus incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatusServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request,Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSerialNumber typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumberClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request,Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request):bool=>value.validValue(),269,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSerialNumber incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumberServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request,Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetUserRole typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvGetUserRoleClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvGetUserRole_Request,Ros2Ur_dashboard_msgsSrvGetUserRole_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetUserRole incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvGetUserRoleServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvGetUserRole_Request,Ros2Ur_dashboard_msgsSrvGetUserRole_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec(),new Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsInRemoteControl typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsInRemoteControl incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControlServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request,Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramRunning typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunningClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request,Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request):bool=>value.validValue(),262,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramRunning incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunningServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request,Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramSaved typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSavedClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request,Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request):bool=>value.validValue(),522,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramSaved incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSavedServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request,Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec(),new Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_dashboard_msgs/srv/Load typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvLoadClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvLoad_Request,Ros2Ur_dashboard_msgsSrvLoad_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvLoad_RequestCodec(),new Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvLoad_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/Load incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvLoadServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvLoad_Request,Ros2Ur_dashboard_msgsSrvLoad_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvLoad_RequestCodec(),new Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvLoad_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/Popup typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvPopupClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvPopup_Request,Ros2Ur_dashboard_msgsSrvPopup_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvPopup_RequestCodec(),new Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvPopup_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/Popup incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvPopupServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvPopup_Request,Ros2Ur_dashboard_msgsSrvPopup_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvPopup_RequestCodec(),new Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvPopup_Response):bool=>value.validValue(),520,capacity);}}

/** Complete ur_dashboard_msgs/srv/RawRequest typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvRawRequestClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvRawRequest_Request,Ros2Ur_dashboard_msgsSrvRawRequest_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec(),new Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvRawRequest_Request):bool=>value.validValue(),260,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/RawRequest incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvRawRequestServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvRawRequest_Request,Ros2Ur_dashboard_msgsSrvRawRequest_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec(),new Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvRawRequest_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetOperationalMode typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalModeClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetOperationalMode incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalModeServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request,Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec(),new Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetUserRole typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvSetUserRoleClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvSetUserRole_Request,Ros2Ur_dashboard_msgsSrvSetUserRole_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec(),new Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetUserRole incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvSetUserRoleServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvSetUserRole_Request,Ros2Ur_dashboard_msgsSrvSetUserRole_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec(),new Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Response):bool=>value.validValue(),260,capacity);}}

/** Complete ur_dashboard_msgs/srv/UploadProgram typed call and real-submit client. */
export class Ros2Ur_dashboard_msgsSrvUploadProgramClient extends BoundedServiceClient<Ros2Ur_dashboard_msgsSrvUploadProgram_Request,Ros2Ur_dashboard_msgsSrvUploadProgram_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Request):bool=>value.validValue(),521,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/UploadProgram incoming request/response owner. */
export class Ros2Ur_dashboard_msgsSrvUploadProgramServer extends BoundedServiceServer<Ros2Ur_dashboard_msgsSrvUploadProgram_Request,Ros2Ur_dashboard_msgsSrvUploadProgram_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec(),new Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec(),(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Response):bool=>value.validValue(),260,capacity);}}

/** Complete custom ur_msgs/action/FollowJointTrajectoryUntil client. Role aliases never imply grants. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntilClient extends BoundedActionClient<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec(),new Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec(),new Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec(),(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal):bool=>value.validValue(),268,2352,cancelAny,capacity);}}

/** Complete custom ur_msgs/action/FollowJointTrajectoryUntil server with matching-ID response codecs. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntilServer extends BoundedActionServer<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result,Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec(),new Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec(),new Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec(),(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result):bool=>value.validValue(),(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback):bool=>value.validValue(),7053,capacity);}}

/** Complete custom ur_msgs/action/SendScript client. Role aliases never imply grants. */
export class Ros2Ur_msgsActionSendScriptClient extends BoundedActionClient<Ros2Ur_msgsActionSendScript_Goal,Ros2Ur_msgsActionSendScript_Result,Ros2Ur_msgsActionSendScript_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Ur_msgsActionSendScript_GoalCodec(),new Ros2Ur_msgsActionSendScript_ResultCodec(),new Ros2Ur_msgsActionSendScript_FeedbackCodec(),(value:Ros2Ur_msgsActionSendScript_Goal):bool=>value.validValue(),261,0,cancelAny,capacity);}}

/** Complete custom ur_msgs/action/SendScript server with matching-ID response codecs. */
export class Ros2Ur_msgsActionSendScriptServer extends BoundedActionServer<Ros2Ur_msgsActionSendScript_Goal,Ros2Ur_msgsActionSendScript_Result,Ros2Ur_msgsActionSendScript_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsActionSendScript_GoalCodec(),new Ros2Ur_msgsActionSendScript_ResultCodec(),new Ros2Ur_msgsActionSendScript_FeedbackCodec(),(value:Ros2Ur_msgsActionSendScript_Result):bool=>value.validValue(),(value:Ros2Ur_msgsActionSendScript_Feedback):bool=>value.validValue(),8465,capacity);}}

/** Complete custom ur_msgs/action/ToolContact client. Role aliases never imply grants. */
export class Ros2Ur_msgsActionToolContactClient extends BoundedActionClient<Ros2Ur_msgsActionToolContact_Goal,Ros2Ur_msgsActionToolContact_Result,Ros2Ur_msgsActionToolContact_Feedback> {constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){super(api,2,send,cancelOwn,feedback,result,new Ros2Ur_msgsActionToolContact_GoalCodec(),new Ros2Ur_msgsActionToolContact_ResultCodec(),new Ros2Ur_msgsActionToolContact_FeedbackCodec(),(value:Ros2Ur_msgsActionToolContact_Goal):bool=>value.validValue(),0,0,cancelAny,capacity);}}

/** Complete custom ur_msgs/action/ToolContact server with matching-ID response codecs. */
export class Ros2Ur_msgsActionToolContactServer extends BoundedActionServer<Ros2Ur_msgsActionToolContact_Goal,Ros2Ur_msgsActionToolContact_Result,Ros2Ur_msgsActionToolContact_Feedback> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsActionToolContact_GoalCodec(),new Ros2Ur_msgsActionToolContact_ResultCodec(),new Ros2Ur_msgsActionToolContact_FeedbackCodec(),(value:Ros2Ur_msgsActionToolContact_Result):bool=>value.validValue(),(value:Ros2Ur_msgsActionToolContact_Feedback):bool=>value.validValue(),0,capacity);}}

/** Complete ur_msgs/srv/GetRobotSoftwareVersion typed call and real-submit client. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersionClient extends BoundedServiceClient<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request,Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec(),new Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec(),(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete ur_msgs/srv/GetRobotSoftwareVersion incoming request/response owner. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersionServer extends BoundedServiceServer<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request,Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec(),new Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec(),(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ur_msgs/srv/SetAnalogOutput typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetAnalogOutputClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetAnalogOutput_Request,Ros2Ur_msgsSrvSetAnalogOutput_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec(),new Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec(),(value:Ros2Ur_msgsSrvSetAnalogOutput_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetAnalogOutput incoming request/response owner. */
export class Ros2Ur_msgsSrvSetAnalogOutputServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetAnalogOutput_Request,Ros2Ur_msgsSrvSetAnalogOutput_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec(),new Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec(),(value:Ros2Ur_msgsSrvSetAnalogOutput_Response):bool=>value.validValue(),6,capacity);}}

/** Complete ur_msgs/srv/SetForceMode typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetForceModeClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetForceMode_Request,Ros2Ur_msgsSrvSetForceMode_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetForceMode_RequestCodec(),new Ros2Ur_msgsSrvSetForceMode_ResponseCodec(),(value:Ros2Ur_msgsSrvSetForceMode_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetForceMode incoming request/response owner. */
export class Ros2Ur_msgsSrvSetForceModeServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetForceMode_Request,Ros2Ur_msgsSrvSetForceMode_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetForceMode_RequestCodec(),new Ros2Ur_msgsSrvSetForceMode_ResponseCodec(),(value:Ros2Ur_msgsSrvSetForceMode_Response):bool=>value.validValue(),331,capacity);}}

/** Complete ur_msgs/srv/SetFrictionModelParameters typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetFrictionModelParametersClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetFrictionModelParameters_Request,Ros2Ur_msgsSrvSetFrictionModelParameters_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec(),new Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec(),(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetFrictionModelParameters incoming request/response owner. */
export class Ros2Ur_msgsSrvSetFrictionModelParametersServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetFrictionModelParameters_Request,Ros2Ur_msgsSrvSetFrictionModelParameters_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec(),new Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec(),(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Response):bool=>value.validValue(),104,capacity);}}

/** Complete ur_msgs/srv/SetGravity typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetGravityClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetGravity_Request,Ros2Ur_msgsSrvSetGravity_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetGravity_RequestCodec(),new Ros2Ur_msgsSrvSetGravity_ResponseCodec(),(value:Ros2Ur_msgsSrvSetGravity_Request):bool=>value.validValue(),261,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetGravity incoming request/response owner. */
export class Ros2Ur_msgsSrvSetGravityServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetGravity_Request,Ros2Ur_msgsSrvSetGravity_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetGravity_RequestCodec(),new Ros2Ur_msgsSrvSetGravity_ResponseCodec(),(value:Ros2Ur_msgsSrvSetGravity_Response):bool=>value.validValue(),164,capacity);}}

/** Complete ur_msgs/srv/SetIO typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetIOClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetIO_Request,Ros2Ur_msgsSrvSetIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetIO_RequestCodec(),new Ros2Ur_msgsSrvSetIO_ResponseCodec(),(value:Ros2Ur_msgsSrvSetIO_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetIO incoming request/response owner. */
export class Ros2Ur_msgsSrvSetIOServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetIO_Request,Ros2Ur_msgsSrvSetIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetIO_RequestCodec(),new Ros2Ur_msgsSrvSetIO_ResponseCodec(),(value:Ros2Ur_msgsSrvSetIO_Response):bool=>value.validValue(),6,capacity);}}

/** Complete ur_msgs/srv/SetPayload typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetPayloadClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetPayload_Request,Ros2Ur_msgsSrvSetPayload_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetPayload_RequestCodec(),new Ros2Ur_msgsSrvSetPayload_ResponseCodec(),(value:Ros2Ur_msgsSrvSetPayload_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetPayload incoming request/response owner. */
export class Ros2Ur_msgsSrvSetPayloadServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetPayload_Request,Ros2Ur_msgsSrvSetPayload_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetPayload_RequestCodec(),new Ros2Ur_msgsSrvSetPayload_ResponseCodec(),(value:Ros2Ur_msgsSrvSetPayload_Response):bool=>value.validValue(),84,capacity);}}

/** Complete ur_msgs/srv/SetSpeedSliderFraction typed call and real-submit client. */
export class Ros2Ur_msgsSrvSetSpeedSliderFractionClient extends BoundedServiceClient<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request,Ros2Ur_msgsSrvSetSpeedSliderFraction_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec(),new Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec(),(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetSpeedSliderFraction incoming request/response owner. */
export class Ros2Ur_msgsSrvSetSpeedSliderFractionServer extends BoundedServiceServer<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request,Ros2Ur_msgsSrvSetSpeedSliderFraction_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec(),new Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec(),(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Response):bool=>value.validValue(),8,capacity);}}
