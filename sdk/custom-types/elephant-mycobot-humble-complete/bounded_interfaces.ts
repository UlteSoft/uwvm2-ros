/** Generated custom RPC facades. Keep beside bounded_records.ts; select the
 * real ros1/ros2 NativeApi and explicit CLI-provisioned aliases. No factory
 * creates a grant, thread, clock, retry or native callback. A Wasm abort/trap
 * cannot promise cleanup; every normal caller-driven pass releases its lease. */
import {Api,Operation,Status} from '../../assemblyscript/api';
import {Capability,Reply} from '../../assemblyscript/roswasm';
import {Codec,validUnicode} from '../../assemblyscript/codecs';
import {decodeUnifiedEvent,UnifiedEvent,EventKind} from '../../assemblyscript/events';
import {Ros2Mecharm_interfacesSrvGetAngles_Request,Ros2Mecharm_interfacesSrvGetAngles_RequestCodec,Ros2Mecharm_interfacesSrvGetAngles_Response,Ros2Mecharm_interfacesSrvGetAngles_ResponseCodec,Ros2Mecharm_interfacesSrvGetCoords_Request,Ros2Mecharm_interfacesSrvGetCoords_RequestCodec,Ros2Mecharm_interfacesSrvGetCoords_Response,Ros2Mecharm_interfacesSrvGetCoords_ResponseCodec,Ros2Mecharm_interfacesSrvGripperStatus_Request,Ros2Mecharm_interfacesSrvGripperStatus_RequestCodec,Ros2Mecharm_interfacesSrvGripperStatus_Response,Ros2Mecharm_interfacesSrvGripperStatus_ResponseCodec,Ros2Mecharm_interfacesSrvPumpStatus_Request,Ros2Mecharm_interfacesSrvPumpStatus_RequestCodec,Ros2Mecharm_interfacesSrvPumpStatus_Response,Ros2Mecharm_interfacesSrvPumpStatus_ResponseCodec,Ros2Mecharm_interfacesSrvSetAngles_Request,Ros2Mecharm_interfacesSrvSetAngles_RequestCodec,Ros2Mecharm_interfacesSrvSetAngles_Response,Ros2Mecharm_interfacesSrvSetAngles_ResponseCodec,Ros2Mecharm_interfacesSrvSetCoords_Request,Ros2Mecharm_interfacesSrvSetCoords_RequestCodec,Ros2Mecharm_interfacesSrvSetCoords_Response,Ros2Mecharm_interfacesSrvSetCoords_ResponseCodec,Ros2Mycobot_interfacesSrvGetAngles_Request,Ros2Mycobot_interfacesSrvGetAngles_RequestCodec,Ros2Mycobot_interfacesSrvGetAngles_Response,Ros2Mycobot_interfacesSrvGetAngles_ResponseCodec,Ros2Mycobot_interfacesSrvGetCoords_Request,Ros2Mycobot_interfacesSrvGetCoords_RequestCodec,Ros2Mycobot_interfacesSrvGetCoords_Response,Ros2Mycobot_interfacesSrvGetCoords_ResponseCodec,Ros2Mycobot_interfacesSrvGripperStatus_Request,Ros2Mycobot_interfacesSrvGripperStatus_RequestCodec,Ros2Mycobot_interfacesSrvGripperStatus_Response,Ros2Mycobot_interfacesSrvGripperStatus_ResponseCodec,Ros2Mycobot_interfacesSrvPumpStatus_Request,Ros2Mycobot_interfacesSrvPumpStatus_RequestCodec,Ros2Mycobot_interfacesSrvPumpStatus_Response,Ros2Mycobot_interfacesSrvPumpStatus_ResponseCodec,Ros2Mycobot_interfacesSrvSetAngles_Request,Ros2Mycobot_interfacesSrvSetAngles_RequestCodec,Ros2Mycobot_interfacesSrvSetAngles_Response,Ros2Mycobot_interfacesSrvSetAngles_ResponseCodec,Ros2Mycobot_interfacesSrvSetCoords_Request,Ros2Mycobot_interfacesSrvSetCoords_RequestCodec,Ros2Mycobot_interfacesSrvSetCoords_Response,Ros2Mycobot_interfacesSrvSetCoords_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvGetAngles_Request,Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCodec,Ros2Mycobot_pro450_interfacesSrvGetAngles_Response,Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvGetCoords_Request,Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCodec,Ros2Mycobot_pro450_interfacesSrvGetCoords_Response,Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCodec,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request,Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCodec,Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response,Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvSetAngles_Request,Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCodec,Ros2Mycobot_pro450_interfacesSrvSetAngles_Response,Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCodec,Ros2Mycobot_pro450_interfacesSrvSetCoords_Request,Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCodec,Ros2Mycobot_pro450_interfacesSrvSetCoords_Response,Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCodec,Ros2Mypalletizer_interfacesSrvGetAngles_Request,Ros2Mypalletizer_interfacesSrvGetAngles_RequestCodec,Ros2Mypalletizer_interfacesSrvGetAngles_Response,Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCodec,Ros2Mypalletizer_interfacesSrvGetCoords_Request,Ros2Mypalletizer_interfacesSrvGetCoords_RequestCodec,Ros2Mypalletizer_interfacesSrvGetCoords_Response,Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCodec,Ros2Mypalletizer_interfacesSrvGripperStatus_Request,Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCodec,Ros2Mypalletizer_interfacesSrvGripperStatus_Response,Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCodec,Ros2Mypalletizer_interfacesSrvPumpStatus_Request,Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCodec,Ros2Mypalletizer_interfacesSrvPumpStatus_Response,Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCodec,Ros2Mypalletizer_interfacesSrvSetAngles_Request,Ros2Mypalletizer_interfacesSrvSetAngles_RequestCodec,Ros2Mypalletizer_interfacesSrvSetAngles_Response,Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCodec,Ros2Mypalletizer_interfacesSrvSetCoords_Request,Ros2Mypalletizer_interfacesSrvSetCoords_RequestCodec,Ros2Mypalletizer_interfacesSrvSetCoords_Response,Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvGetAngles_Request,Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCodec,Ros2Ultraarm_p1_interfacesSrvGetAngles_Response,Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvGetCoords_Request,Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCodec,Ros2Ultraarm_p1_interfacesSrvGetCoords_Response,Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCodec,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request,Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCodec,Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response,Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvSetAngles_Request,Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCodec,Ros2Ultraarm_p1_interfacesSrvSetAngles_Response,Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCodec,Ros2Ultraarm_p1_interfacesSrvSetCoords_Request,Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCodec,Ros2Ultraarm_p1_interfacesSrvSetCoords_Response,Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCodec} from "./bounded_records";

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

/** Complete mecharm_interfaces/srv/GetAngles typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvGetAnglesClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvGetAngles_Request,Ros2Mecharm_interfacesSrvGetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvGetAngles_RequestCodec(),new Ros2Mecharm_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGetAngles_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/GetAngles incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvGetAnglesServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvGetAngles_Request,Ros2Mecharm_interfacesSrvGetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvGetAngles_RequestCodec(),new Ros2Mecharm_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGetAngles_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mecharm_interfaces/srv/GetCoords typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvGetCoordsClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvGetCoords_Request,Ros2Mecharm_interfacesSrvGetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvGetCoords_RequestCodec(),new Ros2Mecharm_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGetCoords_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/GetCoords incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvGetCoordsServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvGetCoords_Request,Ros2Mecharm_interfacesSrvGetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvGetCoords_RequestCodec(),new Ros2Mecharm_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGetCoords_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mecharm_interfaces/srv/GripperStatus typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvGripperStatusClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvGripperStatus_Request,Ros2Mecharm_interfacesSrvGripperStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mecharm_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGripperStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/GripperStatus incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvGripperStatusServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvGripperStatus_Request,Ros2Mecharm_interfacesSrvGripperStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mecharm_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvGripperStatus_Response):bool=>value.validValue(),1,capacity);}}

/** Complete mecharm_interfaces/srv/PumpStatus typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvPumpStatusClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvPumpStatus_Request,Ros2Mecharm_interfacesSrvPumpStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mecharm_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvPumpStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/PumpStatus incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvPumpStatusServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvPumpStatus_Request,Ros2Mecharm_interfacesSrvPumpStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mecharm_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvPumpStatus_Response):bool=>value.validValue(),3,capacity);}}

/** Complete mecharm_interfaces/srv/SetAngles typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvSetAnglesClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvSetAngles_Request,Ros2Mecharm_interfacesSrvSetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvSetAngles_RequestCodec(),new Ros2Mecharm_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvSetAngles_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/SetAngles incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvSetAnglesServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvSetAngles_Request,Ros2Mecharm_interfacesSrvSetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvSetAngles_RequestCodec(),new Ros2Mecharm_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvSetAngles_Response):bool=>value.validValue(),25,capacity);}}

/** Complete mecharm_interfaces/srv/SetCoords typed call and real-submit client. */
export class Ros2Mecharm_interfacesSrvSetCoordsClient extends BoundedServiceClient<Ros2Mecharm_interfacesSrvSetCoords_Request,Ros2Mecharm_interfacesSrvSetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mecharm_interfacesSrvSetCoords_RequestCodec(),new Ros2Mecharm_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvSetCoords_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mecharm_interfaces/srv/SetCoords incoming request/response owner. */
export class Ros2Mecharm_interfacesSrvSetCoordsServer extends BoundedServiceServer<Ros2Mecharm_interfacesSrvSetCoords_Request,Ros2Mecharm_interfacesSrvSetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mecharm_interfacesSrvSetCoords_RequestCodec(),new Ros2Mecharm_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mecharm_interfacesSrvSetCoords_Response):bool=>value.validValue(),26,capacity);}}

/** Complete mycobot_interfaces/srv/GetAngles typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvGetAnglesClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvGetAngles_Request,Ros2Mycobot_interfacesSrvGetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvGetAngles_RequestCodec(),new Ros2Mycobot_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGetAngles_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/GetAngles incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvGetAnglesServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvGetAngles_Request,Ros2Mycobot_interfacesSrvGetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvGetAngles_RequestCodec(),new Ros2Mycobot_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGetAngles_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_interfaces/srv/GetCoords typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvGetCoordsClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvGetCoords_Request,Ros2Mycobot_interfacesSrvGetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvGetCoords_RequestCodec(),new Ros2Mycobot_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGetCoords_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/GetCoords incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvGetCoordsServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvGetCoords_Request,Ros2Mycobot_interfacesSrvGetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvGetCoords_RequestCodec(),new Ros2Mycobot_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGetCoords_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_interfaces/srv/GripperStatus typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvGripperStatusClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvGripperStatus_Request,Ros2Mycobot_interfacesSrvGripperStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mycobot_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGripperStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/GripperStatus incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvGripperStatusServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvGripperStatus_Request,Ros2Mycobot_interfacesSrvGripperStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mycobot_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvGripperStatus_Response):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_interfaces/srv/PumpStatus typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvPumpStatusClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvPumpStatus_Request,Ros2Mycobot_interfacesSrvPumpStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mycobot_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvPumpStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/PumpStatus incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvPumpStatusServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvPumpStatus_Request,Ros2Mycobot_interfacesSrvPumpStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mycobot_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvPumpStatus_Response):bool=>value.validValue(),3,capacity);}}

/** Complete mycobot_interfaces/srv/SetAngles typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvSetAnglesClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvSetAngles_Request,Ros2Mycobot_interfacesSrvSetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvSetAngles_RequestCodec(),new Ros2Mycobot_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvSetAngles_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/SetAngles incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvSetAnglesServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvSetAngles_Request,Ros2Mycobot_interfacesSrvSetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvSetAngles_RequestCodec(),new Ros2Mycobot_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvSetAngles_Response):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_interfaces/srv/SetCoords typed call and real-submit client. */
export class Ros2Mycobot_interfacesSrvSetCoordsClient extends BoundedServiceClient<Ros2Mycobot_interfacesSrvSetCoords_Request,Ros2Mycobot_interfacesSrvSetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_interfacesSrvSetCoords_RequestCodec(),new Ros2Mycobot_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvSetCoords_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_interfaces/srv/SetCoords incoming request/response owner. */
export class Ros2Mycobot_interfacesSrvSetCoordsServer extends BoundedServiceServer<Ros2Mycobot_interfacesSrvSetCoords_Request,Ros2Mycobot_interfacesSrvSetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_interfacesSrvSetCoords_RequestCodec(),new Ros2Mycobot_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mycobot_interfacesSrvSetCoords_Response):bool=>value.validValue(),26,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetAngles typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvGetAnglesClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request,Ros2Mycobot_pro450_interfacesSrvGetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetAngles incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvGetAnglesServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request,Ros2Mycobot_pro450_interfacesSrvGetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetCoords typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvGetCoordsClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request,Ros2Mycobot_pro450_interfacesSrvGetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Request):bool=>value.validValue(),24,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetCoords incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvGetCoordsServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request,Ros2Mycobot_pro450_interfacesSrvGetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetGripperValue typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GetGripperValue incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValueServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request,Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GripperStatus typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvGripperStatusClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request,Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/GripperStatus incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvGripperStatusServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request,Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response):bool=>value.validValue(),1,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/SetAngles typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvSetAnglesClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request,Ros2Mycobot_pro450_interfacesSrvSetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/SetAngles incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvSetAnglesServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request,Ros2Mycobot_pro450_interfacesSrvSetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Response):bool=>value.validValue(),25,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/SetCoords typed call and real-submit client. */
export class Ros2Mycobot_pro450_interfacesSrvSetCoordsClient extends BoundedServiceClient<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request,Ros2Mycobot_pro450_interfacesSrvSetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mycobot_pro450_interfaces/srv/SetCoords incoming request/response owner. */
export class Ros2Mycobot_pro450_interfacesSrvSetCoordsServer extends BoundedServiceServer<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request,Ros2Mycobot_pro450_interfacesSrvSetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCodec(),new Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Response):bool=>value.validValue(),25,capacity);}}

/** Complete mypalletizer_interfaces/srv/GetAngles typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvGetAnglesClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvGetAngles_Request,Ros2Mypalletizer_interfacesSrvGetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvGetAngles_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGetAngles_Request):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/GetAngles incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvGetAnglesServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvGetAngles_Request,Ros2Mypalletizer_interfacesSrvGetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvGetAngles_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGetAngles_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mypalletizer_interfaces/srv/GetCoords typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvGetCoordsClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvGetCoords_Request,Ros2Mypalletizer_interfacesSrvGetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvGetCoords_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGetCoords_Request):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/GetCoords incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvGetCoordsServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvGetCoords_Request,Ros2Mypalletizer_interfacesSrvGetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvGetCoords_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGetCoords_Response):bool=>value.validValue(),0,capacity);}}

/** Complete mypalletizer_interfaces/srv/GripperStatus typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvGripperStatusClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvGripperStatus_Request,Ros2Mypalletizer_interfacesSrvGripperStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/GripperStatus incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvGripperStatusServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvGripperStatus_Request,Ros2Mypalletizer_interfacesSrvGripperStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCodec(),new Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Response):bool=>value.validValue(),1,capacity);}}

/** Complete mypalletizer_interfaces/srv/PumpStatus typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvPumpStatusClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvPumpStatus_Request,Ros2Mypalletizer_interfacesSrvPumpStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/PumpStatus incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvPumpStatusServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvPumpStatus_Request,Ros2Mypalletizer_interfacesSrvPumpStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCodec(),new Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Response):bool=>value.validValue(),3,capacity);}}

/** Complete mypalletizer_interfaces/srv/SetAngles typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvSetAnglesClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvSetAngles_Request,Ros2Mypalletizer_interfacesSrvSetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvSetAngles_RequestCodec(),new Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvSetAngles_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/SetAngles incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvSetAnglesServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvSetAngles_Request,Ros2Mypalletizer_interfacesSrvSetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvSetAngles_RequestCodec(),new Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvSetAngles_Response):bool=>value.validValue(),17,capacity);}}

/** Complete mypalletizer_interfaces/srv/SetCoords typed call and real-submit client. */
export class Ros2Mypalletizer_interfacesSrvSetCoordsClient extends BoundedServiceClient<Ros2Mypalletizer_interfacesSrvSetCoords_Request,Ros2Mypalletizer_interfacesSrvSetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Mypalletizer_interfacesSrvSetCoords_RequestCodec(),new Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvSetCoords_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete mypalletizer_interfaces/srv/SetCoords incoming request/response owner. */
export class Ros2Mypalletizer_interfacesSrvSetCoordsServer extends BoundedServiceServer<Ros2Mypalletizer_interfacesSrvSetCoords_Request,Ros2Mypalletizer_interfacesSrvSetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Mypalletizer_interfacesSrvSetCoords_RequestCodec(),new Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Mypalletizer_interfacesSrvSetCoords_Response):bool=>value.validValue(),17,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetAngles typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvGetAnglesClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request,Ros2Ultraarm_p1_interfacesSrvGetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Request):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetAngles incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvGetAnglesServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request,Ros2Ultraarm_p1_interfacesSrvGetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetCoords typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvGetCoordsClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request,Ros2Ultraarm_p1_interfacesSrvGetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Request):bool=>value.validValue(),16,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetCoords incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvGetCoordsServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request,Ros2Ultraarm_p1_interfacesSrvGetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetGripperValue typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValueClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GetGripperValue incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValueServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request,Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response):bool=>value.validValue(),0,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GripperStatus typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvGripperStatusClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request,Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/GripperStatus incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvGripperStatusServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request,Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response):bool=>value.validValue(),1,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/SetAngles typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvSetAnglesClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request,Ros2Ultraarm_p1_interfacesSrvSetAngles_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/SetAngles incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvSetAnglesServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request,Ros2Ultraarm_p1_interfacesSrvSetAngles_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Response):bool=>value.validValue(),20,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/SetCoords typed call and real-submit client. */
export class Ros2Ultraarm_p1_interfacesSrvSetCoordsClient extends BoundedServiceClient<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request,Ros2Ultraarm_p1_interfacesSrvSetCoords_Response> {constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){super(api,2,alias,new Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Request):bool=>value.validValue(),1,observerAlias,capacity);}}

/** Complete ultraarm_p1_interfaces/srv/SetCoords incoming request/response owner. */
export class Ros2Ultraarm_p1_interfacesSrvSetCoordsServer extends BoundedServiceServer<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request,Ros2Ultraarm_p1_interfacesSrvSetCoords_Response> {constructor(api:Api,advertise:string,respond:string,capacity:i32=8){super(api,2,advertise,respond,new Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCodec(),new Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCodec(),(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Response):bool=>value.validValue(),20,capacity);}}
