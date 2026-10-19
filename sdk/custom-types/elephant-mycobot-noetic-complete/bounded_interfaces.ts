/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros1Mybuddy_communicationGetAnglesRequest,Ros1Mybuddy_communicationGetAnglesRequestCodec,Ros1Mybuddy_communicationGetAnglesResponse,Ros1Mybuddy_communicationGetAnglesResponseCodec,Ros1Mybuddy_communicationGetCoordsRequest,Ros1Mybuddy_communicationGetCoordsRequestCodec,Ros1Mybuddy_communicationGetCoordsResponse,Ros1Mybuddy_communicationGetCoordsResponseCodec,Ros1Mybuddy_communicationGripperStatusRequest,Ros1Mybuddy_communicationGripperStatusRequestCodec,Ros1Mybuddy_communicationGripperStatusResponse,Ros1Mybuddy_communicationGripperStatusResponseCodec,Ros1Mybuddy_communicationPumpStatusRequest,Ros1Mybuddy_communicationPumpStatusRequestCodec,Ros1Mybuddy_communicationPumpStatusResponse,Ros1Mybuddy_communicationPumpStatusResponseCodec,Ros1Mybuddy_communicationSetAnglesRequest,Ros1Mybuddy_communicationSetAnglesRequestCodec,Ros1Mybuddy_communicationSetAnglesResponse,Ros1Mybuddy_communicationSetAnglesResponseCodec,Ros1Mybuddy_communicationSetCoordsRequest,Ros1Mybuddy_communicationSetCoordsRequestCodec,Ros1Mybuddy_communicationSetCoordsResponse,Ros1Mybuddy_communicationSetCoordsResponseCodec,Ros1Mecharm_communicationGetAnglesRequest,Ros1Mecharm_communicationGetAnglesRequestCodec,Ros1Mecharm_communicationGetAnglesResponse,Ros1Mecharm_communicationGetAnglesResponseCodec,Ros1Mecharm_communicationGetCoordsRequest,Ros1Mecharm_communicationGetCoordsRequestCodec,Ros1Mecharm_communicationGetCoordsResponse,Ros1Mecharm_communicationGetCoordsResponseCodec,Ros1Mecharm_communicationGripperStatusRequest,Ros1Mecharm_communicationGripperStatusRequestCodec,Ros1Mecharm_communicationGripperStatusResponse,Ros1Mecharm_communicationGripperStatusResponseCodec,Ros1Mecharm_communicationPumpStatusRequest,Ros1Mecharm_communicationPumpStatusRequestCodec,Ros1Mecharm_communicationPumpStatusResponse,Ros1Mecharm_communicationPumpStatusResponseCodec,Ros1Mecharm_communicationSetAnglesRequest,Ros1Mecharm_communicationSetAnglesRequestCodec,Ros1Mecharm_communicationSetAnglesResponse,Ros1Mecharm_communicationSetAnglesResponseCodec,Ros1Mecharm_communicationSetCoordsRequest,Ros1Mecharm_communicationSetCoordsRequestCodec,Ros1Mecharm_communicationSetCoordsResponse,Ros1Mecharm_communicationSetCoordsResponseCodec,Ros1Myarm_communicationGetAnglesRequest,Ros1Myarm_communicationGetAnglesRequestCodec,Ros1Myarm_communicationGetAnglesResponse,Ros1Myarm_communicationGetAnglesResponseCodec,Ros1Myarm_communicationGetCoordsRequest,Ros1Myarm_communicationGetCoordsRequestCodec,Ros1Myarm_communicationGetCoordsResponse,Ros1Myarm_communicationGetCoordsResponseCodec,Ros1Myarm_communicationGripperStatusRequest,Ros1Myarm_communicationGripperStatusRequestCodec,Ros1Myarm_communicationGripperStatusResponse,Ros1Myarm_communicationGripperStatusResponseCodec,Ros1Myarm_communicationPumpStatusRequest,Ros1Myarm_communicationPumpStatusRequestCodec,Ros1Myarm_communicationPumpStatusResponse,Ros1Myarm_communicationPumpStatusResponseCodec,Ros1Myarm_communicationSetAnglesRequest,Ros1Myarm_communicationSetAnglesRequestCodec,Ros1Myarm_communicationSetAnglesResponse,Ros1Myarm_communicationSetAnglesResponseCodec,Ros1Myarm_communicationSetCoordsRequest,Ros1Myarm_communicationSetCoordsRequestCodec,Ros1Myarm_communicationSetCoordsResponse,Ros1Myarm_communicationSetCoordsResponseCodec,Ros1Mycobot_320_communicationGetAnglesRequest,Ros1Mycobot_320_communicationGetAnglesRequestCodec,Ros1Mycobot_320_communicationGetAnglesResponse,Ros1Mycobot_320_communicationGetAnglesResponseCodec,Ros1Mycobot_320_communicationGetCoordsRequest,Ros1Mycobot_320_communicationGetCoordsRequestCodec,Ros1Mycobot_320_communicationGetCoordsResponse,Ros1Mycobot_320_communicationGetCoordsResponseCodec,Ros1Mycobot_320_communicationGripperStatusRequest,Ros1Mycobot_320_communicationGripperStatusRequestCodec,Ros1Mycobot_320_communicationGripperStatusResponse,Ros1Mycobot_320_communicationGripperStatusResponseCodec,Ros1Mycobot_320_communicationPumpStatusRequest,Ros1Mycobot_320_communicationPumpStatusRequestCodec,Ros1Mycobot_320_communicationPumpStatusResponse,Ros1Mycobot_320_communicationPumpStatusResponseCodec,Ros1Mycobot_320_communicationSetAnglesRequest,Ros1Mycobot_320_communicationSetAnglesRequestCodec,Ros1Mycobot_320_communicationSetAnglesResponse,Ros1Mycobot_320_communicationSetAnglesResponseCodec,Ros1Mycobot_320_communicationSetCoordsRequest,Ros1Mycobot_320_communicationSetCoordsRequestCodec,Ros1Mycobot_320_communicationSetCoordsResponse,Ros1Mycobot_320_communicationSetCoordsResponseCodec,Ros1Mycobot_communicationGetAnglesRequest,Ros1Mycobot_communicationGetAnglesRequestCodec,Ros1Mycobot_communicationGetAnglesResponse,Ros1Mycobot_communicationGetAnglesResponseCodec,Ros1Mycobot_communicationGetCoordsRequest,Ros1Mycobot_communicationGetCoordsRequestCodec,Ros1Mycobot_communicationGetCoordsResponse,Ros1Mycobot_communicationGetCoordsResponseCodec,Ros1Mycobot_communicationGripperStatusRequest,Ros1Mycobot_communicationGripperStatusRequestCodec,Ros1Mycobot_communicationGripperStatusResponse,Ros1Mycobot_communicationGripperStatusResponseCodec,Ros1Mycobot_communicationPumpStatusRequest,Ros1Mycobot_communicationPumpStatusRequestCodec,Ros1Mycobot_communicationPumpStatusResponse,Ros1Mycobot_communicationPumpStatusResponseCodec,Ros1Mycobot_communicationSetAnglesRequest,Ros1Mycobot_communicationSetAnglesRequestCodec,Ros1Mycobot_communicationSetAnglesResponse,Ros1Mycobot_communicationSetAnglesResponseCodec,Ros1Mycobot_communicationSetCoordsRequest,Ros1Mycobot_communicationSetCoordsRequestCodec,Ros1Mycobot_communicationSetCoordsResponse,Ros1Mycobot_communicationSetCoordsResponseCodec,Ros1Mycobot_communicationSetEndTypeRequest,Ros1Mycobot_communicationSetEndTypeRequestCodec,Ros1Mycobot_communicationSetEndTypeResponse,Ros1Mycobot_communicationSetEndTypeResponseCodec,Ros1Mycobot_communicationSetFreshModeRequest,Ros1Mycobot_communicationSetFreshModeRequestCodec,Ros1Mycobot_communicationSetFreshModeResponse,Ros1Mycobot_communicationSetFreshModeResponseCodec,Ros1Mycobot_communicationSetToolReferenceRequest,Ros1Mycobot_communicationSetToolReferenceRequestCodec,Ros1Mycobot_communicationSetToolReferenceResponse,Ros1Mycobot_communicationSetToolReferenceResponseCodec,Ros1Mycobot_communicationSetVisionModeRequest,Ros1Mycobot_communicationSetVisionModeRequestCodec,Ros1Mycobot_communicationSetVisionModeResponse,Ros1Mycobot_communicationSetVisionModeResponseCodec,Ros1Mycobot_pro450_communicationGetAnglesRequest,Ros1Mycobot_pro450_communicationGetAnglesRequestCodec,Ros1Mycobot_pro450_communicationGetAnglesResponse,Ros1Mycobot_pro450_communicationGetAnglesResponseCodec,Ros1Mycobot_pro450_communicationGetCoordsRequest,Ros1Mycobot_pro450_communicationGetCoordsRequestCodec,Ros1Mycobot_pro450_communicationGetCoordsResponse,Ros1Mycobot_pro450_communicationGetCoordsResponseCodec,Ros1Mycobot_pro450_communicationGetGripperValueRequest,Ros1Mycobot_pro450_communicationGetGripperValueRequestCodec,Ros1Mycobot_pro450_communicationGetGripperValueResponse,Ros1Mycobot_pro450_communicationGetGripperValueResponseCodec,Ros1Mycobot_pro450_communicationGripperStatusRequest,Ros1Mycobot_pro450_communicationGripperStatusRequestCodec,Ros1Mycobot_pro450_communicationGripperStatusResponse,Ros1Mycobot_pro450_communicationGripperStatusResponseCodec,Ros1Mycobot_pro450_communicationSetAnglesRequest,Ros1Mycobot_pro450_communicationSetAnglesRequestCodec,Ros1Mycobot_pro450_communicationSetAnglesResponse,Ros1Mycobot_pro450_communicationSetAnglesResponseCodec,Ros1Mycobot_pro450_communicationSetCoordsRequest,Ros1Mycobot_pro450_communicationSetCoordsRequestCodec,Ros1Mycobot_pro450_communicationSetCoordsResponse,Ros1Mycobot_pro450_communicationSetCoordsResponseCodec,Ros1Mycobot_pro450_communicationSetFreshModeRequest,Ros1Mycobot_pro450_communicationSetFreshModeRequestCodec,Ros1Mycobot_pro450_communicationSetFreshModeResponse,Ros1Mycobot_pro450_communicationSetFreshModeResponseCodec,Ros1Mypalletizer_communicationGetAnglesRequest,Ros1Mypalletizer_communicationGetAnglesRequestCodec,Ros1Mypalletizer_communicationGetAnglesResponse,Ros1Mypalletizer_communicationGetAnglesResponseCodec,Ros1Mypalletizer_communicationGetCoordsRequest,Ros1Mypalletizer_communicationGetCoordsRequestCodec,Ros1Mypalletizer_communicationGetCoordsResponse,Ros1Mypalletizer_communicationGetCoordsResponseCodec,Ros1Mypalletizer_communicationGripperStatusRequest,Ros1Mypalletizer_communicationGripperStatusRequestCodec,Ros1Mypalletizer_communicationGripperStatusResponse,Ros1Mypalletizer_communicationGripperStatusResponseCodec,Ros1Mypalletizer_communicationPumpStatusRequest,Ros1Mypalletizer_communicationPumpStatusRequestCodec,Ros1Mypalletizer_communicationPumpStatusResponse,Ros1Mypalletizer_communicationPumpStatusResponseCodec,Ros1Mypalletizer_communicationSetAnglesRequest,Ros1Mypalletizer_communicationSetAnglesRequestCodec,Ros1Mypalletizer_communicationSetAnglesResponse,Ros1Mypalletizer_communicationSetAnglesResponseCodec,Ros1Mypalletizer_communicationSetCoordsRequest,Ros1Mypalletizer_communicationSetCoordsRequestCodec,Ros1Mypalletizer_communicationSetCoordsResponse,Ros1Mypalletizer_communicationSetCoordsResponseCodec,Ros1Ultraarm_communicationGetAnglesRequest,Ros1Ultraarm_communicationGetAnglesRequestCodec,Ros1Ultraarm_communicationGetAnglesResponse,Ros1Ultraarm_communicationGetAnglesResponseCodec,Ros1Ultraarm_communicationGetCoordsRequest,Ros1Ultraarm_communicationGetCoordsRequestCodec,Ros1Ultraarm_communicationGetCoordsResponse,Ros1Ultraarm_communicationGetCoordsResponseCodec,Ros1Ultraarm_communicationGetGripperValueRequest,Ros1Ultraarm_communicationGetGripperValueRequestCodec,Ros1Ultraarm_communicationGetGripperValueResponse,Ros1Ultraarm_communicationGetGripperValueResponseCodec,Ros1Ultraarm_communicationGripperStatusRequest,Ros1Ultraarm_communicationGripperStatusRequestCodec,Ros1Ultraarm_communicationGripperStatusResponse,Ros1Ultraarm_communicationGripperStatusResponseCodec,Ros1Ultraarm_communicationSetAnglesRequest,Ros1Ultraarm_communicationSetAnglesRequestCodec,Ros1Ultraarm_communicationSetAnglesResponse,Ros1Ultraarm_communicationSetAnglesResponseCodec,Ros1Ultraarm_communicationSetCoordsRequest,Ros1Ultraarm_communicationSetCoordsRequestCodec,Ros1Ultraarm_communicationSetCoordsResponse,Ros1Ultraarm_communicationSetCoordsResponseCodec,Ros1Ultraarm_communicationSetFreshModeRequest,Ros1Ultraarm_communicationSetFreshModeRequestCodec,Ros1Ultraarm_communicationSetFreshModeResponse,Ros1Ultraarm_communicationSetFreshModeResponseCodec} from "./bounded_records";

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

/** Complete mybuddy_communication/GetAngles typed call and real-submit client. */
export class Ros1Mybuddy_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mybuddy_communicationGetAnglesRequest,Ros1Mybuddy_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationGetAnglesRequestCodec(),new Ros1Mybuddy_communicationGetAnglesResponseCodec(),(value:Ros1Mybuddy_communicationGetAnglesRequest):bool=>value.validValue(),52,observerAlias,capacity);}}

/** Complete mybuddy_communication/GetAngles incoming request/response owner. */
export class Ros1Mybuddy_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mybuddy_communicationGetAnglesRequest,Ros1Mybuddy_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationGetAnglesRequestCodec(),new Ros1Mybuddy_communicationGetAnglesResponseCodec(),(value:Ros1Mybuddy_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mybuddy_communication/GetCoords typed call and real-submit client. */
export class Ros1Mybuddy_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mybuddy_communicationGetCoordsRequest,Ros1Mybuddy_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationGetCoordsRequestCodec(),new Ros1Mybuddy_communicationGetCoordsResponseCodec(),(value:Ros1Mybuddy_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mybuddy_communication/GetCoords incoming request/response owner. */
export class Ros1Mybuddy_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mybuddy_communicationGetCoordsRequest,Ros1Mybuddy_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationGetCoordsRequestCodec(),new Ros1Mybuddy_communicationGetCoordsResponseCodec(),(value:Ros1Mybuddy_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mybuddy_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mybuddy_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mybuddy_communicationGripperStatusRequest,Ros1Mybuddy_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationGripperStatusRequestCodec(),new Ros1Mybuddy_communicationGripperStatusResponseCodec(),(value:Ros1Mybuddy_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mybuddy_communication/GripperStatus incoming request/response owner. */
export class Ros1Mybuddy_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mybuddy_communicationGripperStatusRequest,Ros1Mybuddy_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationGripperStatusRequestCodec(),new Ros1Mybuddy_communicationGripperStatusResponseCodec(),(value:Ros1Mybuddy_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mybuddy_communication/PumpStatus typed call and real-submit client. */
export class Ros1Mybuddy_communicationPumpStatusClient extends BoundedServiceClient<Ros1Mybuddy_communicationPumpStatusRequest,Ros1Mybuddy_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationPumpStatusRequestCodec(),new Ros1Mybuddy_communicationPumpStatusResponseCodec(),(value:Ros1Mybuddy_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mybuddy_communication/PumpStatus incoming request/response owner. */
export class Ros1Mybuddy_communicationPumpStatusServer extends BoundedServiceServer<Ros1Mybuddy_communicationPumpStatusRequest,Ros1Mybuddy_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationPumpStatusRequestCodec(),new Ros1Mybuddy_communicationPumpStatusResponseCodec(),(value:Ros1Mybuddy_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete mybuddy_communication/SetAngles typed call and real-submit client. */
export class Ros1Mybuddy_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mybuddy_communicationSetAnglesRequest,Ros1Mybuddy_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationSetAnglesRequestCodec(),new Ros1Mybuddy_communicationSetAnglesResponseCodec(),(value:Ros1Mybuddy_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mybuddy_communication/SetAngles incoming request/response owner. */
export class Ros1Mybuddy_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mybuddy_communicationSetAnglesRequest,Ros1Mybuddy_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationSetAnglesRequestCodec(),new Ros1Mybuddy_communicationSetAnglesResponseCodec(),(value:Ros1Mybuddy_communicationSetAnglesResponse):bool=>value.validValue(),53,capacity);}}

/** Complete mybuddy_communication/SetCoords typed call and real-submit client. */
export class Ros1Mybuddy_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mybuddy_communicationSetCoordsRequest,Ros1Mybuddy_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mybuddy_communicationSetCoordsRequestCodec(),new Ros1Mybuddy_communicationSetCoordsResponseCodec(),(value:Ros1Mybuddy_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mybuddy_communication/SetCoords incoming request/response owner. */
export class Ros1Mybuddy_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mybuddy_communicationSetCoordsRequest,Ros1Mybuddy_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mybuddy_communicationSetCoordsRequestCodec(),new Ros1Mybuddy_communicationSetCoordsResponseCodec(),(value:Ros1Mybuddy_communicationSetCoordsResponse):bool=>value.validValue(),26,capacity);}}

/** Complete mecharm_communication/GetAngles typed call and real-submit client. */
export class Ros1Mecharm_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mecharm_communicationGetAnglesRequest,Ros1Mecharm_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationGetAnglesRequestCodec(),new Ros1Mecharm_communicationGetAnglesResponseCodec(),(value:Ros1Mecharm_communicationGetAnglesRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mecharm_communication/GetAngles incoming request/response owner. */
export class Ros1Mecharm_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mecharm_communicationGetAnglesRequest,Ros1Mecharm_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationGetAnglesRequestCodec(),new Ros1Mecharm_communicationGetAnglesResponseCodec(),(value:Ros1Mecharm_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mecharm_communication/GetCoords typed call and real-submit client. */
export class Ros1Mecharm_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mecharm_communicationGetCoordsRequest,Ros1Mecharm_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationGetCoordsRequestCodec(),new Ros1Mecharm_communicationGetCoordsResponseCodec(),(value:Ros1Mecharm_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mecharm_communication/GetCoords incoming request/response owner. */
export class Ros1Mecharm_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mecharm_communicationGetCoordsRequest,Ros1Mecharm_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationGetCoordsRequestCodec(),new Ros1Mecharm_communicationGetCoordsResponseCodec(),(value:Ros1Mecharm_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mecharm_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mecharm_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mecharm_communicationGripperStatusRequest,Ros1Mecharm_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationGripperStatusRequestCodec(),new Ros1Mecharm_communicationGripperStatusResponseCodec(),(value:Ros1Mecharm_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_communication/GripperStatus incoming request/response owner. */
export class Ros1Mecharm_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mecharm_communicationGripperStatusRequest,Ros1Mecharm_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationGripperStatusRequestCodec(),new Ros1Mecharm_communicationGripperStatusResponseCodec(),(value:Ros1Mecharm_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mecharm_communication/PumpStatus typed call and real-submit client. */
export class Ros1Mecharm_communicationPumpStatusClient extends BoundedServiceClient<Ros1Mecharm_communicationPumpStatusRequest,Ros1Mecharm_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationPumpStatusRequestCodec(),new Ros1Mecharm_communicationPumpStatusResponseCodec(),(value:Ros1Mecharm_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_communication/PumpStatus incoming request/response owner. */
export class Ros1Mecharm_communicationPumpStatusServer extends BoundedServiceServer<Ros1Mecharm_communicationPumpStatusRequest,Ros1Mecharm_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationPumpStatusRequestCodec(),new Ros1Mecharm_communicationPumpStatusResponseCodec(),(value:Ros1Mecharm_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete mecharm_communication/SetAngles typed call and real-submit client. */
export class Ros1Mecharm_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mecharm_communicationSetAnglesRequest,Ros1Mecharm_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationSetAnglesRequestCodec(),new Ros1Mecharm_communicationSetAnglesResponseCodec(),(value:Ros1Mecharm_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_communication/SetAngles incoming request/response owner. */
export class Ros1Mecharm_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mecharm_communicationSetAnglesRequest,Ros1Mecharm_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationSetAnglesRequestCodec(),new Ros1Mecharm_communicationSetAnglesResponseCodec(),(value:Ros1Mecharm_communicationSetAnglesResponse):bool=>value.validValue(),25,capacity);}}

/** Complete mecharm_communication/SetCoords typed call and real-submit client. */
export class Ros1Mecharm_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mecharm_communicationSetCoordsRequest,Ros1Mecharm_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mecharm_communicationSetCoordsRequestCodec(),new Ros1Mecharm_communicationSetCoordsResponseCodec(),(value:Ros1Mecharm_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_communication/SetCoords incoming request/response owner. */
export class Ros1Mecharm_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mecharm_communicationSetCoordsRequest,Ros1Mecharm_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mecharm_communicationSetCoordsRequestCodec(),new Ros1Mecharm_communicationSetCoordsResponseCodec(),(value:Ros1Mecharm_communicationSetCoordsResponse):bool=>value.validValue(),26,capacity);}}

/** Complete myarm_communication/GetAngles typed call and real-submit client. */
export class Ros1Myarm_communicationGetAnglesClient extends BoundedServiceClient<Ros1Myarm_communicationGetAnglesRequest,Ros1Myarm_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationGetAnglesRequestCodec(),new Ros1Myarm_communicationGetAnglesResponseCodec(),(value:Ros1Myarm_communicationGetAnglesRequest):bool=>value.validValue(),28,observerAlias,capacity);}}

/** Complete myarm_communication/GetAngles incoming request/response owner. */
export class Ros1Myarm_communicationGetAnglesServer extends BoundedServiceServer<Ros1Myarm_communicationGetAnglesRequest,Ros1Myarm_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationGetAnglesRequestCodec(),new Ros1Myarm_communicationGetAnglesResponseCodec(),(value:Ros1Myarm_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete myarm_communication/GetCoords typed call and real-submit client. */
export class Ros1Myarm_communicationGetCoordsClient extends BoundedServiceClient<Ros1Myarm_communicationGetCoordsRequest,Ros1Myarm_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationGetCoordsRequestCodec(),new Ros1Myarm_communicationGetCoordsResponseCodec(),(value:Ros1Myarm_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete myarm_communication/GetCoords incoming request/response owner. */
export class Ros1Myarm_communicationGetCoordsServer extends BoundedServiceServer<Ros1Myarm_communicationGetCoordsRequest,Ros1Myarm_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationGetCoordsRequestCodec(),new Ros1Myarm_communicationGetCoordsResponseCodec(),(value:Ros1Myarm_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete myarm_communication/GripperStatus typed call and real-submit client. */
export class Ros1Myarm_communicationGripperStatusClient extends BoundedServiceClient<Ros1Myarm_communicationGripperStatusRequest,Ros1Myarm_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationGripperStatusRequestCodec(),new Ros1Myarm_communicationGripperStatusResponseCodec(),(value:Ros1Myarm_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete myarm_communication/GripperStatus incoming request/response owner. */
export class Ros1Myarm_communicationGripperStatusServer extends BoundedServiceServer<Ros1Myarm_communicationGripperStatusRequest,Ros1Myarm_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationGripperStatusRequestCodec(),new Ros1Myarm_communicationGripperStatusResponseCodec(),(value:Ros1Myarm_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete myarm_communication/PumpStatus typed call and real-submit client. */
export class Ros1Myarm_communicationPumpStatusClient extends BoundedServiceClient<Ros1Myarm_communicationPumpStatusRequest,Ros1Myarm_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationPumpStatusRequestCodec(),new Ros1Myarm_communicationPumpStatusResponseCodec(),(value:Ros1Myarm_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete myarm_communication/PumpStatus incoming request/response owner. */
export class Ros1Myarm_communicationPumpStatusServer extends BoundedServiceServer<Ros1Myarm_communicationPumpStatusRequest,Ros1Myarm_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationPumpStatusRequestCodec(),new Ros1Myarm_communicationPumpStatusResponseCodec(),(value:Ros1Myarm_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete myarm_communication/SetAngles typed call and real-submit client. */
export class Ros1Myarm_communicationSetAnglesClient extends BoundedServiceClient<Ros1Myarm_communicationSetAnglesRequest,Ros1Myarm_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationSetAnglesRequestCodec(),new Ros1Myarm_communicationSetAnglesResponseCodec(),(value:Ros1Myarm_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete myarm_communication/SetAngles incoming request/response owner. */
export class Ros1Myarm_communicationSetAnglesServer extends BoundedServiceServer<Ros1Myarm_communicationSetAnglesRequest,Ros1Myarm_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationSetAnglesRequestCodec(),new Ros1Myarm_communicationSetAnglesResponseCodec(),(value:Ros1Myarm_communicationSetAnglesResponse):bool=>value.validValue(),29,capacity);}}

/** Complete myarm_communication/SetCoords typed call and real-submit client. */
export class Ros1Myarm_communicationSetCoordsClient extends BoundedServiceClient<Ros1Myarm_communicationSetCoordsRequest,Ros1Myarm_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Myarm_communicationSetCoordsRequestCodec(),new Ros1Myarm_communicationSetCoordsResponseCodec(),(value:Ros1Myarm_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete myarm_communication/SetCoords incoming request/response owner. */
export class Ros1Myarm_communicationSetCoordsServer extends BoundedServiceServer<Ros1Myarm_communicationSetCoordsRequest,Ros1Myarm_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Myarm_communicationSetCoordsRequestCodec(),new Ros1Myarm_communicationSetCoordsResponseCodec(),(value:Ros1Myarm_communicationSetCoordsResponse):bool=>value.validValue(),26,capacity);}}

/** Complete mycobot_320_communication/GetAngles typed call and real-submit client. */
export class Ros1Mycobot_320_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mycobot_320_communicationGetAnglesRequest,Ros1Mycobot_320_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationGetAnglesRequestCodec(),new Ros1Mycobot_320_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_320_communicationGetAnglesRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_320_communication/GetAngles incoming request/response owner. */
export class Ros1Mycobot_320_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mycobot_320_communicationGetAnglesRequest,Ros1Mycobot_320_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationGetAnglesRequestCodec(),new Ros1Mycobot_320_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_320_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_320_communication/GetCoords typed call and real-submit client. */
export class Ros1Mycobot_320_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mycobot_320_communicationGetCoordsRequest,Ros1Mycobot_320_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationGetCoordsRequestCodec(),new Ros1Mycobot_320_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_320_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_320_communication/GetCoords incoming request/response owner. */
export class Ros1Mycobot_320_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mycobot_320_communicationGetCoordsRequest,Ros1Mycobot_320_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationGetCoordsRequestCodec(),new Ros1Mycobot_320_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_320_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_320_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mycobot_320_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mycobot_320_communicationGripperStatusRequest,Ros1Mycobot_320_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationGripperStatusRequestCodec(),new Ros1Mycobot_320_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_320_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_320_communication/GripperStatus incoming request/response owner. */
export class Ros1Mycobot_320_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mycobot_320_communicationGripperStatusRequest,Ros1Mycobot_320_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationGripperStatusRequestCodec(),new Ros1Mycobot_320_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_320_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_320_communication/PumpStatus typed call and real-submit client. */
export class Ros1Mycobot_320_communicationPumpStatusClient extends BoundedServiceClient<Ros1Mycobot_320_communicationPumpStatusRequest,Ros1Mycobot_320_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationPumpStatusRequestCodec(),new Ros1Mycobot_320_communicationPumpStatusResponseCodec(),(value:Ros1Mycobot_320_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_320_communication/PumpStatus incoming request/response owner. */
export class Ros1Mycobot_320_communicationPumpStatusServer extends BoundedServiceServer<Ros1Mycobot_320_communicationPumpStatusRequest,Ros1Mycobot_320_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationPumpStatusRequestCodec(),new Ros1Mycobot_320_communicationPumpStatusResponseCodec(),(value:Ros1Mycobot_320_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete mycobot_320_communication/SetAngles typed call and real-submit client. */
export class Ros1Mycobot_320_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mycobot_320_communicationSetAnglesRequest,Ros1Mycobot_320_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationSetAnglesRequestCodec(),new Ros1Mycobot_320_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_320_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_320_communication/SetAngles incoming request/response owner. */
export class Ros1Mycobot_320_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mycobot_320_communicationSetAnglesRequest,Ros1Mycobot_320_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationSetAnglesRequestCodec(),new Ros1Mycobot_320_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_320_communicationSetAnglesResponse):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_320_communication/SetCoords typed call and real-submit client. */
export class Ros1Mycobot_320_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mycobot_320_communicationSetCoordsRequest,Ros1Mycobot_320_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_320_communicationSetCoordsRequestCodec(),new Ros1Mycobot_320_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_320_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_320_communication/SetCoords incoming request/response owner. */
export class Ros1Mycobot_320_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mycobot_320_communicationSetCoordsRequest,Ros1Mycobot_320_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_320_communicationSetCoordsRequestCodec(),new Ros1Mycobot_320_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_320_communicationSetCoordsResponse):bool=>value.validValue(),26,capacity);}}

/** Complete mycobot_communication/GetAngles typed call and real-submit client. */
export class Ros1Mycobot_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mycobot_communicationGetAnglesRequest,Ros1Mycobot_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationGetAnglesRequestCodec(),new Ros1Mycobot_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_communicationGetAnglesRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_communication/GetAngles incoming request/response owner. */
export class Ros1Mycobot_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mycobot_communicationGetAnglesRequest,Ros1Mycobot_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationGetAnglesRequestCodec(),new Ros1Mycobot_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_communication/GetCoords typed call and real-submit client. */
export class Ros1Mycobot_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mycobot_communicationGetCoordsRequest,Ros1Mycobot_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationGetCoordsRequestCodec(),new Ros1Mycobot_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_communication/GetCoords incoming request/response owner. */
export class Ros1Mycobot_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mycobot_communicationGetCoordsRequest,Ros1Mycobot_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationGetCoordsRequestCodec(),new Ros1Mycobot_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mycobot_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mycobot_communicationGripperStatusRequest,Ros1Mycobot_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationGripperStatusRequestCodec(),new Ros1Mycobot_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/GripperStatus incoming request/response owner. */
export class Ros1Mycobot_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mycobot_communicationGripperStatusRequest,Ros1Mycobot_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationGripperStatusRequestCodec(),new Ros1Mycobot_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_communication/PumpStatus typed call and real-submit client. */
export class Ros1Mycobot_communicationPumpStatusClient extends BoundedServiceClient<Ros1Mycobot_communicationPumpStatusRequest,Ros1Mycobot_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationPumpStatusRequestCodec(),new Ros1Mycobot_communicationPumpStatusResponseCodec(),(value:Ros1Mycobot_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/PumpStatus incoming request/response owner. */
export class Ros1Mycobot_communicationPumpStatusServer extends BoundedServiceServer<Ros1Mycobot_communicationPumpStatusRequest,Ros1Mycobot_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationPumpStatusRequestCodec(),new Ros1Mycobot_communicationPumpStatusResponseCodec(),(value:Ros1Mycobot_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete mycobot_communication/SetAngles typed call and real-submit client. */
export class Ros1Mycobot_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mycobot_communicationSetAnglesRequest,Ros1Mycobot_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetAnglesRequestCodec(),new Ros1Mycobot_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetAngles incoming request/response owner. */
export class Ros1Mycobot_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mycobot_communicationSetAnglesRequest,Ros1Mycobot_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetAnglesRequestCodec(),new Ros1Mycobot_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_communicationSetAnglesResponse):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_communication/SetCoords typed call and real-submit client. */
export class Ros1Mycobot_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mycobot_communicationSetCoordsRequest,Ros1Mycobot_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetCoordsRequestCodec(),new Ros1Mycobot_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetCoords incoming request/response owner. */
export class Ros1Mycobot_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mycobot_communicationSetCoordsRequest,Ros1Mycobot_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetCoordsRequestCodec(),new Ros1Mycobot_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_communicationSetCoordsResponse):bool=>value.validValue(),26,capacity);}}

/** Complete mycobot_communication/SetEndType typed call and real-submit client. */
export class Ros1Mycobot_communicationSetEndTypeClient extends BoundedServiceClient<Ros1Mycobot_communicationSetEndTypeRequest,Ros1Mycobot_communicationSetEndTypeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetEndTypeRequestCodec(),new Ros1Mycobot_communicationSetEndTypeResponseCodec(),(value:Ros1Mycobot_communicationSetEndTypeRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetEndType incoming request/response owner. */
export class Ros1Mycobot_communicationSetEndTypeServer extends BoundedServiceServer<Ros1Mycobot_communicationSetEndTypeRequest,Ros1Mycobot_communicationSetEndTypeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetEndTypeRequestCodec(),new Ros1Mycobot_communicationSetEndTypeResponseCodec(),(value:Ros1Mycobot_communicationSetEndTypeResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_communication/SetFreshMode typed call and real-submit client. */
export class Ros1Mycobot_communicationSetFreshModeClient extends BoundedServiceClient<Ros1Mycobot_communicationSetFreshModeRequest,Ros1Mycobot_communicationSetFreshModeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetFreshModeRequestCodec(),new Ros1Mycobot_communicationSetFreshModeResponseCodec(),(value:Ros1Mycobot_communicationSetFreshModeRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetFreshMode incoming request/response owner. */
export class Ros1Mycobot_communicationSetFreshModeServer extends BoundedServiceServer<Ros1Mycobot_communicationSetFreshModeRequest,Ros1Mycobot_communicationSetFreshModeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetFreshModeRequestCodec(),new Ros1Mycobot_communicationSetFreshModeResponseCodec(),(value:Ros1Mycobot_communicationSetFreshModeResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_communication/SetToolReference typed call and real-submit client. */
export class Ros1Mycobot_communicationSetToolReferenceClient extends BoundedServiceClient<Ros1Mycobot_communicationSetToolReferenceRequest,Ros1Mycobot_communicationSetToolReferenceResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetToolReferenceRequestCodec(),new Ros1Mycobot_communicationSetToolReferenceResponseCodec(),(value:Ros1Mycobot_communicationSetToolReferenceRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetToolReference incoming request/response owner. */
export class Ros1Mycobot_communicationSetToolReferenceServer extends BoundedServiceServer<Ros1Mycobot_communicationSetToolReferenceRequest,Ros1Mycobot_communicationSetToolReferenceResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetToolReferenceRequestCodec(),new Ros1Mycobot_communicationSetToolReferenceResponseCodec(),(value:Ros1Mycobot_communicationSetToolReferenceResponse):bool=>value.validValue(),24,capacity);}}

/** Complete mycobot_communication/SetVisionMode typed call and real-submit client. */
export class Ros1Mycobot_communicationSetVisionModeClient extends BoundedServiceClient<Ros1Mycobot_communicationSetVisionModeRequest,Ros1Mycobot_communicationSetVisionModeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_communicationSetVisionModeRequestCodec(),new Ros1Mycobot_communicationSetVisionModeResponseCodec(),(value:Ros1Mycobot_communicationSetVisionModeRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_communication/SetVisionMode incoming request/response owner. */
export class Ros1Mycobot_communicationSetVisionModeServer extends BoundedServiceServer<Ros1Mycobot_communicationSetVisionModeRequest,Ros1Mycobot_communicationSetVisionModeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_communicationSetVisionModeRequestCodec(),new Ros1Mycobot_communicationSetVisionModeResponseCodec(),(value:Ros1Mycobot_communicationSetVisionModeResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_pro450_communication/GetAngles typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationGetAnglesRequest,Ros1Mycobot_pro450_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationGetAnglesRequestCodec(),new Ros1Mycobot_pro450_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetAnglesRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/GetAngles incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationGetAnglesRequest,Ros1Mycobot_pro450_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationGetAnglesRequestCodec(),new Ros1Mycobot_pro450_communicationGetAnglesResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_communication/GetCoords typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationGetCoordsRequest,Ros1Mycobot_pro450_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationGetCoordsRequestCodec(),new Ros1Mycobot_pro450_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetCoordsRequest):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/GetCoords incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationGetCoordsRequest,Ros1Mycobot_pro450_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationGetCoordsRequestCodec(),new Ros1Mycobot_pro450_communicationGetCoordsResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_communication/GetGripperValue typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationGetGripperValueClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationGetGripperValueRequest,Ros1Mycobot_pro450_communicationGetGripperValueResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationGetGripperValueRequestCodec(),new Ros1Mycobot_pro450_communicationGetGripperValueResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetGripperValueRequest):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/GetGripperValue incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationGetGripperValueServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationGetGripperValueRequest,Ros1Mycobot_pro450_communicationGetGripperValueResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationGetGripperValueRequestCodec(),new Ros1Mycobot_pro450_communicationGetGripperValueResponseCodec(),(value:Ros1Mycobot_pro450_communicationGetGripperValueResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationGripperStatusRequest,Ros1Mycobot_pro450_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationGripperStatusRequestCodec(),new Ros1Mycobot_pro450_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_pro450_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/GripperStatus incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationGripperStatusRequest,Ros1Mycobot_pro450_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationGripperStatusRequestCodec(),new Ros1Mycobot_pro450_communicationGripperStatusResponseCodec(),(value:Ros1Mycobot_pro450_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_pro450_communication/SetAngles typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationSetAnglesRequest,Ros1Mycobot_pro450_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationSetAnglesRequestCodec(),new Ros1Mycobot_pro450_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/SetAngles incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationSetAnglesRequest,Ros1Mycobot_pro450_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationSetAnglesRequestCodec(),new Ros1Mycobot_pro450_communicationSetAnglesResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetAnglesResponse):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_pro450_communication/SetCoords typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationSetCoordsRequest,Ros1Mycobot_pro450_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationSetCoordsRequestCodec(),new Ros1Mycobot_pro450_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/SetCoords incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationSetCoordsRequest,Ros1Mycobot_pro450_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationSetCoordsRequestCodec(),new Ros1Mycobot_pro450_communicationSetCoordsResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetCoordsResponse):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_pro450_communication/SetFreshMode typed call and real-submit client. */
export class Ros1Mycobot_pro450_communicationSetFreshModeClient extends BoundedServiceClient<Ros1Mycobot_pro450_communicationSetFreshModeRequest,Ros1Mycobot_pro450_communicationSetFreshModeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mycobot_pro450_communicationSetFreshModeRequestCodec(),new Ros1Mycobot_pro450_communicationSetFreshModeResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetFreshModeRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_communication/SetFreshMode incoming request/response owner. */
export class Ros1Mycobot_pro450_communicationSetFreshModeServer extends BoundedServiceServer<Ros1Mycobot_pro450_communicationSetFreshModeRequest,Ros1Mycobot_pro450_communicationSetFreshModeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mycobot_pro450_communicationSetFreshModeRequestCodec(),new Ros1Mycobot_pro450_communicationSetFreshModeResponseCodec(),(value:Ros1Mycobot_pro450_communicationSetFreshModeResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mypalletizer_communication/GetAngles typed call and real-submit client. */
export class Ros1Mypalletizer_communicationGetAnglesClient extends BoundedServiceClient<Ros1Mypalletizer_communicationGetAnglesRequest,Ros1Mypalletizer_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationGetAnglesRequestCodec(),new Ros1Mypalletizer_communicationGetAnglesResponseCodec(),(value:Ros1Mypalletizer_communicationGetAnglesRequest):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete mypalletizer_communication/GetAngles incoming request/response owner. */
export class Ros1Mypalletizer_communicationGetAnglesServer extends BoundedServiceServer<Ros1Mypalletizer_communicationGetAnglesRequest,Ros1Mypalletizer_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationGetAnglesRequestCodec(),new Ros1Mypalletizer_communicationGetAnglesResponseCodec(),(value:Ros1Mypalletizer_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mypalletizer_communication/GetCoords typed call and real-submit client. */
export class Ros1Mypalletizer_communicationGetCoordsClient extends BoundedServiceClient<Ros1Mypalletizer_communicationGetCoordsRequest,Ros1Mypalletizer_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationGetCoordsRequestCodec(),new Ros1Mypalletizer_communicationGetCoordsResponseCodec(),(value:Ros1Mypalletizer_communicationGetCoordsRequest):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete mypalletizer_communication/GetCoords incoming request/response owner. */
export class Ros1Mypalletizer_communicationGetCoordsServer extends BoundedServiceServer<Ros1Mypalletizer_communicationGetCoordsRequest,Ros1Mypalletizer_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationGetCoordsRequestCodec(),new Ros1Mypalletizer_communicationGetCoordsResponseCodec(),(value:Ros1Mypalletizer_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete mypalletizer_communication/GripperStatus typed call and real-submit client. */
export class Ros1Mypalletizer_communicationGripperStatusClient extends BoundedServiceClient<Ros1Mypalletizer_communicationGripperStatusRequest,Ros1Mypalletizer_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationGripperStatusRequestCodec(),new Ros1Mypalletizer_communicationGripperStatusResponseCodec(),(value:Ros1Mypalletizer_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_communication/GripperStatus incoming request/response owner. */
export class Ros1Mypalletizer_communicationGripperStatusServer extends BoundedServiceServer<Ros1Mypalletizer_communicationGripperStatusRequest,Ros1Mypalletizer_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationGripperStatusRequestCodec(),new Ros1Mypalletizer_communicationGripperStatusResponseCodec(),(value:Ros1Mypalletizer_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete mypalletizer_communication/PumpStatus typed call and real-submit client. */
export class Ros1Mypalletizer_communicationPumpStatusClient extends BoundedServiceClient<Ros1Mypalletizer_communicationPumpStatusRequest,Ros1Mypalletizer_communicationPumpStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationPumpStatusRequestCodec(),new Ros1Mypalletizer_communicationPumpStatusResponseCodec(),(value:Ros1Mypalletizer_communicationPumpStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_communication/PumpStatus incoming request/response owner. */
export class Ros1Mypalletizer_communicationPumpStatusServer extends BoundedServiceServer<Ros1Mypalletizer_communicationPumpStatusRequest,Ros1Mypalletizer_communicationPumpStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationPumpStatusRequestCodec(),new Ros1Mypalletizer_communicationPumpStatusResponseCodec(),(value:Ros1Mypalletizer_communicationPumpStatusResponse):bool=>value.validValue(),3,capacity);}}

/** Complete mypalletizer_communication/SetAngles typed call and real-submit client. */
export class Ros1Mypalletizer_communicationSetAnglesClient extends BoundedServiceClient<Ros1Mypalletizer_communicationSetAnglesRequest,Ros1Mypalletizer_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationSetAnglesRequestCodec(),new Ros1Mypalletizer_communicationSetAnglesResponseCodec(),(value:Ros1Mypalletizer_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_communication/SetAngles incoming request/response owner. */
export class Ros1Mypalletizer_communicationSetAnglesServer extends BoundedServiceServer<Ros1Mypalletizer_communicationSetAnglesRequest,Ros1Mypalletizer_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationSetAnglesRequestCodec(),new Ros1Mypalletizer_communicationSetAnglesResponseCodec(),(value:Ros1Mypalletizer_communicationSetAnglesResponse):bool=>value.validValue(),17,capacity);}}

/** Complete mypalletizer_communication/SetCoords typed call and real-submit client. */
export class Ros1Mypalletizer_communicationSetCoordsClient extends BoundedServiceClient<Ros1Mypalletizer_communicationSetCoordsRequest,Ros1Mypalletizer_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Mypalletizer_communicationSetCoordsRequestCodec(),new Ros1Mypalletizer_communicationSetCoordsResponseCodec(),(value:Ros1Mypalletizer_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_communication/SetCoords incoming request/response owner. */
export class Ros1Mypalletizer_communicationSetCoordsServer extends BoundedServiceServer<Ros1Mypalletizer_communicationSetCoordsRequest,Ros1Mypalletizer_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Mypalletizer_communicationSetCoordsRequestCodec(),new Ros1Mypalletizer_communicationSetCoordsResponseCodec(),(value:Ros1Mypalletizer_communicationSetCoordsResponse):bool=>value.validValue(),18,capacity);}}

/** Complete ultraarm_communication/GetAngles typed call and real-submit client. */
export class Ros1Ultraarm_communicationGetAnglesClient extends BoundedServiceClient<Ros1Ultraarm_communicationGetAnglesRequest,Ros1Ultraarm_communicationGetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationGetAnglesRequestCodec(),new Ros1Ultraarm_communicationGetAnglesResponseCodec(),(value:Ros1Ultraarm_communicationGetAnglesRequest):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete ultraarm_communication/GetAngles incoming request/response owner. */
export class Ros1Ultraarm_communicationGetAnglesServer extends BoundedServiceServer<Ros1Ultraarm_communicationGetAnglesRequest,Ros1Ultraarm_communicationGetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationGetAnglesRequestCodec(),new Ros1Ultraarm_communicationGetAnglesResponseCodec(),(value:Ros1Ultraarm_communicationGetAnglesResponse):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_communication/GetCoords typed call and real-submit client. */
export class Ros1Ultraarm_communicationGetCoordsClient extends BoundedServiceClient<Ros1Ultraarm_communicationGetCoordsRequest,Ros1Ultraarm_communicationGetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationGetCoordsRequestCodec(),new Ros1Ultraarm_communicationGetCoordsResponseCodec(),(value:Ros1Ultraarm_communicationGetCoordsRequest):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete ultraarm_communication/GetCoords incoming request/response owner. */
export class Ros1Ultraarm_communicationGetCoordsServer extends BoundedServiceServer<Ros1Ultraarm_communicationGetCoordsRequest,Ros1Ultraarm_communicationGetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationGetCoordsRequestCodec(),new Ros1Ultraarm_communicationGetCoordsResponseCodec(),(value:Ros1Ultraarm_communicationGetCoordsResponse):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_communication/GetGripperValue typed call and real-submit client. */
export class Ros1Ultraarm_communicationGetGripperValueClient extends BoundedServiceClient<Ros1Ultraarm_communicationGetGripperValueRequest,Ros1Ultraarm_communicationGetGripperValueResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationGetGripperValueRequestCodec(),new Ros1Ultraarm_communicationGetGripperValueResponseCodec(),(value:Ros1Ultraarm_communicationGetGripperValueRequest):bool=>value.validValue(),4,observerAlias,capacity);}}

/** Complete ultraarm_communication/GetGripperValue incoming request/response owner. */
export class Ros1Ultraarm_communicationGetGripperValueServer extends BoundedServiceServer<Ros1Ultraarm_communicationGetGripperValueRequest,Ros1Ultraarm_communicationGetGripperValueResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationGetGripperValueRequestCodec(),new Ros1Ultraarm_communicationGetGripperValueResponseCodec(),(value:Ros1Ultraarm_communicationGetGripperValueResponse):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_communication/GripperStatus typed call and real-submit client. */
export class Ros1Ultraarm_communicationGripperStatusClient extends BoundedServiceClient<Ros1Ultraarm_communicationGripperStatusRequest,Ros1Ultraarm_communicationGripperStatusResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationGripperStatusRequestCodec(),new Ros1Ultraarm_communicationGripperStatusResponseCodec(),(value:Ros1Ultraarm_communicationGripperStatusRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_communication/GripperStatus incoming request/response owner. */
export class Ros1Ultraarm_communicationGripperStatusServer extends BoundedServiceServer<Ros1Ultraarm_communicationGripperStatusRequest,Ros1Ultraarm_communicationGripperStatusResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationGripperStatusRequestCodec(),new Ros1Ultraarm_communicationGripperStatusResponseCodec(),(value:Ros1Ultraarm_communicationGripperStatusResponse):bool=>value.validValue(),1,capacity);}}

/** Complete ultraarm_communication/SetAngles typed call and real-submit client. */
export class Ros1Ultraarm_communicationSetAnglesClient extends BoundedServiceClient<Ros1Ultraarm_communicationSetAnglesRequest,Ros1Ultraarm_communicationSetAnglesResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationSetAnglesRequestCodec(),new Ros1Ultraarm_communicationSetAnglesResponseCodec(),(value:Ros1Ultraarm_communicationSetAnglesRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_communication/SetAngles incoming request/response owner. */
export class Ros1Ultraarm_communicationSetAnglesServer extends BoundedServiceServer<Ros1Ultraarm_communicationSetAnglesRequest,Ros1Ultraarm_communicationSetAnglesResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationSetAnglesRequestCodec(),new Ros1Ultraarm_communicationSetAnglesResponseCodec(),(value:Ros1Ultraarm_communicationSetAnglesResponse):bool=>value.validValue(),20,capacity);}}

/** Complete ultraarm_communication/SetCoords typed call and real-submit client. */
export class Ros1Ultraarm_communicationSetCoordsClient extends BoundedServiceClient<Ros1Ultraarm_communicationSetCoordsRequest,Ros1Ultraarm_communicationSetCoordsResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationSetCoordsRequestCodec(),new Ros1Ultraarm_communicationSetCoordsResponseCodec(),(value:Ros1Ultraarm_communicationSetCoordsRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_communication/SetCoords incoming request/response owner. */
export class Ros1Ultraarm_communicationSetCoordsServer extends BoundedServiceServer<Ros1Ultraarm_communicationSetCoordsRequest,Ros1Ultraarm_communicationSetCoordsResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationSetCoordsRequestCodec(),new Ros1Ultraarm_communicationSetCoordsResponseCodec(),(value:Ros1Ultraarm_communicationSetCoordsResponse):bool=>value.validValue(),20,capacity);}}

/** Complete ultraarm_communication/SetFreshMode typed call and real-submit client. */
export class Ros1Ultraarm_communicationSetFreshModeClient extends BoundedServiceClient<Ros1Ultraarm_communicationSetFreshModeRequest,Ros1Ultraarm_communicationSetFreshModeResponse> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,1,alias,new Ros1Ultraarm_communicationSetFreshModeRequestCodec(),new Ros1Ultraarm_communicationSetFreshModeResponseCodec(),(value:Ros1Ultraarm_communicationSetFreshModeRequest):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_communication/SetFreshMode incoming request/response owner. */
export class Ros1Ultraarm_communicationSetFreshModeServer extends BoundedServiceServer<Ros1Ultraarm_communicationSetFreshModeRequest,Ros1Ultraarm_communicationSetFreshModeResponse> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,1,advertise,respond,new Ros1Ultraarm_communicationSetFreshModeRequestCodec(),new Ros1Ultraarm_communicationSetFreshModeResponseCodec(),(value:Ros1Ultraarm_communicationSetFreshModeResponse):bool=>value.validValue(),1,capacity);}}
