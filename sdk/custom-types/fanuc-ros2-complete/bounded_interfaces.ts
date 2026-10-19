/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros2Control_msgsActionFollowJointTrajectory_Goal,Ros2Control_msgsActionFollowJointTrajectory_GoalCodec,Ros2Control_msgsActionFollowJointTrajectory_Result,Ros2Control_msgsActionFollowJointTrajectory_ResultCodec,Ros2Control_msgsActionFollowJointTrajectory_Feedback,Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec,Ros2Fanuc_msgsSrvCfgForceSensor_Request,Ros2Fanuc_msgsSrvCfgForceSensor_RequestCodec,Ros2Fanuc_msgsSrvCfgForceSensor_Response,Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCodec,Ros2Fanuc_msgsSrvGetAnalogIO_Request,Ros2Fanuc_msgsSrvGetAnalogIO_RequestCodec,Ros2Fanuc_msgsSrvGetAnalogIO_Response,Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCodec,Ros2Fanuc_msgsSrvGetBoolIO_Request,Ros2Fanuc_msgsSrvGetBoolIO_RequestCodec,Ros2Fanuc_msgsSrvGetBoolIO_Response,Ros2Fanuc_msgsSrvGetBoolIO_ResponseCodec,Ros2Fanuc_msgsSrvGetCartesianPosition_Request,Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCodec,Ros2Fanuc_msgsSrvGetCartesianPosition_Response,Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCodec,Ros2Fanuc_msgsSrvGetGroupIO_Request,Ros2Fanuc_msgsSrvGetGroupIO_RequestCodec,Ros2Fanuc_msgsSrvGetGroupIO_Response,Ros2Fanuc_msgsSrvGetGroupIO_ResponseCodec,Ros2Fanuc_msgsSrvGetNumReg_Request,Ros2Fanuc_msgsSrvGetNumReg_RequestCodec,Ros2Fanuc_msgsSrvGetNumReg_Response,Ros2Fanuc_msgsSrvGetNumReg_ResponseCodec,Ros2Fanuc_msgsSrvGetPayloadComp_Request,Ros2Fanuc_msgsSrvGetPayloadComp_RequestCodec,Ros2Fanuc_msgsSrvGetPayloadComp_Response,Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCodec,Ros2Fanuc_msgsSrvGetPayloadID_Request,Ros2Fanuc_msgsSrvGetPayloadID_RequestCodec,Ros2Fanuc_msgsSrvGetPayloadID_Response,Ros2Fanuc_msgsSrvGetPayloadID_ResponseCodec,Ros2Fanuc_msgsSrvGetPayloadValue_Request,Ros2Fanuc_msgsSrvGetPayloadValue_RequestCodec,Ros2Fanuc_msgsSrvGetPayloadValue_Response,Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCodec,Ros2Fanuc_msgsSrvGetPosReg_Request,Ros2Fanuc_msgsSrvGetPosReg_RequestCodec,Ros2Fanuc_msgsSrvGetPosReg_Response,Ros2Fanuc_msgsSrvGetPosReg_ResponseCodec,Ros2Fanuc_msgsSrvGetTCPSpeed_Request,Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCodec,Ros2Fanuc_msgsSrvGetTCPSpeed_Response,Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCodec,Ros2Fanuc_msgsSrvGetUFrameData_Request,Ros2Fanuc_msgsSrvGetUFrameData_RequestCodec,Ros2Fanuc_msgsSrvGetUFrameData_Response,Ros2Fanuc_msgsSrvGetUFrameData_ResponseCodec,Ros2Fanuc_msgsSrvGetUFrameUTool_Request,Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCodec,Ros2Fanuc_msgsSrvGetUFrameUTool_Response,Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCodec,Ros2Fanuc_msgsSrvGetUToolData_Request,Ros2Fanuc_msgsSrvGetUToolData_RequestCodec,Ros2Fanuc_msgsSrvGetUToolData_Response,Ros2Fanuc_msgsSrvGetUToolData_ResponseCodec,Ros2Fanuc_msgsSrvReadError_Request,Ros2Fanuc_msgsSrvReadError_RequestCodec,Ros2Fanuc_msgsSrvReadError_Response,Ros2Fanuc_msgsSrvReadError_ResponseCodec,Ros2Fanuc_msgsSrvReset_Request,Ros2Fanuc_msgsSrvReset_RequestCodec,Ros2Fanuc_msgsSrvReset_Response,Ros2Fanuc_msgsSrvReset_ResponseCodec,Ros2Fanuc_msgsSrvSetAnalogIO_Request,Ros2Fanuc_msgsSrvSetAnalogIO_RequestCodec,Ros2Fanuc_msgsSrvSetAnalogIO_Response,Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCodec,Ros2Fanuc_msgsSrvSetBoolIO_Request,Ros2Fanuc_msgsSrvSetBoolIO_RequestCodec,Ros2Fanuc_msgsSrvSetBoolIO_Response,Ros2Fanuc_msgsSrvSetBoolIO_ResponseCodec,Ros2Fanuc_msgsSrvSetGenOverride_Request,Ros2Fanuc_msgsSrvSetGenOverride_RequestCodec,Ros2Fanuc_msgsSrvSetGenOverride_Response,Ros2Fanuc_msgsSrvSetGenOverride_ResponseCodec,Ros2Fanuc_msgsSrvSetGroupIO_Request,Ros2Fanuc_msgsSrvSetGroupIO_RequestCodec,Ros2Fanuc_msgsSrvSetGroupIO_Response,Ros2Fanuc_msgsSrvSetGroupIO_ResponseCodec,Ros2Fanuc_msgsSrvSetNumReg_Request,Ros2Fanuc_msgsSrvSetNumReg_RequestCodec,Ros2Fanuc_msgsSrvSetNumReg_Response,Ros2Fanuc_msgsSrvSetNumReg_ResponseCodec,Ros2Fanuc_msgsSrvSetPayloadComp_Request,Ros2Fanuc_msgsSrvSetPayloadComp_RequestCodec,Ros2Fanuc_msgsSrvSetPayloadComp_Response,Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCodec,Ros2Fanuc_msgsSrvSetPayloadID_Request,Ros2Fanuc_msgsSrvSetPayloadID_RequestCodec,Ros2Fanuc_msgsSrvSetPayloadID_Response,Ros2Fanuc_msgsSrvSetPayloadID_ResponseCodec,Ros2Fanuc_msgsSrvSetPayloadValue_Request,Ros2Fanuc_msgsSrvSetPayloadValue_RequestCodec,Ros2Fanuc_msgsSrvSetPayloadValue_Response,Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCodec,Ros2Fanuc_msgsSrvSetPosReg_Request,Ros2Fanuc_msgsSrvSetPosReg_RequestCodec,Ros2Fanuc_msgsSrvSetPosReg_Response,Ros2Fanuc_msgsSrvSetPosReg_ResponseCodec,Ros2Fanuc_msgsSrvSetUFrameData_Request,Ros2Fanuc_msgsSrvSetUFrameData_RequestCodec,Ros2Fanuc_msgsSrvSetUFrameData_Response,Ros2Fanuc_msgsSrvSetUFrameData_ResponseCodec,Ros2Fanuc_msgsSrvSetUFrameUTool_Request,Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCodec,Ros2Fanuc_msgsSrvSetUFrameUTool_Response,Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCodec,Ros2Fanuc_msgsSrvSetUToolData_Request,Ros2Fanuc_msgsSrvSetUToolData_RequestCodec,Ros2Fanuc_msgsSrvSetUToolData_Response,Ros2Fanuc_msgsSrvSetUToolData_ResponseCodec,Ros2Fanuc_msgsSrvSwitchControlState_Request,Ros2Fanuc_msgsSrvSwitchControlState_RequestCodec,Ros2Fanuc_msgsSrvSwitchControlState_Response,Ros2Fanuc_msgsSrvSwitchControlState_ResponseCodec,Ros2Rmi_msgsSrvAddCall_Request,Ros2Rmi_msgsSrvAddCall_RequestCodec,Ros2Rmi_msgsSrvAddCall_Response,Ros2Rmi_msgsSrvAddCall_ResponseCodec,Ros2Rmi_msgsSrvAddMotionInstruction_Request,Ros2Rmi_msgsSrvAddMotionInstruction_RequestCodec,Ros2Rmi_msgsSrvAddMotionInstruction_Response,Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCodec,Ros2Rmi_msgsSrvAddSetPayload_Request,Ros2Rmi_msgsSrvAddSetPayload_RequestCodec,Ros2Rmi_msgsSrvAddSetPayload_Response,Ros2Rmi_msgsSrvAddSetPayload_ResponseCodec,Ros2Rmi_msgsSrvAddSetUFrame_Request,Ros2Rmi_msgsSrvAddSetUFrame_RequestCodec,Ros2Rmi_msgsSrvAddSetUFrame_Response,Ros2Rmi_msgsSrvAddSetUFrame_ResponseCodec,Ros2Rmi_msgsSrvAddSetUTool_Request,Ros2Rmi_msgsSrvAddSetUTool_RequestCodec,Ros2Rmi_msgsSrvAddSetUTool_Response,Ros2Rmi_msgsSrvAddSetUTool_ResponseCodec,Ros2Rmi_msgsSrvAddWaitDIN_Request,Ros2Rmi_msgsSrvAddWaitDIN_RequestCodec,Ros2Rmi_msgsSrvAddWaitDIN_Response,Ros2Rmi_msgsSrvAddWaitDIN_ResponseCodec,Ros2Rmi_msgsSrvAddWaitTime_Request,Ros2Rmi_msgsSrvAddWaitTime_RequestCodec,Ros2Rmi_msgsSrvAddWaitTime_Response,Ros2Rmi_msgsSrvAddWaitTime_ResponseCodec,Ros2Rmi_msgsSrvCallCommand_Request,Ros2Rmi_msgsSrvCallCommand_RequestCodec,Ros2Rmi_msgsSrvCallCommand_Response,Ros2Rmi_msgsSrvCallCommand_ResponseCodec,Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_RequestCodec,Ros2Std_srvsSrvTrigger_Response,Ros2Std_srvsSrvTrigger_ResponseCodec} from "./bounded_records";

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

/** Complete fanuc_msgs/srv/CfgForceSensor typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvCfgForceSensorClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvCfgForceSensor_Request,Ros2Fanuc_msgsSrvCfgForceSensor_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvCfgForceSensor_RequestCodec(),new Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCodec(),(value:Ros2Fanuc_msgsSrvCfgForceSensor_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/CfgForceSensor incoming request/response owner. */
export class Ros2Fanuc_msgsSrvCfgForceSensorServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvCfgForceSensor_Request,Ros2Fanuc_msgsSrvCfgForceSensor_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvCfgForceSensor_RequestCodec(),new Ros2Fanuc_msgsSrvCfgForceSensor_ResponseCodec(),(value:Ros2Fanuc_msgsSrvCfgForceSensor_Response):bool=>value.validValue(),8,capacity);}}

/** Complete fanuc_msgs/srv/GetAnalogIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetAnalogIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetAnalogIO_Request,Ros2Fanuc_msgsSrvGetAnalogIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetAnalogIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetAnalogIO_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetAnalogIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetAnalogIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetAnalogIO_Request,Ros2Fanuc_msgsSrvGetAnalogIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetAnalogIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetAnalogIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetAnalogIO_Response):bool=>value.validValue(),70,capacity);}}

/** Complete fanuc_msgs/srv/GetBoolIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetBoolIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetBoolIO_Request,Ros2Fanuc_msgsSrvGetBoolIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetBoolIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetBoolIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetBoolIO_Request):bool=>value.validValue(),5,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetBoolIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetBoolIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetBoolIO_Request,Ros2Fanuc_msgsSrvGetBoolIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetBoolIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetBoolIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetBoolIO_Response):bool=>value.validValue(),70,capacity);}}

/** Complete fanuc_msgs/srv/GetCartesianPosition typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetCartesianPositionClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetCartesianPosition_Request,Ros2Fanuc_msgsSrvGetCartesianPosition_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCodec(),new Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Request):bool=>value.validValue(),54,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetCartesianPosition incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetCartesianPositionServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetCartesianPosition_Request,Ros2Fanuc_msgsSrvGetCartesianPosition_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetCartesianPosition_RequestCodec(),new Ros2Fanuc_msgsSrvGetCartesianPosition_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetCartesianPosition_Response):bool=>value.validValue(),1,capacity);}}

/** Complete fanuc_msgs/srv/GetGroupIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetGroupIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetGroupIO_Request,Ros2Fanuc_msgsSrvGetGroupIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetGroupIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetGroupIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetGroupIO_Request):bool=>value.validValue(),6,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetGroupIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetGroupIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetGroupIO_Request,Ros2Fanuc_msgsSrvGetGroupIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetGroupIO_RequestCodec(),new Ros2Fanuc_msgsSrvGetGroupIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetGroupIO_Response):bool=>value.validValue(),70,capacity);}}

/** Complete fanuc_msgs/srv/GetNumReg typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetNumRegClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetNumReg_Request,Ros2Fanuc_msgsSrvGetNumReg_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetNumReg_RequestCodec(),new Ros2Fanuc_msgsSrvGetNumReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetNumReg_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetNumReg incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetNumRegServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetNumReg_Request,Ros2Fanuc_msgsSrvGetNumReg_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetNumReg_RequestCodec(),new Ros2Fanuc_msgsSrvGetNumReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetNumReg_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadComp typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetPayloadCompClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetPayloadComp_Request,Ros2Fanuc_msgsSrvGetPayloadComp_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetPayloadComp_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadComp_Request):bool=>value.validValue(),32,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadComp incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetPayloadCompServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetPayloadComp_Request,Ros2Fanuc_msgsSrvGetPayloadComp_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetPayloadComp_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadComp_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadComp_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadID typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetPayloadIDClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetPayloadID_Request,Ros2Fanuc_msgsSrvGetPayloadID_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetPayloadID_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadID_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadID_Request):bool=>value.validValue(),6,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadID incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetPayloadIDServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetPayloadID_Request,Ros2Fanuc_msgsSrvGetPayloadID_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetPayloadID_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadID_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadID_Response):bool=>value.validValue(),0,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadValue typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetPayloadValueClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetPayloadValue_Request,Ros2Fanuc_msgsSrvGetPayloadValue_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetPayloadValue_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadValue_Request):bool=>value.validValue(),32,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetPayloadValue incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetPayloadValueServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetPayloadValue_Request,Ros2Fanuc_msgsSrvGetPayloadValue_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetPayloadValue_RequestCodec(),new Ros2Fanuc_msgsSrvGetPayloadValue_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPayloadValue_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/GetPosReg typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetPosRegClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetPosReg_Request,Ros2Fanuc_msgsSrvGetPosReg_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetPosReg_RequestCodec(),new Ros2Fanuc_msgsSrvGetPosReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPosReg_Request):bool=>value.validValue(),601,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetPosReg incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetPosRegServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetPosReg_Request,Ros2Fanuc_msgsSrvGetPosReg_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetPosReg_RequestCodec(),new Ros2Fanuc_msgsSrvGetPosReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetPosReg_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/GetTCPSpeed typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetTCPSpeedClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetTCPSpeed_Request,Ros2Fanuc_msgsSrvGetTCPSpeed_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCodec(),new Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Request):bool=>value.validValue(),13,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetTCPSpeed incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetTCPSpeedServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetTCPSpeed_Request,Ros2Fanuc_msgsSrvGetTCPSpeed_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetTCPSpeed_RequestCodec(),new Ros2Fanuc_msgsSrvGetTCPSpeed_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetTCPSpeed_Response):bool=>value.validValue(),1,capacity);}}

/** Complete fanuc_msgs/srv/GetUFrameData typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetUFrameDataClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetUFrameData_Request,Ros2Fanuc_msgsSrvGetUFrameData_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetUFrameData_RequestCodec(),new Ros2Fanuc_msgsSrvGetUFrameData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUFrameData_Request):bool=>value.validValue(),30,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetUFrameData incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetUFrameDataServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetUFrameData_Request,Ros2Fanuc_msgsSrvGetUFrameData_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetUFrameData_RequestCodec(),new Ros2Fanuc_msgsSrvGetUFrameData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUFrameData_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/GetUFrameUTool typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetUFrameUToolClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetUFrameUTool_Request,Ros2Fanuc_msgsSrvGetUFrameUTool_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCodec(),new Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Request):bool=>value.validValue(),7,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetUFrameUTool incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetUFrameUToolServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetUFrameUTool_Request,Ros2Fanuc_msgsSrvGetUFrameUTool_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetUFrameUTool_RequestCodec(),new Ros2Fanuc_msgsSrvGetUFrameUTool_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUFrameUTool_Response):bool=>value.validValue(),1,capacity);}}

/** Complete fanuc_msgs/srv/GetUToolData typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvGetUToolDataClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvGetUToolData_Request,Ros2Fanuc_msgsSrvGetUToolData_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvGetUToolData_RequestCodec(),new Ros2Fanuc_msgsSrvGetUToolData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUToolData_Request):bool=>value.validValue(),30,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/GetUToolData incoming request/response owner. */
export class Ros2Fanuc_msgsSrvGetUToolDataServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvGetUToolData_Request,Ros2Fanuc_msgsSrvGetUToolData_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvGetUToolData_RequestCodec(),new Ros2Fanuc_msgsSrvGetUToolData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvGetUToolData_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/ReadError typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvReadErrorClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvReadError_Request,Ros2Fanuc_msgsSrvReadError_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvReadError_RequestCodec(),new Ros2Fanuc_msgsSrvReadError_ResponseCodec(),(value:Ros2Fanuc_msgsSrvReadError_Request):bool=>value.validValue(),2585,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/ReadError incoming request/response owner. */
export class Ros2Fanuc_msgsSrvReadErrorServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvReadError_Request,Ros2Fanuc_msgsSrvReadError_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvReadError_RequestCodec(),new Ros2Fanuc_msgsSrvReadError_ResponseCodec(),(value:Ros2Fanuc_msgsSrvReadError_Response):bool=>value.validValue(),1,capacity);}}

/** Complete fanuc_msgs/srv/Reset typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvResetClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvReset_Request,Ros2Fanuc_msgsSrvReset_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvReset_RequestCodec(),new Ros2Fanuc_msgsSrvReset_ResponseCodec(),(value:Ros2Fanuc_msgsSrvReset_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/Reset incoming request/response owner. */
export class Ros2Fanuc_msgsSrvResetServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvReset_Request,Ros2Fanuc_msgsSrvReset_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvReset_RequestCodec(),new Ros2Fanuc_msgsSrvReset_ResponseCodec(),(value:Ros2Fanuc_msgsSrvReset_Response):bool=>value.validValue(),0,capacity);}}

/** Complete fanuc_msgs/srv/SetAnalogIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetAnalogIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetAnalogIO_Request,Ros2Fanuc_msgsSrvSetAnalogIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetAnalogIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetAnalogIO_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetAnalogIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetAnalogIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetAnalogIO_Request,Ros2Fanuc_msgsSrvSetAnalogIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetAnalogIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetAnalogIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetAnalogIO_Response):bool=>value.validValue(),74,capacity);}}

/** Complete fanuc_msgs/srv/SetBoolIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetBoolIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetBoolIO_Request,Ros2Fanuc_msgsSrvSetBoolIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetBoolIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetBoolIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetBoolIO_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetBoolIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetBoolIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetBoolIO_Request,Ros2Fanuc_msgsSrvSetBoolIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetBoolIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetBoolIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetBoolIO_Response):bool=>value.validValue(),71,capacity);}}

/** Complete fanuc_msgs/srv/SetGenOverride typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetGenOverrideClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetGenOverride_Request,Ros2Fanuc_msgsSrvSetGenOverride_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetGenOverride_RequestCodec(),new Ros2Fanuc_msgsSrvSetGenOverride_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetGenOverride_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetGenOverride incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetGenOverrideServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetGenOverride_Request,Ros2Fanuc_msgsSrvSetGenOverride_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetGenOverride_RequestCodec(),new Ros2Fanuc_msgsSrvSetGenOverride_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetGenOverride_Response):bool=>value.validValue(),1,capacity);}}

/** Complete fanuc_msgs/srv/SetGroupIO typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetGroupIOClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetGroupIO_Request,Ros2Fanuc_msgsSrvSetGroupIO_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetGroupIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetGroupIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetGroupIO_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetGroupIO incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetGroupIOServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetGroupIO_Request,Ros2Fanuc_msgsSrvSetGroupIO_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetGroupIO_RequestCodec(),new Ros2Fanuc_msgsSrvSetGroupIO_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetGroupIO_Response):bool=>value.validValue(),72,capacity);}}

/** Complete fanuc_msgs/srv/SetNumReg typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetNumRegClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetNumReg_Request,Ros2Fanuc_msgsSrvSetNumReg_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetNumReg_RequestCodec(),new Ros2Fanuc_msgsSrvSetNumReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetNumReg_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetNumReg incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetNumRegServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetNumReg_Request,Ros2Fanuc_msgsSrvSetNumReg_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetNumReg_RequestCodec(),new Ros2Fanuc_msgsSrvSetNumReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetNumReg_Response):bool=>value.validValue(),6,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadComp typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetPayloadCompClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetPayloadComp_Request,Ros2Fanuc_msgsSrvSetPayloadComp_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetPayloadComp_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadComp_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadComp incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetPayloadCompServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetPayloadComp_Request,Ros2Fanuc_msgsSrvSetPayloadComp_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetPayloadComp_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadComp_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadComp_Response):bool=>value.validValue(),30,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadID typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetPayloadIDClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetPayloadID_Request,Ros2Fanuc_msgsSrvSetPayloadID_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetPayloadID_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadID_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadID_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadID incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetPayloadIDServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetPayloadID_Request,Ros2Fanuc_msgsSrvSetPayloadID_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetPayloadID_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadID_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadID_Response):bool=>value.validValue(),2,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadValue typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetPayloadValueClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetPayloadValue_Request,Ros2Fanuc_msgsSrvSetPayloadValue_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetPayloadValue_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadValue_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetPayloadValue incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetPayloadValueServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetPayloadValue_Request,Ros2Fanuc_msgsSrvSetPayloadValue_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetPayloadValue_RequestCodec(),new Ros2Fanuc_msgsSrvSetPayloadValue_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPayloadValue_Response):bool=>value.validValue(),31,capacity);}}

/** Complete fanuc_msgs/srv/SetPosReg typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetPosRegClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetPosReg_Request,Ros2Fanuc_msgsSrvSetPosReg_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetPosReg_RequestCodec(),new Ros2Fanuc_msgsSrvSetPosReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPosReg_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetPosReg incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetPosRegServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetPosReg_Request,Ros2Fanuc_msgsSrvSetPosReg_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetPosReg_RequestCodec(),new Ros2Fanuc_msgsSrvSetPosReg_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetPosReg_Response):bool=>value.validValue(),599,capacity);}}

/** Complete fanuc_msgs/srv/SetUFrameData typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetUFrameDataClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetUFrameData_Request,Ros2Fanuc_msgsSrvSetUFrameData_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetUFrameData_RequestCodec(),new Ros2Fanuc_msgsSrvSetUFrameData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUFrameData_Request):bool=>value.validValue(),5,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetUFrameData incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetUFrameDataServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetUFrameData_Request,Ros2Fanuc_msgsSrvSetUFrameData_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetUFrameData_RequestCodec(),new Ros2Fanuc_msgsSrvSetUFrameData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUFrameData_Response):bool=>value.validValue(),26,capacity);}}

/** Complete fanuc_msgs/srv/SetUFrameUTool typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetUFrameUToolClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetUFrameUTool_Request,Ros2Fanuc_msgsSrvSetUFrameUTool_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCodec(),new Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Request):bool=>value.validValue(),5,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetUFrameUTool incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetUFrameUToolServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetUFrameUTool_Request,Ros2Fanuc_msgsSrvSetUFrameUTool_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetUFrameUTool_RequestCodec(),new Ros2Fanuc_msgsSrvSetUFrameUTool_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUFrameUTool_Response):bool=>value.validValue(),3,capacity);}}

/** Complete fanuc_msgs/srv/SetUToolData typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSetUToolDataClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSetUToolData_Request,Ros2Fanuc_msgsSrvSetUToolData_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSetUToolData_RequestCodec(),new Ros2Fanuc_msgsSrvSetUToolData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUToolData_Request):bool=>value.validValue(),5,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SetUToolData incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSetUToolDataServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSetUToolData_Request,Ros2Fanuc_msgsSrvSetUToolData_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSetUToolData_RequestCodec(),new Ros2Fanuc_msgsSrvSetUToolData_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSetUToolData_Response):bool=>value.validValue(),26,capacity);}}

/** Complete fanuc_msgs/srv/SwitchControlState typed call and real-submit client. */
export class Ros2Fanuc_msgsSrvSwitchControlStateClient extends BoundedServiceClient<Ros2Fanuc_msgsSrvSwitchControlState_Request,Ros2Fanuc_msgsSrvSwitchControlState_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Fanuc_msgsSrvSwitchControlState_RequestCodec(),new Ros2Fanuc_msgsSrvSwitchControlState_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSwitchControlState_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete fanuc_msgs/srv/SwitchControlState incoming request/response owner. */
export class Ros2Fanuc_msgsSrvSwitchControlStateServer extends BoundedServiceServer<Ros2Fanuc_msgsSrvSwitchControlState_Request,Ros2Fanuc_msgsSrvSwitchControlState_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Fanuc_msgsSrvSwitchControlState_RequestCodec(),new Ros2Fanuc_msgsSrvSwitchControlState_ResponseCodec(),(value:Ros2Fanuc_msgsSrvSwitchControlState_Response):bool=>value.validValue(),4,capacity);}}

/** Complete rmi_msgs/srv/AddCall typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddCallClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddCall_Request,Ros2Rmi_msgsSrvAddCall_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddCall_RequestCodec(),new Ros2Rmi_msgsSrvAddCall_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddCall_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddCall incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddCallServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddCall_Request,Ros2Rmi_msgsSrvAddCall_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddCall_RequestCodec(),new Ros2Rmi_msgsSrvAddCall_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddCall_Response):bool=>value.validValue(),9992,capacity);}}

/** Complete rmi_msgs/srv/AddMotionInstruction typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddMotionInstructionClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddMotionInstruction_Request,Ros2Rmi_msgsSrvAddMotionInstruction_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddMotionInstruction_RequestCodec(),new Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddMotionInstruction_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddMotionInstruction incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddMotionInstructionServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddMotionInstruction_Request,Ros2Rmi_msgsSrvAddMotionInstruction_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddMotionInstruction_RequestCodec(),new Ros2Rmi_msgsSrvAddMotionInstruction_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddMotionInstruction_Response):bool=>value.validValue(),4014,capacity);}}

/** Complete rmi_msgs/srv/AddSetPayload typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddSetPayloadClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddSetPayload_Request,Ros2Rmi_msgsSrvAddSetPayload_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddSetPayload_RequestCodec(),new Ros2Rmi_msgsSrvAddSetPayload_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetPayload_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddSetPayload incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddSetPayloadServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddSetPayload_Request,Ros2Rmi_msgsSrvAddSetPayload_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddSetPayload_RequestCodec(),new Ros2Rmi_msgsSrvAddSetPayload_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetPayload_Response):bool=>value.validValue(),1,capacity);}}

/** Complete rmi_msgs/srv/AddSetUFrame typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddSetUFrameClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddSetUFrame_Request,Ros2Rmi_msgsSrvAddSetUFrame_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddSetUFrame_RequestCodec(),new Ros2Rmi_msgsSrvAddSetUFrame_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetUFrame_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddSetUFrame incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddSetUFrameServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddSetUFrame_Request,Ros2Rmi_msgsSrvAddSetUFrame_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddSetUFrame_RequestCodec(),new Ros2Rmi_msgsSrvAddSetUFrame_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetUFrame_Response):bool=>value.validValue(),1,capacity);}}

/** Complete rmi_msgs/srv/AddSetUTool typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddSetUToolClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddSetUTool_Request,Ros2Rmi_msgsSrvAddSetUTool_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddSetUTool_RequestCodec(),new Ros2Rmi_msgsSrvAddSetUTool_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetUTool_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddSetUTool incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddSetUToolServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddSetUTool_Request,Ros2Rmi_msgsSrvAddSetUTool_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddSetUTool_RequestCodec(),new Ros2Rmi_msgsSrvAddSetUTool_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddSetUTool_Response):bool=>value.validValue(),1,capacity);}}

/** Complete rmi_msgs/srv/AddWaitDIN typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddWaitDINClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddWaitDIN_Request,Ros2Rmi_msgsSrvAddWaitDIN_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddWaitDIN_RequestCodec(),new Ros2Rmi_msgsSrvAddWaitDIN_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddWaitDIN_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddWaitDIN incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddWaitDINServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddWaitDIN_Request,Ros2Rmi_msgsSrvAddWaitDIN_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddWaitDIN_RequestCodec(),new Ros2Rmi_msgsSrvAddWaitDIN_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddWaitDIN_Response):bool=>value.validValue(),518,capacity);}}

/** Complete rmi_msgs/srv/AddWaitTime typed call and real-submit client. */
export class Ros2Rmi_msgsSrvAddWaitTimeClient extends BoundedServiceClient<Ros2Rmi_msgsSrvAddWaitTime_Request,Ros2Rmi_msgsSrvAddWaitTime_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvAddWaitTime_RequestCodec(),new Ros2Rmi_msgsSrvAddWaitTime_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddWaitTime_Request):bool=>value.validValue(),8,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/AddWaitTime incoming request/response owner. */
export class Ros2Rmi_msgsSrvAddWaitTimeServer extends BoundedServiceServer<Ros2Rmi_msgsSrvAddWaitTime_Request,Ros2Rmi_msgsSrvAddWaitTime_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvAddWaitTime_RequestCodec(),new Ros2Rmi_msgsSrvAddWaitTime_ResponseCodec(),(value:Ros2Rmi_msgsSrvAddWaitTime_Response):bool=>value.validValue(),4,capacity);}}

/** Complete rmi_msgs/srv/CallCommand typed call and real-submit client. */
export class Ros2Rmi_msgsSrvCallCommandClient extends BoundedServiceClient<Ros2Rmi_msgsSrvCallCommand_Request,Ros2Rmi_msgsSrvCallCommand_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Rmi_msgsSrvCallCommand_RequestCodec(),new Ros2Rmi_msgsSrvCallCommand_ResponseCodec(),(value:Ros2Rmi_msgsSrvCallCommand_Request):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete rmi_msgs/srv/CallCommand incoming request/response owner. */
export class Ros2Rmi_msgsSrvCallCommandServer extends BoundedServiceServer<Ros2Rmi_msgsSrvCallCommand_Request,Ros2Rmi_msgsSrvCallCommand_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Rmi_msgsSrvCallCommand_RequestCodec(),new Ros2Rmi_msgsSrvCallCommand_ResponseCodec(),(value:Ros2Rmi_msgsSrvCallCommand_Response):bool=>value.validValue(),1104,capacity);}}

/** Complete std_srvs/srv/Trigger typed call and real-submit client. */
export class Ros2Std_srvsSrvTriggerClient extends BoundedServiceClient<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Std_srvsSrvTrigger_RequestCodec(),new Ros2Std_srvsSrvTrigger_ResponseCodec(),(value:Ros2Std_srvsSrvTrigger_Request):bool=>value.validValue(),517,observerAlias,capacity);}}

/** Complete std_srvs/srv/Trigger incoming request/response owner. */
export class Ros2Std_srvsSrvTriggerServer extends BoundedServiceServer<Ros2Std_srvsSrvTrigger_Request,Ros2Std_srvsSrvTrigger_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Std_srvsSrvTrigger_RequestCodec(),new Ros2Std_srvsSrvTrigger_ResponseCodec(),(value:Ros2Std_srvsSrvTrigger_Response):bool=>value.validValue(),0,capacity);}}
